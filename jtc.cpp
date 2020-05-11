
/*
 * Created by Dmitry Lyssenko, 2018, ldn.softdev@gmail.com
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <set>
#include <climits>              // LONG_MAX
#include <algorithm>
#include <regex>
#include <functional>
#include <unistd.h>             // STDOUT_FILENO - for term width, sysconf
#include "lib/getoptions.hpp"
#include "lib/Json.hpp"
#include "lib/Streamstr.hpp"
#include "lib/shell.hpp"
#include "lib/dbg.hpp"
#include "lib/ThreadMaster.hpp"
#include "lib/signals.hpp"

using namespace std;

#define PRGNAME "JSON transformational chains"
#define VERSION "1.76a"
#define CREATOR "Dmitry Lyssenko"
#define EMAIL "ldn.softdev@gmail.com"

// jtc: fast and powerful utility to manipulate json format
//
// jtc main features:
//   o walk-path: a set of lexemes (subscripts, searches, directives) which define how
//     jtc should walk the JSON tree (+ supported REGEX)
//   o cache: all of the subscripts and search lexemes cached when walked: cache facilitates
//     super fast recursive walking even huge JSON trees
//   o namespaces: let "memorizing" and reusing memorized JSON values later for interpolation
//   o templating: let produce custom json out of static and previously memorized values
//     in the namespace, allows jsonizing stringified JSONs and stringify JSONs
//   o provides modifications (including in-place) of source json via update/insert/purge/swap
//     operations; the first two optionally could undergo also a shell evaluation
//   o buffered and streamed inputs: the former provides a fast read and parsing, the latter
//     facilitates on-fly json manipulation
//   o employs multithreaded reading and parsing when multiple files given
//   o chains multiple json operations via '/' separator - that replaces shell piping '|' and
//     spares cpu cycles required for json outputting and reading/parsing in the latter case
//  and many more others
//

// facilitate option materialization
// option definitions
#define OPT_RDT -
#define OPT_ALL a
#define OPT_CMP c
#define OPT_DBG d
#define OPT_EXE e
#define OPT_FRC f
#define OPT_GDE g
#define OPT_INS i
#define OPT_JAL J
#define OPT_JSN j
#define OPT_LBL l
#define OPT_MDF m
#define OPT_SEQ n
#define OPT_PRG p
#define OPT_QUT q
#define OPT_RAW r
#define OPT_SWP s
#define OPT_TMP T
#define OPT_IND t
#define OPT_UPD u
#define OPT_WLK w
#define OPT_CMN x
#define OPT_PRT y
#define OPT_SZE z

#include "lib/jtc_guide.hpp"    // don't move this line - above definitions are used in the file

#define XSTR(X) #X
#define STR(X) XSTR(X)
#define XCHR(X) *#X
#define CHR(X) XCHR(X)

#define CHR_NULL '\0'
#define CHR_RSLD '\\'
#define CHR_NLNE '\n'
#define CHR_CRTN '\r'
#define CHR_SQTE '\''
#define CHR_DQTE '"'
#define CHR_SPCE ' '
#define CHR_JILL '\x16'                                         // JSON illegal, various purposes
#define STR_JILL "\x16"                                         // JSON illegal, various purposes
#define STR_DCMP "/"                                            // delimiter for option sets
#define STR_NLNE "\n"
#define STR_DQTE "\""

#define IND_SFX c
#define DBG_APFX "_"                                            // alternative debug prefix
#define CMP_BASE "json_1"
#define CMP_COMP "json_2"
#define JTCS_TKN "size"
#define USE_HPFX -1                     // gates tracking last walked value for {$?} and -x0/-1
#define WLK_HPFX "$?"                   // token for $? interpolation, namespace to reset to dflt
#define WLK_RSTH "#\x16"                // used as a hidden namespace flag to RESET value $? to ""
#define FILE_NSP "$file"
#define SHELLSFX ";printf \"\\x16$?\\x16\"\n"
#define SHELLRGX "([^\x16]*)\x16(\\d+)\x16"


// various return codes
#define RETURN_CODES \
        RC_OK, \
        RC_UNUSED, \
        RC_WP_INV, \
        RC_SC_MISS, \
        RC_CMP_NEQ, \
        RC_ARG_FAIL, \
        RC_END
ENUM(ReturnCodes, RETURN_CODES)

// return code exception offsets
#define OFF_GETOPT RC_END                                       // offset for Getopt exceptions
#define OFF_JSON (OFF_GETOPT + Getopt::end_of_throw)            // offset for Json exceptions
#define OFF_SHELL (OFF_JSON + Getopt::end_of_throw)             // offset for Json exceptions
#define OFF_REGEX (OFF_SHELL + Jnode::end_of_throw)             // offset for Regex exceptions


#define KEY first                                               // semantic for map's pair
#define VALUE second                                            // instead of first/second
#define OPN first                                               // range semantic
#define CLS second                                              // instead of first/second
#define SIZE_T(N) static_cast<size_t>(N)
#define SGNS_T(N) static_cast<signed_size_t>(N)


// simple macro for to expose class aliases
// usage: REVEAL(jtc, opt, json)
#define __REFX__(A) auto & A = STITCH_2TKNS(__reveal_class__,__LINE__).A();
#define REVEAL(X, ARGS...) \
        auto & STITCH_2TKNS(__reveal_class__, __LINE__) = X; \
        MACRO_TO_ARGS(__REFX__, ARGS)

// generic lambda with
#define GLAMBDA(FUNC) [this](auto&&... arg) { return FUNC(std::forward<decltype(arg)>(arg)...); }


typedef vector<string> v_string;
typedef ptrdiff_t signed_size_t;




// Design notes for handling namespaces so that avoid unnecessary copies:
//
// Json class namespaces:
//   o Json class namespace may refer to remote entries (pointed by ptr) but never updates one,
//     instead when new entry to be recorded, even if remote entry exists, the local entry is
//     created
//   o walked (source) Json's NS caries transient data (for the walk duration) only,
//
// Jtc class holds 2 structures facilitating namespace handling:
//  - Global NS: holds namespace values after all walks (-w) are finished in between options sets
//  - WNS: walked namespace snapshots, holds namespaces (snapshots) resulting after each
//         walk iteration (step)
//
// Initializing sequence:
// Jtc implements an adapter for Json JnEntry class, adding 2 methods:
//   o sync_in: sync entries into caller from the argument NS
//   o sync_out: sync entries from the caller class into the argument NS
//
// - before all walking begins (walk_interleaved):
//   o move all NS entries out of Global MS into the first WNS snapshot (by values)
//
// - during walking:
//   o clear-init Json's NS with *references* from last/latest WNS snapshot
//   o make a walk-step: Json NS will record locally only *new values*, while leaving intact
//                       remote entries referenced
//   o record resulted namespaces into WNS: remote entries reman remote (i.e copy pointers),
//                                          local entries to be moved by values
//
// - after walking (before destroying Jtc):
//   o move all values from the last wns snapshot to Global NS
//
// = that way it's ensured that all newly created JSON values will not be copied unnecessarily
//   (only moved) for the duration of all jtc operations.

class map_jnse: public Json::map_jne {
 // this is an adaptor of an Json::map_jne, extended with sync_in(), sinc_out() calls

 public:

    #define NSOPT \
                NsReferAll, /* reference all ns values - local and remote */\
                NsMove,     /* move local ns values, reference remote entriess */\
                NsUpdate,   /* same as NsMove, but for non-existent (new) entries only*/\
                NsMoveAll   /* move all ns values - local & remote */
    ENUM(NsOpType, NSOPT)

    void                sync_out(Json::map_jne &to, NsOpType nsopt);
    void                sync_in(Json::map_jne &from, NsOpType nsopt);
};

#undef NSOPT



void map_jnse::sync_out(Json::map_jne &to, NsOpType nsopt) {
 // sync all entries from this map to Json namespace `to``
 if(nsopt == NsOpType::NsReferAll) {                            // reference all entries
  for(auto &ns: *this) {                                        // for all entries in this map
   if(ns.KEY.front() < ' ' and ns.KEY.front() > '\0') continue; // sync only valid namespaces
   auto ip = to.emplace(ns.KEY, Json::JnEntry{});               // ip: insertion pair
   ip.first->VALUE.ptr(&ns.VALUE.ref());                        // override existing or refer new
  }
  return;
 }

 // else: either NsMove
 for(auto &ns: *this) {                                         // for all entries in this map
  if(ns.KEY.front() < ' ' and ns.KEY.front() > '\0') continue;  // sync only valid namespaces
  if(ns.VALUE.is_remote() and nsopt == NsOpType::NsMove) {      // refer remote
   auto ip = to.emplace(ns.KEY, Json::JnEntry{});               // ip: insertion pair
   ip.first->VALUE.ptr(&ns.VALUE.ref());                        // override existing or refer new
   continue;
  }
  auto found = to.find(ns.KEY);
  if(found != to.end())                                         // found entry in `to`
   found->VALUE = move(ns.VALUE.ref());                         // overwrite to's entry
  else                                                          // no local entry exists
   to.emplace(ns.KEY, move(ns.VALUE.ref()));                    // emplace new entry
 }
}


void map_jnse::sync_in(Json::map_jne &from, NsOpType nsopt) {
 // sync all entries `from` map according to given handling type `nsopt`
 if(nsopt == NsOpType::NsReferAll) {                            // reference all entries
  for(auto &ns: from) {                                         // for all entries in from
   if(ns.KEY.front() < ' ' and ns.KEY.front() > '\0') continue; // sync only valid namespaces
   auto mp = emplace(ns.KEY, Json::JnEntry{});                  // mp: my pair
   mp.first->VALUE.ptr(&ns.VALUE.ref());                        // override w/o version check
  }
  return;
 }

 // else: either NsMove or NsMoveAll or NsUpdate
 for(auto &ns: from) {                                          // for all entries in from
  if(ns.KEY.front() < ' ' and ns.KEY.front() > '\0') continue;  // sync only valid namespaces
  auto found = find(ns.KEY);
  if(nsopt == NsOpType::NsUpdate and found != end()) continue;  // update only new records
  if(ns.VALUE.is_remote() and nsopt != NsOpType::NsMoveAll) {   // refer remote (NsMove/NsUpdate)
   auto mp = emplace(ns.KEY, Json::JnEntry{});                  // mp: my pair
   mp.first->VALUE.ptr(&ns.VALUE.ref());                        // override existing or refer new
   continue;
  }
  if(found != end())                                            // local entry found
   found->VALUE = move(ns.VALUE.ref());                         // overwrite local entry
  else                                                          // no local entry exists
   emplace(ns.KEY, move(ns.VALUE.ref()));                       // emplace new entry
 }
}





class GETopt: public Getopt {
 // a trivial wrapper for Getopt accommodating user flags for imposition of options
 public:

    struct Multifactor {
        // a trivial class to pair up the walk multiplier and the first index to display:
        // only each factor'th walk will be displayed after offset's
        friend SWAP(Multifactor, factor, offset)

                            Multifactor(void) = default;
                            Multifactor(const Multifactor &) = default;
                            Multifactor(Multifactor && mw) = default;
                            Multifactor(size_t x1, signed_size_t x2):    // emplacement
                             factor{x1}, offset{x2} { }
        Multifactor &       operator=(Multifactor mw) noexcept { swap(*this, mw); return *this; }

        size_t              factor;                             // display every factor'th walk
        signed_size_t       offset;                             // starting from offset (0 based)
    };

    #define IMPFLG \
                Install, \
                Erase
    ENUM(ImposeOpt, IMPFLG)
    #undef IMPFLG

    GETopt &            reset(void)
                         { Getopt::reset(); imp_.clear(); return *this; }
    bool                imposed(char opt) const
                         { return imp_.count(opt); }
    void                impose(char opt, ImposeOpt x = ImposeOpt::Install)
                         { if(x == ImposeOpt::Install) imp_.insert(opt); else imp_.erase(opt); }
    set<char> &         imp(void)
                         { return imp_; }
    auto &              wm(void)
                         { return wm_; }
    char                opt_eval(void) const
                         { return opt_eval_; }
    void                opt_eval(char x)
                         { opt_eval_ = x; }

    bool                process_last(size_t total) {            // process -x/-1, if not yet done
                         // i.e. if -x/-1 (-x0) given but last hasn't been output yet
                         return any_of(wm_.begin(), wm_.end(),
                                       [](auto &x){ return x.factor == 0 and x.offset == -1; })
                                        and
                                none_of(wm_.begin(), wm_.end(),
                                        [&](auto &x) {
                                         return x.factor == 0?
                                                 false:
                                                 (total - 1) % x.factor == x.offset % x.factor;
                                        });
                        };
    bool                process_walk(size_t walk) {             // process walk given -xn/n?
                         return any_of(wm_.begin(), wm_.end(),
                                       [&](auto &x) {
                                        return x.factor == 0?
                                         x.offset == static_cast<signed_size_t>(walk):
                                         x.offset > static_cast<signed_size_t>(walk)?
                                          false:
                                          walk % x.factor == x.offset % x.factor;
                                       } );
                        };

 private:

    set<char>           imp_;                                   // imposition of options
    vector<Multifactor> wm_;                                    // walk factor: facilitate -xN/M
    char                opt_eval_{CHR_NULL};
};





class CommonResource {
 // the class facilitates -J option and global namespace:
 // caters user opt (which is copied to each Jtc instance), input string
 // and json for jsonization of all processed/walked jsons

    struct JsonStore {
     // facilitate JSON storage for concurrent reading/parsing
                            JsonStore(void)
                             { task_complete.lock(); }          // DC
        deque<Json>         json_queue;
        mutex               task_complete;
        bool                await_completion{true};
      Streamstr::Filestatus file_status;
        size_t              err_location;
    };
    typedef deque<JsonStore> dequeJS;

 public:

    int                 rc(void) const { return rc_; }
    void                rc(int rc) { rc_ = rc; }
    GETopt &            opt(signed_size_t x = -1) {             // access to Getopt class instance
                         if(x < 0 or SIZE_T(x) >= vopt_.size()) return *opp_;
                         return vopt_[x];
                        }
    map_jnse &          global_ns(void) {  return gns_; }       // access to global namespaces
    Streamstr &         iss(void) {  return iss_; }             // access to stream class
    Json &              global_json(void) { return gjsn_; }     // access to global JSON (-J)
    size_t              elocation(void) { return elocation_; }
    void                elocation(size_t x) { elocation_ = x; }

    void                decompose_opt(int argc, char *argv[]);
    void                display_opts(std::ostream & out);
    bool                is_decomposed_front(void) { return opp_ == &vopt_.front(); }
    bool                is_decomposed_back(void) { return opp_ == &vopt_.back(); }
    void                init_decomposed(void);
    CommonResource &    decomposed_back(void) {  opp_ = &vopt_.back(); return *this; }
    size_t              total_decomposed(void) { return vopt_.size(); }
    size_t              decomposed_idx(void) {
                         for(size_t i = 0; i < vopt_.size(); ++i)
                          if(opp_ == &vopt_[i]) return i;
                         return -1;
                        }
    void                next_decomposed(void) {
                         for(size_t i = 0; i < vopt_.size(); ++i)
                          if(opp_ == &vopt_[i])
                           { opp_ = &vopt_[i == vopt_.size() - 1? 0: i + 1]; break; }
                        }
    char                parse_opt(v_string & args);
    void                enable_global_output(void);
    void                disable_global_output(void);
    void                init_inputs(void);
    void                jsonize(Json jout);
    auto &              wm(void) { return wm_; }

    ThreadMaster &      tm(void) { return tm_; }
    void                decide_on_multithreaded_parsing(void);
    dequeJS &           json_store(void) { return jsd_; };
    size_t              jsq_idx(void) const { return jsq_idx_; };
    void                jsq_idx(size_t x) { jsq_idx_ = x; };

 private:
    bool                is_recompile_required_(const v_string & args);
    char                recompile_args_(v_string &args);
    void                parse_arguments_(const v_string & new_args);
    void                convert_xyw_(void);
    bool                is_x_factor_(const char *str, signed_size_t * recursive = nullptr);

    void                fetch_dispatcher_(void);
    void                read_and_parse_json_(const string & fn, JsonStore & js);

    Json                gjsn_{ ARY{} };                         // global json (facilitates -J)
    map_jnse            gns_;                                   // global namespaces
    bool                read_from_cin_{false};                  // read from <cin>? or files
    Streamstr           iss_;                                   // input Streamstr
    GETopt              opt_;                                   // user options template
    GETopt *            opp_{&opt_};                            // pointer to a current Getopt
    vector<GETopt>      vopt_;                                  // decomposed user options
    vector<GETopt::Multifactor>
                        wm_;                                    // walk factor: facilitate -xN/M
    int                 rc_{RC_OK};
    size_t              elocation_{SIZE_T(-1)};                 // exception location

    ThreadMaster        tm_;
    dequeJS             jsd_;                                   // JsonStore dequeue
    size_t              jsq_idx_{0};

