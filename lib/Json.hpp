
/*
 * Created by Dmitry Lyssenko
 *
 * yet another JSON implementation featuring:
 *  - easy c++ API
 *  - walkable interface allowing iterate over JSON using offsets and search lexemes
 *
 *
 * 1. JSON construction
 *  a) DSL-like style. There are few constructors defined which allow building up
 *     JSON tree: NUL, BUL, NUM, STR, ARY, LBL, OBJ
 *
 *  Synopsis:
 *      Json json {NUL{}};      // or: Json json = {nullptr};
 *      json = BUL{true};       // or: json = {true};
 *      json = NUM{3.14};       // or: json = {3.14};
 *      json = STR{"string"};   // or: json = {"string"};
 *      json = ARY{ NUL{}, nullptr, false, 3.14159, json };
 *      json = OBJ{
 *               LBL{ "label 1", json },
 *               LBL{ "label 2",
 *                    ARY{ nullptr, true, 2, "three" } }
 *             };
 *
 *  For readability though it's best to spell all constructors (and maintain a
 *  DSL style):
 *      json = OBJ{
 *               LBL{ "label 1", json },
 *               LBL{ "label 2",
 *                    ARY{ NUL{}, BUL{true}, NUM{2}, STR{"three"} } }
 *             };
 *
 * CAUTION: be careful with DSL declarations - ARY and OBJ rely on std::initializer_list,
 *          which always produces copies of holding objects, thus move semantic is
 *          unachievable (as of C++14/17, might be lifted later)
 *
 *  b) build JSON by parsing the std::string:
 *      json.parse( "{ \"label 1\": [ null, true, 2, \"three\" ] }" );
 *
 *  ,or better use a raw string:
 *      json.parse( R"({ "label 1": [ null, true, 2, "three" ] })" );
 *
 *  ,or use a string with _json suffix:
 *      json = R"({ "label 1": [ null, true, 2, "three" ] })"_json;
 *
 *
 * 2. Accessing JSON
 *  Say, we have a following JSON:
 *      json = R"({
 *         "Address Book": [
 *             {
 *                 "Name": "Doc Brown",
 *                 "Phone": {
 *                     "Mobile": "+1-202-555-0134",
 *                     "Home": "+1-202-555-0123",
 *                     "Work": ["+1-202-555-0189", "+1-202-555-0188"]
 *                 },
 *                 "Address": {
 *                     "Home": {
 *                         "Street": "Riverside Drive, 1640",
 *                         "City": "Hill Valley",
 *                         "Zip": 12345,
 *                         "Country": "US"
 *                     }
 *                 }
 *             },
 *             {
 *                 "Name": "Sherlock Holmes",
 *                 "Phone": {
 *                     "Mobile": "+44 1632 960151",
 *                     "Home": "+44 1632 960118"
 *                 },
 *                 "Address": {
 *                     "Home": {
 *                         "Street": "Baker St, 221B",
 *                         "City": "London",
 *                         "Zip": "NW1 6XE",
 *                         "Country": "UK"
 *                     }
 *                 }
 *             }
 *         ]
 *      })"_json;
 *
 *  If location is known (e.g., a first entry in address book, a zip label), it
 *  could be accessed like this:
 *      json["Address Book"][0]["Address"]["Home"]["Zip"] = 90001;
 *
 *  i.e., a string subscript addresses a label in the node, numerical index addresses
 *  an ordinal element in the array
 *  the numerical index also can be used to address children in the object node,
 *  but that's fragile - you need to be sure of the order (which is alphabetical,
 *  btw), e.g.:
 *      json[0][0]["Address"]["Home"]["Zip"] = NUM{90001};
 *
 *  Each iterable provides access to the front and back elements through corresponding
 *  calls, e.g.:
 *      json.front().front()["Address"]["Home"].back() = NUM{90001};
 *
 *
 *  Iterating over iterable (objects and arrays) types (Jnode::iterator):
 *  say, we want to dump all the phones in the first entry of the address book:
 *      for(const auto &rec: json["Address Book"][0]["Phone"])
 *          std::cout << rec << std::endl;
 *
 *  Output:
 *  "+1-202-555-0134"
 *  "+1-202-555-0123"
 *  [
 *     "+1-202-555-0189",
 *     "+1-202-555-0188"
 *  ]
 *
 *  Actually, when iterating over a JSON, dereferencing an iterator returns a reference
 *  to a super node - that one contains methods label(), index() and value(), which
 *  allow accessing child's label (index in case of array) and its value respectively,
 *  though when accessing json super node value() method is quite optional. Thus,
 *  to output the same with labels:
 *      for(const auto &rec: json["Address Book"][0]["Phone"])
 *          std::cout << rec.label() << ": " << rec << std::endl;
 *
 *  Output:
 *  Home: "+1-202-555-0134"
 *  Mobile: "+1-202-555-0123"
 *  Work: [
 *     "+1-202-555-0189",
 *     "+1-202-555-0188"
 *  ]
 *
 * ATTN: do not attempt to use label() method on the Json node when it's not dereferenced
 *       from an iterator - an exception will be thrown
 *
 *  Note, any Json element is printed in JSON format (strings are quoted, arrays,
 *  object are enclosed into corresponding braces). If we want to access JSON's
 *  atomic values themselves there methods allowing accessing those:
 *      str() - returns const std::string value, type checked
 *      num() - type checked - return double type, type checked
 *      bul() - returns bool type, type checked
 *      val() - returns std::string value w/o type checking (actually it checks
 *              only if accessed value is atomic: numeric/boolean/string/null and
 *              neither of: array/object)
 *
 *  Internally, Json keeps all the atomic values as std::string along with associated
 *  type - Jtype (String, Number, Bool, Null).
 *  - num() for example, will convert a string to a double and return the value
 *    (but first will check if the accessed JSON has type Jtype::Number - if not
 *     it will throw 'expected_numerical_type' exception)
 *  - boolean values are stored internally as strings "T" and "F" respectively (along
 *    with Jtype::Bool)
 *  - null values are kept as empty string with Jtype::Null type
 *  if we want to access internal (std::string) representation of the atomic value
 *  w/o type checking - val() method to be used.
 *
 *  So, following code prints all phone numbers as native values:
 *      for(const auto &rec: json["Address Book"][1]["Phone"])
 *          std::cout << rec.label() << ": " << rec.str() << std::endl;
 *
 *  Output:
 *  Home: +44 1632 960151
 *  Mobile: +44 1632 960118
 *
 *  Another example: say, we want to dump all phone numbers (knowing those stored
 *  irregularly - as strings and arrays of strings):
 *      for(auto const &rec: json["Address Book"])
 *       if(rec.find("Phone") != rec.end()) {
 *        for(auto const & ph_rec: rec["Phone"])
 *         if(ph_rec.is_string())
 *          std::cout << rec["Name"].str() << ", "
 *                    << ph_rec.label() << ": " << ph_rec.str() << std::endl;
 *         else // if not a string then it's an array
 *          for(auto const &ph_num: ph_rec)
 *           std::cout << rec["Name"].str() << ", "
 *                     << ph_rec.label() << ": " << ph_num.str() << std::endl;
 *       }
 *
 *  Output:
 *  Doc Brown, Home: +1-202-555-0134
 *  Doc Brown, Mobile: +1-202-555-0123
 *  Doc Brown, Work: +1-202-555-0189
 *  Doc Brown, Work: 1-202-555-0188
 *  Sherlock Holmes, Home: +44 1632 960151
 *  Sherlock Holmes, Mobile: +44 1632 960118
 *
 * In addition to label() method, a super node also caters index() method - that
 * one return ordinal index of the JSON element when iterating over arrays.
 *
 *
 * 3. Extending Json.
 *  Extending objects is easy - just address a new one (by default it'll create
 *  an object-type) and assign a new Json to it:
 *      json["Address Book"][1]["email"] = STR{ "sherlock.holmes@gmail.com" };
 *
 *  for extending arrays use good old push_back():
 *      json["Address Book"].push_back( OBJ{ LBL{ "Name", STR{"Sirius Black"} } } );
 *
 *
 * 4. Erasing Json.
 *  non-atomic Json (a.k.a iterables) could be cleared (all their children destroyed):
 *      json["Address Book"][0]["Phone"]["Work"].clear();
 *
 *  - that would leave Doc Brown's with an empty list of work phones: []
 *  To remove an entry entirely, erase() method to be used, which has 3 overloads:
 *   1) erase(idx) - to erase an entry from the array by the index:
 *      json["Address Book"][0]["Phone"]["Work"].erase(1);
 *      - removes 2nd entry in "Work" array
 *
 *   2) erase("label") - to erase an entry in the object by label:
 *      json["Address Book"][0]["Phone"].erase("Work");
 *      - removes entire "Work" entry from "Phone" object
 *
 *   3) erase(iterator) - to erase an entry in the array/object by iterator
 *      auto it = json["Address Book"][0]["Phone"].find("Work");
 *      json["Address Book"][0]["Phone"].erase(it);
 *      - removes entry "Work" from "Phone" object
 *
 *  When erasing using an iterator - no need updating the iterator itself, method
 *  erase() will take care of this, so that iterator is valid and points to the
 *  next element, or to the iterable's end()).
 *
 *
 * 5. Other methods.
 *      to_string()     // returns Json/Jnode converted to std::string
 *
 *  there are a few methods to validate a Json node type:
 *      type()          // return Jnode::Jtype enum
 *      is_object()
 *      is_array()
 *      is_string()
 *      is_number()
 *      is_bool()
 *      is_null()
 *      is_iterable()   // i.e. array or or object
 *      is_atomic()     // i.e. string/number/bull/null
 *
 *  Json trees could be compared using '==' and '!=' operators.
 *      size() - calculates entire JSON tree size (number of nodes)
 *      has_children() - checks if given Json actually has any children (atomic
 *                       values would return false unconditionally
 *      children(void) - returns how many immediate children a JSON has (atomic
 *                       json always return 0)
 *
 *  Facilitating iterations:
 *      begin() - returns iterator / const_iterator
 *      end() - end of iterator / const_iterator
 *      find() - finds and returns an iterator among immediate children
 *      label() - returns a reference to an entry's label - can only be used by
 *                super nodes dereferenced from iterators over objects, otherwise
 *                'label_accessed_not_via_iterator' exception will be thrown
 *      index() - returns an entry's ordinal index - can only be used by super
 *                nodes dereferenced from iterators over arrays, otherwise
 *                'index_accessed_not_via_iterator' exception will be thrown
 *      value() - returns reference to a Jnode - optionally used by super nodes
 *                (methods label() and value() facilitate the same meaning as members
 *                'first' and 'second' when dereferencing map's iterators)
 *
 *  Global Json class printing option:
 *      pretty() - instructs to output JSON using pretty format
 *      raw() - instructs to output JSON in one line
 *      tab() - let setting tab size used for indention (default is 3)
 *
 *
 * 6. Walking JSON
 *  walk() is a class stateful feature, which let searching entire Json and iterate
 *  over immediate children as well as over matches for provided search criteria
 *
 *  a) Walk path:
 *   walk() method accepts a string, which represents a walk path; path is made
 *   of lexemes, which instruct how to traverse Json tree:
 *
 *   1) Offset lexemes: enclosed into square braces '[', ']'; provide various offset
 *      functions. Following notation are possible:
 *      [], [text], [n], [-n], [^n], [+n], [N:N], where n is an integer
 *      - empty offset "[]" matches empty label, e.g.: { "": "empty label" }
 *      - "[text]" offset selects a child in the node with the same label
 *      - "[n]" - numerical offset selects indexed element within iterable, e.g.:
 *        "[0]" selects 1st child (of array / object node) - all indices are zero
 *        based
 *      - "[-n]" - negative numerical offset backs off n levels up in Json's tree;
 *        path like "[0][-1]" selects 1st child in the node and then selects
 *        back its parent.
 *      - "[^n]" - similar to negative's but does reverse thing: for a built path
 *        vector descends n levels from the root; e.g.: following examples produce
 *        equal effect: "[0][1][3][-1]" is the same as "[0][1]" and the same as
 *        "[0][1][3][^2]"
 *      - "[+n]" numerical offset with '+' sign makes the path iterable, e.g.:
 *        [+1] selects 2nd element within immediate children and indicates that
 *        upon the next iteration a next child needs to be selected (i.e. 3rd, 4fth
 *        and so on)
 *      - "[N:N] range offset (same notation as in Python) selects all elements
 *        in the specified range. e.g.: [-3:] selects 3 last elements
 *      - Some notations duplicate each other, e.g.:
 *          [+0] and [:], or [+2] and [2:] and [+2:] have the same effect
 *
 *
 *   2) search lexemes: enclosed into angular braces <>, or ><, instructs to perform
 *      a textual match everywhere under the given json's tree. Following notations
 *      are possible:
 *      <txt>, <txt>n, <txt>+n, <txt>S, <txt>Sn, and reverse notations:
 *      >txt<, >txt<n, >txt<+n, >txt<S, >txt<Sn,
 *      where: txt - is any text to search for,
 *             S - optional one letter suffix,
 *             n - optional quantifier
 *      - if lexeme is given using <> encasement, then forward search is applied
 *        otherwise (i.e. >< encasement), search is performed only among immediate
 *        children in the given node
 *      "<text>": performs search of the string "text" within json tree under
 *                the given node
 *      - optionally a one letter suffix altering search behavior.
 *        for full description see `jtc_guide.hpp`
 *
 *   3) An offset lexeme ([...]) could be concatenated with a search lexeme (either
 *      <..> or >..<) over the colon sign (':'), e.g.: [some label]:<some text>
 *      - such syntax signifies a search (with all due suffixes and quantifiers,
 *       if any) only among Json values attached to the specified label
 *
 *   4) in both lexeme types it's allowed to include inner brackets, but closing
 *      one has to be quoted with preceding backslash '\', e.g.: "[case[0\]]"
 *      specifies an offset lexeme to the label "case[0]", <tag<a\>> - specifies
 *      a search lexeme for the text "tag<a>"
 *
 *
 *  Spaces between lexemes are allowed and ignored. No spaces allowed between suffixes.
 *  If offset lexeme contains any spaces it's classified as textual offset, e.g:
 *  [ 1] and [1 ] are treated as respective textual offsets " 1" and "1 " rather
 *  than a numerical offset 1 (i.e. numerical offsets must contain no spaces within
 *  brackets).
 *  If a numerical label (e.g. label "123") to be searched, then applying rule 3)
 *  walk path could be specified as: "[123]:<.*>R"
 *  - when concatenated, label is always considered to be a text offset
 *
 *
 *  b) Iterating over walked path
 *   walk() returns an iterator pointing to a found Json node. If nothing is found,
 *   root's Json::end() is always returned by this method.
 *
 *   If path is iterable (i.e. contains iterable lexemes), then upon an iteration
 *   a next walked match instance is returned and so on until nothing is found and
 *   Json::end() is returned.
 *
 *   If path is non-iterable (does not have iterable lexemes), then upon an iteration
 *   root's children's Json::end() is returned.
 *   The path may contain multiple iterable lexemes
 *
 *   Unlike Jnode iterator (which is bidirectional), walk iterator (or Json::iterator)
 *   is a forward type and provides some extra methods:
 *      - begin()           // returns reference to itself, so that walk() method
 *                          // could be used in range for-loops
 *      - end()             // for the same purpose, returns Json::root's end()
 *      - is_valid()        // if upper json node was erased, this method let
 *                          // know if iterator is still valid (part of json)
 *      - is_nested()       // checks if both (callee and called) nest one another
 *      - incremented()     // does the same as operator++, but returns true or
 *                          // false if iterator was successfully incremented or
 *                          // reached its end
 *   these iterator methods return properties of walk state:
 *      - walk_size()       // returns a size of the parsed walk state
 *      - counter(pos)      // returns -1 if position in walk step is not iterable,
 *                          // otherwise returns current offset counter
 *      - instance(pos)     // returns -1 if position is not a regular match instance,
 *                          // otherwise returns instance number to match
 *      - offset(pos)       // returns walk state offset
 *      - lexeme(pos)       // returns string lexeme (w/o suffix or quantifier)
 *      - type(pos)         // returns classified search type (Json::Jsearch)
 *
 *    A super node of dereferenced walk iterator also has some extra capabilities
 *    (in addition) to Jnode's:
 *      - is_root()         // checks if dereferenced node is root (returns true)
 *      - operator[]        // indexing operator adds a super power: a negative
 *                          // argument will return reference to its parents, e.g:
 *                          // [-1] will address node's immediate parent,
 *                          // [-2] will address parent of a parent, and so on;
 *                          // too low value (when there are no more parents to
 *                          // resolve) always returns reference to a root node;
 *                          // positive index does the same - address immediate
 *                          // children
 *
 *
 *  c) an optional second parameter for walk() is CacheState, which defaults to
 *   'invalidate' - i.e. upon every new walk the entire search cache will be
 *   invalidated (cleared). If a user wants to keep the cache, he need to specify
 *   'keep_cache' keyword explicitly (at least the decision is conscious)
 *   CAUTION: Keeping the search cache after JSON has been modified may lead to
 *            undefined behavior.
 *
 *
 *  Some examples:
 *  Let's iterate over all labels "Street" in above JSON:
 *      for(const auto &rec: json.walk("[Address Book] <Street>l+0"))
 *          std::cout << "Street: " << rec << std::endl;
 *
 *  Output:
 *  Street: "Riverside Drive, 1640"
 *  Street: "Baker St, 221B"
 *
 *
 *  Let's print a full address of a person whose first name is "Doc". This is a
 *  sloppy way of doing it:
 *      for(const auto &rec: json.walk("[Address Book] <^Doc>R [-1] [Address]"))
 *          std::cout << "address: " << rec << std::endl;
 *
 *  Output:
 *  address: {
 *     "Home": {
 *        "City": "Hill Valley",
 *        "Country": "US",
 *        "Street": "Riverside Drive, 1640",
 *        "Zip": 90001
 *     }
 *  }
 *
 *
 *  The code above is prone to a failure if there was another element starting with
 *  the word "Doc " too. A cleaner way of achieving the same would be:
 *      for(const auto &rec: json.walk("[Address Book] <Name>l+0"))
 *          if(rec.str().find("Doc ") == 0) {
 *              std::cout << "address: " << rec[-1]["Address"] << std::endl;
 *              break;
 *          }
 *
 *
 *  Even nicer way to achieve the same just relying on the walk feature:
 *      for(const auto &rec: json.walk("[Address Book] [Name]:<^Doc>R [-1] [Address]"))
 *       std::cout << "address: " << rec << std::endl;
 *
 *
 * 7. About iterators
 *  Json class is a wrapper for underlying Jnode class, which actually implements
 *  JSON tree. Internally, Jnode class stores both JSON types arrays and objects
 *  in std::map container
 *
 *  walk() method returns Json::iterator, while json's begin() method returns
 *  Jnode::iterator
 *  Given that both types of iterators are based on the same underlying, it's possible
 *  to compare Json::iterator with Jnode::iterator (only '==' and '!=' operators)
 *
 *  When iterator (either) is dereferenced, it returns a reference to a supernode,
 *  which in turn s child class from Jnode. The supernode is catered by the iterator
 *  and thus the supernode's lifetime is the same as iterator's from which it was
 *  dereferenced.
 *
 * 8. Callbacks
 *  there provided 2 types of callbacks for event-driven processing.
 *  Once callbacks are plugged and engaged, they'll get fired only upon performing
 *  recursive search walks e.g. from <..> types of walking.
 *    a) label based callback (will fire upon matching the label)
 *    b) walk based callback (will fire upon matching walk-iterator)
 *
 *  Following API facilitates user interface:
 *      - is_engaged()      // checks if callbacks engaged (or any specific type is engaged)
 *      - engage_callbacks()// arms / disarms callbacks
 *      - callback()        // overload for 2 parameters: string (labels) and walks callback types
 *      - clear_callbacks() // clears all all prior plugged callbacks (engage status not touched)
 *      - lbl_callbacks()   // returns map hosting labeled callbacks { label -> callback }
 *      - wlk_callbacks()   // returns vector<WlkCallback> hosting walk callbacks
 *                          // WlkCallback is a struct catering iterator itself and callback
 *
 * Json class is DEBUGGABLE - see "dbg.hpp"
 */

