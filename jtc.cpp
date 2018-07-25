#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <climits>      // LONG_MAX
#include "lib/Json.hpp"
#include "lib/getoptions.hpp"
#include "lib/dbg.hpp"


using namespace std;

#define VERSION "1.21"


// option definitions
#define OPT_RDT -
#define OPT_DBG d
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


// various return codes
#define RETURN_CODES \
        RC_OK, \
        RC_WLK_MISS, \
        RC_WP_TWO, \
        RC_SP_NST, \
        RC_SP_INV, \
        RC_END
ENUM(ReturnCodes, RETURN_CODES)

// return code exception offsets
#define OFF_GETOPT RC_END                                       // offset for Getopt exceptions
#define OFF_JSON (OFF_GETOPT + Getopt::end_of_trhow)            // offset for Json exceptions
#define OFF_REGEX (OFF_JSON + Jnode::end_of_trhow)              // offset for Regex exceptions



// holding common resources, and declaration helper macro
struct CommonResources {
    Getopt              opt;
    Json                json, jout;                             // source, output
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

int demux_opt(CommonResources &);
void read_json(CommonResources &);
int insert_json(CommonResources &);
int purge_json(CommonResources &);
int update_json(CommonResources &);
int swap_json(CommonResources &);
void collect_walks(vector<walk_vec> & swap_points, CommonResources &);
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
 
 opt.prolog("\nJSON test console. Version " VERSION \
            ", developed by Dmitry Lyssenko (ldn.softdev@gmail.com)\n");
 opt[CHR(OPT_DBG)].desc("turn on debugs (multiple calls increase verbosity)");
 opt[CHR(OPT_GDE)].desc("explain walk path syntax");
 opt[CHR(OPT_SZE)].desc("print json size (total number of nodes in json)");
 opt[CHR(OPT_RAW)].desc("force printing json in a raw format");
 opt[CHR(OPT_PRG)].desc("purge json elements (one or more -" STR(OPT_WLK) " must be given)");
 opt[CHR(OPT_SLD)].desc("enforce quoted solidus parsing");
 opt[CHR(OPT_SWP)].desc("swap around two json elements (two -" STR(OPT_WLK) " must be given)");
 opt[CHR(OPT_SEQ)].desc("do not print walks interleaved (i.e. print sequentually)");
 opt[CHR(OPT_LBL)].desc("print labels too (if any) for walked json");
 opt[CHR(OPT_JSN)].desc("list walked elements as json (see footnote on usage with -"
                        STR(OPT_LBL) ")");
 opt[CHR(OPT_IND)].desc("indent for pretty printing").bind("3").name("indent");
 opt[CHR(OPT_INS)].desc("insert json element (one or more -" STR(OPT_WLK) " must be given)")
                  .name("json");
 opt[CHR(OPT_UPD)].desc("update/replace json element (one or more -" STR(OPT_WLK) \
                        " must be given)").name("json");
 opt[CHR(OPT_WLK)].desc("a standalone walk path (multiple may be given)").name("walkpath");
 opt[CHR(OPT_CMN)].desc("a common part of a path, prepended to every specified -" STR(OPT_PRT))
                  .bind("").name("common");
 opt[CHR(OPT_PRT)].desc("an individual partial path, prepended by specified -" STR(OPT_CMN))
                  .name("partial");
 opt[CHR(OPT_FRC)].desc("apply changes into the file (instead of printing resulting json)");
 opt[0].desc("file to read json from").name("json_file").bind("<stdin>");
 opt.epilog("\nthis tool provides ability to:\n\
 - display JSON (in a raw and pretty formats)\n\
 - walk JSON using various addressing/search criteria (see -" STR(OPT_GDE) ")\n\
 - manipulate JSON via purge/insert/update/swap operations\n\
 for examples run with -" STR(OPT_GDE) " option\n\
\n\
Note on a multiple -" STR(OPT_WLK) " usage:\n\
 - if switch -" STR(OPT_SEQ) " is given, then all walk paths will be processed sequentually,\n\
   otherwise, paths would be grouped by relevance and walks are interleaved\n\
 - the walk path order is honored within the relevant groups only; the groups\n\
   are processed from most generic (least complex walk) to more specific (most\n\
   complex walk)\n\n\
Note on options -" STR(OPT_CMN) " and -" STR(OPT_PRT) " usage:\n\
 - these options must be given together: one -" STR(OPT_CMN) " (in case multiple given, only\n\
   the last one is considered) and multiple -" STR(OPT_PRT) "; each parameter -" STR(OPT_PRT) \
   " will be\n    prepended by parameter from -" STR(OPT_CMN) \
   ", tother they will form an equivalent of -" STR(OPT_WLK) "\n\n\
Note on options -" STR(OPT_JSN) " and -" STR(OPT_LBL) " usage:\n\
 - when -j is given w/o -l, then walked elements will be collected into a JSON\n\
   array; when used together, all walked elements will be grouped into relevant\n\
   objects within a parent array; those walked elements which do not have\n\
   labels will be enumerated within the parent array\n");

