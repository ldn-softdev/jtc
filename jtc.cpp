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

#define VERSION "1.46"


// option definitions
#define OPT_RDT -
#define OPT_DBG d
#define OPT_EXE e
#define OPT_FRC f
#define OPT_GDE g
#define OPT_INS i
#define OPT_JSN j
#define OPT_LBL l
#define OPT_MRG m
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
#define SIZE_PFX "size: "

// various return codes
#define RETURN_CODES \
        RC_OK, \
        RC_WLK_MISS, \
        RC_SP_INV, \
        RC_SC_MISS, \
        RC_OU_MLT, \
        RC_END
ENUM(ReturnCodes, RETURN_CODES)

// return code exception offsets
#define OFF_GETOPT RC_END                                       // offset for Getopt exceptions
#define OFF_JSON (OFF_GETOPT + Getopt::end_of_throw)            // offset for Json exceptions
#define OFF_REGEX (OFF_JSON + Jnode::end_of_throw)              // offset for Regex exceptions


// simple macro for to expose class aliases
#define __REFX__(A) auto & A = __reveal_class__.A();
#define REVEAL(X, ARGS...) \
        auto & __reveal_class__ = X; \
        MACRO_TO_ARGS(__REFX__, ARGS)
// usage: REVEAL(jtc, opt, json)



class Jtc {
    typedef vector<Json::iterator> walk_vec;
    typedef deque<Json::iterator> walk_deq;
    typedef vector<string> v_string;

    #define WALKREQ \
                strict, \
                loose
    ENUM(WalkReq, WALKREQ)

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
    int                 insert_json(void);
    int                 purge_json(void);
    int                 update_json(void);
    int                 swap_json(void);
    int                 walk_json(void);

 private:
    void                convert_xyw_(void);
    bool                is_recompile_required_(v_string & args);
    void                recompile_args_(v_string &args, v_string &new_args);
    void                parse_rebuilt_(v_string & new_args);
    void                check_walk_requirements_(unsigned, WalkReq req=loose);
    void                crop_out_(void);
    bool                remove_others_(set<Jnode*> &ws, Jnode &jn);
    walk_vec            collect_walks_(const string &walk_path);
    void                merge_arrays_(Jnode &dst, const Jnode &src);
    void                merge_objects_(Jnode &dst, const Jnode &src, bool merge_non_obj);
    void                execute_cli_(Json &update, const Jnode &src_jnode);
    Json                parse_json_(const string &param);
    void                walk_sequentual_(void);
    void                walk_interleaved_(void);
    void                print_walk_(vector<walk_deq> &walk_iterators);
    void                process_offsets_(vector<walk_deq> &, vector<vector<long>> &, 
                                         size_t, vector<size_t> &);
    size_t              build_front_matrix_(vector<vector<long>> &, const vector<walk_deq> &);
    void                output_by_iterator_(walk_deq &, size_t);

    Getopt              opt_;
    Json                json_;                                   // source
    Json                jout_;                                   // output
    unsigned            opt_u_found_{0};                         // #times -u args found in user cli
    bool                convert_req_;                            // used in output_by_iterator
    size_t              last_group_{0};                          // used in output_by_iterator

 public:

    DEBUGGABLE(json_, jout_)
};

#undef WALKREQ



string quote_str(const string &src);
int wp_guide(void);