#pragma once

#include <exception>
#include <vector>
#include <map>
#include <string>
#include <functional>           // function objects
#include <sstream>              // std::stringstream
#include <utility>              // std::forward, std::move, std::make_pair, ...
#include <algorithm>            // std::min
#include <limits>               // numeric_limits
#include <climits>              // LONG_MAX, LONG_MIN
#include <iomanip>              // std::setprecision
#include <initializer_list>
#include <regex>
#include "extensions.hpp"
#include "dbg.hpp"
#include "Outable.hpp"
//#include "Blob.hpp"             // SERDES interface (not needed so far)



// Class design notes:
// Jnode represent a single JSON value of any kind (from null to object).
// 1. for storing atomic JSON values (null, bool, string, number) an std:string
//    is used - internally all JSON atomic values are stored like that, type/value
//    validation occurs only during parsing.
// 2. JSON's Arrays and Objects are recurrent structures, which have to be stored
//    in STL containers. Both arrays and objects could be stored using the same
//    container type. Considering all implications, std::map it seems the most
//    suitable container for JSON:
//    - std::map caters random point remove/insert operations efficiently,
//    - label search is O(log(n)) complexity; iteration is not much worse than
//      vector's
//    - for Array storage values (which do not require JSON labels), map keys will
//      be auto-sequenced


#define DBG_WIDTH 74                                            // max print len upon parser's dbg
#define ARRAY_LMT 4                                             // #bytes per array's index
#define KEY first                                               // semantic for map's pair
#define VALUE second                                            // instead of first/second
#define GLAMBDA(FUNC) [this](auto&&... arg) { FUNC(std::forward<decltype(arg)>(arg)...); }

#define PRINT_PRT '\n'                                          // pretty print separator
#define PRINT_RAW ' '                                           // raw print separator
#define CHR_TRUE 'T'                                            // designator for Json 'true' value
#define CHR_FALSE 'F'                                           // designator for Json 'false' val.
#define CHR_NULL '\0'                                           // end of string
#define CHR_EOL '\n'                                            // end of line
#define CHR_RTRN '\r'                                           // return char
#define CHR_QUOT '\\'                                           // json quotation char
#define STR_TRUE "true"                                         // Json true sting value
#define STR_FALSE "false"                                       // Json false sting value
#define STR_ANY "any"                                           // to match any boolean
#define STR_NULL "null"                                         // Json null sting value
#define JSN_OBJ_OPN '{'                                         // Json syntax char: open object
#define JSN_OBJ_CLS '}'                                         // Json syntax char: close object
#define JSN_ARY_OPN '['                                         // Json syntax char: open array
#define JSN_ARY_CLS ']'                                         // Json syntax char: close array
#define JSN_STRQ '"'                                            // Json syntax char: string open chr
#define JSN_ASPR ','                                            // Json syntax char: array separator
#define JSN_NUMM '-'                                            // Json syntax char: minus sign
#define JSN_NUMP '+'                                            // Json syntax char: plus sign
#define JSN_NUMD '.'                                            // Json syntax char: dot sign
#define LXM_SUB_OPN '['                                         // Walk lexeme open offset
#define LXM_SUB_CLS ']'                                         // Walk lexeme close offset
#define LXM_SCH_OPN '<'                                         // Walk lexeme open search
#define LXM_SCH_CLS '>'                                         // Walk lexeme close offset
#define LBL_SPR ':'                                             // Walk lexeme label separator
#define PFX_ITR '+'                                             // prefix of iterable offset
#define PFX_WFR '^'                                             // walk from root offset
#define PFX_WFL '-'                                             // walk from end-leaf offset
#define RNG_SPR ':'                                             // quantifier range separator
#define IDX_FIL '0'                                             // fill char for the node index
#define QNT_OPN '{'                                             // quantifier interpolation open
#define QNT_CLS '}'                                             // quantifier interpolation close


#define JSN_FBDN "\b\f\n\r\t"                                   // forbidden JSON chars
#define JSN_QTD "/\"\\bfnrtu"                                   // chrs following quotation in JSON
#define JSN_TRL "/\"\\\b\f\n\r\tu"                              // translated quoted chars

// NOTE: solidus quotation is optional per JSON spec, a user will have an option
//       to chose between desired quotation behavior





class Json;
class Jnode {
    friend class Json;

  friend std::ostream & operator<<(std::ostream & os, const Jnode & jnode)
                         { long rl{0}; return print_json_(os, jnode, rl); }

    friend void         swap(Jnode &l, Jnode &r) {
                         using std::swap;                       // enable ADL
                         auto & lv = l.value();                 // first resolve super node
                         auto & rv = r.value();
                         swap(lv.type_, rv.type_);
                         swap(lv.value_, rv.value_);
                         swap(lv.descendants_, rv.descendants_);
                        }

    typedef std::map<std::string, Jnode> map_jn;
    typedef map_jn::iterator iter_jn;
    typedef map_jn::const_iterator const_iter_jn;

 public:
    #define THROWREASON \
                start_of_jnode_exceptions, \
                type_non_indexable, \
                type_non_subscriptable, \
                type_non_iterable, \
                expected_string_type, \
                expected_numerical_type, \
                expected_boolean_type, \
                expected_object_type, \
                expected_array_type, \
                expected_atomic_type, \
                label_accessed_not_via_iterator, \
                index_accessed_not_via_iterator, \
                method_accessed_not_via_iterator, \
                label_request_for_non_object_enclosed, \
                index_request_for_non_array_enclosed, \
                index_out_of_range, \
                unexpected_end_of_quotation, \
                unexpected_quotation, \
                end_of_jnode_exceptions, \
                \
                /* now define Json exceptions */ \
                start_of_json_parsing_exceptions, \
                unexpected_end_of_string, \
                unexpected_end_of_line, \
                unquoted_character, \
                unexpected_character_escape, \
                expected_valid_label, \
                expected_json_value, \
                missing_label_separator, \
                missed_prior_enumeration, \
                invalid_number, \
                unexpected_trailing, \
                end_of_json_parsing_exceptions, \
                \
                /* walk iterator exceptions */ \
                start_of_walk_exceptions, \
                json_lexeme_invalid, \
                walk_offset_missing_closure, \
                walk_search_label_with_attached_label, \
                walk_expect_search_label, \
                walk_expect_lexeme, \
                walk_label_seprator_bad_usage,          /* e.g.: ...[0]:: */\
                walk_unexpexted_suffix, \
                walk_bad_number_or_suffix, \
                walk_negative_quantifier, \
                walk_empty_namesapce, \
                walk_bad_quantifier, \
                walk_unexpected_quantifier, \
                walk_root_non_iterable, \
                walk_empty_lexeme, \
                walk_non_empty_lexeme, \
                walk_bad_position, \
                walk_root_has_no_label, \
                walk_non_existant_namespace, \
                walk_non_numeric_namespace, \
                walk_a_bug, \
                end_of_walk_exceptions, \
                end_of_throw
    ENUMSTR(ThrowReason, THROWREASON)

    #define JTYPE \
                Object, \
                Array, \
                String, \
                Number, \
                Bool, \
                Null, \
                Neither
    ENUMSTR(Jtype, JTYPE)

    #define PRETTYTYPE \
                Pretty, \
                Raw
    ENUM(PrettyType, PRETTYTYPE)


                        Jnode(void) = default;                  // DC
                        Jnode(const Jnode &jn): Jnode() {       // CC
                         #ifdef BG_CC                           // -DBG_CC to turn on this debug
                          GUARD(jn.is_pretty, jn.pretty)
                          if(DBG()(0))
                           DOUT() << DBG_PROMPT(0) << "CC copying: " << jn.raw() << std::endl;
                         #endif
                         auto * volatile jnv = &jn.value();     // when walk iterator is copied its
                         if(jnv == nullptr)                     // supernode is empty, hence chck'n
                          { type_ = jn.type_; return; }         // supernode's type is parent's
                         // why volatile? compilers (mistakenly) believe that address of a returned
                         // reference can never be a null, hence optimize out above 2 lines, which
                         // leads to the crash inevitably. "volatile" disables such optimization
                         type_ = jnv->type_;
                         value_ = jnv->value_;
                         descendants_ = jnv->descendants_;
                        }

                        Jnode(Jnode &&jn) {                     // MC
                         auto * volatile jnv = &jn.value();     // same here: moved jn could be an
                         if(jnv == nullptr)                     // empty supernoe, hence checking
                          { std::swap(type_, jn.type_); return; }
                         swap(*this, jn);                       // swap will resolve supernode
                        }

    Jnode &             operator=(Jnode jn) {                   // CA, MA
                         swap(*this, jn);
                         return *this;
                        }

                        // type conversions from Json:
                        Jnode(Json j);

                        // atomic values constructor adapters:
                        Jnode(double x): type_{Number} {
                         std::stringstream ss;
                         ss << std::setprecision(std::numeric_limits<double>::digits10) << x;
                         value_ = ss.str();
                        }

                        Jnode(const std::string & s): type_{String}, value_{s} {}
                        Jnode(const char *s): type_{String}, value_{s} {}

                        template<typename T>
                        Jnode(T b, typename std::enable_if<std::is_same<T,bool>::value>
                                               ::type * = nullptr):
                         type_{Bool}, value_{b? CHR_TRUE: CHR_FALSE} {}
                        // w/o above concept it would clash with double type

                        template<typename T>
                        Jnode(T p, typename std::enable_if<std::is_null_pointer<T>::value>
                                               ::type * = nullptr):
                         type_{Null} {}


                        // JSON atomic type adapters (string, numeric, boolean):
                        operator const std::string & (void) const {
                         if(not is_string()) throw EXP(expected_string_type);
                         return str();
                        }

                        operator double (void) const {
                         if(not is_number()) throw EXP(expected_numerical_type);
                         return num();
                        }

                        operator bool (void) const {
                         if(not is_bool()) throw EXP(expected_boolean_type);
                         return bul();
                        }
                        // concept ensures in-lieu application (avoid clashing with string type)

                        // class interface:
    std::string         to_string(PrettyType pt = Pretty) const {
                         GUARD(Jnode::endl_);
                         std::stringstream ss;
                         ss << pretty(pt == Pretty);
                         return ss.str();
                        }

    Jtype               type(void) const { return value().type_; }
    Jtype &             type(void) { return value().type_; }
    bool                is_object(void) const { return type() == Object; }
    bool                is_array(void) const { return type() == Array; }
    bool                is_string(void) const { return type() == String; }
    bool                is_number(void) const { return type() == Number; }
    bool                is_bool(void) const { return type() == Bool; }
    bool                is_null(void) const { return type() == Null; }
    bool                is_iterable(void) const { return type() <= Array; }
    bool                is_atomic(void) const { return type() > Array; }

    size_t              size(void) const {                      // entire Jnode size
                         size_t size = 1;
                         for(auto &child: children_())
                          size += child.VALUE.size();
                         return size;
                        }

    bool                empty(void) const
                         { return type() <= Array? children_().empty(): false; }

    bool                has_children(void) const
                         { return type() <= Array? not children_().empty(): false; }

    size_t              children(void) const
                         { return children_().size(); }

    Jnode &             clear(void) {
                         if(is_atomic()) throw EXP(type_non_iterable);
                         children_().clear();
                         return *this;
                        }

    virtual Jnode &     operator[](long i) {
                         // long type is used instead of size_t b/c super node's overload
                         // supports negative offsets
                         if(is_atomic()) throw EXP(type_non_indexable);
                         return iterator_by_idx_(i)->VALUE;
                        }

    virtual Jnode &     operator[](int i)
                         { return operator[](static_cast<long>(i)); }

  virtual const Jnode & operator[](long i) const {
                         if(is_atomic()) throw EXP(type_non_indexable);
                         return iterator_by_idx_(i)->VALUE;
                        }

  virtual const Jnode & operator[](int i) const
                         { return operator[](static_cast<long>(i)); }

    Jnode &             operator[](const std::string & l) {
                         if(not is_object()) throw EXP(type_non_subscriptable);
                         return children_()[l];
                        }

    Jnode &             operator[](const char * l) {
                         if(not is_object()) throw EXP(type_non_subscriptable);
                         return children_()[l];
                        }

    const Jnode &       operator[](const std::string & l) const {
                         if(not is_object()) throw EXP(type_non_subscriptable);
                         return children_().at(l);
                        }

    const Jnode &       operator[](const char * l) const {
                         if(not is_object()) throw EXP(type_non_subscriptable);
                         return children_().at(l);
                        }

    Jnode &             front(void) {
                         if(not is_iterable()) throw EXP(type_non_iterable);
                         return children_().begin()->second;
                        }

    const Jnode &       front(void) const {
                         if(not is_iterable()) throw EXP(type_non_iterable);
                         return children_().begin()->second;
                        }

    const std::string & front_label(void) const {
                         if(not is_object()) throw EXP(expected_object_type);
                         return children_().begin()->first;
                        }

    Jnode &             back(void) {
                         if(not is_iterable()) throw EXP(type_non_iterable);
                         return children_().rbegin()->second;
                        }

    const Jnode &       back(void) const {
                         if(not is_iterable()) throw EXP(type_non_iterable);
                         return children_().rbegin()->second;
                        }

    const std::string & back_label(void) const {
                         if(not is_object()) throw EXP(expected_object_type);
                         return children_().rbegin()->first;
                        }

    bool                operator==(const Jnode &jn) const {
                         if(type() != jn.type()) return false;
                         if(is_iterable()) return children_() == jn.children_();
                         else return val() == jn.val();
                        }

    bool                operator!=(const Jnode &jn) const { return not operator==(jn); }

                        // access json types (type checked)
    const std::string & str(void) const {
                         if(not is_string()) throw EXP(expected_string_type);
                         return value().value_;
                        }

    double              num(void) const {
                         if(not is_number()) throw EXP(expected_numerical_type);
                         return stod(value().value_);
                        }

    bool                bul(void) const {
                         if(not is_bool()) throw EXP(expected_boolean_type);
                         return value().value_.front() == CHR_TRUE;
                        }

                        // return atomic value w/o atomic type checking
    const std::string & val(void) const {
                         if(is_iterable()) throw EXP(expected_atomic_type);
                         return value().value_;
                        }

                        // modify json
    Jnode &             erase(const std::string & l) {
                         if(not is_object()) throw EXP(expected_object_type);
                         children_().erase(l);
                         return *this;
                        }

    Jnode &             erase(size_t i) {
                         if(not is_array()) throw EXP(expected_array_type);
                         children_().erase(iterator_by_idx_(i));
                         return *this;
                        }

    Jnode &             push_back(Jnode jn) {
                         if(not is_array()) throw EXP(expected_array_type);
                         children_().emplace(next_key_(), std::move(jn));
                         return *this;
                        }


    Jnode &             pop_back(void) {
                         if(not is_iterable()) throw EXP(type_non_iterable);
                         if(not children_().empty())
                          children_().erase(std::prev(children_().end()));
                         return *this;
                        }

                        // iterators over children nodes/arrays in a node
                        // iterator capabilities:
                        // 1. return iterator: [c]begin/[c]end/find(string/index)
                        // 2. erase by iterator
   template<typename T> class Iterator;
    typedef Iterator<Jnode> iterator;
    typedef Iterator<const Jnode> const_iterator;

    iterator            begin(void);
    const_iterator      begin(void) const;
    const_iterator      cbegin(void) const;
    iterator            end(void);
    const_iterator      end(void) const;
    const_iterator      cend(void) const;
    Jnode &             erase(iterator & it);
    Jnode &             erase(const_iterator & it);
    Jnode &             erase(const_iterator && it);
    size_t              count(const std::string & l) const;
    iterator            find(const std::string & l);            // only for objects
    const_iterator      find(const std::string & l) const;      // only for objects
    iterator            find(size_t i);                         // for both arrays and objects
    const_iterator      find(size_t i) const;                   // for both arrays and objects

                        // facilitating super node powers
    virtual bool        has_label(void) const { throw EXP(label_accessed_not_via_iterator); }
    virtual const std::string &
                        label(void) const { throw EXP(label_accessed_not_via_iterator); }
    virtual bool        has_index(void) const { throw EXP(index_accessed_not_via_iterator); }
    virtual int64_t     index(void) const { throw EXP(index_accessed_not_via_iterator); }
    virtual bool        is_root(void) const { throw EXP(method_accessed_not_via_iterator); }
                        // label() / index() / is_root() supposed to be used by a super
                        // node only (cannot be pure defined, hence throwing)
    virtual Jnode &     value(void) { return *this; }           // for iterator
  virtual const Jnode & value(void) const { return *this; }     // for iterator & const_iterator

                        // global print setting
    bool                is_pretty(void) const { return endl_ == PRINT_PRT; }
    Jnode &             pretty(bool x = true)
                         { endl_ = x? PRINT_PRT: PRINT_RAW; return *this; }
    const Jnode &       pretty(bool x = true) const
                         { endl_ = x? PRINT_PRT: PRINT_RAW; return *this; }
    bool                is_raw(void) const { return endl_ == PRINT_RAW; }
    Jnode &             raw(bool x = true)
                         { endl_ = x? PRINT_RAW: PRINT_PRT; return *this; }
    const Jnode &       raw(bool x = true) const
                         { endl_ = x? PRINT_RAW: PRINT_PRT; return *this; }
    uint8_t             tab(void) const { return tab_; }
    Jnode &             tab(uint8_t n) { tab_ = n; return *this; }

    //SERDES(type_, value_, descendants_)                       // not really needed
    #ifdef BG_CC                                                // if -DBG_CC is given, otherwise
     DEBUGGABLE()                                               // no debugs in Jnode (typically)
    #endif
    EXCEPTIONS(ThrowReason)                                     // see "extensions.hpp"

 protected:
                        Jnode(Jtype t):type_{t} {}              // for internal use

