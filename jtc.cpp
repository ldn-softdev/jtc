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

#define VERSION "1.37"


// option definitions
#define OPT_RDT -
#define OPT_DBG d
#define OPT_EXE e
#define OPT_FRC f
#define OPT_GDE g
#define OPT_INS i
#define OPT_JSN j
#define OPT_LBL l
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

// various return codes
#define RETURN_CODES \
        RC_OK, \
        RC_WLK_MISS, \
        RC_WP_TWO, \
        RC_SP_NST, \
        RC_SP_INV, \
        RC_SC_MISS, \
        RC_SH_ERR, \
        RC_OU_MLT, \
        RC_END
ENUM(ReturnCodes, RETURN_CODES)

// return code exception offsets
#define OFF_GETOPT RC_END                                       // offset for Getopt exceptions
#define OFF_JSON (OFF_GETOPT + Getopt::end_of_throw)            // offset for Json exceptions
#define OFF_REGEX (OFF_JSON + Jnode::end_of_throw)              // offset for Regex exceptions



// holding common shared resources, and declaration helper macro
struct CommonResources {
    Getopt              opt;
    Json                json;                                   // source
    Json                jout;                                   // output
    unsigned            opt_u_found{0};                         // #times -u args found in user cli

    DEBUGGABLE()
};

#define __REFX__(A) auto & A = __common_resource__.A;
#define REVEAL(X, ARGS...) \
        auto & __common_resource__ = X; \
        MACRO_TO_ARGS(__REFX__, ARGS)
// usage: REVEAL(cr, opt, DBG())



// forward declarations
typedef vector<Json::iterator> walk_vec;
typedef deque<Json::iterator> walk_deq;
typedef vector<string> v_string;

void parse_opt(int argc, char *argv[], CommonResources &);
bool is_recompile_required(v_string & args, CommonResources &);
void parse_rebuilt(v_string & sargv, CommonResources &r);
void recompile_args(v_string &args, v_string &sargv, CommonResources &r);
void read_json(CommonResources &);
int demux_opt(CommonResources &);
int insert_json(CommonResources &);
int purge_json(CommonResources &);
int update_json(CommonResources &);
void execute_cli(Json &update, const Jnode &src_jnode, CommonResources &r);
int swap_json(CommonResources &);
void crop_out(CommonResources &r);
int walk_json(CommonResources &);
void walk_sequentual (CommonResources &);
void walk_interleaved(CommonResources &);
void print_walk(vector<walk_deq> &walk_iterators, CommonResources &r);
void process_offsets(vector<walk_deq> &, vector<vector<long>> &,
                     size_t, vector<size_t> &, CommonResources &);
size_t build_front_matrix(vector<vector<long>> &, const vector<walk_deq> &, CommonResources &);
void output_by_iterator(walk_deq &, size_t, CommonResources &);
int wp_guide(void);





