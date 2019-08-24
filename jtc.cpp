#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <set>
#include <climits>      // LONG_MAX
#include <algorithm>
#include "lib/getoptions.hpp"
#include "lib/Json.hpp"
#include "lib/Streamstr.hpp"
#include "lib/shell.hpp"
#include "lib/dbg.hpp"

using namespace std;


#define VERSION "1.73"
#define CREATOR "Dmitry Lyssenko"
#define EMAIL "ldn.softdev@gmail.com"

// jtc: fast a powerful utility to manipulate json format
//
// jtc main features:
//  o walk-path: a set of lexemes (subscripts, searches, directives) which define how
//    jtc should walk the JSON tree (+ supported REGEX)
//  o cache: most of the lexemes (all except few dynamic types) cached when walked,
//    cache facilitates super fast walking even huge JSON trees
//  o namespaces: let "memorizing" and reusing memorized JSON values later for interpolation
//  o templating: let produce custom json out of static and previously memorized values
//    in the namespace, allows jsonizing stringified JSONs and stringify JSONs
//  o buffered and streamed inputs: the former provides a fast read and parsing, the latter
//    facilitates on-fly json manipulation
//  and many more other features


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

// facilitate option materialization
#define STR(X) XSTR(X)
#define XSTR(X) #X
#define CHR(X) XCHR(X)
#define XCHR(X) *#X

#define NULL_CHR '\0'
#define RSLD_CHR '\\'
#define NLNE_CHR '\n'
#define CRTN_CHR '\r'
#define SQTE_CHR '\''
#define DQTE_CHR '"'
#define SPCE_CHR ' '

#define CMP_BASE "json_1"
#define CMP_COMP "json_2"
#define SIZE_PFX "size: "
#define WLK_HPFX "$?"

#include "lib/jtc_guide.hpp"

// various return codes
#define RETURN_CODES \
        RC_OK, \
        RC_UNUSED, \
        RC_WP_INV, \
        RC_SC_MISS, \
        RC_CMP_NEQ, \
        RC_END
ENUM(ReturnCodes, RETURN_CODES)

// return code exception offsets
#define OFF_GETOPT RC_END                                       // offset for Getopt exceptions
#define OFF_JSON (OFF_GETOPT + Getopt::end_of_throw)            // offset for Json exceptions
#define OFF_REGEX (OFF_JSON + Jnode::end_of_throw)              // offset for Regex exceptions

// simple macro for to expose class aliases
// usage: REVEAL(jtc, opt, json)
#define __REVEAL_TKN1__(X,Y) X ## Y
#define __REVEAL_TKN2__(X,Y) __REVEAL_TKN1__(X, Y)
#define __REFX__(A) auto & A = __REVEAL_TKN2__(__reveal_class__,__LINE__).A();
#define REVEAL(X, ARGS...) \
        auto & __REVEAL_TKN2__(__reveal_class__, __LINE__) = X; \
        MACRO_TO_ARGS(__REFX__, ARGS)

#define GLAMBDA(FUNC) [this](auto&&... arg) { return FUNC(std::forward<decltype(arg)>(arg)...); }

#define DBG_WIDTH 67                                            // max print len upon parser's dbg
#define KEY first                                               // semantic for map's pair
#define VALUE second                                            // instead of first/second
#define OPN first                                               // range semantic
#define CLS second                                              // instead of first/second


typedef vector<string> v_string;





class CommonResource {
 // the class facilitates -J option and global namespace:
 // caters user opt (which is copied to each Jtc instance), input string
 // and json for jsonization of all processed/walked jsons

    typedef ptrdiff_t signed_size_t;

    struct Multiwalk {
        // a trivial class to pair up the walk multiplier and the first index to display:
        // only each factor'th walk will be displayed after offset's
                            Multiwalk(void) = default;
                            Multiwalk(size_t x1, signed_size_t x2):    // emplacement
                             factor{x1}, offset{x2} { }
        size_t              factor;                             // display every factor'th walk
        signed_size_t       offset;                             // starting from offset (0 based)
    };

 public:
    int                 rc(void) const { return rc_; }
    void                rc(int rc) { rc_ = rc; }
    Getopt &            opt(void) { return opt_; };             // access to Getopt class instance
    size_t              opt_e_found(void) { return opt_e_found_; }  // used for recompile once
    char                opt_ui(void) {  return opt_ui_; };      // -i or -u for recompile
    Json::map_jn &      global_ns(void) {  return gns_; };      // access to global namespaces
    Streamstr &         iss(void) {  return iss_; };            // access to stream class
    Json &              global_json(void) { return gjns_; };    // access to global JSON (-J)
    auto &              wm(void) { return wm_; };

    void                parse_opt(int argc, char *argv[]);
    Streamstr &         read_inputs(void);
    void                jsonize(Json jout);

 private:
    bool                is_recompile_required_(int argc, char *argv[]);
    void                recompile_args_(v_string &args, v_string &new_args);
    void                parse_rebuilt_(v_string & new_args);
    void                convert_xyw_(void);
    bool                is_x_factor_(const char *str, signed_size_t * recursive = nullptr);

    Json                gjns_{ ARY{} };                         // global json (facilitates -J)
    Json::map_jn        gns_;                                   // global namespaces
    Streamstr           iss_;                                   // input Streamstr
    Getopt              opt_;                                   // user options
    vector<Multiwalk>   wm_;                                    // walk factor: facilitate -xN/M
    int                 rc_{RC_OK};
    size_t              opt_e_found_{0};                        // used for recompile once -e found
    char                opt_ui_{NULL_CHR};                      // either -i or -u for recompile
    bool                aimp_{false};                           // '-a' imposed?
    bool                jimp_{false};                           // '-j' imposed?

 public:
    DEBUGGABLE(iss_, gjns_)
};





class Jtc {
    struct BoundJit {
     // a trivial class to pair up destination walks (-w) with source walks (-i/u/c)
     // and respective name-spaces
                            BoundJit(void) = default;
                            BoundJit(Json::iterator d, Json::iterator s):   // for emplacement
                             dst{d}, src{s}, ns{src.json().ns()}, lbl{"\n"} {}

        bool                update_lbl(void)
                             { return lbl.empty() or lbl.front() != '\n'; }
        void                reset_lbl(void) { lbl = "\n"; }

        Json::iterator      dst;                                // -w walks (iterators) go here
        Json::iterator      src;                                // -i/u/c walks (iterators) go here
        Json::map_jn        ns;                                 // NS from respective -i/u/c iters.
        string              lbl{"\n"};
    };

    struct Grouping {
        // facilitates a pair of group/counter values, used in jsonized_output_ary_
                            Grouping(void) = default;
                            Grouping(size_t x1, size_t x2):     // emplacement
                             group{x1}, counter{x2} { }
        bool                operator>(const Grouping &rhs)
                             { return group > rhs.group and counter > rhs.counter; }
        size_t              group{0};
        size_t              counter{0};
    };

    typedef ptrdiff_t signed_size_t;
    typedef vector<Json::iterator> vec_jit;
    typedef vector<BoundJit> vec_bjit;
    typedef deque<Json::iterator> deq_jit;
    typedef void (Jtc::*mptr)(Json::iterator &wi, Grouping grp);
    typedef map<size_t, Json> map_json;
    typedef map<size_t, Json::iterator> map_jit;
    typedef vector<size_t> vec_wlk;
    typedef map<const Json::iterator*, Json::map_jn> map_ns;

    #define MERGEOBJ \
                preserve,   /* clashing labels of merged object not overwritten */ \
                overwrite   /* clashing labels of merged objects overwritten */
    ENUM(MergeObj, MERGEOBJ)

    #define JITSRC          /* type of source for iterations in -i/u/c operation*/\
                src_input,  /* source is input JSON: `jinp_`: -u<walk> walks input JSON here*/\
                src_jexec,  /* source is in `jexc_`: 1) -eu ... \; 2) -u<STATIC> -u<walk> */\
                src_optarg  /* source is in `jsrc_`: when -u<STATIC>, i.e., w/o walks is used*/
    ENUMSTR(Jitsrc, JITSRC)


 public:
                        Jtc(void) = delete;
                        Jtc(CommonResource & cr): cr_{cr}, opt_{cr.opt()} {
                         ecli_ = opt_[CHR(OPT_EXE)].hits() > 0; // flag used by -i/-u options
                         merge_ = opt_[CHR(OPT_MDF)].hits() > 0;// flag used by -i/-u options
                         is_tmp_per_walk_ = opt_[CHR(OPT_TMP)].hits() > 1 and
                                            opt_[CHR(OPT_SEQ)].hits() < 2 and
                                            opt_[CHR(OPT_TMP)].hits() == opt_[CHR(OPT_WLK)].hits();
                         jinp_.tab(abs(opt_[CHR(OPT_IND)]))     // prepare json_
                              .raw(opt_[CHR(OPT_RAW)])
                              .quote_solidus(opt_[CHR(OPT_QUT)].hits() % 2 == 1);

                         jinp_.callback(GLAMBDA(shell_callback_)).engage_callbacks();
                         jexc_.type(Jnode::Neither);
                        }


    // expose private objects
    auto &              opt(void) { return opt_; }
    auto &              json(void) { return jinp_; }
    auto &              jout(void) { return jout_; }