    map_jn &            children_(void) { return value().descendants_; }
    const map_jn &      children_(void) const { return value().descendants_; }
    iter_jn             iterator_by_idx_(size_t idx);
    const_iter_jn       iterator_by_idx_(size_t idx) const;
    std::string         next_key_(void) const;

                        // Jnode data
    Jtype               type_{Object};
    std::string         value_;                                 // value (number/string/bool/null)
    map_jn              descendants_;                           // array/nodes (objects)

 private:
  static std::ostream & print_json_(std::ostream & os, const Jnode & me, long & rl);
  static std::ostream & print_iterables_(std::ostream & os, const Jnode & me, long & rl);

    static char         endl_;                                  // either for raw or pretty print
    static uint8_t      tab_;                                   // tab size (for indention)

};

// class static definitions
char Jnode::endl_{PRINT_PRT};                                   // default is pretty format
uint8_t Jnode::tab_{3};

STRINGIFY(Jnode::ThrowReason, THROWREASON)
#undef THROWREASON

STRINGIFY(Jnode::Jtype, JTYPE)
#undef JTYPE


// DSL style JSON constructors definitions:
// - when carrying of a performance, be aware, that passing arguments via initializer
// list could not be moved (only copied)

struct NUL: public Jnode {
    NUL(void): Jnode{Null} {}
};

struct BUL: public Jnode {
    BUL(bool x): Jnode{Bool}
     { value_ = x? CHR_TRUE: CHR_FALSE; }
};

struct NUM: public Jnode {
    NUM(double x): Jnode{Number} {
     std::stringstream ss;
     ss << std::setprecision(std::numeric_limits<double>::digits10) << x;
     value_ = ss.str();
    }
};

struct STR: public Jnode {
    STR(const std::string & x): Jnode{String}
     { value_ = x; }
    STR(std::string && x): Jnode{String}
     { value_ = std::move(x); }
};

struct ARY: public Jnode {
    ARY(const std::initializer_list<Jnode> & array): Jnode{Array} {
     for(auto &jn: array)
      children_().emplace(next_key_(), jn);
    }
};

struct LBL: public Jnode {
    LBL(const std::string & l, const Jnode & j): Jnode{std::move(j)}, label{std::move(l)} {}
    std::string         label;
};

struct OBJ: public Jnode {
    OBJ(const std::initializer_list<LBL> & labels): Jnode{Object} {
     for(auto &l: labels)
      children_().emplace(std::move(l.label), l);
    }
};





//                          Jnode iterator implementation
class Json;
template<typename T>
class Jnode::Iterator: public std::iterator<std::bidirectional_iterator_tag, T> {
 // this bidirectional iterator let iterating over children in given JSON iterable
 // (array or object)
 // once iterator is dereferenced it returns a reference to a Jnode's super-node,
 // super-node adds following powers to regular Jnode's:
 // - label() allows accessing node's label if node is nested by object
 // - index() allows accessing node's ordinal index if nested by array
 // - value() provides access to the Jnode itself (but this methods is somewhat redundant)
    friend Jnode;
    friend Json;
    friend void         swap(Jnode::Iterator<T> &l, Jnode::Iterator<T> &r) {
                         using std::swap;                           // enable ADL
                         swap(l.ji_, r.ji_);
                         swap(l.sn_.parent_type(), r.sn_.parent_type());// supernode requires
                        }                                           // swapping of type_ only

    // Supernode definition
    //
    class SuperJnode: public Jnode {
        friend Jnode;
        friend Jnode::Iterator<T>;

     public:
        bool                has_label(void) const
                             { return lbp_ != nullptr and parent_type() == Object; }
        const std::string & label(void) const {
                             if(type_ != Object) throw EXP(label_request_for_non_object_enclosed);
                             return *lbp_;
                            }
        bool                has_index(void) const
                             { return lbp_ != nullptr and parent_type() == Array; }
        int64_t             index(void) const {
                             if(type_ != Array) throw EXP(index_request_for_non_array_enclosed);
                             return std::stoul(lbp_->c_str(), nullptr, 16);
                            }
        Jnode &             value(void) { return *jnp_; }       // do not template
        const Jnode &       value(void) const { return *jnp_; } // these methods

        Jtype               parent_type(void) const { return type_; }
        Jtype &             parent_type(void) { return type_; } // this is a work around GNU's
                            // compiler bug/limitation, which does not extend scoping visibility
                            // onto subclasses: i.e. swap(l.sn_.type_, r.sn_.type_) fails.
     private:
                            SuperJnode(void) = delete;          // DC
                            SuperJnode(Jtype t): Jnode{t} {}    // Init Construct

        SuperJnode &        operator()(const std::string &s, Jnode &jn)
                             { lbp_ = &s; jnp_ = &jn; return *this; }
        const SuperJnode &  operator()(const std::string &s, const Jnode &jn) const
                             { lbp_ = &s; jnp_ = &jn; return *this; }

        const std::string * lbp_{nullptr};                      // pointer to a label string
        Jnode *             jnp_{nullptr};                      // resolved Jnode pointer
    };
    //
    // end of Super node definition

 public:
                        Iterator(void) = default;               // DC
                        Iterator(const Iterator &it):           // CC
                         ji_(it.ji_)
                          { sn_.type_ = it.sn_.type_; }
                        Iterator(Iterator &&it)                 // MC
                         { swap(*this, it); }
    Iterator &          operator=(Iterator it)                  // CA, MA
                         { swap(*this, it); return *this; }

                        // convert to const_iterator (from iterator)
                        operator Iterator<const Jnode>(void) const
                         { return {ji_, sn_.type_}; }

    bool                operator==(const iterator & rhs) const
                         { return underlying_() == rhs.underlying_(); }
    bool                operator!=(const iterator & rhs) const
                         { return underlying_() != rhs.underlying_(); }
    bool                operator==(const const_iterator & rhs) const
                         { return underlying_() == rhs.underlying_(); }
    bool                operator!=(const const_iterator & rhs) const
                         { return underlying_() != rhs.underlying_(); }
    T &                 operator*(void)
                         { return sn_(underlying_()->KEY, underlying_()->VALUE); }
    T *                 operator->(void)
                         { return &sn_(underlying_()->KEY, underlying_()->VALUE); }
    Iterator<T> &       operator++(void) { ++ji_; return *this; }
    Iterator<T> &       operator--(void) { --ji_; return *this; }
    Iterator<T>         operator++(int) { auto tmp{*this}; ++(*this); return tmp; }
    Iterator<T>         operator--(int) { auto tmp{*this}; --(*this); return tmp; }

 protected:
                        // constructor for iterator type:
                        template<typename Q = T>
                        Iterator(iter_jn && mi, Jtype jt,
                                 typename std::enable_if<not std::is_const<Q>::value>
                                             ::type * = nullptr):
                         ji_{mi}, sn_{jt} {}

                        // constructor for const_iterator type:
                        template<typename Q = T>
                        Iterator(const_iter_jn && mi, Jtype jt,
                                 typename std::enable_if<std::is_const<Q>::value>
                                             ::type * = nullptr):
                         ji_{reinterpret_cast<iter_jn&&>(mi)}, sn_{jt} {}

    // reminder: typedef std::map<std::string, Jnode>::iterator iter_jn;
    iter_jn             ji_;
    SuperJnode          sn_{Neither};

 private:
    iter_jn &           underlying_(void) { return ji_; }
    const iter_jn &     underlying_(void) const { return ji_; }

};


Jnode::iterator Jnode::begin(void) {
 if(is_atomic())
  throw EXP(type_non_iterable);
 return {children_().begin(), type()};
}


Jnode::const_iterator Jnode::begin(void) const {
 if(is_atomic())
  throw EXP(type_non_iterable);
 return {children_().begin(), type()};
}


Jnode::const_iterator Jnode::cbegin(void) const
 { return begin(); }


Jnode::iterator Jnode::end(void) {
 if(is_atomic())
  throw EXP(type_non_iterable);
 return {children_().end(), type()};
}


Jnode::const_iterator Jnode::end(void) const {
 if(is_atomic())
  throw EXP(type_non_iterable);
 return {children_().end(), type()};
}


Jnode::const_iterator Jnode::cend(void) const
 { return end(); }


Jnode & Jnode::erase(iterator & it) {
 if(is_atomic())
  throw EXP(type_non_iterable);
 it.underlying_() = children_().erase(it.underlying_());
 return *this;
}


Jnode & Jnode::erase(const_iterator & it) {
 if(is_atomic())
  throw EXP(type_non_iterable);
 it.underlying_() = children_().erase(it.underlying_());
 return *this;
}


Jnode & Jnode::erase(const_iterator && it) {
 if(is_atomic())
  throw EXP(type_non_iterable);
 children_().erase(it.underlying_());
 return *this;
}


size_t Jnode::count(const std::string & l) const {
 if(not is_object())
  throw EXP(expected_object_type);
 return children_().count(l);
}


Jnode::iterator Jnode::find(const std::string & l) {
 if(not is_object())
  throw EXP(expected_object_type);
 return {children_().find(l), type()};
}


Jnode::const_iterator Jnode::find(const std::string & l) const {
 if(not is_object())
  throw EXP(expected_object_type);
 return {children_().find(l), type()};
}


Jnode::iterator Jnode::find(size_t idx) {
 if(not is_iterable())
  throw EXP(type_non_iterable);
 return {iterator_by_idx_(idx), type()};
}


Jnode::const_iterator Jnode::find(size_t idx) const {
 if(not is_iterable())
  throw EXP(type_non_iterable);
 return {iterator_by_idx_(idx), type()};
}


//
// Jnode private methods implementation
//
Jnode::iter_jn Jnode::iterator_by_idx_(size_t idx) {
 // iterator_by_idx_ may be used in both array and dictionary indexing operation.
 if(idx >= children_().size())
  throw EXP(index_out_of_range);

 if(is_array()) {                                               // array could be addressed direct
  size_t key = stoul(children_().rbegin()->KEY, nullptr, 16);
  if(key < children_().size()) {                                // if so, indices are non-tampered
   std::stringstream ss;
   ss << std::hex << std::setfill(IDX_FIL) << std::setw(ARRAY_LMT * 2) << idx;
   return children_().find( ss.str() );
  }
 }                                                              // else: traverse map
 auto it = children_().begin();
 std::advance(it, idx);
 return it;
}


Jnode::const_iter_jn Jnode::iterator_by_idx_(size_t idx) const {
 // const version
 if(idx >= children_().size())
  throw EXP(index_out_of_range);

 if(is_array()) {                                               // array may be addressed directly
  size_t key = stoul(children_().rbegin()->KEY, nullptr, 16);
  if(key < children_().size()) {                                // if so, indices are non-tampered
   std::stringstream ss;
   ss << std::hex << std::setfill(IDX_FIL) << std::setw(ARRAY_LMT * 2) << idx;
   return children_().find(std::move(ss.str()));
  }
 }                                                              // else: traverse map
 auto it = children_().cbegin();
 std::advance(it, idx);
 return it;
}


std::string Jnode::next_key_(void) const {
 size_t key = 0;
 if(not children_().empty())
  key = stoul(children_().rbegin()->KEY, nullptr, 16) + 1;
 std::stringstream ss;
 ss << std::hex << std::setfill(IDX_FIL) << std::setw(ARRAY_LMT * 2) << key;

 return ss.str();
}


std::ostream & Jnode::print_json_(std::ostream & os, const Jnode & me, long & rl) {
 auto & my = me.value();                                        // resolve if virtual object
 switch(my.type()) {
  case Object:
        os << JSN_OBJ_OPN;
        if(my.empty()) return os << JSN_OBJ_CLS;
        os << endl_;
        break;
  case Array:
        os << JSN_ARY_OPN;
        if(my.empty()) return os << JSN_ARY_CLS;
        os << endl_;
        break;
  case Bool:
        return os << (my.bul()? STR_TRUE: STR_FALSE);
  case Null:
        return os << STR_NULL;
  case Number:
        return os << my.val();
  case String:
        return os << JSN_STRQ << my.str() << JSN_STRQ;
  default:
        return os;                                              // ignore unknown type
 }
 return print_iterables_(os, my, rl);
}


std::ostream & Jnode::print_iterables_(std::ostream & os, const Jnode & my, long & rl) {
 // process children in iterables (array or node)
 if(endl_ == PRINT_PRT) ++rl;                                   // if pretty print - adjust level

 for(auto & child: my.children_()) {                            // print all children:
  os << std::string(rl * tab_, ' ');                            // output current indent
  if(not my.is_array())                                         // if parent (me) is not Array
   os << JSN_STRQ << child.KEY << JSN_STRQ << ": ";             //  print label
  print_json_(os, child.VALUE, rl)                              // then print child itself and the
   << (child.KEY != my.children_().rbegin()->KEY? ",": "")      // trailing comma if not the last
   << endl_;
 }

 if(rl > 1) os << std::string((rl - 1) * tab_, ' ');            // would also signify pretty print
 if(my.is_array()) os << JSN_ARY_CLS;                           // close array, or
 if(my.is_object()) os << JSN_OBJ_CLS;                          // close node (object)
 if(endl_ == PRINT_PRT) --rl;                                   // if pretty print - adjust level

 return os;
}





class Json {
  friend std::ostream & operator<<(std::ostream & os, const Json & my)
                         { return os << my.root(); }

    #define PARSE_THROW \
                may_throw, \
                dont_throw, \
                must_be_positive, /* actually positive or 0, '+' */ \
                must_be_negative, /* negative or 0 */ \
                must_be_signless, /* number without '+' or '-' signs */ \
                may_be_any
    ENUM(ParseThrow, PARSE_THROW)

 public:
    #define JS_ENUM \
                regular_match,  /* string match */ \
                Regex_search,   /* RE match for JSON strings only */ \
                digital_match,  /* numerical match */ \
                Digital_regex,  /* RE match for JSON numericals only */ \
                boolean_match,  /* searches among JSON booleans only */ \
                null_match,     /* matches Json null only */ \
                label_match,    /* searches among JSON object labels only */ \
                Label_RE_search,/* RE match among JSON object labels only */ \
                atomic_match,   /* JSON atomic, matches any of: string/number/boolean/null */ \
                object_match,   /* JSON object, matches any object {..} */ \
                indexable_match,/* JSON array, matches any of: [..] */ \
                collection_match,/* JSON array & objects */ \
                end_node_match, /* matches any of: atomics, [], {} */ \
                wide_match,     /* matches any JSON: atomics + objects + arrays */ \
                json_match,     /* matches provided JSON */ \
                search_from_ns, /* matches provided JSON from the namespace */ \
                tag_from_ns,    /* use namespace to find a label/index */ \
                query_unique,   /* use namespaces to find unique JSONs */ \
                Query_duplicate,/* use namespaces to find duplicates JSONs (opposite to q) */ \
                /* following are directives, not search lexemes: */ \
                value_of_json,  /* directive: save current json value */ \
                key_of_value,   /* directive: treat key (label/index) as a value (and save) */ \
                zip_namespace,  /* directive: erase namespace */ \
                fail_stop,      /* directive: stop at the preserved point when ws fails */ \
                Forward_itr,    /* directive: proceed to the next iteration (w/o fail-stops) */ \
                /*user_specific,  directive: user-specific callback */ \
                end_of_lexemes, \
                text_offset,    /* in addition to search, these two used for subscript offsets */ \
                numeric_offset  /* to disambiguate numeric subscripts [0], from textual [zero] */
    ENUMSTR(Jsearch, JS_ENUM)
    // Jsearch is defined so that its first letter corresponds to the suffix

    #define PARSETRAILING \
                /* "relaxed" actions do not throw */ \
                relaxed_trailing,   /* default, allow spaces past valid json, then stops */ \
                relaxed_no_trail,   /* stops right after json, ep_ points to the next char */ \
                /* "strict" actions always throw if described violation */ \
                strict_trailing,    /* allow only white spaces past valid json, throw otherwise */\
                strict_no_trail     /* don't allow anything past json, throw otherwise */
    ENUM(ParseTrailing, PARSETRAILING)

    #define CACHE_STATE \
                invalidate, \
                keep_cache
    ENUM(CacheState, CACHE_STATE)

    typedef std::map<std::string, Jnode> map_jn;


                        Json(void) = default;
                        Json(const Jnode &jn): root_{jn.value()} { }
                        Json(Jnode &&jn): root_{std::move(jn.value())} { }
                        Json(const std::string &str) { parse(str); }
                        Json(const char * c_str) { parse( std::string{c_str} ); }

    // class interface:
    Jnode &             root(void) { return root_; }
    const Jnode &       root(void) const { return root_; }
    Json &              parse(const std::string & jstr, ParseTrailing trail = relaxed_trailing)
                         { return parse(jstr.begin(), trail); }
    Json &              parse(std::string::const_iterator && jsi,
                              ParseTrailing trail = relaxed_trailing)
                         { return parse(jsi, trail); }
    Json &              parse(std::string::const_iterator & jsi,
                              ParseTrailing trail = relaxed_trailing);
    std::string::const_iterator
                        exception_point(void) { return ep_; }
    class iterator;
    iterator            walk(const std::string & walk_string = "", CacheState = invalidate);

    // relayed Jnode interface
    std::string         to_string(Jnode::PrettyType pt = Jnode::Pretty) const
                         { return root().to_string(pt); }
    std::string         unquote_str(std::string && src) const;
    std::string         unquote_str(const std::string & str) const
                         { std::string src{str}; return unquote_str(std::move(src)); }
    std::string         inquote_str(std::string && src) const;
    std::string         inquote_str(const std::string & str) const
                         { std::string src{str}; return inquote_str(std::move(src)); }
    Jnode::Jtype        type(void) const { return root().type(); }
    Jnode::Jtype &      type(void) { return root().type(); }
    bool                is_object(void) const { return root().is_object(); }
    bool                is_array(void) const { return root().is_array(); }
    bool                is_string(void) const { return root().is_string(); }
    bool                is_number(void) const { return root().is_number(); }
    bool                is_bool(void) const { return root().is_bool(); }
    bool                is_null(void) const { return root().is_null(); }
    bool                is_iterable(void) const { return root().is_iterable(); }
    bool                is_atomic(void) const { return root().is_atomic(); }
    size_t              size(void) const { return root().size(); }
    bool                empty(void) const { return root().empty(); }
    bool                has_children(void) const { return root().has_children(); }
    size_t              children(void) const { return root().children(); }
    Json &              clear(void) { root().clear(); return *this; }
    Jnode &             operator[](long i) { return root()[i]; }
    Jnode &             operator[](int i) { return root()[i]; }
    const Jnode &       operator[](long i) const { return root()[i]; }
    const Jnode &       operator[](int i) const { return root()[i]; }
    Jnode &             operator[](const std::string & l) { return root()[l]; }
    const Jnode &       operator[](const std::string & l) const { return root()[l]; }
    Jnode &             front(void) { return root().front(); }
    const Jnode &       front(void) const { return root().front(); }
    const std::string & front_label(void) const { return root().front_label(); }
    Jnode &             back(void) { return root().back(); }
    const Jnode &       back(void) const { return root().back(); }
    const std::string & back_label(void) const { return root().back_label(); }
    bool                operator==(const Json &j) const { return root() == j.root(); }
    bool                operator!=(const Json &j) const { return root() != j.root(); }
    bool                operator==(const Jnode &j) const { return root() == j; }
    bool                operator!=(const Jnode &j) const { return root() != j; }
    const std::string & str(void) const { return root().str(); }
    double              num(void) const { return root().num(); }
    bool                bul(void) const { return root().bul(); }
    const std::string & val(void) const { return root().val(); }
    Json &              erase(const std::string & l) { root().erase(l); return *this; }
    Json &              erase(size_t i) { root().erase(i); return *this; }
    Json &              push_back(Jnode jn)
                         { root().push_back(std::move(jn)); return *this; }
    Json &              pop_back(void)
                         { root().pop_back(); return *this; }