int main(int argc, char *argv[]){

 CommonResources r;
 REVEAL(r, opt, json, jout, DBG())

 opt.prolog("\nJSON test console\nVersion " VERSION \
            ", developed by Dmitry Lyssenko (ldn.softdev@gmail.com)\n");
 opt[CHR(OPT_DBG)].desc("turn on debugs (multiple calls increase verbosity)");
 opt[CHR(OPT_EXE)].desc("treat a parameter for -" STR(OPT_UPD) " as a shell cli (must precede -"
                        STR(OPT_UPD) ")");
 opt[CHR(OPT_GDE)].desc("explain walk path syntax");
 opt[CHR(OPT_SZE)].desc("print JSON size (total number of nodes in JSON)");
 opt[CHR(OPT_RAW)].desc("print JSON in a raw (compact) format");
 opt[CHR(OPT_PRG)].desc("purge all walked elements (-" STR(OPT_PRG) STR(OPT_PRG)
                        ": purge all except walked)");
 opt[CHR(OPT_SLD)].desc("enforce strict quoted solidus parsing");
 opt[CHR(OPT_SWP)].desc("swap around two JSON elements (two -" STR(OPT_WLK) " must be given)");
 opt[CHR(OPT_SEQ)].desc("do not print walks interleaved (i.e. print sequentually)");
 opt[CHR(OPT_LBL)].desc("print labels too (if exist) for walked JSON");
 opt[CHR(OPT_JSN)].desc("wrap walked elements into JSON (see footnote on usage with -"
                        STR(OPT_LBL) ")");
 opt[CHR(OPT_IND)].desc("indent for pretty printing").bind("3").name("indent");
 opt[CHR(OPT_INS)].desc("insert JSON element (one or more -" STR(OPT_WLK) " must be given)")
                  .name("json");
 opt[CHR(OPT_UPD)].desc("update/replace JSON element (one or more -" STR(OPT_WLK) \
                        " must be given)").name("json");
 opt[CHR(OPT_WLK)].desc("a standalone walk path (multiple may be given)").name("walkpath");
 opt[CHR(OPT_CMN)].desc("a common part of a path, prepended to every specified -" STR(OPT_PRT))
                  .bind("").name("common");
 opt[CHR(OPT_PRT)].desc("an individual partial path, prepended by specified -" STR(OPT_CMN))
                  .name("partial");
 opt[CHR(OPT_FRC)].desc("apply changes into the file (instead of printing resulting JSON)");
 opt[0].desc("file to read json from").name("json_file").bind("<stdin>");
 opt.epilog("\nthis tool provides ability to:\n\
 - parse, validate and display JSON (in a raw and pretty formats)\n\
 - walk JSON using various subscripting/search criteria (see -" STR(OPT_GDE) ")\n\
 - manipulate JSON via purge/insert/update/swap operations\n\
 for examples run with -" STR(OPT_GDE) " option\n\
\n\
Note on a multiple -" STR(OPT_WLK) " usage:\n\
 - if switch -" STR(OPT_SEQ) " is given, then all walk paths will be processed sequentually,\n\
   otherwise, paths would be grouped by relevance and walks are interleaved\n\n\
Note on options -" STR(OPT_CMN) " and -" STR(OPT_PRT) " usage:\n\
 - these options must be given together: one -" STR(OPT_CMN) " (in case multiple given, only\n\
   the last one is considered) and multiple -" STR(OPT_PRT)
   "; each parameter -" STR(OPT_PRT) " will be\n\
   prepended by parameter from -" STR(OPT_CMN) \
   ", tother they will form an equivalent of -" STR(OPT_WLK) "\n\n\
Note on options -" STR(OPT_JSN) " and -" STR(OPT_LBL) " usage:\n\
 - when -" STR(OPT_JSN) " is given w/o -" STR(OPT_LBL)
   ", then walked elements will be collected into a JSON\n\
   array; when used together, all walked elements will be grouped into relevant\n\
   objects within a parent array\n\n\
Note on options -" STR(OPT_EXE) " and -" STR(OPT_UPD) " usage:\n\
 - option -" STR(OPT_EXE) " must precede option -" STR(OPT_UPD)
   " when used together; every occurrence of " INTRP_STR "\n\
   is interpolated with walked JSON entry using raw format; interpolated entry\n\
   is completely escaped, thus does not require quoting; all shell-specific\n\
   chars (e.g.: `|', `;', `\"', etc) have to be quoted or escaped; terminate\n\
   the cli with trailing semicolon (which needs to be escapted): \\;\n");


 // prepare debugs
 DBG().use_ostream(cerr);

 // parse options
 parse_opt(argc, argv, r);

 DBG().level(opt[CHR(OPT_DBG)])
      .severity(json, jout);

 json.tab(abs(opt[CHR(OPT_IND)]))
     .raw(opt[CHR(OPT_RAW)])
     .quote_solidus(opt[CHR(OPT_SLD)]);

 // read json and execute as per options
 try {
  read_json(r);
  return demux_opt(r);
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



void parse_opt(int argc, char *argv[], CommonResources &r) {
 // parse options, if option -e detected, rebuild -u's arguments and parse with rebuilt args
 REVEAL(r, opt)

 v_string args{argv, argv + argc};
 if(is_recompile_required(args, r)) {                           // re-compiling required?
  v_string sargv;                                               // newly rebuilt args go here
  recompile_args(args, sargv, r);                               // rebuild -u's arguments as one
  parse_rebuilt(sargv, r);
 }
 else {                                                         // re recompiling, parse normally
  try { opt.parse(argc,argv); }
  catch(Getopt::stdException &e)
   { opt.usage(); exit(e.code() + OFF_GETOPT); }
 }

 if(opt[CHR(OPT_GDE)].hits() > 0) exit(wp_guide());

 for(auto &partial: opt[CHR(OPT_PRT)])                          // concatenate -x+-y and put into -w
  opt[CHR(OPT_WLK)] = opt[CHR(OPT_CMN)].str() + partial;
}



bool is_recompile_required(v_string & args, CommonResources &r) {
 // check if option -e is present in the arguments (then re-parsing is required)
 REVEAL(r, opt)

 for(auto &arg: args) {                                         // go through all args
  if(arg.front() != '-') continue;                              // not an option, don't parse
  bool undefined_opt_found = false;
  bool opt_e_found = false;
  for(const char &chr: arg) {                                   // check all options
   if(&chr == &arg[0]) continue;                                // skip first char '-'
   if(not opt.defined(chr)) undefined_opt_found = true;         // unknown option
   if(chr == CHR(OPT_EXE)) opt_e_found = true;                  // opt -e found
  }
  if(not undefined_opt_found and opt_e_found) return true;
 }

 return false;
}



void parse_rebuilt(v_string & sargv, CommonResources &r) {
 // parse rebuilt arguments
 REVEAL(r, opt, opt_u_found )

 char *nargv[sargv.size()];                                     // here, build a new argv
 for(size_t i = 0; i < sargv.size(); ++i) {
  nargv[i] = new char[sargv[i].size()+1];
  stpcpy(nargv[i], sargv[i].c_str());
 }

 try { opt.reset().parse(sargv.size(), nargv); }                // re-parse newly rebuilt args
 catch(Getopt::stdException & e)
  { opt.usage(); exit(e.code() + OFF_GETOPT); }

 for(size_t i = 0; i < sargv.size(); ++i)                       // clean up nargv now
  delete [] nargv[i];

 if(opt[CHR(OPT_UPD)].hits() >= opt_u_found) {
  cerr << "error: option -" STR(OPT_UPD) " must be given only once" << endl;
  exit(RC_OU_MLT);
 }
}



void recompile_args(v_string & args, v_string &sargv, CommonResources &r) {
 // recompile argv minding -u's arguments, put re-parsed args into sargv
 REVEAL(r, opt, opt_u_found)
 bool semicolon_found = false;

 for(auto &arg: args) {                                         // go through all args
  if(semicolon_found)                                           // -u already found and processed
   { sargv.push_back(arg); continue; }                          // push arg w/o any processing

  if(opt_u_found > 0) {                                         // facing -u; ';' not found yet,
   if(arg.back() == ';') {                                      // ';' found
    semicolon_found = true;
    arg.pop_back();                                             // trim trailing ';'
    if(not arg.empty()) sargv.back() += " " + arg;
    continue;
   }                                                            // ';' not found while processing -u
   if(++opt_u_found > 2) sargv.push_back("-u" + arg);
   else sargv.back() += arg;
   continue;
  }

  if(arg.front() == '-')                                        // option, see if opt -u is present
   for(const char &chr: arg) {
   if(&chr == &arg[0]) continue;                                // skip first char '-'
    if(not opt.defined(chr)) break;                             // undefined option, record arg
    if(chr == CHR(OPT_UPD))                                     // opt -u found, indicate & record
     { opt_u_found = 1; break; }
   }
  sargv.push_back(arg);
 }

 if(not semicolon_found) {
  cerr << "fail: don't see parameter termination of -" STR(OPT_UPD) " option - `;'" << endl;
  exit(RC_SC_MISS);
 }
}



void read_json(CommonResources &r) {
 // read and parse json
 REVEAL(r, opt, json, DBG())

 bool redirect = opt[CHR(OPT_RDT)].hits() != 0 or opt[0].hits() == 0;
 DBG(0)
  DOUT() << "start parsing json from file: " << (redirect? "<stdin>": opt[0].c_str()) <<endl;

 json.parse( string{istream_iterator<char>(redirect?
                                           cin >> noskipws:
                                           ifstream{opt[0].c_str(), ifstream::in} >> noskipws),
                    istream_iterator<char>{}} );

 if(opt[CHR(OPT_SZE)])
  { cout << "json size: " << json.size() << endl; exit(RC_OK); }
}



void write_json(CommonResources &r) {
 // write updated JSON (i.e. resulting from -i/-u/-s/-p/-v options into:
 // a) input json file (if -f given and if the input is not <stdin> ('-' option)
 // b) stdout
 REVEAL(r, opt, json, DBG())

 bool redirect{ opt[CHR(OPT_RDT)].hits() != 0 or opt[0].hits() == 0 };
 if(not opt[CHR(OPT_FRC)] or redirect)                          // stdout if no -f given,
  { cout << json << endl; return; }                             // or redirect '-' is present

 DBG(0) DOUT() << "updating changes into json file: " << opt[0].c_str() << endl;
 ofstream{opt[0].c_str(), ofstream::out} << json << endl;
}



int demux_opt(CommonResources &r) {
 // demultiplex functional options, execute only once
 REVEAL(r, opt, json, DBG())

 for(auto &op: opt) {
  if(DBG()(2))
   if(op.second.hits() > 0)
    DOUT() << DBG_PROMPT(2) << "option: " << (char)op.first
                            << ", hits: " << op.second.hits() << endl;
  switch(op.second.hits() > 0? op.first: 0) {
   case CHR(OPT_INS):
         return insert_json(r);
   case CHR(OPT_PRG):
         return purge_json(r);
   case CHR(OPT_UPD):
         return update_json(r);
   case CHR(OPT_SWP):
         return swap_json(r);
   case CHR(OPT_WLK):
         return walk_json(r);
  }
 }

 cout << json << endl;                                          // in case no exec options given
 return RC_OK;
}


walk_vec collect_walks(const string &walk_path, CommonResources &r) {
 // collect all walk iterations from given walk path
 REVEAL(r, json, DBG())

 walk_vec walk_itr;
 for(auto it = json.walk(walk_path); it != json.end(); ++it)
  walk_itr.push_back(it);
 DBG(0) DOUT() << "walk path: '" << walk_path << "', # instances: " << walk_itr.size() << endl;

 return walk_itr;
}



int insert_json(CommonResources &r) {
 // if wp points to an array - insert json as it is.
 // if wp is an object - then json must be object type itself and inserted by labels
 REVEAL(r, opt, DBG())

 if(opt[CHR(OPT_WLK)].hits() < 1) {
  cerr << "error: at least one -" STR(OPT_WLK) " must be given when inserting" << endl;
  return RC_WLK_MISS;
 }

 vector<Json> inserting(opt[CHR(OPT_INS)].hits());              // all -i go here
 for(size_t i = 0; i < inserting.size(); ++i) {
  DBG().severity(inserting[i]);
  inserting[i].parse(opt[CHR(OPT_INS)].str(i+1));               // +1 adjust for default opt value
 }

 for(auto &wp: opt[CHR(OPT_WLK)]) {                             // process each walk
  walk_vec jip = collect_walks(wp, r);                          // collect all insertion points

  for(size_t i = 0; i < jip.size(); ++i) {                      // insert json into walked instances
   auto & rec = *jip[i];
   DBG(1) DOUT() << "trying to insert into instance " << i << endl;
   if(rec.is_array())                                           // insert into array
    { for(auto &j: inserting) rec.push_back(j);  continue; }    

   if(not rec.is_object())                                      // not an array and not object
    { cerr << "fail: walk path must point to an iterable (walk: " << i << ")" << endl; continue; }

   for(auto &j: inserting)                                      // try all insertions (-i)
    if(j.is_object()) 
     for(auto &ins: j) rec[ins.label()] = ins;                  // merged by labels
    else
     cerr << "fail: only object could be merged with an object" << endl;
  }
 }

 write_json(r);
 return RC_OK;
}



int purge_json(CommonResources &r) {
 // remove all json nodes pointed by iterator(s)
 REVEAL(r, opt, DBG())

 if(opt[CHR(OPT_WLK)].hits() < 1) {
  cerr << "error: at least one -" STR(OPT_WLK) " must be given when purging" << endl;
  return RC_WLK_MISS;
 }

 if(opt[CHR(OPT_PRG)].hits() > 1)
  crop_out(r);
 else {
  for(auto &wp: opt[CHR(OPT_WLK)]) {                             // process all walks
   walk_vec ji = collect_walks(wp, r);                           // collect all purging points

   for(size_t i = 0; i < ji.size(); ++i) {                       // purge all walked instances
    auto & rec = *ji[i];
    if(ji[i].is_valid()) {                                       // i.e. hasn't been deleted already
     DBG(1) DOUT() << "purging walk instance " << i << endl;
     if(rec.is_root()) rec.clear();                              // erase everything
     else rec[-1].erase(ji[i]);
    }
   }
  }
 }

 write_json(r);
 return RC_OK;
}



bool remove_others(set<Jnode*> &ws, Jnode &jn) {
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
    if(remove_others(ws, *it) == true)                          // no walked entries inside
     { jn.erase(it); continue; }
    else remove = false;
   }
  }
  ++it;
 }
 return remove;
}