 public:
    DEBUGGABLE(iss_, gjsn_, tm_)
};





class Jtc {

    struct BoundJit {
     // a trivial class to pair up destination walks (-w) with source walks (-i/u/c)
     // and respective name-spaces
        friend SWAP(BoundJit, dst, src, ns, lbl)
                            BoundJit(void) = default;
                            BoundJit(const BoundJit &) = default;
                            BoundJit(BoundJit && bj) = default;
                            BoundJit(Json::iterator &d, Json::iterator &s):   // for emplacement
                             dst{d}, src{s}, ns{src.json().ns()}, lbl{STR_JILL} {}
                            BoundJit(Json::iterator &d, Json::iterator &s, Json::map_jne &n):
                             dst{d}, src{s}, ns{n}, lbl{STR_JILL} {}
        BoundJit &          operator=(BoundJit bj) noexcept
                             { swap(*this, bj); return *this; }

        bool                update_lbl(void)
                             { return lbl.empty() or lbl.front() != CHR_JILL; }
        void                reset_lbl(void) { lbl = STR_JILL; }

        Json::iterator      dst;                                // -w walks (iterators) go here
        Json::iterator      src;                                // -i/u/c walks (iterators) go here
        Json::map_jne       ns;                                 // NS from respective -i/u/c iters.
        string              lbl{STR_JILL};                      // facilitates -u for label update
                                                                // invalid lbl initially
    };


    struct Grouping {
     // facilitates a pair of group/counter values, used in jsonized_output_ary_
        friend SWAP(Grouping, group, counter)
                            Grouping(void) = default;
                            Grouping(const Grouping &) = default;
                            Grouping(Grouping && gr) = default;
                            Grouping(size_t x1, size_t x2):     // emplacement
                             group{x1}, counter{x2} { }
        Grouping &          operator=(Grouping gr) noexcept { swap(*this, gr); return *this; }

        bool                operator>(const Grouping &rhs)
                             { return group > rhs.group and counter > rhs.counter; }

        size_t              group{0};
        size_t              counter{0};
    };


    struct ShellReturn {
     // keeping return result from running shell commands: string and return codes values
            string          str;
            int             rc;
    };


    typedef vector<Json::iterator> vec_jit;
    typedef vector<BoundJit> vec_bjit;
    typedef deque<Json::iterator> deq_jit;
    typedef void (wlk_subscr)(Json::iterator &wi, Grouping grp);
    typedef map<size_t, Json> map_json;

    #define MERGEOBJ \
                Preserve,   /* clashing labels of merged object not overwritten */ \
                Overwrite   /* clashing labels of merged objects overwritten */
    ENUM(MergeObj, MERGEOBJ)

    #define JSONZIZE \
                Jsonize,    /* parameter for write_json() */\
                Dont_jsonize
    ENUM(Jsonizaion, JSONZIZE)

    #define JITSRC          /* type of source for iterations in -i/u/c operation */\
                Src_input,  /* source is input JSON - jinp_: -u<walk> walks input JSON here */\
                Src_mixed,  /* source is in jsrc_[0]: -u<JSON> -u<walk> -u... */\
                Src_optarg  /* all sources are in jsrc_: -u<JSON> -u<FILE> -u<TMP> */
    ENUMSTR(Jitsrc, JITSRC)

    #define ECLI \
                No_exec, \
                Per_walk_exec, \
                Bulk_exec
    ENUMSTR(Ecli, ECLI)

 public:
                        Jtc(void) = delete;
                        Jtc(CommonResource & cr): cr_{cr} {
                         ecli_ = opt()[CHR(OPT_EXE)].hits() == 0?
                                  Ecli::No_exec: opt()[CHR(OPT_EXE)].hits() == 1?
                                                  Ecli::Bulk_exec: Ecli::Per_walk_exec;
                         merge_ = opt()[CHR(OPT_MDF)].hits() % 2 == 1;// flag used by -i/-u options
                         is_tpw_ = opt()[CHR(OPT_TMP)].hits() > 1 and   // is tmp per walk?
                                   opt()[CHR(OPT_SEQ)].hits() < 2 and   // no '-nn' given
                                   opt()[CHR(OPT_TMP)].hits() == opt()[CHR(OPT_WLK)].hits();
                         // ready jinp_
                         jinp_.tab(opt()[CHR(OPT_IND)].hits() > 0 or                // -t given, or
                                   opt()[CHR(OPT_RAW)].hits() == 0?                 // no -r given
                                    abs(opt()[CHR(OPT_IND)].str() == STR(IND_SFX)?  // -tc?
                                         atoi(opt()[CHR(OPT_IND)].c_str(0)):
                                         static_cast<int>(opt()[CHR(OPT_IND)])): 1)
                              .merge_clashing(opt()[CHR(OPT_MDF)].hits() >= 2)
                              .raw(opt()[CHR(OPT_RAW)].hits() % 2)
                              .semicompact(opt()[CHR(OPT_IND)].str().back() == CHR(IND_SFX))
                              .quote_solidus(opt()[CHR(OPT_QUT)].hits() % 2 == 1);
                         jinp_.callback(GLAMBDA(shell_callback_)).engage_callbacks();
                        }


    // expose private objects
    GETopt &            opt(signed_size_t x = -1) { return cr_.opt(x); }
    auto &              json(void) { return jinp_; }
    auto &              jout(void) { return jout_; }

    // user methods
    bool                parsejson(Streamstr::const_iterator &jsp, Streamstr::const_iterator &pse);
    bool                move_fetched_json(Streamstr::const_iterator &jsp);
    void                write_json(Json & jsn, Jsonizaion allow = Jsonize);
    bool                demux_opt(void);
    void                compare_jsons(void);
    void                compare_bingings(void);
    bool                upsert_json(char op);
    void                collect_itr_bindings(Json::iterator &it, Grouping = {0,0});
    void                update_by_iterator(Json::iterator &it, Grouping = {0,0});
    bool                advance_to_next_src(Json::iterator &it, signed_size_t i = -1);
    void                apply_src_walks(char op);
    void                purge_json(void);
    void                swap_json(void);
    void                walk_json(void);
    void                output_by_iterator(Json::iterator &wi, Grouping = {0, 0});
    void                output_by_iterator(Json::iterator &&wi, Grouping grp = {0, 0})
                         { output_by_iterator(wi, grp); }
    map_jnse &          last_wns_snapshot(void) { return wns_[last_dwi_ptr_]; }
    vec_bjit &          itr_pairs(void) { return psrc_; }

 private:
    void                display_location_(Streamstr::const_iterator & start);
    void                exception_locus_(Streamstr::const_iterator & start);
    void                exception_spot_(Streamstr::const_iterator & start);
    void                ready_params_(char option);
    void                parse_option_arg_(Json &, const string & arg, const string & jfile);
    void                ready_params_walks_(const string & arg, size_t jkey);
    void                read_json_(string &jfile, const string &fname);
    void                maybe_update_lbl_(void);
    void                compare_jsons_(const Jnode &, set<const Jnode*> &,
                                       const Jnode &, set<const Jnode*> &);
    void                merge_jsons_(Json::iterator &dst, Json::iterator src, string *lbl);
    void                merge_into_object_(Jnode &dst, const Jnode &src, MergeObj mode);
    void                merge_into_array_(Jnode &dst, const Jnode &src, MergeObj mode);
    void                update_jsons_(Json::iterator &dst, Json::iterator src, string *lbl);
    vector<ShellReturn> run_bulk_shell_(void);
    bool                execute_cli_(Json &update, Json::iterator &jit,
                                     Json::map_jne &ns, ShellReturn *);
    string              interpolate_shell_str_(Json::iterator &jit, Json::map_jne &ns);
    void                crop_out_(void);
    bool                remove_others_(set<const Jnode*> &ws, Jnode &jn);
    vec_jit             collect_walks_(const string &walk_path);
    void                walk_interleaved_(wlk_subscr Jtc::* subscriber);
    void                process_walk_iterators_(deque<deq_jit> &walk_iterators);
    size_t              build_front_grid_(vector<vector<signed_size_t>> &, const deque<deq_jit> &);
    void                process_offsets_(deque<deq_jit> &, vector<vector<signed_size_t>> &,
                                         size_t, vector<size_t> &);
    void                console_output_(Json::iterator &, Json &jref, Grouping unused);
    void                jsonized_output_(Json::iterator &, Json &jref, Grouping unused);
    void                jsonized_output_obj_(Json::iterator &, Json &jref, Grouping);
    void                jsonized_output_ary_(Json::iterator &, Json &jref, Grouping);
    bool                shell_callback_(const std::string &lxm, Json::iterator &jit) {
                         Json tmp;
                         if(DBG()(0)) DBG().increment(+2, tmp, -2);     // imbue current dbg +3
                         tmp = Json::interpolate(lxm, jit, jit.json().ns(), Json::Dont_parse);
                         sh_.system( tmp.str() );
                         return sh_.rc() == 0;
                        }
    size_t              walk_options_size_(void) const {
                         // return a number of elements except the source itself
                         switch(jsrt_) {
                          case Src_input:                       // all -i<WLK> -i<WLK> ...
                                return ecli_ == Ecli::No_exec? jsrc_.size(): jsrc_.size() - 1;
                          case Src_mixed:                       // mix: -i<JSN> -i<WLK> ...
                                return jsrc_.size() - 1;
                          case Src_optarg:
                                return jsrc_.size();
                         }
                         return 0;                              // covering linux compiler warning
                        }
    size_t              walk_options_start_idx_(void) const
                         // return starting idx for the given type of a source
                         { return jsrt_ == Src_optarg? 0: 1; }

    // private member types:
    CommonResource &    cr_;

    Json                jout_;                                  // json output (-j, -jj)
    Json                jinp_;                                  // input JSON for jtc
    vec_bjit            psrc_;                                  // binding dst walks with src's
    map_json            jtmp_;                                  // for resolving <TMP> in -u/i/c
    map_json            jsrc_;                                  // holding params of -i/u/c
    Json::iterator      jits_;                                  // current source iterator
    Jitsrc              jsrt_{Jitsrc::Src_input};               // source type of jits_
    size_t              jscur_{0};                              // current walk in jsrc_
    map<Json::iterator*, map_jnse>
                        wns_;                                   // namespaces for walked (-w) paths
    Json::iterator *    last_dwi_ptr_{nullptr};
    Jnode               hwlk_{ARY{STR{}}};                      // last walked value (interpolated)
    set<string>         c2a_;                                   // used in jsonized_output_obj_()
    map<size_t, string> tpw_;                                   // tmp per walk, output_by_iterator
    bool                is_tpw_;                                // templates pertain to walks
    bool                is_multi_walk_{false};                  // multiple -w or single iterable?
    bool                convert_req_{false};                    // used in output_by_iterator
    Ecli                ecli_{Ecli::No_exec};                   // -e status for insert/update
    bool                merge_{false};                          // -m status for insert/update
    bool                lbl_update_{false};                     // label update operation detected
    Grouping            last_;                                  // used in output_by_iterator
    size_t              key_{0};                                // general purpose counter
    size_t              upst_key_{0};                           // template idx for -iu round-robin
    size_t              wcnt_{0};                               // counts number of walks
    wlk_subscr Jtc::*   subscriber_;                            // method ptr for output processor
    Shell               sh_;
    long                arg_max_{-1};                           // shell's ARG_MAX

    // output conditions:
    bool                inquote_{opt()[CHR(OPT_RAW)].hits() >= 2};  // facilitate -rr
    bool                unquote_{opt()[CHR(OPT_QUT)].hits() >= 2};  // facilitate -qq
    bool                measure_{opt()[CHR(OPT_SZE)].hits() >= 1};  // facilitate -z
    bool                write_to_file_                              // facilitate -f when file op.
                         {opt(0)[0].hits() > 0 and opt()[CHR(OPT_FRC)].hits() > 0};
    bool                glean_lbls_{opt()[CHR(OPT_LBL)].hits()>=2}; // facilitate -ll
    bool                size_only_{opt()[CHR(OPT_SZE)].hits() > 1}; // facilitate -zz
    bool                use_hpfx_{opt().imposed(USE_HPFX)};         // facilitate '$?' or -x0/-1
    ofstream            fout_                                       // open file handler if needed
                         {write_to_file_? cr_.iss().filename().c_str(): "", Jtc::mod_};
    ostream &           xout_{write_to_file_? fout_: cout};         // demux cout/file outputs

    static ios_base::openmode
                        mod_;                                       // write to file (1st json)
 public:

    DEBUGGABLE(jinp_, jout_, sh_)
};

#undef MERGEOBJ
#undef JSONZIZE
STRINGIFY(Jtc::Jitsrc, JITSRC)
#undef JITSRC


ios_base::openmode  Jtc::mod_{ios_base::out};                   // write to file (first json)


// list of standalone calls
void run_decomposed_optsets(CommonResource &, Streamstr::const_iterator &);
void run_single_optset(CommonResource &, Streamstr::const_iterator &, Json &in, Json &out);
string sh_quote_str(const string &src);