    Jnode::iterator     begin(void) { return root().begin(); }
  Jnode::const_iterator begin(void) const { return root().begin(); }
  Jnode::const_iterator cbegin(void) const { return root().cbegin(); }
    Jnode::iterator     end(void) { return root().end(); }
  Jnode::const_iterator end(void) const { return root().end(); }
  Jnode::const_iterator cend(void) const { return root().cend(); }
    Json &              erase(Jnode::iterator & it) { root().erase(it); return *this; };
    Json &              erase(Jnode::const_iterator & it) { root().erase(it); return *this; };
    size_t              count(const std::string & l) const { return root().count(l); };
    Jnode::iterator     find(const std::string & l) { return root().find(l); }
  Jnode::const_iterator find(const std::string & l) const { return root().find(l); }
    Jnode::iterator     find(size_t i) { return root().find(i); }
  Jnode::const_iterator find(size_t i) const { return root().find(i); };

    bool                is_pretty(void) const { return root().is_pretty(); }
    Json &              pretty(bool x = true) { root().pretty(x); return *this; }
    const Json &        pretty(bool x = true) const { root().pretty(x); return *this; }
    bool                is_raw(void) const { return root().is_raw(); }
    Json &              raw(bool x = true) { root().raw(x); return *this; }
    const Json &        raw(bool x = true) const { root().raw(x); return *this; }
    uint8_t             tab(void) const { return root().tab(); }
    Json &              tab(uint8_t n) { root().tab(n); return *this; }
    bool                is_solidus_quoted(void) const { return jsn_fbdn_[0] == '/'; }
    Json &              quote_solidus(bool quote)
                         { jsn_fbdn_ = quote? "/" JSN_FBDN: JSN_FBDN; return *this; }
    Json &              clear_cache(void) { sc_.clear(); return *this; }
    const map_jn &      ns(void) const { return jns_; }         // namespace
    Json &              clear_ns(const std::string &s = "")     // clear namespace
                         { if(s.empty()) jns_.clear(); else jns_.erase(s); return *this; }

    // calling clear_cache is required once JSON was modified anyhow; it's called
    // anyway every time a new walk is build, thus the end-user must call it only
    // when continue walking iterators (with search iterators) past JSON modification

    //SERDES(root_)                                             // not really needed (so far)
    DEBUGGABLE()
    EXCEPTIONS(Jnode::ThrowReason)

    static Jnode::Jtype json_number_definition(std::string::const_iterator & jsp);
    static size_t utf8_adjusted(size_t start, const std::string &jsrc, size_t end = -1);
    static size_t byte_offset(const std::string &jsrc, size_t utf8_offset);


 protected:
    // protected data structures
    Jnode               root_;                                  // underlying JSON structure
    std::string::const_iterator
                        ep_;                                    // exception pointer
    const char *        jsn_fbdn_{JSN_FBDN};                    // JSN_FBDN pointer
    const char *        jsn_qtd_{JSN_QTD};                      // JSN_QTD pointer
    map_jn              jns_;                                   // jnode name space
    const Jnode *       ujn_ptr_;                               // for is_unique_()
    const Jnode *       djn_ptr_;                               // for is_duplicate_()


 private:
    struct WalkStep;                                            // fwd decl.

    auto                end_(void) { return root().children_().end(); } // frequently used shortcut
    void                parse_(Jnode & node, std::string::const_iterator &jsp);
    void                parse_bool_(Jnode & node, std::string::const_iterator &jsp);
    void                parse_string_(Jnode & node, std::string::const_iterator &jsp);
    void                parse_number_(Jnode & node, std::string::const_iterator &jsp);
    void                parse_array_(Jnode & node, std::string::const_iterator &jsp);
    void                parse_object_(Jnode & node, std::string::const_iterator &jsp);
    char                skip_blanks_(std::string::const_iterator & jsp);
    Jnode::Jtype        classify_jnode_(std::string::const_iterator & jsp);
    std::string::const_iterator &
                        find_delimiter_(char c, std::string::const_iterator & jsp);
    std::string::const_iterator &
                        validate_number_(std::string::const_iterator & jsp);

    typedef map_jn::iterator iter_jn;
    typedef map_jn::const_iterator const_iter_jn;
    typedef std::vector<std::string> vec_str;

    void                compile_walk_(const std::string & wstr, iterator & it) const;
    void                parse_lexemes_(const std::string & wstr, iterator & it) const;
    std::string         extract_lexeme_(std::string::const_iterator &si, char closing) const;
    void                parse_suffix_(std::string::const_iterator &, iterator &, vec_str&) const;
    void                parse_user_json_(WalkStep &ws) const;
    Jsearch             search_suffix_(char sfx) const;
    void                parse_quantifier_(std::string::const_iterator &si, iterator & it) const ;
    void                parse_range(std::string::const_iterator &, WalkStep &, ParseThrow) const;
    std::string         parse_namespace_(std::string::const_iterator &) const;
    long                parse_index_(std::string::const_iterator &,
                                     ParseThrow, ParseThrow = may_be_any) const;


    // Properties and structures used by walk iterator:

    // Itr (iterator-label pair):
    // - used by walk iterator (path_vector) and search cache key
    struct Itr {
        // path-vector is made of Itr - result of walking WalkStep vector (walk path)
        // last Itr in path-vector points to the found JSON element (via jit)
        // lbl and jnp keep a copy of jit's key and Jnode addr: this is required for
        // validation - jit could be invalidated due to a prior JSON manipulation,
        // preserved lbl and jnode addr ensure safe execution of is_nested() and
        // is_valid() methods;
        // wsi (filled only when path is terminated with end() - out of iterations/non
        // iterable) used in increment_() facilitating walk path iterations

                            Itr(void) = default;                // for pv_.resize()
                            Itr(const iter_jn &it):             // for emplacement of good itr
                             jit(it), lbl(it->KEY), jnp(&it->VALUE) {}
                            Itr(const iter_jn &it, bool x):     // for emplacement of endl() only!
                             jit(it) {}

        // typedef std::map<std::string, Jnode>::iterator iter_jn;
        iter_jn             jit;                                // iterator pointing to JSON
        std::string         lbl;                                // label and node ptr are required
        const Jnode *       jnp{nullptr};                       // for iterator (jit) validation
    };
    typedef std::vector<Itr> path_vector;                       // used by iterator, CahceEntry

    // CacheEntry:
    // used by SearchCache
    struct CacheEntry {
        // namespace at the given instance of the path, to support its REGEX values
        map_jn                      ns;                         // ns only for given cache entry
        path_vector                 pv;
    };

    // WalkStep:
    // - used by walk iterator and search cache key
    struct WalkStep {
        // WalkStep contains data describing one walk lexeme (subscript or search)
        // walk path is made of walk steps instructing how JSON tree to be traversed

        #define WALKSTEPTYPE \
            static_select,      /* [3], [0], quantifier: 0, 3, etc */ \
            parent_select,      /* [-2], quantifier: NA */ \
            root_select,        /* [^5], quantifier: NA */ \
            range_walk,         /* [+0], [1:], [4:10], [-5:-1], quantifiers: +0, 3:, 4:10 */ \
            cache_complete      /* this is used in **SearchCacheKey** only */
                                // note: there's no negative range for iterable quantifier: no way
                                // to know upfront the number of hits recursive search'd produce
        ENUMSTR(WalkStepType, WALKSTEPTYPE)

                            WalkStep(void) = delete;
                            WalkStep(std::string && l, Jsearch js): // enable emplacement
                             lexeme(std::move(l)), jsearch(js) {}

        bool                operator<(const WalkStep &r) const {// only for SearchCacheKey:
                             if(is_subscript() or r.is_subscript())
                              return jsearch < r.jsearch;
                             return jsearch != r.jsearch?       // - for caching purpose
                                    jsearch < r.jsearch:        // quantifiers are not compared
                                    ( lexeme != r.lexeme?
                                      lexeme < r.lexeme:
                                      stripped.back() < r.stripped.back()
                                    );
                            }

        bool                is_recursive(void) const
                             { return lexeme.front() == LXM_SCH_OPN; }
        bool                is_non_recursive(void) const
                             { return lexeme.front() == LXM_SCH_CLS; }
        bool                is_not_namespaced(void) const
                             { return not is_namespaced(); }
        bool                is_namespaced(void) const
                             { return jsearch AMONG(search_from_ns,
                                                    tag_from_ns, query_unique, Query_duplicate); }
        bool                is_qnt_namespaced(void) const
                             { return not heads.empty() or not tails.empty(); }
        bool                is_lbl_based(void) const            // operates on label types l/L/t
                             { return jsearch AMONG(label_match, Label_RE_search, tag_from_ns); }
        bool                is_val_based(void) const             // operates on value
                             { return not is_lbl_based(); }
        bool                is_tag_based(void) const            // only tags 't' and 'l'
                             { return jsearch AMONG(label_match, tag_from_ns); }
        bool                is_qnt_relative(void) const         // only in >..<t/l
                             { return is_non_recursive() and is_tag_based(); }
        bool                is_cacheless(void) const
                             { return is_namespaced() or is_qnt_relative(); }

        bool                is_subscript(void) const
                             { return lexeme.front() == LXM_SUB_OPN; }
        bool                is_search(void) const
                             { return not is_subscript(); }
        bool                is_directive(void) const {
                             return jsearch AMONG(key_of_value, value_of_json,
                                                    zip_namespace, fail_stop, Forward_itr);
                            }
        bool                is_search_lexeme(void) const
                             { return not is_directive(); }
        bool                is_lexeme_required(void) const
                             { return jsearch AMONG(Regex_search, digital_match, Digital_regex,
                                                    Label_RE_search, json_match,
                                                    search_from_ns, tag_from_ns, query_unique,
                                                    Query_duplicate, value_of_json); }
        bool                must_lexeme_be_empty(void) const
                             { return jsearch AMONG(null_match, atomic_match, object_match,
                                                    indexable_match, collection_match,
                                                    end_node_match, wide_match);}
        long                load_head(const Json &j) const
                             { return heads.empty()? head: fetch_from_ns(heads, j); }
        long                load_tail(const Json &j) const
                             { return tails.empty()? tail: fetch_from_ns(tails, j); }
        long                load_offset(const Json &j) const {
                             if(offsets.empty()) return offset; // NS is not used
                             return type == range_walk? offset: fetch_from_ns(offsets, j);
                            }
        long                fetch_from_ns(const std::string &val, const Json &j) const {
                             const auto found = j.ns().find(val);
                             if(found == j.ns().end())                  // namespace not found
                              throw j.EXP(Jnode::walk_non_existant_namespace);
                             if(found->VALUE.type() != Jnode::Number)   // value is not a number
                              throw j.EXP(Jnode::walk_non_numeric_namespace);
                             long x =  strtol(found->VALUE.val().c_str(), nullptr, 10 );
                             if(x < 0 and not is_tag_based())   // only t/l can go negative
                              throw j.EXP(Jnode::walk_negative_quantifier); // quant cannot be neg.
                             return x;
                            }

        std::string         lexeme;                             // lexeme w/o suffix and quantifier
        Jsearch             jsearch;                            // Jsearch type
        long                offset{0};                          // current offset
        long                head{0};                            // range walk type
        long                tail{LONG_MAX};                     // by default - till the end
        std::string         offsets;                            // interpolatable offset
        std::string         heads;                              // interpolatable head
        std::string         tails;                              // interpolatable tail
        WalkStepType        type{static_select};
        vec_str             stripped;
                            // stripped[0] -> a stripped lexeme (required)
                            // stripped[1] -> attached label match (optional)
        std::regex          re;                                 // RE for R/L/D suffixes
        Jnode               user_json{Jnode::Neither};          // Json for <>j, or <>v assignment
        path_vector         fs_path;
        // fs_path implementation is somewhat tricky:
        // fail-stops require maintaining disabled (locked) and enabled (unlocked) states:
        // when locking occurs - lock_failstops_ (going backwards) up until 1st range ws found
        // when unlocking occurs - unlock_failstops_ (going forward) up until 1st range ws found
        // 0. initialized in enabled / unlocked state
        // 1. if walking fails: lock_failstops_ staring from the failed ws
        // 2. unlocking occurs when any of ws (range type) is getting incremented
        // 3. failed stop is getting engaged only when walk fails, but not when the walk fails
        //    due to incremented range-type ws

        // Below definitions only for COUTABLE interface
        const char *        search_type() const
                             { return ENUMS(Jsearch, jsearch); }
        const std::string   label() const
                             { return stripped.size()==2? stripped.back(): "N/A"; }
        std::string         json() const
                             { return user_json.type() == Jnode::Neither?
                                       "N/A": user_json.to_string(Jnode::Raw); }
        const char *        ws_type(void) const
                             { return jsearch == text_offset? "N/A": ENUMS(WalkStepType, type); }
        std::string         range() const {
                             if(jsearch == text_offset) return "N/A";
                             if(type != range_walk) return "N/A";
                             std::stringstream ss;
                             ss << '[' << (heads.empty()?
                                           head == 0? "": std::to_string(head):
                                           "{" + heads + "}") << ':'
                                << (tails.empty()?
                                    tail == LONG_MAX? "": std::to_string(tail):
                                    "{" + tails + "}") << ']';
                             return ss.str();
                            }
        std::string         ofst() const {                      // only for COUTABLE
                             if(type == range_walk and offset != LONG_MIN)
                              return std::to_string(offset);
                             std::stringstream ss;
                             ss << (offsets.empty()? std::to_string(offset): "{" + offsets + "}");
                             return ss.str();
                            }
        COUTABLE(WalkStep, search_type(), lexeme, ws_type(), ofst(), range(), label(), json())
    };

    // Search Cache Key:
    // - made of jnode pointer and walk step
    struct SearchCacheKey {
        // Search may occur off any JSON node and in any of WalkSteps, hence a unique
        // key would be their combination
        // WalkStep (ws) needs to be preserved (cannot be referenced) as the original
        // walk path might not even exist, while cache could be still alive and actual

                            SearchCacheKey(void) = delete;
                            SearchCacheKey(const Jnode *jp, const WalkStep & w):
                             jnp(jp), ws(w) {}

        const Jnode *       json_node(void) const { return jnp; }   // only for COUTABLE

        static bool         cmp(const SearchCacheKey &l, const SearchCacheKey &r)
                             { return l.jnp != r.jnp? l.jnp<r.jnp: l.ws<r.ws; }

        const Jnode *       jnp;
        WalkStep            ws;

        COUTABLE(SearchCacheKey, json_node(), ws)
    };
    typedef std::map<SearchCacheKey, std::vector<CacheEntry>, decltype(&SearchCacheKey::cmp)>
                        SeachCache;


    // parse_subscript_type_() is dependent on WalkStep definition, hence moved down here
    void                parse_subscript_type_(WalkStep & state) const;

 public:

    //                      walk iterator
    //
    // Json::iterator (a.k.a. walk iterator): needs to be defined in-class to facilitate
    // container storage with the iterator (e.g. walk-based callbacks)
    //
    class iterator: public std::iterator<std::forward_iterator_tag, Jnode> {
     // this forward iterator let iterating over *iterable* walk paths
     // once iterator is dereferenced it returns a reference to a Jnode's super-node,
     // super-node adds following powers to regular Jnode:
     // - label() allows accessing node's label if node is nested by object
     // - index() allows accessing node's ordinal index if nested by array
     // - value() provides access to the Jnode itself (but this methods is quite
     //   redundant)
     // - is_root() let checking if the node is root() or not
     // - operator[] receives a new capability: on a negative index it will address
     //              indexed levels up in the JSON's tree hierarchy (e.g." [-1] will
     //              address a parent of the dereferenced node, and so on)
        friend Json;
        friend void         swap(Json::iterator &l, Json::iterator &r) {
                             using std::swap;                   // enable ADL
                             swap(l.ws_, r.ws_);
                             swap(l.jp_, r.jp_);
                             swap(l.pv_, r.pv_);
                             swap(l.sn_.parent_type(), r.sn_.parent_type());// supernode requires
                            }                                   // swapping of type_ values only

        // walk iterator's Super node class definition
        //
        class SuperJnode: public Jnode {
            friend Json::iterator;

         public:
            bool                has_label(void) const {
                                 return lbl_ != &Json::iterator::empty_lbl_ and
                                        parent_type() == Object;
                                }
            const std::string & label(void) const {
                                 if(type_ != Object)
                                  throw EXP(label_request_for_non_object_enclosed);
                                 return *lbl_;
                                }
            bool                has_index(void) const {
                                 return lbl_ != &Json::iterator::empty_lbl_ and
                                        parent_type() == Array;
                                }
            int64_t             index(void) const {
                                 if(type_ != Array)
                                  throw EXP(index_request_for_non_array_enclosed);
                                 return std::stoul(lbl_->c_str(), nullptr, 16);
                                }
            Jnode &             value(void) { return *jnp_; }
            const Jnode &       value(void) const { return *jnp_; }
            bool                is_root(void) const { return &jit_->jp_->root() == jnp_; }
            Jnode &             operator[](long i) {
                                 // in addition to Jnode::iterator's, this one adds capability
                                 // to address supernode with negative index, e.g: [-1],
                                 // like in the walk-string to reference a parent
                                 if(i >= 0) return Jnode::operator[](i);
                                 return -i >= static_cast<long>(jit_->pv_.size())?
                                        jit_->json().root():
                                        jit_->pv_[jit_->pv_.size() + i -1].jit->VALUE;
                                }
            const Jnode &       operator[](long i) const {
                                 if(i >= 0) return Jnode::operator[](i);
                                 return -i >= static_cast<long>(jit_->pv_.size())?
                                        jit_->json().root():
                                        jit_->pv_[jit_->pv_.size() + i -1].jit->VALUE;
                                }
            Jtype               parent_type(void) const { return type_; }
            Jtype &             parent_type(void) { return type_; }
                                // this is a work around GNU's compiler bug-limitation, which
                                // does not extend scoping onto subclasses, i.e.:
                                // swap(l.sn_.type_, r.sn_.type_) fails.

         private:
                                SuperJnode(Jnode::Jtype t = Jnode::Neither): Jnode{t} {}

            SuperJnode &        operator()(const std::string &l, Jnode &jn, Json::iterator * jit)
                                 { lbl_ = &l; jnp_ = &jn; jit_ = jit; return *this; }
            SuperJnode &        operator()(Jnode &jn, Json::iterator * jit) {
                                 lbl_ = &Json::iterator::empty_lbl_;
                                 jnp_ = &jn;
                                 jit_ = jit;
                                 return *this;
                                }