    // user methods
    void                parsejson(Streamstr::const_iterator & jsp);
    void                write_json(Json & jsn, bool jsnize = true);
    void                demux_opt(void);
    void                compare_jsons(void);
    void                compare_bingings(void);
    void                upsert_json(char op);
    void                collect_itr_bindings(Json::iterator &it, Grouping = {0,0});
    void                update_by_iterator(Json::iterator &it, Grouping = {0,0});
    bool                advance_to_next_src(signed_size_t i = -1);
    void                apply_src_walks(char op);
    void                purge_json(void);
    void                swap_json(void);
    void                walk_json(void);
    void                output_by_iterator(Json::iterator &wi, Grouping = {0, 0});
    void                output_by_iterator(Json::iterator &&wi, Grouping grp = {0, 0})
                         { output_by_iterator(wi, grp); }

 private:
    void                location_(Streamstr::const_iterator & start);
    void                ready_params_(char option);
    void                maybe_update_lbl_(void);
    void                compare_jsons_(const Jnode &, set<const Jnode*> &,
                                       const Jnode &, set<const Jnode*> &);
    void                merge_jsons_(Json::iterator &dst, Json::iterator src, string *lbl);
    void                merge_into_object_(Jnode &dst, const Jnode &src, MergeObj mode);
    void                merge_into_array_(Jnode &dst, const Jnode &src, MergeObj mode);
    void                update_jsons_(Json::iterator &dst, Json::iterator src, string *lbl);
    bool                processed_by_cli_(Json::iterator &it);
    bool                execute_cli_(Json &update, Json::iterator &jit, const Json::map_jn &ns);
    string              reconcile_ui_(Json::iterator &jit, const Json::map_jn &ns);
    void                crop_out_(void);
    bool                remove_others_(set<const Jnode*> &ws, Jnode &jn);
    vec_jit             collect_walks_(const string &walk_path);
    void                walk_interleaved_(void);
    void                update_wlk_history_(Json::iterator &);
    void                process_walk_iterators_(deque<deq_jit> &walk_iterators);
    size_t              build_front_grid_(vector<vector<signed_size_t>> &, const deque<deq_jit> &);
    void                process_offsets_(deque<deq_jit> &, vector<vector<signed_size_t>> &,
                                         size_t, vector<size_t> &);
    void                console_output_(Json::iterator &, Json &jref, Grouping unused);
    void                jsonized_output_(Json::iterator &, Json &jref, Grouping unused);
    void                jsonized_output_obj_(Json::iterator &, Json &jref, Grouping);
    void                jsonized_output_ary_(Json::iterator &, Json &jref, Grouping);
    void                merge_ns_(Json::map_jn &to, const Json::map_jn &from)
                         { for(const auto & ns: from) to[ns.KEY] = ns.VALUE; }
    bool                shell_callback_(const std::string &lxm, Json::iterator jit) {
     // reminder:
     //typedef std::function<bool(const std::string &, const Jnode &)> uws_callback;
                         Json tmp;
                         DBG().increment(+2, tmp, -2);
                         tmp = Json::interpolate(lxm, jit, jit.json().ns(), Json::dont_parse);
                         sh_.system( tmp.str() );
                         return sh_.rc() == 0;
                        }

    // private member types:
    CommonResource &    cr_;
    Getopt              opt_;

    Json                jinp_;                                  // input JSON for jtc
    Json                jout_;                                  // json output (-j, -jj)
    Json                jexc_;                                  // json for -e (or mixed args use)
     // jexc_ is used to facilitates sources of -e[iuc] arguments and -[iuc] for mixed arguments
     // usage, e.g: -i<json> -i<wlk1> -i<wlk2> ... (in both use-cases only 1 json source allowed)
    map_json            jsrc_;                                  // static JSONs -iuc (no mixed arg)
     // jsrc_ facilitates cases when -[iuc] contain json only (no walks), multiple allowed
    vec_bjit            psrc_;                                  // binding dst walks with src's
    vec_wlk             wsrc_;                                  // enlist all walks in -[iuc]<walk>
    Json::iterator      jits_;                                  // current source iterator
    Jitsrc              jitt_{Jitsrc::src_input};               // source of jits_
    size_t              wcur_{0};                               // current walk in wsrc_
    map_ns              wns_;                                   // namespaces for walked (-w) paths
    Jnode               hwlk_{ARY{STR{""}}};                    // walks history init value

    set<string>         c2a_;                                   // used in jsonized_output_obj_()
    map<size_t, string> tpw_;                                   // tmp per walk, output_by_iterator
    bool                is_tmp_per_walk_;                       // templates pertain to walks
    bool                is_tmp_per_upst_;                       // templates pertain to walks, -i/u
    bool                is_multi_walk_{false};                  // multiple -w or single iterable?
    bool                convert_req_{false};                    // used in output_by_iterator
    bool                ecli_{false};                           // -e status for insert/update
    bool                merge_{false};                          // -m status for insert/update
    bool                lbl_update_{false};                     // label update operation detected
    Grouping            last_;                                  // used in output_by_iterator
    size_t              key_{0};                                // general purpose counter
    size_t              upst_key_{0};                           // template idx for -iu round-robin
    size_t              wcnt_{0};                               // counts number of walks
    mptr                subscriber_;                            // method ptr for output processor
    Shell               sh_;

 public:

    DEBUGGABLE(jinp_, jout_, jexc_, sh_)
};

#undef MERGEOBJ
STRINGIFY(Jtc::Jitsrc, JITSRC)
#undef JITSRC


// list of standalone callse
string sh_quote_str(const string &src);





int main(int argc, char *argv[]) {

 CommonResource cr;
 REVEAL(cr, opt)

 opt.prolog("\nJSON test console\nVersion " VERSION \
            ", developed by " CREATOR " (" EMAIL ")\n");
 opt[CHR(OPT_ALL)].desc("process all inputs (by default only the first JSON processed); -"
                        STR(OPT_FRC) " is ignored");
 opt[CHR(OPT_CMN)].desc("a common part of a path, prepended to every followed -" STR(OPT_PRT)
                        " option").name("common_wp");
 opt[CHR(OPT_CMP)].desc("compare JSONs: display delta between given JSONs").name("f|j|w");
 opt[CHR(OPT_DBG)].desc("turn on debugs (multiple calls increase verbosity)");
 opt[CHR(OPT_EXE)].desc("make option parameters for -" STR(OPT_INS) ", -" STR(OPT_UPD)
                        " undergo a shell evaluation; see -" STR(OPT_GDE) " for more info");
 opt[CHR(OPT_FRC)].desc("apply changes into the file (instead of printing resulting JSON"
                        " to stdout)");
 opt[CHR(OPT_GDE)].desc("mini USER-GUIDE: explain walk path syntax, usage notes, some examples");
 opt[CHR(OPT_IND)].desc("indent for pretty printing").bind("3").name("indent");
 opt[CHR(OPT_INS)].desc("insert either a static JSON, or pointed by a walk-path; see with -"
                        STR(OPT_GDE) " for more").name("f|j|w");
 opt[CHR(OPT_JAL)].desc("wrap all processed JSONs into an array (option -"
                        STR(OPT_ALL) " assumed, buffered read imposed)");
 opt[CHR(OPT_JSN)].desc("wrap walked JSON elements into a JSON array (-" STR(OPT_JSN) STR(OPT_JSN)
                        " wrap into a JSON object)");
 opt[CHR(OPT_LBL)].desc("print labels (if present) for walked JSON; together with -"
                        STR(OPT_JSN) " wrap into objects");
 opt[CHR(OPT_MDF)].desc("modifier: toggle merging for options -" STR(OPT_INS) ", -" STR(OPT_UPD)
                        "; see with -" STR(OPT_GDE) " for more info");
 opt[CHR(OPT_PRG)].desc("purge all walked JSON elements (-" STR(OPT_PRG) STR(OPT_PRG)
                        ": purge all elements except walked)");
 opt[CHR(OPT_PRT)].desc("an individual partial path, prepended by preceding -" STR(OPT_CMN)
                        " option").name("partial_wp");
 opt[CHR(OPT_QUT)].desc("enforce strict quoted solidus parsing "
                        "(-qq: unquote isolated JSON strings)");
 opt[CHR(OPT_RAW)].desc("print JSON in a raw (compact, one-line) format"
                        " (-rr stringify resulting JSON)");
 opt[CHR(OPT_SEQ)].desc("do not print/process walks interleaved (i.e. print/process all walks "
                        "sequentially)");
 opt[CHR(OPT_SWP)].desc("swap around JSON elements pointed by a pair of walks");
 opt[CHR(OPT_SZE)].desc("print size (number of nodes in JSON) at the end of output (-"
                        STR(OPT_SZE) STR(OPT_SZE) " prints size only)");
 opt[CHR(OPT_TMP)].desc("a template to interpolate and apply upon -" STR(OPT_INS) ", -" STR(OPT_UPD)
                        " and standalone -" STR(OPT_WLK) " operations").name("template");
 opt[CHR(OPT_UPD)].desc("update static JSON, or pointed by a walk-path; see with -"
                        STR(OPT_GDE) " for more").name("f|j|w");
 opt[CHR(OPT_WLK)].desc("a standalone walk path (multiple may be given); see with -"
                        STR(OPT_GDE) " for more").name("walkpath");
 opt[0].desc("file to read json from").name("json_file").bind("<stdin>");

 opt.epilog(R"(
this tool provides ability to:
 - parse, validate and display JSON (in a compact and pretty formats)
 - walk about input JSON using various subscript and search criteria (see with -)" STR(OPT_GDE)
 R"( for more)
 - manipulate JSON via purge/insert/copy/merge/update/replace/move/swap/interpolate operations
 - compare JSONs (print diffs)

by default, input JSONs processed via buffered read (first read, then parse); streamed read (i.e.
parse JSON immediately as data arrives) is engaged when option -)"
STR(OPT_ALL) R"( given and <stdin> input selected
(though -)" STR(OPT_JAL) R"( overrides the streamed read and reverts to buffered)

for walk-path explanation, usage notes and examples run with -)" STR(OPT_GDE) R"(
for a complete user guide visit https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md
)");
 opt.variadic();


 // parse options
 cr.parse_opt(argc, argv);

 DEBUGGABLE()
 DBG().use_ostream(cerr)
      .level(opt[CHR(OPT_DBG)]);

 // read json (ready stream buffer, or read file/cin)
 Streamstr::const_iterator jsp = cr.read_inputs().begin();      // global parse pointer
 Streamstr::const_iterator pse;                                 // parsed json end iterator

 // execute as per read options
 try {
  do {
   Jtc jtc(cr);
   jtc.parsejson(jsp);
   jtc.demux_opt();
   pse = jsp;                                                   // to verify exception
  } while(jsp != cr.iss().end() and opt[CHR(OPT_ALL)].hits() > 0);
 }
 catch(Jnode::stdException & e) {
  DBG(1) DOUT() << "exception raised by: " << e.where() << endl;
  cerr << opt.prog_name() << " jnode exception: " << e.what() << endl;
  cr.rc(e.code() + OFF_JSON);
 }
 catch(Json::stdException & e) {
  DBG(1) DOUT() << "exception raised by: " << e.where() << endl;    // might need suppression
  if(e.code() == Jnode::unexpected_end_of_string and jsp.is_streamed()) {
   for(;pse.offset() < jsp.offset(); ++pse) if(*pse > ' ') break;
   if(pse.offset() == jsp.offset()) {
    DBG(1) DOUT() << "suppressing exception: blank trails past last parsed json" << endl;
    exit(cr.rc());                                              // provide normal exit then
   }
  }
  cerr << opt.prog_name() << " json exception: " << e.what() << endl;
  cr.rc(e.code() + OFF_JSON);
 }
 catch(std::regex_error & e) {
  cerr << "regexp exception: " << e.what() << endl;
  cr.rc(e.code() + OFF_REGEX);
 }

 if(cr.global_json().empty()) exit(cr.rc());

 Jtc jtc(cr);                                                   // global (-J) resulting json
 for(const char *o = STR(OPT_JAL) STR(OPT_JSN) STR(OPT_QUT) STR(OPT_RAW); *o != NULL_CHR; ++o)
  jtc.opt()[*o].reset();                                        // above options to be ignored
 jtc.write_json(cr.global_json());
 exit(cr.rc());
}