int main(int argc, char *argv[]) {

 Signal sgn;
 CommonResource cr;
 REVEAL(cr, opt)

 opt.prolog("\n" PRGNAME "\nVersion " VERSION \
            ", developed by " CREATOR " (" EMAIL ")\n");
 opt[CHR(OPT_ALL)].desc("process all JSONs from source, or disable multithreading if multiple"
                        " sources given");
 opt[CHR(OPT_CMN)].desc("a common part of a walk-path, prepended to every followed -" STR(OPT_PRT)
                        " option").name("common_wp");
 opt[CHR(OPT_CMP)].desc("compare with JSON (given as JSON/walk/template): display delta between"
                        " given JSONs").name("j|w|t");
 opt[CHR(OPT_DBG)].desc("turn on debugs (multiple calls increase verbosity)");
 opt[CHR(OPT_EXE)].desc("make option parameters for -" STR(OPT_INS) "/-" STR(OPT_UPD)
                        " undergo a shell evaluation; see -" STR(OPT_GDE) " for more info");
 opt[CHR(OPT_FRC)].desc("apply changes into the file argument instead of printing resulting JSON"
                        " to stdout");
 opt[CHR(OPT_GDE)].desc("mini USER-GUIDE: explain walk path syntax, usage notes, some examples");
 opt[CHR(OPT_IND)].desc("indent for pretty printing (suffix `" STR(IND_SFX)
                         "' enforces semi-compact format)").bind("3").name("indent");
 opt[CHR(OPT_INS)].desc("insert JSON (given as JSON/walk/template); see with -" STR(OPT_GDE)
                        " for more").name("j|w|t");
 opt[CHR(OPT_JAL)].desc("wrap all processed JSONs into an array (option -"
                        STR(OPT_ALL) " assumed, buffered read imposed)");
 opt[CHR(OPT_JSN)].desc("wrap all walked elements from one JSON into a JSON array (-"
                        STR(OPT_JSN) STR(OPT_JSN) " wrap into an object)");
 opt[CHR(OPT_LBL)].desc("print labels (if present) for walked JSONs; together with -"
                        STR(OPT_JSN) " wrap into objects");
 opt[CHR(OPT_MDF)].desc("modifier: toggle merging for options -" STR(OPT_INS) ", -" STR(OPT_UPD)
                        ", -" STR(OPT_JSN) STR(OPT_JSN) "; see with -" STR(OPT_GDE)
                        " for more info");
 opt[CHR(OPT_PRG)].desc("purge all walked JSON elements (-" STR(OPT_PRG) STR(OPT_PRG)
                        ": purge all elements except ones walked)");
 opt[CHR(OPT_PRT)].desc("an individual part of a walk-path, prepended by preceding -" STR(OPT_CMN)
                        " option").name("partial_wp");
 opt[CHR(OPT_QUT)].desc("enforce strict quoted solidus parsing"
                        " (-" STR(OPT_QUT) STR(OPT_QUT) ": unquote JSON strings)");
 opt[CHR(OPT_RAW)].desc("print JSON in a raw (compact, one-line) format"
                        " (-" STR(OPT_RAW) STR(OPT_RAW) " stringify resulting JSON)");
 opt[CHR(OPT_SEQ)].desc("do not print/process walks interleaved (i.e. print/process all walks "
                        "sequentially)");
 opt[CHR(OPT_SWP)].desc("swap around JSON elements pointed by pairs of walks");
 opt[CHR(OPT_SZE)].desc("print size (number of nodes in JSON) at the end of output (-"
                        STR(OPT_SZE) STR(OPT_SZE) " print size only)");
 opt[CHR(OPT_TMP)].desc("a template to interpolate and apply upon -" STR(OPT_INS) "/"
                        STR(OPT_UPD) "/" STR(OPT_CMP) " and standalone -"
                        STR(OPT_WLK) " operations").name("template");
 opt[CHR(OPT_UPD)].desc("update with JSON (given as either JSON/walk/template); see with -"
                        STR(OPT_GDE) " for more").name("j|w|t");
 opt[CHR(OPT_WLK)].desc("a standalone walk path (multiple may be given); see with -"
                        STR(OPT_GDE) " for more").name("walkpath");
 opt[0].desc("file(s) to read from, or next set of options chained over '/'").name("args")
       .bind("<stdin>");

 opt.epilog(R"(
this tool provides ability to:
 - parse, validate and display JSON (in a compact, semi-compact and pretty formats)
 - walk about input JSON using various subscript and search criteria (see with -)" STR(OPT_GDE)
 R"( for more)
 - manipulate JSON via purge/insert/copy/merge/update/replace/move/swap/interpolate operations
 - compare JSONs (print diffs)

by default, input JSONs processed via buffered read (first read, then parse); streamed read (i.e.,
parse JSON immediately as data arrive) is engaged when option -)"
STR(OPT_ALL) R"( given and <stdin> input selected
(though -)" STR(OPT_JAL) R"( overrides the streamed read and reverts to buffered)

for walk-path explanation, usage notes and examples run with -)" STR(OPT_GDE) R"(
for a complete user guide visit https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md
)");
 opt.variadic();


 // parse options
 cr.decompose_opt(argc, argv);                                  // from now on opt is dynamic!
                                                                // use cr.opt() instead
 if(cr.opt()[CHR(OPT_DBG)])                                     // caveat: only debug in the first
  sgn.install_all();                                            // decomposition will trigger it

 DEBUGGABLE()
 DBG().use_ostream(cerr)
      .level(cr.opt()[CHR(OPT_DBG)])
      .alt_prefix(DBG_APFX);
 #ifdef BG_mTS
  DBG().stamp_ms(true);
 #endif
 #ifdef BG_uTS
  DBG().stamp_us(true);
 #endif
 #ifdef BG_dTS
  DBG().stamp_delta(true);
 #endif

 if(DBG()(0)) cr.display_opts(DOUT());

 // speedup cout & cin
 ios_base::sync_with_stdio(false);
 cin.tie(nullptr);

 // decide if multithreaded parsing to be engaged
 cr.init_inputs();
 cr.decide_on_multithreaded_parsing();

 // ready to read json (ready stream buffer, produce iterator)
 Streamstr::const_iterator jsp = cr.iss().begin();              // global parse pointer

 // execute as per read options
 try {
  // if streamed cin, then 1st option set is processed differently: each (newly read) json
  // will trigger processing of all subsequent option sets
  do run_decomposed_optsets(cr, jsp); while(jsp.is_streamed() and jsp != cr.iss().end());
 }
 catch(Jnode::stdException & e) {
  DBG(1) DOUT() << "exception raised by: " << e.where() << endl;
  cerr << cr.opt().prog_name() << " jnode exception: " << e.what() << endl;
  cr.rc(e.code() + OFF_JSON);
 }
 catch(Json::stdException & e) {
  DBG(1) DOUT() << "exception raised by: " << e.where() << endl;
  cerr << cr.opt().prog_name() << " json" << (cr.elocation() == SIZE_T(-1)? "":" parsing" )
       << " exception"
       << (cr.elocation() == SIZE_T(-1)? string{""}:
           (" (" + (cr.iss().filename().empty()? cr.opt()[0].str(0):cr.iss().filename())
                 + ":" + to_string(cr.elocation()) + ")"))
       << ": " << e.what() << endl;
  cr.rc(e.code() + OFF_JSON);
 }
 catch(Shell::stdException & e) {
  DBG(1) DOUT() << "exception raised by: " << e.where() << endl;
  cerr << cr.opt().prog_name() << " shell exception: " << e.what() << endl;
  cr.rc(e.code() + OFF_SHELL);
 }
 catch(std::regex_error & e) {
  cerr << "regexp exception: " << e.what() << endl;
  cr.rc(e.code() + OFF_REGEX);
 }

 if(cr.global_json().is_empty()) exit(cr.rc());

 Jtc jtc(cr.decomposed_back());                                 // need for access to write_json()
 for(const char *o = STR(OPT_JAL) STR(OPT_JSN) STR(OPT_QUT) STR(OPT_RAW); *o != CHR_NULL; ++o)
  jtc.opt()[*o].reset();                                        // above options to be ignored
 jtc.write_json(cr.global_json());

 exit(cr.rc());
}



void run_decomposed_optsets(CommonResource &cr, Streamstr::const_iterator &jsp) {
 DEBUGGABLE()
 #include "lib/dbgflow.hpp"
 // run demux_opt for all decomposed options (in vopt_)

 Json itrmi, itrmo;                                               // interim input/output
 if(DBG()(0)) { itrmi.DBG().severity(NDBG); itrmo.DBG().severity(NDBG); }
 cr.global_ns().clear();

 do {   // do all opt sets
  DBG().level(cr.opt()[CHR(OPT_DBG)]);
  DBG(1) DOUT() << "pass for set[" << cr.decomposed_idx() << "]" << endl;
  itrmo = ARY();                                                 // output store

  // in a streamed cin option processing of -J is reduced to -j - because of a difference in
  // the was streamed cin processed vs other modes.
  if(jsp.is_streamed() and cr.opt()[CHR(OPT_JAL)].hits() > 0) {
   cr.disable_global_output();
   cr.opt()[CHR(OPT_JAL)].reset();
   if(cr.opt()[CHR(OPT_JSN)].hits() == 0) cr.opt()[CHR(OPT_JSN)].hit();
   cerr << "notice: in " << ENUMS(Streamstr::Strmod, Streamstr::Strmod::streamed_cin)
        << " mode, behavior of option -" STR(OPT_JAL) " is reduced to -" STR(OPT_JSN) << endl;
  }
  GUARD(cr.opt())

  // the output of demux_opt() could be multiple JSONs (to the console) or single json (to gjsn_),
  // to ensure option sets processing always impose -J (redirect to gjsn_)
  if(not cr.is_decomposed_back()) {                             // output to be redirected to -J
   if(cr.opt()[CHR(OPT_JAL)].hits() == 0)                       // impose one if not given
    { cr.enable_global_output(); cr.opt().impose(CHR(OPT_JAL)); }  // and indicate -J was imposed
   else                                                         // -J given by user
    itrmo.push_back(ARY{});                                     // provide single storage
  }

  // process all jsons either from <input>, or from interim (itrmi)
  run_single_optset(cr, jsp, itrmi, itrmo);

  itrmi = move(itrmo);
  cr.next_decomposed();                                         // point opt() to the next set
  DBG(1) DOUT() << "moving to processing next option set[" << cr.decomposed_idx() << "]" << endl;
 } while(not cr.is_decomposed_front());                         // until wraps back to 1st set
}



void run_single_optset(CommonResource &cr,
                       Streamstr::const_iterator &jsp, Json &itrmi, Json &itrmo) {
 // process json(s) from  a single option set
 do {
  Jtc jtc(cr);
  if(cr.is_decomposed_front()) {                                // in optset[0] - always parse json
   auto pse = jsp;
   if(jtc.parsejson(jsp, pse) == false)                         // if nothing left, or streamed cin
    break;                                                      // move to a next set
  }
  else {                                                        // take jsons from itrmi
   if(itrmi.is_empty()) break;                                  // nothing left, move to next set
   jtc.json().root() = move(itrmi.front());
   itrmi.erase(0).normalize_idx();
  }

  bool maybe_tampered = jtc.demux_opt();                        // run jtc operations

  if(not cr.is_decomposed_back()) {                             // interim decomposed step
   if(maybe_tampered) cr.global_json().normalize_idx();         // sanitize arrays indices
   if(cr.opt().imposed(CHR(OPT_JAL)))                           // -J was imposed
    for(auto &v: cr.global_json().root()) itrmo.push_back(move(v));
   else                                                         // -J given by user
    for(auto &v: cr.global_json().root()) itrmo.front().push_back(move(v));
   cr.global_json() = ARY{};
  }
  cr.global_ns().sync_in(jtc.itr_pairs().empty()? jtc.last_wns_snapshot():
                          jtc.itr_pairs().back().ns, map_jnse::NsOpType::NsMoveAll);
  if(cr.is_decomposed_front() and jsp.is_streamed()) break;     // if streamed cin move to next set
 } while((cr.opt()[CHR(OPT_ALL)].hits() > 0 and not cr.opt().imposed(CHR(OPT_ALL))) or // -a given
         (cr.opt()[CHR(OPT_JAL)].hits() > 0 and not cr.opt().imposed(CHR(OPT_JAL))) or // -J given
         (cr.is_decomposed_front() and not cr.json_store().empty()));          // multiple files
}



//
// CR PUBLIC methods definitions
//

// populate vector of options (vopt_) with each decomposed GETopt
// initially opt() will be pointing to a global opt_ (defined in main),
// after init_decomposed it will point to first GETopt in vopt_
void CommonResource::decompose_opt(int argc, char *argv[]) {
 #include "lib/dbgflow.hpp"
 // decompose options by delimiter '/' and then parse each option separately
 v_string args{argv, argv + argc};
 v_string newargs;

 do {
  newargs.clear();
  char chr_eval = parse_opt(args);                              // returns '\0', or 'i', or 'u'

  if(opt().arguments() and opt()[0].str(1) == STR_DCMP) {       // found '/' delimiter, decompose
   newargs.push_back(move(args[0]));                            // move progname to newargs
   args.clear();
   args.push_back(newargs[0]);                                  // reinstate progname in args

   for(auto & o: opt().ordinal())                               // copy all options to newargs
    if(o.kind() == Option::opt) {
     newargs.push_back(string{o.id() == CHR(OPT_RDT)? "": "-"} + static_cast<char>(o.id()));
     if(o.type() == Option::parametric) newargs.push_back(o.str()); // copy argument e.g. "[]"
    }

   for(int i = 2; i <= opt().arguments(); ++i)                  // copy now all values to args
    args.push_back(opt()[0].str(i));                            // excluding first '/' argument
   if(newargs.size() > 1) {                                     // re-parse if there are any params
    opt().Getopt::reset();                                      // reset opt (w/o touching imposed)
    parse_arguments_(newargs);
   }
  }

  if(newargs.size() != 1) {                                     // record only if newargs is empty
   vopt_.push_back(opt());                                      // or has arguments (b/s progname)
   vopt_.back().opt_eval(chr_eval);
   if(not wm().empty())
    { vopt_.back().wm() = move(wm()); wm().clear(); }
  }
  opt().reset();
 }
 while(not newargs.empty());                                    // i.e. repeat while '/' detected

 if(vopt_.size() > 1) {                                         // copy all standalone args from
  for(auto & val: vopt_.back()[0]) vopt_.front()[0] = val;      // last Getopt to front one
  vopt_.back()[0].reset();                                      // and reset last Getopt's arg.
 }

 init_decomposed();                                             // make opt() point to vopt_.front
}



void CommonResource::init_decomposed(void) {
 // init opp_ point to the front decomposed set,
 // also remove non-transient options single -l, -r, -t, -q, '-', -f from interims sets
 // - bare qualifier '-' allowed only in 1st set and not in the others
 for(auto &opt: vopt_)
  for(const char *o = STR(OPT_RAW) STR(OPT_IND) STR(OPT_QUT)
                       STR(OPT_SZE) STR(OPT_FRC) STR(OPT_RDT); *o != CHR_NULL; ++o) {
   if(&opt == &vopt_.front()) {                                 // handle 1st set
    if(*o == CHR(OPT_RDT)) continue;                            // '-' allowed only in 1st set
    if(*o == CHR(OPT_QUT) and opt[*o].hits() % 2 == 1) continue;// -q allowed only in 1st set
   }

   if(&opt == &vopt_.back())                                    // handle last set
    if((*o == CHR(OPT_RDT) and opt[*o].hits() > 0) or
       (*o == CHR(OPT_QUT) and opt[*o].hits() % 2 == 1)) {
     cerr << "notice: ignoring qualifier '" << *o << "' in a non-initial option set" << endl;
     opt[*o].reset();
     continue;
    }

   if(&opt != &vopt_.back()) {                                  // handle interim sets
    if(opt[*o].hits() > 0)
     cerr << "notice: ignoring non-transient option(s) "
          << (*o == CHR(OPT_RDT)? "":"-") << *o << " in an interim option set" << endl;
    opt[*o].reset();
    continue;
   }
  }

 auto & last_opt = opt(total_decomposed() - 1);
 if(last_opt[CHR(OPT_FRC)].hits() > 0 and opt(0)[0].hits() > 1) {   // disable -f if multiple files
  cerr << "notice: ignoring option -" STR(OPT_FRC) " b/c of multi-file processing" << endl;
  last_opt[CHR(OPT_FRC)].reset();
 }

 opp_ = &vopt_.front();
}



void CommonResource::display_opts(std::ostream & out) {
 // display all options
 for(size_t i = 0; i < vopt_.size(); ++i) {
  auto &opt = vopt_[i];

  out << DBG_PROMPT(0) << "option set[" << i << "]:";
  for(size_t j = 0; j < opt.ordinal().size(); ++j)
   if(opt.ordinal(j).id() < 256)
    out << (opt.ordinal(j).id()==CHR(OPT_RDT)? " ": " -") << (char)opt.ordinal(j).id()
        << (opt.ordinal(j).type() == Option::boolean? string{}: "'" + opt.ordinal(j).str() + "'");
   else
    out << " '" << opt.ordinal(j).c_str() << "'";

  out << " (internally imposed:";
  for(auto &i: opt.imp()) out << " -" << i;
  out << " )" << endl;
 }
}



char CommonResource::parse_opt(v_string & args) {
 // 1. parse options, if option -e detected, rebuild -u/i's arguments and parse with rebuilt args
 // 2. reinstate -w options from -x/-y
 // 3. process options dependencies
 char opt_eval{CHR_NULL};                                       // holds either 'i' or 'u' for -e
 if(is_recompile_required_(args))                               // re-compiling required?
  opt_eval = recompile_args_(args);
 parse_arguments_(args);

 if(opt()[CHR(OPT_GDE)].hits() > 0)
  { GuideJtc::instance().print(); exit(RC_OK); }                // -g, print guide

 convert_xyw_();                                                // -x + -y... = -w ...

 if(opt()[CHR(OPT_JAL)].hits() > 0)                             // -J: insert -j, -a
  enable_global_output();

 for(auto &o: opt()[CHR(OPT_TMP)])                              // check every -T
  if(o.find(string{"{"} + WLK_HPFX + "}" ) != string::npos)     // if {$?} detected
   opt().impose(USE_HPFX);                                      // enable tracking last walk output

 if(any_of(wm().begin(), wm().end(),                            // if any of -x is -x0/-1
    [](GETopt::Multifactor &x){ return x.factor == 0 and x.offset == -1; }))
  opt().impose(USE_HPFX);                                       // enable tracking last walk output
 // detecting and imposing USE_HPFX is mere a performance optimization; for all other cases: when
 // no {$?} or no -x0, -x/-1 given there's no need tracking last walk output - it speeds up a bit
 // as copying last walked (and templated) JSON might be very expensive

 return opt_eval;
}



void CommonResource::enable_global_output(void) {
 #include "lib/dbgflow.hpp"
 // this pattern is used multiple times - inserts -j and -a options
 if(opt()[CHR(OPT_JAL)].hits() == 0) opt()[CHR(OPT_JAL)].hit();
 for(char x: {CHR(OPT_ALL), CHR(OPT_JSN)})
  if(opt()[x].hits() == 0) {                                    // if opt not given,
   opt().impose(x);                                             // indicate that it was imposed
   opt()[x].hit();                                              // ensure the opt is hit
  }
}



void CommonResource::disable_global_output(void) {
 #include "lib/dbgflow.hpp"
 // reinstate -a, -j options as they were before -J was enabled
 opt()[CHR(OPT_JAL)].reset();

 for(char x: {CHR(OPT_ALL), CHR(OPT_JSN)})                      // process options -a, -j
  if(opt().imposed(x)) {                                        // if opt was imposed,
   opt().impose(x, GETopt::ImposeOpt::Erase);                   // remove imposition flag
   opt()[x].reset();                                            // reset the opt
  }
}



void CommonResource::init_inputs(void) {
 #include "lib/dbgflow.hpp"
 // initialize iss_ with correct read mode: streamed/cin/buffered
 read_from_cin_ = opt()[0].hits() == 0 or opt()[CHR(OPT_RDT)].hits() > 0;   // no files, or '-'
 DBG(0)
  DOUT() << "reading json from " << (read_from_cin_? "<stdin>": "file-arguments:") << endl;

 for(int arg = 1; arg <= opt().arguments(); ++arg) {            // register all filenames with iss
  iss_.source_file(opt()[0].str(arg));                          // will set read mode to file
  DBG(0) DOUT() << "file argument: " << iss_.filenames()[arg - 1] << endl;
 }

 if(read_from_cin_) {                                           // <cin> either stream, or buffered
  if(opt()[CHR(OPT_ALL)].hits() and opt()[CHR(OPT_JAL)].hits() == 0)    // -a, no -J, stream read
   iss_.reset(Streamstr::streamed_cin);
  else {                                                        // <cin> buffered (all other cases)
   if(opt()[CHR(OPT_JAL)].hits() > 0 and
      opt().imposed(CHR(OPT_ALL)) == false)                     // -J and -a were given by user
    cerr << "notice: option -" STR(OPT_JAL) " cancels streamed input, reverts to buffered" << endl;
   iss_.reset(Streamstr::buffered_cin);
  }
 }

 if(iss_.filenames().size() > 1)                                // if multiple files given:
  if(opt()[CHR(OPT_ALL)].hits() == 0) {                         // if -a not given, impose one
   opt()[CHR(OPT_ALL)].hit();                                   // ensure -a for multiple files
   opt().impose(CHR(OPT_ALL));                                  // indicate it's imposed
  }
}