            const std::string * lbl_{&Json::iterator::empty_lbl_}; // lbl_ should never be nullptr
            Jnode *             jnp_{nullptr};                  // iterator's Jnode pointer
            Json::iterator *    jit_{nullptr};                  // back to iterator, for [-n]
        };
        //
        // end of Super node definition

     public:
                            iterator(void) = default;           // DC
                            iterator(const iterator &it):       // CC
                             ws_(it.ws_), pv_(it.pv_), jp_(it.jp_) {
                             sn_.type_ = it.sn_.type_;
                            }
                            iterator(iterator &&it) {           // MC
                             swap(*this, it);
                            }
        iterator &          operator=(iterator it) {            // CA, MA
                             swap(*this, it);
                             return *this;
                            }

                            // adapters to Jnode::iterator
                            operator Jnode::iterator (void) const {
                             auto it = pv_.empty()?
                                       jp_->root().children_().begin(): pv_.back().jit;
                             return Jnode::iterator{std::move(it), sn_.type_};
                            }
                            operator Jnode::const_iterator(void) const {
                             auto it = pv_.empty()?
                                       jp_->root().children_().begin(): pv_.back().jit;
                             return Jnode::const_iterator{std::move(it), sn_.type_};
                            }

        bool                operator==(const iterator & rhs) const {
                             if(pv_.empty()) return rhs.pv_.empty()? jp_ == rhs.jp_: false;
                             if(rhs.pv_.empty()) return false;
                             return pv_.back().jit == rhs.pv_.back().jit;
                            }
        bool                operator!=(const iterator & rhs) const
                             { return not operator==(rhs); }
        template<typename T>
        bool                operator==(const T & rhs) const {   // Jnode::iterator comparator
                             if(pv_.empty()) return false;
                             // i.e. Jonde::iterator can never point to a root, hence always false
                             return pv_.back().jit == rhs.underlying_();
                            }
        template<typename T>
        bool                operator!=(const T & rhs) const
                             { return not operator==(rhs); }
        Jnode &             operator*(void) {
                             if(not ws_.empty() and ws_.back().jsearch == key_of_value) {
                              // return value of label/index in the supernode's JSON
                              if(pv_.empty()) throw jp_->EXP(Jnode::walk_root_has_no_label);
                              switch (sn_.type_) {
                               case Jnode::Object:
                                     sn_.type_ = Jnode::String;
                                     sn_.value_ = pv_.back().jit->KEY;
                                     break;
                               case Jnode::Array:
                                     sn_.type_ = Jnode::Number;
                                     sn_.value_ = std::to_string(
                                                   std::stol(pv_.back().jit->KEY, nullptr, 16));
                                     break;
                               default:
                                     break;
                              }
                              return sn_(pv_.back().jit->KEY, static_cast<Jnode&>(sn_), this);
                             }
                             return pv_.empty()?
                              sn_(jp_->root(), this):
                              sn_(pv_.back().jit->KEY, pv_.back().jit->VALUE, this);
                            }
        Jnode *             operator->(void)
                             { return & operator*(); }
        iterator &          operator++(void)
                             { incremented(); return *this; }
        bool                incremented(void);
        iterator &          begin(void)
                             { return *this; }
        iterator            end(void) const {
                             if(not jp_->root().is_iterable())
                              throw jp_->EXP(Jnode::walk_root_non_iterable);
                             return jp_->end_();
                            }
        bool                is_valid(void) const {
                             // end iterator is considered invalid path
                             if(pv_.empty()) return true;
                             if(pv_.back().jit == jp_->end_()) return false;
                             return is_valid_(jp_->root(), 0);
                            }
        bool                is_nested(iterator & it) const;

        // work with path vector:
        const path_vector & path(void) const { return pv_; }    // path to the element
        const Itr &         operator[](size_t idx) const { return pv_.at(idx); }
        size_t              path_size(void) const { return pv_.size(); }

        // work with walk steps:
        const std::vector<WalkStep> &
                            walks(void) const { return ws_; }
        size_t              walk_size(void) const { return ws_.size(); }
        long                counter(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].type == WalkStep::range_walk?
                                    ws_[position].load_offset(json()): -1;
                            }
        long                instance(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].type == WalkStep::static_select?
                                    ws_[position].load_offset(json()): -1;
                            }
        long                offset(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].load_offset(json());
                            }
        const std::string & lexeme(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].lexeme;
                            }
        Jsearch             type(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].jsearch;
                            }
        bool                is_walkable(void) const
                             { return next_iterable_ws_(walk_path_().size()) >= 0; }
        Json &              json(void) const { return *jp_; }

     protected:
                            iterator(Json *ptr): jp_{ptr} {};
                            iterator(const iter_jn & it)        // used to return end() iterator
                             { pv_.emplace_back(it, true); }

      std::vector<WalkStep> ws_;                                // walk state vector (walk path)
        path_vector         pv_;                                // path_vector (result of walking)
        Json *              jp_;                                // json pointer (for json().end())
        SuperJnode          sn_{Jnode::Neither};                // super node type_ holds parent's

     private:
        #define SEARCH_TYPE \
                recursive, \
                non_recursive
        ENUMSTR(SearchType, SEARCH_TYPE)

        bool                is_valid_(Jnode & jnp, size_t idx) const;
    std::vector<WalkStep> & walk_path_(void) { return ws_; }
        const std::vector<WalkStep> &
                            walk_path_(void) const { return ws_; }
        //Json &              json_(void) const { return *jp_; }
        auto &              sn_type_ref_(void) { return sn_.type_; }    // original container type

        size_t              walk_(void);
        void                walk_step_(size_t wsi, Jnode *);
        void                show_built_pv_(std::ostream &out) const;
        void                walk_numeric_offset_(size_t wsi, Jnode *);
        Json::iter_jn       build_cache_(Jnode *jn, size_t wsi);
        void                walk_text_offset_(size_t wsi, Jnode *);
        void                walk_search_(size_t wsi, Jnode *);
        size_t              normalize_(long offset, Jnode *jn) const {  // norm. by [jn->chldrn]
                             long children = static_cast<long>(jn->children_().size());
                             if(offset >= 0) return offset > children? children: offset;
                             return children + offset < 0? 0: children + offset;
                            }
        long                re_normalize_(long offset, Jnode *jn) const {   // relative normalize
                             long children = static_cast<long>(jn->children_().size());
                             return offset > children? children:
                                    -offset > children? -children: offset;
                            }

        void                research_(Jnode *jn, size_t wsi,
                                      std::vector<Json::CacheEntry> *, SearchCacheKey *);
        bool                re_search_(Jnode *jn, WalkStep &, const char *lbl, long &instance,
                                       long cf, SearchType st, std::vector<Json::CacheEntry> *);
        bool                build_cache_(long &instance, long cf, std::vector<Json::CacheEntry> *);
        bool                match_iterable_(Jnode *jn, const char *lbl, const WalkStep &);
        bool                atomic_match_(const Jnode *jn, const char *lbl,
                                          const WalkStep &ws, map_jn * ns);
        bool                string_match_(const Jnode *jn, const WalkStep &, map_jn * ns) const;
        bool                regex_match_(const std::string &val, const WalkStep &, map_jn *) const;
        bool                label_match_(map_jn::iterator jit, const Jnode *jn, long idx,
                                         WalkStep & ws, map_jn * ns) const;
        bool                bull_match_(const Jnode *jn, const WalkStep &) const;
        bool                is_unique_(const Jnode & jn, const WalkStep &ws);
        bool                is_duplicate_(const Jnode & jn, const WalkStep &ws);
        void                purge_ns_(const std::string & pfx);

        void                lbl_callback_(const char *lbl,
                                          const std::vector<Json::CacheEntry> * = nullptr);
        void                wlk_callback_(const Jnode *);
        void                dmx_callback_(const Jnode *, const char *lbl,
                                          const std::vector<Json::CacheEntry> * = nullptr);

        bool                increment_(long wsi);
        long                next_iterable_ws_(long wsi) const;
        void                lock_failstops_(long wsi) {         // lock out FS until 1st range
                             for(; wsi >= 0; --wsi) {           // traversing ws backwards
                              auto & ws = walk_path_()[wsi];
                              if(ws.type == WalkStep::range_walk) break;
                              if(ws.jsearch == Jsearch::fail_stop)
                               ws.fs_path.emplace_back(json().end_(), 0);
                             }
                            }
        void                unlock_failstops_(size_t wsi) {     // unlock fail-stop till 1st range
                             for(++wsi; wsi < walk_path_().size(); ++wsi) { // traversing forward
                              auto & ws = walk_path_()[wsi];
                              if(ws.type == WalkStep::range_walk) break;
                              if(ws.jsearch == Jsearch::fail_stop)
                               ws.fs_path.clear();
                             }
                            }
        bool                failed_stop_(long wsi);

        static std::string  empty_lbl_;                         // empty (default) label
    };
    //
    // end of walk Iterator's definition


 private:
    // data structures (cache storage and callbacks)
     // cache:
     //      key: made of { WalkStep, *Jnode };
     //      value: vector<CacheEntry: { namespace (map_jn); vector<path_vector> }>;
     // - namespace is required to be a part of the cache to support REGEX values only!
    SeachCache          sc_{SearchCacheKey::cmp};           // search cache itself
                        // search cache is the array of all path_vector's for given
                        // search key (combination of jnode and walk step)


    struct WlkCallback {
     // this class facilitates wlk_callback_vec type storage, binding Json::iterator
     // and callback itself together
     // There are 2 types of callbacks:
     // 1. callback hooked onto the JSON labels (lbl_callback_map)
     // 2. callback hooked onto Json::iterators (wlk_callback_vec)
                            WlkCallback(iterator &&itr, std::function<void(const Jnode &)> &&cb):
                             itr{std::move(itr)}, callback(std::move(cb)) {}
        iterator            itr;
        std::function<void(const Jnode &)>
                            callback;
    };


    typedef std::map<std::string, std::function<void(const Jnode &)>> lbl_callback_map;
    typedef std::vector<WlkCallback> wlk_callback_vec;

    lbl_callback_map    lcb_;                                   // label callbacks storage
    wlk_callback_vec    wcb_;                                   // walk based callback storage
    bool                ce_{false};                             // callbacks engaged? flag

 public:
    #define CALLBACK_TYPE \
                label_callback, \
                walk_callback, \
                any_callback
    ENUM(CallbackType, CALLBACK_TYPE)

    // user callbacks interface:
    bool                is_engaged(CallbackType ct = any_callback) const {
                         if(ct == label_callback) return ce_ and not lcb_.empty();
                         if(ct == walk_callback) return ce_ and not wcb_.empty();
                         return ce_;
                        }
    Json &              engage_callbacks(bool x=true)           // engage/disengage all callbacks
                         { ce_ = x; return *this; };
    Json &              callback(const std::string &lbl,        // plug-in label-callback
                                 std::function<void(const Jnode &)> &&cb) {
                         lcb_.emplace(std::move(lbl), std::move(cb));
                         return *this;
                        }
    Json &              callback(iterator itr,                  // plug-in iter-callback (from walk)
                                 std::function<void(const Jnode &)> &&cb) {
                         if(itr != itr.end())                   // don't plug ended iterators
                          wcb_.emplace_back(std::move(itr), std::move(cb));
                         return *this;
                        }
    Json &              clear_callbacks(void)
                         { lcb_.clear(); wcb_.clear(); return *this; }
    lbl_callback_map &  lbl_callbacks(void) { return lcb_; }    // access to labeled callbacks
const lbl_callback_map& lbl_callbacks(void) const { return lcb_; }
    wlk_callback_vec &  wlk_callbacks(void) { return wcb_; }    // access to iterator callbacks
const wlk_callback_vec& wlk_callbacks(void) const { return wcb_; }

};

// class static definitions
STRINGIFY(Json::Jsearch, JS_ENUM)
STRINGIFY(Json::WalkStep::WalkStepType, WALKSTEPTYPE)
STRINGIFY(Json::iterator::SearchType, SEARCH_TYPE)

#undef JS_ENUM
#undef CACHE_STATE
#undef PARSETRAILING
#undef WALKSTEPTYPE
#undef SEARCH_TYPE
#undef CALLBACK_TYPE
#undef PARSE_THROW



// Jnode methods requiring Json definition:
Jnode::Jnode(Json j)                                            // type conversion Json -> Jnode
 { swap(*this, j.root()); }


// Json definitions:
Json operator ""_json(const char *c_str, std::size_t len) {
 // raw string parsing
 Json x;
 return x.parse(std::string{c_str});
}



Json & Json::parse(std::string::const_iterator & jsp, ParseTrailing trail) {
 // parse input string. this is a wrapper for parse_(), where actual parsing occurs
 root() = OBJ{};
 parse_(root_, jsp);

 if(root_.type() == Jnode::Neither)
  { ep_ = jsp; throw EXP(Jnode::expected_json_value); }

 if(trail == relaxed_no_trail) return *this;
 if(trail == strict_no_trail)
  if(*jsp != CHR_NULL)
   { ep_ = jsp; throw EXP(Jnode::unexpected_trailing); }

 for(; *jsp != CHR_NULL; ++jsp) {
  if(*jsp > 0 and *jsp <= ' ') continue;
  if(trail == strict_trailing)
   { ep_ = jsp; throw EXP(Jnode::unexpected_trailing); }
  break;
 }

 return *this;
}


void Json::parse_(Jnode & node, std::string::const_iterator &jsp) {
 // parse JSON from string (unicode UTF-8 compliant)
 skip_blanks_(jsp);
 node.type_ = classify_jnode_(jsp);

 DBG(4) {                                                       // print currently parsed point
   const char* pfx{"parsing point ->"};
   std::string str{&*jsp};
   if(utf8_adjusted(0, str) > (DBG_WIDTH-sizeof(pfx))) {
    str = str.erase(byte_offset(str, DBG_WIDTH - sizeof(pfx) - 3));
    str += "...";
   }
   for(auto &c: str) {
    if(c AMONG(CHR_EOL, CHR_RTRN)) c = '|';                     // replace \r \n with |
    if(c > 0 and c < ' ') c = ' ';
   }
   DOUT() << pfx << str << std::endl;
 }

 if(node.type_ == Jnode::Neither) return;
 DBG(5) DOUT() << "classified as: " << ENUMS(Jnode::Jtype, node.type()) << std::endl;

 switch(node.type()) {
  case Jnode::Object: parse_object_(node, ++jsp); break;        // skip '{' with ++jsp
  case Jnode::Array: parse_array_(node, ++jsp); break;          // skip '[' with ++jsp
  case Jnode::String: parse_string_(node, ++jsp); break;        // skip '"' with ++jsp
  case Jnode::Number: parse_number_(node, jsp); break;
  case Jnode::Bool: parse_bool_(node, jsp); break;
  case Jnode::Null: jsp += 4; break;                            // leave node blank, skip "null"
  default: break;                                               // covering warning of the compiler
 }
}


void Json::parse_bool_(Jnode & node, std::string::const_iterator &jsp) {
 // Parse first character of lexeme ([tT] or [fF])
 node.value_ = toupper(*jsp);                                   // i.e. store either 'T' or 'F'
 if(node.value_.front() == CHR_FALSE) ++jsp;
 jsp += 4;
}


void Json::parse_string_(Jnode & node, std::string::const_iterator &jsp) {
 // parse string value - from `"` till `"'
 auto sp = jsp;                                                 // copy, for a work-around
 auto ep = find_delimiter_(JSN_STRQ, jsp);
 node.value_ = std::string{sp, ep};                             // work around gnu's compiler bug
 //node.value_ = std::string{jsp, find_delimiter_('"', jsp)};   // this is broken in linux gnu c++
 ++jsp;
}


void Json::parse_number_(Jnode & node, std::string::const_iterator &jsp) {
 // parse number, as per JSON number definition
 auto sp = jsp;                                                 // copy, for a work-around
 auto ep = validate_number_(jsp);
 node.value_ = std::string{sp, ep};                             // work around gnu's compiler bug
 //node.value_ = std::string{jsp, validate_number_(jsp)};       // this is broken in linux gnu c++
}


void Json::parse_array_(Jnode & node, std::string::const_iterator &jsp) {
 // parse elements of JSON Array (recursively)
 for(bool comma_read = false; true;) {
  Jnode child;
  parse_(child, jsp);

  if(child.type() == Jnode::Neither) {
   if(*jsp == JSN_ARY_CLS) {
    if(node.empty()) { ++jsp; return; }                         // empty array: [ ]
    if(not comma_read) { ++jsp; return; }                       // end of array: ..., "last" ]
   }
   if(*jsp == JSN_ASPR)                                         // == ','
    if(not comma_read and node.has_children())
     { ++jsp; comma_read = true; continue; }                    // interleaving comma: .. 2, 3, ..
   // here: either a double comma: " ... ,,", or leading comma: "[ , ...
   ep_ = jsp; throw EXP(Jnode::expected_json_value);            // e.g.: "[ , ...", or "[ 123,, ]"
  }
  if(not comma_read and node.has_children())                    // e.g.: [ "abc" 3.14 ]
   { ep_ = jsp; throw EXP(Jnode::missed_prior_enumeration); }

  node.children_().emplace(node.next_key_(), std::move(child));
  comma_read = false;
 }
}


void Json::parse_object_(Jnode & node, std::string::const_iterator &jsp) {
 // parse elements of JSON Object (recursively)
 for(bool comma_read = false; true;) {
  skip_blanks_(jsp);
  auto lsp = jsp;                                               // label's begin pointer

  Jnode label;
  parse_(label, jsp);                                           // must be a string (label)
  if(not label.is_string()) {
   if(label.type() == Jnode::Neither) {                         // parsing of label failed
    if(*jsp == JSN_OBJ_CLS) {
     if(node.empty()) { ++jsp; return; }                        // empty object: { }
     if(not comma_read) { ++jsp; return; }                      // end of object: ..."last" }
    }
    if(*jsp == JSN_ASPR)                                        // == ','
     if(not comma_read and node.has_children())
      { ++jsp; comma_read = true; continue; }                   // interleaving comma
   }
   ep_ = lsp; throw EXP(Jnode::expected_valid_label);
  }

  if(skip_blanks_(jsp) != LBL_SPR)                              // label was read, expecting ':'
   { ep_ = jsp; throw EXP(Jnode::missing_label_separator); }

  Jnode child;
  parse_(child, ++jsp);
  if(child.type() == Jnode::Neither)                            // after 'label:' there must follow
   { ep_ = jsp; throw EXP(Jnode::expected_json_value); }        // a valid JSON value

  if(not comma_read and node.has_children())                    // e.g.: [ "abc" 3.14 ]
   { ep_ = jsp; throw EXP(Jnode::missed_prior_enumeration); }

  node.children_().emplace(std::move(label.str()), std::move(child));
  comma_read = false;
 }
}