 // parse options
 try { opt.parse(argc,argv); }
 catch (stdException & e) { opt.usage(); return e.code() + OFF_GETOPT; }
 if(opt[CHR(OPT_GDE)]) return wp_guide();
 for(auto &partial: opt[CHR(OPT_PRT)])                          // concatenate -x+-y and put into -w
  opt[CHR(OPT_WLK)] = opt[CHR(OPT_CMN)].str() + partial;

 // prepare debugs
 json.tab(abs(opt[CHR(OPT_IND)]))
     .raw(opt[CHR(OPT_RAW)])
     .quoted_solidus(opt[CHR(OPT_SLD)]);
 DBG().level(opt[CHR(OPT_DBG)])
      .use_ostream(cerr)
      .severity(json, jout);

 // read json and execute as per options
 try {
  read_json(r);
  return demux_opt(r);
 }
 catch( stdException & e ) {
  cerr << opt.prog_name() << " exception: " << e.what() << endl;
  return e.code() + OFF_JSON;
 }
 catch (std::regex_error & e) {
  cerr << "regexp exception: " << e.what() << endl;
  return e.code() + OFF_REGEX;
 }
}





void read_json(CommonResources &r) {
 // read and parse json
 REVEAL(r, opt, json, DBG())

 ifstream file(opt[0].c_str(), ifstream::in);
 bool redirect = opt[CHR(OPT_RDT)].hits() != 0 or opt[0].hits() == 0;
 DBG(0)
  DOUT() << "start parsing json from file: "
         << (redirect? "<stdin>": opt[0].c_str()) <<endl;

 json.parse( string(istream_iterator<char>(redirect? cin>>noskipws : file>>noskipws),
                    istream_iterator<char>{}) );

 if(opt[CHR(OPT_SZE)])
  cout << "read json size: " << json.size() << endl;
}





void write_json(CommonResources &r) {
 // write updated JSON (i.e. resulting from -i/-u/-s/-p options into:
 // a) input json file (if -f given and if the input is not <stdin> ('-' option)
 // b) stdout
 REVEAL(r, opt, json, DBG())
 
 bool redirect = opt[CHR(OPT_RDT)].hits() != 0 or opt[0].hits() == 0;
 if(not opt[CHR(OPT_FRC)] or redirect)                          // stdout if no -f given,
  { cout << json << endl; return; }                             // or redirect '-' is present

 DBG(0) DOUT() << "updating changes into json file: " << opt[0].c_str() << endl;
 ofstream file(opt[0].c_str(), ofstream::out);
 file << json << endl;
}





int demux_opt(CommonResources &r) {
 // demultiplex functional options, execute only once
 REVEAL(r, opt, json)

 for(auto &op: opt)
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

 cout << json << endl;                                          // in case no exec options given
 return RC_OK;
}





