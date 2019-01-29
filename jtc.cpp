#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <set>
#include <climits>      // LONG_MAX
#include "lib/getoptions.hpp"
#include "lib/Json.hpp"
#include "lib/shell.hpp"
#include "lib/dbg.hpp"


using namespace std;

#define VERSION "1.51"


// option definitions
#define OPT_RDT -
#define OPT_DBG d
#define OPT_EXE e
#define OPT_FRC f
#define OPT_GDE g
#define OPT_INS i
#define OPT_JSN j
#define OPT_LBL l
#define OPT_MDF m
#define OPT_SEQ n
#define OPT_PRG p
#define OPT_SLD q
#define OPT_RAW r
#define OPT_SWP s
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

#define INTRP_STR "{}"
#define INTRP_SWQ "}{"                                          // same as {} but w/o quotes
#define SIZE_PFX "size: "

#include "lib/jtc_guide.hpp"

// various return codes
#define RETURN_CODES \
        RC_OK, \
        RC_WLK_MISS, \
        RC_WP_INV, \
        RC_SC_MISS, \
        RC_MLT_EXE, \
        RC_END
ENUM(ReturnCodes, RETURN_CODES)

// return code exception offsets
#define OFF_GETOPT RC_END                                       // offset for Getopt exceptions
#define OFF_JSON (OFF_GETOPT + Getopt::end_of_throw)            // offset for Json exceptions
#define OFF_REGEX (OFF_JSON + Jnode::end_of_throw)              // offset for Regex exceptions

// simple macro for to expose class aliases
// usage: REVEAL(jtc, opt, json)
#define __REFX__(A) auto & A = __reveal_class__.A();
#define REVEAL(X, ARGS...) \
        auto & __reveal_class__ = X; \
        MACRO_TO_ARGS(__REFX__, ARGS)

#define DBG_WIDTH 92                                            // max print len upon parser's dbg
#define KEY first                                               // semantic for map's pair
#define VALUE second                                            // instead of first/second



class Jtc {
    typedef vector<Json::iterator> walk_vec;
    typedef deque<Json::iterator> walk_deq;
    typedef vector<string> v_string;
    typedef void (Jtc::*mptr)(Json::iterator &wi, size_t group);
    typedef map<size_t, Json> map_json;
    typedef map<size_t, Json::iterator> map_jit;

    #define WALKREQ \
                exact,  /* walk requirement: number of given -w must match exactly */\
                minimum /* number of given -w must be at least given minimum */
    ENUM(WalkReq, WALKREQ)

    #define MERGEOBJ \
                preserve,   /* clashing labels of merged object not overwritten */\
                overwrite   /* clashing labels of merged objects overwritten */
    ENUM(MergeObj, MERGEOBJ)

 public:

    // expose private objects
    auto &              opt(void) { return opt_; }
    auto &              json(void) { return json_; }
    auto &              jout(void) { return jout_; }

    // user methods
    void                parse_opt(int argc, char *argv[]);
    void                read_json(void);
    int                 write_json(void);
    int                 demux_opt(void);
    void                insert_json(void);
    void                insert_by_iterator(Json::iterator &it, size_t group);
    void                purge_json(void);
    void                update_json(void);
    void                update_by_iterator(Json::iterator &it, size_t group);
    int                 swap_json(void);
    int                 walk_json(void);
    void                output_by_iterator(Json::iterator &wi, size_t);

 private:
    void                convert_xyw_(void);
    bool                is_recompile_required_(v_string & args);
    void                recompile_args_(v_string &args, v_string &new_args);
    void                parse_rebuilt_(v_string & new_args);
    void                check_walk_requirements_(unsigned, WalkReq req=minimum);
    void                crop_out_(void);
    bool                remove_others_(set<Jnode*> &ws, Jnode &jn);
    walk_vec            collect_walks_(const string &walk_path);
    void                merge_jsons_(Json::iterator &dst, Json::iterator src);
    void                merge_into_array_(Jnode &dst, const Jnode &src, MergeObj mode);
    void                merge_into_object_(Jnode &dst, const Jnode &src, MergeObj mode);
    void                update_jsons_(Json::iterator &dst, Json::iterator src);
    bool                execute_cli_(Json &update, const Jnode &src_jnode);
    void                parse_params_(char option);
    Json                parse_src_json_(const string &param);
    void                walk_interleaved_(void);
    void                process_walk_iterators(vector<walk_deq> &walk_iterators);
    void                process_offsets_(vector<walk_deq> &, vector<vector<long>> &,
                                         size_t, vector<size_t> &);
    size_t              build_front_grid_(vector<vector<long>> &, const vector<walk_deq> &);