std::string::const_iterator & Json::find_delimiter_(char c, std::string::const_iterator & jsp) {
 // find next occurrence of character (actually it's used only to find `"')
 while(*jsp != c) {
  if(*jsp AMONG(CHR_NULL, CHR_EOL, CHR_RTRN))                   // JSON string does not support
   { ep_ = jsp; throw EXP(Jnode::unexpected_end_of_line); }     // multiline, hence throwing
  if(strchr(jsn_fbdn_, *jsp) != nullptr)                        // i.e. found illegal JSON control
   { ep_ = jsp; throw EXP(Jnode::unquoted_character); }

  if(*jsp == CHR_QUOT) {
   ++jsp;                                                       // skip presumably quoted char
   if(*jsp == CHR_NULL)                                         // found end of string after '\'
    { ep_ = jsp; throw EXP(Jnode::unexpected_end_of_line); }
   if(strchr(jsn_qtd_, *jsp) == nullptr)                        // it's not JSON char quotation
    { ep_ = jsp; throw EXP(Jnode::unexpected_character_escape); }
  }
  ++jsp;
 }
 return jsp;
}


std::string::const_iterator & Json::validate_number_(std::string::const_iterator & jsp) {
 // wrapper for static json_number_definition()
 if(json_number_definition(jsp) != Jnode::Number)               // failed to convert
  { ep_ = jsp; throw EXP(Jnode::invalid_number); }
 return jsp;
}


Jnode::Jtype Json::json_number_definition(std::string::const_iterator & jsp) {
 // conform JSON's definition of a number
 if(*jsp == JSN_NUMM) ++jsp;                                    // == '-'
 if(not isdigit(*jsp)) return Jnode::Neither;                   // digit must follow '-' sign
 if(*jsp > '0')
  while(isdigit(*jsp)) ++jsp;
 else                                                           // next could be only [.eE] or end
  ++jsp;                                                        // skip leading 0
 // here it could be either of [.eE] or end
 if(*jsp == JSN_NUMD) {                                         // == '.'
  if(not isdigit(*++jsp)) return Jnode::Neither;                // digit must follow '.'
  while(isdigit(*jsp)) ++jsp;
 }
 // here could be [eE] or end
 if(*jsp AMONG('e', 'E')) {
  ++jsp;                                                        // skip [eE]
  if(*jsp AMONG(JSN_NUMP, JSN_NUMM)) ++jsp;                     // skip [+-]
  if(not isdigit(*jsp)) return Jnode::Neither;                  // digit must follow [eE][+/]
  while(isdigit(*jsp)) ++jsp;
 }
 return Jnode::Number;
}


Jnode::Jtype Json::classify_jnode_(std::string::const_iterator & jsp) {
 // classify returns either of the Jtypes, or Neither
 // it does not move the pointer
 if(*jsp == JSN_OBJ_OPN) return Jnode::Object;
 if(*jsp == JSN_ARY_OPN) return Jnode::Array;
 if(*jsp == JSN_STRQ) return Jnode::String;
 if(isdigit(*jsp) or (*jsp == JSN_NUMM and isdigit(*(jsp+1)))) return Jnode::Number;

 const char * str = &*jsp;
 if(std::strncmp(str, STR_TRUE, sizeof(STR_TRUE)-1) == 0) return Jnode::Bool;
 if(std::strncmp(str, STR_FALSE, sizeof(STR_FALSE)-1) == 0) return Jnode::Bool;
 if(std::strncmp(str, STR_NULL, sizeof(STR_NULL)-1) == 0) return Jnode::Null;
 return Jnode::Neither;
}


char Json::skip_blanks_(std::string::const_iterator & jsp) {
 // skip_blanks_() sets pointer to the first a non-blank character
 while(*jsp >= 0 and *jsp <= ' ') {                             // '*jsp >= 0' to support UTF-8
  if(*jsp == CHR_NULL)
   { ep_ = jsp; throw EXP(Jnode::unexpected_end_of_string); }
  ++jsp;
 }
 return *jsp;
}


// Walk path is a stateful feature. The path is a string, always refers from
// the root and made of lexemes
//
// Walk path design notes (brief):
// 1. offset lexemes:
//    [], [n], [text], [-n], [^n], [+n], [N:N] where n is an integer,
//      N an optional integer with preceding + or - sign, e.g. +3:-3
//    - empty offset [] matches empty label, e.g.: { "": "blah" }
//    - [text] offset matches a child in the node with the same label; to match
//      a text label that looks like a number, i.e { "3": ... } - use search lexeme
//      with 'l' suffix: "<3>l"
//    - numerical offset performs various operation deepening on the suffix:
//      in the simplest form: [n]: selects n'th (zero based) child in the node
//      [-n]: back off n levels up in Json hierarchy,
//      [^n]: in a way does reverse thing: descends n nodes from the root for
//            currently built path-vector
//      +n: indicates that the path is iterable (from n'th child in the node)
//      [N:N] is a python-like addressing
// 2. search lexemes:
//    <text>SN
//    - <text>: perform search of the entire node to match the text
//    - <..> perform recursive search, >..< perform non-recursive search among immediate
//      children only
//    - optional suffix S alters search behavior, see descriptions in the header
//    - N optional quantifier (must take the last position), could be in the form of:
//       n - search find' n'th (zero based) match instance
//      +n - indicates that the path is iterable (starting from n'th match)
//     n:n - python like addressing, except that n cannot go negative here
//
// Examples of walk path:
//      "[Address Book][+0][Phones][Mobile][+0]"
//      "[Address Book] <Mobile>l+0"
//      "<Elm Street>m+0 [-2] [Phones] [+0] [Mobile]"
//
// spaces between lexemes are allowed and ignored.
//
// 3. in both lexeme types, it's allowed to include inner brackets, but closing
//    one if it' a part of the data, needs to be quoted with preceding '\', e.g.:
//    "[case[0\]]" - specifies an offset lexeme to the label "case[0]",
//    <tag<a\>> - specifies search lexeme for the text "tag<a>"
//
// 4. a search lexeme could be appended to the offset lexeme like this: "[lbl]:<txt>"
//    in that case the search will be performed only among entries whose label
//    matches prepending lexeme; in that case suffixes 'l' and 'L' cannot be used.
//
// API design notes:
//    a. walk compiles a provided walk path, building an internal state.
//    b. walk the json per given path - returns the walked json node (it's always
//       an iterator) i.e. non-iterable (atomic) JSONs are not walked (but those
//       are very limited cases)
//    c. further walking is done through iterator (forward iterator only); when
//       all values walked (iterated), finally returns end() iterator (to json's
//       root.children().end())


std::string Json::iterator::empty_lbl_;


Json::iterator Json::walk(const std::string & wstr, CacheState action) {
 // return Json::iterator to the first element pointed by the walk string.
 // otherwise end-iterator returned
 DBG(0) DOUT() << "walk string: '" << wstr << "'" << std::endl;

 iterator it = this, &itr = it;                                 // itr - for return by reference
 compile_walk_(wstr, it);
 it.pv_.reserve(it.walk_path_().size());                        // iter's performance optimization

 DBG(0) {
  DOUT() << "dump of completed lexemes:" << std::endl;
  size_t i = 0;
  for(const auto &ws: it.walk_path_())
   DOUT() << DBG_PROMPT(0) << '[' << i++ << "]: " << ws << std::endl;
 }

 if(action == invalidate) {                                     // talking a conservative approach:
  clear_cache();                                                // user must specify "keep" himself
  DBG(0) DOUT() << "invalidated search cache" << std::endl;
 }

 it.walk_();
 if(it.pv_.empty() or it.pv_.back().jit != end_()) {
  DBG(0) DOUT() << "initial walk: successful match" << std::endl;
  return itr;                                                   // must resolve reference
 }
 // here walk_() returned end(), but it might not be a true end, an increment step is required
 // in order to confirm that it's either a true end; return then incremented reference
 DBG(0) DOUT() << "initial walk requires iteration" << std::endl;
 it.pv_.clear();                                                // here walk_() returned end(), so
 return ++it;                                                   // advance to next, or true end
}


std::string Json::unquote_str(std::string && src) const {
 // unquote JSON source string as per JSON quotation.
 // even though it looks static, it's best to keep it in-class, due to throwing mechanism
 std::stringstream ss;
 size_t start=0;
 for(size_t end = src.find('\\', start);
     end != std::string::npos;
     end = src.find('\\', start)) {
  char qc = src[++end];                                         // quoted character
  if(end >= src.size())                                         // i.e. line ending "...\"
   throw EXP(Jnode::unexpected_end_of_quotation);
  const char * ptr = strchr(jsn_qtd_, qc);                      // #define JSN_QTD "/\"\\bfnrtu"
  if(ptr == nullptr)                                            // i.e other (non-Json) char quoted
   throw EXP(Jnode::unexpected_quotation);
  src[end-1] = '\0';
  ss << src.data() + start << (qc == 'u'? "\\": "") << JSN_TRL[ptr - jsn_qtd_];
  start = ++end;
 }
 ss << src.c_str() + start;
 return ss.str();
}


std::string Json::inquote_str(std::string && src) const {
 // quote quotation marks and back slashes only, as src is expected to be a valid JSON string
 // because of unquote_str(), keeping this in-class defined, though it's entirely static
 std::stringstream ss;
 size_t start=0;
 for(size_t end{ src.find_first_of("\"\\", start) };
     end != std::string::npos;
     end = src.find_first_of("\"\\", start)) {
  char chr = src[end];
  src[end] = '\0';
  ss << src.data() + start << '\\' << chr;
  start = end + 1;
 }
 ss << src.c_str() + start;
 return ss.str();
}


void Json::compile_walk_(const std::string & wstr, iterator & it) const {
 // parse walk string and compile all parts for ws_;
 parse_lexemes_(wstr, it);
 for(auto & walk_step: it.walk_path_())
  parse_subscript_type_(walk_step);                             // fix textual offset
}


void Json::parse_lexemes_(const std::string & wstr, iterator & it) const {
 // parse full lexemes: offsets/search lexemes + possible suffixes in search lexemes
 auto & ws = it.walk_path_();
 vec_str req_label;                                             // would hold stripped [label]:

 for(auto si = wstr.begin(); si != wstr.end();) {               // si: input string iterator
   if(not ws.empty() and ws.back().jsearch == Forward_itr) break;
   DBG(1) {
   DOUT() << "walked string: " << wstr << std::endl;
   DOUT() << DBG_PROMPT(1) << "parsing here: "
          << std::string(utf8_adjusted(0, wstr, si-wstr.begin()), '-') << ">|" << std::endl;
  }
  switch(*si) {
   case ' ': ++si; continue;                                    // space in between lexemes allowed
   case LXM_SUB_OPN:                                            // opening subscript: '['
         if(not req_label.empty())                              // only search <..> allowed
          throw EXP(Jnode::walk_expect_search_label);           // after [label]:
         ws.emplace_back(extract_lexeme_(si, LXM_SUB_CLS), numeric_offset);
         break;
   case LXM_SCH_OPN:                                            // opening search fwd '<'
         ws.emplace_back(extract_lexeme_(si, LXM_SCH_CLS), regular_match);  // initially, all
         break;                                                 // search will have regular_match
   case LXM_SCH_CLS:                                            // opening immediate search '>'
         ws.emplace_back(extract_lexeme_(si, LXM_SCH_OPN), regular_match);
         break;
   default:                                                     // outside of lexeme there could be
         parse_suffix_(si, it, req_label);                      // only suffixes, quantifiers & ':'
         if(req_label.empty()) parse_quantifier_(si, it);       // req_label indicate ':' found
         continue;
  }
  // store a stripped offset lexeme here (after parsing one)
  ws.back().stripped.push_back(ws.back().lexeme.substr(1, ws.back().lexeme.size() - 2));
  if(not req_label.empty()) {                                   // [label]: was extracted
   ws.back().stripped.push_back( std::move(req_label.front()) );
   req_label.clear();
  }
 }
 if(not req_label.empty()) throw EXP(Jnode::walk_expect_lexeme);
}


std::string Json::extract_lexeme_(std::string::const_iterator &si, char closing) const {
 // offset lexemes: [], [n], [+n], [-n], [n:n] [any text, possibly with \]]
 // search lexemes w/o suffix: <>, <text<\>>
 auto begin = si;
 while(*si != closing) {                                        // advance till closing bracket
  if(*si == CHR_NULL)
   throw EXP(Jnode::walk_offset_missing_closure);
  if(*si == CHR_QUOT)
   if(*++si != closing) --si;
  ++si;
 }

 std::string lexeme{begin, ++si};                               // lexeme with encasement
 std::string quoted{"\\"};                                      // replace quoted closing bracket
 quoted += closing;
 for(size_t p = lexeme.find(quoted); p != std::string::npos; p = lexeme.find(quoted, p))
  lexeme.replace(p, 2, 1, closing);
 DBG(1) DOUT() << "parsed lexeme: " << lexeme << std::endl;
 return lexeme;
}


void Json::parse_suffix_(std::string::const_iterator &si,
                         iterator & it, vec_str &req_label) const {
 // parse suffix following a text offset, e.g.: ..>r, ..>d, ..>L and suffix ':' in [..]:<..>
 if(it.walk_path_().empty())                                    // expect some elements already in
  throw EXP(Jnode::walk_expect_lexeme);                         // walks may start with lexeme only

 auto & back_ws = it.walk_path_().back();
 if(*si == LBL_SPR) {                                           // separator (':') eg: in [..]:<..>
  if(not req_label.empty())                                     // label already extracted!
   throw EXP(Jnode::walk_label_seprator_bad_usage);
  if(back_ws.jsearch == regular_match) return;                  // it's a quantifier then (<>:...)
  req_label.push_back( std::move(it.walk_path_().back().stripped.back()) );
  it.walk_path_().pop_back();                                   // label belongs to the next lexeme
  ++si;
  return;
 }

 if(back_ws.lexeme.front() == LXM_SUB_OPN  )                    // i.e. it's '['
  throw EXP(Jnode::walk_unexpexted_suffix);                     // suffix may follow search only

 Jsearch sfx = search_suffix_(*si);                             // see if a search suffix valid
 if(sfx < end_of_lexemes) {
  back_ws.jsearch = sfx;                                        // set suffix for given walkstep
  if(sfx AMONG(json_match, value_of_json, fail_stop, Forward_itr))  // process user_json
   parse_user_json_(back_ws);
  else
   if(sfx AMONG(Regex_search, Label_RE_search, Digital_regex))
    back_ws.re = back_ws.stripped.front();                      // it's a RE, book RE then
  DBG(1) DOUT() << "search type sfx: " << ENUMS(Jsearch, sfx) << std::endl;

  if(back_ws.stripped.front().empty()) {                        // lexeme is empty, e.g.: <>r
   if(back_ws.is_lexeme_required())
    throw EXP(Jnode::walk_empty_lexeme);
  }
  else                                                          // lexeme is non-empty (<..>)
   if(back_ws.must_lexeme_be_empty())
    throw EXP(Jnode::walk_non_empty_lexeme);

  ++si;
 }                                                              // else jsearch = regular_match
}


void Json::parse_user_json_(WalkStep &ws) const {
 // process options that require parsing json into ws.user_json_
 const char *json_ptr{nullptr};
 size_t json_start;
 switch (ws.jsearch) {
  case json_match:
        json_ptr = ws.stripped.front().c_str();
        break;
  case fail_stop:
  case Forward_itr:
  case value_of_json:
        json_start = ws.stripped.front().find(":");
        if(json_start == std::string::npos) return;
        json_ptr = &ws.stripped.front().c_str()[json_start + 1];
        ws.stripped[0].erase(json_start);
        break;
  default:
        throw EXP(Jnode::walk_a_bug);                           // covering compiler's warning
 }
 try { ws.user_json = Json{ json_ptr }; }
 catch(Json::stdException & e) { throw EXP(Jnode::json_lexeme_invalid); }
}


Json::Jsearch Json::search_suffix_(char sfx) const {
 // Jsearch is defined so that its first letter corresponds to the suffix
 for(int i = regular_match; i < end_of_lexemes; ++i)
  if(sfx == ENUMS(Jsearch, i)[0])
   return static_cast<Jsearch>(i);
 return end_of_lexemes;                                         // when no match found
}


void Json::parse_quantifier_(std::string::const_iterator &si, iterator & it) const {
 // search quantifier could be: [+]0, :, [+]1:, , :[+]2, [+]3:[+]4 and {..}
 auto & back_ws = it.walk_path_().back();
 parse_range(si, back_ws, may_throw);
 if(back_ws.is_directive())                                     // directives cannot have
  back_ws.type = WalkStep::static_select;                       // range type, overriding
 DBG(2) DOUT() << "offset / range: " << back_ws.ofst() << " / " << back_ws.range() << std::endl;

 while(*si == ' ') ++si;
 if(not(*si AMONG(LXM_SUB_OPN, LXM_SCH_OPN, LXM_SCH_CLS, CHR_NULL)))
  throw EXP(Jnode::walk_expect_lexeme);
}


void Json::parse_range(std::string::const_iterator &si, WalkStep & ws, ParseThrow throwing) const {
 // parse quantifier or subscript, update WalkStep's: offset, head, tail, type
 auto end_of_qnt = [](std::string::const_iterator &si)          // legit end of quant.
  { return *si AMONG(' ', CHR_NULL, LXM_SUB_OPN, LXM_SCH_OPN, LXM_SCH_CLS); };
 if(end_of_qnt(si)) return;
 bool quantifier = throwing == may_throw;                       // allowed when parsing quantifiers
 bool subscript = not quantifier;
 auto sic = si;                                                 // si copy

 switch(*si) {                                                  // process by first char in qnt.
  case PFX_WFR:                                                 // '^'
        if(quantifier)                                          // i.e. >^
         throw EXP(Jnode::walk_bad_number_or_suffix);           // >^ not allowed in quantifiers
        ws.type = WalkStep::root_select;                        // [^ -> a subscript (text or num)
        if(*(si + 1) == CHR_NULL) return;                       // i.e. [^] - indicate text subscr.
        ws.offset = parse_index_(++si, dont_throw, must_be_signless);
        break;
  case PFX_ITR:                                                 // '+'
        ws.type = WalkStep::range_walk;                         // assume for both [+ and >+
        ws.head = ws.offset = parse_index_(si, throwing, must_be_positive);
        if(sic == si) return;                                   // indicate text subscript: [+-...
        break;                                                  // process further possible range
  case PFX_WFL:                                                 // '-'
        if(subscript)
         ws.type = WalkStep::parent_select;                     // [- -> parent select
        ws.head = ws.offset = parse_index_(si, throwing, must_be_negative);
        if(sic == si) return;                                   // indicate text subscript: [-+..
        break;
  case QNT_OPN:                                                 // '{'
        if(subscript) return;                                   // [{ - text subscript
        ws.heads = ws.offsets = parse_namespace_(si);           // preserve namespace in ws
        ws.offset = LONG_MIN;                                   // indicate NS resolution required
 case RNG_SPR:                                                  // ':'
        if(ws.is_qnt_relative() and ws.heads.empty())           // i.e. if >..<l|t, but not {idx}
         ws.head = ws.offset = LONG_MIN + 1;                    // indicate entire range select [:]
        break;
  default:                                                      // must be numeric
        ws.head = ws.offset = parse_index_(si, throwing);
        if(sic == si) return;
 }
 // all quantifiers have been classified
 if(*si == RNG_SPR) {                                           // only ':' can pass here
  ws.type = WalkStep::range_walk;
  if(*++si == QNT_OPN)                                          // ":{
   { if(quantifier) ws.tails = parse_namespace_(si); }          // preserve namespace in ws
  else                                                          // ":... - must be a number
   if(not end_of_qnt(si)) ws.tail = parse_index_(si, throwing);
 }

 if(ws.type == WalkStep::root_select or (quantifier and not ws.is_qnt_relative()))
  if(ws.head < 0 or ws.tail < 0)                                // then cannot be negative
    throw EXP(Jnode::walk_negative_quantifier);
}