int insert_json(CommonResources &r) {
 // if wp points to an array - insert json as it is.
 // if wp is an object - then json must be object type itself and inserted by labels
REVEAL(r, opt, DBG(), json)

 if(opt[CHR(OPT_WLK)].hits() < 1) {
  cerr << "error: at least one -" STR(OPT_WLK) " must be given when inserting" << endl;
  return RC_WLK_MISS;
 }

 Json inserting;
 DBG().severity(inserting);
 inserting.parse(opt[CHR(OPT_INS)].str());

 for(auto &wp: opt[CHR(OPT_WLK)]) {                             // process each walk
  walk_vec ji;                                                  // collect all insertion points
  for(auto it = json.walk(wp); it != json.end(); ++it)
   ji.push_back(it);
  DBG(0) DOUT() << "path: '" << wp << "', #instances: " << ji.size() << endl;

  for(size_t i = 0; i < ji.size(); ++i) {                       // insert json into walked instances
  auto & rec = *ji[i];
   DBG(1) DOUT() << "trying to insert into instance " << i << endl;
   if(rec.is_array())
    { rec.push_back(inserting); continue; }                     // insert into array
   if(not rec.is_object())
    { cerr << "fail: walk path must point to an iterable (" << i << ")" << endl; continue; }
   if(inserting.is_object()) {
    for(auto &ins: inserting) rec[ins.label()] = ins;         // insert into object
    continue;
   }
   cerr << "fail: only object could be inserted into an object" << endl;
  }
 }

 write_json(r);
 return RC_OK;
}





int purge_json(CommonResources &r) {
 // remove all json nodes pointed by iterator(s)
 REVEAL(r, opt, json, DBG())

 if(opt[CHR(OPT_WLK)].hits() < 1) {
  cerr << "error: at least one -" STR(OPT_WLK) " must be given when purging" << endl;
  return RC_WLK_MISS;
 }

 for(auto &wp: opt[CHR(OPT_WLK)]) {                             // process all walks
  walk_vec ji;                                                  // collect all purging points
  for(auto it = json.walk(wp); it != json.end(); ++it) ji.push_back(it);
  DBG(0) DOUT() << "path: '" << wp << "', #instances: " << ji.size() << endl;

  for(size_t i = 0; i < ji.size(); ++i) {                       // purge all walked instances
   auto & rec = *ji[i];
   if(ji[i].is_valid()) {                                       // i.e. hasn't been deleted already
    DBG(1) DOUT() << "purging walk instance " << i << endl;
    rec[-1].erase(ji[i]);
   }
  }
 }

 write_json(r);
 return RC_OK;
}





int update_json(CommonResources &r) {
 // update json in -u into all iterator(s) points
 REVEAL(r, opt, json, DBG())

 if(opt[CHR(OPT_WLK)].hits() < 1) {
  cerr << "error: at least one -" STR(OPT_WLK) " must be given when updating" << endl;
  return RC_WLK_MISS;
 }

 Json update;
 DBG().severity(update);
 update.parse(opt[CHR(OPT_UPD)].str());

 for(auto &wp: opt[CHR(OPT_WLK)]) {                             // process all walks
  walk_vec ji;                                                  // collect all update points
  for(auto it = json.walk(wp); it != json.end(); ++it) ji.push_back(it);
  DBG(0) DOUT() << "path: '" << wp << "', #instances: " << ji.size() << endl;

  for(size_t i = 0; i < ji.size(); ++i) {
   auto & rec = *ji[i];
   DBG(1) DOUT() << "trying to update walk instance " << i << endl;
   rec = update;
  }
 }

 write_json(r);
 return RC_OK;
}





int swap_json(CommonResources &r) {
 // swap around nodes pointed by 2 walk paths
 REVEAL(r, opt)

 if(opt[CHR(OPT_WLK)].hits() != 2) {
  cerr << "error: exactly 2 -" STR(OPT_WLK) " must be given when swapping" << endl;
  return RC_WP_TWO;
 }

 vector<walk_vec> swaps{2};                                     // collect all walks in here
 collect_walks(swaps, r);

 size_t maxi = min(swaps[0].size(), swaps[1].size());
 for(size_t i = 0; i < maxi; ++i) {                             // swap only paired walks
  if(swaps[0][i].is_nested(swaps[1][i]))
   { cerr << "error: walk paths must not nest one another" << endl; return RC_SP_NST; }
  if(not swaps[0][i].is_valid() or not swaps[1][i].is_valid())
   { cerr << "error: walk path instance " << i << " became invalid" << endl; return RC_SP_INV; }
  swap(*swaps[0][i], *swaps[1][i]);
 }

 write_json(r);
 return RC_OK;
}