    Getopt              opt_;
    Json                json_;                                  // jtc source JSON
    Json                jout_{ARY{}};                           // json output
    map_json            jsrc_;                                  // json in parameters of -i, -u
    map_jit             isrc_;                                  // walk iterators in params -i, -u
    bool                is_multi_walk_{false};                  // multiple -w or single iterable?
    bool                convert_req_{false};                    // used in output_by_iterator
    bool                ecli_{false};                           // -e status for insert/update
    bool                merge_{false};                          // -m status for insert/update
    char                recomp_{'\0'};                          // either -i or -u for recompile
    size_t              opt_r_found_{0};                        // used for recompile once -e found
    size_t              last_group_{0};                         // used in output_by_iterator
    size_t              key_{0};                                // for -i, -u options processing
    mptr                subscriber_;                            // method ptr for output processor

 public:

    DEBUGGABLE(json_, jout_)
};

#undef WALKREQ
#undef MERGEOBJ


string quote_str(const string &src);





int main(int argc, char *argv[]){

 Jtc jtc;
 REVEAL(jtc, opt, json)

 opt.prolog("\nJSON test console\nVersion " VERSION \
            ", developed by Dmitry Lyssenko (ldn.softdev@gmail.com)\n");
 opt[CHR(OPT_DBG)].desc("turn on debugs (multiple calls increase verbosity)");
 opt[CHR(OPT_EXE)].desc("make option parameters for -" STR(OPT_INS) ", -" STR(OPT_UPD)
                        " undergo shell evaluation; see -" STR(OPT_GDE) " for more info");
 opt[CHR(OPT_FRC)].desc("apply changes into the file (instead of printing resulting JSON"
                        " to stdout)");
 opt[CHR(OPT_GDE)].desc("explain walk path syntax, usage notes and examples");
 opt[CHR(OPT_JSN)].desc("wrap walked JSON elements into array; see with -"
                        STR(OPT_GDE) " for more info");
 opt[CHR(OPT_LBL)].desc("print labels (if present) for walked JSON; together with -"
                        STR(OPT_JSN) "  warp into objects");
 opt[CHR(OPT_MDF)].desc("modifier: toggle merging for options -" STR(OPT_INS) ", -" STR(OPT_UPD)
                        "; see with -" STR(OPT_GDE) " for more info");
 opt[CHR(OPT_SEQ)].desc("do not print/process walks interleaved (i.e. print/process all walks "
                        "sequentially)");
 opt[CHR(OPT_PRG)].desc("purge all walked JSON elements (-" STR(OPT_PRG) STR(OPT_PRG)
                        ": purge all elements except walked)");
 opt[CHR(OPT_SLD)].desc("enforce strict quoted solidus parsing");
 opt[CHR(OPT_RAW)].desc("print JSON in a raw (compact, one-line) format");
 opt[CHR(OPT_SWP)].desc("swap around two JSON elements pointed by walks" STR(OPT_WLK)
                        " must be given)");
 opt[CHR(OPT_SZE)].desc("print JSON size (number of nodes in JSON) at the end of JSON/walks");
 opt[CHR(OPT_INS)].desc("insert/merge JSON, or from file, or pointed by a walk-path, see with -"
                        STR(OPT_GDE) " for more").name("f|j|w");
 opt[CHR(OPT_IND)].desc("indent for pretty printing").bind("3").name("indent");
 opt[CHR(OPT_UPD)].desc("update/replace JSON, of from file, or pointed by a walk-path, see with -"
                        STR(OPT_GDE) " for more").name("f|j|w");
 opt[CHR(OPT_WLK)].desc("a standalone walk path (multiple may be given); see with -"
                        STR(OPT_GDE) " for more").name("walkpath");
 opt[CHR(OPT_CMN)].desc("a common part of a path, prepended to every followed -" STR(OPT_PRT)
                        " option").name("common wp");
 opt[CHR(OPT_PRT)].desc("an individual partial path, prepended by preceding -" STR(OPT_CMN)
                        " option").name("partial wp");


 opt[0].desc("file to read json from").name("json_file").bind("<stdin>");
 opt.epilog(R"(
this tool provides ability to:
 - parse, validate and display JSON (in a compact and pretty formats)
 - walk about input JSON using various subscripting and search criteria (see with -)" STR(OPT_GDE)
 R"( fore more)
 - manipulate JSON via purge/insert/copy/merge/update/replace/move/swap operations

for walk-path explanation, usage notes and examples run with -)" STR(OPT_GDE) R"(
)");


 // parse options
 jtc.parse_opt(argc, argv);

 DEBUGGABLE()
 DBG().use_ostream(cerr)
      .level(opt[CHR(OPT_DBG)]);

 json.tab(abs(opt[CHR(OPT_IND)]))
     .raw(opt[CHR(OPT_RAW)])
     .quote_solidus(opt[CHR(OPT_SLD)]);

 // read json and execute as per options
 try {
  jtc.read_json();
  return jtc.demux_opt();
 }
 catch(Jnode::stdException & e) {
  DBG(1) DOUT() << "exception raised by: " << e.where() << endl;
  cerr << opt.prog_name() << " jnode exception: " << e.what() << endl;
  return e.code() + OFF_JSON;
 }
 catch(Json::stdException & e) {
  DBG(1) DOUT() << "exception raised by: " << e.where() << endl;
  cerr << opt.prog_name() << " json exception: " << e.what() << endl;
  return e.code() + OFF_JSON;
 }
 catch(std::regex_error & e) {
  cerr << "regexp exception: " << e.what() << endl;
  return e.code() + OFF_REGEX;
 }
}