void CommonResource::jsonize(Json jout) {
 #include "lib/dbgflow.hpp"
 // put all walked json results into a global json
 if(opt().imposed(CHR(OPT_JSN)) and jout.is_iterable()) {       // -j imposed by -J & not -rr
  for(auto &jn: jout)                                           // therefore push one by one
   global_json().push_back(move(jn));
  return;
 }
                                                                // -j given by user (not imposed)):
 global_json().push_back(move(jout));                           // move jout as it is
}



void CommonResource::decide_on_multithreaded_parsing(void) {
 #include "lib/dbgflow.hpp"
 // if conditions are right enable multi-threaded read/parsing
 if(tm().seats_total() == 1) return;                            // cpu should have more than 1 core
 if(read_from_cin_ == true) return;                             // must be reading from files
 if(opt()[CHR(OPT_ALL)].hits() - opt().imposed(CHR(OPT_ALL)) > 0) return;  // -a disables m.t.
 if(opt()[0].hits() <= 1) return;                               // there must be more than 1 file

 json_store().resize(iss().filenames().size());                 // enable multithreading
 DBG(0) DOUT() << "starting dispatcher for json parsers in a new thread" << endl;
 tm().run(GLAMBDA(fetch_dispatcher_));                          // fetch jsons starting from 2 file
 iss().defer_reading_files();                                   // b/c m-thread reading is engaged
}


//
// CR PRIVATE methods definitions
//
bool CommonResource::is_recompile_required_(const v_string & args) {
 #include "lib/dbgflow.hpp"
 // check if option -e is present in the arguments (if so, indicate re-parsing is required)
 char *nargv[args.size()];                                      // here, build a new argv
 auto alloc_args = [&] {                                        // populate nargv from args
       for(size_t i = 0; i < args.size(); ++i)
        { nargv[i] = new char[args[i].size() + 1];
          strcpy(nargv[i], args[i].c_str()); }
       return true;                                             // facilitate return type for GUARD
      };
 auto free_args = [&](bool unused)
       { for(auto &a: nargv) delete [] a; };

 GUARD(alloc_args, free_args)
 try { opt().parse(args.size(), nargv); }
 catch(GETopt::stdException &e)
  { opt().usage(); exit(e.code() + OFF_GETOPT); }

 bool irr = opt()[CHR(OPT_EXE)];
 if(irr)
  for(auto &o: opt().ordinal()) {
   if(o.id() == CHR(OPT_EXE)) break;
   if(static_cast<char>(o.id()) AMONG(static_cast<char>(CHR(OPT_INS)),CHR(OPT_UPD),CHR(OPT_CMP))) {
    cerr << "fail: option -'" << CHR(OPT_EXE)
         << "' must precede options -" STR(OPT_INS) ", -" STR(OPT_UPD) ", -" STR(OPT_CMP) << endl;
    exit(RC_ARG_FAIL);
   }
 }

 opt().reset();
 return irr;
}



char CommonResource::recompile_args_(v_string & args) {
 // recompile argv minding -u/i's arguments, put re-parsed args into new_args
 // return opt_eval char ('i'/'u', or '\0')
 v_string new_args;
 bool semicolon_found = false;
 char opt_eval{CHR_NULL};

 for(auto &arg: args) {                                         // go through all args
  if(semicolon_found)                                           // -i/u already found and processed
   { new_args.push_back(arg); continue; }                       // push arg w/o any processing

  if(opt_eval) {                                                // facing -i/u; '\;' not found yet
   if(arg == ";")                                               // terminating ';' found
    { semicolon_found = true; arg.pop_back(); }                 // trim trailing ';'
   if(not arg.empty())
    new_args.back() += string{(new_args.back().empty()? "":" ")} + arg;
   continue;
  }

  if(arg.front() == '-')                                        // opt, see if opt -i/u is present
   for(const char &chr: arg) {
    if(&chr == &arg[0]) continue;                               // skip first char '-'
    if(not opt().defined(chr)) break;                           // undefined option, process arg
    if(chr == CHR(OPT_UPD) or chr == CHR(OPT_INS)) {            // opt -i/u found, indicate&record
     opt_eval = chr;
     new_args.push_back(arg.substr(0, &chr - arg.c_str() + 1)); // push options only, e.g.: "-eu"
     arg = arg.substr((&chr) - arg.c_str() + 1);
     break;
    }
   }
  new_args.push_back(arg);
 }

 if(opt_eval > 0 and not semicolon_found) {
  cerr << "fail: don't see parameter termination of '-" << opt_eval << "' - `\\;'" << endl;
  exit(RC_SC_MISS);
 }
 args = move(new_args);
 return opt_eval;
}



void CommonResource::parse_arguments_(const v_string & new_args) {
 #include "lib/dbgflow.hpp"
 // parse rebuilt arguments (after recompilation for -e option)
 char *nargv[new_args.size()];                                  // here, build a new argv
 auto alloc_args = [&](void) {                                  // populate nargv from new_args
                    for(size_t i = 0; i < new_args.size(); ++i)
                     { nargv[i] = new char[new_args[i].size()+1];
                       strcpy(nargv[i], new_args[i].c_str()); }
                    return false;
                   };
 auto free_args = [&](bool unused)
                   { for(size_t i = 0; i < new_args.size(); ++i) delete [] nargv[i]; };

 GUARD(alloc_args, free_args)
 try { opt().parse(new_args.size(), nargv); }                   // re-parse newly rebuilt args
 catch(GETopt::stdException & e)
  { opt().usage(); exit(e.code() + OFF_GETOPT); }
}



void CommonResource::convert_xyw_(void) {
 #include "lib/dbgflow.hpp"
 // convert -x, -y options into -w:
 // standalone (isolated) -x and -y also converted to -w
 string last_x, last_y;
 v_string new_w;                                                // record new -w options here

 for(auto &option: opt().ordinal()) {                           // go by options order
  if(option.id() == CHR(OPT_CMN)) {                             // option -x, process it
   if(is_x_factor_(option.c_str())) continue;
   if(not last_x.empty() and last_y.empty())                    // it's like: -x... -x...
    new_w.push_back(move(last_x));                              // standalone -x is converted to -w
   last_x = move(option.str());
   last_y.clear();
   continue;
  }
  if(option.id() == CHR(OPT_PRT)) {                             // option -y
   last_y = move(option.str());
   new_w.push_back(last_x + last_y);
  }
 }

 if(not last_x.empty() and last_y.empty())                      // option -x... is given alone
  new_w.push_back(move(last_x));

 for(auto &opt_w: new_w)                                        // move all new '-w' to opt
  opt()[CHR(OPT_WLK)] = opt_w;
}



bool CommonResource::is_x_factor_(const char * x_str, signed_size_t * recursive) {
 #include "lib/dbgflow.hpp"
 // process -xN/N option here: notation -x/N is allowed, all other forms return `false`
 signed_size_t x1, x2;
 char *endptr;
 x1 = strtol(x_str, &endptr, 10);

 if(recursive)                                                  // resolving a second value now
  { if(*endptr == CHR_NULL)
   { *recursive = x1; return true; } return false; }

 x2 = abs(x1) - 1;
 if(*endptr == '/' and endptr[1] != CHR_NULL)
  { if(not is_x_factor_(endptr + 1, &x2)) return false; }       // resolve second value here
 else
  if(*endptr != CHR_NULL) return false;

 wm_.emplace_back(abs(x1), x2);
 return true;
}



void CommonResource::fetch_dispatcher_(void) {
 // dispatch threads fetching jsons when multiple files given
 DBG(0) DOUT() << "got " << json_store().size() << " filename(s) to fetch via dispatcher" << endl;
 auto & fnv = iss().filenames();                                // file name vector

 auto rpj = [&](auto&&... arg)
       { read_and_parse_json_(std::forward<decltype(arg)>(arg)...); };

 for(size_t i = 0; i < json_store().size(); ++i)
  tm().run(rpj, ref(fnv[i]), ref(json_store()[i]));
}



void CommonResource::read_and_parse_json_(const string &filename, JsonStore & js) {
 // isolated thread to read and parse all JSONs from file
 auto dummy = [&] { return true; };                             // b/c it comes in already locked
 auto remove_lock = [&](bool  unused) { js.task_complete.unlock(); };
 GUARD(dummy, remove_lock)

 // debugs may interfere with cout outputs (as the latter not mutex'ed), hence commented out
 //DBG(0) DOUT() << "parsing file " << filename << endl;
 Streamstr jstream{filename, Streamstr::Verbosity::Quiet};
 if(DBG()(0)) jstream.DBG().severity(NDBG);
 auto jsp = jstream.begin();
 js.file_status = jstream.filestatuses().front();

 while(jsp != jstream.end()) {
  Json j;
  if(DBG()(0)) j.DBG().severity(NDBG);
  Streamstr::const_iterator jbegin = jsp;                       // for elocation only
  j.parse_throwing(false).parse(jsp, Json::Relaxed_trailing);
  js.json_queue.push_back(move(j));
  if(js.json_queue.back().parsing_failed()) {                   // exception occurred
   js.err_location = Json::utf8_adjusted(0, jstream.str(),
                                         distance(jbegin, js.json_queue.back().exception_point()));
   break;
  }
 }
 //DBG(0) DOUT() << "finished parsing file " << filename
 //              << ", parsed " << js.json_queue.size() << " json(s)" << endl;
}





//
// Jtc PUBLIC methods definitions
//
bool Jtc::parsejson(Streamstr::const_iterator &jsp, Streamstr::const_iterator &pse) {
 #include "lib/dbgflow.hpp"
 // parse read json text via Streamstr. Return true if actual json was parsed / false otherwise

 // if parsing was in other threads, fetch those one by one
 if(not cr_.json_store().empty())                               // concurrency was employed
  return move_fetched_json(jsp);

 if(jsp == cr_.iss().end()) return false;                       // reached end of buffer(s)
 cr_.global_ns()[FILE_NSP] = STR{cr_.iss().filename()};         // update gns with current filename

 Streamstr::const_iterator jbegin = jsp;                        // for debugs / location_ only

 try
  { json().parse(jsp, cr_.iss().is_streamed()? Json::Relaxed_no_trail: Json::Relaxed_trailing); }
 catch(Json::stdException & e) {
  if(e.code() == Jnode::unexpected_end_of_string and jsp.is_streamed()) {
   for(; pse.offset() < jsp.offset(); ++pse) if(*pse > CHR_SPCE) break;
   if(pse.offset() == jsp.offset()) {
    DBG(0) DOUT() << "suppressing exception: blank trail detected" << endl;
    return false;                                               // no json parsing occurred
   }
  }
  if(e.code() > Jnode::start_of_json_parsing_exceptions and
     e.code() < Jnode::end_of_json_parsing_exceptions)
   display_location_(jbegin);
  throw;
 }
 return true;
}



bool Jtc::move_fetched_json(Streamstr::const_iterator &jsp) {
 #include "lib/dbgflow.hpp"
 // set json() to the fetched value in the other threads
 size_t idx = cr_.jsq_idx();                                    // idx for currently processed jsq

 while(idx < cr_.json_store().size()) {
  cr_.global_ns()[FILE_NSP] = STR{cr_.iss().filenames()[idx]};  // update gns with current filename
  auto & jsq = cr_.json_store()[idx];                           // currently used queue from store

  if(jsq.await_completion) {                                    // checking for the first time
   DBG(0) DOUT() << "awaiting completion of another thread parsing" << endl;
   jsq.task_complete.lock();                                    // thread supposed to release mutex
   jsq.await_completion = false;                                // no further sync is required
   if(jsq.file_status == Streamstr::Filestatus::Failure)
    cerr << "error: could not open file '" << cr_.global_ns()[FILE_NSP].str() << "'" << endl;
  }

  if(jsq.json_queue.empty())                                    // if all jsons released
   { cr_.jsq_idx(++idx); continue; }                            // process next parsed json queue

  auto & jsn = jsq.json_queue.front();
  if(jsn.parsing_failed()) {                                    // handle failed parsing
   cr_.elocation(jsq.err_location);
   cr_.iss().reset(Streamstr::buffered_file).source_file(cr_.global_ns()[FILE_NSP].str());
   throw jsn.EXP(jsn.exception_reason());
  }

  json() = move(jsn);                                           // release json to processing
  jsq.json_queue.pop_front();
  if(jsq.json_queue.empty() and idx == cr_.json_store().size() - 1) // if last json was released
   jsp = cr_.iss().end();                                       // then indicate the true end
  return true;
 }
 jsp = cr_.iss().end();                                         // a case when all trailing files
 return false;                                                  // are failed to be open
}



// write_json(src_json, allow_jsonizing) prints only a **single** JSON:
//   o will print size of src_json (if -zz given)
//   o if -j given and jsonizing allowed, will encapsulate src_json into array: [ src_json ]
//   o if -rr given (and no -qq) then stringifies src_json (or [ src_json ] if -j given)
//   o if -J given then src_json (or [ src_json ]) is output to global json (gjsn_) while
//     encapsulated into array and returns:
//     - in such case write_json() will be called from main() once all JSONs are processed
//   o then, if no -J: (i.e. non-empty gjsn_ always indicates that -J was processed) then
//     DEMUX output destination - either to file (-f), or to console
//   o if -qq given (overrides -rr) then jsonsizes (unqoutes) src_json if it's a JSON string
//   o write into demuxed destination (file/console) and append json size at the end (if -z given)
//
// write_json() is used in:
//  - at the end of main (after processing all input jsons) if gjsn_ is non-empty
//  - demux_opt() for those cases which reach the end of the function:
//      o insert (-i),
//      o update (-u),
//      o purge (-p),
//      o swap (-s)
//  - compare_json()
//      o will use write_json() only if -j (imposed by -J or given by user) is present
//      o otherwise result is prited via compare_bingings() -> output_by_iterator()
//  - walk_json()
//      o will use write_json() only if -j (imposed by -J or given by user) is present
//      o otherwise result is prited via output_by_iterator()

void Jtc::write_json(Json & json, Jsonizaion allow_encap) {
 #include "lib/dbgflow.hpp"
 // write whole json to output (demultiplexing file and stdout), featuring:
 // inquoting/unquoting json string, putting json into array (-j), printing size to stdout

 bool global_jsn{opt()[CHR(OPT_JAL)].hits() > 0};               // -J given

 if(allow_encap == Jsonize and opt()[CHR(OPT_JSN)].hits() == 1) // -j given, force jsonizing
  json = ARY{ move(json) };
 if(not unquote_ and inquote_)
  json.root() = json.inquote_str(json.to_string(Jnode::Raw));

 size_t jsize{0};
 if(not global_jsn) {
  if(measure_) jsize = json.size();                             // -z or -zz
  if(size_only_) json.root() = jsize;                           // -zz
 }

 DBG(0)
  DOUT() << "outputting json to "
         << (write_to_file_?
              cr_.iss().filename():
              global_jsn? "<JSON>": opt()[CHR(OPT_JSN)]? "<json>": "<stdout>") << endl;
 if(global_jsn)                                                 // -J, jsonize to global, defer
  { cr_.jsonize(move(json)); return; }                          // output until all JSON processed

 Jtc::mod_ |= ios_base::app;                                    // next time will append

 if(unquote_ and json.is_string())
  { if(not json.str().empty()) xout_ << json.unquote_str(json.str()) << endl; }
 else xout_ << json << endl;

 if(opt()[CHR(OPT_SZE)].hits() == 1)                            // -z
  xout_ << OBJ{LBL{JTCS_TKN, static_cast<double>(jsize)}} << endl;
}



bool Jtc::demux_opt(void) {
 #include "lib/dbgflow.hpp"
 // demultiplex functional options, execute once in order: -[ciuspw]
 // return true/false in case if JSON could be tampered or not
 if(opt()[CHR(OPT_WLK)].hits() == 0 and opt()[CHR(OPT_TMP)].hits() >= 1)  // no -w, but -T is given
  opt()[CHR(OPT_WLK)] = "";                                     // provide default walk off root

 bool is_tampered{false};
 for(char op: STR(OPT_CMP)STR(OPT_INS)STR(OPT_UPD)STR(OPT_SWP)STR(OPT_PRG)STR(OPT_WLK)) {
  if(op == CHR_NULL or opt()[op].hits() == 0) continue;
  DBG(1) DOUT() << "option: '-" << op << "', hits: " << opt()[op].hits() << endl;

  switch(op) {
   case CHR(OPT_CMP): {  compare_jsons(); return false; }       // will print result itself
   case CHR(OPT_WLK): { walk_json(); return false; }            // will print result itself
   case CHR(OPT_INS):
   case CHR(OPT_UPD): is_tampered = upsert_json(op); break;
   case CHR(OPT_PRG): purge_json(); is_tampered =  true; break;
   case CHR(OPT_SWP): swap_json(); break;
   default: continue;
  }
  break;
 }

 // this write will be used only by -[iups]
 write_json(json());
 return is_tampered;
}