int main(int argc, char *argv[]){

 Jtc jtc;
 REVEAL(jtc, opt, json)

 opt.prolog("\nJSON test console\nVersion " VERSION \
            ", developed by Dmitry Lyssenko (ldn.softdev@gmail.com)\n");
 opt[CHR(OPT_DBG)].desc("turn on debugs (multiple calls increase verbosity)");
 opt[CHR(OPT_EXE)].desc("treat a parameter for -" STR(OPT_UPD) " as a shell cli (must precede -"
                        STR(OPT_UPD) ")");
 opt[CHR(OPT_GDE)].desc("explain walk path syntax");
 opt[CHR(OPT_SZE)].desc("print JSON size (number of nodes in JSON)");
 opt[CHR(OPT_RAW)].desc("print JSON in a raw (compact) format");
 opt[CHR(OPT_PRG)].desc("purge all walked elements (-" STR(OPT_PRG) STR(OPT_PRG)
                        ": purge all except walked)");
 opt[CHR(OPT_SLD)].desc("enforce strict quoted solidus parsing");
 opt[CHR(OPT_SWP)].desc("swap around two JSON elements (two -" STR(OPT_WLK) " must be given)");
 opt[CHR(OPT_SEQ)].desc("do not print walks interleaved (i.e. print sequentually)");
 opt[CHR(OPT_LBL)].desc("print labels too (if present) for walked JSON");
 opt[CHR(OPT_JSN)].desc("wrap walked elements into JSON (see a footnote on usage with -"
                        STR(OPT_LBL) ")");
 opt[CHR(OPT_IND)].desc("indent for pretty printing").bind("3").name("indent");
 opt[CHR(OPT_MRG)].desc("for -" STR(OPT_INS) ", -" STR(OPT_UPD) 
                        ": toggle merge for clashing JSON elements");
 opt[CHR(OPT_INS)].desc("insert into array / merge objects").name("json|file");
 opt[CHR(OPT_UPD)].desc("update/replace JSON element (one or more -" STR(OPT_WLK) \
                        " must be given)").name("json|file");
 opt[CHR(OPT_WLK)].desc("a standalone walk path (multiple may be given)").name("walkpath");
 opt[CHR(OPT_CMN)].desc("a common part of a path, prepended to every followed -" STR(OPT_PRT))
                  .bind("").name("common wp");
 opt[CHR(OPT_PRT)].desc("an individual partial path, prepended by preceding -" STR(OPT_CMN))
                  .name("partial wp");
 opt[CHR(OPT_FRC)].desc("apply changes into the file (instead of printing resulting JSON)");
 opt[0].desc("file to read json from").name("json_file").bind("<stdin>");
 opt.epilog("\nthis tool provides ability to:\n\
 - parse, validate and display JSON (in a raw and pretty formats)\n\
 - walk JSON using various subscripting/search criteria (see -" STR(OPT_GDE) ")\n\
 - manipulate JSON via purge/insert/merge/update/swap operations\n\
 for examples run with -" STR(OPT_GDE) " option\n\
\n\
Note on a multiple -" STR(OPT_WLK) " usage:\n\
 - if switch -" STR(OPT_SEQ) " is given, then all walk paths will be processed sequentially,\n\
   otherwise, paths would be grouped by relevance and walks are interleaved;\n\
   the order of provided walks will be (if can be) honored\n\n\
Note on options -" STR(OPT_CMN) " and -" STR(OPT_PRT) " usage:\n\
 - these options must be given together: one -" STR(OPT_CMN) " and multiple -" STR(OPT_PRT)
  "; each parameter\n   -" STR(OPT_PRT) " will be prepended with preceding -" STR(OPT_CMN) \
   ", together they will form an equivalent\n\
   of -" STR(OPT_WLK) ", e.g.: -xA -y1 -y2 -xB -xC -y3 is converted to: -wA1 -WA2 -wB -wC3\n\n\
Note on options -" STR(OPT_JSN) " and -" STR(OPT_LBL) " usage:\n\
 - when -" STR(OPT_JSN) " is given w/o -" STR(OPT_LBL)
   ", then walked elements will be collected into a JSON\n\
   array; when used together, all walked elements will be grouped into relevant\n\
   objects within a parent array\n\n\
Note on options -" STR(OPT_EXE) " and -" STR(OPT_UPD) " usage:\n\
 - when used together, option -" STR(OPT_EXE) " must precede option -" STR(OPT_UPD)
   "; every occurrence of " INTRP_STR "\n\
   is interpolated with walked JSON entry using raw format; interpolated entry\n\
   is completely escaped, thus does not require quoting; all shell-specific\n\
   chars (e.g.: `|', `;', `\"', etc) have to be quoted or escaped; terminate\n\
   the cli with trailing semicolon (which needs to be escapted): \\;\n");


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
  DBG(0) DOUT() << "exception raised by: " << e.where() << endl;
  cerr << opt.prog_name() << " jnode exception: " << e.what() << endl;
  return e.code() + OFF_JSON;
 }
 catch(Json::stdException & e) {
  DBG(0) DOUT() << "exception raised by: " << e.where() << endl;
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
  recompile_args_(args, new_args);                              // rebuild -u's arguments as one
  parse_rebuilt_(new_args);
 }
 else {                                                         // re recompiling, parse normally
  try { opt_.parse(argc, argv); }
  catch(Getopt::stdException &e)
   { opt_.usage(); exit(e.code() + OFF_GETOPT); }
 }

 if(opt_[CHR(OPT_GDE)].hits() > 0) exit(wp_guide());
 convert_xyw_();
}



void Jtc::read_json(void) {
 // read and parse json
 bool read_from_cin = opt_[CHR(OPT_RDT)].hits() != 0 or opt_[0].hits() == 0;
 DBG(0)
  DOUT() << "start parsing json from file: " 
         << (read_from_cin? "<stdin>": opt_[0].c_str()) <<endl;

 json_.parse( string{istream_iterator<char>(read_from_cin?
                                            cin >> noskipws:
                                            ifstream{opt_[0].c_str(), ifstream::in} >> noskipws),
                     istream_iterator<char>{}} );
}