std::string Json::parse_namespace_(std::string::const_iterator &si) const {
 // parse interpolatable / named quantifier: {..}
 auto qs = ++si;
 for(; *si != QNT_CLS; ++si)
  if(*si == CHR_NULL)
   throw EXP(Jnode::walk_bad_quantifier);
 std::string qn{qs, si};
 if(qn.empty())
  throw EXP(Jnode::walk_empty_namesapce);
 ++si;
 return qn;
}


long Json::parse_index_(std::string::const_iterator &si,
                        ParseThrow throwing, ParseThrow sign_prescribtion) const {
 // validate if string iterator points to a valid integer.
 // parse_index is called from parse_range, which in turn could be called either from
 // parse_quantifier (<>...), or parse_subscript_type_ ([..]) those 2 parsings assume
 // different exception behavior:
 //  o parse_quantifier will throw upon failure
 //  o parse_subscript_type_ does not throw, but indicates successful completion by
 //    updating string iterator 'si' past parsed value
 // parse_index_ does not update 'si' in case of failure to indicate the failure
 char *endptr;
 const char * str{&*si};

 if(sign_prescribtion == must_be_signless and *si AMONG(PFX_ITR, PFX_WFL)) { // validate signless
  if(throwing == may_throw) throw EXP(Jnode::walk_bad_number_or_suffix);
  else return 0;
 }
 auto sic = si;                                                 // si copy for validating number
 if(*sic == PFX_ITR) ++sic;                                     // WA tolerating +N JSON definition
 if(json_number_definition(sic) != Jnode::Number) {             // validate number
  if(throwing == may_throw) throw EXP(Jnode::walk_bad_number_or_suffix);
  else return 0;
 }

 long idx = strtol(str, &endptr, 10);

 if(sign_prescribtion == must_be_positive and                   // validate positive
    (idx < 0 or str[0] == PFX_WFL)) {                           // and exclude case '-0'
  if(throwing == may_throw) throw EXP(Jnode::walk_bad_number_or_suffix);
  else return 0;
 }
 if(sign_prescribtion == must_be_negative and idx > 0) {        // validate negative
  if(throwing == may_throw) throw EXP(Jnode::walk_bad_number_or_suffix);
  else return 0;
 }
 si += endptr - str;
 return idx;
}


void Json::parse_subscript_type_(WalkStep & ws) const {
 // separates numerical subscripts (e.g. [1]) from textual's (e.g. [ 1])
 // numerical offsets are: [2], [-1], [+3], [3:], [3:1], [-5:-1], [+3:5], [:], [:-1]
 // parse_subscript_type_ is run after all lexemes are parsed
 DBG(1) DOUT() << "incomplete: " << ws << std::endl;

 if(ws.is_search()) {                                           // it's a search lexeme's walkstep
  if(ws.stripped.size() == 2)                                   // there's an attached label scope
   if(ws.is_lbl_based())                                        // label (l/L/t) search type cannot
    throw EXP(Jnode::walk_search_label_with_attached_label);    // have attached label offset
  return;                                                       // don't process search lexeme
 }

 auto si = ws.stripped[0].cbegin();
 parse_range(si, ws, dont_throw);
 if(ws.stripped[0].empty() or *si != CHR_NULL) {
  ws.jsearch = text_offset;                                     // parsing failed - it's a text
  ws.type = WalkStep::static_select;                            // text can only be static_select
 }
 DBG(2) DOUT() << "offset / range: " << ws.ofst() << " / " << ws.range() << std::endl;
}





//
//
// Json::iterator methods
//
//


bool Json::iterator::is_nested(iterator & it) const {
 // check if we're nesting iterator. end() iterator considered to be non-nested
 if(pv_.empty()) return jp_->is_iterable();                     // root always nests (if iterable)
 if(pv_.back().jit == jp_->end_())                              // end() does not nest
  return false;
 for(size_t i = 0; i<pv_.size() and i<it.pv_.size(); ++i)
  if(pv_[i].lbl != it.pv_[i].lbl)
   return false;
 return true;
}


bool Json::iterator::is_valid_(Jnode & jn, size_t idx) const {
 // check if all labels in path-vector are present
 if(idx >= pv_.size())                                          // no more pv_ idx to check
  return true;                                                  // all checked, return true then
 auto it = jn.children_().find(pv_[idx].lbl);                   // first try by label, if found
 if(it != jn.children_().end() and &it->VALUE == pv_[idx].jnp)  // then validate by Jnode addr
  return is_valid_(it->VALUE, idx+1);                           // check the rest of the tree
 return false;
}


size_t Json::iterator::walk_(void) {
 // walk 'ws' structure from the root building a path vector
 // empty pv_ addresses json.root().
 json().ujn_ptr_  = json().djn_ptr_ = nullptr;                  // indicate beginning of walk
 Jnode * jnp = & json().root();
 pv_.clear();                                                   // path-vector being built

 size_t i;
 for(i = 0; i < ws_.size(); ++i) {
  walk_step_(i, jnp);                                           // walkStep builds up a path-vector
  if(pv_.empty())
   jnp = & json().root();
  else {                                                        // pv_ is non-empty
   if(pv_.back().jit == json().end_()) {                        // and walk_step failed
    DBG(json(), 2) show_built_pv_(DOUT(json()));
    if(failed_stop_(i)) { lock_failstops_(i); break; }          // possibly engage <>f lexeme
    return i;
   }
   jnp = &pv_.back().jit->VALUE;
  }
  DBG(json(), 2) show_built_pv_(DOUT(json()));
 }
                                                                // successfully walked all ws
 sn_type_ref_() = pv_.size()>1? pv_[pv_.size()-2].jit->VALUE.type(): json().type();
 DBG(json(), 2) DOUT(json()) << "finished walking" << std::endl;
 return -1;
}


void Json::iterator::show_built_pv_(std::ostream &out) const {
 out << "built path vector:";
 for(auto &it: pv_)
  out << (&it == &pv_.front()? " ":"-> ")
      << (it.jit == json().end_()? "(end)": it.lbl);
 out << std::endl;
}


void Json::iterator::walk_step_(size_t wsi, Jnode *jn) {        // wsi: walk-step idx
 // walk a single lexeme
 auto & ws = ws_[wsi];
 DBG(json(), 3) DOUT(json()) << "walking step: [" << wsi << "]" << std::endl;
 switch(ws.jsearch) {
  case numeric_offset:                                          // [123]
        return walk_numeric_offset_(wsi, jn);
  case text_offset:                                             // [abc]
        return walk_text_offset_(wsi, jn);
  case zip_namespace:                                           // facilitate <..>z
        DBG(json(), 3) DOUT(json()) << "erased namespace: "
                                    << (ws.stripped[0].empty()? "all":
                                         "'" + ws.stripped[0] + "'") << std::endl;
        json().clear_ns(ws.stripped[0]);
        return;
  case fail_stop:                                               // facilitate <..>f
        if(not ws.fs_path.empty() and ws.fs_path.back().jit == json().end_()) {
         DBG(json(), 3) DOUT(json()) << "fail-stop [" << wsi << "] locked out" << std::endl;
         return;
        }
        ws.fs_path = pv_;
        DBG(json(), 3) DOUT(json()) << "recorded fail-stop: [" << wsi << "]" << std::endl;
        json().jns_[ws.stripped[0]] = ws.user_json.type() == Jnode::Neither?
                                       *jn: ws.user_json;
        if(ws.stripped[0].empty()) break;                       // otherwise record custom JSON
  case Forward_itr:                                             // facilitate <..>F
        if(ws.jsearch == Forward_itr)                           // could be fail_stop, hence chkn'
         pv_.emplace_back(json().end_(), true);
  case value_of_json:                                           // facilitate <..>v
        json().jns_[ws.stripped[0]] = ws.user_json.type() == Jnode::Neither?
                                       *jn: ws.user_json;
        DBG(json(), 3) DOUT(json()) << "saved jnode into namespace: '"
                                    << ws.stripped[0] << "': "
                                    << jn->to_string(Jnode::Raw) << std::endl;
        return;
  case key_of_value:
        if(not ws.stripped[0].empty()) {                        // facilitate <..>k
         if(pv_.empty()) throw jp_->EXP(Jnode::walk_root_has_no_label);
         auto & parent = pv_.size() == 1? json().root(): pv_[pv_.size()-2].jit->VALUE;
         json().jns_[ws.stripped[0]] = parent.type_ == Jnode::Object?
                                       Jnode{ pv_.back().jit->KEY }:
                                       Jnode{ static_cast<double>(
                                              std::stol(pv_.back().jit->KEY, nullptr, 16)) };
         DBG(json(), 3) DOUT(json()) << "saved label into namespace: '"
                                     << ws.stripped[0] << "': "
                                     << json().jns_[ws.stripped[0]] << std::endl;
        }
        return;
  default:
        return walk_search_(wsi, jn);                           // facilitate all others: <..>/>..<
 }
}


void Json::iterator::walk_numeric_offset_(size_t wsi, Jnode *jn) {
 // walk a numerical offset, e.g.: [3], [+0], [-1]
 auto &ws = ws_[wsi];

 if(ws.type == WalkStep::root_select)                           // root offset, e.g.: [^2]
  return pv_.resize(ws.offset > static_cast<long>(pv_.size())?
         pv_.size(): ws.offset);                                // shrinking path

 if(ws.type == WalkStep::parent_select)                         // negative offset, e.g.: [-2]
  return pv_.resize(-ws.offset <= static_cast<long>(pv_.size())? pv_.size() + ws.offset: 0);

 // [0], [+1], [..:..] etc
 size_t node_size = jn->children_().size();
 size_t offset = normalize_(ws.offset, jn);
 if(ws.type == WalkStep::range_walk) ws.offset = offset;        // ws iterable, require normalizing
 if(offset >= node_size or offset >= normalize_(ws.tail, jn))   // beyond children's size/tail
  return pv_.emplace_back(json().end_(), true);

 if(ws.type == WalkStep::static_select and jn->is_array())      // [N] - subscript array
  return pv_.emplace_back(jn->iterator_by_idx_(offset));

 auto it = build_cache_(jn, wsi);

 DBG(json(), 1) DOUT(json()) << "found cached idx " << offset << std::endl;
 pv_.emplace_back(it);
}


Json::iter_jn Json::iterator::build_cache_(Jnode *jn, size_t wsi) {
 // build cache for subscripts
 auto &ws = ws_[wsi];
 auto & cache_map = json().sc_;                                 // all caches map
 SearchCacheKey skey{jn, ws};                                   // prepare a search key
 auto found_cache = cache_map.find(skey);
 bool build_cache = found_cache == cache_map.end() or           // cache does not exist, or
                    (found_cache->KEY.ws.type != WalkStep::cache_complete and   // not cached yet
                     static_cast<size_t>(ws.offset) >= found_cache->VALUE.front().pv.size());
 if(build_cache) {
  DBG(json(), 1) DOUT(json()) << "building cache for [" << wsi << "] " << skey << std::endl;
  auto & cache = found_cache == cache_map.end()? cache_map[skey]: found_cache->VALUE;
  if(cache.empty())
   { cache.resize(1); cache.front().pv.emplace_back(jn->children_().begin()); }

  size_t size = ws.type == WalkStep::static_select? ws.offset: normalize_(ws.tail, jn) - 1;
  for(size_t i = cache.front().pv.size(); i <= size; ++i) {
   auto it = cache.front().pv.back().jit;
   cache.front().pv.emplace_back(++it);
  }
  if(cache.front().pv.size() == jn->children_().size()) {
   // indication that cache is complete is stored in the SearchCacheKey:
   skey.ws.type = WalkStep::cache_complete;                     // indicate cache completed
   auto pc = move(cache);                                       // move/preserve the cache
   json().sc_.erase(skey);                                      // erase old skey
   json().sc_[skey] = move(pc);                                 // create new one
  }

  found_cache = cache_map.find(skey);
  DBG(json(), 1)
   DOUT(json()) << "built cache size: " << found_cache->VALUE.front().pv.size()
                << ", cache status: " << ENUMS(WalkStep::WalkStepType, found_cache->KEY.ws.type)
                << std::endl;
 }
 return found_cache->VALUE.front().pv[ws.offset].jit;
}


void Json::iterator::walk_text_offset_(size_t wsi, Jnode *jn) {
 // walk a text offset, e.g.: [label]
 auto &ws = ws_[wsi];
 auto it = jn->children_().find(ws.stripped.front());           // see if label exist
 if(it == jn->children_().end())
  pv_.emplace_back(json().end_(), true);
 else
  pv_.emplace_back(it);                                         // if so, add to the path-vector
}


/* walk_search_ for given WalkStep[wsi] finds (and possibly caches) found JSON path:
 * - all cacheable paths are cached like this:
 *   o if search_key is non-existent, then build cache up until static_select'ed entry,
 *     or up to WalkStep's tail
 *   o if search_key exists, check if instance ('i') < cache's size:
 *     - if true: (entry instance was already cached) return cached path
 *     - if false (entry instance outside of cache), then check if cache has been completed?
 *       - if type == cache_complete - return global end (there's no such instance for sure)
 *       - otherwise re-search up till the offset/tail index (for static/range respectively)
 * - Caching eligibility:
 *   o non-eligible:
 *     - dynamic tags: namely: s,t,q,Q (cannot be cached in principle)
 *     - non-instance searches - entries with relative quantifiers: >..<t, >..<l
 *   o the rest of searches are eligible for caching
 */
void Json::iterator::walk_search_(size_t wsi, Jnode *jn) {
 // if search is const type, build a cache (if not yet), otherwise do a cache-less search
 auto &ws = ws_[wsi];
 if(ws.offset == LONG_MIN)                                      // offset is in NS, needs reloading
  ws.offset = ws.fetch_from_ns(ws.offsets, json());             // LONG_MIN triggers init from NS

 // check range:
 size_t offset = ws.load_offset(json());
 size_t tail = ws.load_tail(json());
 if(ws.is_qnt_relative()) {                                     // offset & tail must be signed
  if(re_normalize_(ws.load_offset(json()), jn) >= re_normalize_(ws.load_tail(json()), jn))
   return pv_.emplace_back(json().end_(), true);
 }
 else
  if(offset >= tail)                                            // counted beyond tail
   return pv_.emplace_back(json().end_(), true);                // return |nothing found|

 // engage cache-less search:
 if(ws.is_cacheless())
  return research_(jn, wsi, nullptr, nullptr);                  // vpv=null: cache-less search

 // engage search with caching:
 auto & cache_map = json().sc_;                                 // all caches map
 SearchCacheKey skey{jn, ws};                                   // prepare a search key
 auto found_cache = cache_map.find(skey);
 bool build_cache = found_cache == cache_map.end() or           // cache does not exist, or
                    (found_cache->KEY.ws.type != WalkStep::cache_complete and   // not cached yet
                     offset >= found_cache->VALUE.size());
 if(build_cache) {
  DBG(json(), 1) DOUT(json()) << "building cache for [" << wsi << "] " << skey << std::endl;
  auto & cache = found_cache == cache_map.end()? cache_map[skey]: found_cache->VALUE;
  research_(jn, wsi, &cache, &skey);                            // build cache
  found_cache = cache_map.find(skey);
  DBG(json(), 1)
   DOUT(json()) << "built cache size: " << found_cache->VALUE.size() << ", cache status: "
                << ENUMS(WalkStep::WalkStepType, found_cache->KEY.ws.type) << std::endl;
 }

 // find cached entry (if exist in the cache):
 if(offset >= found_cache->VALUE.size()) {                      // offset outside of cache:
  DBG(json(), 1) DOUT(json()) << "no cached instance found"<< std::endl;
  return pv_.emplace_back(json().end_(), true);                 // return global end()
 }
 DBG(json(), 1) DOUT(json()) << "found cached idx " << offset << std::endl;
 for(const auto &path: found_cache->VALUE[offset].pv)           // otherwise augment the path
  pv_.push_back(path);
 for(const auto &kv: found_cache->VALUE[offset].ns)             // and so do the namespaces
  json().jns_[kv.KEY] = kv.VALUE;
}


void Json::iterator::research_(Jnode *jn, size_t wsi,
                               std::vector<Json::CacheEntry> * vpv, SearchCacheKey *skey) {
 // wrapper for re_search_
 auto & ws = ws_[wsi];

 long i = ws.is_cacheless()?                                    // i: instance
          ws.load_offset(json()):                               // cache up to offset
          ws.load_tail(json()) -1;                              // cache up to tail
 long cache_from = vpv? i - vpv->size(): i;

 if(vpv) vpv->resize(vpv->size() + 1);                          // prepare place for 1st pathvector
 DBG(json(), 3)
  DOUT(json()) << "instance: " << i << ", cache from: " << cache_from
               << ", descend: " << ENUMS(SearchType, ws.is_recursive()? recursive: non_recursive)
               << ", caching? " << (vpv? "yes": "no") << std::endl;

 bool found = re_search_(jn, ws, nullptr, i, cache_from, non_recursive, vpv);
 if(vpv) vpv->pop_back();                                       // last entry is redundant

 if(not found) {                                                // entire Jnode was searched
  if(vpv == nullptr)                                            // indicate |nothing found|
   return pv_.emplace_back(json().end_(), true);                // for cacheless search type
  // indication that cache is complete (entire Jnode was searched) is stored in the SearchCacheKey:
  skey->ws.type = WalkStep::cache_complete;                     // indicate cache completed
  auto nvp = move(*vpv);                                        // move/preserve the cache
  json().sc_.erase(*skey);                                      // erase old skey
  json().sc_[*skey] = move(nvp);                                // create new one
 }
}