void Jtc::compare_jsons() {
 #include "lib/dbgflow.hpp"
 // plug-in compare_by_iterator (Jtc::collect_itr_bindings) and let process walks (print results)
 if(opt()[CHR(OPT_FRC)].hits() > 0 and
    opt()[CHR(OPT_JSN)].hits() == 0)                            // -f w/o -j (imposed or by user)
  cerr << "notice: ignoring -" STR(OPT_FRC)
          " due to multi-json output (use additionally -" STR(OPT_JSN)
          " to save the result in file)" << endl;

 opt().wm().clear();                                            // -xn/n is irrelevant here
 if(opt()[CHR(OPT_WLK)].hits() == 0)                            // no -w?
  opt()[CHR(OPT_WLK)] = "";                                     // then walk from root
 ready_params_(CHR(OPT_CMP));

 walk_interleaved_(&Jtc::collect_itr_bindings);
 compare_bingings();

 if(opt()[CHR(OPT_JSN)].hits() > 0)
  write_json(jout_);
}



void Jtc::compare_bingings(void) {
 #include "lib/dbgflow.hpp"
 // compare JSONs: pairs of compared and comparing iterators (pointers) come from the
 // subscriber collect_itr_bindings() called from walk_interleaved_()
 static string lbl[2] { CMP_BASE, CMP_COMP };
 for(auto &pair: psrc_) {
  vector<Json> jv{2};                                           // 2 JSONs to compare

  jv.front()[lbl[0]] = *pair.dst;                               // 1st comes form walk_interleaved
  jv.back()[lbl[1]] = *pair.src;                                // 2nd comes from -c <walk-path>
  if(DBG()(0)) DBG().increment(+2, jv.front(), jv.back(), -2);  // imbue current dbg +3

  vector<set<const Jnode*>> node_set{2};                        // preserved different node ptrs
  compare_jsons_(jv.front()[lbl[0]], node_set.front(),
                 jv.back() [lbl[1]], node_set.back());

  for(size_t i = 0; i < node_set.size(); ++i)
   DBG(1) DOUT() << "found diffs (" << lbl[i]
                 << ", instance " << key_++ << "): " << node_set[i].size() << endl;

  for(size_t i = 0; i < jv.size(); ++i)
   if(jv[i][ lbl[i] ].is_iterable())                            // if root is iterable
    remove_others_(node_set[i], jv[i][ lbl[i] ]);               // remove then all matching nodes
   else                                                         // root is atomic
    if(node_set[i].empty()) jv[i][ lbl[i] ] = OBJ{};            // set is as an empty set {}

  for(auto &json: jv) {                                         // output compared jsons
   auto jit = json.walk("[0]");
   if(not jit->is_empty()) cr_.rc(RC_CMP_NEQ);
   output_by_iterator(jit);
  }
  if(jv.front().front().type() != jv.back().front().type())
   cr_.rc(RC_CMP_NEQ);
 }
}



// insert handles 4 parameter types (the same applies to updates):
//  1. static insert ( -i <json> )
//  2. insert from file ( -i <file> ) - file must contain a valid JSON
//  3. insert from cli ( -e -i <cli> \; ) - cli must return a valid JSON
//  4. insert by walk-path (-i <src walk-path>) effectively performing copy operation
// Destination walks (-w <dst walk-path>) define mode of insertion:
//  - if insertion point is single (only one walk given and it's non-iterable), then
//    all source jsons/walks will be attempted to be inserted into a single location
//  - if multiple insertions points given (ether multiple -w, or single -w with
//    an iterable walk-path), then sources are inserted in a circular manner
//
// insert matrix (src -> dst):
//                                    insert w/o -m
//   to  \ from  |     [3,4]     |    {"a":3,"c":4}    |      "a":3,"c":4      |     "c"
//---------------+---------------+---------------------+-----------------------+---------------+	
//      [1,2]    |   [1,2,[3,4]  | [1,2,{"a":3,"c":4}] | [1,2,{"a":3},{"c":4}] |  [1,2,"c"]
// {"a":1,"b":2} | {"a":1,"b":2} | {"a":1,"b":2,"c":4} |  {"a":1,"b":2,"c":4}  | {"a":1,"b":2}
//     "a":1     |     "a":1     |        "a":1        |         "a":1         |     "a":1
//
//                                       insert with -m
//  to  \ from |        [3,4]        |     {"a":3,"c":4}     |      "a":3,"c":4      |     "c"
//-------------+---------------------+-----------------------+-----------------------+------------+
//    [1,2]    |      [1,2,3,4]      |       [1,2,3,4]       |       [1,2,3,4]       |  [1,2,"c"]
//{"a":1,"b":2}|{"a":[1,3],"b":[2,4]}|{"a":[1,3],"b":2,"c":4}|{"a":[1,3],"b":2,"c":4}|{"a":1,"b":2}
//    "a":1    |     "a":[1,3,4]     |      "a":[1,3,4]      |      "a":[1,3,4]      |  "a":[1,"c"]
//
// update matrix (src -> dst):
//                               update w/o -m
//   to  \ from  |   [3,4]   |   {"a":3,"c":4}   | "a":3 |  "c"
//---------------+-----------+-------------------+-------+--------+
//      [1,2]    |   [3,4]   |   {"a":3,"c":4}   |   3   |   "c"
// {"a":1,"b":2} |   [3,4]   |   {"a":3,"c":4}   |   3   |   "c"
//     "a":1     | "a":[3,4] | "a":{"a":3,"c":4} | "a":3 | "a":"c"
//
//                                     update with -m
//   to  \ from  |     [3,4]     |    {"a":3,"c":4}    |     "a":3     |     "c"
//---------------+---------------+---------------------+---------------+----------------+
//      [1,2]    |     [3,4]     |        [3,4]        |     [3,2]     |   ["c",2]
// {"a":1,"b":2} | {"a":3,"b":4} | {"a":3,"b":2,"c":4} | {"a":3,"b":2} | {"a":"c","b":2}
//     "a":1     |   "a":[3,4]   |  "a":{"a":3,"c":4}  |  "a":{"a":3}  |   "a":"c"
//
// Basic principles:
//  o w/o '-m' src param (-i src) is considered as a whole, with '-m' as an mergeable iterable
//  o insert operation never rewrites the dst data (-w dst), while '-m' may extend it
//  o update operation rewrites the dst data (-w dst)


bool Jtc::upsert_json(char op) {
 #include "lib/dbgflow.hpp"
 // plug-in <insert/update>_by_iterator and let process walks (options -i/-u)
 // return true / false if there was purging / or not
 static wlk_subscr Jtc::* upsert_mtd[2] = {&Jtc::collect_itr_bindings, &Jtc::update_by_iterator};

 if(opt()[CHR(OPT_WLK)].hits() == 0)                            // no -w?
  opt()[CHR(OPT_WLK)] = "";                                     // then walk from root
 ready_params_(op);

 is_tpw_ = opt()[CHR(OPT_TMP)].hits() > 1 and opt()[CHR(OPT_SEQ)].hits() < 2 and
           opt()[CHR(OPT_TMP)].hits() == walk_options_size_();  // tpw for insert/update walks

 walk_interleaved_(upsert_mtd[op == CHR(OPT_UPD)]);
 DBG(1) DOUT() << "collected " << psrc_.size() << (op == CHR(OPT_UPD)? " update":" insert")
               << "-walk pairs" << endl;
 apply_src_walks(op);

 maybe_update_lbl_();                                           // will update lbl if any pending

 if(opt()[CHR(OPT_PRG)].hits() > 0)                             // only work when walk-path is src
  { purge_json(); return true; }

 return false;
}



// collect_itr_bindings() is a common subscriber for multiple functions:
//   o compare_jsons() - facilitating -c
//   o upsert_json facilitating -i, -u (-u uses it via wrapper update_by_iterator())
//
// collect_itr_bindings() performs pairing of the source walked jsons and respective destinations:
// e.g.: when inserting json: source might be an inserted files, or JSONs, or the walk-iterators
// from the inserted file(s)/json(s) or from the input json.
// the destination always will be a walk-itertor over input json (-w):
//   - the destinations always comes as a parameter (Json::iterator &it) from walk_interleaved_()
//   - the sources will be demuxed in advance_to_next_src() into the jits_

void Jtc::collect_itr_bindings(Json::iterator &it, Grouping unused) {
 #include "lib/dbgflow.hpp"
 // facilitate insert each/all -[iu] processed jsons:
 // per each walked destination (-w, facilitated by &it), collect each respective source(s) (jits_)
 // Grouping arg is unused here, but is required by subscriber_'s definition

 if(jits_.walk_size() > 0)                                      // walk_size > 0 means init'ed jits
  wns_[&it].sync_out(jits_.json().ns(), map_jnse::NsOpType::NsReferAll);

 if(ecli_ > Ecli::No_exec and walk_options_size_() == 0)        // -e ... \; alone
  { psrc_.emplace_back(it, it, wns_[&it]); return; }            // collect iterator and ns

 while(advance_to_next_src(it)) {
  DBG(2) DOUT() << "optarg idx [" << jscur_ << "] out of " << walk_options_size_()
                << " (" << (jsrt_ == Src_optarg? "json": "walk") << ")" << endl;
  psrc_.emplace_back(it, jits_);                                // collect iterator
  if(is_multi_walk_) break;
 }
}



void Jtc::update_by_iterator(Json::iterator &it, Grouping unused) {
 #include "lib/dbgflow.hpp"
 // update each/all -u processed jsons
 // Grouping arg is unused here, but is required by subscriber_'s definition
 if(lbl_update_ == false)                                       // not faced label update yet
  lbl_update_ = not it.walks().empty() and it.walks().back().jsearch == Json::key_of_json;
 else                                                           // lbl update occurred, then
  if(not it.is_valid())                                         // verify sanity of dst walks
   { cerr << "error: destination walk became invalid, skipping update" << endl; return; }

 collect_itr_bindings(it);
}



bool Jtc::advance_to_next_src(Json::iterator &jit, signed_size_t i) {
 #include "lib/dbgflow.hpp"
 // iterate current (valid) walk, or begin iterating next one (if available), may come with:
 //  i < 0          - first (non-recursive) call
 //  i >= 0         - recursive call, index of a next source,
 auto idx = [&i, this] { return i < 0? jscur_: i; };            // index select

 DBG(4)
  DOUT() << (i < 0? "non-": "")  << "recusive call, type: " << ENUMS(Jtc::Jitsrc, jsrt_)
         << ", walk src: " << idx() << ", walk/json: '"
         << (jsrc_[idx()].is_neither()? jsrc_[idx()].val(): jsrc_[idx()].to_string(Jnode::Raw, 1))
         << "'" << endl;

 if(jits_.is_valid()) {                                         // if true, walk_size must be > 0
  if((++jits_).is_valid())                                      // end() not reached yet
   { jscur_ = idx(); return true; }
  DBG(4) DOUT() << "source-walk increment failed, advancing to the next source" << endl;
  return advance_to_next_src(jit, idx() + 1 > jsrc_.rbegin()->KEY?
                                   walk_options_start_idx_(): idx() + 1);   // else use next src
 }
 // jits_ is invalid here, could be due to:
 // 1. init call,
 // 2. change source (reached end, i >= 0, walk_size here is always > 0)
 if(not is_multi_walk_ and i == SGNS_T(walk_options_start_idx_())) {    // single_walk, change src
   DBG(4) DOUT() << "single-walk all sources have been walked, ending src advancing" << endl;
  jits_ = Json::iterator{}; return false;                       // and loop => end of operations
 }
 // select type of source (json)
 auto & srcj = jsrt_ == Src_input? json():
               jsrt_ == Src_mixed? (jsrc_[0].is_neither()? jtmp_[jtmp_.size()]: jsrc_[0]):
                (jsrc_[idx()].is_neither()? jtmp_[jtmp_.size()]: jsrc_[idx()]);

 if(jits_.walk_size() == 0)                                     // merge upon jits_ initialization
  wns_[&jit].sync_out(srcj.ns(), map_jnse::NsOpType::NsReferAll);   // merge global ns to -u/i's ns

 jits_ = srcj.walk(jsrt_ == Src_optarg? "": jsrc_[idx()].str());
 // if src arg is a template then resolve:
 if(not jtmp_.empty() and &srcj == &jtmp_.rbegin()->VALUE) {
  auto & jtmp = jtmp_.rbegin()->VALUE;
  jtmp = Json::interpolate(jsrc_[jsrt_ == Src_mixed? 0: idx()].val(),
                                 jit, jits_.json().ns(), Json::ParseTrailing::Relaxed_no_trail);
  if(jtmp.is_neither())
   { cerr << "fail: template argument failed interpolation" << endl;  exit(RC_ARG_FAIL); }
 }


 if(jits_.is_valid()) {                                         // jits_ resolved
  DBG(4) DOUT() << "next selected source idx: " << idx() << endl;
  jscur_ = idx(); return true;
 }
 // jits_ is invalid here
 if(i == static_cast<signed_size_t>(jscur_)) {                  // multiwalk: no more valid itr/src
  DBG(4) DOUT() << "all sources are invalid, failing src advancing" << endl;
  jits_ = Json::iterator{}; return false;                       // fail operation
 }

 DBG(4) DOUT() << "new source-walk increment failed, advancing to the next source" << endl;
 return advance_to_next_src(jit, idx() + 1 > jsrc_.rbegin()->KEY?
                                  walk_options_start_idx_(): idx() + 1);    // return next src itr
}



void Jtc::apply_src_walks(char op) {
 #include "lib/dbgflow.hpp"
 // apply all src walks collected in <insert/update>_by_iterator
 typedef void (Jtc::*ups_ptr)(Json::iterator &dst, Json::iterator src, string *lbl);
 static ups_ptr upsert[2] = {&Jtc::merge_jsons_, &Jtc::update_jsons_};

 vec_bjit vsrc(psrc_.size());                                   // copy of valid iterators
 vector<ShellReturn> vsr;
 Json jexec;                                                    // for temp. copy of jsrc_[0]
 if(ecli_ == Ecli::Bulk_exec) vsr = run_bulk_shell_();

 for(size_t i = 0; i < psrc_.size(); ++i) {
  auto &pair = psrc_[i];
  if(not pair.dst.is_valid())
   { cerr << "error: dst walk " << key_++ << " invalided by prior operations" << endl; continue; }
  if(jsrt_ == Src_input and not pair.src.is_valid())
   { cerr << "error: src walk " << key_++ << " invalided by prior operations" << endl; continue; }

  bool is_ecli_success{false};
  Json::iterator ewlk;                                          // pointing to root of jsrc_[0]
  if(ecli_ > Ecli::No_exec) {
   jexec = jsrc_[0];
   if(DBG()(0)) DBG().increment(+2, jexec, -2);
   is_ecli_success = execute_cli_(jexec, pair.src, pair.ns, vsr.size()? &vsr[i]: nullptr);
   ewlk = jexec.walk();
  }

  if(ecli_ == Ecli::No_exec or is_ecli_success) {
   Json tmp;
   if(DBG()(0)) tmp.DBG().severity(NDBG);
   tmp.type(Jnode::Neither);

   if(opt()[CHR(OPT_TMP)].hits() > 0) {                         // -T given
    if(is_tpw_)                                                 // is template per walk? then:
     tpw_.emplace(pair.src.walk_uid(),                          // relate interleaved walks to tmp
                  tpw_.size() < opt()[CHR(OPT_TMP)].size() - 1?
                   opt()[CHR(OPT_TMP)].str(tpw_.size() + 1): "");
    else                                                        // circular template application
     tpw_.emplace(upst_key_ % (opt()[CHR(OPT_TMP)].size() - 1),
                  opt()[CHR(OPT_TMP)].str(tpw_.size() + 1));
    size_t tmp_idx = is_tpw_?
            pair.src.walk_uid(): upst_key_++ % (opt()[CHR(OPT_TMP)].size() - 1);
    tmp = Json::interpolate(tpw_[tmp_idx], ecli_ > Ecli::No_exec? ewlk: pair.src, pair.ns);
   }

   (this->*upsert[op == CHR(OPT_UPD)])                           // demux -i and -u
    (pair.dst, not tmp.is_neither()? tmp.walk():
                                      ecli_ > Ecli::No_exec? ewlk: pair.src, &pair.lbl);
  }

  vsrc[key_] = move(psrc_[key_]);
  ++key_;
 }
 psrc_ = move(vsrc);
}