void collect_walks(vector<walk_vec> & sp, CommonResources &r) {
 // load up all swap iterators (swap points), check walk restrictions
 REVEAL(r, opt, json)

 long i = 0;
 for(auto &wp: opt[CHR(OPT_WLK)]) {
  for(auto it = json.walk(wp); it != json.end(); ++it)
   sp[i].push_back(it);
  ++i;
 }
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

  for(auto jit = json.walk(wp); jit != jit.end(); ++jit) {
   auto &rec = *jit;
    if( opt[CHR(OPT_JSN)] ) {                                   // -j option given
     walk_deq deq(1, jit);                                      // required by output_by_iterator
     output_by_iterator(deq, 0, r);
     continue;
   }                                                            // else, no -j given
   if( opt[CHR(OPT_LBL)] )                                      // -l given
    if(not rec.is_root() and rec[-1].is_object())               // if label exists
     cout << '"' << rec.label() << "\": ";
   cout << rec << endl;
  }
 }

 if( opt[CHR(OPT_JSN)] ) cout << jout << endl;                  // print if -j option
}


// interleaved walks engaged with option -j and multiple walk paths are given
// processing of interleaved walks is somewhat complex:
// 1. collect all iterators produced by all walk paths (wpi)
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
  if(walk_str.empty()) wpi.push_back( {json.walk("[^0]")} );
  else wpi.push_back( {json.walk(walk_str)} );                  // push initial walk iterator
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
                     vector<size_t> &actual_instances, CommonResources &r) {
 // scans each offset's row (in wpi) and prints actual (non-empty) and relevant elements
 REVEAL(r, DBG())
 DBG(2) DOUT() << "walking offsets";
 for(size_t offset=0; offset<longest_walk; ++offset) {          // go across all offsets
  vector<size_t> new_ai;
  long lowest_offset = LONG_MAX;
  if(DBG()(2)) DOUT() << ", [" << offset << "]:";
  for(auto ai: actual_instances) {                              // a.inst. are with lowest offset
   if(offset >= fom[ai].size()) {                               // more generic path, print first
    if(DBG()(2)) DOUT() << endl;
    DBG(2) DOUT() << "output instance: " << ai
                  << ", actuals: " << actual_instances.size() << endl;
    output_by_iterator(wpi[ai], actual_instances.size(), r);
    return;
   }
   if(DBG()(2)) DOUT() << ' ' << ai;
   if(fom[ai][offset] < lowest_offset)
    { lowest_offset = fom[ai][offset]; new_ai.clear(); }
   if(fom[ai][offset] == lowest_offset)
    new_ai.push_back(ai);
  }
  actual_instances = move(new_ai);
  if(actual_instances.size() == 1) break;
 }
 if(DBG()(2)) DOUT() << endl;                                   // close debug line
 if(not actual_instances.empty()) {
  DBG(2) DOUT() << "output instance: " << actual_instances.front()
                << ", actuals: " << actual_instances.size() << endl;
  output_by_iterator(wpi[actual_instances.front()], actual_instances.size(), r);
 }
 else                                                           // normally should never be the case
  wpi.clear();                                                  // in case, avoiding endless loop
}



size_t build_front_matrix(vector<vector<long>> &fom,
                          const vector<walk_deq> &wpi, CommonResources &r) {
 // build matrix from front iterator of each walk: using iterator's counter() method
 REVEAL(r, DBG())

 size_t longest = 0;
 for(size_t idx = 0; idx < wpi.size(); ++idx) {
  auto & wi = wpi[idx];
  if(wi.empty()) continue;                                        // no more iterators in this walk

  DBG(2) DOUT() << "instance " << idx;
  for(size_t position=0; position<wi.front().walk_size(); ++position) {
   fom[idx].push_back( wi.front().counter(position) );
   if(DBG()(2)) DOUT() << (position==0? " front offsets matrix: ": ", ") << fom[idx].back();
  }
  if(DBG()(2)) DOUT() << endl;
  longest = max(longest, fom[idx].size());
 }
 return longest;
}