//
// Jtc methods definitions
//
void Jtc::parse_opt(int argc, char *argv[]) {
 // parse options, if option -e detected, rebuild -u's arguments and parse with rebuilt args
 v_string args{argv, argv + argc};

 if(is_recompile_required_(args)) {                             // re-compiling required?
  v_string new_args;                                            // newly rebuilt args go here
  recompile_args_(args, new_args);                              // rebuild -u/i's arguments as one
  parse_rebuilt_(new_args);
 }
 else {                                                         // re recompiling, parse normally
  try { opt_.parse(argc, argv); }
  catch(Getopt::stdException &e)
   { opt_.usage(); exit(e.code() + OFF_GETOPT); }
 }

 if(opt_[CHR(OPT_GDE)].hits() > 0) exit(print_guide());
 convert_xyw_();

 ecli_ = opt_[CHR(OPT_EXE)].hits() > 0;                         // flag used by -i/-u options
 merge_ = opt_[CHR(OPT_MDF)].hits() > 0;                        // flag used by -i/-u options
}



void Jtc::read_json(void) {
 // read and parse json
 bool read_from_cin = opt_[CHR(OPT_RDT)].hits() != 0 or opt_[0].hits() == 0;
 DBG(0)
  DOUT() << "start parsing json from file: "
         << (read_from_cin? "<stdin>": opt_[0].c_str()) <<endl;

 string jsrc{istream_iterator<char>(read_from_cin?
                                    cin >> noskipws:
                                    ifstream{opt_[0].c_str(), ifstream::in} >> noskipws),
             istream_iterator<char>{}};

 try { json_.parse(jsrc); }
 catch(Json::stdException & e) {
  if(e.code() < Jnode::start_of_json_parsing_exceptions or
     e.code() > Jnode::end_of_json_parsing_exceptions) throw e;
  DBG(0) {                                                      // format pretty exception point
   const char * pfx = "exception locus: ";
   for(auto &c: jsrc)
    c = c AMONG('\r', '\n')? '|': c < ' '? ' ': c;
   size_t from_start = json_.exception_point() - jsrc.begin();
   size_t to_end = jsrc.end() - json_.exception_point();
   size_t ptr = from_start;
   if(from_start + to_end > DBG_WIDTH) {
    if(from_start > DBG_WIDTH/2)
     { jsrc = "..." + jsrc.substr(from_start - DBG_WIDTH/2 + 3); ptr = DBG_WIDTH/2; }
    if(to_end > DBG_WIDTH/2) jsrc = jsrc.substr(0, DBG_WIDTH - 3) + "...";
   }
   DOUT() << pfx << jsrc << endl << DBG_PROMPT(0) << "exception spot: "
          << string(ptr, '-') << ">| (offset: " << from_start << ")" << endl;
  }
  throw e;
 }
}



int Jtc::write_json(void) {
 // write updated JSON (i.e. resulting from -i/-u/-s/-p/-w options into:
 // a) input json file (if -f given and if the input is not <stdin> ('-' option)
 // b) stdout
 bool write_to_stdout{ opt_[CHR(OPT_RDT)].hits() != 0 or opt_[0].hits() == 0 };
 if(not opt_[CHR(OPT_FRC)] or write_to_stdout)                  // stdout if no -f given,
  cout << json_ << endl;                                        // or redirect '-' is present
 else {
      DBG(0) DOUT() << "updating changes into json file: " << opt_[0].c_str() << endl;
  ofstream{opt_[0].c_str(), ofstream::out} << json_ << endl;
 }
 if(opt_[CHR(OPT_SZE)])
  cout << SIZE_PFX << json_.size() << endl;

 return RC_OK;
}



int Jtc::demux_opt(void) {
 // demultiplex functional options, execute once
 for(char opt: STR(OPT_INS) STR(OPT_UPD) STR(OPT_SWP) STR(OPT_PRG) STR(OPT_WLK)) {
  if(opt == '\0' or opt_[opt].hits() == 0) continue;
  DBG(2) DOUT() << "option: " << opt << ", hits: " << opt_[opt].hits() << endl;
  switch(opt) {
   case CHR(OPT_INS):
         insert_json(); break;
   case CHR(OPT_UPD):
         update_json(); break;
   case CHR(OPT_SWP): {
         auto rc = swap_json();
         if(rc != RC_OK) return rc;
         break;
        }
   case CHR(OPT_PRG):
         purge_json(); break;
   case CHR(OPT_WLK):
         return walk_json();
   default: continue;
  }
  break;
 }

 return write_json();
}