void Jtc::purge_json(void) {
 #include "lib/dbgflow.hpp"
 // remove all json nodes pointed by iterator(s), or do reverse
 if(opt()[CHR(OPT_WLK)].hits() == 0)                            // no -w?
  opt()[CHR(OPT_WLK)] = "";                                     // then walk from root

 if(opt()[CHR(OPT_PRG)].hits() > 1)                             // -pp process here
  return crop_out_();

 for(auto &wp: opt()[CHR(OPT_WLK)]) {                           // process all walks -w
  vec_jit ji = collect_walks_(wp);                              // collect all purging points

  for(size_t i = 0; i < ji.size(); ++i) {                       // purge all walked instances
   if(ji[i].is_valid()) {                                       // i.e. hasn't been deleted already
    auto & rec = *ji[i];
    if(rec.is_root()) rec.clear();                              // if root, erase everything
    else rec[-1].erase(ji[i]);
   }
  }
 }
}



void Jtc::swap_json(void) {
 #include "lib/dbgflow.hpp"
 // swap around nodes pointed by 2 walk pairs
 for(size_t si = 1; si < opt()[CHR(OPT_WLK)].hits(); si += 2) {
  vector<vec_jit> swaps{2};                                     // collect all walks in here
  swaps[0] = collect_walks_(opt()[CHR(OPT_WLK)].str(si));
  swaps[1] = collect_walks_(opt()[CHR(OPT_WLK)].str(si+1));

  size_t max_i = min(swaps[0].size(), swaps[1].size());
  for(size_t i = 0; i < max_i; ++i) {                           // swap only paired walks
   if(not swaps[0][i].is_valid() or not swaps[1][i].is_valid()) {
    cerr << "error: walk instance " << i
         << " became invalid due to prior operations, skipping" << endl;
    cr_.rc(RC_WP_INV);
    break;
   }
   swap(*swaps[0][i], *swaps[1][i]);
  }
 }
}



void Jtc::walk_json(void) {
 #include "lib/dbgflow.hpp"
 // walk all -w paths, output via subscriber
 Json jdb;                                                      // integrity check in debugs only
 if(DBG()(0)) {
  DOUT() << DBG_PROMPT(0) << "copying input json for integrity check (debug only)" << endl;
  jdb = json();
 }

 if(opt()[CHR(OPT_JSN)].hits() == 1) jout_ = ARY{};             // otherwise jout_ is OBJ (i.e -jj)
 walk_interleaved_(&Jtc::output_by_iterator);

 if(use_hpfx_ and wcnt_ > 0 and opt().process_last(wcnt_)) {    // if there was output
  GUARD(opt().wm())                                             // preserve multi-walk factors and
  GUARD(opt()[CHR(OPT_TMP)])                                    // preserve templates, and do not
  opt().wm().clear();                                           // let them interfere inc. template
  opt()[CHR(OPT_TMP)].reset();                                  // as value already interpolated
  Json h(hwlk_);                                                // produce an itr out of last walk
  output_by_iterator(h.walk("[0]"));
 }

 // after walking all paths
 if(opt()[CHR(OPT_JSN)].hits() > 0)                             // -j, jout_ contains the output
   write_json(jout_, Dont_jsonize);                             // jsonization is done by walking
                                                                // otherwise it's already stdout'ed
 // check json integrity...
 DBG(0)
  DOUT() << "source json integrity check: "
         << (jdb == json()? "Good. (...phew!)": "BROKEN! (aaaa! panic! bug found!)" ) << endl;
}



// output_by_iterator() is one of the subscribers: subscriber model (in general) is used to process
//  walked (-w) JSON iterators, while output_by_iterator is used to either print
//  walked JSON element to the console or out jout_ (facilitating -j / -jj options)
//
// output_by_iterator() handles:
//   o syncs name-space (global with walked iterator's as well as last walked historical's)
//   o handles -T (template interpolation) option(s)
//   o demux output destination - to the console or to jout (facilitating -j / -jj)
//
// - output_by_iterator() is used by:
//     o compare_bingings (to print 2 json diffs)
//     o walk_json() as a subscriber, or once directly for ensuring option variant -x0 (-x/-1)

void Jtc::output_by_iterator(Json::iterator &wi, Grouping grp) {
 #include "lib/dbgflow.hpp"
 // prints json element from given iterator (used by -w)
 // in case of -j option: collect into provided json container rather than print to console
 Json tmp;                                                      // template-interpolation goes here
 tmp.type(Jnode::Neither);
 if(DBG()(0)) DBG().increment(+2, tmp, -2);                     // imbue current dbg +3 in tmp

 if(use_hpfx_) {
  auto found = wns_[&wi].find(WLK_RSTH);
  if(found != wns_[&wi].end() and wns_[&wi].find(WLK_RSTH)->VALUE.bul() == true)
   hwlk_[0] = STR{};
  wns_[&wi][WLK_HPFX] = move(hwlk_[0]);
  hwlk_[0].type(Jnode::Neither);
 }
 if(opt()[CHR(OPT_TMP)].hits() > 0) {                           // interpolate each tmp per walk
  // in all other cases - templates are round-robin applied onto each walk iteration
  if(is_tpw_)                                                   // template per walk:
   tpw_.emplace(wi.walk_uid(),                                  // relate interleaved walks to tmp
                 tpw_.size() < opt()[CHR(OPT_TMP)].size() - 1?
                  opt()[CHR(OPT_TMP)].str(tpw_.size() + 1): "");
  else                                                          // circular template application
   tpw_.emplace(key_ % (opt()[CHR(OPT_TMP)].size() - 1),
                 opt()[CHR(OPT_TMP)].str(tpw_.size() + 1));
  size_t tmp_idx = is_tpw_?
         wi.walk_uid(): key_++ % (opt()[CHR(OPT_TMP)].size() - 1);
  tmp = Json::interpolate(tpw_[tmp_idx], wi, wns_[&wi]);
  if(use_hpfx_) hwlk_ = move(ARY{tmp.root()});
 }

 if(use_hpfx_ and hwlk_[0].is_neither()) {                      // templating failed or was none
  if(wi->has_label()) hwlk_ = move(OBJ{LBL{wi->label(), *wi}}); // preserve also the label if exist
  else hwlk_ = move(ARY{*wi});
 }

 typedef void (Jtc::*jd_ptr)(Json::iterator &, Json &, Grouping);   // demux cout and -j outputs
 static jd_ptr demux_out[2] = {&Jtc::console_output_, &Jtc::jsonized_output_};
 if(opt().wm().empty() or opt().process_walk(wcnt_))            // output taking into account -xn/n
  (this->*demux_out[opt()[CHR(OPT_JSN)].hits() > 0])(wi, tmp, grp);
 last_ = grp;
 ++wcnt_;
}



//
// Jtc PRIVATE methods definitions
//
void Jtc::display_location_(Streamstr::const_iterator &jbegin) {
 // debug show locus of the exception, unicode UTF-8 supported
 exception_locus_(jbegin);
 exception_spot_(jbegin);
}



void Jtc::exception_locus_(Streamstr::const_iterator &jbegin) {
 // shows whereabouts of the exception
 string jsrc{jbegin.str()};
 size_t el =  jbegin.is_streamed()? jsrc.size() - 1: distance(jbegin, json().exception_point());
 DBG(0) DOUT() << Debug::ctw(el) << jsrc << endl;
}



void Jtc::exception_spot_(Streamstr::const_iterator &jbegin) {
 // points to the spot of the exception in the about debug (exception_locus_)
 // this method name must be 1 char shorter then previous (exception_locus_)
 string jsrc{jbegin.str()};
 size_t from_start = Json::utf8_adjusted(0, jsrc, jbegin.is_streamed()?
                                                   jsrc.size() - 1:
                                                   distance(jbegin, json().exception_point()));
 cr_.elocation(jbegin.is_streamed()? cr_.iss().stream_size() - 1: from_start);
 DBG(0) DOUT() << string(DBG().ctw_adjust(), '-')
               << ">| (offset: " << cr_.elocation() << ")" << endl;
}

// certain operations in jtc (-i, -u, and also -c) may have different types of parameters
// and they all may refer to different types of sources. There are 4 resources which select
// source of operation for the operations:
//   o type of source: jsrt_:
//     may be set to either of values: [Src_input] / Src_mixed / Src_optarg
//   o iterator over the source (Json::iterator):
//     jits_ - holds a walk() results over the respective source
//   o the source container (jsrc_):
//     that one could be a different resource depending on the type of operation,
//     there are following types in total (considering -u in examples):
//     1. -u<WLK> -u<WLK> ...
//        walk(s) here refers to walking source JSON (jinp_ is a src JSON container),
//        jsrt_ = Src_input; jits_ = walk() from source (i.e. jinp_.walk(...))
//        jscur_ points to a index in jsrc_, which holds walk strings
//     2. -u<JSN> -u<JSN> ...
//        option parameter(s) is either a JSON(s) (literally spelled), or a file, or a template
//        jsrt_ = Src_optarg; jits_ = walk() from jsrc_ - source container,
//        jsrc_ holds all JSON(s)/file(s),/template(s), while
//        jscur_ points to currently walked JSON (index in jsrc_)
//     3. -u<JSN> -u<WLK> -u<WLK> ...
//        walk parameter here refers to walking a JSON,
//        jsrt_ = Src_mixed,
//        jits_ = walk() from either jsrc_[0] - src holding literal JSON, or read from file, or to
//        jtmp_ - resolved json in case if jsrc[0] is a template string
//        jscur_ points to current walk string (index in jsrc_)
//     4. -eu <...> \;
//        the entire cli argument <...> is stored in jsrc_[0]
//     5. -eu <...> \; -u<WLK> -u<WLK> ...
//        jsrt_ = Src_input;
//        jits_ = jinp_.walk(...)
//        jsrc_ - holds cli argument in jsrc_[0], the rest hold walk-strings
//        jscur_ points to current walk string (index in jsrc_)
//
//
// Design notes:
// jsrc_ is a container for all kinds of option argument for -i/u/c and its type is a map of json:
//      typedef map<size_t, Json> map_json;
//      , where key is just an ordinal index (preserving the order of argument occurrence)
//
// the container must be able to hold 3 different types of arguments:
//  a. it may hold an ordinary JSONs (read from file, or spelled literally)
//  b. it may hold a JSON string - as a JSON template, JSON type is set to Neither
//     (to indicate a template)
//  c. it may hold a JSON string - as a WALK argument.
//  storage formats per type:
//    o Src_input: would store only <WLK>, starting from index 1
//    o Src_optarg: would store only jsons/templates (starting from index 0)
//    o Src_mixed: jrc_[0] would hold only json/template, while the rest are <WLK> only
//
// the disambiguation path for the option argument type is:
//  1. assume the argument is a file and attempt to read and parse one
//  2. if 1 fails (0 bytes read), assume it's a literal JSON and attempt to parse as JSON
//  3. if 2 fails, assume it's a walk-path and attempt to compile it
//  4. if 3 fails (exception occurs), finally assume it's a template, defer resolution until later
//
// Supported arguments combinations (e.g., for -i):
//  A. -i<WLK> -i<WLK> ... -> type: Src_input -> all walks done on input json (jinp_)
//  B. -i<JSON> -i<JSON> ..., where JSON is either a file, a literal JSON, or a template,
//      type: Src_optarg
//  C. -i<JSON> -i<WLK> -i<WLK> ... -> type: Src_mixed,
//     o the type or the 1st argument(s) here is either file/literal/template
//     o multiple walk path could be given
//     o thus, jsrc_[0] holds the source of operation, while subsequent elements - walks
//       of the jsrc_[0]
//
// when JSON is a template - the template actualization occurs during `advance_to_next_src()` call
// (i.e. after all input JSON walking is done and therefore all namespaces are resolved) and must
// result in a valid JSON, otherwise an exception will be thrown in that case.
//
// to handle advancing to a next source smoothly for all types of arguments, store policy is this:
//  o for all <WLK> arguments, start filling jsrc_ from index 1
//  o for all <JSON> arguments (file/literal/template), start filling jsrc_ from index 0
//  - with that policy, it'll be programmatically easy to converge to Src_mixed type
//
// policy for filling arguments:
//  o when multiple (>1) Src_optarg already given, fail shall the other arg types occur
//  o when multiple Src_input given, may convert to Src_mixed for the 1st parsed JSON/template
//  o when in Src_mixed mode, fail shall further json/templates occur


void Jtc::ready_params_(char option) {
 #include "lib/dbgflow.hpp"
 // fill / prepare data:
 // jsrc: to fill up with respect to the jsrt_ mode
 for(string & arg: opt()[option]) {
  size_t jkey = jsrc_.empty()? 0: jsrc_.rbegin()->KEY + 1;
  if(jkey == 0 and ecli_ > Ecli::No_exec)
   { jsrc_[jkey].root() = arg; continue; }                      // preserve in jsrc_[0] -e argument
  if(jsrt_ == Jitsrc::Src_mixed or ecli_ > Ecli::No_exec)       // preserve only <WLK>
   { jsrc_[jkey]; ready_params_walks_(arg, jkey); continue; }
  // first assume arg is a <FILE>, and try reading from file,
  // if file read failed (0 bytes read) then assume it's a literal <JSON> and try parsing it
  string jfile;
  read_json_(jfile, arg);
  jsrc_[jkey];                                                  // provision a place for json
  if(DBG()(0)) DBG().increment(+2, jsrc_[jkey], -2);            // imbue current debug +3
  DBG(1) DOUT() << "attempting to parse parameter (" << arg << ") as a static json" << endl;

  parse_option_arg_(jsrc_[jkey], arg, jfile);
  if(jsrc_[jkey].parsing_failed()) {    // it could be a <WLK>, or a <TMP>
   if(not jfile.empty())                                        // file was read but failed parsing
    { cerr << "fail: file '" << arg << "' holds an invalid JSON" << endl; exit(RC_ARG_FAIL); }
   ready_params_walks_(arg, jkey);                              // process <WLK> or <TMP>
  }
  else {                                // parsed a valid <JSN>
   if(jkey == 0) { jsrt_ = Jitsrc::Src_optarg; continue; }      // 1st argument: Src_optarg
   // here it could be Src_input, or Src_optarg (Src_mixed handled above)
   if(jsrt_ == Jitsrc::Src_input)                               // only walks recorded so far
    { jsrc_[0].root() = move(jsrc_[jkey].root()); jsrc_.erase(jkey); jsrt_ = Src_mixed; }
  }
 }

 jscur_ = walk_options_start_idx_();
 DBG(1)  DOUT() << "total jsons: " << (jsrc_.size())
                << ", arg mode: " << ENUMS(Jtc::Jitsrc, jsrt_)
                << ", starting walk idx: " << jscur_ << endl;
}



void Jtc::parse_option_arg_(Json &jsn, const string & arg, const string & jfile) {
 #include "lib/dbgflow.hpp"
 // parse arg or jfile, convert jfile to JSON array if it's a stream of JSONs
 Streamstr is;
 Streamstr::const_iterator isp = is.source_buffer(jfile.empty()? arg: jfile).begin();

 jsn.parse_throwing(false);
 jsn.parse(isp, jfile.empty()? Json::Strict_no_trail: Json::Strict_trailing);

 if(not jsn.parsing_failed()) return;                           // successful parsing
 if(jfile.empty()) return;                                      // arg has failed parsing
 if(jsn.exception_reason() != Jnode::ThrowReason::unexpected_trailing)
  return;                                                       // jfile contains bad JSON

 // jfile might hold a stream of JSONs - attempt to convert to array
 Json j{ARY{}};                                                 // accumulate streamed JSONs here
 if(DBG()(0)) j.DBG().severity(NDBG);

 while(jsn.exception_reason() == Jnode::ThrowReason::unexpected_trailing) {
  j.push_back(move(jsn.root()));
  jsn.parse(isp, Json::Strict_trailing);
 }
 if(not jsn.parsing_failed()) {
  j.push_back(move(jsn.root()));
  jsn = move(j);
 }
}