void crop_out(CommonResources &r) {
 // output walked elements preserving source json structure
 REVEAL(r, opt, json, DBG())

 // typedef vector<Json::iterator> walk_vec;
 set<Jnode*> walk_set;
 for(auto &wp: opt[CHR(OPT_WLK)])                               // process all walks
  for(auto it = json.walk(wp); it != json.end(); ++it)
   walk_set.insert(&it->value());
 DBG(0) DOUT() << "preserved instances: " << walk_set.size() << endl;
 
 remove_others(walk_set, json.root());
}



int update_json(CommonResources &r) {
 // update json in -u into all iterator(s) points
 REVEAL(r, opt, DBG())

 if(opt[CHR(OPT_WLK)].hits() < 1) {
  cerr << "error: at least one -" STR(OPT_WLK) " must be given when updating" << endl;
  return RC_WLK_MISS;
 }

 Json update;
 DBG().severity(update);
 if(opt[CHR(OPT_EXE)].hits() == 0)                              // this means, that -u is Json, then
  update.parse(opt[CHR(OPT_UPD)].str());                        // parse it

 for(auto &wp: opt[CHR(OPT_WLK)]) {                             // process all walks
  walk_vec ji = collect_walks(wp, r);                           // collect all update points

  for(size_t i = 0; i < ji.size(); ++i) {                       // go over all update (walk) points
   auto & rec = *ji[i];
   DBG(1) DOUT() << "trying to update walk instance " << i << endl;
   if(opt[CHR(OPT_EXE)].hits() == 0)                            // no -e was given
    rec = update;                                               // here we need to copy from source
   else {                                                       // -e was given, execute it
    update = OBJ{};
    execute_cli(update, rec, r);
    if(not update.empty())
     rec = move(update);                                        // update can be moved!
   }
  }
 }

 write_json(r);
 return RC_OK;
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



void execute_cli(Json &json, const Jnode &src_jnode, CommonResources &r) {
 // execute cli in -u option (interpolating src_jnode if required) and parse the result into json
 REVEAL(r, opt, DBG())

 stringstream is;
  bool pp = json.is_pretty();                                   // build a raw string of src_jnode
  json.raw();
  is << src_jnode;
  json.pretty(pp);
 string src_jnode_raw = is.str();

 is.str(string());                                              // clear input stream is
 for(auto opt_u_str: opt[CHR(OPT_UPD)]) {
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
 DBG().increment(+1, sh, -1);                                   // pop last space from input stream
 sh.system(is.str());

 if(sh.rc() != 0)
  { DBG(1) DOUT() << "shell returned error: " << sh.rc() << endl; return; }
 if(sh.stdout() == "")
  { DBG(1) DOUT() << "shell returned empty result, not updating" << endl; return; }

 json.parse(sh.stdout());
}



int swap_json(CommonResources &r) {
 // swap around nodes pointed by 2 walk paths
 REVEAL(r, opt)

 if(opt[CHR(OPT_WLK)].hits() != 2) {
  cerr << "error: exactly 2 -" STR(OPT_WLK) " must be given when swapping" << endl;
  return RC_WP_TWO;
 }

 vector<walk_vec> swaps{2};                                     // collect all walks in here
 swaps[0] = collect_walks(opt[CHR(OPT_WLK)].str(1), r);
 swaps[1] = collect_walks(opt[CHR(OPT_WLK)].str(2), r);

 size_t max_i = min(swaps[0].size(), swaps[1].size());
 for(size_t i = 0; i < max_i; ++i) {                            // swap only paired walks
  if(swaps[0][i].is_nested(swaps[1][i]))
   { cerr << "error: walk paths must not nest one another" << endl; return RC_SP_NST; }
  if(not swaps[0][i].is_valid() or not swaps[1][i].is_valid())
   { cerr << "error: walk path instance " << i << " became invalid" << endl; return RC_SP_INV; }
  swap(*swaps[0][i], *swaps[1][i]);
 }

 write_json(r);
 return RC_OK;
}


int walk_json(CommonResources &r) {
 // process demux sequential or interleaved walks
 REVEAL(r, opt, json, DBG())

 Json jdb;                                                      // integrity check in debugs only
 DBG(0) {
  DOUT() << "copying input json for integrity check (debug only)" << endl;
  jdb = json;
 }

 if(opt[CHR(OPT_WLK)].hits() >= 2 and not opt[CHR(OPT_SEQ)])     // no -n and more than 2 walks given
  walk_interleaved(r);
 else
  walk_sequentual(r);

 // check json integrity...
 DBG(0)
  DOUT() << "json integrity check: "
         << (jdb == json? "Good. (...phew!)": "BROKEN! (aaaa! panic! bug found!)" ) << endl;
 return RC_OK;
}



void walk_sequentual(CommonResources &r) {
 // process walks sequentially
 REVEAL(r, opt, json, jout, DBG())
 jout = ARY{};                                                  // output container for OPT_JSN

 for(auto &wp: opt[CHR(OPT_WLK)]) {                             // -n given, walk paths sequentially
  DBG(0) DOUT() << "walking path: " << wp << endl;

  for(auto jit = json.walk(wp, Json::keep_cache); jit != jit.end(); ++jit) {
   auto &rec = *jit;
    if( opt[CHR(OPT_JSN)] ) {                                   // -j option given
     walk_deq deq(1, jit);                                      // required by output_by_iterator
     output_by_iterator(deq, 0, r);
     continue;
   }                                                            // else, no -j given
   if( opt[CHR(OPT_LBL)] )                                      // -l given
    if(rec.has_label())                                         // if label exists
     cout << '"' << rec.label() << "\": ";
   cout << rec << endl;
  }
 }

 if( opt[CHR(OPT_JSN)] ) cout << jout << endl;                  // print if -j option
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

void walk_interleaved(CommonResources &r) {
 // process interleaved walks
 REVEAL(r, opt, json, jout, DBG())
 jout = ARY{};                                             // output container for -j
 vector<walk_deq> wpi;

 for(const auto &walk_str: opt[CHR(OPT_WLK)]) {                 // process all -w arguments
  if(walk_str.empty()) wpi.push_back( {json.walk("[^0]", Json::keep_cache)} );
  else wpi.push_back( {json.walk(walk_str, Json::keep_cache)} );    // push initial walk iterator
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
  print_walk(wpi, r);

 if( opt[CHR(OPT_JSN)] ) cout << jout << endl;
}



void print_walk(vector<walk_deq> &wpi, CommonResources &r) {
 // build front iterators offset matrix: wpi may contain empty deque
 vector<vector<long>> fom(wpi.size());                          // front offset matrix

 size_t longest_walk = build_front_matrix(fom, wpi, r);

 vector<size_t> actual_instances;
 for(size_t idx = 0; idx < fom.size(); ++idx)                   // init non-empty fom indices
  if(not fom[idx].empty()) actual_instances.push_back(idx);

 process_offsets(wpi, fom, longest_walk, actual_instances, r);
}



void process_offsets(vector<walk_deq> &wpi, vector<vector<long>> &fom, size_t longest_walk,
                     vector<size_t> &actuals, CommonResources &r) {
 // scans each offset's row (in wpi) and prints actual (non-empty) and relevant elements
 REVEAL(r, DBG())

 int grouping = 0;                                              // group size (negative locks value)
 DBG(2) DOUT() << "walking offsets";
 for(size_t offset = 0; offset < longest_walk; ++offset) {      // go across all offsets
  vector<size_t> new_ai;                                        // build new actuals in here
  long lowest_offset = LONG_MAX;                                // helper to build new actuals
  if(DBG()(2)) DOUT() << ", [" << offset << "]:";

  for(auto ai: actuals) {                                       // a.inst. are with lowest offset
   if(offset >= fom[ai].size()) {                               // more generic path, print first
    if(DBG()(2)) DOUT() << endl;
    DBG(2) DOUT() << "output instance: " << ai << ", group size: " << grouping << endl;
    return output_by_iterator(wpi[ai], abs(grouping), r);
   }
   if(DBG()(2)) DOUT() << ' ' << ai;

   if(fom[ai][offset] == -1 and grouping > 0)                   // i.e. a non-counter offset
    grouping = -grouping;                                       // lock grouping counter

   if(fom[ai][offset] < lowest_offset)                          // found a lower counter
    { lowest_offset = fom[ai][offset]; new_ai.clear(); }        // clear prior found actuals
   if(fom[ai][offset] == lowest_offset)                         // update new actuals
    new_ai.push_back(ai);
  }

  actuals = move(new_ai);                                       // update list of current actuals
  if(grouping >= 0) grouping = actuals.size();                  // update groping if not locked
  if(abs(grouping) == 1) break;                                 // only 1 walk (instance) left
 }

 if(DBG()(2)) DOUT() << endl;                                   // close debug line
 if(not actuals.empty()) {
  DBG(2) DOUT() << "output instance: " << actuals.front() << ", group size: " << grouping <<endl;
  output_by_iterator(wpi[actuals.front()], abs(grouping), r);
 }
 else                                                           // normally should never be the case
  wpi.clear();                                                  // in case, avoiding endless loop
}



size_t build_front_matrix(vector<vector<long>> &fom,
                          const vector<walk_deq> &wpi, CommonResources &r) {
 // build matrix from front iterator of each walk: using iterator's counter() method
 // that way it'll be possible to group relevant walk-paths during output
 REVEAL(r, DBG())

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



void output_by_iterator(walk_deq &wi, size_t group, CommonResources &cr) {
 // prints json element from given iterator, removes printed iterator from the dequeue
 // in case of -j option: collect into provided json container rather than print
 REVEAL(cr, opt, jout)

 static size_t last_group = 0;                                  // walking happens once per run,
 auto &sr = *(wi.front());                                      // sr is a super node (super record)

 if(opt[CHR(OPT_JSN)]) {                                        // -j given (jsonize output)
  if(not opt[CHR(OPT_LBL)]) jout.push_back(sr);                 // -l not given, make simple array
  else                                                          // -l given (combine relevant group)
   if(not sr.has_label()) jout.push_back(sr);                   // parent is root or not object
   else {                                                       // parent is an obect
    if(group > last_group or jout.empty())                      // time to create a new object
     jout.push_back( OBJ{} );
    if(not jout.back().is_object())
     jout.push_back( OBJ{} );
    if(jout.back().count(sr.label()) == 0)                      // no label recorded yet
     jout.back()[sr.label()] = sr;                              // copy supernode
    else {                                                      // label exist
     if(not jout.back()[sr.label()].is_array()) {               // and it's not an array
      Json tmp{ move(jout.back()[sr.label()]) };                // convert to array then
      (jout.back()[sr.label()] = ARY{}).push_back( move(tmp) );
     }
     jout.back()[sr.label()].push_back( sr );                   // & push back into converted array
    }
   }
 }
 else {                                                         // no -j option
  if(opt[CHR(OPT_LBL)] and sr.has_label())                      // -l given
   cout << '"' << sr.label() << "\": ";                         // then print label (if present)
  cout << sr << endl;
 }

 wi.pop_front();
 last_group = group;
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


- to select node "children" from John, run:
jtc -)" STR(OPT_WLK) R"( "[Relation][0][children]" example.json
[
   "Sophia",
   "Olivia"
]


- to select all children records individually (from John), run:
jtc -)" STR(OPT_WLK) R"( "[Relation] [0] [children] [+0]" example.json
"Sophia"
"Olivia"


- to select all children (from all parents) and their parents as well, run:
jtc -)" STR(OPT_WLK) R"("[Relation][+0][parent]" -)" STR(OPT_WLK) \
R"( "[Relation][+0][children][+0]" example.json
"John Smith"
"Sophia"
"Olivia"
"Anna Johnson"
"John"

- the same could be achieved through a bit more succint syntax:
jtc -)" STR(OPT_CMN) R"("[Relation][+0]" -)" STR(OPT_PRT) R"("[parent]" -)" STR(OPT_PRT) \
R"( "[children][+0]" example.json
- Here, all concatenations of an option -)" STR(OPT_CMN) R"( with every option -)" \
STR(OPT_PRT) R"( is transformed
  into -)" STR(OPT_WLK) \
R"( then compiled and executed. That syntax is supposed to save the input
  space when a common path is present in multiple walks


- to select all children, from the node whose parent's name starts with "John",
  run:
jtc -)" STR(OPT_WLK) R"( "[Relation]<^John>R[-1][children][+0]" example.json
"Sophia"
"Olivia"


- to add (insert) a child "James" to a parent whose name starts with "John",
  run:
jtc -)" STR(OPT_FRC) STR(OPT_WLK) R"( "[0] [parent]:<^John>R[-1][children]" -)" \
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
jtc -)" STR(OPT_FRC) STR(OPT_WLK) R"( "[0] <John Smith>" -)" STR(OPT_UPD) \
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
jtc -)" STR(OPT_FRC) STR(OPT_WLK) R"( "[0] [parent]:<Jane Smith> [-1]" -)" \
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
   "John Smith",
   "Anna Johnson"
]

if we throw in an option -)" STR(OPT_LBL) R"(, then output JSON format ensures that entries with
labels will be displayed accordingly:
jtc -)" STR(OPT_WLK) R"( '[Relation][+0][parent]' -)" STR(OPT_JSN) STR(OPT_LBL) R"( example.json
[
   {
      "parent": [
         "John Smith",
         "Anna Johnson"
      ]
   }
]


- finally, an update option -)" STR(OPT_UPD) R"( could be subjected for a shell cli evaluation,
  say we want to capitalize all parents names:
jtc -)" STR(OPT_WLK) R"('[parent]:<.*>R+0' -)" STR(OPT_EXE) STR(OPT_UPD)
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





