// insert handles 4 parameter types:
//  1. static insert ( -i <json> )
//  2. insert from file ( -i <file> ) - file must contain a valid JSON
//  3. insert from cli ( -e -i <cli> \; ) - cli must return a valid JSON
//  4. insert by walk-path (-i <src walk-path>) effectively performing copy operation
// Destination walks (-w <dst walk-path>) define mode of insertion:
//  - if insertion point is single (only one walk given and it's non-iterable), then
//    all source jsons/walks will be attempted to be inserted into a single location
//  - if multiple insertions points given (ether multiple -w, or single -w with
//    an iterable walk-path), then sources are inserted in a circular manner//
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
// Basic principles:
//  o w/o '-m' src param (-i src) is considered as a whole, with '-m' as an mergeable iterable
//  o insert operation never rewrites the dst data (-w dst), while '-m' may extend it


void Jtc::insert_json() {
 check_walk_requirements_(1);
 parse_params_(CHR(OPT_INS));                                   // collect all sources

 subscriber_ = &Jtc::insert_by_iterator;
 walk_interleaved_();

 if(not isrc_.empty() and opt_[CHR(OPT_PRG)].hits() > 0)        // only work when walk-path is a src
  purge_json();
}



void Jtc::insert_by_iterator(Json::iterator &it, size_t group) {
 // insert each/all -i processed jsons
 if(ecli_) {                                                    // -e was given
  Json insert;
  DBG().severity(insert);
  if(execute_cli_(insert, *it))                                 // cli resulted in a valid json
   merge_jsons_(it, insert.walk());
  return;
 }

 while(key_ < jsrc_.size() + isrc_.size()) {
  DBG(1) DOUT() << "trying to insert instance " << key_ << " out of "
                << jsrc_.size() + isrc_.size() << endl;

  if(jsrc_.count(key_) == 0 and not isrc_[key_].is_valid())     // key is in isrc_
   cerr << "error: walk instance " << key_
        << " became invalid due to prior operations, skipping" << endl;
  else
   merge_jsons_(it, jsrc_.count(key_)==0? isrc_[key_]: jsrc_[key_].walk("", Json::keep_cache));

  ++key_;
  if(is_multi_walk_) {
   if(key_ >= jsrc_.size() + isrc_.size()) key_ = 0;
   break;
  }
 }
}



void Jtc::purge_json(void) {
 // remove all json nodes pointed by iterator(s), or do reverse
 check_walk_requirements_(1);

 if(opt_[CHR(OPT_PRG)].hits() > 1)
  crop_out_();
 else {
  for(auto &wp: opt_[CHR(OPT_WLK)]) {                           // process all walks
   walk_vec ji = collect_walks_(wp);                            // collect all purging points

   for(size_t i = 0; i < ji.size(); ++i) {                      // purge all walked instances
    auto & rec = *ji[i];
    if(ji[i].is_valid()) {                                      // i.e. hasn't been deleted already
     DBG(1) DOUT() << "purging walk instance " << i << endl;
     if(rec.is_root()) rec.clear();                             // if root, erase everything
     else rec[-1].erase(ji[i]);
    }
   }
  }
 }
}



// update handles 4 cases:
//  1. static update ( -u <json> )
//  2. update from file ( -u <file> ) - file must contain a valid JSON
//  3. update from cli ( -e -u <cli> \; ) - cli must return a valid JSON
//  4. update by walk-path (-u <src walk-path>) effectively performing overwrite/move operation
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
//  o w/o '-m' dst param (-u dst) is considered as a whole, with '-m' as an mergeable iterable
//  o update operation rewrites the dst data (-w dst)

void Jtc::update_json() {
 check_walk_requirements_(1);
 parse_params_(CHR(OPT_UPD));                                   // collect all sources

 subscriber_ = &Jtc::update_by_iterator;
 walk_interleaved_();

 if(not isrc_.empty() and opt_[CHR(OPT_PRG)].hits() > 0)        // only work when walk-path is a src
  purge_json();
}



void Jtc::update_by_iterator(Json::iterator &it, size_t group) {
 // insert each/all -i processed jsons
 if(ecli_) {                                                    // -e was given
  Json update;
  DBG().severity(update);
  if(execute_cli_(update, *it))                                 // cli resulted in a valid json
   update_jsons_(it, update.walk());
  return;
 }

 while(key_ < jsrc_.size() + isrc_.size()) {
  DBG(1) DOUT() << "trying to update instance " << key_ << " out of "
                << jsrc_.size() + isrc_.size() << endl;

  if(jsrc_.count(key_) == 0 and not isrc_[key_].is_valid())
    cerr << "error: walk instance " << key_
         << " became invalid due to prior operations, skipping" << endl;
  else
   update_jsons_(it, jsrc_.count(key_)==0? isrc_[key_]: jsrc_[key_].walk("", Json::keep_cache));

  ++key_;
  if(is_multi_walk_) {
   if(key_ >= jsrc_.size() + isrc_.size()) key_ = 0;
   break;
  }
 }
}