void Jtc::ready_params_walks_(const string &arg, size_t jkey) {
 #include "lib/dbgflow.hpp"
 // process walks and templates
 DBG(1) DOUT() << "attempting to compile parameter (" << arg << ") as a walk" << endl;
 try {
  Json::iterator dummy;
  json().compile_walk(arg, dummy);

  if(jsrt_ == Jitsrc::Src_optarg and jsrc_.size() > 2) {
   cerr << "fail: argument walk-type (" << arg << ") not allowed, due to mode "
         << ENUMS(Jtc::Jitsrc, Src_optarg) << " already set" << endl;
   exit(RC_ARG_FAIL);
  }
  jsrc_[jkey].root() = arg;
  if(jkey == 0)                                                 // Src_input must start with idx 1
   { jsrc_[1].root() = move(jsrc_[0].root()); jsrc_.erase(0); } // jsrt_ = Src_input by default
  if(jsrt_ == Jitsrc::Src_optarg)                               // assert: jsrc_.size() == 2
   jsrt_ = Jitsrc::Src_mixed;
 }
 catch (Json::stdException & e) {                               // it's a TMP then
  if(ecli_ > Ecli::No_exec) {                                   // -e was given, only <WLK> allowed
   cerr << "fail: non-walk argument (" << arg << ") not allowed, due to option -"
        << CHR(OPT_EXE) << " given" << endl;
   exit(RC_ARG_FAIL);
  }
  DBG(1) DOUT() << "assuming parameter (" << arg << ") is a template" << endl;
  jsrc_[jkey].root() = arg; jsrc_[jkey].type(Jnode::Neither);   // indicate it's a <TMP>
  if(jkey == 0) { jsrt_ = Jitsrc::Src_optarg; return; }         // 1st argument: Src_optarg
  if(jsrt_ == Jitsrc::Src_mixed) {                              // jsrc_[0] already exists!
   cerr << "fail: non-walk argument (" << arg << ") not allowed, due to mode "
        << ENUMS(Jtc::Jitsrc, Src_mixed) << " already set" << endl;
   exit(RC_ARG_FAIL);
  }
  if(jsrt_ == Jitsrc::Src_input)                                // only walks recorded so far
   { jsrc_[0].root() = move(jsrc_[jkey].root()); jsrc_.erase(jkey); jsrt_ = Src_mixed; }
 }
}



void Jtc::read_json_(string &jfile, const string &fname) {
 #include "lib/dbgflow.hpp"
 // read json from file (doing it a non-idiomatic c++ way for performance reason)
 std::ifstream fin(fname, std::ios::in);
 if(not fin) return;

 fin.seekg(0, std::ios::end);
 jfile.resize(fin.tellg());

 fin.seekg(0, std::ios::beg);
 fin.read(&jfile[0], jfile.size());
}



void Jtc::maybe_update_lbl_(void) {
 #include "lib/dbgflow.hpp"
 // update labels (-u operations, for '<>k' lexemes ") if any pending update
 size_t lpv_idx;                                                // longest path_vector's index
 do {
  lpv_idx = SIZE_T(-1);
  for(size_t i = 0, lpv = 0; i < psrc_.size(); ++i)
   if(psrc_[i].update_lbl())
    if(lpv < psrc_[i].dst.path_size())
     { lpv_idx = i; lpv = psrc_[i].dst.path_size(); }
  if(lpv_idx == SIZE_T(-1)) continue;

  auto & pair = psrc_[lpv_idx];
  if(not pair.dst.is_valid())
   { cerr << "error: dst walk invalided by prior operations" << endl; pair.reset_lbl(); continue; }
  auto & parent = (*pair.dst)[-1];
  parent[pair.lbl] = move(parent[pair.dst->str()]);             // lbl update is in fact moving to
  parent.erase(pair.dst->str());                                // a new label and erasing old one
  pair.reset_lbl();

 } while(lpv_idx != SIZE_T(-1));
}



void Jtc::compare_jsons_(const Jnode &j1, set<const Jnode*> &s1,
                         const Jnode &j2, set<const Jnode*> &s2) {
 #include "lib/dbgflow.hpp"
 // compare j1 <> j2, put into s1 and j2 nodes which are different (require preserving)
 if(j1.type() != j2.type()) {
  s1.insert(&j1);
  s2.insert(&j2);
  return;
 }

 if(j1.is_object()) {
  set<string> keys;
  for(auto &r: j1) keys.insert(r.label());
  for(auto &r: j2) keys.insert(r.label());
  for(auto &l: keys) {
   if(j1.count(l) == j2.count(l))
    { compare_jsons_(j1[l], s1, j2[l], s2); continue; }
   if(j1.count(l) == 0) s2.insert(&j2[l]);
   else s1.insert(&j1[l]);
  }
  return;
 }

 if(j1.is_array()) {
  auto i1 = j1.begin(), i2 = j2.begin();
  while(i1 != j1.end() or i2 != j2.end()) {
   if(i1 == j1.end()) { s2.insert(&i2->value()); ++i2; continue; }
   if(i2 == j2.end()) { s1.insert(&i1->value()); ++i1; continue; }
   compare_jsons_(i1->value(), s1, i2->value(), s2);
   ++i1; ++i2;
  }
  return;
 }

 if(j1 == j2) return;
 s1.insert(&j1);
 s2.insert(&j2);
}



void Jtc::merge_jsons_(Json::iterator &it_dst, Json::iterator it_src, string *unused) {
 #include "lib/dbgflow.hpp"
 // merge 2 jsons. convert to array non-array dst jsons (predicated by -m)
 if(it_dst.reinterpret_label())                                 // '<>k' facing
  { cerr << "error: insert into label not applicable, use update" << endl; return; }

 if(it_dst->is_object()) {                                      // dst is object
  if(it_src->has_label())                                       // it's coming from -i walk-path
   merge_into_object_(*it_dst, OBJ{ LBL{it_src->label(), *it_src} }, Preserve);
  else
   if(it_src->is_iterable())                                    // from either json/walk-path
    merge_into_object_(*it_dst, *it_src, Preserve);
   else
    cerr << "error: only an iterable could be insert-merged with an object" << endl;
  return;
 }

 if(merge_) {                                                   // -m given
  DBG(1) DOUT() << "merging into array" << endl;
  merge_into_array_(*it_dst, *it_src, Preserve);                // merge any jsons
  return;
 }

 if(it_dst->is_array()) {                                       // merge only 1 json object
  it_dst->push_back(it_src->has_label()? OBJ{ LBL{it_src->label(), *it_src} }: *it_src);
  return;
 }
 cerr << "error: walk-path must point to an iterable, or use merging" << endl;
}



void Jtc::merge_into_object_(Jnode &dst, const Jnode &src, MergeObj mode) {
 #include "lib/dbgflow.hpp"
 // merge into object, recursively. assert(dst.is_object() and src.is_iterable())
 if(not src.is_object()) {                                      // merge arr->obj, node by node
  DBG(2) DOUT() << "merge array into object" << endl;
  if(not merge_) return;                                        // only if -m given
  auto di = dst.begin();
  for(auto &src_child: src.is_array()? src: ARY{ move(src) }) { // go by every element in the src
   if(di == dst.end()) break;
   if(mode == Overwrite)
    *di = src_child;                                            // overwrite upon instruction only
   else
    merge_into_array_(*di, src_child, mode);
   ++di;
  }
  return;
 }
                                                                // merge object->object recursively
 DBG(2) DOUT() << "merge objects" << endl;
 for(auto &src_child: src) {                                    // go by every element in the src
  auto di = dst.find(src_child.label());                        // dst iterator
  if(di == dst.end())                                           // labels not clashing
   { dst[src_child.label()] = src_child; continue; }            // merge non-clashing labels

  if(di->is_object() and src_child.is_object())                 // both clashing elements are OBJ
   { merge_into_object_(*di, src_child, mode); continue; }      // merge OBJ elements recursively

  if(merge_) {                                                  // if -m given
   if(mode == Overwrite)
    *di = src_child;                                            // overwrite upon instruction only
   else
    merge_into_array_(*di, src_child, mode);                    // merge clashing non-obj into arr
  }
 }
}



void Jtc::merge_into_array_(Jnode &dst, const Jnode &src, MergeObj mode) {
 #include "lib/dbgflow.hpp"
 // merge 2 jsons into array.
 // convert dst to array if non-array. convert src to array if non-iterable
 const Jnode *src_ptr = &src;
 Jnode src_array;

 if(not dst.is_array()) dst = ARY{ move(dst) };                 // convert to arrays if not yet
 if(not src.is_iterable())
  { src_array = ARY{ move(src) }; src_ptr = &src_array; }

 if(mode == Preserve) {
  DBG(2) DOUT() << "pushing into destination" << endl;
  size_t safity_cnt = src_ptr->children();                      // to avoid endless loop when src
  for(auto &src_child: *src_ptr) {
   dst.push_back(src_child);
   if(--safity_cnt == 0) break;                                 // and dst is the same array
  }
  return;
 }
                                                                // mode == overwrite
 DBG(2) DOUT() << "overwriting destination" << endl;
 if(dst.is_empty()) return;
 auto di = dst.begin();
 for(auto &src_child: *src_ptr) {
  *di = src_child;
  if(++di == dst.end()) break;
 }
}



void Jtc::update_jsons_(Json::iterator &it_dst, Json::iterator it_src, string *lbl) {
 #include "lib/dbgflow.hpp"
 // update dst with src, merge jsons with overwrite if -m is given
 if(it_dst.reinterpret_label()) {                               // '<>k' facing
  DBG(2) DOUT() << "label being updated" << endl;               // facilitate '<>k' (empty lexeme)
  if(merge_)
   { cerr << "error: merge not applicable in label update, ignoring" << endl; }
  if(it_src->is_iterable())
   { cerr << "error: label could be updated only with JSON atomic value" << endl; return; }
  auto & parent = (*it_dst)[-1];
  if(not parent.is_object())
   { cerr << "error: labels could be updated in objects only" << endl; return; }
  if(*it_src == *it_dst) return;                                // do not move then
  // labels update is in fact moving to a new label and removing the old one.
  // This creates a dilemma for renaming at once all nested labels. To handle it w/o failures
  // a *delayed processing* is required (like with purging):
  //    o here, mark only labeled operation, that's it
  //    o once all processing finished - rename labels by selecting deepest walks
  if(lbl) {                                                     // record pending label update
   *lbl = it_src->is_null()? "null": it_src->is_bool()? (*it_src? "true":"false"): it_src->val();
   return;
  }
  parent[it_src->str()] = move(parent[it_dst->str()]);          // lbl update is in fact moving to
  parent.erase(it_dst->str());                                  // a new label and erasing old one
  return;
 }

 if(not merge_) {
  DBG(2) DOUT() << "destination being overwritten" << endl;
  *it_dst = *it_src;
  return;
 }

 DBG(2) DOUT() << "destination being merged" << endl;
 if(it_dst->is_object())
  merge_into_object_(*it_dst, it_src->has_label()?
                              OBJ{ LBL{ it_src->label(), *it_src } }: *it_src, Overwrite);
 else
  if(it_dst->is_array())
   merge_into_array_(*it_dst, *it_src, Overwrite);
  else
   *it_dst = it_src->has_label()? OBJ{ LBL{ it_src->label(), *it_src } }: *it_src;
}



vector<Jtc::ShellReturn> Jtc::run_bulk_shell_(void) {
 #include "lib/dbgflow.hpp"
 // build vector<ShellReturn> by trying to run all commands in one shell
 vector<ShellReturn> vsr;
 if(arg_max_ < 0) {
  arg_max_ = sysconf(_SC_ARG_MAX);                              // resolve ARG_MAX
  if(arg_max_ < 0) {                                            // resort to a legacy way
   cerr << "notice: cannot resolve ARG_MAX: have to run shell cli one by one" << endl;
   ecli_ = Ecli::Per_walk_exec;
   return vsr;
  }
 }

 vector<std::string> vcli(1);
 for(auto &pair: psrc_) {                                       // build vector of cli strings
  string clistr = interpolate_shell_str_(pair.src, pair.ns) + SHELLSFX;
  if(SGNS_T(clistr.size() + vcli.back().size()) >= arg_max_/2)  // ARG_MAX is not reliable, so
   vcli.push_back(string{});                                    // playing safe by lowering limit
  vcli.back() += move(clistr);
 }

 GUARD(sh_.DBG().severity, sh_.DBG().severity)
 if(DBG()(0)) sh_.DBG().severity(NDBG);
 auto re = regex(SHELLRGX);
 vsr.reserve(psrc_.size());
 for(const auto &cli: vcli) {
  sh_.system( cli );
  for(sregex_iterator it = sregex_iterator(sh_.out().begin(), sh_.out().end(), re);
      it != sregex_iterator(); ++it) {
   std::smatch m = *it;
   vsr.push_back( ShellReturn{move(m[1]), stoi(m[2])} );
  }
 }
 if(vsr.size() == psrc_.size())  return vsr;

 // resort to a legacy way
 cerr << "notice: running bulk cli failed, reverting to running cli one by one" << endl;
 ecli_ = Ecli::Per_walk_exec;
 vsr.clear();
 return vsr;
}



bool Jtc::execute_cli_(Json &json, Json::iterator &jit, Json::map_jne &ns, ShellReturn * srptr) {
 #include "lib/dbgflow.hpp"
 // execute cli in -i/u option (interpolating jit if required) and parse the result into json
 ShellReturn sr;
 if(srptr == nullptr) {
  sh_.system( interpolate_shell_str_(jit, ns) );
  sr = ShellReturn{move(sh_.out()), sh_.rc()};
 }
 else
  sr = move(*srptr);

 if(sr.rc != 0)
  { cerr << "error: shell returned error (" << sr.rc << ")" << endl; return false; }

 while(not sr.str.empty() and sr.str[sr.str.size() - 1] <= CHR_SPCE)// remove all trailing spaces
  sr.str.pop_back();
 if(sr.str.empty())
  { DBG(1) DOUT() << "shell returned empty result, not updating" << endl; return false; }

 try { json.parse(sr.str); }
 catch(Json::stdException & e) {                                // promote output to JSON string
  if(e.code() == Jnode::unexpected_end_of_string) throw;        // sh.stdout had no valid json
  sr.str = json.inquote_str(sr.str);                            // qoute '\' and '"' chars
  // translate \r,\n,\t and rid of dodgy characters
  typedef pair<const char*, const char*> pos;                   // pair of strings
  for(auto &p: {pos{"\r", "\\r"}, pos{"\n", "\\n"}, pos{"\t","\\t"}})
   sr.str = regex_replace(sr.str, regex{p.first}, p.second);    // escape \n, \r, \t
  sr.str = regex_replace(sr.str, regex{"[\01-\037]"}, "");      // remove all other dodgy spaces
  json.parse(STR_DQTE + sr.str + STR_DQTE);
 }
 return true;
}



string Jtc::interpolate_shell_str_(Json::iterator &jit, Json::map_jne &ns) {
 #include "lib/dbgflow.hpp"
 // interpolate arg for -e -i/u <arg> and escape bash shell dodgy characters
 Json ij = Json::interpolate(jsrc_[0].str(), jit, ns, Json::Dont_parse);
 DBG(2) DOUT() << "interpoldated & quoted string: '" << sh_quote_str(ij.str()) << "'" << endl;
 return sh_quote_str(ij.str());
}



void Jtc::crop_out_(void) {
 #include "lib/dbgflow.hpp"
 // output walked elements preserving source json structure
 set<const Jnode*> preserve;

 for(auto &wp: opt()[CHR(OPT_WLK)])                             // process all walks (-w)
  for(auto &ji: collect_walks_(wp))
   preserve.insert(&ji->value());

 DBG(0) DOUT() << "preserved instances: " << preserve.size() << endl;
 remove_others_(preserve, json().root());
}



bool Jtc::remove_others_(set<const Jnode*> &ws, Jnode &jn) {
 #include "lib/dbgflow.hpp"
 // return true if node has to be removed. remove all but those in ws
 bool remove = true;
 if(ws.count(&jn.value())) return false;                        // preserve root itself if found

 for(auto it = jn.begin(); it != jn.end();) {
  if(it->is_atomic()) {
   if(ws.count(&it->value()) == 0)                              // atomic entry is not among walked
    { jn.erase(it); continue; }                                 // erase the entry then
   else remove = false;                                         // don't erase this iterable
  }
  else {                                                        // it's nested iterable
   if(ws.count(&it->value()) == 1)                              // if itself a walked entry
    remove = false;                                             // then preserve it entirely
   else {                                                       // otherwise figure if to removed
    if(remove_others_(ws, *it) == true)                         // no walked entries inside
     { jn.erase(it); continue; }
    else remove = false;
   }
  }
  ++it;
 }
 return remove;
}



Jtc::vec_jit Jtc::collect_walks_(const string &walk_path) {
 #include "lib/dbgflow.hpp"
 // collect all walk iterations from given walk path (used by swap/purge)
 vec_jit walk_itr;
 if(opt()[CHR(OPT_INS)].hits() > 0 or opt()[CHR(OPT_UPD)].hits() > 0 ) {    // process -p -i/u
  for(auto &pair: psrc_) {                                      // psrc is filled when walked -[ui]
   auto &it = jsrt_ == Src_input? pair.src: pair.dst;
   if(not it.is_valid()) continue;
   walk_itr.push_back(move(it));
  }
  DBG(0) DOUT() << "source of iterations: '" << ENUMS(Jitsrc, jsrt_)
                << "', instances: " << walk_itr.size() << endl;
  return walk_itr;
 }

 for(auto it = json().walk(walk_path, Json::Keep_cache); it != json().end(); ++it)
  walk_itr.push_back(it);
 wns_[last_dwi_ptr_].sync_in(json().ns(), map_jnse::NsMoveAll);

 DBG(0) DOUT() << "walk path: '" << walk_path << "', instances: " << walk_itr.size() << endl;

 return walk_itr;
}