int Jtc::write_json(void) {
 // write updated JSON (i.e. resulting from -i/-u/-s/-p/-v options into:
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
 for(auto &opt: opt_) {
  if(DBG()(2))
   if(opt.second.hits() > 0)
    DOUT() << DBG_PROMPT(2) << "option: " << (char)opt.first
                            << ", hits: " << opt.second.hits() << endl;
  switch(opt.second.hits() > 0? opt.first: 0) {
   case CHR(OPT_INS): return insert_json();
   case CHR(OPT_PRG): return purge_json();
   case CHR(OPT_UPD): return update_json();
   case CHR(OPT_SWP): return swap_json();
   case CHR(OPT_WLK): return walk_json();
  }
 }

 cout << json_ << endl;                                         // in case no exec options given
 if(opt_[CHR(OPT_SZE)])
  cout << SIZE_PFX << json_.size() << endl;
 return RC_OK;
}



int Jtc::insert_json() {
 // when inserting iterative element into iterative, merging of elements occurs 
 // if wp points to an array - insert json as it is (by pushing) (unless -m given, then merge)
 // if wp is an object - then json must be object type itself, merge recursively
 check_walk_requirements_(1);

 vector<Json> inserting(opt_[CHR(OPT_INS)].hits());             // all -i go here
 for(size_t i = 0; i < inserting.size(); ++i)
  inserting[i] = parse_json_(opt_[CHR(OPT_INS)].str(i + 1));

 for(auto &wp: opt_[CHR(OPT_WLK)]) {                            // process each walk
  walk_vec jip = collect_walks_(wp);                            // collect all insertion points
  for(size_t i = 0; i < jip.size(); ++i) {                      // insert json into walked instances
   auto & rec = *jip[i];
   DBG(1) DOUT() << "trying to insert into instance " << i << endl;
   if(rec.is_array()) {                                         // insert into array
    for(auto &j: inserting)
     if(opt_[CHR(OPT_MRG)].hits() == 0) rec.push_back(j);
     else merge_arrays_(rec, j);
    continue;
   }
   if(not rec.is_object())                                      // not an array and not object
    { cerr << "fail: walk path must point to an iterable (walk: " << i << ")" << endl; continue; }
   for(auto &j: inserting)                                      // try all insertions (-i)
    if(j.is_object()) merge_objects_(rec, j, opt_[CHR(OPT_MRG)].hits() > 0);
    else cerr << "fail: only an object could be merged with an object" << endl;
  }
 }

 return write_json();
}



int Jtc::purge_json(void) {
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

 return write_json();
}



int Jtc::update_json(void) {
 // update json in -u into all iterator(s) points
 check_walk_requirements_(1);

 vector<Json> updating(opt_[CHR(OPT_UPD)].hits());                // all -u go in here
 if(opt_[CHR(OPT_EXE)].hits() == 0)                             // this means, that -u is Json, then
  for(size_t i = 0; i < updating.size(); ++i)
   updating[i] = parse_json_(opt_[CHR(OPT_UPD)].str(i + 1));

 //Json update;
 //if(opt_[CHR(OPT_EXE)].hits() == 0)                             // this means, that -u is Json, then
 // update = parse_json_(opt_[CHR(OPT_UPD)].str());

 for(auto &wp: opt_[CHR(OPT_WLK)]) {                            // process all walks
  walk_vec ji = collect_walks_(wp);                             // collect all update points

  for(size_t i = 0; i < ji.size(); ++i) {                       // go over all update (walk) points
   auto & rec = *ji[i];
   DBG(1) DOUT() << "trying to update walk instance " << i << endl;
   for(auto &update: updating)
    if(opt_[CHR(OPT_EXE)].hits() == 0) {                        // no -e was given, its JSON
     if(opt_[CHR(OPT_MRG)].hits() == 0) rec = update;           // here we need to copy from source
     else merge_arrays_(rec, update);
    }
    else {                                                      // -e was given, execute it
     update = OBJ{};
     execute_cli_(update, rec);
     if(not update.empty()) {
      if(opt_[CHR(OPT_MRG)].hits() == 0) rec = move(update);    // update can be moved here
      else merge_arrays_(rec, update);
     }
    }
  }
 }

 return write_json();
}