void output_by_iterator(walk_deq &wi, size_t actuals, CommonResources &cr) {
 // prints json element from given iterator, removes printed iterator from the dequeue
 // in case of -j option: collect into provided json container rather than print
 REVEAL(cr, opt, jout)
 static size_t last_actuals{0};                                 // walking happens once per run,
                                                                // so it's okay to make it static
 auto &r = *(wi.front());
 auto label_present = [&r](void){ return not r.is_root() and r[-1].is_object(); };
 auto start_new_object = [actuals](void){ return actuals >= last_actuals; };

 if(opt[CHR(OPT_JSN)]) {                                        // -j given
  if(opt[CHR(OPT_LBL)]){                                        // -l given
   if(label_present()) {                                        // label is present
    if(start_new_object() or not jout.has_children()) jout.push_back( OBJ{} );
    if(not jout.back().is_object()) jout.push_back( OBJ{} );
    if(jout.back().find(r.label()) == jout.back().end())        // no label recorded yet
     jout.back()[r.label()] = r;
    else {                                                      // label exist
     if(not jout.back()[r.label()].is_array())                  // and it's not an array
      jout.back()[r.label()] = ARY{ move(jout.back()[r.label()]) }; // convert to array
     jout.back()[r.label()].push_back( r );
    }
   }
   else                                                         // no label present
    jout.push_back(r);
  }
  else                                                          // no -l, just -j,
   jout.push_back(r);                                           // make a simple an array
 }
 else {                                                         // no -j option
  if(opt[CHR(OPT_LBL)] and label_present())                     // -l given
   cout << '"' << r.label() << "\": ";
  cout << r << endl;
 }

 wi.pop_front();
 last_actuals = actuals;
}



int wp_guide(void){
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
   "<txt>", "<txt>n", "<txt>+n", "<txt>s", "<txt>sn", "<txt>s+n"
   and respective reverse forms:
   ">txt<", ">txt<n", ">txt<+n", ">txt<s", ">txt<sn", ">txt<s+n",
   where txt - is any text to search for, s - an optional one letter suffix
   - if a lexeme is given using "<...>" encasement, then forward search is
     applied, otherwise (i.e. ">...<") - backward
   - "<text>": performs search  of "text" under a JSON tree off the given node
   among JSON strings only (default behavior).
   - optionally a one letter suffix could be used, either of these: [rlRLdbn],
     each one affecting the search in a following way:
     r - apply exact match (default) while searching string values,
     l - apply exact match while searching labels only,
     d - match a number (i.e. searching only numeric JSON values),
     b - match a boolean (i.e. searching only boolean values), true/false
         must be fully spelled, e.g.: "<true>b",
     n - match null values, the content within the encasement could be empty,
         or anything - it's ignored, e.g.: <>n, >null<Nn, etc
     R - same as r, but expression in braces is a Regex (regex search applied)
     L - same as l, but expression in braces is a Regex (regex search applied)
     D - same as d, but expression in braces is a Regex (regex search applied)
   n - an integer quantifier specifying search match instance, e.g.: "<text>2"
       will match only upon a 3rd (quantifiers are zero based) encounter of
       the word "text"
  +n - collects all search instances staring from n (zero based) that produce
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

(!) However, the same operation in reverse direction would fail:
    jtc -)" STR(OPT_WLK) R"( "[Relation]>^John<R[-1][children][+0]" example.json
    <empty output>
    Why? Any search operation (in that case, a reverse search of regexp "^John")
    continues only until a complete walk path produces failure (does not yield
    a match). In the example, search ">^John<R" would match on JSON value
    "John" - son of "Anna Johnson", but that JSON value's parent (which is an
    array), does not have the label "children", hence walk fails.
    A proper way to accomplish the task requires specifying search attached to
    the specific label "parent". Following example has fixed syntax:

jtc -)" STR(OPT_WLK) R"( "[Relation] [parent]:>^John<R [-1] [children] [+0]" example.json
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
)";
 return RC_OK;
}





