bool Json::iterator::re_search_(Jnode *jn, WalkStep &ws, const char *lbl, long &i,
                                long cf, SearchType pass, std::vector<Json::CacheEntry> * vpv) {
 // build cache, return true/false if match found (i'th instance), otherwise false
 // returning false globally indicates entire json has been searched up
 // 1. implementing recursive, const/static type (building cache, vpv != nullptr)
 // 2. implementing recursive, dynamic type (vpv == nullptr)
 // 3. implementing non-recursive, const type (building cache, vpv != nullptr)
 dmx_callback_(jn, lbl, vpv);

 if(pass == recursive or ws.is_recursive()) {                   // >..<: allow in recursive pass
  if(jn->is_atomic())
   return atomic_match_(jn, lbl, ws, &(vpv? vpv->back().ns: json().jns_)) and
          build_cache_(i, cf, vpv);
                                                                // here jn is iterable
  if(match_iterable_(jn, lbl, ws) and build_cache_(i, cf, vpv))
   return true;
 }

 if(pass == recursive and ws.is_non_recursive())                // >..<: further recursion denied
  return false;

 long idx = 0;
 for(auto it = jn->children_().begin(); it != jn->children_().end(); ++it) {
  if(vpv) vpv->back().pv.emplace_back(it); else pv_.emplace_back(it);

  long j = 0;                                                   // to be used in lieu of i
  if(ws.is_lbl_based())
   if(label_match_(it, jn, idx, ws, &json().jns_) and
      build_cache_(ws.is_qnt_relative()? j: i, ws.is_qnt_relative()? j: cf, vpv))
    return true;

  if(re_search_(&it->VALUE, ws, jn->is_object()? it->KEY.c_str(): nullptr, i, cf, recursive, vpv))
   return true;

  if(vpv) vpv->back().pv.pop_back(); else pv_.pop_back();
  ++idx;
 }

 return false;
}


bool Json::iterator::build_cache_(long &i, long cf, std::vector<Json::CacheEntry> * vpv) {
 // build up cache if present (if vpv - cache given)
 // but only upon i <= 0 return true, otherwise return false (not requested instance yet)
 if(--i >= cf) return false;                                    // cache entry already exists

 if(vpv != nullptr) {                                           // build cache then
  vpv->push_back(vpv->back());                                  // cache-in (built cache actually)
  vpv->back().ns.clear();                                       // this is required aft REGEX match
 }
 return i < 0;
}


bool Json::iterator::match_iterable_(Jnode *jn, const char *lbl, const WalkStep &ws) {
 // match any iterable suffixes and attached label (if any)
 // assert(jn->is_iterable())
 if(ws.stripped.size() > 1)                                     // there's an attached search label
  if(lbl == nullptr or ws.stripped.back() != lbl )              // there's no label
   return false;                                                // label does not match

 if(ws.jsearch == search_from_ns) {                             // facilitating <..>s
  const auto found = json().jns_.find(ws.stripped[0]);          // see if value was preserved
  if(found == json().jns_.end())
   throw json().EXP(Jnode::walk_non_existant_namespace);
  return *jn == found->VALUE;
 }
 if(ws.jsearch == json_match)                                   // facilitating <..>j
  return *jn == ws.user_json;

 if(ws.jsearch == query_unique)                                 // <..>q|Q
  return is_unique_(*jn, ws);
 if(ws.jsearch == Query_duplicate)
  return is_duplicate_(*jn, ws);

 if((jn->is_object() and ws.jsearch == object_match) or         // <..>o|i|c|w|e
    (jn->is_array() and ws.jsearch == indexable_match) or
    (ws.jsearch AMONG(wide_match, collection_match)) or
    (ws.jsearch == end_node_match and jn->empty()))
  return true;
 return false;
}


bool Json::iterator::atomic_match_(const Jnode *jn, const char *lbl,
                                   const WalkStep &ws, map_jn * nsp) {
 // see if string/number/bool/null value matches
 if(ws.stripped.size() > 1)                                     // label attached: try matching
  if(lbl == nullptr or ws.stripped.back() != lbl) return false; // no label, or not matching

 auto user_json_ptr = &ws.user_json;                            // set user_json from walkstep
 switch (ws.jsearch) {
  case object_match:
  case indexable_match:
  case collection_match:
        return false;
  case wide_match:
  case end_node_match:
  case atomic_match:
        return true;
  case search_from_ns: {
        const auto found = nsp->find(ws.stripped[0]);           // see if value was preserved
        if(found == nsp->end())
         throw json().EXP(Jnode::walk_non_existant_namespace);
        user_json_ptr = &found->VALUE;                          // set user_json from namespace
       }
  case json_match:
        return *user_json_ptr == *jn;
  case query_unique:
        return is_unique_(*jn, ws);
  case Query_duplicate:
        return is_duplicate_(*jn, ws);
  case regular_match:
  case Regex_search:
  case digital_match:
  case Digital_regex:
        return string_match_(jn, ws, nsp);
  case label_match:
  case Label_RE_search:
  case tag_from_ns:
        return false;
  default:
        return bull_match_(jn, ws);
 }
}


bool Json::iterator::string_match_(const Jnode *jn, const WalkStep &ws, map_jn * nsp) const {
 // match numbers and strings also using RE
 switch(ws.jsearch) {
  case digital_match:
        return jn->is_number() and jn->val() == ws.stripped.front();
  case Digital_regex:
        return jn->is_number() and regex_match_(jn->val(), ws, nsp);
  case regular_match:
        return jn->is_string() and jn->val() == ws.stripped.front();
  case Regex_search:
        return jn->is_string() and regex_match_(jn->val(), ws, nsp);
  default:                                                      // should never reach here
        throw json().EXP(Jnode::walk_a_bug);                    // covering compiler's warning
 }
}


bool Json::iterator::regex_match_(const std::string &val, const WalkStep &ws, map_jn * nsp) const {
 // see if regex matches and if instance matches too - only then update the namespace
 std::smatch m;
 if(not std::regex_search(val, m, ws.re)) return false;
 for(size_t i = 0; i < m.size() ; ++i)                          // save matches in the namespace
  nsp->emplace( "$" + std::to_string(i), STR{std::move(m[i])});
 return true;
}


bool Json::iterator::label_match_(map_jn::iterator jit, const Jnode *jn, long idx,
                                  WalkStep &ws, map_jn * nsp) const {
 // return true if instance i of label (l,t) matches, false otherwise
 static map_jn::iterator found;
 if(ws.jsearch == tag_from_ns) {                                // facilitate <..>t / >..<t
  found = json().jns_.find(ws.stripped[0]);                     // see if value was preserved
  if(found == json().jns_.end())
   throw json().EXP(Jnode::walk_non_existant_namespace);        // value not found in NS, throw
  if(not (found->VALUE.type() AMONG(Jnode::String, Jnode::Number)))
   return false;                                                // value is neither string/numeric
 }
 else                                                           // <..>l|L or >..<l|L
  if(jn->is_array()) return false;                              // do not operate on ARY

 if(ws.is_recursive()) {                                        // <..>
  if(ws.jsearch == tag_from_ns)                                 // <..>t
   return jn->is_object()? jit->KEY == found->VALUE.val(): false;
  if(ws.jsearch == label_match)                                 // <..>l
   return jit->KEY == ws.stripped.front();
  return regex_match_(jit->KEY, ws, nsp);                       // <..>L
 }

 if(ws.jsearch == Label_RE_search)                              // facilitate >..<L
  return regex_match_(jit->KEY, ws, nsp);
                                                                // >..<: quant. is relative here
 long ws_off = ws.load_offset(json()),
      jn_size = static_cast<long>(jn->children_().size());

 if(ws.jsearch == tag_from_ns and found->VALUE.is_number()) {   // >..<t is numeric value
  long idx_val = found->VALUE.num();                            // resolve >..<t value in NS
  if(idx_val < 0 or idx_val >= jn_size) return false;           // outside of jn's chldren?
  if(ws_off < -idx_val)                                         // ws_off value too low?
   ws.head = ws.offset = ws_off = -idx_val;                     // fix too low head/offset values
  return idx - ws_off == idx_val;                               // return position match
 }
                                                                // jn is OBJ, all ARY processed
 auto found_lbl = jn->children_().find(ws.jsearch == tag_from_ns?
                                       found->VALUE.val(): ws.stripped.front());
 if(found_lbl == jn->children_().end()) return false;           // >..<t/l value's not found
 if(idx == 0) {                                                 // first run:
  long idx_val = std::distance(jn->children_().begin(), found_lbl);
  if(ws_off < -idx_val)                                         // offset too low?
   ws.head = ws.offset = ws_off = -idx_val;                     // fix too low offset
 }
 if(idx - ws_off < 0) return false;                             // outside of jn's children
 std::advance(jit, -ws_off);
 if(ws.jsearch == tag_from_ns)                                  // facilitate >..<t
  return jit->KEY == found->VALUE.val();                        // OBJ: return label match
 return jit->KEY == ws.stripped.front();                        // facilitate >..<l
}


bool Json::iterator::bull_match_(const Jnode *jn, const WalkStep &ws) const {
 // match bool and null (hence bull)
 switch(ws.jsearch) {
  case boolean_match:
        return jn->is_bool() and
               (ws.stripped.front() == STR_ANY or ws.stripped.empty() or
                (jn->bul()? ws.stripped.front() == STR_TRUE: ws.stripped.front() == STR_FALSE));
  case null_match:
        return jn->is_null();
  default:                                                      // should never reach here.
        throw json().EXP(Jnode::walk_a_bug);                    // covering compiler's warning
 }
}


bool Json::iterator::is_unique_(const Jnode & jn, const WalkStep &ws) {
 // match unique jsons: keep track of matched jsons in the namespaces
 static const Jnode not_unique;
 const std::string & prefix = ws.stripped.front();

 if(json().ujn_ptr_ == nullptr) {                               // first time, prepare
  purge_ns_(prefix);
  json().ujn_ptr_ = &not_unique;
 }
 if(json().ujn_ptr_ == &jn)
  return true;                                                  // same path being walked again

 size_t idx = 0;
 std::string key = prefix + std::to_string(idx);
 auto found = json().ns().find(key);

 while(found != json().ns().end()) {
  if(found->VALUE == jn)                                        // found, i.e. not unique
   { json().ujn_ptr_ = &not_unique; return false; }
  key = prefix + std::to_string(++idx);
  found = json().ns().find(key);
 }

 json().jns_[key] = jn;                                         // store in ns a unique value
 json().jns_[prefix] = jn;                                      // provide to user the unique value
 json().ujn_ptr_ = &jn;                                         // cache in, for re-walking
 return true;
}


bool Json::iterator::is_duplicate_(const Jnode & jn, const WalkStep &ws) {
 // match duplicate jsons: keep track of matched jsons in the namespaces
 static const Jnode unique;
 const std::string & prefix = ws.stripped.front();

 if(json().djn_ptr_ == nullptr) {                               // first time, prepare
  purge_ns_(prefix);
  json().djn_ptr_ = &unique;
 }
 if(json().djn_ptr_ == &jn)
  return true;                                                  // same path being walked again

 size_t idx = 0;
 std::string key = prefix + std::to_string(idx);
 auto found = json().ns().find(key);

 while(found != json().ns().end()) {
  if(found->VALUE == jn) {                                      // found, i.e. duplicate
   json().jns_[prefix] = jn;                                    // provide to user the dup value
   json().djn_ptr_ = &jn;                                       // cache in, for re-walking
   return true;
  }
  key = prefix + std::to_string(++idx);
  found = json().ns().find(key);
 }

 json().djn_ptr_ = &unique;
 json().jns_[key] = jn;                                         // store in ns a unique value
 return false;
}


void Json::iterator::purge_ns_(const std::string &pfx) {
 // erase from ns all cached value for given prefix
 size_t idx = 0;
 for(auto found = json().ns().find(pfx + "0");
     found != json().ns().end();
     found = json().ns().find(pfx + std::to_string(++idx)))
  json().jns_.erase(found);
}


void Json::iterator::lbl_callback_(const char *label,
                                   const std::vector<Json::CacheEntry> *vpv) {
 // invoke callback attached to the label (if there's one)
 if(json().lbl_callbacks().count(label) == 0) return;           // label not registered?

 GUARD(sn_type_ref_())                                          // not needed but idiomatically good
 sn_type_ref_() = Jnode::Object;                                // ensure supernode's correct type

 if(vpv == nullptr)                                             // callback w/o cache
  return json().lbl_callbacks()[label]( operator*() );          // call back passing a super node

 // vpv != nullptr: callback from search_all_()
 GUARD(pv_.size, pv_.resize)                                    // preserve pv_ and restore at exit
 for(auto &path: vpv->back().pv) pv_.push_back(path);           // augment path-vector
 json().lbl_callbacks()[label]( operator*() );                  // call back passing a super node
}


void Json::iterator::wlk_callback_(const Jnode *jnp) {
 // invoke callbacks matching given iterator
 for(auto &ic: json().wlk_callbacks()) {
  if(ic.itr == ic.itr.end()) continue;                          // iterations are over
  if(&ic.itr->value() != jnp) continue;                         // it's a different node

  ic.callback( *ic.itr );                                       // call back passing a super node
  json().engage_callbacks(false);
  ++ic.itr;
  json().engage_callbacks(true);
 }
}


void Json::iterator::dmx_callback_(const Jnode *jn, const char *lbl,
                                   const std::vector<Json::CacheEntry> *vpv) {
 // demux callbacks
 if(json().is_engaged(walk_callback)) wlk_callback_(jn);
 if(lbl and json().is_engaged(label_callback)) lbl_callback_(lbl, vpv);
}


bool Json::iterator::incremented(void) {
 // produce a next iterator per walk string (this call is just a wrapper for increment_)
 if(not pv_.empty() and pv_.back().jit == json().end_())
  return false;                                                 // don't attempt incrementing end()

 long wsi = next_iterable_ws_(walk_path_().size());
 if(wsi < 0) {
  pv_.emplace_back(json().end_(), true);
  DBG(json(), 2) DOUT(json()) << "path is out of iterations / non-iterable" << std::endl;
  return false;
 }

 return increment_(wsi);
}


bool Json::iterator::increment_(long wsi) {
 // increment walk step and re-walk: returns true / false upon successful / unsuccessful walk
 auto & ws = walk_path_()[ wsi ];

 ++ws.offset;
 unlock_failstops_(wsi);
 DBG(json(), 2) DOUT(json()) << "next increment: [" << wsi << "] " << ws << std::endl;
 size_t failed_wsi = walk_();
 if(pv_.empty() or pv_.back().jit != json().end_())
  return true;                                                  // successful walk

 DBG(json(), 2)
  DOUT(json()) << "WalkStep idx from a fruitless walk: " << failed_wsi << std::endl;
 if(wsi < static_cast<long>(failed_wsi)) {                      // it's not my position, plus
  DBG(json(), 2)                                                // it's in less significant place
   DOUT(json()) << "walk [" << failed_wsi
                << "] is out of iterations / non-iterable" << std::endl;
  // even if walk_ does not yield a result for a given wsi, if walk failed because of
  // other walk step (index), then next walk still might yield a match in the next record.
  // That logic is required to handle irregular JSONs
  long next_wsi = next_iterable_ws_(failed_wsi);
  return next_wsi < 0? false: increment_( next_wsi );
 }

 wsi = next_iterable_ws_(wsi);                                  // get next more significant wsi
 if(wsi < 0) return false;                                      // out of iteratables
 // here we need to reload the walkstep's offset with the head's value
 ws.offset = ws.type == WalkStep::range_walk and not ws.heads.empty()?
             LONG_MIN: ws.head;
             // LONG_MIN: indicates delayed (until actual walk) resolution
             // cannot resolve right now, cause NS might have changed by the next walking this ws
 return increment_(wsi);
}


long Json::iterator::next_iterable_ws_(long wsi) const {
 // get next more significant position (index within walk-path) of iterable offset,
 // otherwise (whole path non-iterable) return -1
 while(--wsi >= 0)
  if(walk_path_()[wsi].type == WalkStep::range_walk) break;
 DBG(json(), 3)
  DOUT(json()) << "iterable walk-step: ["
               << (wsi < 0? "N/A": std::to_string(wsi)) << "]" << std::endl;
 return wsi;
}


bool Json::iterator::failed_stop_(long wsi) {
 // check if there's a valid fail stop in the path (only first found FS should be checked?)
 auto & ws = walk_path_()[wsi];
 if(ws.jsearch == Forward_itr) return false;
 if(ws.type == WalkStep::range_walk) {
  if(ws.is_subscript()) return false;                           // failed subscript: dont engage FS
  if(ws.offset > ws.head) return false;                         // ws is search
 }

 for(; wsi >= 0; --wsi) {                                       // going backwards
  auto & ws = walk_path_()[wsi];
  if(ws.jsearch != Jsearch::fail_stop) continue;
  if(not ws.fs_path.empty() and ws.fs_path.back().jit == json().end_())
   return false;                                                // check only 1st found one
  DBG(json(), 3)
   DOUT(json()) << "found fail-stop at [" << wsi << "], restoring path " << std::endl;
  pv_ = ws.fs_path;
  return true;
 }

 return false;
}



size_t Json::utf8_adjusted(size_t start, const std::string &jsrc, size_t end) {
 // return string offset counted in unicode (UTF-8) characters
 size_t adj{0};
 if(end == static_cast<unsigned long>(-1)) end = jsrc.size();
 for(size_t i = start; i < end; ++i) {
  char c = jsrc[i];
  if(c >= 0) continue;                                          // non-unicode char
  if((c & 0x40) == 0) continue;                                 // UTF-8 non-initial byte
  c <<= 1;                                                      // must be an initial UTF-8 byte
  while((c & 0x80) > 0) { ++adj; c <<= 1; }                     // count leading bits but 1st one
 }
 return end - adj - start;
}


size_t Json::byte_offset(const std::string &jsrc, size_t utf8_offset) {
 // count unicode (wide) chars offset (utf8_offset) from the beginning of the byte-string
 size_t chr_offset{0};

 for(auto c: jsrc) {
  if(utf8_offset == 0) return chr_offset;
  if(c >= 0)                                                    // ascii char (0x0xxxxxxx)
   { --utf8_offset; ++chr_offset; continue; }
  if((c & 0x40) == 0)                                           // non-initial byte (0x10xxxxxx)
   { ++chr_offset; continue; }
  --utf8_offset;                                                // initial8 byte (0x11xxxxxx)
  while((c & 0x80) > 0)
   { ++chr_offset; c <<= 1; }                                   // skip leading bits eg: 0x1110xxxx
 }

 return chr_offset;
}





#undef DBG_WIDTH
#undef ARRAY_LMT
#undef KEY
#undef VALUE
#undef GLAMBDA

#undef PRINT_PRT
#undef PRINT_RAW
#undef CHR_TRUE
#undef CHR_FALSE
#undef CHR_NULL
#undef CHR_EOL
#undef CHR_RTRN
#undef CHR_QUOT
#undef STR_TRUE
#undef STR_FALSE
#undef STR_ANY
#undef STR_NULL
#undef JSN_OBJ_OPN
#undef JSN_OBJ_CLS
#undef JSN_ARY_OPN
#undef JSN_ARY_CLS
#undef JSN_STRQ
#undef JSN_ASPR
#undef JSN_NUMM
#undef JSN_NUMP
#undef JSN_NUMD
#undef LXM_SUB_OPN
#undef LXM_SCH_OPN
#undef LXM_SUB_CLS
#undef LXM_SCH_CLS

#undef LBL_SPR
#undef PFX_ITR
#undef PFX_WFR
#undef PFX_WFL
#undef RNG_SPR
#undef IDX_FIL

#undef JSN_FBDN
#undef JSN_QTD
#undef JSN_TRL