int Jtc::swap_json(void) {
 // swap around nodes pointed by 2 walk paths
 check_walk_requirements_(2, strict);

 vector<walk_vec> swaps{2};                                     // collect all walks in here
 swaps[0] = collect_walks_(opt_[CHR(OPT_WLK)].str(1));
 swaps[1] = collect_walks_(opt_[CHR(OPT_WLK)].str(2));

 size_t max_i = min(swaps[0].size(), swaps[1].size());
 for(size_t i = 0; i < max_i; ++i) {                            // swap only paired walks
  if(not swaps[0][i].is_valid() or not swaps[1][i].is_valid())
   { cerr << "error: walk path instance " << i << " became invalid" << endl; return RC_SP_INV; }
  swap(*swaps[0][i], *swaps[1][i]);
 }

 return write_json();
}



int Jtc::walk_json(void) {
 // process demux sequential or interleaved walks
 Json jdb;                                                      // integrity check in debugs only
 DBG(0) {
  DOUT() << "copying input json for integrity check (debug only)" << endl;
  jdb = json_;
 }

 if(opt_[CHR(OPT_WLK)].hits() >= 2 and not opt_[CHR(OPT_SEQ)])  // no -n and more than 2 walks given
  walk_interleaved_();
 else
  walk_sequentual_();

 // check json integrity...
 DBG(0)
  DOUT() << "json integrity check: "
         << (jdb == json_? "Good. (...phew!)": "BROKEN! (aaaa! panic! bug found!)" ) << endl;
 return RC_OK;
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
  if(semicolon_found)                                           // -u already found and processed
   { new_args.push_back(arg); continue; }                       // push arg w/o any processing

  if(opt_u_found_ > 0) {                                        // facing -u; ';' not found yet,
   if(arg.back() == ';') {                                      // ';' found
    semicolon_found = true;
    arg.pop_back();                                             // trim trailing ';'
    if(not arg.empty()) new_args.back() += " " + arg;
    continue;
   }                                                            // ';' not found while processing -u
   if(++opt_u_found_ > 2) new_args.push_back("-u" + arg);
   else new_args.back() += arg;
   continue;
  }

  if(arg.front() == '-')                                        // option, see if opt -u is present
   for(const char &chr: arg) {
   if(&chr == &arg[0]) continue;                                // skip first char '-'
    if(not opt_.defined(chr)) break;                            // undefined option, process arg
    if(chr == CHR(OPT_UPD))                                     // opt -u found, indicate & record
     { opt_u_found_ = 1; break; }
   }
  new_args.push_back(arg);
 }

 if(not semicolon_found) {
  cerr << "fail: don't see parameter termination of '-" STR(OPT_UPD) "' - `\\;'" << endl;
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

 if(opt_[CHR(OPT_UPD)].hits() >= opt_u_found_) {
  cerr << "error: option -" STR(OPT_UPD) 
          " when used together with -" STR(OPT_EXE) " must be given once" << endl;
  exit(RC_OU_MLT);
 }
}



void Jtc::check_walk_requirements_(unsigned x, WalkReq req) {
 // check if actual number of walks fits min requirements
 if(req == strict)
  { if(opt_[CHR(OPT_WLK)].hits() == x) return; }
 else
  { if(opt_[CHR(OPT_WLK)].hits() >= x) return; }

 cerr << "error: " << (req==strict? "exactly ":"at least ") << x << " '-" STR(OPT_WLK) 
         "' must be given" << endl;
 exit(RC_WLK_MISS);
}



void Jtc::crop_out_(void) {
 // output walked elements preserving source json structure
 set<Jnode*> walk_set;

 for(auto &wp: opt_[CHR(OPT_WLK)])                              // process all walks
  for(auto it = json_.walk(wp); it != json_.end(); ++it)
   walk_set.insert(&it->value());                               // collect all walked elements
 DBG(0) DOUT() << "preserved instances: " << walk_set.size() << endl;

 remove_others_(walk_set, json_.root());
}



bool Jtc::remove_others_(set<Jnode*> &ws, Jnode &jn) {
 // return true if node has to be removed
 bool remove = true;

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
 // collect all walk iterations from given walk path
 walk_vec walk_itr;
 for(auto it = json_.walk(walk_path); it != json_.end(); ++it)
  walk_itr.push_back(it);

 DBG(0) DOUT() << "walk path: '" << walk_path << "', # instances: " << walk_itr.size() << endl;
 return walk_itr;
}



void Jtc::merge_arrays_(Jnode &dst, const Jnode &src) {
 // merge 2 jsons into array. convert to array any of non-array jsons
 const Jnode *src_ptr = &src;
 Jnode src_array;
 
 if(not dst.is_array()) dst = ARY{ move(dst) };
 if(not src.is_array())
  { src_array = ARY{ src }; src_ptr = &src_array; }             // here source has to be copied

 for(auto &src_child: *src_ptr)
  dst.push_back(src_child);
}