int Jtc::swap_json(void) {
 // swap around nodes pointed by 2 walk paths
 check_walk_requirements_(2, exact);

 vector<walk_vec> swaps{2};                                     // collect all walks in here
 swaps[0] = collect_walks_(opt_[CHR(OPT_WLK)].str(1));
 swaps[1] = collect_walks_(opt_[CHR(OPT_WLK)].str(2));

 size_t max_i = min(swaps[0].size(), swaps[1].size());
 for(size_t i = 0; i < max_i; ++i) {                            // swap only paired walks
  if(not swaps[0][i].is_valid() or not swaps[1][i].is_valid()) {
   cerr << "error: walk instance " << i
        << " became invalid due to prior operations, aborting" << endl;
   return RC_WP_INV;
  }
  swap(*swaps[0][i], *swaps[1][i]);
 }
 return RC_OK;
}



int Jtc::walk_json(void) {
 // walk all -w paths
 Json jdb;                                                      // integrity check in debugs only
 DBG(0) {
  DOUT() << "copying input json for integrity check (debug only)" << endl;
  jdb = json_;
 }

 subscriber_ = &Jtc::output_by_iterator;
 walk_interleaved_();

 if(opt_[CHR(OPT_JSN)]) {                                       // jout_ contains the output
  cout << jout_ << endl;
  if(opt_[CHR(OPT_SZE)])
   cout << SIZE_PFX << jout_.size() << endl;
 }

 // check json integrity...
 DBG(0)
  DOUT() << "json integrity check: "
         << (jdb == json_? "Good. (...phew!)": "BROKEN! (aaaa! panic! bug found!)" ) << endl;
 return RC_OK;
}



void Jtc::output_by_iterator(Json::iterator &wi, size_t group) {
 // prints json element from given iterator
 // in case of -j option: collect into provided json container rather than print
 auto create_obj = [&]{ return opt_[CHR(OPT_SEQ)].hits() > 0?
                               group >= last_group_: group > last_group_; };
 auto &sr = *wi;                                                // sr is a super node (super record)
 if(opt_[CHR(OPT_JSN)]) {                                       // -j given (jsonize output)
  if(not opt_[CHR(OPT_LBL)]) jout_.push_back(sr);               // -l not given, make simple array
  else                                                          // -l given (combine relevant group)
   if(not sr.has_label()) jout_.push_back(sr);                  // sr has no label, push to back
   else {                                                       // sr has label, merge
    if(create_obj() or jout_.empty())                           // time to create a new object
     { jout_.push_back( OBJ{} ); convert_req_ = false; }
    if(not jout_.back().is_object()) jout_.push_back( OBJ{} );
    if(jout_.back().count(sr.label()) == 0) {                   // no label recorded (first time)
     jout_.back()[sr.label()] = sr;                             // copy supernode
     if(sr.is_array()) convert_req_ = true;                     // if subsequent update, will conv.
    }
    else {                                                      // label already exist
     if(convert_req_ or not jout_.back()[sr.label()].is_array()) {  // convert to array then
      Json tmp{ move(jout_.back()[sr.label()]) };
      (jout_.back()[sr.label()] = ARY{}).push_back( move(tmp) );
      convert_req_ = false;
     }
     jout_.back()[sr.label()].push_back( sr );                  // & push back into converted array
    }
   }
 }
 else {                                                         // no -j option, output to stdout
  if(opt_[CHR(OPT_LBL)] and sr.has_label())                     // -l given
   cout << '"' << sr.label() << "\": ";                         // then print label (if present)
  cout << sr << endl;
  if(opt_[CHR(OPT_SZE)])                                        // -z given
   cout << SIZE_PFX << sr.size() << endl;
 }

 last_group_ = group;
}