//
// CR PUBLIC methods definitions
//
void CommonResource::parse_opt(int argc, char *argv[]) {
 // 1. parse options, if option -e detected, rebuild -u/i's arguments and parse with rebuilt args
 // 2. reinstate -w options from -x/-y
 // 3. process options dependencies
 v_string args{argv, argv + argc};

 if(is_recompile_required_(argc, argv)) {                       // re-compiling required?
  v_string new_args;                                            // newly rebuilt args go here
  recompile_args_(args, new_args);                              // rebuild -u/i's arguments as one
  parse_rebuilt_(new_args);
 }
 else                                                           // parse normally
  try { opt_.parse(argc, argv); }
  catch(Getopt::stdException &e)
   { opt_.usage(); exit(e.code() + OFF_GETOPT); }

 convert_xyw_();                                                // -w = -x + -y...

 if(opt_[CHR(OPT_GDE)].hits() > 0) exit(print_guide());         // -g, print guide
 if(opt_[CHR(OPT_JAL)].hits() > 0) {                            // -J:
  if(opt_[CHR(OPT_ALL)].hits() == 0) aimp_ = true;              // indicate that -a was imposed
  opt_[CHR(OPT_ALL)].hit();                                     // ensure -a hit unconditionally
  if(opt_[CHR(OPT_JSN)].hits() == 0)                            // if no -j given
   { opt_[CHR(OPT_JSN)].hit(); jimp_ = true; }                  // impose one and remember
 }
 if(opt_[CHR(OPT_ALL)].hits() > 0 and opt_[CHR(OPT_FRC)].hits() > 0) {
  cerr << "notice: ignoring option -" STR(OPT_FRC) " b/c of multi-input processing" << endl;
  opt_[CHR(OPT_FRC)].reset();                                   // -a (-J), ensure -f ignored
 }
}



Streamstr & CommonResource::read_inputs(void) {
 // initialize `iss` with correct read mode: streamed/cin/buffered
 bool read_from_cin{opt_[0].hits() == 0 or opt_[CHR(OPT_RDT)].hits() > 0}; // no arg, or forced '-'
 DBG(0)
  DOUT() << "reading json from " << (read_from_cin? "<stdin>": "file-arguments:") << endl;

 for(int arg = 1; arg <= opt_.arguments(); ++arg) {
  iss_.source_file(opt_[0].str(arg));                           // will set read mode to file
  DBG(0) DOUT() << "source file: " << opt_[0].str(arg) << endl;
 }
 if(opt_.arguments() > 1) opt_[CHR(OPT_ALL)].hit();             // impose -a if multiple files

 if(read_from_cin) {                                            // either stream, or buffered
  if(opt_[CHR(OPT_ALL)].hits() > 0 and opt_[CHR(OPT_JAL)].hits() == 0)  // -a, no -J, stream read
   iss_.reset(Streamstr::streamed_cin);
  else {                                                        // cin buffered
   if(opt_[CHR(OPT_JAL)].hits() > 0 and aimp_ == false)         // -J given and -a wasn't imposed
    cerr << "notice: option -" STR(OPT_JAL) " cancels streaming input" << endl;
   iss_.reset(Streamstr::buffered_cin);
  }
 }
 return iss_;
}



void CommonResource::jsonize(Json jout) {
 // put all walked json results into a global json
 if(jimp_ and jout.is_iterable()) {                             // -j was imposed
  for(auto &jn: jout) gjns_.push_back(move(jn));                // therefore push one by one
  return;
 }
                                                                // no -j were imposed:
 gjns_.push_back(move(jout));                                   // move jout as it is
}



//
// CR PRIVATE methods definitions
//
bool CommonResource::is_recompile_required_(int argc, char *argv[]) {
 // check if option -e is present in the arguments (if so, indicate re-parsing is required)
 opt_.suppress_opterr(true);
 try { opt_.parse(argc, argv); }
 catch(Getopt::stdException &e) { }

 bool rr = opt_[CHR(OPT_EXE)];
 opt_.reset().suppress_opterr(false);
 return rr;
}



void CommonResource::recompile_args_(v_string & args, v_string &new_args) {
 // recompile argv minding -u/i's arguments, put re-parsed args into new_args
 bool semicolon_found = false;

 for(auto &arg: args) {                                         // go through all args
  if(semicolon_found)                                           // -i/u already found and processed
   { new_args.push_back(arg); continue; }                       // push arg w/o any processing

  if(opt_e_found_ > 0) {                                        // facing -i/u; ';' not found yet,
   if(arg.back() == ';')                                        // ';' found
    { semicolon_found = true; arg.pop_back(); }                 // trim trailing ';'
   if(not arg.empty()) {
    if(opt_e_found_++ == 1) new_args.back() += arg;             // first argument
    else new_args.push_back(string("-") + opt_ui_ + arg);       // any subsequent arg
   }
   continue;
  }

  if(arg.front() == '-')                                        // opt, see if opt -i/u is present
   for(const char &chr: arg) {
    if(&chr == &arg[0]) continue;                               // skip first char '-'
    if(not opt_.defined(chr)) break;                            // undefined option, process arg
    if(chr == CHR(OPT_UPD) or chr == CHR(OPT_INS)) {            // opt -i/u found, indicate&record
     opt_e_found_ = 1;
     opt_ui_ = chr;
     if(arg.back() == ';')                                      // ';' found
      { semicolon_found = true; arg.pop_back(); ++opt_e_found_; }
     else
      if(&arg.back() != &chr)                                   // arg is attached to the option
       ++opt_e_found_;
     break;
    }
   }
  new_args.push_back(arg);
 }

 if(opt_e_found_ > 0 and not semicolon_found) {
  cerr << "fail: don't see parameter termination of '-" << opt_ui_ << "' - `\\;'" << endl;
  exit(RC_SC_MISS);
 }
}



void CommonResource::parse_rebuilt_(v_string & new_args) {
 // parse rebuilt arguments (after recompilation for -e option)
 char *nargv[new_args.size()];                                  // here, build a new argv
 for(size_t i = 0; i < new_args.size(); ++i) {
  nargv[i] = new char[new_args[i].size()+1];
  stpcpy(nargv[i], new_args[i].c_str());
 }

 try { opt_.reset().parse(new_args.size(), nargv); }            // re-parse newly rebuilt args
 catch(Getopt::stdException & e)
  { opt_.usage(); exit(e.code() + OFF_GETOPT); }

 for(size_t i = 0; i < new_args.size(); ++i)                    // clean up nargv now
  delete [] nargv[i];
}