void Jtc::merge_objects_(Jnode &dst, const Jnode &src, bool merge_non_obj) {
 // merge 2 objects recursively
 for(auto &src_child: src) {                                    // go by every element in the src
  auto di = dst.find(src_child.label());                        // dst iterator
  if(di == dst.end())                                           // labels not clashing
   { dst[src_child.label()] = src_child; continue; }

  if(di->is_object() and src_child.is_object())                 // both clashing elements are OBJ
   { merge_objects_(*di, src_child, merge_non_obj); continue; }

  if(merge_non_obj) merge_arrays_(*di, src_child);              // merge clashing non-obj into array
  else *di = src_child;                                         // overwrite otherwise
 }
}



void Jtc::execute_cli_(Json &json, const Jnode &src_jnode) {
 // execute cli in -u option (interpolating src_jnode if required) and parse the result into json
 stringstream is;
 GUARD(json.is_pretty, json.pretty)
 is << src_jnode.raw();
 string src_jnode_raw = is.str();

 is.str(string());                                              // clear input stream is
 for(auto opt_u_str: opt_[CHR(OPT_UPD)]) {
  size_t interpolate = opt_u_str.find(INTRP_STR);               // see if interpolation required
  while(interpolate != string::npos) {                          // replace every occurrence of {}
   opt_u_str.replace(interpolate, sizeof(INTRP_STR), src_jnode_raw);
   interpolate = opt_u_str.find(INTRP_STR);
  }
  is << quote_str(opt_u_str) << " ";                            // quote argument
 }
 is.seekp(-1, ios_base::cur) << '\0';
 DBG(1) DOUT() << "interpolated & quoted update string: '" << is.str() << "'" << endl;

 Shell sh;
 DBG().increment(+1, sh, -1);
 sh.system(is.str());

 if(sh.rc() != 0)
  { DBG(1) DOUT() << "shell returned error: " << sh.rc() << endl; return; }
 if(sh.stdout() == "")
  { DBG(1) DOUT() << "shell returned empty result, not updating" << endl; return; }

 json.parse(sh.stdout());
}



Json Jtc::parse_json_(const string &param) {
 // attempt read JSON assuming param is a filename, otherwise assume param is JSON
 Json j;
 DBG().severity(j);
 string jstr{istream_iterator<char>(ifstream{param, ifstream::in} >> noskipws),
             istream_iterator<char>{}};

 j.parse(jstr.empty()? param: jstr);
 return j;
}



void Jtc::walk_sequentual_(void) {
 // process walks sequentially
 jout_ = ARY{};                                                 // output container for OPT_JSN

 for(auto &wp: opt_[CHR(OPT_WLK)]) {                            // -n given, walk paths sequentially
  DBG(0) DOUT() << "walking path: " << wp << endl;

  for(auto jit = json_.walk(wp, Json::keep_cache); jit != jit.end(); ++jit) {
   auto &rec = *jit;
    if( opt_[CHR(OPT_JSN)] ) {                                  // -j option given
     walk_deq deq(1, jit);                                      // required by output_by_iterator
     output_by_iterator_(deq, 0);
     continue;
   }                                                            // else, no -j given
   if( opt_[CHR(OPT_LBL)] )                                     // -l given
    if(rec.has_label())                                         // if label exists
     cout << '"' << rec.label() << "\": ";
   cout << rec << endl;
   if(opt_[CHR(OPT_SZE)])                                       // -z
    cout << SIZE_PFX << rec.size() << endl;
  }
 }

 if( opt_[CHR(OPT_JSN)] ) {
  cout << jout_ << endl;                                        // print if -j option
  if(opt_[CHR(OPT_SZE)])
   cout << SIZE_PFX << jout_.size() << endl;
 }
}



// interleaved walks engaged with option -j and multiple walk paths are given
// processing of interleaved walks is somewhat complex:
// 1. collect all iterators produced by each walk path (wpi)
// 2. for all instances build a matrix from front iterators offsets (fom) only, for
//    entire walk state:
//    - iterator offset for a built walk state returns -1 if walk step is non-iterable,
//      otherwise walk position's current offset is returned
// 3. then build a vector of actual instances (actual_instances): actual is the
//    instance that is either the shortest (in fom), or whose actual (i.e. non -1)
//    offset index is the lowest
// 4. output the most actual iterator and remove it from the wpi
// 5. repeat until entire wpi is empty