//
// private methods
//
void Jtc::convert_xyw_(void) {
 // convert -x, -y options into -w
 string last_x, last_y;
 vector<string> new_w;                                          // record new -w options here

 for(auto &option: opt_.order()) {                              // go by options order
  if(option.id() == CHR(OPT_CMN)) {                             // option -x, process it
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



bool Jtc::is_recompile_required_(v_string & args) {
 // check if option -e is present in the arguments (then re-parsing is required)
 for(auto &arg: args) {                                         // go through all args
  if(arg.front() != '-') continue;                              // not an option, don't parse
  bool undefined_opt_found = false;
  bool opt_e_found = false;
  for(const char &chr: arg) {                                   // check all options
   if(&chr == &arg[0]) continue;                                // skip first char '-'
   if(not opt_.defined(chr)) undefined_opt_found = true;        // unknown option
   if(chr == CHR(OPT_EXE)) opt_e_found = true;                  // opt -e found
  }
  if(not undefined_opt_found and opt_e_found) return true;
 }

 return false;
}



void Jtc::recompile_args_(v_string & args, v_string &new_args) {
 // recompile argv minding -u's arguments, put re-parsed args into new_args
 bool semicolon_found = false;

 for(auto &arg: args) {                                         // go through all args
  if(semicolon_found)                                           // -i/u already found and processed
   { new_args.push_back(arg); continue; }                       // push arg w/o any processing

  if(opt_r_found_ > 0) {                                        // facing -i/u; ';' not found yet,
   if(arg.back() == ';') {                                      // ';' found
    semicolon_found = true;
    arg.pop_back();                                             // trim trailing ';'
    if(not arg.empty()) new_args.back() += " " + arg;
    continue;
   }                                                            // ';' not found while processing
   if(++opt_r_found_ > 2) new_args.push_back(string("-") + recomp_ + arg);
   else new_args.back() += arg;
   continue;
  }

  if(arg.front() == '-')                                        // option, see if opt -i/u is present
   for(const char &chr: arg) {
   if(&chr == &arg[0]) continue;                                // skip first char '-'
    if(not opt_.defined(chr)) break;                            // undefined option, process arg
    if(chr == CHR(OPT_UPD) or chr == CHR(OPT_INS))              // opt -i/u found, indicate & record
     { opt_r_found_ = 1; recomp_ = chr; break; }
   }
  new_args.push_back(arg);
 }

 if(not semicolon_found) {
  cerr << "fail: don't see parameter termination of '-" << recomp_ << "' - `\\;'" << endl;
  exit(RC_SC_MISS);
 }
}



void Jtc::parse_rebuilt_(v_string & new_args) {
 // parse rebuilt arguments
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

 if(opt_[recomp_].hits() >= opt_r_found_) {
  cerr << "error: option -" << recomp_ <<
          " when used together with -" STR(OPT_EXE) " must be given once" << endl;
  exit(RC_MLT_EXE);
 }
}



void Jtc::check_walk_requirements_(unsigned x, WalkReq req) {
 // check if actual number of walks fits min requirements
 if(req == exact)
  { if(opt_[CHR(OPT_WLK)].hits() == x) return; }
 else
  { if(opt_[CHR(OPT_WLK)].hits() >= x) return; }

 cerr << "error: " << (req==exact? "exactly ":"at least ") << x << " '-" STR(OPT_WLK)
         "' must be given" << endl;
 exit(RC_WLK_MISS);
}



void Jtc::crop_out_(void) {
 // output walked elements preserving source json structure
 set<Jnode*> walk_set;

 for(auto &wp: opt_[CHR(OPT_WLK)])                              // process all walks
  for(auto it = json_.walk(wp, Json::keep_cache); it != json_.end(); ++it)
   walk_set.insert(&it->value());                              // collect all walked elements

 DBG(0) DOUT() << "preserved instances: " << walk_set.size() << endl;
 remove_others_(walk_set, json_.root());
}


bool Jtc::remove_others_(set<Jnode*> &ws, Jnode &jn) {
 // return true if node has to be removed
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
   else {                                                       // otherwise figure if to be removed
    if(remove_others_(ws, *it) == true)                         // no walked entries inside
     { jn.erase(it); continue; }
    else remove = false;
   }
  }
  ++it;
 }
 return remove;
}



Jtc::walk_vec Jtc::collect_walks_(const string &walk_path) {
 // collect all walk iterations from given walk path (used by insert/update/swap/purge)
 walk_vec walk_itr;
 if(isrc_.empty()) {                                            // called for a standalone key
  for(auto it = json_.walk(walk_path, Json::keep_cache); it != json_.end(); ++it)
   walk_itr.push_back(it);
  DBG(0) DOUT() << "walk path: '" << walk_path << "', instances: " << walk_itr.size() << endl;
 }
 else {                                                         // called from either insert/update
  for(auto &it: isrc_)
   walk_itr.push_back(it.VALUE);
  DBG(0) DOUT() << "collected iterations instances: " << walk_itr.size() << endl;
 }
 return walk_itr;
}



void Jtc::merge_jsons_(Json::iterator &it_dst, Json::iterator it_src) {
 // merge 2 jsons. convert to array non-array dst jsons (predicated by -m)
 if(it_dst->is_object()) {                                      // dst is object
  if(it_src->has_label())                                       // it's coming from -i walk-path
   merge_into_object_(*it_dst, OBJ{ LBL{it_src->label(), *it_src} }, preserve); //
  else
   if(it_src->is_iterable())                                    // from either json/walk-path
    merge_into_object_(*it_dst, *it_src, preserve);
   else
    cerr << "fail: only an iterable could be insert-merged with an object" << endl;
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
 cerr << "fail: walk-path must point to an iterable, or use merging" << endl;
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
 auto di = dst.begin();
 for(auto &src_child: *src_ptr) {
  *di = src_child;
  if(++di == dst.end()) break;
 }
}



void Jtc::merge_into_object_(Jnode &dst, const Jnode &src, MergeObj mode) {
 // merge int object, recursively. assert(dst.is_object() and src.is_iterable())
 if(not src.is_object()) {                                           // merge array->object, node by node
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
   { merge_into_object_(*di, src_child, mode); continue; }          // merge OBJ elements recursively

  if(merge_) {                                                  // if -m given
   if(mode == overwrite)
    *di = src_child;                                            // overwrite upon instruction only
   else
    merge_into_array_(*di, src_child, mode);                    // merge clashing non-obj into array
  }
 }
}



void Jtc::update_jsons_(Json::iterator &it_dst, Json::iterator it_src) {
 // update dst with src, merge jsons with overwrite if -m is given
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



bool Jtc::execute_cli_(Json &json, const Jnode &src_jnode) {
 // execute cli in -i/u option (interpolating src_jnode if required) and parse the result into json
 stringstream is;
 GUARD(json.is_pretty, json.pretty)
 is << src_jnode.raw();
 string src_raw = is.str();
 string src_nkd = src_raw.front() == '"'?                       // w/o double quotes (naked)
                  src_raw.substr(1, src_raw.size()-2): src_raw;

 is.str(string());                                              // clear input stream is
 for(auto opt_param: opt_[recomp_]) {
  size_t interpolate = opt_param.find(INTRP_STR);               // see if interpolation required
  while(interpolate != string::npos) {                          // replace every occurrence of {}
   opt_param.replace(interpolate, sizeof(INTRP_STR) - 1, src_raw);
   interpolate = opt_param.find(INTRP_STR);
  }
  size_t inter_naked = opt_param.find(INTRP_SWQ);               // see if naked interp. required
  while(inter_naked != string::npos) {                          // replace every occurrence of }{
   opt_param.replace(inter_naked, sizeof(INTRP_SWQ) - 1, src_nkd);
   inter_naked = opt_param.find(INTRP_SWQ);
  }
  is << quote_str(opt_param) << " ";                            // quote argument
 }
 is.seekp(-1, ios_base::cur) << '\0';
 DBG(1) DOUT() << "interpolated & quoted cli string: '" << is.str() << "'" << endl;

 Shell sh;
 DBG().increment(+1, sh, -1);
 sh.system(is.str());

 if(sh.rc() != 0)
  { DBG(1) DOUT() << "shell returned error: " << sh.rc() << endl; return false; }
 if(sh.stdout() == "")
  { DBG(1) DOUT() << "shell returned empty result, not updating" << endl; return false; }

 json.parse(sh.stdout());
 return true;
}



void Jtc::parse_params_(char option) {
 // attempt to parse parameter (of either -i or -u), first assume it's a file,
 // then assume it's a JSON, finally it's a walk-path
 if(ecli_) return;                                              // do not attempt if -e given

 size_t key = 0;
 for(auto & jsrc: opt_[option])
  try {
   string jstr{istream_iterator<char>(ifstream{jsrc, ifstream::in} >> noskipws),
               istream_iterator<char>{}};
   DBG().severity(jsrc_[key]);
   jsrc_[key].parse(jstr.empty()? jsrc: jstr, Json::strict_trailing);
   ++key;
  }
  catch(Json::stdException & e) {                               // it's not a json
   jsrc_.erase(key_);
   auto jit = json_.walk(jsrc, Json::keep_cache);
   while(jit != jit.end() )                                     // extend all iterators until end
    { isrc_[key++] = jit; ++jit; }
  }
 DBG(0)
  DOUT() << "option '-" << option << "' parsing, total jsons: " << jsrc_.size()
         << ", total iterations: " << isrc_.size() << endl;
}



Json Jtc::parse_src_json_(const string &param) {
 // attempt read JSON assuming param is a filename, otherwise assume it's a walk-path,
 // otherwise assume param is JSON
 Json j;
 DBG().severity(j);
 string jstr{istream_iterator<char>(ifstream{param, ifstream::in} >> noskipws),
             istream_iterator<char>{}};

 j.parse(jstr.empty()? param: jstr);
 return j;
}



// interleaved walks engaged when multiple walk paths are given (in absence of -n)
// processing of interleaved walks is somewhat complex:
// 1. collect all iterators produced by each walk path into a vector (wpi), e.g
//    [ [w0_i0, w0_i1, w0_iend],
//      [w1_i0, w1_i1, w1_i2, w1_iend] ]
// 2. for all instances build a matrix from front iterators offsets (fom) only for
//    entire walk state:
//    - iterator offset for a built walk state returns -1 if walk step is non-iterable,
//      otherwise walk position's current offset is returned, e.g.:
//      w0_i0 and w1_i0, result in the probable FOM:
//          [ [w0_i0_p0 (0), w0_i0_p1 (0), w0_i0_p2 (-1),
//            [w1_i0_p0 (0), w1_i0_p1 (-1)] ]
// 3. then build a vector referring actual instances: actual is the instance that is
//    non-empty (e.g.: 0, 1)
// 4. among actual iterators, select first in the relevant group, give it to the subscriber
//    and remove it from the wpi
// 5. repeat until entire wpi is empty

void Jtc::walk_interleaved_(void) {
 // collect all walks (-w) and feed one by one to the subscriber
 vector<walk_deq> wpi;

 for(const auto &walk_str: opt_[CHR(OPT_WLK)]) {                // process all -w arguments
  wpi.push_back( {json_.walk(walk_str.find_first_not_of(" ") == string::npos?
                             "[^0]": walk_str, Json::keep_cache)} );
  if(opt_[CHR(OPT_SEQ)] and wpi.size() > 1) {                   // -n is given and -w is secondary
   wpi.front().push_back( move(wpi.back().front()) );           // move iterator to front wpi
   wpi.pop_back();                                              // drop last instance
  }

  auto & wi = wpi.back();                                       // wi: deque<Json::iterators>
  while(wi.back() != wi.back().end() )                          // extend all iterators until end
   { wi.push_back(wi.back()); ++wi.back(); }
  wi.pop_back();                                                // remove last (->end()) iterator
 }

 is_multi_walk_ = opt_[CHR(OPT_WLK)].hits() > 1 or
                  not(wpi.size() == 1 and not wpi.front().front().is_walkable());
 DBG(1) {
  DOUT() << "multi-walk: " << (is_multi_walk_? "true": "false") << endl;
  DOUT() << DBG_PROMPT(1) << "walk-path instances: " << wpi.size() << ":" << endl;
  for(size_t i = 0; i < wpi.size(); ++i)
   DOUT() << DBG_PROMPT(1) << "instance: " << i << ", iterations: " << wpi[i].size() << endl;
 }

 while( any_of(wpi.begin(), wpi.end(), [](walk_deq &wi){ return not wi.empty(); }) )
  process_walk_iterators(wpi);
}



void Jtc::process_walk_iterators(vector<walk_deq> &wpi) {
 // build front iterators offset matrix: wpi may contain empty deque
 vector<vector<long>> fom(wpi.size());                          // front offset matrix

 size_t longest_walk = build_front_grid_(fom, wpi);
 vector<size_t> actual_instances;                               // ai: those which not ended yet
 for(size_t idx = 0; idx < fom.size(); ++idx)                   // init non-empty fom indices
  if(not fom[idx].empty()) actual_instances.push_back(idx);

 process_offsets_(wpi, fom, longest_walk, actual_instances);
}



void Jtc::process_offsets_(vector<walk_deq> &wpi, vector<vector<long>> &fom,
                      size_t longest_walk, vector<size_t> &actuals) {
 // scans each offset's row (in wpi) and prints actual (non-empty) and relevant elements
 DBG(2) DOUT() << "walking offsets (" << longest_walk << ")";

 int grouping = 0;                                              // group size
 for(size_t offset = 0; offset < longest_walk; ++offset) {      // go across all offsets
  map<size_t, size_t> pos_ai, neg_ai;                           // build new actuals in here
  long lowest_offset = LONG_MAX, cnt = 0;                       // helpers to build new actuals
  if(DBG()(2)) DOUT() << ", [" << offset << "]:";

  for(auto ai: actuals) {                                       // a.inst. are with lowest offset
   if(DBG()(2)) DOUT() << ' ' << ai;
   if(offset >= fom[ai].size() or fom[ai][offset] < 0)          // negative or short actuals offset
    { neg_ai[cnt++] = ai; continue; }                           // collected separately
   if(fom[ai][offset] < lowest_offset)                          // found a lower counter
    { lowest_offset = fom[ai][offset]; pos_ai.clear(); }        // clear prior found actuals
   if(fom[ai][offset] == lowest_offset)                         // update new actuals
    pos_ai[cnt++] = ai;
  }

  actuals.clear();
  for(long idx = 0; idx < cnt; ++idx)
   if(pos_ai.count(idx)) actuals.push_back(pos_ai[idx]);
   else if(neg_ai.count(idx)) actuals.push_back(neg_ai[idx]);
  grouping = actuals.size();                                    // update groping
  if(grouping == 1) break;                                      // performance optimization
 }

 if(DBG()(2)) DOUT() << endl;                                   // close debug line
 if(actuals.empty())                                            // normally should never be the case
  { wpi.clear(); return; }                                      // in case, avoiding endless loop
 DBG(2) DOUT() << "output instance: " << actuals.front() << ", group size: " << grouping <<endl;
 (this->*subscriber_)(wpi[actuals.front()].front(), grouping);
 wpi[actuals.front()].pop_front();
}



size_t Jtc::build_front_grid_(vector<vector<long>> &fom, const vector<walk_deq> &wpi) {
 // build matrix from front iterator of each walk: using iterator's counter() method
 // that way it'll be possible to group relevant walk-paths during output
 size_t longest = 0;
 for(size_t idx = 0; idx < wpi.size(); ++idx) {                 // go over all given walk paths
  auto & wi = wpi[idx];
  if(wi.empty()) continue;                                      // no more iterators in this walk

  DBG(2) DOUT() << "instance " << idx;
  for(size_t position = 0; position < wi.front().walk_size(); ++position) {
   fom[idx].push_back( wi.front().counter(position) );
   if(DBG()(2)) DOUT() << (position == 0? " front offsets matrix: ": ", ") << fom[idx].back();
  }

  if(DBG()(2)) DOUT() << endl;
  longest = max(longest, fom[idx].size());
 }

 return longest;
}



string quote_str(const string &src) {
 string quoted;
 if(src == "|") return src;
 for(auto chr: src) {
  if(not isalnum(chr)) quoted += '\\';
  quoted += chr;
 }
 return quoted;
}



