void CommonResource::convert_xyw_(void) {
 // convert -x, -y options into -w:
 // standalone (isolated) -x and -y also converted to -w
 string last_x, last_y;
 v_string new_w;                                                // record new -w options here

 for(auto &option: opt_.ordinal()) {                            // go by options order
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
  opt_[CHR(OPT_WLK)] = opt_w;
}



bool CommonResource::is_x_factor_(const char * x_str, signed_size_t * recursive) {
 // process -xN/N option here: notation -x/N is allowed, all other forms return `false`
 signed_size_t x1, x2;
 char *endptr;
 x1 = strtol(x_str, &endptr, 10);

 if(recursive)                                                  // resolving a second value now
  { if(*endptr == '\0')
   { *recursive = x1; return true; } return false; }

 x2 = abs(x1) - 1;
 if(*endptr == '/' and endptr[1] != '\0')
  { if(not is_x_factor_(endptr + 1, &x2)) return false; }       // resolve second value here
 else
  if(*endptr != '\0') return false;

 wm_.emplace_back(abs(x1), x2);
 return true;
}



//
// Jtc PUBLIC methods definitions
//
void Jtc::parsejson(Streamstr::const_iterator & jsp) {
 // parse read json text via Streamstr iterator
 Streamstr::const_iterator jbegin = jsp;                        // for debugs / location_ only

 try
  { jinp_.parse(jsp, cr_.iss().is_streamed()? Json::relaxed_no_trail: Json::relaxed_trailing); }
 catch(Json::stdException & e) {
  if(e.code() >= Jnode::start_of_json_parsing_exceptions and
     e.code() <= Jnode::end_of_json_parsing_exceptions)
   DBG(0) location_(jbegin);
  throw e;
 }

 if(opt_[CHR(OPT_WLK)].hits() == 0 and opt_[CHR(OPT_TMP)].hits() >= 1)  // no -w, but -T is given
  opt_[CHR(OPT_WLK)] = "[^0]";                                  // provide default walk off root
}



void Jtc::write_json(Json & json, bool jsonize) {
 // write whole json to output (demultiplexing file and stdout), featuring:
 // inquoting/unquoting json string, putting json into array (-j), printing size to stdout
 if(opt_[CHR(OPT_SZE)].hits() > 1)                              // -zz
  { cout << json.size() << endl; return; }

 bool write_to_file{opt_[0].hits() > 0 and opt_[CHR(OPT_FRC)].hits() > 0};  // args and -f given
 bool unquote{opt_[CHR(OPT_QUT)].hits() >= 2};                  // -qq given, unquote
 bool inquote{opt_[CHR(OPT_RAW)].hits() >= 2};                  // -rr given, inquote

 if(jsonize and opt_[CHR(OPT_JSN)].hits() == 1)                 // -j given, force jsonizing
  json = ARY{ move(json) };

 if(not unquote and inquote)
  json.root() = json.inquote_str(json.to_string(Jnode::Raw));
 DBG(0)
  DOUT() << "outputting json to " << (write_to_file?
                                       cr_.iss().filename():
                                       opt_[CHR(OPT_JSN)]? "<json>": "<stdout>") << endl;
 if(opt_[CHR(OPT_JAL)].hits() > 0)                              // -J, jsonize to global
  { cr_.jsonize(move(json)); return; }

 ofstream fout{write_to_file? cr_.iss().filename().c_str(): nullptr};
 ostream & xout = write_to_file? fout: cout;                    // demux cout/file outputs

 if(unquote and json.is_string())
  { if(not json.str().empty()) xout << json.unquote_str(json.str()) << endl; }
 else xout << json << endl;

 if(opt_[CHR(OPT_SZE)])
  cout << SIZE_PFX << json.size() << endl;
}



void Jtc::demux_opt(void) {
 // demultiplex functional options, execute once in order: -[ciuspw]
 for(char opt: STR(OPT_CMP)STR(OPT_INS)STR(OPT_UPD)STR(OPT_SWP)STR(OPT_PRG)STR(OPT_WLK)) {
  if(opt == NULL_CHR or opt_[opt].hits() == 0) continue;
  DBG(1) DOUT() << "option: '-" << opt << "', hits: " << opt_[opt].hits() << endl;

  switch(opt) {
   case CHR(OPT_CMP): return compare_jsons();                   // will print result itself
   case CHR(OPT_WLK): return walk_json();                       // will print result itself
   case CHR(OPT_INS):
   case CHR(OPT_UPD): upsert_json(opt); break;
   case CHR(OPT_PRG): purge_json(); break;
   case CHR(OPT_SWP): swap_json();  break;
   default: continue;
  }
  break;
 }

 // this write will be used only by -[iups]
 write_json(jinp_);
}



void Jtc::compare_jsons() {
 // plug-in compare_by_iterator and let process walks (print results)
 cr_.wm().clear();
 if(opt_[CHR(OPT_WLK)].hits() == 0)                             // no -w?
  opt_[CHR(OPT_WLK)] = "[^0]";                                  // then walk from root
 ready_params_(CHR(OPT_CMP));

 subscriber_ = &Jtc::collect_itr_bindings;
 walk_interleaved_();
 compare_bingings();

 if(opt_[CHR(OPT_JSN)].hits() > 0)
  write_json(jout_);
}



void Jtc::compare_bingings(void) {
 // compare two JSONs: one pointed by iterator (it), another taken from params
 static string lbl[2] { CMP_BASE, CMP_COMP };
 for(auto &pair: psrc_) {
  vector<Json> jv{2};                                           // 2 JSONs to compare

  jv.front()[lbl[0]] = *pair.dst;                               // 1st comes form walk_interleaved
  jv.back()[lbl[1]] = *pair.src;                                // 2nd comes from -c <walk-path>
  DBG().increment(+2, jv.front(), jv.back(), -2);

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
   if(not jit->empty()) cr_.rc(RC_CMP_NEQ);
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


void Jtc::upsert_json(char op) {
 // plug-in <insert/update>_by_iterator and let process walks (options -i/-u)
 static mptr upsert_meth[2] = {&Jtc::collect_itr_bindings, &Jtc::update_by_iterator};

 if(opt_[CHR(OPT_WLK)].hits() == 0)                             // no -w?
  opt_[CHR(OPT_WLK)] = "[^0]";                                  // then walk from root
 ready_params_(op);
 is_tmp_per_upst_ = opt()[CHR(OPT_TMP)].hits() > 1 and opt()[CHR(OPT_SEQ)].hits() < 2 and
                    opt()[CHR(OPT_TMP)].hits() == wsrc_.size();

 subscriber_ = upsert_meth[op == CHR(OPT_UPD)];
 walk_interleaved_();
 if(ecli_ == false or not wsrc_.empty())                        // no -e, or -e with -i/u
  apply_src_walks(op);

 maybe_update_lbl_();                                           // will update lbl if any pending

 if(opt_[CHR(OPT_PRG)].hits() > 0)                              // only work when walk-path is src
  purge_json();
}



void Jtc::collect_itr_bindings(Json::iterator &it, Grouping unused) {
 // insert each/all -[iu] processed jsons
 if(processed_by_cli_(it)) return;                              // -ei ... \; w/o trailing -i<..>

 merge_ns_(cr_.global_ns(), wns_[&it]);                         // syncup global ns
 if(jits_.walk_size() > 0)                                      // walk_size > 0 means init'ed jits
  merge_ns_(jits_.json().ns(), cr_.global_ns());                // syncup current json source

 while(advance_to_next_src()) {
  DBG(1) DOUT() << "optarg idx [" << wcur_ << "] out of "
                << (jitt_ == src_optarg? jsrc_.size(): wsrc_.size()) << " ("
                << (jitt_ == src_optarg? "static":opt_.ordinal(wsrc_[wcur_]).c_str()) << ")" <<endl;
  psrc_.emplace_back(it, jits_);                                // collect iterator
  if(is_multi_walk_) break;
 }
}



void Jtc::update_by_iterator(Json::iterator &it, Grouping unused) {
 // update each/all -u processed jsons
 if(lbl_update_ == false)                                       // not faced label update yet
  lbl_update_ = not it.walks().empty() and it.walks().back().jsearch == Json::key_of_json;
 else                                                           // lbl update occurred, then
  if(not it.is_valid())                                         // verify sanity of dst walks
   { cerr << "error: destination walk became invalid, skipping update" << endl; return; }

 collect_itr_bindings(it);
}



bool Jtc::advance_to_next_src(signed_size_t i) {
 // iterate current (valid) walk, or begin iterating next one (if available), may come with:
 //  i = -1      - first non-recursive call
 //  i != wcur_  - must process next src,
 //  i == 0/wcur_  - all src-itr walked (loop detect for single/multi walks, recursive)
 auto idx = [&i, this](void){ return wcur_ = i >= 0? i: wcur_; };   // index select

 DBG(4) DOUT() << "walk src: " << i << ", walk/json: '"
               << (jitt_ == src_optarg?
                    jsrc_[idx()].to_string(Jnode::Raw):
                    opt_.ordinal(wsrc_[idx()]).str()) << "'" << endl;
 size_t jc_size = jitt_ == src_optarg? jsrc_.size(): wsrc_.size();  // json source container size
 if(jits_.is_valid()) {                                         // if true, walk_size must be > 0
  if((++jits_).is_valid())                                      // end() not reached yet
   { wcur_ = idx(); return true; }
  return advance_to_next_src(idx() + 1 >= jc_size? 0: idx() + 1); // else use next src
 }
 // jits_ could be invalid due to: 1. init call (walk_size == 0),
 // 2. change source (reached end, i >= 0, walk_size here is always > 0)
 if(not is_multi_walk_ and i == 0)                              // single_walk, change source
  { jits_ = Json::iterator{}; return false; }                   // and loop => end of operations

 auto & srcj = jitt_ == src_input? jinp_: jitt_ == src_jexec? jexc_: jsrc_[idx()];
 if(jits_.walk_size() == 0)                                     // merge upon jits_ initialization
  merge_ns_(srcj.ns(), cr_.global_ns());                        // merge global ns to -u/i's ns

 switch(jitt_) {
  case src_input:   jits_ = srcj.walk(opt_.ordinal(wsrc_[idx()]).str()); break;
  case src_jexec:   jits_ = srcj.walk(opt_.ordinal(wsrc_[idx()]).str()); break;
  case src_optarg:  jits_ = srcj.walk(); break;
 }
 if(jits_.is_valid())                                           // jits_ resolved
  { wcur_ = idx(); return true; }
 if(i == static_cast<signed_size_t>(wcur_))                     // multiwalk: no more valid itr/src
  { jits_ = Json::iterator{}; return false; }                   // fail operation
 return advance_to_next_src(idx() + 1 >= jc_size? 0: idx() + 1); // return next src itr
}



void Jtc::apply_src_walks(char op) {
 // apply all src walks collected in <insert/update>_by_iterator
 typedef void (Jtc::*ups_ptr)(Json::iterator &dst, Json::iterator src, string *lbl);
 static ups_ptr upsert[2] = {&Jtc::merge_jsons_, &Jtc::update_jsons_};

 vec_bjit vsrc(psrc_.size());                                   // copy of valid iterators

 for(auto &pair: psrc_) {
  if(not pair.dst.is_valid())
   { cerr << "error: dst walk " << key_ << " invalided by prior operations" << endl; continue; }
  if(jitt_ == src_input and not pair.src.is_valid())
   { cerr << "error: src walk " << key_ << " invalided by prior operations" << endl; continue; }

  if(not ecli_) {
   Json tmp;
   tmp.type(Jnode::Neither);
   DBG().increment(+2, tmp, -2);

   if(opt_[CHR(OPT_TMP)].hits() == 0)                           // no -T given
    (this->*upsert[op == CHR(OPT_UPD)])(pair.dst, pair.src, &pair.lbl); // upsert w/o interpolation
   else {                                                       // -T(s) given
    // if number of templates matches number of -[iu]<walk>s, then apply template per relevant walk
    // otherwise apply templates round-robin
    if(is_tmp_per_upst_) {                                      // template per walk:
     if(tpw_.count(pair.src.walk_id()) == 0) {                  // relate interleaved walks to tmp
      auto & r = tpw_[pair.src.walk_id()];                      // create entry first
      r = tpw_.size() < opt_[CHR(OPT_TMP)].size()? opt_[CHR(OPT_TMP)].str(tpw_.size()): "";
     }
     tmp = Json::interpolate(tpw_[pair.src.walk_id()], pair.src, pair.ns);
    }
    else {                                                      // round-robin (1 templ. or 1 walk)
     tmp = Json::interpolate(opt_[CHR(OPT_TMP)].str(upst_key_ + 1), pair.src, pair.ns);
     if(++upst_key_ >= opt_[CHR(OPT_TMP)].size() - 1) upst_key_ = 0; // -1: adjust for opt_ default
    }
    (this->*upsert[op == CHR(OPT_UPD)])(pair.dst, tmp.is_neither()?
                                         pair.src: tmp.walk(), &pair.lbl);
   }
  }
  else // ecli_ == true
   if(execute_cli_(jexc_, pair.src, pair.ns) == true)
    (this->*upsert[op == CHR(OPT_UPD)])(pair.dst, jexc_.walk(), &pair.lbl);

  vsrc[key_] = move(psrc_[key_]);
  ++key_;
 }
 psrc_ = move(vsrc);
}



void Jtc::purge_json(void) {
 // remove all json nodes pointed by iterator(s), or do reverse
 if(opt_[CHR(OPT_WLK)].hits() == 0)                             // no -w?
  opt_[CHR(OPT_WLK)] = "[^0]";                                  // then walk from root

 if(opt_[CHR(OPT_PRG)].hits() > 1)                              // -pp process here
  return crop_out_();

 for(auto &wp: opt_[CHR(OPT_WLK)]) {                            // process all walks -w
  vec_jit ji = collect_walks_(wp);                              // collect all purging points

  for(size_t i = 0; i < ji.size(); ++i) {                       // purge all walked instances
   auto & rec = *ji[i];
   if(ji[i].is_valid()) {                                       // i.e. hasn't been deleted already
    DBG(1) DOUT() << "purging walk instance " << i << endl;
    if(rec.is_root()) rec.clear();                              // if root, erase everything
    else rec[-1].erase(ji[i]);
   }
  }
 }
}



void Jtc::swap_json(void) {
 // swap around nodes pointed by 2 walk pairs
 for(size_t si = 1; si < opt_[CHR(OPT_WLK)].hits(); si += 2) {
  vector<vec_jit> swaps{2};                                     // collect all walks in here
  swaps[0] = collect_walks_(opt_[CHR(OPT_WLK)].str(si));
  swaps[1] = collect_walks_(opt_[CHR(OPT_WLK)].str(si+1));

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
 // walk all -w paths, output via subscriber
 Json jdb;                                                      // integrity check in debugs only
 DBG(0)
  { jdb = jinp_; DOUT() << "copying input json for integrity check (debug only)" << endl; }

 if(opt_[CHR(OPT_JSN)].hits() == 1) jout_ = ARY{};              // otherwise jout_ is OBJ (i.e -jj)
 subscriber_ = &Jtc::output_by_iterator;
 walk_interleaved_();

 auto process_x0 = [&](void) {                                  // process -x/-1, if not yet done
  return any_of(cr_.wm().begin(), cr_.wm().end(),
                [](auto &x){ return x.factor == 0 and x.offset == -1; }) and
         none_of(cr_.wm().begin(), cr_.wm().end(),
                 [&](auto &x) { return x.factor == 0? false:
                                        (wcnt_ - 1) % x.factor == x.offset % x.factor; }); };
 if(wcnt_ > 0 and process_x0()) {                               // if there was any output
  GUARD(cr_.wm()) cr_.wm().clear();                             // do not let factors interfere
  opt_[CHR(OPT_TMP)].reset();                                   // and so do not templates
  Json j(hwlk_);                                                // produce a itr out of last walk
  output_by_iterator(j.walk("[0]"));
 }

 // after walking all paths
 if(opt_[CHR(OPT_JSN)].hits() > 0)                              // -j, jout_ contains the output
   write_json(jout_, false);
                                                                // otherwise it's already stdout'ed
 // check json integrity...
 DBG(0)
  DOUT() << "source json integrity check: "
         << (jdb == jinp_? "Good. (...phew!)": "BROKEN! (aaaa! panic! bug found!)" ) << endl;
}



void Jtc::output_by_iterator(Json::iterator &wi, Grouping grp) {
 // prints json element from given iterator (used by -w)
 // in case of -j option: collect into provided json container rather than print
 Json tmp;
 tmp.type(Jnode::Neither);
 DBG().increment(+2, tmp, -2);

 wns_[&wi][WLK_HPFX] = move(hwlk_[0]);                          // merge prior hist. into namespace
 hwlk_[0].type(Jnode::Neither);
 if(opt_[CHR(OPT_TMP)].hits() >= 1) {                           // -T given, interpolate
  merge_ns_(cr_.global_ns(), wns_[&wi]);
  // if multiple interleaved walks and multiple templates, then relate each template per walk-path
  // in all other cases - templates are round-robin applied onto each walk iteration
  if(is_tmp_per_walk_) {                                        // each template is per walk:
   if(tpw_.count(wi.walk_id()) == 0) {                          // relate interleaved walks to tmp
    auto & r = tpw_[wi.walk_id()];                              // create entry first
    r = tpw_.size() < opt_[CHR(OPT_TMP)].size()? opt_[CHR(OPT_TMP)].str(tpw_.size()): "";
   }
   tmp = Json::interpolate(tpw_[wi.walk_id()], wi, cr_.global_ns());
   hwlk_ = move(ARY{tmp.root()});
  }
  else {                                                        // round-robin (1 templ. or 1 walk)
   tmp = Json::interpolate(opt_[CHR(OPT_TMP)].str(key_ + 1), wi, cr_.global_ns());
   hwlk_ = move(ARY{tmp.root()});
   if(++key_ >= opt_[CHR(OPT_TMP)].size() - 1) key_ = 0;        // -1: adjust b/c of opt_'s default
  }
 }
 if(hwlk_[0].is_neither()) {                                    // templating failed or was none
  if(wi->has_label()) hwlk_ = move(OBJ{LBL{wi->label(), *wi}}); // preserve also the label if exist
  else hwlk_ = move(ARY{*wi});
 }

 typedef void (Jtc::*jd_ptr)(Json::iterator &, Json &, Grouping);   // demux cout and -j outputs
 static jd_ptr demux_out[2] = {&Jtc::console_output_, &Jtc::jsonized_output_};
 if(cr_.wm().empty() or                                         // output taking into account
    any_of(cr_.wm().begin(), cr_.wm().end(),                    // possible -xN/N option
           [&](auto &x){ return x.factor == 0?
                                 x.offset == static_cast<signed_size_t>(wcnt_):
                                 x.offset > static_cast<signed_size_t>(wcnt_)?
                                  false: wcnt_ % x.factor == x.offset % x.factor; } ))
  (this->*demux_out[opt_[CHR(OPT_JSN)].hits() > 0])(wi, tmp, grp);
 last_ = grp;
 ++wcnt_;
}



//
// Jtc PRIVATE methods definitions
//
void Jtc::location_(Streamstr::const_iterator &jbegin) {
 // show locus of the exception, unicode UTF-8 supported        // don't alter name, it's adjusted!
 string jsrc{jbegin.str()};
 const char * pfx = "exception locus: ";
 for(auto &chr: jsrc)                                           // replace non-readable with spaces
  chr = chr AMONG(CRTN_CHR, NLNE_CHR)?                          // and \n\r with `|`
         '|': static_cast<unsigned char>(chr) < SPCE_CHR? SPCE_CHR: chr;

 size_t from_start = Json::utf8_adjusted(0, jsrc, jbegin.is_streamed()?
                                                   jsrc.size()-1:
                                                   distance(jbegin, jinp_.exception_point()));
 size_t to_end = Json::utf8_adjusted(distance(jbegin, jinp_.exception_point()), jsrc);
 size_t ptr = from_start;

 if(from_start + to_end > DBG_WIDTH) {
  if(from_start > DBG_WIDTH/2) {
   jsrc = "..." + jsrc.substr(Json::byte_offset(jsrc, from_start - DBG_WIDTH/2 + 3));
   ptr = DBG_WIDTH/2;
  }
  if(to_end > DBG_WIDTH/2)
   jsrc = jsrc.substr(0, Json::byte_offset(jsrc, DBG_WIDTH - 3)) + "...";
 }

 DOUT() << pfx << jsrc << endl << DBG_PROMPT(0) << "exception spot: "
        << string(ptr, '-') << ">| (offset: "
        << (jbegin.is_streamed()? cr_.iss().stream_size() - 1: from_start) << ")" << endl;
}



void Jtc::ready_params_(char option) {
 // fill / prepare data: jsrc_ (if multiple jsons, no walks: -u<static> -u<static>),
 // or jexc_ & wsrc_ (in case like: -eu -u... & -u<static> -u<walk> respectively)
 if(ecli_)                                                      // -e detected
  for(size_t i = cr_.opt_e_found(); cr_.opt_ui() and i <= opt_[cr_.opt_ui()].hits(); ++i)
   wsrc_.push_back(opt_[option].ordinal(i + cr_.opt_e_found() + 1));    // +1 to account dflt val.
   // the loop conditions work only if trailing options (-u/-i) are present
 else                                                           // no -e given
  for(size_t arg_idx = 1; arg_idx < opt_[option].size(); ++arg_idx)
   try {
    auto & arg = opt_[option].str(arg_idx);
    string jfile {istream_iterator<char>(ifstream{arg, ifstream::in} >> noskipws),
                  istream_iterator<char>{}};
    DBG().increment(+2, jsrc_[jsrc_.size()], -2);
    DBG(1) DOUT() << "attempting to parse parameter (" << arg << ") as json" << endl;
    jsrc_[jsrc_.size()-1].parse(jfile.empty()? arg: jfile,
                                jfile.empty()? Json::strict_no_trail: Json::strict_trailing);
    if(not jexc_.is_neither()) { jsrc_.clear(); continue; }     // jexc_ contains src json already
    jitt_ = Jitsrc::src_optarg;
   }
   catch(Json::stdException & e) {                              // not a static json - a walk-path
    DBG(1) DOUT() << "treating parameter as a walk-path" << endl;
    jsrc_.erase(jsrc_.size()-1);
    if(not jsrc_.empty()) {                                     // mix of args: <file> <walk-path>
     jexc_ = move(jsrc_[0]);                                    // use only 1st static json
     jsrc_.clear();
     jitt_ = Jitsrc::src_jexec;
    }
    wsrc_.push_back(opt_[option].ordinal(arg_idx));
   }

 DBG(1) {
  DOUT() << "total jsons: " << (jexc_.is_neither()? jsrc_.size(): 1)
         << ", ordinal position of option -" << option << " walks: [";
  string dlm;
  for(auto w: wsrc_) { DOUT() << dlm << w; dlm = ", "; }
  DOUT() << "], jit source: " << ENUMS(Jitsrc, jitt_) << endl;
 }
}



void Jtc::maybe_update_lbl_(void) {
 // update labels if any pending update
 size_t lpv_idx;                                                // longest path_vector's index
 do {
  lpv_idx = static_cast<size_t>(-1);
  for(size_t i = 0, lpv = 0; i < psrc_.size(); ++i)
   if(psrc_[i].update_lbl())
    if(lpv < psrc_[i].dst.path_size())
     { lpv_idx = i; lpv = psrc_[i].dst.path_size(); }
  if(lpv_idx == static_cast<size_t>(-1)) continue;

  auto & pair = psrc_[lpv_idx];
  if(not pair.dst.is_valid())
   { cerr << "error: dst walk invalided by prior operations" << endl; pair.reset_lbl(); continue; }
  auto & parent = (*pair.dst)[-1];
  parent[pair.lbl] = move(parent[pair.dst->str()]);             // lbl update is in fact moving to
  parent.erase(pair.dst->str());                                // a new label and erasing old one
  pair.reset_lbl();

 } while(lpv_idx != static_cast<size_t>(-1));
}



void Jtc::compare_jsons_(const Jnode &j1, set<const Jnode*> &s1,
                         const Jnode &j2, set<const Jnode*> &s2) {
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
 // merge 2 jsons. convert to array non-array dst jsons (predicated by -m)
 if(it_dst.walks().back().jsearch == Json::key_of_json and
    it_dst.walks().back().stripped[0].empty())                  // '<>k' facing
  { cerr << "error: insert into label not applicable, use update" << endl; return; }

 if(it_dst->is_object()) {                                      // dst is object
  if(it_src->has_label())                                       // it's coming from -i walk-path
   merge_into_object_(*it_dst, OBJ{ LBL{it_src->label(), *it_src} }, preserve); //
  else
   if(it_src->is_iterable())                                    // from either json/walk-path
    merge_into_object_(*it_dst, *it_src, preserve);
   else
    cerr << "error: only an iterable could be insert-merged with an object" << endl;
  return;
 }

 if(merge_) {                                                   // -m given
  DBG(1) DOUT() << "merging into array" << endl;
  merge_into_array_(*it_dst, *it_src, preserve);                // merge any jsons
  return;
 }

 if(it_dst->is_array()) {                                       // merge only 1 json object
  it_dst->push_back(it_src->has_label()? OBJ{ LBL{it_src->label(), *it_src} }: *it_src);
  return;
 }
 cerr << "error: walk-path must point to an iterable, or use merging" << endl;
}



void Jtc::merge_into_object_(Jnode &dst, const Jnode &src, MergeObj mode) {
 // merge into object, recursively. assert(dst.is_object() and src.is_iterable())
 if(not src.is_object()) {                                      // merge arr->obj, node by node
  DBG(2) DOUT() << "merge array into object" << endl;
  if(not merge_) return;                                        // only if -m given
  auto di = dst.begin();
  for(auto &src_child: src.is_array()? src: ARY{ move(src) }) { // go by every element in the src
   if(di == dst.end()) break;
   if(mode == overwrite)
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
   if(mode == overwrite)
    *di = src_child;                                            // overwrite upon instruction only
   else
    merge_into_array_(*di, src_child, mode);                    // merge clashing non-obj into arr
  }
 }
}



void Jtc::merge_into_array_(Jnode &dst, const Jnode &src, MergeObj mode) {
 // merge 2 jsons into array.
 // convert dst to array if non-array. convert src to array if non-iterable
 const Jnode *src_ptr = &src;
 Jnode src_array;

 if(not dst.is_array()) dst = ARY{ move(dst) };                 // convert to arrays if not yet
 if(not src.is_iterable())
  { src_array = ARY{ move(src) }; src_ptr = &src_array; }

 if(mode == preserve) {
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
 if(dst.empty()) return;
 auto di = dst.begin();
 for(auto &src_child: *src_ptr) {
  *di = src_child;
  if(++di == dst.end()) break;
 }
}



void Jtc::update_jsons_(Json::iterator &it_dst, Json::iterator it_src, string *lbl) {
 // update dst with src, merge jsons with overwrite if -m is given
 if(it_dst.walks().back().jsearch == Json::key_of_json and
    it_dst.walks().back().stripped[0].empty()) {
  DBG(2) DOUT() << "label being updated" << endl;               // facilitate '<>k' (empty lexeme)
  if(merge_)
   { cerr << "error: merge not applicable in label update, ignoring" << endl; }
  if(not it_src->is_string())
   { cerr << "error: only labels could be updated with valid JSON strings" << endl; return; }
  auto & parent = (*it_dst)[-1];
  if(not parent.is_object())
   { cerr << "error: labels could be updated in objects only" << endl; return; }
  if(*it_src == *it_dst) return;                                // do not move then
  // labels update is in fact moving to a new label and removing the old one.
  // This creates a dilemma for renaming at once all nested labels. To handle it w/o failures
  // a *delayed processing* is required (like with purging):
  //    o here, mark only labeled operation, that's it
  //    o once all processing finished - rename labels by selecting deepest walks
  if(lbl) { *lbl = it_src->str(); return; }                     // record pending label update
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
                              OBJ{ LBL{ it_src->label(), *it_src } }: *it_src, overwrite);
 else
  if(it_dst->is_array())
   merge_into_array_(*it_dst, *it_src, overwrite);
  else
   *it_dst = it_src->has_label()? OBJ{ LBL{ it_src->label(), *it_src } }: *it_src;
}



bool Jtc::processed_by_cli_(Json::iterator &it) {
 // if -e given w/o trailing options - execute cli, return true if -e given alone and executed,
 // return false otherwise: -e not given, or given with trailing options - don't execute if so
 if(ecli_ == false or not wsrc_.empty()) return false;          // no -e, or wsrc has trailing opt.

 if(execute_cli_(jexc_, it, wns_[&it]) == true) {               // cli resulted in a valid json
  typedef void (Jtc::*ups_ptr)(Json::iterator &dst, Json::iterator src, string *lbl);
  static ups_ptr upsert[2] = {&Jtc::update_jsons_, &Jtc::merge_jsons_};
  (this->*upsert[subscriber_ == &Jtc::collect_itr_bindings])(it, jexc_.walk(), nullptr);
 }
 return true;
}



bool Jtc::execute_cli_(Json &json, Json::iterator &jit, const Json::map_jn &ns) {
 // execute cli in -i/u option (interpolating jit if required) and parse the result into json
 sh_.system( reconcile_ui_(jit, ns) );
 if(sh_.rc() != 0)
  { cerr << "error: shell returned error (" << sh_.rc() << ")" << endl; return false; }

 string out{sh_.stdout()};
 while(not out.empty() and out[out.size()-1] <= SPCE_CHR)       // remove all trailing spaces
  out.pop_back();
 if(out.empty())
  { DBG(1) DOUT() << "shell returned empty result, not updating" << endl; return false; }

 try { json.parse(out); }
 catch(Json::stdException & e) {                                // promote output to JSON string
  if(e.code() == Jnode::unexpected_end_of_string) throw e;      // sh.stdout had no valid json
  out = regex_replace(json.inquote_str(out), regex{"\r"}, "\\r");
  out = regex_replace(out, regex{"\n"}, "\\n");
  json.parse("\"" + out + "\"");
 }
 return true;
}



string Jtc::reconcile_ui_(Json::iterator &jit, const Json::map_jn &ns) {
 // reconcile here options -i, or -u. or interpolate a static json string
 stringstream is;                                               // is: interpolation stream
 size_t opt_idx = 0;
 string dlm;
 for(const auto & opt_param: opt_[cr_.opt_ui()]) {              // reconcile here all -u/-i options
  if(++opt_idx >= cr_.opt_e_found()) break;
  is << dlm << opt_param;
  dlm = SPCE_CHR;
 }

 Json ij = Json::interpolate(is.str(), jit, ns, Json::dont_parse);
 DBG(1) DOUT() << "interpolated & quoted string: '" << sh_quote_str(ij.str()) << "'" << endl;
 return sh_quote_str(ij.str());
}



void Jtc::crop_out_(void) {
 // output walked elements preserving source json structure
 set<const Jnode*> preserve;

 for(auto &wp: opt_[CHR(OPT_WLK)])                              // process all walks (-w)
  for(auto &ji: collect_walks_(wp))
   preserve.insert(&ji->value());

 DBG(0) DOUT() << "preserved instances: " << preserve.size() << endl;
 remove_others_(preserve, jinp_.root());
}



bool Jtc::remove_others_(set<const Jnode*> &ws, Jnode &jn) {
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
 // collect all walk iterations from given walk path (used by swap/purge)
 vec_jit walk_itr;
 if(not psrc_.empty()) {                                        // psrc is filled when walked -[ui]
  for(auto &pair: psrc_)
   walk_itr.push_back(move(jitt_ == src_input?                  // -[iu] by collect_itr_bindings
                            pair.src: pair.dst));
  DBG(0) DOUT() << "source of iterations: '" << ENUMS(Jitsrc, jitt_)
                << "', instances: " << walk_itr.size() << endl;
  return walk_itr;
 }

 for(auto it = jinp_.walk(walk_path, Json::keep_cache); it != jinp_.end(); ++it)
  walk_itr.push_back(it);
 DBG(0) DOUT() << "walk path: '" << walk_path << "', instances: " << walk_itr.size() << endl;

 return walk_itr;
}



// interleaved walks engaged when multiple walk paths are given (in absence of -n)
// processing of interleaved walks is somewhat complex:
// 1. collect all iterators produced by each walk path into a vector (wpi), e.g
//    [ [w0_i0, w0_i1, w0_iend],
//      [w1_i0, w1_i1, w1_i2, w1_iend],
//      [...],
//      ... ]
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

void Jtc::walk_interleaved_(void) {
 // collect all walks (-w/-i/-u) and feed one by one to the subscriber
 deque<deq_jit> wpi;                                            // wpi holds queues of iterators

 for(const auto &walk_str: opt_[CHR(OPT_WLK)]) {                // process all -w arguments
  jinp_.clear_ns();
  wpi.push_back( {jinp_.walk(walk_str.find_first_not_of(" ") == string::npos?
                              "[^0]": walk_str, Json::keep_cache)} );
  if(opt_[CHR(OPT_SEQ)] and wpi.size() > 1) {                   // -n and multiple -w given
   wpi.front().push_back( move(wpi.back().front()) );           // move queue to front wpi
   wpi.pop_back();                                              // drop last instance
  }

  auto & wi = wpi.back();                                       // wi: deque<Json::iterator>
  if(wi.back() == wi.back().end()) { wi.pop_back(); continue; } // end iterator
  while(wi.back() != wi.back().end()) {                         // extend all iterators until end
   update_wlk_history_(wi.back());                               // preserve walked namespace
   wi.push_back(wi.back());                                     // make new copy (next instance)
   ++wi.back();                                                 // and iterate
  }
  wi.pop_back();                                                // remove last (->end()) iterator
 }
 hwlk_ = move(ARY{STR{""}});                                    // reset hwlk_ to init value

 is_multi_walk_ = opt_[CHR(OPT_WLK)].hits() > 1 or              // or else hits == 1 and iterable
                  not(wpi.size() == 1 and not wpi.front().front().is_walkable());
 DBG(1) {
  DOUT() << "multi-walk: " << (is_multi_walk_? "true": "false") << endl;
  DOUT() << DBG_PROMPT(1) << "interleaved walk-path instances (-w: " << wpi.size() << "):" << endl;
  for(size_t i = 0; i < wpi.size(); ++i)
   DOUT() << DBG_PROMPT(1) << "instance: " << i << ", iterators: " << wpi[i].size() << endl;
 }

 while( any_of(wpi.begin(), wpi.end(), [](deq_jit &wi){ return not wi.empty(); }) )
  process_walk_iterators_(wpi);
}


void Jtc::update_wlk_history_(Json::iterator & jit) {
 // preserve walked namespace, and add auto-ns: `$?`
 //
 // there are 4 cases of template interpolations:
 // 1. in shell_callback_: <...>u
 // 2. -u<walk> -T... (or -u static.json -u<walk> -T...)
 // 3. -w<walk> -T...
 // 4. -e -u ... \;
 // Only case 3 transform a walked output - the rest are referring to original,
 // (non-transformed) walks. Thus:
 // a) when collecting walks (walk_interleaved_), populate each walk's ns with last walk value
 // b) when walking case 3 (output_by_iterator) - ensure walks are populated from hwlk_, which
 // now stores result of interpolation

 wns_[&jit] = jinp_.ns();                                       // preserve walked namespace
 wns_[&jit][WLK_HPFX] = move(hwlk_[0]);                         // add historical namespace
 if(jit->has_label()) hwlk_ = move(OBJ{LBL{jit->label(), *jit}});
 else hwlk_ = move(ARY{*jit});
}



void Jtc::process_walk_iterators_(deque<deq_jit> &wpi) {
 // build front iterators offset matrix: wpi may contain empty deque
 vector<vector<signed_size_t>> fom(wpi.size());                 // front offset matrix

 size_t longest_walk = build_front_grid_(fom, wpi);             // build FOM here
 vector<size_t> actual_instances;                               // ai: those which not ended yet
 for(size_t idx = 0; idx < fom.size(); ++idx)                   // init non-empty fom indices
  if(not fom[idx].empty()) actual_instances.push_back(idx);

 process_offsets_(wpi, fom, longest_walk, actual_instances);
}



size_t Jtc::build_front_grid_(vector<vector<signed_size_t>> &fom, const deque<deq_jit> &wpi) {
 // build matrix from front iterator of each walk (FOM): using iterator's counter() method
 // that way it'll be possible to group relevant walk-paths during output
 size_t longest = 0;                                            // longest row in the fom matrix
 for(size_t idx = 0; idx < wpi.size(); ++idx) {                 // go over all given walk paths -w
  auto & wi = wpi[idx];                                         // set of iterators for -w instance
  if(wi.empty()) continue;                                      // no more iterators in this walk

  DBG(3) DOUT() << "walkpath (-w) instance " << idx;            // go over walk path counters only
  string dlm = " front ws_counters matrix: ";                   // of the front iterator's instance
  for(size_t position = 0; position < wi.front().walk_size(); ++position) {
   if(wi.front().walks()[position].is_directive()) continue;
   fom[idx].push_back( wi.front().counter(position) );
   if(DBG()(2)) { DOUT() << dlm << fom[idx].back(); dlm = ", "; }
  }

  if(fom[idx].empty()) fom[idx].push_back( -1 );                // ensure fom filled for: -w'<vv>v'
  if(DBG()(3)) DOUT() << endl;
  longest = max(longest, fom[idx].size());
 }

 return longest;
}



void Jtc::process_offsets_(deque<deq_jit> &wpi, vector<vector<signed_size_t>> &fom,
                           size_t longest_walk, vector<size_t> &actuals) {
 // scans each offset's row (in wpi) and prints actual (non-empty) and relevant elements
 DBG(2) DOUT() << "walk offsets (longest " << longest_walk << ")";

 size_t grouping = 0,                                           // group size (of lowest offsets)
        lf_counter = -1;                                        // lowest front (ws) counter
 for(size_t offset = 0; offset < longest_walk; ++offset) {      // go across all offsets
  map<size_t, size_t> pos_ai, neg_ai;                           // build new actuals in here
  signed_size_t lowest_offset = LONG_MAX, cnt = 0;              // helpers to build new actuals
  if(DBG()(2)) DOUT() << "; [" << offset << "]:";

  for(auto ai: actuals) {                                       // a.inst. are with lowest offset
   if(offset >= fom[ai].size() or fom[ai][offset] < 0)          // negative or short actuals offset
    { neg_ai[cnt++] = ai; continue; }                           // collected separately
   if(DBG()(2)) DOUT() << " " << ai;
   if(fom[ai][offset] < lowest_offset)                          // found a lower counter
    { lowest_offset = fom[ai][offset]; pos_ai.clear(); }        // clear prior found actuals
   if(fom[ai][offset] == lowest_offset)                         // update new actuals
    pos_ai[cnt++] = ai;
  }
  if(lf_counter == static_cast<size_t>(-1) and not pos_ai.empty())
   lf_counter = lowest_offset;

  actuals.clear();
  for(signed_size_t idx = 0; idx < cnt; ++idx)
   if(pos_ai.count(idx)) actuals.push_back(pos_ai[idx]);
   else if(neg_ai.count(idx)) actuals.push_back(neg_ai[idx]);
  grouping = actuals.size();                                    // update groping
 }

 if(DBG()(2)) DOUT() << endl;                                   // close debug line
 if(actuals.empty())                                            // should never be the case
  { wpi.clear(); return; }                                      // in case, avoiding endless loop
 DBG(2) DOUT() << "output instance: " << actuals.front()
               << ", #lowest offsets/counter: {" << grouping << ", }" << endl;
 (this->*subscriber_)(wpi[actuals.front()].front(), {grouping, lf_counter});
 wpi[actuals.front()].pop_front();
}



void Jtc::console_output_(Json::iterator &wi, Json &jtmp_ref, Grouping unused) {
 // no -j given, print out element pointed by iter wi
 if(opt_[CHR(OPT_SZE)].hits() > 1)                              // -zz
  { cout << wi->size() << endl; return; }

 Jnode::iterator tl;                                            // to facilitate -ll
 auto & src = jtmp_ref.type() == Jnode::Neither? *wi: jtmp_ref.root();
 auto & sr = src.is_object() and src.has_children() and opt_[CHR(OPT_LBL)].hits() > 1?
              *(tl = src.begin()): src;

 bool unquote{opt_[CHR(OPT_QUT)].hits() >= 2};                  // -qq given, unquote
 bool inquote{opt_[CHR(OPT_RAW)].hits() >= 2};                  // -rr given, inquote

 if(opt_[CHR(OPT_LBL)] and sr.has_label())                      // -l given
  { cout << '"' << sr.label() << "\": ";  unquote = false; }    // then print label (if present)
 if(unquote and sr.is_string())                                 // don't try collapsing it into
  { if(not sr.str().empty()) cout << jinp_.unquote_str(sr.str()) << endl; }
 else {
  if(inquote) cout << '"' << jinp_.inquote_str(sr.to_string(Jnode::Raw)) << '"' << endl;
  else cout << sr << endl;                                      // a single operation!
 }

 if(opt_[CHR(OPT_SZE)])                                         // -z given
  cout << SIZE_PFX << sr.size() << endl;
}



void Jtc::jsonized_output_(Json::iterator &wi, Json &jtmp_ref, Grouping grp) {
 // demux output based on jout_ state: either to ARY or OBJ
 typedef void (Jtc::*jo_ptr)(Json::iterator &, Json &, Grouping);
 static jo_ptr jsonize[2] = {&Jtc::jsonized_output_obj_, &Jtc::jsonized_output_ary_};

 (this->*jsonize[jout_.is_array()])(wi, jtmp_ref, grp);
}



void Jtc::jsonized_output_obj_(Json::iterator &wi, Json &jtmp_ref, Grouping unused) {
 // if -jj option given, output into jout_ as Object (items w/o label are ignored)
 Jnode::iterator tl;                                            // to facilitate -ll
 auto & src = jtmp_ref.type() == Jnode::Neither? *wi: jtmp_ref.root();
 auto & srr = src.is_object() and src.has_children() and opt_[CHR(OPT_LBL)].hits() > 1?
               *(tl = src.begin()): src;

 if(not srr.has_label()) return;                                // sr has no label, ignore

 auto collect = [&] (decltype(srr) sr) {
  auto found = jout_.find(sr.label());
  if(found == jout_.end())                                      // it's a new label
   { jout_[sr.label()] = sr; return; }                          // copy supernode
                                                                // label already exist
  if(c2a_.count(sr.label()) == 0) {                             // and not converted to array yet
   jout_[sr.label()] = ARY{ move(jout_[sr.label()]) };
   c2a_.emplace(sr.label());
  }
  jout_[sr.label()].push_back(sr);
 };

 collect(srr);
}



void Jtc::jsonized_output_ary_(Json::iterator &wi, Json &jtmp_ref, Grouping grp) {
 // if -j option given, output into jout_ as Array
 auto create_obj = [&]{ return opt_[CHR(OPT_SEQ)].hits() == 0?
                                grp > last_: opt_[CHR(OPT_SEQ)].hits() < 2; };
 Jnode::iterator tl;                                            // to facilitate -ll
 auto & src = jtmp_ref.type() == Jnode::Neither? *wi: jtmp_ref.root();
 auto & sr = src.is_object() and src.has_children() and opt_[CHR(OPT_LBL)].hits() > 1?
              *(tl = src.begin()): src;

 if(not opt_[CHR(OPT_LBL)])                                     // -l not given, make simple array
  { jout_.push_back(sr); return; }
                                                                // -l given (combine relevant grp)
 if(not sr.has_label())                                         // sr has no label, push to back
  { jout_.push_back(sr); return; }
                                                                // sr has label, merge
 if(create_obj() or jout_.empty())                              // time to create a new object
  { jout_.push_back( OBJ{} ); convert_req_ = false; }
 if(not jout_.back().is_object())
  jout_.push_back( OBJ{} );

 if(jout_.back().count(sr.label()) == 0) {                     // no label recorded (first time)
  jout_.back()[sr.label()] = sr;                               // copy supernode
  if(sr.is_array()) convert_req_ = true;                       // if subsequent update, will conv.
  return;
 }
                                                                // label already exist
 if(convert_req_ or not jout_.back()[sr.label()].is_array()) {  // convert to array then
  Json tmp{ move(jout_.back()[sr.label()]) };
  (jout_.back()[sr.label()] = ARY{}).push_back( move(tmp) );
  convert_req_ = false;
 }
 jout_.back()[sr.label()].push_back( sr );                      // & push back into converted array
}



//
// Standalone calls
//
string sh_quote_str(const string &src) {
 // quote src: quote chars (which due to be quoted) outside of literals only
 string quoted;
 char inside_literal = NULL_CHR;
 short backslash_seen = 0;
 for(auto chr: src) {
  if(not backslash_seen)
   if(chr == RSLD_CHR) backslash_seen = 1;

  if(chr AMONG(SQTE_CHR, DQTE_CHR)) {
   if(not backslash_seen) {                                     // not quoted ' or "
    if(chr == inside_literal)                                   // inside literal right now
     inside_literal = NULL_CHR;                                 // exit literal (go outside)
    else
     if(inside_literal == NULL_CHR)                             // outside of literal
      inside_literal = chr;                                     // go inside literal w. given char
   }
  }
  else                                                          // not ' or "
   if(inside_literal == NULL_CHR) {                             // outside of literal, quote some
    if(not isalnum(chr)) quoted += RSLD_CHR;
    if(strchr("~`#$&*()\\|{};<>?!. \t", chr) != nullptr)        // don't quote those bash specials
     quoted.pop_back();
   }
  quoted += chr;

  if(backslash_seen)
    { if(++backslash_seen > 2) backslash_seen = 0; }
 }
 return quoted;
}




