void Jtc::walk_interleaved_(void) {
 // process interleaved walks
 jout_ = ARY{};                                                 // output container for -j
 vector<walk_deq> wpi;

 for(const auto &walk_str: opt_[CHR(OPT_WLK)]) {                // process all -w arguments
  if(walk_str.empty())
   wpi.push_back( {json_.walk("[^0]", Json::keep_cache)} );
  else 
   wpi.push_back( {json_.walk(walk_str, Json::keep_cache)} );   // push initial walk iterator
  auto & wi = wpi.back();                                       // wi: deque<Json::iterators>
  while(wi.back() != wi.back().end() )                          // extend all iterators
   { wi.push_back(wi.back()); ++wi.back(); }
  wi.pop_back();                                                // remove last (->end()) iterator
 }

 DBG(1) {
  DOUT() << "walk path instances: " << wpi.size() << ":" << endl;
  for(size_t i = 0; i < wpi.size(); ++i)
   DOUT() << DBG_PROMPT(1) << "instance: " << i << ", iterations: " << wpi[i].size() << endl;
 }

 while( any_of(wpi.begin(), wpi.end(), [](walk_deq &wi){ return not wi.empty(); }) )
  print_walk_(wpi);

 if( opt_[CHR(OPT_JSN)] ) {
  cout << jout_ << endl;
  if(opt_[CHR(OPT_SZE)])
   cout << SIZE_PFX << jout_.size() << endl;
 }
}



void Jtc::print_walk_(vector<walk_deq> &wpi) {
 // build front iterators offset matrix: wpi may contain empty deque
 vector<vector<long>> fom(wpi.size());                          // front offset matrix

 size_t longest_walk = build_front_matrix_(fom, wpi);

 vector<size_t> actual_instances;
 for(size_t idx = 0; idx < fom.size(); ++idx)                   // init non-empty fom indices
  if(not fom[idx].empty()) actual_instances.push_back(idx);

 process_offsets_(wpi, fom, longest_walk, actual_instances);
}



void Jtc::process_offsets_(vector<walk_deq> &wpi, vector<vector<long>> &fom, 
                      size_t longest_walk, vector<size_t> &actuals) {
 // scans each offset's row (in wpi) and prints actual (non-empty) and relevant elements
 DBG(2) DOUT() << "walking offsets";

 int grouping = 0;                                              // group size (negative locks value)
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
 if(not actuals.empty()) {
  DBG(2) DOUT() << "output instance: " << actuals.front() << ", group size: " << grouping <<endl;
  output_by_iterator_(wpi[actuals.front()], grouping);
 }
 else                                                           // normally should never be the case
  wpi.clear();                                                  // in case, avoiding endless loop
}