// interleaved walks engaged when multiple walk paths are given (in absence of -n)
// processing of interleaved walks is somewhat complex:
// 1. collect all iterators produced by each walk path into a vector (wpi), e.g
//    [ [w0_i0, w0_i1, w0_iend],
//      [w1_i0, w1_i1, w1_i2, w1_iend],
//      [...],
//      ...
//    ]
// 2. for all instances build a matrix from front iterators offsets (fom) only, for
//    entire walk state:
//    - iterator offset for a built walk state returns -1 if walk step is non-iterable,
//      otherwise walk position's current offset is returned, e.g.:
//      w0_i0 and w1_i0, result in the probable FOM:
//          [ [w0_i0_p0 (0), w0_i0_p1 (0), w0_i0_p2 (-1),
//            [w1_i0_p0 (0), w1_i0_p1 (-1)] ]
// 3. then build a vector referring *actual* instances: *actual* is the instance that is
//    non-empty (e.g.: 0, 1)
// 4. among actual iterators, select first in the relevant group, give it to the subscriber
//    and remove it from the wpi
// 5. repeat until entire wpi is empty
//
// Namespaces:
// each walk (-w) starts with a blank namespace sync'ing only global NS (which carries namespaces
// from prior option sets).
// each walk's iteration namespaces are also preserved in (NS per each iterator in wns_)

void Jtc::walk_interleaved_(wlk_subscr Jtc::* subscriber) {
 #include "lib/dbgflow.hpp"
 // collect all walks (-w) and feed one by one to the subscriber
 subscriber_ = subscriber;
 deque<deq_jit> wpi;                                            // wpi holds queues of iterators

 cr_.global_ns().sync_out(json().ns(), map_jnse::NsOpType::NsReferAll);

 for(const auto &walk_str: opt()[CHR(OPT_WLK)]) {               // process each -w arguments
  // wpi: push back deq_jit init'ed with -w inited walk
  wpi.push_back( deq_jit{json().walk(walk_str.find_first_not_of(" ") == string::npos?
                                     "": walk_str, Json::Keep_cache)} );
  if(opt()[CHR(OPT_SEQ)].hits() % 2 == 1 and wpi.size() > 1) {  // -n and multiple -w given
   wpi.front().push_back( move(wpi.back().front()) );           // move queue to front wpi
   wpi.pop_back();                                              // drop last instance
  }
  auto & dwi = wpi.back();                                      // dwi: deque<Json::iterator>
  while(dwi.back() != dwi.back().end()) {                       // extend all iterators until end
   //cerr << " extending interators" << endl;
   if(wns_.empty()) wns_[&dwi.back()].sync_in(json().ns(), map_jnse::NsOpType::NsMoveAll);
   else wns_[&dwi.back()].sync_in(json().ns(), map_jnse::NsOpType::NsMove);
   if(use_hpfx_) {
    wns_[&dwi.back()][WLK_RSTH] = BUL{wns_[&dwi.back()].erase(WLK_HPFX)? true: false};
    if(dwi.back()->has_label()) hwlk_ = move(OBJ{ LBL{dwi.back()->label(), *dwi.back()} });
    else hwlk_ = move(ARY{*dwi.back()});
   }
   wns_[&dwi.back()].sync_out(json().clear_ns().ns(), map_jnse::NsReferAll);
   last_dwi_ptr_ = &dwi.back();
   dwi.push_back(dwi.back());                                   // make new copy (next instance)
   ++dwi.back();                                                // json.ns now is partial
  }
  dwi.pop_back();                                               // remove last (->end()) iterator
 }
 hwlk_ = move(ARY{STR{}});                                      // reset hwlk_ to init value
 // json.ns() may hold now the latest updated namespace while wns_ might not even have been
 // updated (e.g.: when walk has ended as <>F, or out of iterations), thus require syncing
 wns_[last_dwi_ptr_].sync_in(json().ns(), map_jnse::NsOpType::NsUpdate);

 is_multi_walk_ = opt()[CHR(OPT_WLK)].hits() > 1 or             // i.e. -w.. -w.., else (one -w..)
                  wpi.size() > 1 or                             // wpi.size > 1, otherwise:
                  (wpi.size() and wpi.front().size() > 1);      // multiple iterators
 DBG(1) {
  DOUT() << "multi-walk: " << (is_multi_walk_? "true": "false") << endl;
  DOUT() << DBG_PROMPT(1) << "interleaved walk-path instances (-w: " << wpi.size() << "):" << endl;
  for(size_t i = 0; i < wpi.size(); ++i)
   DOUT() << DBG_PROMPT(1) << "instance: " << i << ", iterators: " << wpi[i].size() << endl;
 }

 while( any_of(wpi.begin(), wpi.end(), [](deq_jit &wi){ return not wi.empty(); }) )
  process_walk_iterators_(wpi);
}



void Jtc::process_walk_iterators_(deque<deq_jit> &wpi) {
 #include "lib/dbgflow.hpp"
 // build front iterators offset matrix: wpi may contain empty deque
 vector<vector<signed_size_t>> fom(wpi.size());                 // front offset matrix

 size_t longest_walk = build_front_grid_(fom, wpi);             // build FOM here
 vector<size_t> actual_instances;                               // ai: those which not ended yet
 actual_instances.reserve(fom.size());
 for(size_t idx = 0; idx < fom.size(); ++idx)                   // init non-empty fom indices
  if(not fom[idx].empty()) actual_instances.push_back(idx);

 process_offsets_(wpi, fom, longest_walk, actual_instances);
}



size_t Jtc::build_front_grid_(vector<vector<signed_size_t>> &fom, const deque<deq_jit> &wpi) {
 #include "lib/dbgflow.hpp"
 // build matrix from front iterator of each walk (FOM): using iterator's counter() method
 // that way it'll be possible to group relevant walk-paths during output
 size_t longest = 0;                                            // longest row in the fom matrix
 string dlm;                                                    // for debug output only
 for(size_t idx = 0; idx < wpi.size(); ++idx) {                 // go over all given walk paths -w
  auto & wi = wpi[idx];                                         // set of iterators for -w instance
  if(wi.empty()) continue;                                      // no more iterators in this walk

  DBG(3) {
   DOUT() << "walkpath (-w) instance " << idx;                  // go over walk path counters only
   dlm = " front ws_counters matrix: ";                         // of the front iterator's instance
  }
  fom[idx].reserve(wi.front().walk_size());                     // optimize memory management
  for(size_t position = 0; position < wi.front().walk_size(); ++position) {
   if(wi.front().walks()[position].is_directive()) continue;
   fom[idx].push_back( wi.front().counter(position) );
   if(DBG()(3)) { DOUT() << dlm << fom[idx].back(); dlm = ", "; }
  }

  if(fom[idx].empty()) fom[idx].push_back( -1 );                // ensure fom filled for: -w'<vv>v'
  if(DBG()(3)) DOUT() << endl;
  longest = max(longest, fom[idx].size());
 }

 return longest;
}



void Jtc::process_offsets_(deque<deq_jit> &wpi, vector<vector<signed_size_t>> &fom,
                           size_t longest_walk, vector<size_t> &actuals) {
 #include "lib/dbgflow.hpp"
 // scans each offset's row (in wpi) and prints actual (non-empty) and relevant elements
 DBG(2) DOUT() << "walk offsets (longest " << longest_walk << ")";

 size_t grouping = 0,                                           // group size (of lowest offsets)
        lf_counter = SIZE_T(-1);                                // lowest front (ws) counter
 for(size_t offset = 0; offset < longest_walk; ++offset) {      // go across all offsets
  map<size_t, size_t> pos_ai, neg_ai;                           // build new actuals in here
  signed_size_t lowest_offset = LONG_MAX, cnt = 0;              // helpers to build new actuals
  if(DBG()(2)) DOUT() << " / [" << offset << "]:";

  for(auto ai: actuals) {                                       // act.inst. are with lowest offset
   if(offset >= fom[ai].size() or fom[ai][offset] < 0)          // negative or short actuals offset
    { neg_ai[cnt++] = ai; continue; }                           // collected separately
   if(DBG()(2)) DOUT() << " " << ai;
   if(fom[ai][offset] < lowest_offset)                          // found a lower counter
    { lowest_offset = fom[ai][offset]; pos_ai.clear(); }        // clear prior found actuals
   if(fom[ai][offset] == lowest_offset)                         // update new actuals
    pos_ai[cnt++] = ai;
  }
  if(lf_counter == SIZE_T(-1) and not pos_ai.empty())
   lf_counter = lowest_offset;

  actuals.clear();                                              // clear and rebuild actuals
  for(signed_size_t idx = 0; idx < cnt; ++idx) {
   IF_FOUND(pos_ai, idx) actuals.push_back(FITR->VALUE);
   else
    { IF_FOUND(neg_ai, idx) actuals.push_back(FITR->VALUE); }
  }
  grouping = actuals.size();                                    // update groping
 }

 if(DBG()(2)) DOUT() << endl;                                   // close debug line
 if(actuals.empty())                                            // should never be the case
  { wpi.clear(); return; }                                      // in case, avoiding endless loop
 DBG(2) DOUT() << "output instance: " << actuals.front()
               << ", #lowest offsets/counter: {" << grouping << ", "
               << (lf_counter == SIZE_T(-1)? "lowest": to_string(lf_counter)) << "}" << endl;
 (this->*subscriber_)(wpi[actuals.front()].front(), {grouping, lf_counter});
 wpi[actuals.front()].pop_front();
}



// console_output_() is similar to write_json() in function, but there are some differences:
//   = takes care of printing size (-z or -zz)
//   = stringifying and jsonizing jsons/strings respectively
//   - but does not facilitate -j/-J options (that is done by walk)
//   + facilitate labeled print (-l)
//   + prints either walked iterator's value (-w), or respective template-interpolated value (-T)
//
// thus: console_output_() is used to print only walked (-w) elements, while write_json()
//       outputs entire (final) JSONs (like after -i/-u/-s/-p, or with -j/-J)

void Jtc::console_output_(Json::iterator &wi, Json &jtmp_, Grouping unused) {
 #include "lib/dbgflow.hpp"
 // no -j given, print out element pointed by iter wi
 // there are 3 source to chose from: 1. template (jtmp_); 2. current walk value (*wi);
 // 3. if -ll given then and conditions met (glean_lbls), then either 1. or 2. to be iterated
 auto & src = jtmp_.type() == Jnode::Neither? *wi: jtmp_.root();
 bool glean_lbls{glean_lbls_ and src.is_object() and src.has_children()};
 Jtc::mod_ |= ios_base::app;                                    // next time will append
 size_t size{0};

 if(size_only_)                                                 // -zz
  { xout_ << wi->size() << endl; return; }

 Jnode dummy = ARY{nullptr};
 for(auto itl = (glean_lbls? src: dummy).begin(); itl != (glean_lbls? src: dummy).end(); ++itl) {
  auto & srr = glean_lbls? *itl: src;                           // source reference

  if(opt()[CHR(OPT_LBL)] and srr.has_label())                   // -l given then print label
   { xout_ << CHR_DQTE << srr.label() << CHR_DQTE << ": "; unquote_ = false; }  // if present
  if(unquote_ and srr.is_string())                               // don't try collapsing it into
   { if(not srr.str().empty()) xout_ << json().unquote_str(srr.str()) << endl; }
  else {
   if(inquote_)
    xout_ << CHR_DQTE << json().inquote_str(srr.to_string(Jnode::Raw)) << CHR_DQTE << endl;
   else xout_ << srr << endl;                                    // a single operation!
  }
  if(measure_) size += srr.size();
 }

 if(measure_)                                                    // -z given
  xout_ << OBJ{LBL{JTCS_TKN, static_cast<double>(size)}} << endl;
}



void Jtc::jsonized_output_(Json::iterator &wi, Json &jtmp_, Grouping grp) {
 #include "lib/dbgflow.hpp"
 // demux output based on jout_ state: either to ARY or OBJ (-j or -jj respectively)
 typedef void (Jtc::*jo_ptr)(Json::iterator &, Json &, Grouping);
 static jo_ptr jsonize[2] = {&Jtc::jsonized_output_obj_, &Jtc::jsonized_output_ary_};

 (this->*jsonize[jout_.is_array()])(wi, jtmp_, grp);
}



void Jtc::jsonized_output_obj_(Json::iterator &wi, Json &jtmp_, Grouping unused) {
 #include "lib/dbgflow.hpp"
 // if -jj option given, output into jout_ as Object (items w/o label are ignored)
 Jnode::iterator itl;                                           // to facilitate -ll

 auto & src = jtmp_.type() == Jnode::Neither? *wi: jtmp_.root();
 bool glean_lbls{src.is_object() and src.has_children() and glean_lbls_};
 bool collect_clashing = opt()[CHR(OPT_MDF)].hits() % 2 == 1;

 auto collect = [&](Jnode & sr) {
  auto found = jout_.find(sr.label());
  if(found == jout_.end() or not collect_clashing)              // it's a new label & -m not given
   { jout_[sr.label()] = sr; return; }                          // copy supernode
                                                                // label already exist
  if(c2a_.count(sr.label()) == 0) {                             // and not converted to array yet
   jout_[sr.label()] = ARY{ jout_[sr.label()] };
   c2a_.emplace(sr.label());
  }
  jout_[sr.label()].push_back(sr);
 };

 Jnode dummy = ARY{nullptr};
 for(auto itl = (glean_lbls? src: dummy).begin(); itl != (glean_lbls? src: dummy).end(); ++itl) {
  auto & srr = glean_lbls? *itl: src;                           // source reference
  if(not srr.has_label()) continue;                             // no label - ignore
  collect(srr);
 }
}



void Jtc::jsonized_output_ary_(Json::iterator &wi, Json &jtmp_, Grouping grp) {
 #include "lib/dbgflow.hpp"
 // if -j option given, output into jout_ as Array
 auto create_obj = [&]{ return opt()[CHR(OPT_SEQ)].hits() == 0?  // see if new obj required
                                grp > last_: opt()[CHR(OPT_SEQ)].hits() < 2; };
 auto & src = jtmp_.type() == Jnode::Neither? *wi: jtmp_.root();
 bool glean_lbls{src.is_object() and src.has_children() and glean_lbls_};

 Jnode dummy = ARY{nullptr};
 for(auto itl = (glean_lbls? src: dummy).begin(); itl != (glean_lbls? src: dummy).end(); ++itl) {
  auto & srr = glean_lbls? *itl: src;                           // source reference

  if(not opt()[CHR(OPT_LBL)])                                   // -l not given, make simple array
   { jout_.push_back(srr); continue; }
                                                                // -l given (combine relevant grp)
  if(not srr.has_label())                                       // srr has no label, push to back
   { jout_.push_back(srr); continue; }
                                                                // srr has label, merge
  if(create_obj() or jout_.is_empty())                          // time to create a new object
   { jout_.push_back( OBJ{} ); convert_req_ = false; }
  if(not jout_.back().is_object())
   jout_.push_back( OBJ{} );

  if(jout_.back().count(srr.label()) == 0) {                    // no label recorded (first time)
   jout_.back()[srr.label()] = srr;                             // copy supernode
   if(srr.is_array()) convert_req_ = true;                      // if subsequent update, will conv.
   continue;
  }
                                                                // label already exist
  if(convert_req_ or not jout_.back()[srr.label()].is_array()) {// convert to array then
   Json tmp{ move(jout_.back()[srr.label()]) };
   (jout_.back()[srr.label()] = ARY{}).push_back( move(tmp) );
   convert_req_ = false;
  }
  jout_.back()[srr.label()].push_back( srr );                   // & push back into converted array
 }
}



//
// Standalone calls
//
#define SHELL_SPECIAL "~`#$&*()\\|{};<>?!. \t"

string sh_quote_str(const string &src) {
 // quote src: quote chars (which due to be quoted) outside of literals only
 // this routine is bash shell oriented
 string quoted;
 char inside_literal = CHR_NULL;
 short backslash_seen = 0;
 for(auto chr: src) {
  if(not backslash_seen)
   if(chr == CHR_RSLD) backslash_seen = 1;

  if(chr AMONG(CHR_SQTE, CHR_DQTE)) {
   if(not backslash_seen) {                                     // not quoted ' or "
    if(chr == inside_literal)                                   // inside literal right now
     inside_literal = CHR_NULL;                                 // exit literal (go outside)
    else
     if(inside_literal == CHR_NULL)                             // outside of literal
      inside_literal = chr;                                     // go inside literal w. given char
   }
  }
  else                                                          // not ' or "
   if(inside_literal == CHR_NULL) {                             // outside of literal, quote some
    if(not isalnum(chr)) quoted += CHR_RSLD;
    if(strchr(SHELL_SPECIAL, chr) != nullptr)                   // don't quote those
     quoted.pop_back();
   }
  quoted += chr;

  if(backslash_seen)
    { if(++backslash_seen > 2) backslash_seen = 0; }
 }
 return quoted;
}





