size_t Jtc::build_front_matrix_(vector<vector<long>> &fom, const vector<walk_deq> &wpi) {
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



void Jtc::output_by_iterator_(walk_deq &wi, size_t group) {
 // prints json element from given iterator, removes printed iterator from the dequeue
 // in case of -j option: collect into provided json container rather than print
 auto create_obj = [&]{ return opt_[CHR(OPT_SEQ)].hits()>0?
                               group>=last_group_:
                               group>last_group_; };
 auto &sr = *(wi.front());                                      // sr is a super node (super record)
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
 else {                                                         // no -j option
  if(opt_[CHR(OPT_LBL)] and sr.has_label())                     // -l given
   cout << '"' << sr.label() << "\": ";                         // then print label (if present)
  cout << sr << endl;
  if(opt_[CHR(OPT_SZE)])
   cout << SIZE_PFX << sr.size() << endl;
 }

 wi.pop_front();
 last_group_ = group;
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



int wp_guide(void) {
 cout << R"(
    Walk path syntax:

Walk-path made of various lexemes that instruct how to traverse JSON allows
selecting single of multiple JSON elements

There are 2 types of lexemes:
 a) offset lexemes - provide addressing children nodes
 b) search lexemes - perform search down the JSON tree

a. offset lexemes: enclosed into square braces '[', ']', their meaning deppends
   on the context; following notations are possible:
   "[]", "[text]", "[n]", "[-n]", "[^n]", "[+n]"
   - empty offset "[]" matches an empty label, like this: { "": "empty label
     example" }
   - "[text]" offset selects a child in the node by the label, e.g. "[number]"
     selects element 3.14 in JSON: { "number": 3.14, "false": true }
   - plain numerical offset selects indexed element within an iterable
     (iterable is either a JSON array or object), e.g.: "[1]" - selects 2nd
     child of array or object node (all indices and quantifiers in walk path
     are zero based) - in the above JSON element 'false' will be selected
   - negative numerical offset "[-n]" backs off n levels up in JSON's tree;
     e.g.: a path like "[0][0][-2]" descends 2 levels down the JSON tree and
     then selects back a root node (ascend 2 levels up)
   - numerical offsets with preceding circumflex accent "[^n]" do similar thing
     as negative offsets, but instead descend from the root; e.g.: following
     walk paths results are equal: "[0][1][2][-1]", "[0][1]", "[0][1][2][^2]"
   - numerical offset with '+' sign makes the path iterable, e.g.: "[+1]"
     selects all children within a given JSON node (array or object) staring
     from the 2nd child.

b. search lexemes: enclosed into angular braces '<', '>', instruct to perform
   a textual search everywhere under given JSON element tree. Following
   notations are possible:
   "<txt>", "<txt>N", "<txt>+N", "<txt>s", "<txt>sN", "<txt>s+N"
   and respective non-recursive search lexemes:
   ">txt<", ">txt<N", ">txt<+N", ">txt<s", ">txt<sN", ">txt<s+N",
   where txt - is any text to search for, s - an optional one letter suffix
   - if a lexeme is given using "<...>" encasement, then a recursive search is
     applied (from current JSON node),
     otherwise (i.e. ">...<" encasement given) - a non-recursive search is
     performed among immediate JSON node's children only
   - "<text>": performs search of "text" under a JSON tree off the given node
   among JSON strings only (default behavior).
   - optionally a one letter suffix could be used, either of these: [rRlLdDbn],
     each one affecting the search in a following way:
     r - apply exact match (default) while searching string values
     R - same as r, but expression in braces is a Regex (regex search applied)
     l - apply exact match while searching labels only
     L - same as l, but expression in braces is a Regex (regex search applied)
     d - match a number (i.e. searching only numeric JSON values)
     D - same as d, but expression in braces is a Regex (regex search applied)
     b - match a boolean (i.e. searching only boolean values), true/false/any
         must be fully spelled, e.g.: "<true>b", "<any>b"
     n - match null values, the content within the encasement could be empty,
         or anything - it's ignored, e.g.: <>n, >null<n, etc
     a - match any atomic JSON value (string, numeric, boolean, null); the
         content within the encasement is ignored
     j - match user specific JSON value, the content within the encasement
         should be a valid JSON value, e.g.: "<[]>j+0" - matches all empty
         arrays
   N - an integer quantifier specifying search match instance, e.g.: "<text>2"
       will match only upon a 3rd (quantifiers are zero based) encounter of
       the word "text"
  +N - collects all search instances staring from n (zero based) that produce
       a match, e.g.: "<text>+1" will match 2nd found instance of "text", 3rd,
       4th and so on untill all matches found.
   - empty lexemes "<>", "><" may only have either r suffix - matches an empty
     string, or l - to match an empty label, or n suffix (null match),
     otherwise an exception is thrown

Both lexeme types allow specifying inner brackets, however, the closing one
has to be quoted with preceding backslash '\', e.g.: "[case[0\]]" - specifies
an offset lexeme to the label "case[0]"; "<tag<a\>>" - denotes a search lexeme
for the text "tag<a>"

Spaces between lexemes are allowed and ignored. If an offset lexeme contains
any spaces it's classified as textual offset, e.g: "[ 1]" and "[1 ]" are
treated as respective textual offsets " 1" and "1 " rather than a numerical
offset 1 (i.e. numerical offsets must contain no spaces within brackets)

There's a case when a value needs to be searched that is attached only to a
specific label, following syntax faciliatates such case: "[lbl]:<some text>"
- in that case "some text" will be searched (and matched) only among JSON
values which are attached to the label "lbl". In that notation all suffixes
(and quantifiers) are allowed except 'l' and 'L'.

    Examples:

 Consider following JSON (stored in the file example.json):
    {
        "Relation": [
            {
                "parent": "John Smith",
                "age": 31,
                "city": "New York",
                "children": [ "Sophia", "Olivia" ]
            },
            {
                "parent": "Anna Johnson",
                "age": 28,
                "city": "Chicago",
                "children": [ "John" ]
            }
        ]
    }


- to select node "children" for the first record, run:
jtc -)" STR(OPT_WLK) R"( [Relation][0][children] example.json
[
   "Sophia",
   "Olivia"
]


- to select all children records individually (for the first record), run:
jtc -)" STR(OPT_WLK) R"( '[Relation] [0] [children] [+0]' example.json
"Sophia"
"Olivia"


- to select all children (for all records) and their parents as well, run:
jtc -)" STR(OPT_WLK) R"('[Relation][+0][parent]' -)" STR(OPT_WLK) \
R"( '[Relation][+0][children][+0]' example.json
"John Smith"
"Sophia"
"Olivia"
"Anna Johnson"
"John"

- the same could be achieved through a bit more succinct syntax:
jtc -)" STR(OPT_CMN) R"('[Relation][+0]' -)" STR(OPT_PRT) R"([parent] -)" STR(OPT_PRT) \
R"( '[children][+0]' example.json
  Here, all concatenations of an option -)" STR(OPT_CMN) R"( with every option -)" \
STR(OPT_PRT) R"( is transformed
  into -)" STR(OPT_WLK) \
R"( then compiled and executed. That syntax is supposed to save the input
  space when a common path is present in multiple walks


- to select all children, from the node whose parent's name starts with "John",
  run:
jtc -)" STR(OPT_WLK) R"( '<^John>R[-1][children][+0]' example.json
"Sophia"
"Olivia"

- However, the above walk syntax is prone to false positives, as it finds any
  string, starting with "John". To imporve it, we have to ensure that the
  search is attached to the label, i.e., '"parent": "John..."':
jtc -)" STR(OPT_WLK) R"( '[parent]:<^John>R[-1][children][+0]' example.json


- to add (insert) a child "James" to a parent whose name starts with "John"
  and reflect the changes into the file, run:
jtc -)" STR(OPT_FRC) STR(OPT_WLK) R"( '[parent]:<^John>R[-1][children]' -)" \
STR(OPT_INS) R"( '"James"' example.json
{
   "Relation": [
      {
         "age": 31,
         "children": [
            "Sophia",
            "Olivia",
            "James"
         ],
         "city": "New York",
         "parent": "John Smith"
      },
      {
         "age": 28,
         "children": [
            "John"
         ],
         "city": "Chicago",
         "parent": "Anna Johnson"
      }
   ]
}


- to alter the parent's name from "John Smith" into "Jane Smith" run:
jtc -)" STR(OPT_FRC) STR(OPT_WLK) R"( '<John Smith>' -)" STR(OPT_UPD) \
R"( '"Jane Smith"' example.json
{
   "Relation": [
      {
         "age": 31,
         "children": [
            "Sophia",
            "Olivia",
            "James"
         ],
         "city": "New York",
         "parent": "Jane Smith"
      },
      {
         "age": 28,
         "children": [
            "John"
         ],
         "city": "Chicago",
         "parent": "Anna Johnson"
      }
   ]
}


- to add a new record:
jtc -)" STR(OPT_FRC) STR(OPT_WLK) R"( '[parent]:<Jane Smith> [-1]' -)" \
STR(OPT_INS) R"( '{"Y-chromosome": true}' example.json
{
   "Relation": [
      {
         "Y-chromosome": true,
         "age": 31,
         "children": [
            "Sophia",
            "Olivia",
            "James"
         ],
         "city": "New York",
         "parent": "Jane Smith"
      },
      {
         "age": 28,
         "children": [
            "John"
         ],
         "city": "Chicago",
         "parent": "Anna Johnson"
      }
   ]
}


- it's possible to wrap walked results back into JSON, with help of -)" STR(OPT_JSN) R"( option:
jtc -)" STR(OPT_WLK) R"( '[Relation][+0][parent]' -)" STR(OPT_JSN) R"( example.json
[
   "Jane Smith",
   "Anna Johnson"
]


- if we throw in an option -)" STR(OPT_LBL) R"(, then output JSON format ensures that entries with
labels will be displayed accordingly:
jtc -)" STR(OPT_WLK) R"( '[Relation][+0][parent]' -)" STR(OPT_JSN) STR(OPT_LBL) R"( example.json
[
   {
      "parent": [
         "Jane Smith",
         "Anna Johnson"
      ]
   }
]


- finally, an update option -)" STR(OPT_UPD) R"( could be subjected for a shell cli evaluation,
  say we want to capitalize all parent names:
jtc -)" STR(OPT_WLK) R"('<parent>l+0' -)" STR(OPT_EXE) STR(OPT_UPD)
R"( echo {} \| tr "[:lower:]" "[:upper:]" \; example.json
{
   "Relation": [
      {
         "Y-chromosome": true,
         "age": 31,
         "children": [
            "Sophia",
            "Olivia",
            "James"
         ],
         "city": "New York",
         "parent": "JANE SMITH"
      },
      {
         "age": 28,
         "children": [
            "John"
         ],
         "city": "Chicago",
         "parent": "ANNA JOHNSON"
      }
   ]
}

)";
 return RC_OK;
}





















