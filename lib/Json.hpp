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
 *      - counter(pos)      // returns -1 if position in walk step is not iterable, otherwise
 *                          // returns current offset counter (i.e., works for *iterables*)
 *      - instance(pos)     // returns -1 if position is not a match instance ([n]), otherwise
 *                          // returns instance number to match (works with *non-iterable*)
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
 *   'Invalidate' - i.e. upon every new walk the entire search cache will be
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
 *
 */


#pragma once

#include <exception>
#include <vector>
#include <map>
#include <set>
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
#include <cstddef>
#include <bitset>
#include <unistd.h>             // for STDOUT_FILENO - for term width
#include "extensions.hpp"
#include "dbg.hpp"
#include "Outable.hpp"
#include "Streamstr.hpp"
//#include "Blob.hpp"             // SERDES interface (not needed so far)



// Class design notes:
// Jnode represents a single JSON value of any kind (from null to object).
// 1. for storing atomic JSON values (null, bool, string, number) an std:string
//    is used - internally all JSON atomic values are stored like that, type/value
//    validation occurs only during parsing.
// 2. JSON's Arrays and Objects are recurrent structures, which have to be stored
//    in STL containers. Both arrays and objects could be stored using the same
//    container type. Considering all implications, std::map it seems the most
//    suitable container for JSON:
//    - std::map caters random point remove/insert operations efficiently,
//    - label search is O(log(n)) complexity; iteration is not much worse than vector's
//    - for Array storage values (which do not require JSON labels), map keys will
//      be auto-sequenced

#define XSFY(X) #X
#define SFY(X) XSFY(X)                                          // stringify character

#define ARRAY_LMT 4                                             // #bytes per array's index
#define WLK_SUCCESS LONG_MIN                                    // walk() uses it as success
#define SIZE_T(N) static_cast<size_t>(N)
#define SGNS_T(N) static_cast<signed_size_t>(N)
#define KEY first                                               // semantic for map's pair
#define VALUE second                                            // instead of first/second
#define GLAMBDA(FUNC) [this](auto&&... arg) { return FUNC(std::forward<decltype(arg)>(arg)...); }

#define PRINT_PRT "\n"                                          // pretty print separator
#define PRINT_RAW ' '                                           // raw print separator
#define CHR_TRUE 't'                                            // designator for Json 'true' value
#define CHR_FALSE 'f'                                           // designator for Json 'false' val.
#define CHR_NULL '\0'                                           // end of string
#define CHR_SPCE ' '                                            // space
#define CHR_EOL '\n'                                            // end of line
#define CHR_RTRN '\r'                                           // return char
#define CHR_QUOT '\\'                                           // json quotation char
#define CHR_SLDS '/'                                            // solidus char
#define STR_TRUE "true"                                         // Json true sting value
#define STR_FALSE "false"                                       // Json false sting value
#define STR_NULL "null"                                         // Json null sting value
#define JSN_OBJ_OPN '{'                                         // Json syntax char: open object
#define JSN_OBJ_CLS '}'                                         // Json syntax char: close object
#define JSN_ARY_OPN '['                                         // Json syntax char: open array
#define JSN_ARY_CLS ']'                                         // Json syntax char: close array
#define JSN_STRQ '"'                                            // Json syntax char: string open
#define JSN_ASPR ','                                            // Json syntax char: val. separator
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
#define TKN_EMP "\x16"                                          // empty token in NS

#define JSN_FBDN "\b\f\n\r\t"                                   // forbidden JSON chars
#define JSN_QTD "/\"\\bfnrtu"                                   // chrs following quotation in JSON
#define JSN_TRL "/\"\\\b\f\n\r\t"                               // translated quoted chars
// NOTE: solidus quotation is optional per JSON spec, a user will have an option
//       to chose between desired quotation behavior

#define ITRP_BRC "{}"                                           // interpolation encasements
#define ITRP_JSY "<>"                                           // jsonization/stringification
#define ITRP_OPN first                                          // interpolation range semantic
#define ITRP_CLS second                                         // instead of first/second
#define UTF8_ILL "\xFF"                                         // used as placeholder for interp.
#define REGX_SPCL "\\^$.|?*+()[{"                               // regex special chars
#define UTF8_RRM1 "(?: *(?:"                                    // RE to erase UTF8_ILL enumeration
#define UTF8_RRM2 ")+ *)?|(?: *(?:"                             // part 2
#define UTF8_RRM3 ")+ *)?"                                      // part 3

#define ITRP_ALLM "$@"                                          // namespace for all REGEX matches
#define ITRP_ADLM "$#"                                          // token holding array delimiter
#define ITRP_GDLM "$$?"                                         // token holding delimiter for $?
#define ITRP_PDLM "$_"                                          // token holding path delimiter
#define ITRP_ASPR ", "                                          // default array separators
#define ITRP_GSPR ","                                           // default value when int'p. $?
#define ITRP_PSPR "_"                                           // stringified path separator
#define ITRP_PJSN "$PATH"                                       // token for JSON path interp.
#define ITRP_PSTR "$path"                                       // token for stringified path





class Json;
//
//                      Jnode class
//
//  o facilitates JSON tree itself
//  o programmatic DSL interface for JSON construction and all access methods
//  o iteration over JSON (objects/arrays)
//
class Jnode {
    friend class Json;

  friend std::ostream & operator<<(std::ostream & os, const Jnode & jnode)
                         { signed_size_t rl{0}; return print_json_(os, jnode, rl); }

    friend void         swap(Jnode &l, Jnode &r) {
                         using std::swap;                       // enable ADL
                         auto & lv = l.value();                 // first resolve super node
                         auto & rv = r.value();
                         swap(lv.type_, rv.type_);
                         swap(lv.value_, rv.value_);
                         swap(lv.descendants_, rv.descendants_);
                        }

    typedef ptrdiff_t signed_size_t;

 public:
    typedef std::map<std::string, Jnode> map_jn;
    typedef map_jn::iterator iter_jn;
    typedef map_jn::const_iterator const_iter_jn;

    #define THROWREASON /* throwreason defined with lowercase as they printed to the console */\
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
                invalid_code_point, \
                invalid_surrogate_code_pair, \
                unexpected_character_escape, \
                expected_valid_label, \
                expected_json_value, \
                expected_value_separator, \
                expecting_label_separator, \
                missed_prior_enumeration, \
                invalid_number, \
                unexpected_trailing, \
                end_of_json_parsing_exceptions, \
                \
                /* walk iterator exceptions */ \
                start_of_walk_exceptions, \
                json_lexeme_invalid, \
                walk_lexeme_missing_closure, \
                walk_search_label_with_attached_label, \
                walk_expect_search_label, \
                walk_expect_lexeme, \
                walk_label_seprator_bad_usage,          /* e.g.: ...[0]:: */\
                walk_unexpexted_suffix, \
                walk_bad_number_or_suffix, \
                walk_negative_quantifier, \
                walk_bad_quantifier, \
                walk_empty_lexeme, \
                walk_non_empty_lexeme, \
                walk_bad_position, \
                walk_root_has_no_label, \
                walk_non_existant_namespace, \
                walk_non_numeric_namespace, \
                Walk_callback_not_engaged, \
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

    #define CLASHINGLABELS \
                Override, \
                Merge
    ENUM(ClashingLabels, CLASHINGLABELS)

    #define PRETTYTYPE \
                Pretty, \
                Raw
    ENUM(PrettyType, PRETTYTYPE)


                        Jnode(void) = default;                  // DC
    virtual            ~Jnode(void) = default;                  // DD
                        Jnode(const Jnode &jn): Jnode() {       // CC

                         #ifdef BG_CC                           // -DBG_CC to compile this debug
                          if(DBG()(__Dbg_flow__::ind())) {        // dodge DBG's mutex dead-lock
                           GUARD(DBG().alt_prefix, DBG().alt_prefix)
                           DBG().alt_prefix(">");
                           DOUT() << DBG().prompt(__func__,
                                                  __Dbg_flow__::ind() + 1,
                                                  Debug::Indention::Alternative)
                                  << "CC copying: "
                                  << jn.to_string(Jnode::PrettyType::Raw, 0) << std::endl;
                          }
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

                        Jnode(Jnode &&jn) noexcept {            // MC
                         auto * volatile jnv = &jn.value();     // same here: moved jn could be an
                         if(jnv == nullptr)                     // empty supernode, hence checking
                          { std::swap(type_, jn.type_); return; }
                         swap(*this, jn);                       // swap will resolve supernode
                        }

    Jnode &             operator=(Jnode jn) noexcept {          // CA, MA
                         swap(*this, jn);
                         return *this;
                        }

                        // type conversions from Json:
                        Jnode(Json j);

                        // atomic values constructor adapters:
                        Jnode(double x): type_{Jtype::Number} {
                         std::stringstream ss;
                         ss << std::setprecision(std::numeric_limits<double>::digits10) << x;
                         value_ = ss.str();
                        }

                        Jnode(const std::string & s): type_{Jtype::String}, value_{s} {}
                        Jnode(const char *s): type_{Jtype::String}, value_{s} {}

                        template<typename T>
                        Jnode(T b, typename std::enable_if<std::is_same<T,bool>::value>
                                               ::type * = nullptr):
                         type_{Jtype::Bool}, value_{b? CHR_TRUE: CHR_FALSE} {}
                        // w/o above concept it would clash with double type

                        template<typename T>
                        Jnode(T, typename std::enable_if<std::is_null_pointer<T>::value>
                                                        ::type * = nullptr):
                         type_{Jtype::Null} {}


                        // JSON atomic type adapters (string, numeric, boolean):
                        operator const std::string & (void) const {
                         if(not is_string()) throw EXP(ThrowReason::expected_string_type);
                         return str();
                        }

                        operator double (void) const {
                         if(not is_number()) throw EXP(ThrowReason::expected_numerical_type);
                         return num();
                        }

                        operator bool (void) const {
                         if(not is_bool()) throw EXP(ThrowReason::expected_boolean_type);
                         return bul();
                        }
                        // concept ensures in-lieu application (avoid clashing with string type)

                        // class interface:
    std::string         to_string(PrettyType pt = PrettyType::Pretty,
                                  signed_size_t t = -1, bool sc = false) const {
                         GUARD(Jnode::endl_);
                         GUARD(Jnode::tab_);
                         GUARD(Jnode::semicompact_);
                         if(t >= 0) Jnode::tab(t);
                         std::stringstream ss;
                         ss << pretty(pt == PrettyType::Pretty).semicompact(sc);
                         return ss.str();
                        }

    Jtype               type(void) const { return value().type_; }
    Jnode &             type(Jtype x) { value().type_ = x; return *this; }
    bool                is_object(void) const { return type() == Jtype::Object; }
    bool                is_array(void) const { return type() == Jtype::Array; }
    bool                is_string(void) const { return type() == Jtype::String; }
    bool                is_number(void) const { return type() == Jtype::Number; }
    bool                is_bool(void) const { return type() == Jtype::Bool; }
    bool                is_null(void) const { return type() == Jtype::Null; }
    bool                is_iterable(void) const { return type() <= Jtype::Array; }
    bool                is_atomic(void) const { return type() > Jtype::Array; }
    bool                is_neither(void) const { return type() == Jtype::Neither; }

    size_t              size(void) const {                      // entire Jnode size
                         size_t size = 1;
                         for(auto &child: children_())
                          size += child.VALUE.size();
                         return size;
                        }

    bool                is_empty(void) const
                         { return type() <= Jtype::Array? children_().empty(): false; }

    bool                has_children(void) const                // i.e. non-empty()
                         { return type() <= Jtype::Array? not children_().empty(): false; }

    size_t              children(void) const
                         { return children_().size(); }

    Jnode &             clear(void) {
                         if(is_atomic()) throw EXP(ThrowReason::type_non_iterable);
                         children_().clear();
                         return *this;
                        }

    virtual Jnode &     operator[](long i) {
                         // signed_size_t type is used instead of size_t b/c super node's overload
                         // supports negative offsets
                         if(is_atomic()) throw EXP(ThrowReason::type_non_indexable);
                         return iterator_by_idx_(i)->VALUE;
                        }

    virtual Jnode &     operator[](int i)
                         { return operator[](SGNS_T(i)); }

  virtual const Jnode & operator[](long i) const {
                         if(is_atomic()) throw EXP(ThrowReason::type_non_indexable);
                         return iterator_by_idx_(i)->VALUE;
                        }

  virtual const Jnode & operator[](int i) const
                         { return operator[](SGNS_T(i)); }

    Jnode &             operator[](const std::string & l) {
                         if(not is_object()) throw EXP(ThrowReason::type_non_subscriptable);
                         return children_()[l];
                        }

    Jnode &             operator[](const char *l) {
                         if(not is_object()) throw EXP(ThrowReason::type_non_subscriptable);
                         return children_()[l];
                        }

    const Jnode &       operator[](const std::string & l) const {
                         if(not is_object()) throw EXP(ThrowReason::type_non_subscriptable);
                         return children_().at(l);
                        }

    const Jnode &       operator[](const char *l) const {
                         if(not is_object()) throw EXP(ThrowReason::type_non_subscriptable);
                         return children_().at(l);
                        }

    Jnode &             front(void) {
                         if(not is_iterable()) throw EXP(ThrowReason::type_non_iterable);
                         return children_().begin()->second;
                        }

    const Jnode &       front(void) const {
                         if(not is_iterable()) throw EXP(ThrowReason::type_non_iterable);
                         return children_().begin()->second;
                        }

    const std::string & front_label(void) const {
                         if(not is_object()) throw EXP(ThrowReason::expected_object_type);
                         return children_().begin()->first;
                        }

    Jnode &             back(void) {
                         if(not is_iterable()) throw EXP(ThrowReason::type_non_iterable);
                         return children_().rbegin()->second;
                        }

    const Jnode &       back(void) const {
                         if(not is_iterable()) throw EXP(ThrowReason::type_non_iterable);
                         return children_().rbegin()->second;
                        }

    const std::string & back_label(void) const {
                         if(not is_object()) throw EXP(ThrowReason::expected_object_type);
                         return children_().rbegin()->first;
                        }

    bool                operator==(const Jnode &jn) const {
                         if(type() != jn.type()) return false;
                         if(is_atomic())
                          return is_number()? num() == jn.num(): val() == jn.val();
                         return children_() == jn.children_();
                        }

    bool                operator!=(const Jnode &jn) const { return not operator==(jn); }

    bool                operator<(const Jnode &jn) const;

                        // access json types (type checked)
    const std::string & str(void) const {
                         if(not is_string()) throw EXP(ThrowReason::expected_string_type);
                         return value().value_;
                        }

    double              num(void) const {
                         if(not is_number()) throw EXP(ThrowReason::expected_numerical_type);
                         return stod(value().value_);
                        }

    size_t              integer(void) const {
                         if(not is_number()) throw EXP(ThrowReason::expected_numerical_type);
                         return stoul(value().value_);
                        }


    bool                bul(void) const {
                         if(not is_bool()) throw EXP(ThrowReason::expected_boolean_type);
                         return value().value_.front() == CHR_TRUE;
                        }

                        // return atomic value w/o atomic type checking
    const std::string & val(void) const {
                         if(is_iterable()) throw EXP(ThrowReason::expected_atomic_type);
                         return value().value_;
                        }

                        // modify json
    Jnode &             erase(const std::string & l) {
                         if(not is_object()) throw EXP(ThrowReason::expected_object_type);
                         children_().erase(l);
                         return *this;
                        }

    Jnode &             erase(size_t i) {
                         if(not is_array()) throw EXP(ThrowReason::expected_array_type);
                         children_().erase(iterator_by_idx_(i));
                         return *this;
                        }

    Jnode &             push_back(Jnode jn) {
                         if(not is_array()) throw EXP(ThrowReason::expected_array_type);
                         children_().emplace(next_key_(), std::move(jn));
                         return *this;
                        }


    Jnode &             pop_back(void) {
                         if(not is_iterable()) throw EXP(ThrowReason::type_non_iterable);
                         if(not children_().empty())
                          children_().erase(std::prev(children_().end()));
                         return *this;
                        }

    Jnode &             normalize_idx(void);                    // after removal

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
    virtual bool        has_label(void) const
                         { throw EXP(ThrowReason::label_accessed_not_via_iterator); }
    virtual const std::string &
                        label(void) const
                         { throw EXP(ThrowReason::label_accessed_not_via_iterator); }
    virtual bool        has_index(void) const
                         { throw EXP(ThrowReason::index_accessed_not_via_iterator); }
  virtual signed_size_t index(void) const
                         { throw EXP(ThrowReason::index_accessed_not_via_iterator); }
    virtual bool        is_root(void) const
                         { throw EXP(ThrowReason::method_accessed_not_via_iterator); }
                        // label() / index() / is_root() supposed to be used by a super
                        // node only (cannot be pure defined, hence throwing)
    virtual Jnode &     value(void) { return *this; }           // for iterator
  virtual const Jnode & value(void) const { return *this; }     // for iterator & const_iterator

                        // global setting
    bool                is_merging(void) const
                         { return clashing_labels_ == ClashingLabels::Merge; }
    Jnode &             merge_clashing(bool x = true) {
                         clashing_labels_ = x? ClashingLabels::Merge: ClashingLabels::Override;
                         return *this;
                        }
    const Jnode &       merge_clashing(bool x = true) const {
                         clashing_labels_ = x? ClashingLabels::Merge: ClashingLabels::Override;
                         return *this;
                        }
    bool                is_overriding(void) const
                         { return clashing_labels_ == ClashingLabels::Override; }
    Jnode &             override_clashing(bool x = true) {
                         clashing_labels_ = x? ClashingLabels::Override: ClashingLabels::Merge;
                         return *this;
                        }
    const Jnode &       override_clashing(bool x = true) const {
                         clashing_labels_ = x? ClashingLabels::Override: ClashingLabels::Merge;
                         return *this;
                        }

    bool                is_pretty(void) const { return endl_ == PRINT_PRT; }
    Jnode &             pretty(bool x = true)
                         { endl_ = x? PRINT_PRT: std::string(tab(), PRINT_RAW); return *this; }
    const Jnode &       pretty(bool x = true) const
                         { endl_ = x? PRINT_PRT: std::string(tab(), PRINT_RAW); return *this; }
    bool                is_raw(void) const { return endl_ != PRINT_PRT; }
    Jnode &             raw(bool x = true)
                         { endl_ = x? std::string(tab(), PRINT_RAW): PRINT_PRT; return *this; }
    const Jnode &       raw(bool x = true) const
                         { endl_ = x? std::string(tab(), PRINT_RAW): PRINT_PRT; return *this; }
    size_t              tab(void) const { return tab_; }
    Jnode &             tab(size_t n) { tab_ = n; return *this; }
    const Jnode &       tab(size_t n) const { tab_ = n; return *this; }
    bool                is_semicompact(void) const { return semicompact_; }
    Jnode &             semicompact(bool x = true)
                         { semicompact_ = x; return *this; }
    const Jnode &       semicompact(bool x = true) const
                         { semicompact_ = x; return *this; }

    //SERDES(type_, value_, descendants_)                       // not really needed yet
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
    std::string         idx2lbl_(size_t idx) const {
                         std::stringstream ss;
                         ss << std::hex << std::setfill(IDX_FIL)
                                        << std::setw(ARRAY_LMT * 2) << idx;
                         return ss.str();
                        }

                        // Jnode data
    Jtype               type_{Jtype::Object};
    std::string         value_;                                 // value (number/string/bool)
    map_jn              descendants_;                           // array/dictionary

 private:
  static std::ostream & print_json_(std::ostream & os, const Jnode & me, signed_size_t & rl);
  static std::ostream & print_iterables_(std::ostream & os, const Jnode & me, signed_size_t & rl);

    static Jnode::ClashingLabels
                        clashing_labels_;
    static std::string  endl_;                                  // either for raw or pretty print
    static size_t       tab_;                                   // tab size (for indention)
    static bool         semicompact_;                           // arrays made of atomics compacted
};


// class static definitions
Jnode::ClashingLabels Jnode::clashing_labels_{Jnode::ClashingLabels::Override};
std::string Jnode::endl_{PRINT_PRT};                            // default is pretty format
size_t Jnode::tab_{1};
bool Jnode::semicompact_{false};

STRINGIFY(Jnode::ThrowReason, THROWREASON)
#undef THROWREASON

STRINGIFY(Jnode::Jtype, JTYPE)
#undef JTYPE



//
// DSL style JSON constructors definitions:
//
struct NUL: public Jnode {
                        NUL(void): Jnode{Jtype::Null} {}
};



struct BUL: public Jnode {
                        BUL(bool x): Jnode{Jtype::Bool}
                         { value_ = x? CHR_TRUE: CHR_FALSE; }
};



struct NUM: public Jnode {
                        NUM(double x): Jnode{Jtype::Number} {
                         std::stringstream ss;
                         ss << std::setprecision(std::numeric_limits<double>::digits10) << x;
                         value_ = ss.str();
                        }
};



struct STR: public Jnode {
                        STR(void): Jnode{Jtype::String}
                         { }
                        STR(const std::string & x): Jnode{Jtype::String}
                         { value_ = x; }
                        STR(std::string && x): Jnode{Jtype::String}
                         { value_ = std::move(x); }
};



struct ARY: public Jnode {
                        ARY(void): Jnode{Jtype::Array} {}
                        template<typename... Args>
                        ARY(Args&&... args): Jnode{Jtype::Array}
                         { add_(std::forward<Args>(args)...); }
 private:
    void                add_(const Jnode & jn)
                         { children_().emplace(next_key_(), jn); };
    void                add_(Jnode && jn)
                         { children_().emplace(next_key_(), std::move(jn)); };
    template<typename... Args>
    void                add_(Jnode && first, Args && ... rest) {
                         add_(std::forward<decltype(first)>(first));
                         add_(std::forward<Args>(rest)...);
                        }
};





struct LBL: public Jnode {
                        LBL(const std::string & l, const Jnode & j):
                         Jnode{j}, label{l} {}
                        LBL(const std::string & l, Jnode && j):
                         Jnode{std::move(j)}, label{l} {}
                        LBL(std::string && l, const Jnode & j):
                         Jnode{j}, label{std::move(l)} {}
                        LBL(std::string && l, Jnode && j):
                         Jnode{std::move(j)}, label{std::move(l)} {}
    std::string         label;
};



struct OBJ: public Jnode {
                        OBJ(void): Jnode{Jtype::Object} {}
                        template<typename... Args>
                        OBJ(Args&&... args): Jnode{Jtype::Object}
                         { add_(std::forward<Args>(args)...); }
 private:
    void                add_(LBL && lbl)
                         { children_().emplace(std::move(lbl.label), std::move(lbl)); };
    template<typename... Args>
    void                add_(LBL && first, Args && ... rest) {
                         add_(std::forward<decltype(first)>(first));
                         add_(std::forward<Args>(rest)...);
                        }
};





//
//                          Jnode iterator implementation
//
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
    friend SWAP(Jnode::Iterator<T>, ji_, sn_.parent_type())
    // sn_.parent_type(): supernode requires swapping of type_ only

    // Supernode definition
    //
    class SuperJnode: public Jnode {
        friend Jnode;
        friend Jnode::Iterator<T>;

     public:
        bool                has_label(void) const
                             { return lbp_ != nullptr and parent_type() == Jtype::Object; }
        const std::string & label(void) const {
                             if(type_ != Jtype::Object)
                              throw EXP(ThrowReason::label_request_for_non_object_enclosed);
                             return *lbp_;
                            }
        bool                has_index(void) const
                             { return lbp_ != nullptr and parent_type() == Jtype::Array; }
        signed_size_t       index(void) const {
                             if(type_ != Jtype::Array)
                              throw EXP(ThrowReason::index_request_for_non_array_enclosed);
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
                        Iterator(Iterator &&it) noexcept        // MC
                         { swap(*this, it); }
    Iterator &          operator=(Iterator it) noexcept         // CA, MA
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
    Iterator<T>         operator++(int) { auto tmp = *this; ++(*this); return tmp; }
    Iterator<T>         operator--(int) { auto tmp = *this; --(*this); return tmp; }

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
    SuperJnode          sn_{Jtype::Neither};

 private:
    iter_jn &           underlying_(void) { return ji_; }
    const iter_jn &     underlying_(void) const { return ji_; }

};



bool Jnode::operator<(const Jnode &jn) const {
 // comparing '<' Jnodes:
 // if types are different, then compare types only (in the reverse order they defined)
 //   - e.g., an empty object {} is bigger than a non-empty array [...], etc
 // otherwise (types are the same), arbitration is following:
 //   1. compare by sizes, otherwise
 //   2. compare by depth: shallower is less than deeper, otherwise
 //   3. if atomic: compare atomics by values, otherwise,
 //   4. compare child by child, if all children values are the same,
 //   5. if objects: compare label by label, if the same,
 //   6. return false - Jnodes are equal
 if(type() != jn.type()) return type() > jn.type();

 Jnode::const_iterator chld_ptr;
 size_t xl, xr;

 switch (type()) {                                              // same types here
  case Jtype::Object:
  case Jtype::Array:
        xl = size();
        xr = jn.size();
        if(xl != xr) return xl < xr;
        if(children() != jn.children()) return children() > jn.children();
        chld_ptr = jn.begin();
        for(auto &chld: *this) {
         if(chld_ptr == jn.end()) return false;                 // jn out of nodes, we're bigger
         if(chld != *chld_ptr) return chld < *chld_ptr;         // compare children
         ++chld_ptr;
        }
        if(children() < jn.children()) return true;             // we are smaller (a subset of jn)
        if(is_object()) {                                       // objects are the  same in values
         chld_ptr = jn.begin();                                 // compare by labels
         for(auto &chld: *this) {
          if(chld.label() != chld_ptr->label())
           return chld.label() < chld_ptr->label();
          ++chld_ptr;
         }
        }
        return false;                                           // completely equal iterables
  case Jtype::String:
        return str() < jn.str();
  case Jtype::Number:
        return num() < jn.num();
  case Jtype::Bool:
        return bul() < jn.bul();
  case Jtype::Null:
  case Jtype::Neither:
        return false;
 }
 throw EXP(Jnode::ThrowReason::walk_a_bug);
}



Jnode & Jnode::normalize_idx(void) {
 // after yanking from JSON array, the array indices require re-normalization
 if(is_atomic()) return *this;

 if(is_array() and has_children())                              // normalize only if tampered
  if(stoul(children_().rbegin()->KEY, nullptr, 16) >= children_().size()) { // indices are tampered
   bool good_idx{true};
   map_jn new_children;                                         // move normalized children here
   for(auto &child: children_()) {                              // fix all indices in the array
    if(good_idx and new_children.size() == stoul(child.KEY, nullptr, 16))
     new_children.emplace(child.KEY, std::move(child.VALUE));
    else {
     good_idx = false;
     new_children.emplace(idx2lbl_(new_children.size()), std::move(child.VALUE));
    }
   }
   children_() = std::move(new_children);
  }

 for(auto &child: children_())                                  // process recursively
  child.VALUE.normalize_idx();

 return *this;
}



Jnode::iterator Jnode::begin(void) {
 if(is_atomic())
  throw EXP(ThrowReason::type_non_iterable);
 return {children_().begin(), type()};
}



Jnode::const_iterator Jnode::begin(void) const {
 if(is_atomic())
  throw EXP(ThrowReason::type_non_iterable);
 return {children_().begin(), type()};
}



Jnode::const_iterator Jnode::cbegin(void) const {
 return begin();
}



Jnode::iterator Jnode::end(void) {
 if(is_atomic())
  throw EXP(ThrowReason::type_non_iterable);
 return {children_().end(), type()};
}



Jnode::const_iterator Jnode::end(void) const {
 if(is_atomic())
  throw EXP(ThrowReason::type_non_iterable);
 return {children_().end(), type()};
}



Jnode::const_iterator Jnode::cend(void) const {
 return end();
}



Jnode & Jnode::erase(iterator & it) {
 if(is_atomic())
  throw EXP(ThrowReason::type_non_iterable);
 it.underlying_() = children_().erase(it.underlying_());
 return *this;
}



Jnode & Jnode::erase(const_iterator & it) {
 if(is_atomic())
  throw EXP(ThrowReason::type_non_iterable);
 it.underlying_() = children_().erase(it.underlying_());
 return *this;
}



Jnode & Jnode::erase(const_iterator && it) {
 if(is_atomic())
  throw EXP(ThrowReason::type_non_iterable);
 children_().erase(it.underlying_());
 return *this;
}



size_t Jnode::count(const std::string & l) const {
 if(not is_object())
  throw EXP(ThrowReason::expected_object_type);
 return children_().count(l);
}



Jnode::iterator Jnode::find(const std::string & l) {
 if(not is_object())
  throw EXP(ThrowReason::expected_object_type);
 return {children_().find(l), type()};
}



Jnode::const_iterator Jnode::find(const std::string & l) const {
 if(not is_object())
  throw EXP(ThrowReason::expected_object_type);
 return {children_().find(l), type()};
}



Jnode::iterator Jnode::find(size_t idx) {
 if(not is_iterable())
  throw EXP(ThrowReason::type_non_iterable);
 return {iterator_by_idx_(idx), type()};
}



Jnode::const_iterator Jnode::find(size_t idx) const {
 if(not is_iterable())
  throw EXP(ThrowReason::type_non_iterable);
 return {iterator_by_idx_(idx), type()};
}



//
// Jnode private methods implementation
//
Jnode::iter_jn Jnode::iterator_by_idx_(size_t idx) {
 // iterator_by_idx_ may be used in both array and dictionary indexing operation.
 if(idx >= children_().size())
  throw EXP(ThrowReason::index_out_of_range);

 if(is_array()) {                                               // array could be addressed direct
  size_t key = stoul(children_().rbegin()->KEY, nullptr, 16);
  if(key < children_().size())                                  // if so, indices are non-tampered
   return children_().find(std::move(idx2lbl_(idx)));
 }                                                              // else: traverse map
 auto it = children_().begin();
 std::advance(it, idx);
 return it;
}



Jnode::const_iter_jn Jnode::iterator_by_idx_(size_t idx) const {
 // const version
 if(idx >= children_().size())
  throw EXP(ThrowReason::index_out_of_range);

 if(is_array()) {                                               // array may be addressed directly
  size_t key = stoul(children_().rbegin()->KEY, nullptr, 16);
  if(key < children_().size())                                  // if so, indices are non-tampered
   return children_().find(std::move(idx2lbl_(idx)));
 }                                                              // else: traverse map
 auto it = children_().cbegin();
 std::advance(it, idx);
 return it;
}



std::string Jnode::next_key_(void) const {
 size_t key = 0;
 if(not children_().empty())
  key = stoul(children_().rbegin()->KEY, nullptr, 16) + 1;
 return idx2lbl_(key);
}



std::ostream & Jnode::print_json_(std::ostream & os, const Jnode & me, signed_size_t & rl) {
 // output Jnode to `os`
 auto & my = me.value();                                        // resolve if virtual object
 auto sc_print =
  [&] {                                                         // semi-compact print
   if(not semicompact_) return false;
   if(std::any_of(my.children_().begin(), my.children_().end(),
                  [](const auto &j){ return j.VALUE.is_iterable() and j.VALUE.has_children(); } ))
    return false;
   GUARD(Jnode::endl_)                                          // facilitate semi-compact printing
   GUARD(Jnode::tab_)
   my.tab(1).raw();
   os << endl_;
   signed_size_t rl{0};
   print_iterables_(os, my, rl);
   return true;
  };
 switch(my.type()) {
  case Jtype::Object:
        os << JSN_OBJ_OPN;
        if(my.is_empty()) return os << JSN_OBJ_CLS;
        if(sc_print()) return os;
        os << endl_;
        break;
  case Jtype::Array:
        os << JSN_ARY_OPN;
        if(my.is_empty()) return os << JSN_ARY_CLS;
        if(sc_print()) return os;
        os << endl_;
        break;
  case Jtype::Bool:
        return os << (my.bul()? STR_TRUE: STR_FALSE);
  case Jtype::Null:
        return os << STR_NULL;
  case Jtype::Number:
        return os << my.val();
  case Jtype::String:
        return os << JSN_STRQ << my.str() << JSN_STRQ;
  default:                                                      // case Neither
        #ifdef BG_CC
         return os;
        #else
         throw me.EXP(ThrowReason::walk_a_bug);
        #endif
 }
 return print_iterables_(os, my, rl);
}



std::ostream & Jnode::print_iterables_(std::ostream & os, const Jnode & my, signed_size_t & rl) {
 // process children in iterables (array or object)
 if(endl_ == PRINT_PRT) ++rl;                                   // if pretty print - adjust level

 for(auto & child: my.children_()) {                            // print all children:
  os << std::string(rl * tab_, PRINT_RAW);                      // output current indent
  if(my.is_object())                                            // if parent (me) is Object
   os << JSN_STRQ << child.KEY << JSN_STRQ                      // print label
      << LBL_SPR << (endl_.empty()? "":" ");                    // and separator
  print_json_(os, child.VALUE, rl)                              // then print child itself and the
   << (child.KEY != my.children_().rbegin()->KEY? ",": "")      // trailing comma if not the last
   << endl_;
 }

 if(rl > 1) os << std::string((rl - 1) * tab_, PRINT_RAW);      // would also signify pretty print
 os << (my.is_array()? JSN_ARY_CLS: JSN_OBJ_CLS);               // close array/object

 if(endl_ == PRINT_PRT) --rl;                                   // if pretty print - adjust level
 return os;
}





//
//                              Json class:
//  o wraps Jnode, relaying all Jnode interfaces
//  o provides parsing of JSON
//  o walk-path methods and walk-iterator
//  o class is extensively debuggable
//


class Json {
    friend std::ostream & operator<<(std::ostream & os, const Json & my)
                           { return os << my.root(); }

    #define PARSE_THROW /* used in parsing, parse_range_, validate_number_, parse_index_ */ \
                May_throw, \
                Dont_throw
    ENUM(ParseThrow, PARSE_THROW)

    #define SIGN_VALIDATION /* used in parsing, parse_range_, validate_number_, parse_index_ */ \
                Must_be_non_negative,   /* 0, or positive, '+' sign allowed */ \
                Must_be_positive,       /* strictly positive, '+' sign allowed */ \
                Must_be_non_positive,   /* negative or 0, '-' sign allowed */ \
                Must_be_signless, /* number without '+' or '-' signs, 0 allowed */ \
                May_be_any
    ENUM(SignLogic, SIGN_VALIDATION)

 public:
    typedef ptrdiff_t signed_size_t;

    // Design notes on Namespace handling in Json class:
    // o Json class does access namespace entry through ptr()/ref(), meaning that if remote
    //   entry is referenced, then the remote entry is accessed, otherwise it's a local entry
    // o Upon [re]write always a local namespace entry is created (even if a remote entry
    //   currently exists)
    class JnEntry: public Jnode {
     // entry for name-space, facilitates version (increments every time entry updates)
     public:
                            JnEntry(void) = default;
                            JnEntry(Jnode &&jn): Jnode(std::move(jn)) {}
        JnEntry &           operator=(Jnode jn) noexcept {          // CA, MA
                             swap(static_cast<Jnode&>(*this), jn);
                             ptr(nullptr);                      // ensure remote is dereferenced
                             return *this;
                            }
        // access methods
        JnEntry *           ptr(void) { return ptr_ == nullptr? this: ptr_; }
        const JnEntry *     ptr(void) const { return ptr_ == nullptr? this: ptr_; }
        JnEntry &           ref(void) { return ptr_ == nullptr? *this: *ptr_; }
        const JnEntry &     ref(void) const { return ptr_ == nullptr? *this: *ptr_; }
        bool                is_local() const { return ptr_ == nullptr; }
        bool                is_remote() const { return ptr_ != nullptr; }

        void                ptr(JnEntry *p) { ptr_ = p; }

     private:
        JnEntry *           ptr_{nullptr};
    };
    typedef std::map<std::string, JnEntry> map_jne;

    // suffixes taken : abcdDefFgGiIjklLnNoPrRstuvqQwWzZ
    #define JS_ENUM /* first letter defines the lexeme */\
                regular_match,  /* string match */ \
                Regex_search,   /* RE match for JSON strings only */ \
                Phrase_match,   /* matches any JSON string value - same as <.*>R */ \
                digital_match,  /* numerical match */ \
                Digital_regex,  /* RE match for JSON numericals only */ \
                Numerical_match,/* matches any JSON numerical value - same as <.*>D */ \
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
                query_original, /* use namespaces to find original (first-seen) JSONs */ \
                Query_duplicate,/* use namespaces to find duplicates JSONs (opposite to q) */ \
                go_ascending,   /* walk elements sorted in the ascending order */ \
                Go_descending,  /* walk elements sorted in the descending order */ \
                /* following are directives, not search lexemes: */ \
                value_of_json,  /* directive: save current json value */ \
                key_of_json,    /* directive: treat key (label/index) as a JSON (and save) */ \
                zip_namespace,  /* directive: erase namespace */ \
                fail_safe,      /* directive: stop at the preserved point when ws fails */ \
                Forward_itr,    /* directive: proceed to the next iteration (w/o fail_safe) */ \
                user_handler,   /* directive: user-specific callback to validate the path */ \
                Increment_num,  /* directive: increments JSON numeric value by offset*/ \
                Zip_size,       /* directive: save into the namespace size of a current entry*/ \
                Walk_path,      /* directive: preserve current walk-path in a namespace*/ \
                Step_walk,      /* directive: counterpart of Walk_path: walks preserved path*/ \
                end_of_lexemes, \
                text_offset,    /* in addition to search, these two used for subscript offsets */ \
                numeric_offset  /* to disambiguate numeric subscripts [0], from textual [zero] */
    ENUMSTR(Jsearch, JS_ENUM)
    // Jsearch lexemes are defined so that their first letter corresponds to the suffix

    #define PARSETRAILING \
                /* "relaxed" actions do not throw once parsing of JSON is complete */ \
                Relaxed_trailing,   /* default, allow all spaces past valid json, then stops */ \
                Relaxed_no_trail,   /* stops right after json, exp_ points to the next char */ \
                /* "strict" actions always throw if described violation: */ \
                Strict_trailing,    /* allow only white spaces past valid json, throw otherwise */\
                Strict_no_trail,    /* don't allow anything past json, throw otherwise */ \
                /* not for parsing, used by interpolate() only: */ \
                Dont_parse          /* put result of interpolation into a JSON string */
    ENUMSTR(ParseTrailing, PARSETRAILING)

    #define CACHE_STATE         /* all non-dynamic searches are cached (when walking) */\
                Invalidate, \
                Keep_cache
    ENUM(CacheState, CACHE_STATE)

    #define KOJ_ARG             /* argument for is_koj_last() */\
                EmptyKoj,       /* <>k */\
                NonEmptyKoj     /* <..>k */
    ENUM(KeyOfJson, KOJ_ARG)


                        Json(void) = default;
                        Json(const Jnode &jn): root_{jn.value()} { }
                        Json(Jnode &&jn): root_{std::move(jn.value())} { }

                        Json(const std::string &str) { parse(str); }
                        Json(const char *c_str) { parse( std::string{c_str} ); }

    // class interface:
    Jnode &             root(void) { return root_; }
    const Jnode &       root(void) const { return root_; }
    Json &              parse(std::string jstr, ParseTrailing trail = Relaxed_trailing) {
                         Streamstr tmp;
                         DBG().severity(tmp);
                         tmp.source_buffer(std::move(jstr));
                         return parse(tmp.begin(), trail);
                        }
    Json &              parse(Streamstr::const_iterator && jsi, ParseTrailing t = Relaxed_trailing)
                         { return parse(jsi, t); }
    Json &              parse(Streamstr::const_iterator & jsi, ParseTrailing = Relaxed_trailing);
    Json &              parse_throwing(bool x) {
                         parse_throw_ = x? ParseThrow::May_throw: ParseThrow::Dont_throw;
                         return *this;
                        }
    bool                parse_throwing(void) const
                         { return parse_throw_ == ParseThrow::May_throw; }
    Streamstr::const_iterator
                        exception_point(void) { return exp_; }
    Jnode::ThrowReason
                        exception_reason(void) { return exr_; }
    bool                parsing_failed(void) const
                         { return exr_ != Jnode::ThrowReason::start_of_json_parsing_exceptions; }
    class iterator;
    iterator            walk(const std::string & walk_string = std::string{},
                             CacheState = Invalidate);
    void                compile_walk(const std::string & wstr, iterator & it) const;
    std::string         unquote_str(const std::string & src) const;
    std::string         inquote_str(const std::string & src) const;

    // relayed Jnode interface
    std::string         to_string(Jnode::PrettyType pt = Jnode::PrettyType::Pretty,
                                  signed_size_t t = -1) const
                         { return root().to_string(pt, t); }
    Jnode::Jtype        type(void) const { return root().type(); }
    Json &              type(Jnode::Jtype x) { root().type(x); return *this; }
    bool                is_object(void) const { return root().is_object(); }
    bool                is_array(void) const { return root().is_array(); }
    bool                is_string(void) const { return root().is_string(); }
    bool                is_number(void) const { return root().is_number(); }
    bool                is_bool(void) const { return root().is_bool(); }
    bool                is_null(void) const { return root().is_null(); }
    bool                is_iterable(void) const { return root().is_iterable(); }
    bool                is_atomic(void) const { return root().is_atomic(); }
    bool                is_neither(void) const { return root().is_neither(); }
    size_t              size(void) const { return root().size(); }
    bool                is_empty(void) const { return root().is_empty(); }
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
    bool                operator<(const Json &j) const { return root() < j.root(); };
    bool                operator<(const Jnode &j) const { return root() < j; };

    const std::string & str(void) const { return root().str(); }
    double              num(void) const { return root().num(); }
    size_t              integer(void) const { return root().integer(); }
    bool                bul(void) const { return root().bul(); }
    const std::string & val(void) const { return root().val(); }
    Json &              erase(const std::string & l) { root().erase(l); return *this; }
    Json &              erase(size_t i) { root().erase(i); return *this; }
    Json &              push_back(Jnode jn)
                         { root().push_back(std::move(jn)); return *this; }
    Json &              pop_back(void)
                         { root().pop_back(); return *this; }
    Json &              normalize_idx(void) { root().normalize_idx(); return *this; }

    Jnode::iterator     begin(void) { return root().begin(); }
  Jnode::const_iterator begin(void) const { return root().begin(); }
  Jnode::const_iterator cbegin(void) const { return root().cbegin(); }
    Jnode::iterator     end(void) { return root().end(); }
  Jnode::const_iterator end(void) const { return root().end(); }
  Jnode::const_iterator cend(void) const { return root().cend(); }
    Json &              erase(Jnode::iterator & it) { root().erase(it); return *this; }
    Json &              erase(Jnode::const_iterator & it) { root().erase(it); return *this; }
    size_t              count(const std::string & l) const { return root().count(l); }
    Jnode::iterator     find(const std::string & l) { return root().find(l); }
  Jnode::const_iterator find(const std::string & l) const { return root().find(l); }
    Jnode::iterator     find(size_t i) { return root().find(i); }
  Jnode::const_iterator find(size_t i) const { return root().find(i); }

    bool                is_merging(void) const { return root().is_merging(); }
    Json &              merge_clashing(bool x = true) { root().merge_clashing(x); return *this; }
    const Json &        merge_clashing(bool x = true) const
                         { root().merge_clashing(x); return *this; }
    bool                is_overriding(void) const { return root().is_overriding(); }
    Json &              override_clashing(bool x = true)
                         { root().override_clashing(x); return *this; }
    const Json &        override_clashing(bool x = true) const
                         { root().override_clashing(x); return *this; }
    bool                is_pretty(void) const { return root().is_pretty(); }
    Json &              pretty(bool x = true) { root().pretty(x); return *this; }
    const Json &        pretty(bool x = true) const { root().pretty(x); return *this; }
    bool                is_raw(void) const { return root().is_raw(); }
    Json &              raw(bool x = true) { root().raw(x); return *this; }
    const Json &        raw(bool x = true) const { root().raw(x); return *this; }
    size_t              tab(void) const { return root().tab(); }
    Json &              tab(size_t n) { root().tab(n); return *this; }
    bool                is_semicompact(void) const { return root().is_semicompact(); }
    Json &              semicompact(bool x = true) { root().semicompact(x); return *this; }
    const Json &        semicompact(bool x = true) const { root().semicompact(x); return *this; }
    bool                is_solidus_quoted(void) const { return jsn_fbdn_[0] == CHR_SLDS; }
    Json &              quote_solidus(bool quote) {
                         jsn_fbdn_ = quote? "/" JSN_FBDN: JSN_FBDN;
                         quotedsolidus_ = quote;
                         return *this;
                        }
    Json &              clear_cache(void) { srchc_.clear(); sortc_.clear(); return *this; }
    map_jne &           ns(void) { return jns_; }               // namespace
    const map_jne &     ns(void) const { return jns_; }         // const namespace
                        // use ns(key) form only for an access, for write use ns()[key] instead,
                        // because Json class always creates a new entry in the NS upon write
                        // and does not rewrite a remote one
    JnEntry &           ns(const std::string &key) { return jns_[key].ref(); }
    const JnEntry &     ns(const std::string &key) const { return jns_.at(key).ref(); }
    Json &              clear_ns(const std::string &s = "")     // clear namespace
                         { if(s.empty()) ns().clear(); else ns().erase(s); return *this; }

    // calling clear_cache is required once JSON was modified anyhow; it's called
    // anyway every time a new walk is build, thus the end-user must call it only
    // when continue walking iterators (with search iterators) past JSON modification

    //SERDES(root_)                                             // not required (so far)
    DEBUGGABLE()
    EXCEPTIONS(Jnode::ThrowReason)

    static Jnode::Jtype json_number_definition(Streamstr::const_iterator & jsp);
    static Jnode::Jtype json_number_definition(std::string::const_iterator & jsp);
    static size_t utf8_adjusted(size_t start, const std::string &jsrc, size_t end = -1);


 protected:
    // protected data structures
    Jnode               root_;                                  // underlying JSON structure
    Streamstr::const_iterator
                        exp_;                                   // exception pointer
    Jnode::ThrowReason  exr_{Jnode::ThrowReason::start_of_json_parsing_exceptions}; // excp. reason
    ParseThrow          parse_throw_{ParseThrow::May_throw};
    map_jne             jns_;                                   // jnode namespace
    const char *        jsn_fbdn_{JSN_FBDN};                    // JSN_FBDN pointer
    const char *        jsn_qtd_{JSN_QTD};                      // JSN_QTD pointer


 private:
    typedef std::vector<std::string> vec_str;
    struct WalkStep;                                            // required for some methods below
    bool                quotedsolidus_{false};                  // preserve solidus quoted'\/'?

    auto                end_(void) { return root().children_().end(); } // frequently used shortcut
    auto                end_(void) const { return root().children_().end(); }
    void                parse_(Jnode & node, Streamstr::const_iterator &jsp);
    void                parse_bool_(Jnode & node, Streamstr::const_iterator &jsp);
    void                parse_string_(Jnode & node, Streamstr::const_iterator &jsp);
    void                parse_number_(Jnode & node, Streamstr::const_iterator &jsp);
    void                parse_array_(Jnode & node, Streamstr::const_iterator &jsp);
    void                parse_object_(Jnode & node, Streamstr::const_iterator &jsp);
    void                merge_(std::set<std::string> &c2a, Jnode &n, Jnode &&l, Jnode &&c);
    char                skip_blanks_(Streamstr::const_iterator & jsp);
    std::string         readup_str_(Streamstr::const_iterator & jsp, size_t n);
    Jnode::Jtype        classify_jnode_(Streamstr::const_iterator & jsp);
    Streamstr::const_iterator &
                        find_delimiter_(char c, Streamstr::const_iterator & jsp);
    Streamstr::const_iterator &
                        validate_number_(Streamstr::const_iterator & jsp);

    // methods facilitating walk-path feature
    void                parse_lexemes_(const std::string & wstr, iterator & it) const;
    std::string         extract_lexeme_(std::string::const_iterator &si, char closing) const;
    void                parse_suffix_(std::string::const_iterator &, iterator &, vec_str&) const;
    void                parse_user_json_(WalkStep &ws) const;
  std::regex::flag_type parse_RE_flags_(std::string & re_str) const;
    Jsearch             search_suffix_(char sfx) const;
    void                parse_quantifier_(std::string::const_iterator &si, iterator & it) const ;
    void                parse_range_(std::string::const_iterator &, WalkStep &, ParseThrow) const;
    std::string         parse_namespaced_qnt_(std::string::const_iterator &) const;
    signed_size_t       parse_index_(std::string::const_iterator &,
                                     ParseThrow, SignLogic = SignLogic::May_be_any) const;
    Json &              erase_ns_(const std::string &s) {       // unlike clear_ns, this one
                         ns()[s].type(Jnode::Jtype::Neither);   // never erases NS, rather
                         ns()[s].ptr(nullptr);
                         return *this;                          // marks existing nodes as erased
                        }
    static void         build_path_(Jnode &jpath, const Json::iterator &jit);
    //
    // Properties and structures used by walk Json::iterator:
    //

    // Itr (iterator-label pair):
    // - used by walk iterator (path_vector) and search cache key
    struct Itr {
        // path-vector is made of Itr - result of walking WalkStep vector (walk path)
        // last Itr in path-vector points to the found JSON element (via jit)
        // lbl and jnp keep a copy of jit's key and Jnode addr: this is required for
        // validation - jit could be invalidated due to a prior JSON manipulation,
        // preserved lbl and jnode addr ensure safe execution of is_nested() and
        // is_valid() methods;
        friend SWAP(Itr, jit, lbl, jnp)

                            Itr(void) = default;                // for pv_.resize()
                            Itr(const Itr &) = default;         // CC
                            Itr(Itr && itr) = default;          // MC
        Itr &               operator=(Itr itr) noexcept         // CA, MA
                             { swap(*this, itr); return *this; }

                            Itr(const Jnode::iter_jn &it):      // for emplacement of good itr
                             jit(it), lbl(it->KEY), jnp(&it->VALUE) {}
                            Itr(const Jnode::iter_jn &it, bool):
                             jit(it) {}                         // for emplacement of end() only!

        // reminder:    typedef std::map<std::string, Jnode>::iterator iter_jn;
        Jnode::iter_jn      jit;                                // iterator pointing to JSON node
        std::string         lbl;                                // label and node ptr are required
        const Jnode *       jnp{nullptr};                       // for iterator (jit) validation
    };

    typedef std::vector<Itr> path_vector;                       // used by iterator and CahceEntry


    // SearchCacheEntry:
    // - used by SearchCache: preserves a path for a found search (and its current namespace)
    struct SearchCacheEntry {
        // namespace at the given instance of the path, to support its REGEX values
        friend SWAP(SearchCacheEntry, ns, pv)

                            SearchCacheEntry(void) = default;
                            SearchCacheEntry(const SearchCacheEntry &) = default;   // CC
                            SearchCacheEntry(SearchCacheEntry && ce) = default;     // MC
        SearchCacheEntry &  operator=(SearchCacheEntry ce) noexcept                 // CA, MA
                             { swap(*this, ce); return *this; }

        map_jne             ns;                         // ns only for given cache entry
        path_vector         pv;
    };


    struct Gremap {
        // facilitate entries being sorted in SortCacheEntry
        friend SWAP(Gremap, jnp, ord);
                            Gremap(void) = delete;                  // DC
                            Gremap(const Gremap &) = default;       // CC
                            Gremap(Gremap && x) = default;          // MC
                            Gremap(const Jnode *jp, size_t o = 0):  // for emplacement
                             jnp(jp), ord(o) {}
        Gremap &            operator=(Gremap x) noexcept            // CA, MA
                             { swap(*this, x); return *this; }

        const Jnode *       jnp;                                    // jnode being cached
        size_t              ord;                                    // encounter index in search

     static bool            cmp(const Gremap &l, const Gremap &r)
                             { return *l.jnp == *r.jnp? l.ord < r.ord: *l.jnp < *r.jnp; }
    };

    struct SortCacheEntry {
        // cache (for a given <>g/G lexeme) holding all entries in a sorted order
        friend SWAP(SortCacheEntry, sortset, offsetmap)

                            SortCacheEntry(void) = default;
                            SortCacheEntry(const SortCacheEntry &) = default;   // CC
                            SortCacheEntry(SortCacheEntry && ce) = default;     // MC
        SortCacheEntry &    operator=(SortCacheEntry ce) noexcept               // CA, MA
                             { swap(*this, ce); return *this; }

        std::set<Gremap, decltype(&Gremap::cmp)>
                            sortset{Gremap::cmp};               // sorted  jnodes (w. ordinal idx)
        std::vector<size_t> offsetmap;                          // remap encounter idx->sorted idx
    };


    // WalkStep:
    // - used by walk iterator and search cache key
    struct WalkStep {
        // WalkStep contains data describing one walk lexeme (subscript or search or directive)
        // walk path is made of walk steps instructing how JSON tree to be traversed
        friend SWAP(WalkStep, jsearch, type,
                              offset, head, tail, step, offsets, heads, tails, steps,
                              lexeme, stripped, rexp, user_json, locked, wsuid, fs_path)

        #define WALKSTEPTYPE    /* walk types for subscripts only - irrelevant in searches */\
            Static_select,      /* [3], [0], quantifier: 0, 3, etc */ \
            Parent_select,      /* [-2], quantifier: NA */ \
            Root_select,        /* [^5], quantifier: NA */ \
            Range_walk,         /* [+0], [1:], [4:10], [-5:-1], quantifiers: +0, 3:, 4:10 */ \
            Directive,          /* this is used for directives only */ \
            Directive_inactive, /* this is used to deactivate directives */ \
            Cache_complete      /* this is used in **CacheKey** only */
                                // Note: there's no negative range for iterable quantifier: no way
                                // to know upfront the number of hits a recursive search'd produce
        ENUMSTR(WsType, WALKSTEPTYPE)

                            WalkStep(void):                         // DC
                             wsuid{Json::iterator::uuid_++} {}
                            WalkStep(const WalkStep &) = default;   // CC
                            WalkStep(WalkStep && ws) = default;     // MC
        WalkStep &          operator=(WalkStep ws) noexcept         // CA, MA
                             { swap(*this, ws); return *this; }

                            WalkStep(std::string && l, Jsearch js): // enable emplacement
                             jsearch(js), lexeme(std::move(l)), wsuid{Json::iterator::uuid_++} {}

        bool                operator<(const WalkStep &r) const {    // used only in CacheKey:
                             // CacheKey is the key of SearchCache. Even though WalkStep has a
                             // wsuid, which uniquely identifies walk step, CacheKey provides
                             // below differentiation logic (instead of basing on a simple wsuid)
                             // for 2 reasons:
                             //  1) similar WS could be reused for cacheing purpose
                             //  2) original WalkStep (wsuid) might not even exist, while cache
                             //     still could be actual and hence reused
                             // WS with json capturing properties must be considered as empty
                             if(is_subscript() or r.is_subscript())
                              return jsearch < r.jsearch;

                             static const std::string empty{};      // used for capturing lexemes
                             std::vector<const std::string *> lptr{&stripped.front()},
                                                              rptr{&r.stripped.front()};
                             lptr.push_back(stripped.size() > 1? &stripped.back(): &empty);
                             rptr.push_back(r.stripped.size() > 1? &r.stripped.back(): &empty);
                             if(is_json_capturing_lexeme()) lptr.front() = &empty;
                             if(r.is_json_capturing_lexeme()) rptr.front() = &empty;

                             return jsearch != r.jsearch?       // for caching purpose quantifiers
                                     jsearch < r.jsearch:       // are not considered
                                     is_user_json_search()?
                                      ( user_json != r.user_json?
                                         user_json < r.user_json:
                                         *lptr.back() < *rptr.back()
                                      ):
                                      is_regex()?
                                       // regex match uses user_json as a str value and user_json
                                       // type as a flag for interpolation, thus using val()
                                       ( user_json.val() != r.user_json.val()?
                                          user_json.val() < r.user_json.val():
                                          *lptr.back() < *rptr.back()
                                       ):
                                       ( *lptr.front() != *rptr.front()?
                                          *lptr.front() < *rptr.front():
                                          *lptr.back() < *rptr.back()
                                       );
                            }
        bool                resolve_ns(Json::iterator &jit) {
                             // resolve namespace value for dynamic lexemes into user_json:
                             // return true only if it's required setting user_json back to Neither
                             auto & jsn = jit.json();
                             switch(jsearch) {
                              case Jsearch::search_from_ns:
                              case Jsearch::tag_from_ns: {
                                    const auto found = jsn.ns().find(stripped.front());
                                    if(found == jsn.ns().end() or found->VALUE.ref().is_neither())
                                     throw jsn.EXP(Jnode::ThrowReason::walk_non_existant_namespace);
                                    user_json = found->VALUE.ref();
                                    break;
                                   }
                              case Jsearch::json_match: {
                                    if(user_json.is_neither()) {    // i.e. it's a template
                                     user_json = Json::interpolate(stripped.front(), jit);
                                     if(user_json.is_neither())
                                      throw jsn.EXP(Jnode::ThrowReason::json_lexeme_invalid);
                                     return true;                   // ensure reset to Neither
                                    }
                                    // user_json is a valid JSON, but it still could be a template
                                    // if template was within a string. Try interpolating once
                                    // and see if there's a difference
                                    if(not stripped.front().empty()) {
                                     Jnode j = std::move(user_json);
                                     user_json = Json::interpolate(stripped.front(), jit);
                                      if(user_json.is_neither())
                                       throw jsn.EXP(Jnode::ThrowReason::json_lexeme_invalid);
                                     if(user_json != j) return true;// tmp: reset it to Neither
                                     stripped.front().clear();      // indicate no intp. needed
                                    }
                                    return false;                   // don't reset to Neither
                                   }
                              case Jsearch::Regex_search:
                              case Jsearch::Digital_regex:
                              case Jsearch::Label_RE_search: {
                                    // interpolation renders RE search slower, so use it
                                    // only if there is interpolation token in RE lexeme
                                    if(user_json.is_neither())      // request to interpolate
                                     user_json = Json::interpolate(stripped.front(), jit, jsn.ns(),
                                                                   Json::ParseTrailing::Dont_parse);
                                    auto restr = user_json.val();   // copy is required here
                                    rexp = std::regex(restr, jsn.parse_RE_flags_(restr));
                                    if(user_json.val() == stripped.front())
                                     { user_json.type(Jnode::Jtype::Neither); break; }
                                    // user_json will not be set to Neither => dont intp. next time
                                    else return true;
                                    // user_json will be set to Neither => interpolate next time
                                   }
                              default: throw jsn.EXP(Jnode::ThrowReason::walk_a_bug);
                             }
                             return false;
                            }

        // various walk properties
        bool                is_recursive(void) const
                             { return lexeme.front() == LXM_SCH_OPN; }
        bool                is_non_recursive(void) const
                             { return lexeme.front() == LXM_SCH_CLS; }
        bool                is_regex(void) const
                             { return jsearch AMONG(Jsearch::Regex_search, Jsearch::Digital_regex,
                                                    Jsearch::Label_RE_search); }

        bool                is_lexeme_dynamic(void) const
                             { return jsearch == Jsearch::search_from_ns or
                                      jsearch == Jsearch::tag_from_ns; }
        bool                may_interpolate_content(void) const
                             { return jsearch == Jsearch::json_match or is_regex(); }
        bool                require_ns_resolution(void) const
                             { return is_lexeme_dynamic() or may_interpolate_content(); }
        bool                is_user_json_search(void) const     // search based on user_json
                             { return is_lexeme_dynamic() or jsearch == Jsearch::json_match; }

        bool                is_qnt_namespace_based(void) const
                             { return not heads.empty() or not tails.empty(); }
        bool                is_lbl_based(void) const            // operates on label types l/L/t
                             { return jsearch AMONG(Jsearch::label_match, Jsearch::Label_RE_search,
                                                    Jsearch::tag_from_ns); }
        bool                is_val_based(void) const            // operates on value
                             { return not is_lbl_based(); }
        bool                is_tag_based(void) const            // only tags 't' and 'l'
                             { return jsearch == Jsearch::label_match or
                                      jsearch == Jsearch::tag_from_ns; }
        bool                is_qnt_relative(void) const         // only in >..<t/l/I
                             { return jsearch == Jsearch::Increment_num or  // let parsing <..>I-n
                                      (is_non_recursive() and is_tag_based()); }
        bool                is_cacheless(void) const
                             { return is_qnt_relative(); }
        bool                is_subscript(void) const
                             { return lexeme.front() == LXM_SUB_OPN; }
        bool                is_search(void) const
                             { return not is_subscript(); }
        bool                is_directive(void) const {
                             return jsearch AMONG(Jsearch::key_of_json, Jsearch::value_of_json,
                                                  Jsearch::zip_namespace, Jsearch::fail_safe,
                                                  Jsearch::Forward_itr, Jsearch::user_handler,
                                                  Jsearch::Increment_num, Jsearch::Zip_size,
                                                  Jsearch::Walk_path, Jsearch::Step_walk);
                            }
        bool                is_not_directive(void) const
                             { return not is_directive(); }
        bool                is_lexeme_required(void) const
                             { return jsearch AMONG(Jsearch::Regex_search, Jsearch::digital_match,
                                                    Jsearch::Digital_regex,
                                                    Jsearch::Label_RE_search, Jsearch::json_match,
                                                    Jsearch::search_from_ns, Jsearch::tag_from_ns,
                                                    Jsearch::value_of_json, Jsearch::user_handler,
                                                    Jsearch::Increment_num, Jsearch::zip_namespace,
                                                    Jsearch::Zip_size, Jsearch::Walk_path,
                                                    Jsearch::Step_walk); }
        bool                must_lexeme_be_empty(void) const
                             { return false; }
        bool                is_json_capturing_lexeme(void) const {
                             // i.e. if lexeme suppose to preserve JSON into the namespace
                             // e.g.: <namespace>v, or <ns:user_json>v
                             if(jsearch == Jsearch::boolean_match)
                              return stripped.front() == STR_TRUE? false:
                                     stripped.front() == STR_FALSE? false:
                                     true;
                             return jsearch AMONG(Jsearch::null_match, Jsearch::atomic_match,
                                                  Jsearch::object_match, Jsearch::indexable_match,
                                                  Jsearch::collection_match,
                                                  Jsearch::end_node_match, Jsearch::wide_match,
                                                  Jsearch::Phrase_match, Jsearch::Numerical_match,
                                                  Jsearch::value_of_json, Jsearch::key_of_json,
                                                  Jsearch::fail_safe, Jsearch::Forward_itr,
                                                  Jsearch::query_original,
                                                  Jsearch::Query_duplicate, Jsearch::go_ascending,
                                                  Jsearch::Go_descending
                                                  // Increment_num: don't include here
                                                 ); }
        signed_size_t       load_offset(const Json &j) const {
                             if(offsets.empty() or type == WsType::Range_walk) return offset;
                             return fetch_from_ns(offsets, j);
                            }
        signed_size_t       load_head(const Json &j) const
                             { return heads.empty()? head: fetch_from_ns(heads, j); }
        signed_size_t       load_tail(const Json &j) const
                             { return tails.empty()? tail: fetch_from_ns(tails, j); }
        signed_size_t       load_step(const Json &j) const
                             { return steps.empty()? step: fetch_from_ns(steps,j); }
        signed_size_t       fetch_from_ns(const std::string &val, const Json &j) const {
                             const auto found = j.ns().find(val);
                             if(found == j.ns().end() or found->VALUE.ref().is_neither())
                              throw j.EXP(Jnode::ThrowReason::walk_non_existant_namespace);
                             if(found->VALUE.ref().type() != Jnode::Jtype::Number)
                              throw j.EXP(Jnode::ThrowReason::walk_non_numeric_namespace);
                             signed_size_t x = strtol(found->VALUE.ref().val().c_str(),nullptr,10);
                             if(x < 0 and not is_tag_based())   // only t/l can go negative
                              throw j.EXP(Jnode::ThrowReason::walk_negative_quantifier);
                             return x;
                            }
        bool                is_locked(void) const { return locked; }

        // Walkstep data:
        Jsearch             jsearch;                            // Jsearch type (r/R/d/D/n/l/L etc)
        WsType              type{WsType::Static_select};        // lexeme's type (static/range/etc)
        signed_size_t       offset{0};                          // current offset
        signed_size_t       head{0};                            // range walk type
        signed_size_t       tail{LONG_MAX};                     // by default - till the end
        signed_size_t       step{1};                            // increment step
        std::string         offsets;                            // interpolatable offset
        std::string         heads;                              // interpolatable head
        std::string         tails;                              // interpolatable tail
        std::string         steps;                              // interpolatable step
        std::string         lexeme;                             // lexeme w/o suffix and quantifier
        vec_str             stripped;                           // stripped lexeme, + optional lbl
                            // stripped[0] (always present): holds a stripped lexeme,
                            // or NS w/o initializer, or a expanded template value
                            // stripped[1]: attached label match (e.g.: [label]:<..>), if required
        std::regex          rexp;                               // RE for R/L/D suffixes
        Jnode               user_json{Jnode::Jtype::Neither};   // for those allowing storing JSON
        bool                locked{false};                      // indicate locked/unlocked state
        unsigned short      wsuid;                              // unique walk-id for ws
        path_vector         fs_path;                            // preserved path for fail-safe
        // fail_safe/Forward_itr design notes:
        // 1. fail_safe (FS) <..>f and Forward_itr (FI) <..>F directives design:
        //  o directives may be found in 2 states: locked and unlocked
        //    - in unlocked state FS can be engaged - i.e. it preserves pv and may intercept
        //      walking upon failure
        //    - in the locked state FS is disengaged - no action is taken (by directive) and
        //      walking continues past locked directive's domain (either to <end> or to past the
        //      next <>F / ><F directive)
        //  o when FS directive is in locked, it also locks its entire domain of operation: from
        //    the FS directive itself up until <end> or to the next <>F / ><F directive inclusive
        //
        // ---- clarification on locking behavior ----
        // is FS locking really required? consider following cases:
        //       A: a <>f b [:] c
        //       B: a <>f b [:] c ><F d [:] e
        // from the above, locked state should not provide any increments or processing, until FS
        // domain is unlocked:
        //  - if b-c domain fails (which locks FS): then d-e domain needs to be incremented all the
        //    way until it's out, then `a` needs to be incremented (which unlocks FS `b-c` domain)
        //  - if b-c does not fail, (FS is unlocked) then increments must begin from domain `b-c`
        //    (d-e never gets engaged)
        //
        // Thus:
        //  - locking occurs upon failure within <>f domain (b-c)
        //  - unlocking (of <>f) then must occur when incrementing an iterator prior <>f - (a)
        // ----
        //  o FS is engaged when failure of walk-steps within the FS domain occurs:
        //    - `failed_stop_(wsi)` - validates FS engagement: starting from WS index (wsi):
        //    0.a. if failed WS is a range (e.g. [:])
        //          AND failing JSON is iterable (array/object)
        //          AND ws offset (counter) is >= #iterable's children,
        //         - then it's legit failure - don't engage
        //    0.b. if failed WS is a range (e.g. [:])
        //          AND failing JSON is *atomic* (increment is an artifact of prior fs engagement)
        //          AND offset was incremented (i.e. above ws' head)
        //         - then it's a legit failure - don't engage
        //    - otherwise, process all ws going backwards:
        //    1. if WS <..>F/>..<F faced - it's another domain
        //       (domain of<>f is until {end, <..>F/>..<F}) - don't engage
        //    2. finally restore the path, lock FS domain and return WS index of <..>f
        //  o locking out <>f domain:
        //    - locking occurs immediately when failure is detected within FS domain
        //    - locked state is maintained within WS structure and entire domain must be locked
        //  o unlocking <>f domain:
        //    - unlocking occurs by increment of preceding of FS' domain iterator:
        //      e.g.: `[+0] <>f a <>F b <..>f c` - unlocking of both <>f and <..>f must occur
        //    - locked flag is taken off from entire domain
        //
        // 2. Forward_itr (FI) <..>F, >..<F directive:
        //  o behavior depends on the search semantic given:
        //     - <..>F directive instructs to skip silently to the next walk iteration w/o
        //       displaying the result
        //     - >..<F directive instructs stop further processing (display found / walked value)
        //     - usage of <>F, ><F (with default or 0 quantifier) makes sense only when paired
        //       to <>f directive
        //  o if quantifier (n) is 0 the behavior is as described above, though if n > 0 then:
        //     - <>Fn: jumps n lexemes further, e.g.: <>F1 executes exactly the following lexeme
        //             <>F2 will skip to 2nd following lexeme, etc
        //     - ><Fn: replication directive: makes the whole walk-path additionally repeat n
        //             times, e.g,: ><F9 will result in 10 exact walks (1 original and 9
        //             additional)
        //     - when n > 0, the directive (in both forms) is  meaningful to be used as standalone,
        //       as well as when paired with <>f
        //
        // 3. implications on incrementing walk-path (incremented(), next_iterable_ws_())
        //    <>f directive introduces branching into walk-path, it affects then how successive
        //    increments of the walk path work:
        //   o when incremented() calls next_iterable_ws_() - it must first figure if there was
        //     a FS engaged or not:
        //     - if was, then FS (locked) domain must be skipped and ws next_iterable_ws_() must be
        //       called with the respective LS walk-step:
        //       `... <>f a b ><F c d` - if `a` or `b` failed (`<>f` domain is locked) then
        //       next_iterable_ws_() must be called with `d`
        //     - otherwise (FS did not fail) the next_iterable_ws_() must find the respective WS
        //       within its domain:
        //       `... <>f a b ><F c d` - <>f domain is unlocked, then next_iterable_ws_() must be
        //       called with `b` (or allowed even with ><F's index)
        //   o when next_iterable_ws_() searches for the next MS iterator, it must skip locked
        //     domains entirely

        // Below definitions only for COUTABLE interface
        const char *        search_type() const
                             { return ENUMS(Jsearch, jsearch); }
        const std::string   label() const
                             { return stripped.size()==2? stripped.back(): "N/A"; }
        std::string         json() const
                             { return user_json.is_neither()?
                                       "N/A": user_json.to_string(Jnode::PrettyType::Raw, 1); }
        const char *        ws_type(void) const
                             { return jsearch != Jsearch::numeric_offset?
                                       "N/A": ENUMS(WsType, type); }
        std::string         range() const {
                             if(type != WsType::Range_walk) return "N/A";
                             std::stringstream ss;
                             ss << "[" << (heads.empty()?
                                           head == 0? "": std::to_string(head):
                                           "{" + heads + "}") << ':'
                                << (tails.empty()?
                                    tail == LONG_MAX? "": std::to_string(tail):
                                    "{" + tails + "}")
                                << (steps.empty()?
                                    step == 1? "": ":" + std::to_string(step):
                                    ":{" + steps + "}") << "]";
                             return ss.str();
                            }
        std::string         ofst() const {                      // only for COUTABLE
                             if(type == WsType::Range_walk and offset != LONG_MIN)
                              return std::to_string(offset);
                             std::stringstream ss;
                             ss << (offsets.empty()? std::to_string(offset): "{" + offsets + "}");
                             return ss.str();
                            }
        COUTABLE(WalkStep, search_type(), lexeme, ws_type(), ofst(), range(), label(), json())
    };


    // Search Cache Key:
    // - made of jnode pointer and walk step
    struct CacheKey {
        // Search may occur off any JSON node and in any of WalkSteps, hence a unique
        // key would be their combination
        // WalkStep (ws) needs to be preserved (cannot be referenced) as the original
        // walk path might not even exist, while the cache could be still alive and actual
        friend SWAP(CacheKey, jnp, ws)                              // provide ADL swap operator

                            CacheKey(void) = delete;                // DC
                            CacheKey(const CacheKey &) = default;   // CC
                            CacheKey(CacheKey && sck) = default;    // MC
        CacheKey &          operator=(CacheKey sck) noexcept        // CA, MA
                             { swap(*this, sck); return *this; }

                            CacheKey(const Jnode *jp, const WalkStep & w):// for emplacement
                             jnp(jp), ws(w) {}

        const Jnode *       jnp;
        WalkStep            ws;

        static bool         cmp(const CacheKey &l, const CacheKey &r)
                             { return l.jnp != r.jnp? l.jnp<r.jnp: l.ws<r.ws; }

        const Jnode *       json_node(void) const { return jnp; }   // only for COUTABLE
        COUTABLE(CacheKey, json_node(), ws)
    };                                                          // end of struct WalkStep

    static std::map<std::string, std::vector<WalkStep>>
                        compiled_walks_;                        // compiled walks cache

    typedef std::map<CacheKey,
                     std::vector<SearchCacheEntry>,
                     decltype(&CacheKey::cmp)>
                        SearchCache;

    typedef std::map<unsigned short, SortCacheEntry>
                        SortCache;

    struct Stringover: public std::string {
        // a trivial std::string adapter: overrides string holder only if argument is non-empty
        // utilized by interpolation routines
                            Stringover(std::string str):
                             std::string{std::move(str)} {}

        Stringover &        operator=(const std::string &str)
                             { if(not str.empty()) std::string::operator=(str); return *this; }

        bool                ends_with(std::string const &tail) const {
                             if(std::string::size() < tail.size()) return false;
                             return std::string::compare(std::string::size() - tail.size(),
                                                         tail.size(), tail) == 0;
                            }

    };

    // parse_subscript_type_() is dependent on WalkStep definition, hence moved down here
    void                parse_subscript_type_(WalkStep & state) const;

    static map_jne          dummy_ns_;                          // default arg in interpolate()


 public:

    //                      walk Json::iterator
    //
    // Json::iterator (a.k.a. walk iterator): needs to be defined in-class to facilitate
    // container storage with the iterator (e.g. walk-based callbacks)
    //
    class iterator: public std::iterator<std::forward_iterator_tag, Jnode> {
     // this forward iterator let iterating *walk paths* over JSON
     // once iterator is dereferenced it returns a reference to a Jnode's super-node,
     // Json::iterator's super-node adds following powers to a regular Jnode:
     // - label() allows accessing node's label if node is nested by object
     // - index() allows accessing node's ordinal index if nested by array
     // - value() provides access to the Jnode itself (but this methods is quite
     //   redundant)
     // - is_root() let checking if the node is root() or not
     // - operator[] receives a new capability: on a negative index it will address
     //              indexed levels up in the JSON's tree hierarchy (e.g." [-1] will
     //              address a parent of the dereferenced node, and so on)
        friend Json;
        friend class GuideWp;

        friend COPY(Json::iterator, lwsi_, ws_, pv_, jp_, wuid_, sn_.parent_type())
        friend SWAP(Json::iterator, lwsi_, ws_, pv_, jp_, wuid_, sn_.parent_type())
        // sn_.parent_type(): supernode requires swapping of type_ values only

        // walk iterator's Super node class definition
        //
        class SuperJnode: public Jnode {
            friend Json::iterator;

         public:
            bool                has_label(void) const {
                                 return lbl_ != &Json::iterator::dummy_lbl_ and
                                        parent_type() == Jtype::Object;
                                }
            const std::string & label(void) const {
                                 if(type_ != Jtype::Object)
                                  throw EXP(ThrowReason::label_request_for_non_object_enclosed);
                                 return *lbl_;
                                }
            bool                has_index(void) const {
                                 return lbl_ != &Json::iterator::dummy_lbl_ and
                                        parent_type() == Jtype::Array;
                                }
            signed_size_t       index(void) const {
                                 if(type_ != Jtype::Array)
                                  throw EXP(ThrowReason::index_request_for_non_array_enclosed);
                                 return std::stoul(lbl_->c_str(), nullptr, 16);
                                }
            Jnode &             value(void) { return *jnp_; }
            const Jnode &       value(void) const { return *jnp_; }
            bool                is_root(void) const { return &jit_->json().root() == jnp_; }
            Jnode &             operator[](signed_size_t i) {
                                 // in addition to Jnode::iterator's, this one adds capability
                                 // to address supernode with negative index, e.g: [-1],
                                 // like in the walk-string to reference a parent
                                 if(i >= 0) return Jnode::operator[](i);
                                 return -i >= SGNS_T(jit_->pv_.size())?
                                         jit_->json().root():
                                         jit_->pv_[jit_->pv_.size() + i -1].jit->VALUE;
                                }
            const Jnode &       operator[](signed_size_t i) const {
                                 if(i >= 0) return Jnode::operator[](i);
                                 return -i >= SGNS_T(jit_->pv_.size())?
                                         jit_->json().root():
                                         jit_->pv_[jit_->pv_.size() + i -1].jit->VALUE;
                                }
            Jtype               parent_type(void) const { return type_; }
            Jtype &             parent_type(void) { return type_; }
                                // that is a work around GNU's compiler bug-limitation, which
                                // does not extend scoping onto subclasses, i.e.:
                                // swap(l.sn_.type_, r.sn_.type_) fails.

         private:
                                SuperJnode(Jnode::Jtype t = Jnode::Jtype::Neither): // private DC
                                 Jnode{t} {}

            SuperJnode &        operator()(const std::string &l, Jnode &jn, Json::iterator *jit)
                                 { lbl_ = &l; jnp_ = &jn; jit_ = jit; return *this; }

            SuperJnode &        operator()(Jnode &jn, Json::iterator * jit) {
                                 lbl_ = &Json::iterator::dummy_lbl_;
                                 jnp_ = &jn;
                                 jit_ = jit;
                                 return *this;
                                }

            const std::string * lbl_{&Json::iterator::dummy_lbl_}; // lbl_ should never be nullptr
            Jnode *             jnp_{nullptr};                  // iterator's Jnode pointer
            Json::iterator *    jit_{nullptr};                  // back to iterator, for [-n]
        };
        //
        // end of Super node definition

     public:
                            iterator(void):                     // DC
                             wuid_{Json::iterator::uuid_++} {}
                            iterator(const iterator &it)        // CC
                             { copy(*this, it); }
                            iterator(iterator &&it) noexcept    // MC
                             { swap(*this, it); }
        iterator &          operator=(iterator it) noexcept     // CA, MA
                             { swap(*this, it); return *this; }

                            // adapters to Jnode::iterator
                            operator Jnode::iterator (void) const {
                             auto it = pv_.empty()?
                                        json().root().children_().begin(): pv_.back().jit;
                             return Jnode::iterator{std::move(it), sn_.type_};
                            }
                            operator Jnode::const_iterator(void) const {
                             auto it = pv_.empty()?
                                        json().root().children_().begin(): pv_.back().jit;
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
        Jnode &             operator*(void);
        Jnode *             operator->(void)
                             { return & operator*(); }
        iterator &          operator++(void)
                             { incremented(); return *this; }
        bool                incremented(void);
        bool                incremented_(void);
        iterator &          begin(void)
                             { return *this; }
        iterator            end(void) const
                             { iterator it; it.pv_.emplace_back(json().end_(), true); return it; }
        bool                is_valid(void) const {
                             // end iterator is considered invalid path
                             if(jp_ == nullptr) return false;       // iterator un-initialized
                             if(pv_.empty()) return true;           // root is always a valid itr
                             if(pv_.back().jit == json().end_()) return false;  // end is invalid
                             return is_valid_(json().root(), 0);
                            }
        bool                is_nested(iterator & it) const;

        // path vector methods:
        const path_vector & path(void) const { return pv_; }    // path to the element
        const Itr &         operator[](size_t idx) const { return pv_.at(idx); }
        size_t              path_size(void) const { return pv_.size(); }

        // walk steps methods:
        const std::vector<WalkStep> &
                            walks(void) const { return ws_; }
        size_t              walk_uid(void) const { return wuid_; }
        size_t              walk_size(void) const { return ws_.size(); }
        bool                is_koj_last(Json::KeyOfJson x = Json::EmptyKoj) const
                             // x==EmptyKoj check if <>k was last executed; otherwise if <..>k was
                             { return
                                lwsi_ < ws_.size() and
                                not ws_[lwsi_].is_locked() and
                                ws_[lwsi_].jsearch == Jsearch::key_of_json and
                                ((x == Json::NonEmptyKoj) ^ ws_[lwsi_].stripped.front().empty()); }
        signed_size_t       counter(size_t position) const {
                             // returns iterable lexeme's current offset/ounter in the position
                             if(position >= ws_.size())
                              throw json().EXP(Jnode::ThrowReason::walk_bad_position);
                             return ws_[position].type == WalkStep::WsType::Range_walk?
                                    ws_[position].load_offset(json()): -1;
                            }
        signed_size_t       instance(size_t position) const {
                             // returns static lexeme's offset in position (-1 otherwise)
                             if(position >= ws_.size())
                              throw json().EXP(Jnode::ThrowReason::walk_bad_position);
                             return ws_[position].type == WalkStep::WsType::Static_select?
                                    ws_[position].load_offset(json()): -1;
                            }
        signed_size_t       offset(size_t position) const {
                             // returns (static/iterable) lexeme's offset (counter) in position
                             if(position >= ws_.size())
                              throw json().EXP(Jnode::ThrowReason::walk_bad_position);
                             return ws_[position].load_offset(json());
                            }
        const std::string & lexeme(size_t position) const {
                             // returns reference to the lexeme (non-stripped)
                             if(position >= ws_.size())
                              throw json().EXP(Jnode::ThrowReason::walk_bad_position);
                             return ws_[position].lexeme;
                            }
        Jsearch             type(size_t position) const {
                             // returns Jsearch type of the lexeme (regular_match, etc)
                             if(position >= ws_.size())
                              throw json().EXP(Jnode::ThrowReason::walk_bad_position);
                             return ws_[position].jsearch;
                            }
        bool                is_walkable(void)
                             // checks if the walk-path is *iterable*
                             { return next_iterable_ws_(walk_path_().size()) >= 0; }
        Json &              json(void) const { return *jp_; }


     protected:
                            iterator(Json *ptr):                // private custom C, used by walk()
                             jp_{ptr}, wuid_{Json::iterator::uuid_++} {};

        size_t              lwsi_{SIZE_T(-1)};                  // last walked ws index
      std::vector<WalkStep> ws_;                                // walk state vector (walk path)
        path_vector         pv_;                                // path_vector (result of walking)
        Json *              jp_{nullptr};                       // json pointer (for json().end())
        unsigned short      wuid_;                              // unique walk-id
        SuperJnode          sn_{Jnode::Jtype::Neither};         // supernode, type_ holds parent's

        static std::string  dummy_lbl_;                         // empty (default) label
      static unsigned short uuid_;                              // universal uid


     private:
        #define SEARCH_TYPE     /* lexeme type: <..> vs >..< */\
                Recursive, \
                Non_recursive
        ENUMSTR(SearchType, SEARCH_TYPE)

        #define ORGSEMANTIC     /* straight/reverse  semantic for is_original_ method */\
                Straight, \
                Reverse
        ENUM(OriginalSrch, ORGSEMANTIC)

        bool                is_valid_(Jnode & jnp, size_t idx) const;
    std::vector<WalkStep> & walk_path_(void) { return ws_; }
        const std::vector<WalkStep> &
                            walk_path_(void) const { return ws_; }
        const WalkStep &    last_walked_(void) const            // predicate this call by checking
                             { return ws_[lwsi_]; }             // if lwsi_ is valid within ws_
        void                end_path_(void)
                             { pv_.emplace_back(json().end_(), true); }
        auto &              sn_type_ref_(void) { return sn_.type_; }    // original container type

        signed_size_t       walk_(void);
        void                walk_step_(size_t wsi, Jnode *);
        void                process_directive_(size_t wsi, Jnode *jn);
        void                step_walk_(size_t wsi);
        void                process_directive_I_(size_t wsi, Jnode *jn);
        void                show_built_pv_(std::ostream &out) const;
        void                walk_numeric_offset_(size_t wsi, Jnode *);
        Jnode::iter_jn      build_cache_(Jnode *jn, size_t wsi);
        void                walk_text_offset_(size_t wsi, Jnode *);
        void                walk_search_(size_t wsi, Jnode *);
        void                walk_search_cached_(size_t wsi, Jnode *, size_t offset);
        size_t              normalize_(signed_size_t offset, Jnode *jn) const {
                             if(jn->is_atomic()) return offset;     // norm. by [jn->chldrn]
                             signed_size_t children = SGNS_T(jn->children_().size());
                             if(offset >= 0) return offset > children? children: offset;
                             return children + offset < 0? 0: children + offset;
                            }
        signed_size_t       re_normalize_(signed_size_t offset, Jnode *jn) const {  // relative
                             signed_size_t children = SGNS_T(jn->children_().size());
                             return offset > children? children:
                                    -offset > children? -children: offset;
                            }
        void                research_(Jnode *jn, size_t wsi,
                                      std::vector<Json::SearchCacheEntry> *, CacheKey *);
        bool                re_search_(Jnode *jn, WalkStep &, const char *lbl,
                                       signed_size_t &instance, signed_size_t cdf, SearchType st,
                                       std::vector<Json::SearchCacheEntry> *);
        bool                cache_entry_(signed_size_t &instance, signed_size_t cdf,
                                         std::vector<Json::SearchCacheEntry> *);
        bool                match_iterable_(Jnode *jn, const char *lbl, WalkStep &);
        bool                atomic_match_(const Jnode *jn, const char *lbl,
                                          WalkStep &ws, map_jne * ns);
        bool                string_match_(const Jnode *, const WalkStep &, map_jne *nsp) const;
        bool                regex_match_(const std::string &, const WalkStep &, map_jne *) const;
        bool                label_match_(Jnode::map_jn::iterator jit,
                                         const Jnode *jn, signed_size_t idx,
                                         WalkStep & ws, map_jne * nsp) const;
        bool                bull_match_(const Jnode *jn, const WalkStep &) const;
        bool                json_match_(const Jnode *jn, const WalkStep &);
        bool                is_original_(const Jnode &jn, const WalkStep &ws,
                                         OriginalSrch reverse = Straight) {
                             // facilitate query_original (reverse = false),
                             // and Query_duplicate (reverse = true) lexemes
                             return json().sortc_.emplace(ws.wsuid, SortCacheEntry{})
                                           .first->VALUE.sortset.emplace(&jn).second xor reverse;
                            }
        size_t              gsort_matches_(const Jnode * jn, WalkStep &ws, size_t offset = -1);
        void                purge_ns_(const std::string & pfx);
        void                maybe_nsave_(WalkStep & ws, const Jnode *jn);

        // callbacks interface
        void                lbl_callback_(const char *lbl,
                                          const std::vector<Json::SearchCacheEntry> * = nullptr);
        void                wlk_callback_(const Jnode *);
        void                dmx_callback_(const Jnode *, const char *lbl,
                                          const std::vector<Json::SearchCacheEntry> * = nullptr);

        // facilitate actual walking (i.e. iterations for walks)
        signed_size_t       increment_(signed_size_t wsi);
        signed_size_t       next_iterable_ws_(signed_size_t wsi);

        // facilitate <>f and <>F lexemes
        void                lock_fs_domain_(size_t wsi) {       // lock FS domain till end of FI
                             for(; wsi < walk_path_().size(); ++wsi) { // traversing forward
                              auto & ws = walk_path_()[wsi];
                              ws.locked = true;
                              if(ws.jsearch == Jsearch::Forward_itr) break;
                             }
                            }
        void                unlock_fs_domain_(signed_size_t wsi) {  // unlock all FS domains
                             for(++wsi; wsi < SGNS_T(walk_path_().size()); ++wsi)
                              walk_path_()[wsi].locked = false;
                            }
        signed_size_t       failed_stop_(signed_size_t wsi);
    };
    //
    // end of walk Iterator's definition


    // interpolation routines: Stringover is defined in above private scope
    static Json         interpolate(Stringover templ, iterator & jit,
                                    map_jne &name_space = dummy_ns_,
                                    ParseTrailing = Json::ParseTrailing::Strict_trailing);

 private:
    #define INTPIDX  /* define index semantics for bitset using in interpolation */ \
                Normal_pass,            /* first pass interpolation */ \
                Stringify_atomics,      /* try stringifying atomics in iterables recursively */ \
                Attempt_as_array,       /* enable interpolating objects as arrays */ \
                Obj_attempted,          /* indicate if object was processed */ \
                Is_double_brace_notaion,/* indicate type of interpolation {{}} vs {} */ \
                Interpolate_labels,     /* interpolate labels instead of values in `"{}"` */ \
                Size
    ENUM(IntpBit, INTPIDX)

    typedef std::vector<std::pair<size_t, size_t>> vec_range;

    static void         generate_auto_tokens_(Stringover &tmp,
                                              Json::iterator &, map_jne &ns);
    static void         interpolate_path_(Stringover &tmp, Json::iterator &, map_jne &ns);
    static std::string  interpolate_tmp_(std::string templ, Json::iterator &, map_jne &ns,
                                         std::bitset<IntpBit::Size> &prty);
    static void         interpolate_tmp__(std::string &tmp, Json::iterator &,
                                          map_jne &ns, std::string &&skey, Jnode &nse,
                                          std::bitset<IntpBit::Size> &prty,
                                          vec_range &irng, std::string &u8id);
    static std::string  replace_utf8_ill_(std::string & str, const map_jne &ns);
    static std::string  get_delimiter_(const map_jne &ns, const char *dlm, const char *spr) {
                         // if dlm is not found in the namespace return spr, or found value
                         auto found = ns.find(dlm);
                         if(found == ns.end() or found->VALUE.ref().is_neither())
                          return {spr};
                         return found->VALUE.ref().str();
                        }
    #define GET_DLM_(T, NS) /* instantiate delimiters and separators like ITRP_ADLM, ITRP_ASPR */\
        get_delimiter_(NS, ITRP_ ## T ## DLM, ITRP_ ## T ## SPR)

    static std::string  interpolate_jsn_(std::string templ, const Json &);


    // data structures (cache storage and callbacks)
    // cache:
    //      key: made of { WalkStep, *Jnode };
    //      value: vector<SearchCacheEntry: { namespace (map_jn); vector<path_vector> }>;
    // - namespace is required to be a part of the cache to support REGEX values only!
    SearchCache         srchc_{CacheKey::cmp};                  // search lexemes cache itself
                        // search cache is the array of all path_vector's for given
                        // search key (combination of jnode and walk step)

    SortCache           sortc_;                                 // mappings for g/G lexemes
    //
    // sortc_:
    //  map: unsigned short wsuid -> SortCacheEntry
    //       0 -> SortCacheEntry:{ sortset, offsetmap }
    //       1 -> SortCacheEntry:{ set<Gremap>:{ const Jnode *jnp, size_t order} sortset,
    //                             vector<siz_t> offsetmap }
    //
    // - design/logic for <>g, <>G lexemes:
    // there are 2 types of calls for the routine facilitating lexeme (gsort_matches_()):
    // 1. when called while building cache:
    //    o upon entering cache building the entire cache to be built, regardless of quantifier
    //      - in research_() set i to LONG_MAX-1 (to ensure all matches)
    //    o during matches always return true (to ensure all matches go into cache)
    // 2. Once cache is build call gsort_matches_() to remap current 'offset' onto sorted one
    //    o to distinguish calls from cache building phase and remapping, the argument
    //      'offset' to be sent with '-1' value in the former case
    //
    // To facilitate ascending/descending order of entries, 'Gremap' structure to be provided:
    //    o it'll keep jnode pointer and ordinal number (order it came)
    //    o however, each Gremap structure is stored in std::set container, which will sort
    //      entries by {jnode, order it came}, thus each ordinal entry in std::set will refer
    //      to the sorted entry (in the ascending order) in the cahe.

    struct WlkCallback {
     // this class facilitates wlk_callback_vec type storage, binding Json::iterator
     // and callback itself together
     // There are 3 types of callbacks (this one facilitates 2nd):
     // 1. callback hooked onto the JSON labels (lbl_callback_map)
     // 2. callback hooked onto Json::iterators (wlk_callback_vec)
     // 3. callback for user lexeme <..>i (uws_callback_vec)
                            WlkCallback(iterator &&itr, std::function<void(const Jnode &)> &&cb):
                             itr{std::move(itr)}, callback(std::move(cb)) {}
        iterator            itr;
        std::function<void(const Jnode &)>
                            callback;
    };


    typedef std::map<std::string, std::function<void(const Jnode &)>> lbl_callback_map;
    typedef std::vector<WlkCallback> wlk_callback_vec;
    typedef std::function<bool(const std::string &lxm, Json::iterator &jit)> uws_callback;
    typedef std::vector<uws_callback> uws_callback_vec;

    lbl_callback_map    lcb_;                                   // label callbacks storage
    wlk_callback_vec    wcb_;                                   // walk based callback storage
    uws_callback_vec    ucb_;                                   // WS '<..>u#' callbacks
    bool                ce_{false};                             // callbacks engaged? flag


 public:

    #define CALLBACK_TYPE \
                Label_callback, \
                Walk_callback, \
                Lexeme_callback, \
                Any_callback
    ENUM(CbType, CALLBACK_TYPE)

    // user callbacks interface:
    bool                is_engaged(CbType ct = Any_callback) const {
                         // check is any or a specific callback is engaged
                         if(ct == CbType::Label_callback) return ce_ and not lcb_.empty();
                         if(ct == CbType::Walk_callback) return ce_ and not wcb_.empty();
                         if(ct == CbType::Lexeme_callback) return ce_ and not ucb_.empty();
                         return ce_;
                        }
    Json &              engage_callbacks(bool x = true)         // engage/disengage all callbacks
                         { ce_ = x; return *this; }
    Json &              clear_callbacks(CbType ct = Any_callback) {
                         if(ct == CbType::Any_callback)
                          { lcb_.clear(); wcb_.clear(); ucb_.clear(); return *this; }
                         if(ct == CbType::Label_callback) { lcb_.clear(); return *this; }
                         if(ct == CbType::Walk_callback) { wcb_.clear(); return *this; }
                         if(ct == CbType::Lexeme_callback) ucb_.clear();
                         return *this;
                        }

    Json &              callback(const std::string &lbl,        // plug-in label-callback
                                 std::function<void(const Jnode &)> &&cb) {
                         lcb_.emplace(std::move(lbl), std::move(cb));
                         return *this;
                        }
    Json &              callback(iterator itr,                  // plug-in itr-callback (from walk)
                                 std::function<void(const Jnode &)> &&cb) {
                         if(itr != itr.end())                   // don't plug ended iterators
                          wcb_.emplace_back(std::move(itr), std::move(cb));
                         return *this;
                        }
    Json &              callback(uws_callback &&cb) {           // plug-in callback for u-lexeme
                          ucb_.emplace_back(std::move(cb));
                         return *this;
                        }

    lbl_callback_map &  lbl_callbacks(void) { return lcb_; }    // access to labeled callbacks
const lbl_callback_map& lbl_callbacks(void) const { return lcb_; }
    wlk_callback_vec &  wlk_callbacks(void) { return wcb_; }    // access to iterator callbacks
const wlk_callback_vec& wlk_callbacks(void) const { return wcb_; }
    uws_callback_vec &  uws_callbacks(void) { return ucb_; }    // access to iterator callbacks
const uws_callback_vec& uws_callbacks(void) const { return ucb_; }

};                                                              // end of Json class


// Json class static definitions
Json::map_jne Json::dummy_ns_;                                  // facilitate default arg
std::map<std::string, std::vector<Json::WalkStep>> Json::compiled_walks_;   // compiled walks cache

STRINGIFY(Json::Jsearch, JS_ENUM)
STRINGIFY(Json::ParseTrailing, PARSETRAILING)
STRINGIFY(Json::WalkStep::WsType, WALKSTEPTYPE)
STRINGIFY(Json::iterator::SearchType, SEARCH_TYPE)

#undef JS_ENUM
#undef CACHE_STATE
#undef KOJ_ARG
#undef PARSETRAILING
#undef WALKSTEPTYPE
#undef SEARCH_TYPE
#undef ORGSEMANTIC
#undef CALLBACK_TYPE
#undef PARSE_THROW
#undef SIGN_VALIDATION
#undef INTPIDX


// THROW_EXP is a polymorphic macro wrapper to update exception pointer exp_ with current pointer
// jsp and throw if allowed or return value (RV) if throwing is not allowed, used only in parsing:
//   THROW_EXP(exception)           // return void if not throwing
//   THROW_EXP(exception, rv)       // return rv if not throwing
#define __THROW_EXP_1_ARG__(E) \
    { exp_ = jsp; if(parse_throw_ == ParseThrow::May_throw) throw EXP(E); exr_ = E; return; }
#define __THROW_EXP_2_ARG__(E, RV) \
    { exp_ = jsp; if(parse_throw_ == ParseThrow::May_throw) throw EXP(E); exr_ = E; return RV; }
#define __THROW_EXP_3RD_ARG__(ARG1, ARG2, ARG3, ...) ARG3
#define __THROW_EXP_CHOOSER__(ARGS...) \
    __THROW_EXP_3RD_ARG__(ARGS, __THROW_EXP_2_ARG__, __THROW_EXP_1_ARG__)
#define THROW_EXP(ARGS...) \
    __THROW_EXP_CHOOSER__(ARGS)(ARGS)


// MAY_RETURN_ON_FAILURE macro wrapper handles code which might throw when throwing is disabled
#define MAY_RETURN_ON_FAILURE(CODE) \
    CODE; \
    if(not parse_throwing() and parsing_failed()) \
     { node.type(Jnode::Jtype::Neither); return; }





//
// Jnode class methods requiring Json definition:
//
Jnode::Jnode(Json j) {                                          // type conversion Json -> Jnode
 swap(*this, j.root());
}


//
// Json class definitions:
//
Json operator ""_json(const char *c_str, std::size_t) {
 // raw string parsing
 Json x;
 return x.parse(std::string{c_str});
}



Json & Json::parse(Streamstr::const_iterator & jsp, ParseTrailing trail) {
 // parse input string. this is a wrapper for parse_(), where actual parsing occurs
 exp_ = Streamstr::const_iterator{};                            // reset exception pointer
 exr_ = Jnode::ThrowReason::start_of_json_parsing_exceptions;   // reset exception reason
 root() = OBJ{};                                                // most probable JSON root
 parse_(root(), jsp);
 // when parse_throwing is disabled, it also affects (disables throwing) the strict trailing modes,
 // however, the exception_reason is still set to `unexpected_trailing`
 // root node is invalidated (set to Neither) only if parsing failed due to other than
 // unexpected_trailing (regardless parse_throwing mode)
 if(not parse_throwing() and parsing_failed())                  // throwing disabled and failed
  { type(Jnode::Jtype::Neither); return *this; }

 if(is_neither())                                               // parsing could not classify node
  THROW_EXP(Jnode::ThrowReason::expected_json_value, *this)

 DBG(1) DOUT() << "finished parsing json" << std::endl;         // at this point JSON is valid
 if(trail == ParseTrailing::Relaxed_no_trail)                   // in this mode only, exp_ points
  { exp_ = jsp; return *this; }                                 // to the end of JSON

 if(trail == ParseTrailing::Strict_no_trail)
  if(jsp != jsp.streamstr().end())
   THROW_EXP(Jnode::ThrowReason::unexpected_trailing, *this);

 // Relaxed_trailing or Strict_trailing
 for(; jsp != jsp.streamstr().end(); ++jsp) {
  if(jsp.is_front_chr()) break;
  if(*jsp > 0 and *jsp <= CHR_SPCE) continue;
  if(trail == ParseTrailing::Strict_trailing)
   THROW_EXP(Jnode::ThrowReason::unexpected_trailing, *this)
  break;                                                        // trail == Relaxed_trailing
 }

 return *this;
}



void Json::parse_(Jnode & node, Streamstr::const_iterator &jsp) {
 // recursively parse JSON from string (unicode UTF-8 compliant)
 MAY_RETURN_ON_FAILURE( skip_blanks_(jsp) )
 node.type( classify_jnode_(jsp) );

 #ifndef NDBG_PARSER
 DBG(4) {                                                       // print currently parsed point
  if(not jsp.is_streamed())
   DOUT() << Debug::btw << "parsing point ->" << jsp.str(Debug::term_width() * 4) << std::endl;
  else                                                         // streamed input
   DOUT() << Debug::ftw << jsp.str(Debug::term_width() * 4) << "<- parsing point" << std::endl;

  if(DBG()(5))
   if(not node.is_neither())
    DOUT() << DBG_PROMPT(5)
           << "parsing as " << ENUMS(Jnode::Jtype, node.type()) << std::endl;
 }
 #endif

 switch(node.type()) {
  case Jnode::Jtype::Object: parse_object_(node, ++jsp); break; // skip '{' with ++jsp
  case Jnode::Jtype::Array: parse_array_(node, ++jsp); break;   // skip '[' with ++jsp
  case Jnode::Jtype::String: parse_string_(node, ++jsp); break; // skip '"' with ++jsp
  case Jnode::Jtype::Number: parse_number_(node, jsp); break;
  case Jnode::Jtype::Bool: parse_bool_(node, jsp); break;
  case Jnode::Jtype::Null: advance(jsp, 4); break;              // leave node blank, skip "null"
  default: break;                                               // covering warning of the compiler
 }
}



void Json::parse_bool_(Jnode & node, Streamstr::const_iterator &jsp) {
 // Parse first character of lexeme ([tT] or [fF])
 node.value_ = *jsp;                                            // i.e. store either 't' or 'f'
 if(node.value_.front() == CHR_FALSE) ++jsp;                    // false is 1 char bigger than true
 advance(jsp, 4);
}



void Json::parse_string_(Jnode & node, Streamstr::const_iterator &jsp) {
 // parse string value - from `"` till `"'
 auto sp = jsp;                                                 // copy, for a work-around
 MAY_RETURN_ON_FAILURE( auto ep = find_delimiter_(JSN_STRQ, jsp) )
 node.value_ = readup_str_(sp, distance(sp, ep));
 ++jsp;
}



void Json::parse_number_(Jnode & node, Streamstr::const_iterator &jsp) {
 // parse number, as per JSON number definition
 auto sp = jsp;                                                 // copy, for a work-around
 MAY_RETURN_ON_FAILURE( auto ep = validate_number_(jsp) )
 node.value_ = readup_str_(sp, distance(sp, ep));
}



void Json::parse_array_(Jnode & node, Streamstr::const_iterator &jsp) {
 // parse elements of JSON Array (recursively)
 for(bool comma_read = false; true;) {
  Jnode child;
  MAY_RETURN_ON_FAILURE( parse_(child, jsp) )

  if(child.is_neither()) {
   if(*jsp == JSN_ARY_CLS) {
    if(node.is_empty()) { ++jsp; return; }                      // empty array: [ ]
    if(not comma_read) { ++jsp; return; }                       // end of array: ..., "last" ]
   }
   if(*jsp == JSN_ASPR)                                         // == ','
    if(not comma_read and node.has_children())
     { ++jsp; comma_read = true; continue; }                    // interleaving comma: .. 2, 3, ..
   // here: either a double comma: " ... ,,", or leading comma: "[ , ...
   if(not comma_read and node.has_children())
    THROW_EXP(Jnode::ThrowReason::expected_value_separator)     // [ *val* *anything but coma*
   THROW_EXP(Jnode::ThrowReason::expected_json_value)           // e.g.: "[ , ...", or "[ 123,, ]"
  }
  if(not comma_read and node.has_children())                    // e.g.: [ "abc" 3.14 ]
   THROW_EXP(Jnode::ThrowReason::missed_prior_enumeration)

  node.children_().emplace(node.next_key_(), std::move(child));
  comma_read = false;
 }
}



void Json::parse_object_(Jnode & node, Streamstr::const_iterator &jsp) {
 // parse elements of JSON Object (recursively)
 std::set<std::string> c2a;                                     // converted to array (-m)

 for(bool comma_read = false; true;) {
  MAY_RETURN_ON_FAILURE( skip_blanks_(jsp) )
  auto lsp = jsp;                                               // label's begin pointer

  Jnode label;
  MAY_RETURN_ON_FAILURE( parse_(label, jsp) )                   // must be a string (label)

  if(not label.is_string()) {
   if(label.is_neither()) {                                     // parsing if label failed
    if(*jsp == JSN_OBJ_CLS) {                                   // == '}'
     if(node.is_empty()) { ++jsp; return; }                     // empty object: { }
     if(not comma_read) { ++jsp; return; }                      // end of object: ..."last" }
    }
    if(*jsp == JSN_ASPR)                                        // == ','
     if(not comma_read and node.has_children())
      { ++jsp; comma_read = true; continue; }                   // interleaving comma
   }                                                            // else label is not a string
   exr_ = Jnode::ThrowReason::expected_valid_label;
   MAY_RETURN_ON_FAILURE(exp_ = lsp)
   if(not comma_read and node.has_children())
    THROW_EXP(Jnode::ThrowReason::expected_value_separator)     // { *val* *anything but coma*
   throw EXP(Jnode::ThrowReason::expected_valid_label);
  }

  MAY_RETURN_ON_FAILURE( char lblspr = skip_blanks_(jsp) )      // label just hass been parsed
  if(lblspr != LBL_SPR)                                         // label was read, expecting ':'
   THROW_EXP(Jnode::ThrowReason::expecting_label_separator)

  Jnode child;
  MAY_RETURN_ON_FAILURE( parse_(child, ++jsp) )
  if(child.is_neither())                                        // after 'label:' there must follow
   THROW_EXP(Jnode::ThrowReason::expected_json_value)           // a valid JSON value

  if(not comma_read and node.has_children())                    // e.g.: [ "abc" 3.14 ]
   THROW_EXP(Jnode::ThrowReason::missed_prior_enumeration)

  if(node.children_().find(label.str()) == node.children_().end())
   node.children_().emplace(std::move(label.str()), std::move(child));
  else
   if(is_merging())
    merge_(c2a, node, std::move(label), std::move(child));
  comma_read = false;
 }
}



void Json::merge_(std::set<std::string> &c2a, Jnode &node, Jnode &&label, Jnode &&child) {
 // merge a child into existing label - that function is outside of JSON's specification
 // however, it allows merging clashing labels of ill formed JSONs
 auto & nl = node[label.str()];
 if(c2a.count(label) == 0) {                                    // if not converted yet
  nl = ARY{ std::move(nl) };                                    // convert to array
  c2a.emplace(label);                                           // indicate label was converted
 }
 nl.push_back(std::move(child));
}



Streamstr::const_iterator & Json::find_delimiter_(char c, Streamstr::const_iterator & jsp) {
 // find next occurrence of character (actually it's used only to find closing `"')
 int cp{0};                                                     // code point
 auto validate_hex = [&](char c) {
       if(c >= '0' and c <= '9') return;
       if(c >= 'A' and c <= 'F') return;
       if(c >= 'a' and c <= 'f') return;
       THROW_EXP(Jnode::ThrowReason::invalid_code_point)
      };

 while(*jsp != c) {
  if(cp > 0)
   { cp--; validate_hex(*jsp); ++jsp; continue; }
  if(*jsp AMONG(CHR_NULL, CHR_EOL, CHR_RTRN))                   // JSON string does not support
   THROW_EXP(Jnode::ThrowReason::unexpected_end_of_line, jsp)   // multiline, hence throwing
  if(strchr(jsn_fbdn_, *jsp) != nullptr)                        // i.e. found illegal JSON control
   THROW_EXP(Jnode::ThrowReason::unquoted_character, jsp)

  if(*jsp == CHR_QUOT) {
   ++jsp;                                                       // skip presumably quoted char
   if(jsp == jsp.streamstr().end())                             // found end of string after '\'
    THROW_EXP(Jnode::ThrowReason::unexpected_end_of_line, jsp)
   if(strchr(jsn_qtd_, *jsp) == nullptr)                        // it's not JSON char quotation
    THROW_EXP(Jnode::ThrowReason::unexpected_character_escape, jsp)
   if(*jsp == 'u') cp = 4;                                      // \u code point found, 4 hex'es
  }
  ++jsp;
 }
 if(cp != 0 )
  THROW_EXP(Jnode::ThrowReason::invalid_code_point, jsp)
 return jsp;
}



Streamstr::const_iterator & Json::validate_number_(Streamstr::const_iterator & jsp) {
 // wrapper for static json_number_definition()
 if(json_number_definition(jsp) != Jnode::Jtype::Number)        // failed to convert
  THROW_EXP(Jnode::ThrowReason::invalid_number, jsp)
 return jsp;
}



Jnode::Jtype Json::json_number_definition(std::string::const_iterator & jsp) {
 // wrapper for json_number_definition using string iterator
 Streamstr ss;
 ss.DBG().severity(NDBG);                                       // disable debugs in ss
 ss.source_buffer(std::string{&*jsp});
 auto it = ss.begin();
 auto rval =  Json::json_number_definition(it);
 jsp += std::distance(ss.begin(), it);
 return rval;
}



Jnode::Jtype Json::json_number_definition(Streamstr::const_iterator & jsp) {
 // conform JSON's definition of a number (https://json.org/index.html)
 if(*jsp == JSN_NUMM) ++jsp;                                    // == '-'
 if(not isdigit(*jsp)) return Jnode::Jtype::Neither;            // digit must follow '-' sign
 if(*jsp > '0')
  while(isdigit(*jsp)) ++jsp;
 else                                                           // next could be only [.eE] or end
  ++jsp;                                                        // skip leading 0
 // here it could be either of [.eE] or end
 if(*jsp == JSN_NUMD) {                                         // == '.'
  if(not isdigit(*++jsp)) return Jnode::Jtype::Neither;         // digit must follow '.'
  while(isdigit(*jsp)) ++jsp;
 }
 // here could be [eE] or end
 if(*jsp == 'e' or *jsp == 'E') {
  ++jsp;                                                        // skip [eE]
  if(*jsp == JSN_NUMP or *jsp == JSN_NUMM) ++jsp;               // skip [+-]
  if(not isdigit(*jsp)) return Jnode::Jtype::Neither;           // digit must follow [eE][+/]
  while(isdigit(*jsp)) ++jsp;
 }
 return Jnode::Jtype::Number;
}



Jnode::Jtype Json::classify_jnode_(Streamstr::const_iterator & jsp) {
 // classify returns either of the Jtypes, or Neither
 // it does not move the pointer
 if(*jsp == JSN_OBJ_OPN) return Jnode::Jtype::Object;
 if(*jsp == JSN_ARY_OPN) return Jnode::Jtype::Array;
 if(*jsp == JSN_STRQ) return Jnode::Jtype::String;
 if(isdigit(*jsp)) return Jnode::Jtype::Number;
 if(*jsp == JSN_NUMM) {                                         // starts with -, possibly a number
  std::string str = readup_str_(jsp, 2);
  return str.size() == 2 and isdigit(str.back())? Jnode::Jtype::Number: Jnode::Jtype::Neither;
 }

 #define JTERM \
            JT_true, \
            JT_false, \
            JT_null
 ENUM(Jterm, JTERM)
 #undef JTERM
 static const std::string jterm[] = {"true", "false", "null"};

 size_t sjt;                                                    // selected jterm
 switch(*jsp) {
  case 't': sjt = Jterm::JT_true; break;
  case 'f': sjt = Jterm::JT_false; break;
  case 'n': sjt = Jterm::JT_null; break;
  default: return Jnode::Jtype::Neither;
 }

 for(size_t i = 1; i < jterm[sjt].size(); ++i) {                // fully validate jterm
  std::string str = readup_str_(jsp, i + 1);
  if(str.size() != i + 1 or jterm[sjt][i] != str[i])
   return Jnode::Jtype::Neither;
 }

 return sjt == JT_null? Jnode::Jtype::Null: Jnode::Jtype::Bool;
}



char Json::skip_blanks_(Streamstr::const_iterator & jsp) {
 // skip_blanks_() sets pointer to the first a non-blank character
 if(jsp == jsp.streamstr().end())
  THROW_EXP(Jnode::ThrowReason::unexpected_end_of_string, *jsp)

 while(*jsp >= 0 and *jsp <= CHR_SPCE) {                        // '*jsp >= 0' to support UTF-8
  ++jsp;
  if(jsp == jsp.streamstr().end())
   THROW_EXP(Jnode::ThrowReason::unexpected_end_of_string, *jsp)
 }
 return *jsp;
}



std::string Json::readup_str_(Streamstr::const_iterator & jsp, size_t n) {
 // read at most next n chars and return as a string, rewind stream iterator to initial position
 // in buffer mode readup should not go beyond current file boundary
 std::string str;
 for(size_t i = 0; i < n; ++i) {
  str += *jsp;
  if(quotedsolidus_ == false and
     str.size() >= 2 and str[str.size() - 2] == CHR_QUOT and str.back() == CHR_SLDS)
   { str.pop_back(); str.pop_back(); str += CHR_SLDS; }         // replace '\/' with '/'
  if(jsp.is_back_chr())                                         // do not advance to next file,
   { advance(jsp, -SGNS_T(i)); return str; }                    // return string up to end of file
  else
   if(++jsp == jsp.streamstr().end())                           // there's no next file, it's a
    { advance(jsp, -SGNS_T(i) - 1); return str; }               // global end, return string
 }
 advance(jsp, -SGNS_T(n));
 return str;
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
//      "<Elm Street>a+0 [-2] [Phones] [:] [Mobile]"
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

std::string Json::iterator::dummy_lbl_;                         // used to indicate empty label
unsigned short Json::iterator::uuid_{0};                        // facilitates unique walk-id: wuid


Json::iterator Json::walk(const std::string & wstr, CacheState action) {
 #include "dbgflow.hpp"
 // Json iterator factory: returns Json::iterator to the first element pointed
 //                        by the walk string, otherwise end-iterator returned
 DBG(0) DOUT() << "walk string: '" << wstr << "'" << std::endl;

 iterator it{this}, &itr = it;                                  // itr - for return by reference
 if(wstr.empty()) return itr;                                   // empty walk() optimization

 compile_walk(wstr, it);
 it.pv_.reserve(it.walk_path_().size());                        // iter's performance optimization

 if(action == CacheState::Invalidate) {                         // talking a conservative approach:
  clear_cache();                                                // user must specify "keep" himself
  DBG(0) DOUT() << "invalidated search/sort caches" << std::endl;
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



void Json::compile_walk(const std::string & wstr, iterator & it) const {
 #include "dbgflow.hpp"
 // parse walk string and compile all parts for ws_;
 auto wsit = Json::compiled_walks_.find(wstr);
 if(wsit != Json::compiled_walks_.end()) {
  DBG(0) DOUT() << "found compiled walk for: '" << wstr << "' in cache" << std::endl;
  it.walk_path_() = wsit->VALUE;
  return;
 }

 parse_lexemes_(wstr, it);
 for(auto & walk_step: it.walk_path_())
  parse_subscript_type_(walk_step);                             // fix textual offset

 DBG(0) DOUT() << "dump of completed lexemes:" << std::endl;
 for(size_t i = 0; i < it.walk_path_().size(); ++i)
  DBG(0) DOUT() << '[' << i << "]: " << it.walk_path_()[i] << std::endl;

 Json::compiled_walks_[wstr] = it.walk_path_();
 DBG(0) DOUT() << "cached in compiled walk '" << wstr << "'" << std::endl;
}



std::string Json::unquote_str(const std::string & src) const {
 #include "dbgflow.hpp"
 // unquote JSON source string as per JSON quotation.
 // even though it looks static, it's best to keep it in-class, due to throwing mechanism
 std::stringstream ss;

 auto h2i = [](char c) { return c >= 'a'? c - 'a' + 10: c >= 'A'? c - 'A' + 10: c - '0'; };
 auto utf8out = [&](size_t cp, short n) {                       // output n utf-8 bytes
       if(n == 1) { ss << static_cast<char>(cp); return; }      // assert: cp <= 0x7F
       short s{5};
       for(auto rs: {18, 12, 6, 0}) {
        if(--s > n) continue;
        size_t m = s == n? ((1 << n) - 1) << (8 - n): 0x80;     // mask for the respective byte
        ss << static_cast<char>(m | (((m ^ 0xFF) >> 1) & (cp >> rs)));
       }
      };

 size_t start=0;
 size_t hs = 0;

 for(size_t end = src.find(CHR_QUOT, start);
            end != std::string::npos;
            end = src.find(CHR_QUOT, start)) {
  char qc = src[++end];                                         // quoted character
  if(end >= src.size())                                         // i.e. line ending "...\"
   throw EXP(Jnode::ThrowReason::unexpected_end_of_quotation);
  const char * ptr = strchr(jsn_qtd_, qc);                      // #define JSN_QTD "/\"\\bfnrtu"
  if(ptr == nullptr)                                            // i.e other (non-Json) char quoted
   throw EXP(Jnode::ThrowReason::unexpected_quotation);
  ss.write(src.data() + start, end - start - 1);
  if(qc == 'u') {                                               // process \u
   size_t cp{0};
   for(int i = 0; i < 4; ++i)                                   // calculate code-point
    { cp <<= 4; cp |= h2i(src[++end]); }
   if((cp & 0xFC00) == 0xD800) {                                // high surrogate discovered
    if(hs != 0)
     throw EXP(Jnode::ThrowReason::invalid_surrogate_code_pair);// high surrogate after high
    hs = 0x10000 | ((cp & 0x03FF) << 10); start = ++end;
    continue;
   }
   if((cp & 0xFC00) == 0xDC00) {                                // low surrogate discovered
    if(hs == 0)
     throw EXP(Jnode::ThrowReason::invalid_surrogate_code_pair);// low surrogate w/o high surrogate
    cp = hs | (cp & 0x03FF); hs = 0;
   }
   utf8out(cp, cp <= 0x7F? 1: cp <= 0x7FF? 2: cp <= 0xFFFF? 3: 4);
  }
  else {
   if(hs != 0)
    throw EXP(Jnode::ThrowReason::invalid_code_point);          // high surrogate w/o low surrogate
   ss << JSN_TRL[ptr - jsn_qtd_];                               // translate quotation
  }
  start = ++end;
 }
 if(hs != 0)
  throw EXP(Jnode::ThrowReason::invalid_surrogate_code_pair);   // high surrogate w/o low surrogate
 ss << src.c_str() + start;
 return ss.str();
}



std::string Json::inquote_str(const std::string & src) const {
 #include "dbgflow.hpp"
 // quote quotation marks and back slashes only, as src is expected to be a valid JSON string
 // because of unquote_str(), keeping this one in-class defined, though it's entirely static
 std::stringstream ss;
 size_t start=0;
 for(size_t end{ src.find_first_of("\"\\", start) };
            end != std::string::npos;
            end = src.find_first_of("\"\\", start)) {
  ss.write(src.data() + start, end - start);
  ss << CHR_QUOT << src[end];
  start = end + 1;
 }
 ss << src.c_str() + start;
 return ss.str();
}



void Json::parse_lexemes_(const std::string & wstr, iterator & it) const {
 #include "dbgflow.hpp"
 // parse full lexemes: offsets/search lexemes + possible suffixes in search lexemes
 auto & ws = it.walk_path_();
 vec_str req_label;                                             // would hold stripped [label]:

 for(auto si = wstr.begin(); si != wstr.end();) {               // si: input string iterator
  DBG(1) {
   DOUT() << "walked string: " << wstr << std::endl;
   DOUT() << DBG_PROMPT(1) << "parsing here: "
          << std::string(utf8_adjusted(0, wstr, si - wstr.begin()), '-') << ">|" << std::endl;
  }
  switch(*si) {
   case CHR_SPCE: ++si; continue;                               // space in between lexemes allowed
   case LXM_SUB_OPN:                                            // opening subscript: '['
         if(not req_label.empty())                              // only search [..]:<..> allowed
          throw EXP(Jnode::ThrowReason::walk_expect_search_label);  // after [label]:
         ws.emplace_back(extract_lexeme_(si, LXM_SUB_CLS), Jsearch::numeric_offset);
         break;
   case LXM_SCH_OPN:                                            // opening search fwd '<'
         ws.emplace_back(extract_lexeme_(si, LXM_SCH_CLS), Jsearch::regular_match);
         break;                                                 // search will have regular_match
   case LXM_SCH_CLS:                                            // opening immediate search '>'
         ws.emplace_back(extract_lexeme_(si, LXM_SCH_OPN), Jsearch::regular_match);
         break;
   default:                                                     // outside of lexeme there could be
         parse_suffix_(si, it, req_label);                      // only suffixes, quantifiers & ':'
         if(req_label.empty()) parse_quantifier_(si, it);       // if [label]: found, no quantifier
         continue;
  }
  // store a stripped offset lexeme here (after parsing one)
  ws.back().stripped.push_back(ws.back().lexeme.substr(1, ws.back().lexeme.size() - 2));
  if(not req_label.empty()) {                                   // [label]: was extracted
   ws.back().stripped.push_back( std::move(req_label.front()) );
   req_label.clear();
  }
 }
 if(not req_label.empty()) throw EXP(Jnode::ThrowReason::walk_expect_lexeme);
}



std::string Json::extract_lexeme_(std::string::const_iterator &si, char closing) const {
 #include "dbgflow.hpp"
 // offset lexemes: [], [n], [+n], [-n], [n:n] [any text, possibly with \]]
 // search lexemes w/o suffix: <>, <text<\>>
 auto begin = si;
 while(*si != closing) {                                        // advance till closing bracket
  if(*si == CHR_QUOT) ++si;                                     // si -> "\...
  if(*si == CHR_NULL)
   throw EXP(Jnode::ThrowReason::walk_lexeme_missing_closure);
  ++si;
 }

 std::string lexeme{begin, ++si};                               // lexeme with encasement
 std::string quoted{"\\"};                                      // replace quoted closing bracket
 quoted += closing;
 for(size_t p = lexeme.find(quoted); p != std::string::npos; p = lexeme.find(quoted, p))
  if(p + quoted.size() >= lexeme.size()) break;                 // replace only if it's not last
  else lexeme.replace(p, 2, 1, closing);
 DBG(1) DOUT() << "parsed lexeme: " << lexeme << std::endl;
 return lexeme;
}



void Json::parse_suffix_(std::string::const_iterator &si,
                         iterator & it, vec_str &req_label) const {
 #include "dbgflow.hpp"
 // parse suffix following a text offset, e.g.: ..>r, ..>d, ..>L and spacer ':' in [..]:<..>
 if(it.walk_path_().empty())                                    // expect some elements already in
  throw EXP(Jnode::ThrowReason::walk_expect_lexeme);            // walks may start with lexeme only

 auto & back_ws = it.walk_path_().back();
 // first check if si is not pointing to ':' (cases [..]:  or <..>: )
 if(*si == LBL_SPR) {                                           // separator (':') eg: in [..]:<..>
  if(not req_label.empty())                                     // label already extracted!
   throw EXP(Jnode::ThrowReason::walk_label_seprator_bad_usage);
  if(back_ws.jsearch == Jsearch::regular_match) return;         // it's a quantifier then (<>:...)
  req_label.push_back( std::move(back_ws.stripped.back()) );    // it's scoped lexeme: [..]:
  it.walk_path_().pop_back();                                   // label belongs to the next lexeme
  ++si;
  return;
 }
 // anything besides ':' following subscript [..] is not allowed
 if(back_ws.is_subscript())                                     // i.e. it's '['
  throw EXP(Jnode::ThrowReason::walk_unexpexted_suffix);        // suffix may follow searches only
 // it's search lexeme (<..>, >.,<, find / parse suffix and quantifiers)
 Jsearch sfx = search_suffix_(*si);                             // see if a search suffix valid
 if(sfx == Jsearch::end_of_lexemes) return;                     // default (<..>) - regular_match

 back_ws.jsearch = sfx;                                         // set suffix for given walkstep
 if(back_ws.is_json_capturing_lexeme() or
    sfx == Jsearch::json_match or sfx == Jsearch::Increment_num)// not capturing wakled Json, but
  parse_user_json_(back_ws);                                    // parsing <ns:user json>
 DBG(1) DOUT() << "search type sfx: " << ENUMS(Jsearch, sfx) << std::endl;

 if(back_ws.stripped.front().empty()) {                         // lexeme is empty, e.g.: <>r
  if(back_ws.is_lexeme_required())
   throw EXP(Jnode::ThrowReason::walk_empty_lexeme);
 }
 else                                                           // lexeme is non-empty (<..>)
  if(back_ws.must_lexeme_be_empty())
   throw EXP(Jnode::ThrowReason::walk_non_empty_lexeme);
 ++si;
}



void Json::parse_user_json_(WalkStep &ws) const {
 #include "dbgflow.hpp"
 // process options that require parsing json into ws.user_json_: <ns:user_json>, namely:
 // 1. all lexemes in "is_json_capturing_lexeme()", e.g. <ns:user_json>a, <ns:user_json>v, etc
 // 2. lexeme <user_json>j, here user_json might be a template
 // 3. also handle <ns:user_json>k - here user_json might be only a JSON string type
 const char * json_ptr{ ws.stripped.front().c_str() };          // case: ws.jsearch == json_match
 // find a beginning of user-json (i.e. skip 'ns:' part)
 if(ws.jsearch != Jsearch::json_match) {                        // lexeme is namespace (<ns:json>)
  size_t json_start = ws.stripped.front().find(RNG_SPR);        // lookging for ':'
  if(json_start == std::string::npos or json_start == 0) return;// no ':' then there's no user_json
  json_ptr = &ws.stripped.front().c_str()[json_start + 1];
  ws.stripped.front().erase(json_start);
 }
 // try parsing user_json
 try {
  Json j;
  j.DBG().severity(NDBG);
  ws.user_json = std::move(j.parse(json_ptr, ParseTrailing::Strict_no_trail).root());
 }
 catch(Json::stdException & e) {                                // parsing as JSON fails
  if(ws.jsearch != Jsearch::json_match)                         // i.e. if not <..>j
   try { Json j; ws.user_json = std::move(j.parse(std::string{"\""} + json_ptr + "\"",
                                                  ParseTrailing::Strict_no_trail).root()); }
   catch(Json::stdException & e)                                // and only if it fails - throw
    { throw EXP(Jnode::ThrowReason::json_lexeme_invalid); }
  return;                                                       // here <..>j is a template
 }

 // check for <..>k lexeme that user_json is a string type
 if(ws.jsearch == Jsearch::key_of_json and not ws.user_json.is_string()) // lexeme is <..>k (label)
  throw EXP(Jnode::ThrowReason::expected_string_type);          // but parsed json is not string
}



std::regex::flag_type Json::parse_RE_flags_(std::string & re_str) const {
 #include "dbgflow.hpp"
 // parse optional RE flags from RE expression. Flags are specified as trailing control chars
 // (which are illegal in Re),e.g.: '...\I\O\P>R' and removed from the RE expression
 std::regex::flag_type re_grammar{std::regex::ECMAScript};      // default regex grammar
 std::regex::flag_type flag{re_grammar};                        // default regex flag
 static const char * flgs[] = {"\\I", "\\N", "\\O", "\\C",                  // flags
                               "\\E", "\\S", "\\X", "\\A", "\\G", "\\P"};   // grammar selection
 #define FLEN (sizeof("\\I") - 1)                               // i.e. FLEN = 3 - 1 = 2

 auto set_grammar = [&flag, &re_grammar](std::regex::flag_type new_grm)
                     { flag ^= re_grammar; re_grammar = new_grm; flag |= re_grammar; };
 size_t rest_size;
 do {
  rest_size = re_str.size();
  for(auto &c_flg: flgs)
   if(re_str.size() >= (FLEN + 1) and
      re_str.compare(re_str.size() - FLEN, FLEN, c_flg, FLEN) == 0) {
    re_str.erase(re_str.size() - FLEN, FLEN);
    switch(c_flg[1]) {
     case 'I': flag |= std::regex::icase; break;
     case 'N': flag |= std::regex::nosubs; break;
     case 'O': flag |= std::regex::optimize; break;
     case 'C': flag |= std::regex::collate; break;
     case 'E': set_grammar(std::regex::ECMAScript); break;
     case 'S': set_grammar(std::regex::basic); break;
     case 'X': set_grammar(std::regex::extended); break;
     case 'A': set_grammar(std::regex::awk); break;
     case 'G': set_grammar(std::regex::grep); break;
     case 'P': set_grammar(std::regex::egrep); break;
    }
    DBG(1) DOUT() << "RE flag: '" << c_flg[1] << "' is set" << std::endl;
   }
 } while(rest_size != re_str.size());

 return flag;
}
#undef FLEN



Json::Jsearch Json::search_suffix_(char sfx) const {
 #include "dbgflow.hpp"
 // Jsearch is defined so that its first letter corresponds to the suffix
 for(int i = Jsearch::regular_match; i < Jsearch::end_of_lexemes; ++i)
  if(sfx == ENUMS(Jsearch, i)[0])
   return static_cast<Jsearch>(i);
 return Jsearch::end_of_lexemes;                                // when no match found
}



void Json::parse_quantifier_(std::string::const_iterator &si, iterator & it) const {
 #include "dbgflow.hpp"
 // parse quantifiers in searches (<..>), which could have following notation:
 // 1. numbers might be spelled with preceding + or w/o: <..>n, <..>+n
 // 2. numbers might be entirely missed: <..>, <..>n:, <..>:n, <..>:
 // 3. instead of any numbers there could be interpolation operator: <..>{ns}, <..>:{ns}, etc
 // 4. additionally increment step could be given: <..>::n, etc
 auto & back_ws = it.walk_path_().back();
 parse_range_(si, back_ws, ParseThrow::May_throw);
 if(back_ws.is_directive())                                     // directives requires
  back_ws.type = WalkStep::WsType::Directive;                   // directive type overriding
 DBG(2) DOUT() << "offset / range: " << back_ws.ofst() << " / " << back_ws.range() << std::endl;

 // advance to the next lexeme
 while(*si == CHR_SPCE) ++si;
 if(not(*si AMONG(LXM_SUB_OPN, LXM_SCH_OPN, LXM_SCH_CLS, CHR_NULL)))
  throw EXP(Jnode::ThrowReason::walk_expect_lexeme);
}



void Json::parse_range_(std::string::const_iterator &si, WalkStep &ws, ParseThrow throwing) const {
 #include "dbgflow.hpp"
 // parse search quantifier or subscript range, update WalkStep's: offset, head, tail, type
 // when parsing subscript range: successful parse is indicated by si pointing to terminating '\0'
 // when parsing search quantifier: parsing ends where legit quantifier parsing ends
 auto end_of_qnt = [](std::string::const_iterator &si) {        // legit end of quant.
                    return *si AMONG(CHR_SPCE, CHR_NULL, LXM_SUB_OPN, LXM_SCH_OPN, LXM_SCH_CLS);
                   };
 if(end_of_qnt(si)) return;
 bool quantifier = throwing == ParseThrow::May_throw;           // quantifiers in search: <..>
 bool subscript = not quantifier;                               // range in a subscript: [..]
 auto sic = si;                                                 // preserved si copy

 switch(*si) {                                                  // process by first char in qnt.
  case PFX_WFR:                                                 // '^'
        if(quantifier)                                          // i.e. <..>^
         throw EXP(Jnode::ThrowReason::walk_bad_number_or_suffix); // >^ not allowed in quantifiers
        ws.type = WalkStep::WsType::Root_select;                // [^ -> a subscript (text or num)
        if(*(si + 1) == CHR_NULL) return;                       // i.e. [^] - indicate text subscr.
        ws.offset = parse_index_(++si, ParseThrow::Dont_throw, SignLogic::Must_be_signless);
        break;
  case PFX_ITR:                                                 // '+'
        ws.type = WalkStep::WsType::Range_walk;                 // assume for both [+ and >+
        ws.head = ws.offset = parse_index_(si, throwing, SignLogic::Must_be_non_negative);
        if(sic == si) return;                                   // indicate text subscript: [+-...
        break;                                                  // process further possible range
  case PFX_WFL:                                                 // '-'
        if(subscript)
         ws.type = WalkStep::WsType::Parent_select;             // [- -> parent select
        ws.head = ws.offset = parse_index_(si, throwing, SignLogic::Must_be_non_positive);
        if(sic == si) return;                                   // indicate text subscript: [-+..
        break;
  case QNT_OPN:                                                 // '{'
        if(subscript) return;                                   // [{ - text subscript
        ws.heads = ws.offsets = parse_namespaced_qnt_(si);      // preserve namespace in ws
        ws.offset = LONG_MIN;                                   // indicate NS resolution required
        break;
 case RNG_SPR:                                                  // ':'
        if(ws.is_qnt_relative() and ws.heads.empty())           // i.e. if >..<l|t:, but not {idx}:
         ws.head = ws.offset = LONG_MIN + 1;                    // indicate entire range select [:
        break;
  default:                                                      // must be numeric
        ws.head = ws.offset = parse_index_(si, throwing);
        if(sic == si) return;
 }
 // all head quantifiers have been classified
 if(*si == RNG_SPR) {                                           // only ':' can pass here
  ws.type = WalkStep::WsType::Range_walk;
  if(*++si == QNT_OPN)                                          // ":{
   { if(quantifier) ws.tails = parse_namespaced_qnt_(si); }     // preserve namespace in ws
  else                                                          // ":... - must be a number
   if(not end_of_qnt(si) and not (*si == RNG_SPR))
    ws.tail = parse_index_(si, throwing);
  if(*si == RNG_SPR) {                                          // ':' must be an increment only
   if(*++si == QNT_OPN)                                         // ":{
    { if(quantifier) ws.steps = parse_namespaced_qnt_(si); }    // preserve namespace in ws
   else                                                         // ":... - must be a number
    if(not end_of_qnt(si))
     ws.step = parse_index_(si, throwing, SignLogic::Must_be_positive);
  }
 }

 if(ws.type == WalkStep::WsType::Root_select or (quantifier and not ws.is_qnt_relative()))
  if(ws.head < 0 or ws.tail < 0)                                // then cannot go negative
    throw EXP(Jnode::ThrowReason::walk_negative_quantifier);
}



std::string Json::parse_namespaced_qnt_(std::string::const_iterator &si) const {
 #include "dbgflow.hpp"
 // parse named quantifier: '{..}'
 auto qs = ++si;
 for(; *si != QNT_CLS; ++si)
  if(*si == CHR_NULL)
   throw EXP(Jnode::ThrowReason::walk_bad_quantifier);

 std::string qn{qs, si};
 if(qn.empty()) qn = TKN_EMP;
 // this value (which normally cannot exist in the namespace) is reserved to indicate currently
 // walked value to facilitate case like <..>I{}
 ++si;
 return qn;
}



Json::signed_size_t Json::parse_index_(std::string::const_iterator &si,
                                       ParseThrow throwing, SignLogic sign_prescribtion) const {
 #include "dbgflow.hpp"
 // validate if string iterator points to a valid integer.
 // parse_index is called from parse_range, which in turn could be called either from
 // parse_quantifier (<>...), or parse_subscript_type_ ([..]) those 2 parsings assume
 // different exception behavior:
 //  o parse_quantifier will throw upon failure
 //  o parse_subscript_type_ does not throw, but indicates successful completion by
 //    updating string iterator 'si' past parsed value (i.e. it should point to '\0')
 // parse_index_ does not update 'si' in case of failure to indicate the failure
 char *endptr;
 const char * str{&*si};

 if(sign_prescribtion == SignLogic::Must_be_signless and *si AMONG(JSN_NUMP, JSN_NUMM)) {
  if(throwing != ParseThrow::May_throw) return 0;
  throw EXP(Jnode::ThrowReason::walk_bad_number_or_suffix);
 }

 auto sic{si};                                                  // si copy for validating number
 if(*sic == PFX_ITR) ++sic;                                     // WA tolerating +N JSON definition
 if(json_number_definition(sic) != Jnode::Jtype::Number or      // validate number
    isdigit(*sic)) {                                            // and a single JSON number
  if(throwing != ParseThrow::May_throw) return 0;
  throw EXP(Jnode::ThrowReason::walk_bad_number_or_suffix);
 }

 signed_size_t idx = strtol(str, &endptr, 10);

 if(sign_prescribtion == SignLogic::Must_be_non_negative and    // validate positive or 0,
    (idx < 0 or str[0] == JSN_NUMM)) {                          // and exclude case '-0'
  if(throwing != ParseThrow::May_throw) return 0;
  throw EXP(Jnode::ThrowReason::walk_bad_number_or_suffix);
 }

 if(sign_prescribtion == SignLogic::Must_be_positive and idx == 0) {    // validate positive
  if(throwing != ParseThrow::May_throw) return 0;
  throw EXP(Jnode::ThrowReason::walk_bad_number_or_suffix);
 }

 if(sign_prescribtion == SignLogic::Must_be_non_positive and idx > 0) {// validate negative
  if(throwing != ParseThrow::May_throw) return 0;
  throw EXP(Jnode::ThrowReason::walk_bad_number_or_suffix);
 }

 si += endptr - str;
 return idx;
}



void Json::build_path_(Jnode &jpath, const Json::iterator &jit) {
 // build json path (Json ARRAY) for given Json::iterator - used in interpolate() and in <..>W
 Jnode *node = &jit.json().root();

 for(auto &itr: jit.path())
  if(node->is_array()) {
   signed_size_t idx = strtol(itr.lbl.c_str(), nullptr, 16);
   jpath.push_back(idx);
   node = &(*node)[idx];
  }
  else {
   jpath.push_back(itr.lbl);
   node = &(*node)[itr.lbl];
  }
}



void Json::parse_subscript_type_(WalkStep & ws) const {
 #include "dbgflow.hpp"
 // separates numerical subscripts (e.g. [1]) from textual's (e.g. [ 1])
 // numerical offsets are: [2], [-1], [+3], [3:], [3:1], [-5:-1], [+3:5], [:], [:-1]
 // parse_subscript_type_ is run after all lexemes are parsed
 DBG(2) DOUT() << "partial: " << ws << std::endl;

 if(ws.is_search()) {                                           // it's a search lexeme's walkstep
  if(ws.stripped.size() == 2)                                   // there's an attached label scope
   if(ws.is_lbl_based())                                        // label (l/L/t) search type cannot
    throw EXP(Jnode::ThrowReason::walk_search_label_with_attached_label);  // attached label offset
  return;                                                       // don't process search lexeme
 }
 // if not search (and/or directives too), it must be a subscript: [..]
 auto si = ws.stripped.front().cbegin();
 parse_range_(si, ws, ParseThrow::Dont_throw);
 if(ws.stripped.front().empty() or *si != CHR_NULL) {
  ws.jsearch = Jsearch::text_offset;                            // parsing failed - it's a text
  ws.type = WalkStep::WsType::Static_select;                    // text can only be Static_select
 }
 DBG(2) DOUT() << "offset / range: " << ws.ofst() << " / " << ws.range() << std::endl;
}





//
//
// Json::iterator methods
//
//


Jnode & Json::iterator::operator*(void) {
 // dereference Json::iterator
 if(not is_koj_last())
  return pv_.empty()?                                           // then return normal super-node
          sn_(json().root(), this):
          sn_(pv_.back().jit->KEY, pv_.back().jit->VALUE, this);
                                                                // walk has been completed here
 // return value of label/index in the supernode's JSON (re-interpret node)
 if(pv_.empty())
  throw json().EXP(Jnode::ThrowReason::walk_root_has_no_label);

 auto &parent_node = pv_.size() == 1? json().root(): *pv_[pv_.size() - 2].jnp;
 switch (parent_node.type()) {
  case Jnode::Jtype::Object:                                    // <>k may be re-interpreted
        sn_.type_ = Jnode::Jtype::String;                       // storing value and type
        sn_.value_ = pv_.back().jit->KEY;                       // into supernode itself
        break;
  case Jnode::Jtype::Array:
        sn_.type_ = Jnode::Jtype::Number;
        sn_.value_ = std::to_string(std::stol(pv_.back().jit->KEY, nullptr, 16));
        break;
  default:
        break;
 }
 return sn_(pv_.back().jit->KEY, static_cast<Jnode&>(sn_), this);
}



#define DBGBL_REF (json())


bool Json::iterator::incremented(void) {
 #include "dbgflow.hpp"
 // a wrapper for incremented_() facilitating replication >..<Fn (n >= 1) functionality
 if(incremented_()) return true;

 auto is_Fn_replicator = [&](WalkStep &ws)
       { return not ws.is_locked() and ws.jsearch == Jsearch::Forward_itr and
         ws.is_non_recursive() and  ws.offset > 0; };
 if(none_of(ws_.begin(), ws_.end(), is_Fn_replicator)) return false;
 // now we don't know if path is walkable or not, but if there are replicators, will try re-walking
 for(auto &ws: ws_)                                             // decr. first facing replicator
  if(is_Fn_replicator(ws))
   { --ws.offset; break; }

 for(auto &ws: ws_) {                                           // reset entire walk-path but NS
  if(ws.type == WalkStep::WsType::Range_walk)                   // reload [_:_] or <>_: lexeme
   ws.offset = ws.type == WalkStep::WsType::Range_walk and not ws.heads.empty()? LONG_MIN: ws.head;
  if(ws.type == Json::WalkStep::WsType::Directive_inactive)     // activate inactivated directives
     ws.type = Json::WalkStep::WsType::Directive;
 }
 unlock_fs_domain_(-1);                                         // unlock possibly locked domains
 // json().clear_ns();                                          // let NS survive over reps.

 DBG(json(), 2) DOUT(json()) << "replicating entire walk" << std::endl;
 walk_();
 if(pv_.empty() or pv_.back().jit != json().end_()) return true;
 return pv_.clear(), incremented_();
}



bool Json::iterator::incremented_(void) {
 #include "dbgflow.hpp"
 // produce a next iterator per walk string (this call is just a wrapper for increment_)
 if(not pv_.empty() and pv_.back().jit == json().end_())
  return false;                                                 // don't attempt incrementing end()

 signed_size_t wsi = 0;                                         // find ws index where to start
 for(bool fs_seen = false; wsi < SGNS_T(walk_path_().size()); ++wsi) {
  const auto & ws = walk_path_()[ wsi ];
  if(ws.is_locked()) continue;                                  // ignore locked out domains
  if(ws.jsearch == Jsearch::fail_safe) { fs_seen = true; continue; }
  if(fs_seen and ws.jsearch == Jsearch::Forward_itr and ws.offset == 0)
   break;                                                       // end of unlocked FS domain
 }

 wsi = next_iterable_ws_(wsi);
 if(wsi < 0) {
  end_path_();
  DBG(json(), 2) DOUT(json()) << "path is out of iterations / non-iterable" << std::endl;
  return false;
 }

 signed_size_t rv = increment_(wsi);                            // rv>0 indicates further increment
 while(rv >= 0) rv = increment_(rv);                            // is required, recursion avoidance

 return rv == WLK_SUCCESS;
}



bool Json::iterator::is_nested(iterator & it) const {
 #include "dbgflow.hpp"
 // check if we're nesting iterator. end() iterator considered to be non-nested
 if(pv_.empty()) return json().is_iterable();                   // root always nests (if iterable)
 if(pv_.back().jit == json().end_())                            // end() does not nest
  return false;
 for(size_t i = 0; i<pv_.size() and i<it.pv_.size(); ++i)
  if(pv_[i].lbl != it.pv_[i].lbl)
   return false;
 return true;
}



bool Json::iterator::is_valid_(Jnode & jn, size_t idx) const {
 #include "dbgflow.hpp"
 // check if all labels in path-vector are present
 if(idx >= pv_.size())                                          // no more pv_ idx to check
  return true;                                                  // all checked, return true then
 auto it = jn.children_().find(pv_[idx].lbl);                   // first try by label, if found
 if(it != jn.children_().end() and &it->VALUE == pv_[idx].jnp)  // then validate by Jnode addr
  return is_valid_(it->VALUE, idx + 1);                         // check the rest of the tree
 return false;
}



Json::signed_size_t Json::iterator::walk_(void) {
 #include "dbgflow.hpp"
 // walk 'ws' structure from the root building a path vector
 // empty pv_ addresses json.root()
 auto init_sqnc = [&] { pv_.clear(); lwsi_ = SIZE_T(-1); return true; };
 auto exit_sqnc = [&] (bool) { if(lwsi_ >= ws_.size()) lwsi_ = ws_.size() - 1; };
 GUARD(init_sqnc, exit_sqnc)

 Jnode * jnp = & json().root();
 for(size_t i = 0; i < ws_.size(); lwsi_ = i++) {
  if(ws_[i].is_locked()) continue;                              // locked domain - do not process
  walk_step_(i, jnp);                                           // walkStep builds up a path-vector
  DBG(json(), 2) show_built_pv_(DOUT(json()));
  if(pv_.empty())
   { jnp = & json().root(); continue; }                         // empty pv_ is a valid path: root
  if(pv_.back().jit != json().end_())                           // walk_step_ did not fail,
   { jnp = &pv_.back().jit->VALUE; continue; }                  // continue walking then

  // walk_step FAILED here (pv_.back().jit == json().end_())
  if(ws_[i].jsearch == Jsearch::Forward_itr) {                  // Forward_itr faced while walking
   DBG(json(), 3)
    DOUT(json()) << ENUMS(Json::Jsearch, Forward_itr) << " at [" << i << "], "
                 << (ws_[i].is_recursive()? "skipping (qnt: ":"stopping (qnt: ")
                 << ws_[i].load_offset(json()) << ")" << std::endl;
   if(ws_[i].is_non_recursive())                                // ><F, - "stop" type
    { pv_.pop_back(); lwsi_ = i - 1; break; }
   if(ws_[i].load_offset(json()) != 0)                          // facilitate <>Fn
    { pv_.pop_back(); i += ws_[i].load_offset(json()) - 1; continue; }
   return -i;                                                   // facilitate <>F0 (continue)
  }

  signed_size_t fs_wsi = failed_stop_(i);                       // check if FS gets locked there
  if(fs_wsi < 0) return i;                                      // no fail_safe, return failing wsi
  // else - continue, the domain is locked by now, pv is restored
  jnp = pv_.empty()? & json().root(): &pv_.back().jit->VALUE;
 }
                                                                // successfully walked ws
 DBG(json(), 2) { DOUT(json()) << "finished walking with "; show_built_pv_(DOUT(json())); }
 sn_type_ref_() = pv_.size()>1? pv_[pv_.size()-2].jit->VALUE.type(): json().type();
 return WLK_SUCCESS;
}



void Json::iterator::show_built_pv_(std::ostream &out) const {
 std::string dlm = "built path vector: ";
 for(auto &it: pv_) {
  out << dlm << (it.jit == json().end_()? "(end)": "[" + it.lbl + "]");
  dlm = "->";
 }
 if(dlm != "->") out << dlm << "(root)";
 out << std::endl;
}



void Json::iterator::walk_step_(size_t wsi, Jnode *jn) {        // wsi: walk-step idx
 #include "dbgflow.hpp"
 // walk a single lexeme
 auto & ws = ws_[wsi];
 DBG(json(), 3)
  DOUT(json()) << "walking step: [" << wsi << "], lexeme: " << ws.search_type() << std::endl;

 if(ws.type >= Json::WalkStep::WsType::Directive)
  return process_directive_(wsi, jn);

 switch(ws.jsearch) {
  case Jsearch::numeric_offset:                                 // [123]
        return walk_numeric_offset_(wsi, jn);
  case Jsearch::text_offset:                                    // [abc]
        return walk_text_offset_(wsi, jn);
  default:
        return walk_search_(wsi, jn);                           // facilitate all others: <..>/>..<
 }
}



void Json::iterator::process_directive_(size_t wsi, Jnode *jn) {
 #include "dbgflow.hpp"
 // factoring walk_step_
 auto & ws = ws_[wsi];
 if(ws.type == Json::WalkStep::WsType::Directive_inactive) {
  DBG(json(), 3) DOUT(json()) << "directive is inactive, skipping" << std::endl;
  return;
 }
 auto engage_tkn = [&] { json().ns(TKN_EMP) = jn->is_number()? *jn: NUL{}; return true; };
 auto erase_tkn = [&](bool) { json().ns().erase(TKN_EMP); };
 GUARD(engage_tkn, erase_tkn)

 switch(ws.jsearch) {
  case Jsearch::Zip_size:                                       // facilitate <..>Z
        json().ns()[ws.stripped.front()] = ws.is_non_recursive()?
                                            jn->children():     // >..<Z
                                            ws.load_head(json()) == 1?  // <..>Z[1]
                                             (jn->is_string()? jn->str().size(): -1.): jn->size();
        break;
  case Jsearch::zip_namespace:                                  // facilitate <..>z
        json().erase_ns_(ws.stripped.front());
        break;
  case Jsearch::Walk_path: {                                    // facilitate <..>W
         auto & jp = json().ns()[ws.stripped.front()] = ARY{};
         build_path_(jp, *this);
        }
        break;
  case Jsearch::Step_walk:                                      // facilitate <..>S
        return step_walk_(wsi);
  case Jsearch::fail_safe:                                      // facilitate <..>f:
        ws.fs_path = pv_;                                       // preserve path vector if WS fails
        DBG(json(), 3) DOUT(json()) << "recorded fail-safe: [" << wsi << "]" << std::endl;
        return maybe_nsave_(ws, jn);                            // cannot be inactive, hence return
  case Jsearch::Forward_itr:                                    // facilitate <..>F / >..<F
        if(ws.is_non_recursive() and ws.offset == LONG_MIN)     // offset is in NS, needs init'ing
         ws.offset = ws.fetch_from_ns(ws.offsets, json());      // LONG_MIN triggers init from NS
        end_path_();                                            // let process it in walk_()
        return maybe_nsave_(ws, jn);
  case Jsearch::value_of_json:                                  // facilitate <..>v (non-empty)
  case Jsearch::key_of_json:
        maybe_nsave_(ws, jn);
        break;
  case Jsearch::Increment_num:
        process_directive_I_(wsi, jn);
        break;
  case Jsearch::user_handler:                                   // facilitate <..>u#
        if(not json().is_engaged(CbType::Lexeme_callback)
           or ws.load_offset(json()) >= SGNS_T(json().uws_callbacks().size()))
         throw json().EXP(Jnode::ThrowReason::Walk_callback_not_engaged);
        if(not json().uws_callbacks()[ws.load_offset(json())](ws.stripped.front(), *this))
         end_path_();
        return;                                                 // don't inactivate this directive
  default:
        throw json().EXP(Jnode::ThrowReason::walk_a_bug);       // covering compiler's warning
 }
 // all directives which made to this point have to be deactivated until re-activated
 // in next_iterable_ws_() - i.e. when lexeme with most significant position was incremneted
 ws.type = Json::WalkStep::WsType::Directive_inactive;
}



void Json::iterator::step_walk_(size_t wsi) {
 #include "dbgflow.hpp"
 // re-walk (from the root) a path preserved in the NS (facilitate <..>S lexeme)
 auto & ws = ws_[wsi];
 // 0. check if ns exists
 auto found_ns = json().ns().find(ws.stripped.front());
 if(found_ns == json().ns().end() or found_ns->VALUE.ref().is_neither())    // non-exist. or erased
  throw json().EXP(Jnode::ThrowReason::walk_non_existant_namespace);
 auto & nse = found_ns->VALUE.ref();

 // 1. validate a path - see if it's an array
 if(not nse.is_array())                                         // is an array (i.e. a path)?
  return end_path_();                                           // let process it in walk_()

 // 2. let's walk it then
 Jnode * jnp = & json().root();
 pv_.clear();                                                   // path-vector being built
 for(auto &pe: nse) {                                           // pe: path-vector entry
  if(not jnp->is_iterable()) return end_path_();                // jnp must point to array or obj.

  std::string idx;
  const std::string *lbl{&idx};
  switch(pe.type()) {
   case Jnode::Jtype::Number:
         if(jnp->is_object()) return end_path_();               // numeric label => jnp must be arr
         idx = pe.idx2lbl_(pe.integer());
         break;
   case Jnode::Jtype::String:
         if(jnp->is_array()) return end_path_();                // label is text => jnp must be obj
         lbl = &pe.str();
         break;
   default:
         return end_path_();;                                   // path can be made of num and str
  }

  auto found = jnp->children_().find(*lbl);
  if(found == jnp->children_().end()) return end_path_();       // label not found
  jnp = & found->VALUE;
  pv_.emplace_back(std::move(found));
 }
}



void Json::iterator::process_directive_I_(size_t wsi, Jnode *jn) {
 #include "dbgflow.hpp"
 // facilitate <..>In:m directive (optional: n - increment, m - factor)
 // first increment is added then factored, i.e. <a>In:m -> (a + n) * m
 auto & ws = ws_[wsi];
 auto found_ns = json().ns().find(ws.stripped.front());
 auto already_nsaved{false};

 // create / init namespace (if not yet)
 if(found_ns == json().ns().end() or found_ns->VALUE.ref().is_neither()) {  // non-exist. or erased
  if(found_ns != json().ns().end())                             // if was erased
   json().clear_ns(ws.stripped.front());                        // then clear erased value
  if(ws.user_json.is_neither())                                 // NO user json (<a:user_json>)
   json().ns().emplace(ws.stripped.front(), 0);                 // init with 0
  else                                                          // i.e. <a:user_json>
   { maybe_nsave_(ws, jn); already_nsaved = true; }             // i.e. preserve only on init
  found_ns = json().ns().find(ws.stripped.front());
 }
 else                                                           // ns already exists
  if(not found_ns->VALUE.ref().is_number())
   return;                                                      // <..>I processes only numerics

 // namespace already exists
 signed_size_t incr = ws.load_head(json());
 signed_size_t mult = ws.load_tail(json());

 if(not ws.user_json.is_neither() and incr == 0 and mult == LONG_MAX) { // it's init-only <v:..>I
  if(not already_nsaved) maybe_nsave_(ws, jn);                  // must nsave, (if not already)
  return;
 }
 // it's non-initial increment/multiplier
 found_ns->VALUE = (found_ns->VALUE.ref().num() + (incr == LONG_MIN + 1? 0: incr))
                   * (mult == LONG_MAX? 1: mult);
}



void Json::iterator::walk_numeric_offset_(size_t wsi, Jnode *jn) {
 #include "dbgflow.hpp"
 // walk a numerical offset, e.g.: [3], [+0], [-1], [^2], [:], etc
 auto &ws = ws_[wsi];

 if(ws.type == WalkStep::WsType::Root_select)                   // root offset, e.g.: [^2]
  return pv_.resize(ws.offset > SGNS_T(pv_.size())?
         pv_.size(): ws.offset);                                // shrinking path

 if(ws.type == WalkStep::WsType::Parent_select)                 // negative offset, e.g.: [-2]
  return pv_.resize(-ws.offset <= SGNS_T(pv_.size())? pv_.size() + ws.offset: 0);
 // [0], [+1], [..:..] etc
 size_t node_size = jn->children_().size();
 size_t offset = normalize_(ws.offset, jn);
 if(ws.type == WalkStep::WsType::Range_walk) ws.offset = offset;// ws iterable, require normalizing
 if(offset >= node_size or offset >= normalize_(ws.tail, jn))   // beyond children's size/tail
  return end_path_();

 if(ws.type == WalkStep::WsType::Static_select and jn->is_array())  // [N] - subscript array
  { pv_.emplace_back(jn->iterator_by_idx_(offset)); return; }

 auto it = build_cache_(jn, wsi);

 DBG(json(), 1) DOUT(json()) << "found cached idx " << offset << std::endl;
 pv_.emplace_back(it);
}



Jnode::iter_jn Json::iterator::build_cache_(Jnode *jn, size_t wsi) {
 #include "dbgflow.hpp"
 // build cache for subscripts: [..]
 auto &ws = ws_[wsi];
 auto & cache_map = json().srchc_;                              // all caches map
 CacheKey skey{jn, ws};                                         // prepare a search key
 auto found_cache = cache_map.find(skey);
 bool build_cache = found_cache == cache_map.end() or           // cache does not exist, or
                    (found_cache->KEY.ws.type != WalkStep::WsType::Cache_complete and // not cached
                     SIZE_T(ws.offset) >= found_cache->VALUE.front().pv.size());

 if(build_cache) {
  DBG(json(), 1) DOUT(json()) << "building cache for [" << wsi << "] " << skey << std::endl;
  auto & cache = found_cache == cache_map.end()? cache_map[skey]: found_cache->VALUE;
  if(cache.empty())
   { cache.resize(1); cache.front().pv.emplace_back(jn->children_().begin()); }

  size_t size = ws.type == WalkStep::WsType::Static_select? ws.offset: normalize_(ws.tail, jn) - 1;
  for(size_t i = cache.front().pv.size(); i <= size; ++i) {
   auto it = cache.front().pv.back().jit;
   cache.front().pv.emplace_back(++it);
  }
  if(cache.front().pv.size() == jn->children_().size()) {
   // indication that cache is complete is stored in the CacheKey:
   skey.ws.type = WalkStep::WsType::Cache_complete;             // indicate cache completed
   auto pc = move(cache);                                       // move/preserve the cache
   json().srchc_.erase(skey);                                   // erase old skey
   json().srchc_[skey] = move(pc);                              // create new one
  }

  found_cache = cache_map.find(skey);
  DBG(json(), 1)
   DOUT(json()) << "built cache size: " << found_cache->VALUE.front().pv.size()
                << ", cache status: " << ENUMS(WalkStep::WsType, found_cache->KEY.ws.type)
                << std::endl;
 }

 return found_cache->VALUE.front().pv[ws.offset].jit;
}



void Json::iterator::walk_text_offset_(size_t wsi, Jnode *jn) {
 #include "dbgflow.hpp"
 // walk a text offset, e.g.: [label]
 auto &ws = ws_[wsi];
 auto it = jn->children_().find(ws.stripped.front());           // see if label exist
 if(it == jn->children_().end())
  end_path_();
 else
  pv_.emplace_back(it);                                         // if so, add to the path-vector
}



// walk_search_ for given WalkStep[wsi] finds (and possibly caches) found JSON path:
// - all cacheable paths are cached like this:
//   o if search_key is non-existent, then build cache up until Static_select'ed entry,
//     or up to WalkStep's tail
//   o if search_key exists, check if instance ('i') < cache's size:
//     - if true: (entry instance was already cached) return cached path
//     - if false (entry instance outside of cache), then check if cache has been completed?
//       - if type == Cache_complete - return global end (there's no such instance for sure)
//       - otherwise re-search up till the offset/tail index (for static/range respectively)
// - Caching eligibility:
//   o non-eligible:
//     - dynamic tags: namely: s, t, j(templated) - cannot be cached in principle
//     - non-instance searches - entries with relative quantifiers: >..<t, >..<l
//   o the rest of searches are eligible for caching
//
void Json::iterator::walk_search_(size_t wsi, Jnode *jn) {
 #include "dbgflow.hpp"
 // check range (if offset beyond the tail) and do cache-less search, then engaged cached search
 auto & ws = ws_[wsi];
 if(ws.offset == LONG_MIN)                                      // offset is in NS, needs reloading
  ws.offset = ws.fetch_from_ns(ws.offsets, json());             // LONG_MIN triggers init from NS

 size_t offset = ws.load_offset(json());
 size_t tail = ws.load_tail(json());
 if(ws.is_qnt_relative()) {                                     // offset & tail must be signed
  if(re_normalize_(ws.load_offset(json()), jn) >= re_normalize_(ws.load_tail(json()), jn))
   return end_path_();
 }
 else
  if(offset >= tail)                                            // counted beyond tail
   return end_path_();                                          // return |nothing found|

 // engage cache-less search:
 if(ws.is_cacheless())
  return research_(jn, wsi, nullptr, nullptr);                  // vpv=null: cache-less search

 bool set_neither{false};                                       // set walk's user_json to Neither?
 if(ws.require_ns_resolution())
  set_neither = ws.resolve_ns(*this);                           // only some would require this

 // continue with cached walk search
 walk_search_cached_(wsi, jn, offset);

 if(set_neither)                                                // eg json_match requires that type
  ws.user_json.type(Jnode::Jtype::Neither);                     // is set to Neither for resolution
}



void Json::iterator::walk_search_cached_(size_t wsi, Jnode *jn, size_t offset) {
 #include "dbgflow.hpp"
 // engage search with caching (factored walk_search_):
 auto &ws = ws_[wsi];
 if(ws.jsearch == Jsearch::query_original or ws.jsearch == Jsearch::Query_duplicate)
  json().sortc_.erase(ws.wsuid);
  // sort cache has to be erased in each (re)walk for original/dup queries to ensure
  // a local significance of a search

 auto & cache_map = json().srchc_;                              // all caches map
 CacheKey skey{jn, ws};                                         // prepare a search key
 auto found_cache = cache_map.find(skey);
 bool build_cache = found_cache == cache_map.end() or           // cache does not exist, or
                    (found_cache->KEY.ws.type != WalkStep::Cache_complete and   // not cached yet
                     offset >= found_cache->VALUE.size());
 if(build_cache) {
  DBG(json(), 1) DOUT(json()) << "building cache for [" << wsi << "] " << skey << std::endl;
  auto & cache = found_cache == cache_map.end()? cache_map[skey]: found_cache->VALUE;
  research_(jn, wsi, &cache, &skey);                            // build cache
  found_cache = cache_map.find(skey);
  DBG(json(), 1)
   DOUT(json()) << "built cache size: " << found_cache->VALUE.size() << ", cache status: "
                << ENUMS(WalkStep::WsType, found_cache->KEY.ws.type) << std::endl;
 }

 // find cached entry (if one exist in the cache):
 if(offset >= found_cache->VALUE.size()) {                      // offset outside of cache:
  DBG(json(), 1) DOUT(json()) << "no cached instance found"<< std::endl;
  return end_path_();                                           // return global end()
 }

 DBG(json(), 1) DOUT(json()) << "found cached idx " << offset << std::endl;
 if(ws.jsearch  == Jsearch::go_ascending or ws.jsearch == Jsearch::Go_descending) {
  offset = gsort_matches_(found_cache->VALUE[offset].pv.empty()?
                          jn: found_cache->VALUE[offset].pv.back().jnp, ws, offset);
  DBG(json(), 1) DOUT(json()) << "remapped cached idx " << offset << std::endl;
 }
 for(const auto &path: found_cache->VALUE[offset].pv)           // otherwise augment the path
  pv_.push_back(path);
 for(const auto &kv: found_cache->VALUE[offset].ns)             // and so do the namespaces
  json().ns()[kv.KEY] = kv.VALUE.ref();
 maybe_nsave_(ws, pv_.empty()? &json().root(): pv_.back().jnp); // save into NS if lexeme is right
}



void Json::iterator::research_(Jnode *jn, size_t wsi,
                               std::vector<Json::SearchCacheEntry> * vpv, CacheKey *skey) {
 #include "dbgflow.hpp"
 // wrapper for re_search_
 auto & ws = ws_[wsi];

 signed_size_t i = ws.is_cacheless()?                           // i: instance
                    ws.load_offset(json()):                     // find only current instance
                    ws.load_tail(json()) -1;                    // find all up to 'tail' instance
 if(ws.jsearch == Jsearch::go_ascending or ws.jsearch == Jsearch::Go_descending) i = LONG_MAX - 1;
 signed_size_t cache_down_from = vpv? i - vpv->size(): i;

 if(vpv) vpv->resize(vpv->size() + 1);                          // placeholder for 1st pathvector
 DBG(json(), 3)
  DOUT(json()) << "top instance idx: " << (i == LONG_MAX - 1? "all": std::to_string(i))
               << ", cache down from: " << (cache_down_from > LONG_MAX/2? "tail":
                                             std::to_string(cache_down_from))
               << ", descend: " << ENUMS(SearchType, ws.is_recursive()? Recursive: Non_recursive)
               << ", caching? " << (vpv? "yes": "no") << std::endl;

 bool found = re_search_(jn, ws, nullptr, i, cache_down_from, SearchType::Non_recursive, vpv);
 if(vpv) vpv->pop_back();                                       // last entry is redundant
 if(found) {
  if(ws.is_cacheless()) maybe_nsave_(ws, jn);                   // cache-less do not intersect with
  return;                                                       // namespace lexemes, so far ...
 }

 if(vpv == nullptr)                                             // indicate |nothing found|
  return end_path_();                                           // for cacheless search type

 // indication that cache is complete (entire Jnode was searched) is stored in the CacheKey:
 skey->ws.type = WalkStep::Cache_complete;                      // indicate cache completed
 auto nvp = move(*vpv);                                         // move/preserve the cache
 json().srchc_.erase(*skey);                                    // erase old skey
 json().srchc_[*skey] = move(nvp);                              // create new one
}



bool Json::iterator::re_search_(Jnode *jn, WalkStep &ws, const char *lbl, signed_size_t &i,
                                signed_size_t cdf, SearchType pass,
                                std::vector<Json::SearchCacheEntry> * vpv) {
 #include "dbgflow.hpp"
 // build cache recursively, return true/false if match found (i'th instance), otherwise false
 // returning false globally indicates entire json has been searched up
 // 1. implementing recursive, const/static type (building cache, vpv != nullptr)
 // 2. implementing recursive, dynamic type (vpv == nullptr)
 // 3. implementing non-recursive, const type (building cache, vpv != nullptr)
 dmx_callback_(jn, lbl, vpv);

 if(pass == SearchType::Recursive or ws.is_recursive()) {       // >..<: allow in recursive pass
  if(jn->is_atomic())
   return atomic_match_(jn, lbl, ws, &(vpv? vpv->back().ns: json().ns())) and
          cache_entry_(i, cdf, vpv);
                                                                // here jn is iterable
  if(match_iterable_(jn, lbl, ws) and cache_entry_(i, cdf, vpv))
   return true;
 }

 if(pass == SearchType::Recursive and ws.is_non_recursive())    // >..<: further recursion denied
  return false;

 signed_size_t idx = 0;
 for(auto it = jn->children_().begin(); it != jn->children_().end(); ++it) {
  if(vpv) vpv->back().pv.emplace_back(it); else pv_.emplace_back(it);

  signed_size_t j = 0;                                          // to be used in lieu of i
  if(ws.is_lbl_based())
   if(label_match_(it, jn, idx, ws, &(vpv? vpv->back().ns: json().ns())) and
      cache_entry_(ws.is_qnt_relative()? j: i, ws.is_qnt_relative()? j: cdf, vpv))
    return true;

  if(re_search_(&it->VALUE, ws, jn->is_object()? it->KEY.c_str(): nullptr,
                i, cdf, SearchType::Recursive, vpv))
   return true;

  if(vpv) vpv->back().pv.pop_back(); else pv_.pop_back();
  ++idx;
 }

 return false;
}



bool Json::iterator::cache_entry_(signed_size_t &i, signed_size_t cdf,
                                  std::vector<Json::SearchCacheEntry> * vpv) {
 #include "dbgflow.hpp"
 // cache in entry if cache pointer was given (if vpv != nullptr)
 // if "to idx" (i) above "cache_from" -
 // but only upon i <= 0 return true, otherwise return false (not requested instance yet)
 if(--i >= cdf) return false;                                   // index above "cache from"

 if(vpv != nullptr) {                                           // build cache then
  vpv->push_back(vpv->back());                                  // cache-in (built cache actually)
  vpv->back().ns.clear();                                       // this is required aft REGEX match
 }
 return i < 0;
}



bool Json::iterator::match_iterable_(Jnode *jn, const char *lbl, WalkStep &ws) {
 #include "dbgflow.hpp"
 // match any iterable suffixes and attached label (if any)
 // assert(jn->is_iterable())
 if(ws.stripped.size() > 1)                                     // there's an attached search label
  if(lbl == nullptr or ws.stripped.back() != lbl )              // there's no label
   return false;                                                // label does not match

 if(ws.jsearch == Jsearch::search_from_ns) {                    // facilitating <..>s
  const auto found = json().ns().find(ws.stripped.front());     // see if value was preserved
  if(found == json().ns().end() or found->VALUE.ref().is_neither())
   throw json().EXP(Jnode::ThrowReason::walk_non_existant_namespace);
  return *jn == found->VALUE.ref();
 }
 if(ws.jsearch == Jsearch::json_match)                          // facilitating <..>j
  return json_match_(jn, ws);

 if(ws.jsearch == Jsearch::query_original)                      // <..>q|Q
  return is_original_(*jn, ws);
 if(ws.jsearch == Jsearch::Query_duplicate)
  return is_original_(*jn, ws, OriginalSrch::Reverse);          // reverse means find duplicate

 if((jn->is_object() and ws.jsearch == Jsearch::object_match) or    // <..>o|i|c|w|e
    (jn->is_array() and ws.jsearch == Jsearch::indexable_match) or
    (ws.jsearch == Jsearch::wide_match or ws.jsearch == Jsearch::collection_match) or
    (ws.jsearch == Jsearch::end_node_match and jn->is_empty()))
  return true;

 if(ws.jsearch == Jsearch::go_ascending or ws.jsearch == Jsearch::Go_descending) // during cache
  { gsort_matches_(jn, ws); return true; }                      // build phase always return true

 return false;
}



bool Json::iterator::atomic_match_(const Jnode *jn,
                                   const char *lbl, WalkStep &ws, map_jne * nsp) {
 #include "dbgflow.hpp"
 // see if string/number/bool/null value matches
 if(ws.stripped.size() > 1)                                     // label attached: try matching
  if(lbl == nullptr or ws.stripped.back() != lbl) return false; // no label, or not matching

 auto user_json_ptr = &ws.user_json;                            // set user_json from walkstep
 switch (ws.jsearch) {
  case Jsearch::object_match:
  case Jsearch::indexable_match:
  case Jsearch::collection_match:
        return false;
  case Jsearch::wide_match:
  case Jsearch::end_node_match:
  case Jsearch::atomic_match:
        return true;
  case Jsearch::go_ascending:
  case Jsearch::Go_descending:
        gsort_matches_(jn, ws);                                 // during cache building phase
        return true;                                            // always return true
  case Jsearch::search_from_ns:
        return *user_json_ptr == *jn;
  case Jsearch::json_match:
        return json_match_(jn, ws);
  case Jsearch::query_original:
        return is_original_(*jn, ws);
  case Jsearch::Query_duplicate:
        return is_original_(*jn, ws, OriginalSrch::Reverse);    // reverse means find duplicate
  case Jsearch::regular_match:
  case Jsearch::Regex_search:
  case Jsearch::digital_match:
  case Jsearch::Digital_regex:
  case Jsearch::Phrase_match:
  case Jsearch::Numerical_match:
        return string_match_(jn, ws, nsp);
  case Jsearch::label_match:
  case Jsearch::Label_RE_search:
  case Jsearch::tag_from_ns:
        return false;
  default:
        return bull_match_(jn, ws);
 }
}



bool Json::iterator::string_match_(const Jnode *jn,
                                   const WalkStep &ws, map_jne * nsp) const {
 #include "dbgflow.hpp"
 // match numbers and strings also using RE
 switch(ws.jsearch) {
  case Jsearch::Phrase_match:
        return jn->is_string();
  case Jsearch::Numerical_match:
        return jn->is_number();
  case Jsearch::digital_match:
        return jn->is_number() and jn->val() == ws.stripped.front();
  case Jsearch::Digital_regex:
        return jn->is_number() and regex_match_(jn->val(), ws, nsp);
  case Jsearch::regular_match:
        return jn->is_string() and jn->val() == ws.stripped.front();
  case Jsearch::Regex_search:
        return jn->is_string() and regex_match_(jn->val(), ws, nsp);
  default:                                                      // should never reach here
        throw json().EXP(Jnode::ThrowReason::walk_a_bug);       // covering compiler's warning
 }
}



bool Json::iterator::regex_match_(const std::string &val,
                                  const WalkStep &ws, map_jne * nsp) const {
 #include "dbgflow.hpp"
 // see if regex matches and if instance matches too - only then update the namespace

 Jnode am{ARY{}};                                               // all matches - go into '$@' ns
 std::smatch m;
 bool retval{false};
 for(std::sregex_iterator it = std::sregex_iterator(val.begin(), val.end(), ws.rexp);
     it != std::sregex_iterator(); ++it) {
  for(size_t i = 0; i < it->size() ; ++i) {                     // save matches in the namespace:
   if(it->size() == 1 or i > 0)                                 // groups only or entire match
    am.push_back( std::move(STR{retval? std::move((*it)[i]): (*it)[i]}) );  // but not both
   if(retval) continue;                                         // save only 1st match results
   nsp->emplace( "$" + std::to_string(i), STR{std::move((*it)[i])});
   DBG(json(), 6)
    DOUT(json()) << Debug::btw << "preserved in namespace [" << "$" + std::to_string(i) << "]: "
                 << nsp->at("$" + std::to_string(i)).ref().to_string(Jnode::Raw, 1) << std::endl;
  }
  retval = true;
 }
 if(retval) {
  nsp->emplace( ITRP_ALLM, std::move(am));            // namespace $@ holds all matches
  DBG(json(), 6)
   DOUT(json()) << Debug::btw << "preserved in namespace [" << ITRP_ALLM << "]: "
                 << nsp->at(ITRP_ALLM).ref().to_string(Jnode::Raw, 1) << std::endl;
 }

 return retval;
}



bool Json::iterator::label_match_(Jnode::map_jn::iterator jit, const Jnode *jn, signed_size_t idx,
                                  WalkStep &ws, map_jne * nsp) const {
 #include "dbgflow.hpp"
 // return true if instance i of label (l,t) matches, false otherwise
 map_jne::iterator found;
 if(ws.jsearch == Jsearch::tag_from_ns) {                       // facilitate <..>t / >..<t
  found = json().ns().find(ws.stripped.front());                // see if value was preserved
  if(found == json().ns().end() or found->VALUE.ref().is_neither())
   throw json().EXP(Jnode::ThrowReason::walk_non_existant_namespace);   // value not found in NS
  if(not (found->VALUE.ref().type() == Jnode::Jtype::String or
          found->VALUE.ref().type() == Jnode::Jtype::Number))
   return false;                                                // value is neither string/numeric
 }
 else                                                           // <..>l|L or >..<l|L
  if(jn->is_array()) return false;                              // do not operate on ARY

 if(ws.is_recursive()) {                                        // <..>
  if(ws.jsearch == Jsearch::tag_from_ns)                        // <..>t
   return jn->is_object()? jit->KEY == found->VALUE.ref().val(): false;
  if(ws.jsearch == Jsearch::label_match)                        // <..>l
   return jit->KEY == ws.stripped.front();
  return regex_match_(jit->KEY, ws, nsp);                       // <..>L
 }

 if(ws.jsearch == Jsearch::Label_RE_search)                     // facilitate >..<L
  return regex_match_(jit->KEY, ws, nsp);
                                                                // >..<: quant. is relative here
 signed_size_t ws_off = ws.load_offset(json()),
               jn_size = SGNS_T(jn->children_().size());

 if(ws.jsearch == Jsearch::tag_from_ns and found->VALUE.ref().is_number()) {    // >..<t is numeric
  signed_size_t idx_val = found->VALUE.ref().num();             // resolve >..<t value in NS
  if(idx_val < 0 or idx_val >= jn_size) return false;           // outside of jn's chldren?
  if(ws_off < -idx_val)                                         // ws_off value too low?
   ws.head = ws.offset = ws_off = -idx_val;                     // fix too low head/offset values
  return idx - ws_off == idx_val;                               // return position match
 }
                                                                // jn is OBJ, all ARY processed
 auto found_lbl = jn->children_().find(ws.jsearch == Jsearch::tag_from_ns?
                                       found->VALUE.ref().val(): ws.stripped.front());
 if(found_lbl == jn->children_().end()) return false;           // >..<t/l value's not found
 if(idx == 0) {                                                 // first run:
  signed_size_t idx_val = std::distance(jn->children_().begin(), found_lbl);
  if(ws_off < -idx_val)                                         // offset too low?
   ws.head = ws.offset = ws_off = -idx_val;                     // fix too low offset
 }
 if(idx - ws_off < 0) return false;                             // outside of jn's children
 std::advance(jit, -ws_off);
 if(ws.jsearch == Jsearch::tag_from_ns)                         // facilitate >..<t
  return jit->KEY == found->VALUE.ref().val();                  // OBJ: return label match
 return jit->KEY == ws.stripped.front();                        // facilitate >..<l
}



bool Json::iterator::bull_match_(const Jnode *jn, const WalkStep &ws) const {
 #include "dbgflow.hpp"
 // match bool and null (hence bull)
 switch(ws.jsearch) {
  case Jsearch::boolean_match:
        return jn->is_bool() and
               (ws.stripped.front() == STR_TRUE? jn->bul():
                ws.stripped.front() == STR_FALSE? not jn->bul():
                true);
  case Jsearch::null_match:
        return jn->is_null();
  default:                                                      // should never reach here.
        throw json().EXP(Jnode::ThrowReason::walk_a_bug);       // covering compiler's warning
 }
}



bool Json::iterator::json_match_(const Jnode *jn, const WalkStep &ws) {
 #include "dbgflow.hpp"
 // facilitate match for <..>j lexeme
 if(not ws.user_json.is_neither())                              // i.e. user_json is a valid JSON
  return *jn == ws.user_json;                                   // and not a template

 Json jns = Json::interpolate(ws.stripped.front(), *this);      // otherwise, it's a template
 return jns.is_neither()? false: *jn == jns;
}



size_t Json::iterator::gsort_matches_(const Jnode *jn, WalkStep &ws, size_t offset) {
 #include "dbgflow.hpp"
 // facilitates go_ascending, Go_descending searches
 // if offset == -1, it's called during building cache (matches collection), otherwise,
 // offset remapping required (two phases: 1 matches collection, 2 cache remapping)
 // during the cache collection - collect jnode pointers (into a set - in a sorted order)
 // after cache completion return remapped value for given offset

 auto & sce = json().sortc_.emplace(ws.wsuid, SortCacheEntry{}).first->VALUE;  // sort cache entry
 auto & remap = sce.sortset;
 auto & mapset = sce.offsetmap;
 if(offset == SIZE_T(-1)) {                                     // keep collecting matches
  if(not mapset.empty()) mapset.clear();
  remap.emplace(jn, remap.size());                              // collect jnodes in sorted order
  return 0;                                                     // return value is irrelevant here
 }

 // assert: remap.size() > 0 - guaranteed by checking cache size in walk_search_()
 if(mapset.empty()) {                                           // first time out of collection
  mapset.reserve(remap.size());
  for(auto &entry: remap) mapset.push_back(entry.ord);
  remap.clear();
 }

 if(ws.jsearch == Jsearch::Go_descending)
  offset = mapset.size() - offset - 1;
 if(offset >= mapset.size())
  throw json().EXP(Jnode::ThrowReason::walk_a_bug);             // should never reach here

 return mapset[offset];                                         // return sorted offset
}



void Json::iterator::purge_ns_(const std::string &pfx) {
 #include "dbgflow.hpp"
 // erase from ns all cached keys starting with given prefix
 std::vector<map_jne::iterator> to_delete;
 to_delete.reserve(json().ns().size());

 for(auto it = json().ns().begin(); it != json().ns().end(); ++it)
  if(it->KEY.compare(0, pfx.size(), pfx, 0, pfx.size()) == 0)
   to_delete.push_back(it);

 for(auto & it :to_delete)
  json().ns().erase(it);
}



void Json::iterator::maybe_nsave_(WalkStep & ws, const Jnode *jn) {
 #include "dbgflow.hpp"
 // if lexeme is right, save JSON into the namespace, also facilitate ns init <ns:JSON>
 if(ws.jsearch != Jsearch::Increment_num)                       // it allows <uaer_json> saving
  if(not ws.is_json_capturing_lexeme() or ws.stripped.front().empty())  // don't process empty: <>
   return;                                                      // or those which are non-capturing

 if(ws.jsearch == Jsearch::key_of_json and ws.user_json.is_neither()) { // i.e.: <namespace>k
  if(pv_.empty())
   throw json().EXP(Jnode::ThrowReason::walk_root_has_no_label);
  auto & parent = pv_.size() == 1? json().root(): pv_[pv_.size()-2].jit->VALUE;
  json().ns()[ws.stripped.front()] = parent.type_ == Jnode::Jtype::Object?
                                      Jnode{ pv_.back().jit->KEY }:
                                      Jnode{ static_cast<double>(
                                             std::stol(pv_.back().jit->KEY, nullptr, 16)) };
 }
 else                                                           // namespaced
  json().ns()[ws.stripped.front()] = ws.user_json.is_neither()? *jn: ws.user_json;
 DBG(json(), 3)
  DOUT(json()) << Debug::btw << "preserved in namespace [" << ws.stripped[0] << "]: "
               << json().ns(ws.stripped[0]).to_string(Jnode::PrettyType::Raw, 1) << std::endl;
 return;
}



void Json::iterator::lbl_callback_(const char *label,
                                   const std::vector<Json::SearchCacheEntry> *vpv) {
 #include "dbgflow.hpp"
 // invoke callback attached to the label (if there's one)
 if(json().lbl_callbacks().count(label) == 0) return;           // label not registered?

 GUARD(sn_type_ref_())                                          // not needed, idiomatically good
 sn_type_ref_() = Jnode::Jtype::Object;                         // ensure supernode's correct type

 if(vpv == nullptr)                                             // callback w/o cache
  return json().lbl_callbacks()[label]( operator*() );          // call back passing a super node

 // vpv != nullptr: callback from search_all_()
 GUARD(pv_.size, pv_.resize)                                    // preserve pv_ and restore at exit
 for(auto &path: vpv->back().pv) pv_.push_back(path);           // augment path-vector
 json().lbl_callbacks()[label]( operator*() );                  // call back passing a super node
}



void Json::iterator::wlk_callback_(const Jnode *jnp) {
 #include "dbgflow.hpp"
 // invoke callbacks matching given iterator
 for(auto &ic: json().wlk_callbacks()) {
  if(ic.itr == ic.itr.end()) continue;                          // iterations are over
  if(&ic.itr->value() != jnp) continue;                         // it's a different node

  ic.callback( *ic.itr );                                       // call back passing a super node
  GUARD(jp_->is_engaged, jp_->engage_callbacks)
  json().engage_callbacks(false);
  ++ic.itr;
 }
}



void Json::iterator::dmx_callback_(const Jnode *jn, const char *lbl,
                                   const std::vector<Json::SearchCacheEntry> *vpv) {
 #include "dbgflow.hpp"
 // demux callbacks
 if(json().is_engaged(CbType::Walk_callback)) wlk_callback_(jn);
 if(lbl and json().is_engaged(CbType::Label_callback)) lbl_callback_(lbl, vpv);
}



Json::signed_size_t Json::iterator::increment_(signed_size_t wsi) {
 #include "dbgflow.hpp"
 // increment walk step and re-walk: returns true / false upon successful / unsuccessful walk
 auto & ws = walk_path_()[ wsi ];

 auto step = ws.load_step(json());
 ws.offset += step <= 0? 1: step;                               // next iteration
 unlock_fs_domain_(wsi);
 DBG(json(), 2) DOUT(json()) << "next incremented: [" << wsi << "] " << ws << std::endl;

 signed_size_t failed_wsi = walk_();

 if(pv_.empty() or pv_.back().jit != json().end_())             // successful walk
  return WLK_SUCCESS;
 if(failed_wsi < 0 and failed_wsi != WLK_SUCCESS)               // it's an <>F lexeme, then exit
  return next_iterable_ws_(-failed_wsi);                        // to continue walking

 DBG(json(), 2)
  DOUT(json()) << "WalkStep idx from a fruitless walk: " << failed_wsi << std::endl;

 if(wsi < SGNS_T(failed_wsi)) {                                 // it's not my position, plus
  DBG(json(), 2)                                                // it's in less significant place
   DOUT(json()) << "walk [" << failed_wsi
                << "] is out of iterations / non-iterable" << std::endl;
  // even if walk_ does not yield a result for a given wsi, if walk failed because of
  // other walk step (index), then next walk still might yield a match in the next record.
  // That logic is required to handle irregular JSONs
  signed_size_t next_wsi = next_iterable_ws_(failed_wsi);
  return next_wsi;
 }

 // walk at wsi failed, get next more significant ws index and reload offset at wsi
 signed_size_t next_wsi = next_iterable_ws_(wsi);               // get next more significant wsi
 if(next_wsi < 0) return next_wsi;                              // out of iteratables

 // here we need to reload the walkstep's offset with the head's value
 ws.offset = ws.type == WalkStep::WsType::Range_walk and not ws.heads.empty()?
              LONG_MIN: ws.head;
              // LONG_MIN: indicates delayed (until next actual walk) resolution
              // cannot resolve right now, cause NS might have changed by the next walking this ws
 DBG(json(), 3) DOUT(json()) << "head-initialized offset [" << wsi << "]" << std::endl;
 return next_wsi;
}



Json::signed_size_t Json::iterator::next_iterable_ws_(signed_size_t wsi) {
 #include "dbgflow.hpp"
 // get next more significant position (index within walk-path) of iterable offset,
 // otherwise (whole path non-iterable) return -1: end of iteration
 while(--wsi >= 0) {
  auto & ws = walk_path_()[ wsi ];
  if(ws.is_locked()) continue;

  if(ws.type == Json::WalkStep::WsType::Directive_inactive)
   ws.type = Json::WalkStep::WsType::Directive;
  if(ws.type == WalkStep::WsType::Range_walk) break;
 }
 DBG(json(), 3)
  DOUT(json()) << "iterable walk-step: ["
               << (wsi < 0? "N/A": std::to_string(wsi)) << "]" << std::endl;
 return wsi;
}



Json::signed_size_t Json::iterator::failed_stop_(signed_size_t wsi) {
 #include "dbgflow.hpp"
 // check if there's a valid fail stop in the path (only first found FS should be checked)
 // assert pv_.back().jit == json().end_()
 // return fail_safe ws index, or -1 (no fs found, or fs should not be engaged)
 const auto & ws = walk_path_()[wsi];
 if(ws.type == WalkStep::WsType::Range_walk) {                  // ws is a range search
  const auto & j =  pv_.size() >= 2?                            // j here is a failing json node
                     pv_[pv_.size()-2].jit->VALUE.value(): json().root();
  if(j.is_iterable() and ws.offset >= SGNS_T(j.children()))
   return WLK_SUCCESS;                                          // iterable is failing counter here
  if(ws.offset > ws.load_head(json()))                          // means there were already matches
   return WLK_SUCCESS;                                          // in that iterable
 }

 for(; wsi >= 0; --wsi) {                                       // going backwards
  auto & ws = walk_path_()[wsi];
  if(ws.jsearch == Jsearch::Forward_itr) return WLK_SUCCESS;    // not FS domain
  if(ws.jsearch != Jsearch::fail_safe) continue;                // some other walk-step
  pv_ = ws.fs_path;                                             // restore the path vector
  DBG(json(), 3)
   { DOUT(json()) << "found fs[" << wsi << "], restored "; show_built_pv_(DOUT(json())); }
  lock_fs_domain_(wsi);                                         // lock out FS domain immediately
  return wsi;
 }

 return WLK_SUCCESS;
}

#undef DBGBL_REF





 #define DBGBL_REF (jit.json())


Json Json::interpolate(Stringover tmp, Json::iterator &jit,
                       map_jne &ns, Json::ParseTrailing parse_type) {
 #include "dbgflow.hpp"
 // wrapper for interpolate_tmp, designed behavior:
 //  o Template (tmp) may or may not contain interpolations e.g.: `null` or  `{{..}}`
 //  o returned result must indicate either a successful interpolation of failure
 //  o a successful interpolation results in a valid JSON
 //  o returned result is JSON, so invalid interpolations indicated by returning Json type Neither
 //  o user might not desire the interpolated value to undergo JSON parsing, then
 //    the indication of that is passed via `parse_type` and result returned as a JSON string
 std::bitset<IntpBit::Size> prty;                               // various bits used for interp.

 #define DBG_AND_RET(JSN) \
  { DBG(jit.json(), 4) \
     DOUT(jit.json()) << Debug::btw << "result" << (tmp.front() == *TKN_EMP? " (shortcut)":"") \
                      << ": " << ((JSN).is_neither()? "failed interpolation": \
                                  (JSN).to_string(Jnode::PrettyType::Raw, 1)) << std::endl; \
    return JSN; }

 map_jne *nsp = &ns == &Json::dummy_ns_? &jit.json().ns(): &ns;
 map_jne auto_ns;
 auto_ns.emplace("", STR{TKN_EMP});                             // provide an easy phony JSON
 // empty namespace ("") is used to interpolate {}, {{}} tokens, however, defer providing mapping
 // here - JSON might be huge but such tokens might not even be present in the template, load later
 // only when needed (if tokens {}/{{}} are found)
 auto_ns.emplace(ITRP_GDLM, GET_DLM_(G, *nsp));                 // set ns for $$?
 auto_ns.emplace(ITRP_ADLM, GET_DLM_(A, *nsp));                 // set ns for $#
 auto_ns.emplace(ITRP_PDLM, GET_DLM_(P, *nsp));                 // set ns for $_
 interpolate_path_(tmp, jit, auto_ns);                          // set $path, $PATH if present
 generate_auto_tokens_(tmp, jit, auto_ns);                      // {$a}, {$A}, {$b}, ... if any

 Json rj;                                                       // returned json value
 rj.DBG().severity(NDBG);                                       // suppress debugs in rj
 rj.parse_throwing(false);

 Stringover tmpc(tmp);                                          // preserved copy of tmp
 std::vector<IntpBit> bv{IntpBit::Normal_pass,
                         IntpBit::Attempt_as_array, IntpBit::Stringify_atomics};
 for(auto ibv = bv.begin(); ibv != bv.end(); ++ibv, tmp = tmpc) {
  if(*ibv == IntpBit::Attempt_as_array and not prty[IntpBit::Obj_attempted]) continue;
  prty[*ibv] = true;
  tmp = interpolate_tmp_(tmp, jit, auto_ns, prty);              // interpolate {}, {{}}
  if(tmp.front() == *TKN_EMP) DBG_AND_RET(*jit)                 // performance optimization
  tmp = interpolate_tmp_(tmp, jit, *nsp, prty);                 // interpolate all NS values
  if(tmp.front() == *TKN_EMP) DBG_AND_RET(nsp->at(tmp.substr(1, tmp.size() - 1)).ref())
  tmp = interpolate_jsn_(tmp, jit.json());                      // jsonize/stringify (<<>> / >><<)

  if(parse_type == ParseTrailing::Dont_parse)
   rj.root() = std::move(STR{tmp});
  else {                                                        // json parse resulted template
   Streamstr tis(std::move(tmp));                               // tmp input Streamstr
   if(not rj.parse(tis.begin(), parse_type).parsing_failed() and// if parsing succeeds, however,
      parse_type == ParseTrailing::Relaxed_no_trail and         // relaxed trailing succeeds only
      not tmpc.ends_with(rj.exception_point().str())) continue; // if parsed's tail matches tmp's
  }

  if(not rj.parsing_failed()) {                                 // if successful interpolation
   if(not (jit.is_koj_last(Json::NonEmptyKoj) and               // and last walked step is not
           jit.last_walked_().stripped[0] == SFY(LBL_SPR))      //  <:>k - a label interpolation
      or jit->is_atomic()                                       // request,
      or prty[IntpBit::Interpolate_labels] == true)             // or it's already done - then
    break;                                                      // end interpolation, otherwise -
   prty[IntpBit::Interpolate_labels] = true;                    // ensure correct "phase" for
   --ibv;                                                       // label interpolation request:
   continue;                                                    // either `><w` or `<>a`
  }
 }

 if(rj.parsing_failed()) rj.type(Jnode::Neither);
 // interpolate passes the notion of failure also through type Neither
 DBG_AND_RET(rj)
 #undef DBG_AND_RET
}



void Json::generate_auto_tokens_(Stringover &tmp, Json::iterator &jit, map_jne &ns) {
 #include "dbgflow.hpp"
 // generate auto tokens {$a}, {$b}, {$A}, ... for iterables
 static auto sre = std::regex(R"(\{\$a\})", std::regex::optimize | std::regex::icase);
 static auto mre = std::regex(R"(\{(?:(\$[a-z]{1,3})|(\$[A-Z]{1,3}))\})", std::regex::optimize);
 std::smatch m;

 if(jit->is_atomic()) {
  if(not std::regex_search(static_cast<std::string&>(tmp), m, sre)) return;
  ns["$a"] = *jit;
  if(jit->has_label()) ns["$A"] = jit->label();
  else if(jit->has_index()) ns["$A"] = jit->index();
  return;
 }

 auto to_lower = [](std::string &&s) {
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
  return std::move(s);
 };
 auto to_upper = [](std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c); });
  return s;
 };
 auto str2idx = [](const std::string &tkn) {                    // convert token (a,A,etc) to index
  signed_size_t idx{0};
  for(char c: tkn)
   { if(c == '$') continue;
     idx = idx*('z' - 'a' + 1) + c - (islower(c)? 'a': 'A') + 1; }
  return idx - 1;
 };

 std::set<std::string> uct, lct, sst;                           // upper/lower/super case tokens
 for(std::sregex_iterator it = std::sregex_iterator(static_cast<std::string&>(tmp).begin(),
                                                    static_cast<std::string&>(tmp).end(), mre);
     it != std::sregex_iterator(); ++it) {
  if((*it)[2].length() == 0)
   { lct.insert( (*it)[1] ); sst.insert( std::move((*it)[1]) ); }
  else
   { uct.insert( (*it)[2] ); sst.insert( to_lower((*it)[2]) ); }
 }

 Json w;                                                        // needed for walk()
 w.DBG().severity(NDBG);
 DBG(jit.json(), 4) {                                           // debug print all found tokens
  DOUT(jit.json()) << "found tokens: ";
  std::string dlm;
  for(const auto &t: sst) {
   if(uct.count(to_upper(t))) DOUT(jit.json()) << dlm << to_upper(t); dlm = ", ";
   if(lct.count(t)) DOUT(jit.json()) << dlm << t; dlm = ", ";
  }
  DOUT(jit.json()) << std::endl;
 }

 auto move2json = [&w, &jit](void) { w.root() = std::move(*jit); return true; };
 auto reinstate = [&w, &jit](bool) { *jit = std::move(w.root()); };
 GUARD(move2json, reinstate);

 auto wi = w.walk(sst.empty()?"":"><w:", Json::CacheState::Keep_cache);// walk only if there tokens
 signed_size_t last_idx{0};
 for(const auto &t: sst) {                                      // interpolate found tokens into ns
  auto idx = str2idx(t);
  std::advance(wi, idx - last_idx);
  if(wi == wi.end()) {
   if(wi.type(0) != Jsearch::wide_match) break;
   wi = w.walk("<>a:", Json::CacheState::Keep_cache);           // second: walk all the atomics
   std::advance(wi, idx - SGNS_T(wi.json().root().children()));
   if(wi == wi.end()) break;
  }
  last_idx = idx;
  if(uct.count(to_upper(t))) {
   if(wi->has_label()) ns[to_upper(t)] = STR{wi->label()};
   else if(wi->has_index()) ns[to_upper(t)] = NUM{static_cast<double>(wi->index())};
  }
  if(lct.count(t)) ns[t] = *wi;
 }
}



void Json::interpolate_path_(Stringover &tmp, Json::iterator &jit, map_jne &auto_ns) {
 #include "dbgflow.hpp"
 // facilitate interpolation of {$path, $PATH} namespaces

 std::bitset<IntpBit::Size> prty;
 typedef std::pair<std::string, Jnode> ns_type;
 Jnode jpath{ARY{}};
 auto_ns.emplace(ns_type{ITRP_PSTR, Jnode{}});                  // map "$path" -> {}, for check

 // first interpolate {$path}
 if(not interpolate_tmp_(tmp, jit, auto_ns, prty).empty()) {    // check if {$path} is present
  build_path_(jpath, jit);                                      // build path array [ ... ]
  std::string string_path;
  for(auto &item: jpath) string_path += item.val() + GET_DLM_(P, auto_ns); // stringify from array
  if(not jpath.is_empty())
   for(size_t i = 0; i < GET_DLM_(P, auto_ns).size(); ++i)
    string_path.pop_back();                                     // remove trailing PATH_SPR
  auto_ns[ITRP_PSTR] = std::move(STR{std::move(string_path)});  // incorporate $path into namespace
 }

 // then interpolate {$PATH}
 auto_ns.emplace(ns_type(ITRP_PJSN, Jnode{}));                  // check if {$PATH} is present
 if(not interpolate_tmp_(tmp, jit, auto_ns, prty).empty()) {
  if(jpath.is_empty()) build_path_(jpath, jit);
  auto_ns[ITRP_PJSN] = std::move(jpath);
 }
}



std::string Json::interpolate_tmp_(std::string tmp, Json::iterator &jit, map_jne &ns,
                                   std::bitset<IntpBit::Size> &prty) {
 #include "dbgflow.hpp"
 // - interpolate template (tmp) from the namespace (ns),
 // - avoid recursive interpolation within already interpolated templates
 // - return empty string if no interpolation occurred (tmp was not altered)
 // 3rd parameter behavior (prty[Attempt_as_array], prty[Obj_attempted]):
 //  o if Attempt_as_array is false - process (interpolate) obj as usually, no deviations, but
 //    setup bit Obj_attempted (case 1)
 //  o if Attempt_as_array is true - process OBJECTS as if they were ARRAYS (case 2)

 static std::vector<std::pair<std::string, std::string>> ibv {  // interpolation braces vector
         {std::string(2, ITRP_BRC[0]), std::string(2, ITRP_BRC[1])},    // pair: "{{", "}}"
         {{ITRP_BRC[0]}, {ITRP_BRC[1]}}                                 // pair: "{", "}"
        };
 // this is an interpolation optimization: when tmp is "{{}}" or "{{..}}" then no need
 // going through the entire interpolation (which is quite expensive), instead indicate
 // (via returning the special symbol TKN_EMP + key) that json can be taken directly
 // from the iterator or a namespace and hence skip interpolations and following parsing
 auto opn = tmp.find(ibv.front().ITRP_OPN);                     // find opening "{{"
 if(opn != std::string::npos and tmp.find_first_not_of(" ", 0) == opn) {    // tmb begins with "{{"
  auto cls = tmp.find(ibv.front().ITRP_CLS);                    // find closing "}}"
  if(cls != std::string::npos) {
   std::string key = tmp.substr(opn + 2, cls - opn - 2);        // extract key between {{..}}
   if(key.empty()) { tmp = TKN_EMP; return tmp; }               // key is empty (i.e. {{}})
   auto found = ns.find(key);
   if(found != ns.end() and found->KEY.front() != '$')          // key does not begin with $
    { tmp = TKN_EMP + key; return tmp; }
  }
 }

 vec_range irng;                                                // track interpolated ranges
 std::string u8id;                                              // UTF8_ILL detected?

 for(const auto &br: ibv)                                       // interpolate {{..}}, then {..}
  for(auto & nse: ns) {                                         // go by each Entry in NS
   if(nse.VALUE.ref().is_neither()) continue;                   // erased (with <..>z) NS, skip
   prty[IntpBit::Is_double_brace_notaion] = br.ITRP_OPN.size() == 2;
   Json::interpolate_tmp__(tmp, jit, ns, br.ITRP_OPN + nse.KEY + br.ITRP_CLS,
                           nse.VALUE.ref(), prty, irng, u8id);
  }

 if(irng.empty()) { tmp.clear(); return tmp; }                  // there was no interpolation
 return u8id.empty()? tmp: replace_utf8_ill_(tmp, ns);
}



std::string Json::replace_utf8_ill_(std::string & str, const map_jne &ns) {
 // regex-replace all occurrences of UTF8_ILL and special delimiter char, considering $$? namespace
 std::string dlm = GET_DLM_(G, ns);

 size_t pos = 0;                                                // quote all regex specials in dlm
 while((pos = dlm.find_first_of(REGX_SPCL, pos)) != std::string::npos) {
  dlm.insert(pos, 1, CHR_QUOT);
  pos += 2;
 }

 std::string replacement = UTF8_ILL;
 replacement += UTF8_RRM1 + dlm + UTF8_RRM2 + dlm + UTF8_RRM3 + UTF8_ILL;
 return std::regex_replace(str, std::regex{replacement}, "");
}



void Json::interpolate_tmp__(std::string &tmp, Json::iterator &jit, map_jne &ns, std::string &&nsk,
                             Jnode &nse,  std::bitset<IntpBit::Size> &prty,
                             vec_range &irng, std::string &u8id) {
 // interpolate_tmp_ factorization
 std::string found;                                             // found ns key

 auto intp_obj = [&] {
         prty[IntpBit::Obj_attempted] = true;                   // indicate that obj was attempted
         std::string dlm;
         for(auto const &rec: nse) {                            // strip { .. }
          found += dlm + "\"" + rec.label() + "\": " + rec.to_string(Jnode::PrettyType::Raw,1);
          dlm = ITRP_ASPR;
         }
         if(found.empty()) u8id = found = UTF8_ILL;             // indicate repl. required
        };
 auto intp_arr = [&] (std::string & ns_dlm, Json &jsn) {
       std::string dlm;
       for(const auto &rec: jsn.walk(prty[IntpBit::Stringify_atomics]? "<>a:": "><w:")) {
        std::string avs = prty[IntpBit::Interpolate_labels]?    // avs: array value stringified
                           rec[-1].is_object()? rec.label(): std::to_string(rec.index()):
                           rec.to_string(Jnode::PrettyType::Raw, 1);
        if(prty[IntpBit::Stringify_atomics] and avs.front() == '"')
         avs = avs.substr(1, avs.size()-2);
        found += dlm + avs;
        if(dlm.empty()) dlm = ns_dlm;
       }
       if(found.empty()) u8id = found = UTF8_ILL;               // indicate repl. required
      };

 Json j;                                                        // needed for walk()
 j.DBG().severity(NDBG);
 auto move2json = [&j, &nse](void) { j.root() = std::move(nse); return true; };
 auto reinstate = [&j, &nse](bool) { nse = std::move(j.root()); };

 for(size_t ipos = tmp.find(nsk);                               // go by found ns key (namespaces)
            ipos != std::string::npos;                          // ipos: interpolating position
            ipos = tmp.find(nsk, ipos + found.size()), found.clear()) {
  std::string ns_dlm{ GET_DLM_(A, ns) };                        // ns_dlm is AR/OBJ stringification

  if(nse.is_string() and nse.str() == TKN_EMP)                  // phony json to fill {}/{{}}
   nse = jit->value();                                          // load on demand

  if(prty[IntpBit::Is_double_brace_notaion])                    // i.e. {{..}} interpolation
   found = nse.to_string(Jnode::PrettyType::Raw, 1);            // found: json literal as it is
  else                                                          // {..} interpolation
   switch(nse.type()) {
    case Jnode::Jtype::String:
          found = nse.str();                                    // interpolate w/o "" quotes
          if(found.empty()) u8id = found = UTF8_ILL;            // indicate repl. required
          break;
    case Jnode::Jtype::Object:
          if(not prty[IntpBit::Attempt_as_array])               // case 1, process objects
           { intp_obj(); break; }
          // otherwise - fall thru and process case Jnode::Jtype::Array
    case Jnode::Jtype::Array: {
          GUARD(move2json, reinstate);
          intp_arr(ns_dlm, j);
          break;
         }
    default:
          found = nse.to_string(Jnode::PrettyType::Raw, 1);
   }

  if( any_of(irng.begin(), irng.end(),                          // check if already interpolated
             [ipos](const auto &pos){ return ipos >= pos.ITRP_OPN and ipos < pos.ITRP_CLS;}) )
   continue;                                                    // don't interpolate interpolated

  tmp.replace(ipos, nsk.size(), found);                         // interpolate!
  for(auto &rng: irng)                                          // adjust ranges
   if(ipos < rng.ITRP_OPN)                                      // if new found is before existing
    { rng.ITRP_OPN += found.size() - nsk.size(); rng.ITRP_CLS += found.size() - nsk.size(); }
  irng.emplace_back(ipos, ipos + found.size());                 // record interpolated range
 }
}

#undef DBGBL_REF



std::string Json::interpolate_jsn_(std::string tmp, const Json & jsn) {
 #define DBGBL_REF (jsn)
 #include "dbgflow.hpp"
 #undef DBGBL_REF
 // interpolate jsonization (<>, <<>>) and stringification (><, >><<) template requests
 static std::vector<std::pair<std::string, std::string>> ibv    // interpolation braces vector
  { {std::string(2, ITRP_JSY[0]), std::string(2, ITRP_JSY[1])}, {{ITRP_JSY[0]}, {ITRP_JSY[1]}},
    {std::string(2, ITRP_JSY[1]), std::string(2, ITRP_JSY[0])}, {{ITRP_JSY[1]}, {ITRP_JSY[0]}} };
 typedef std::pair<size_t, std::string> repl;                   // {replaced length, replacing str}
 std::map<size_t, repl> irng;                                   // map: start -> {len, repl_str}
 Json ij;                                                       // interpolated json
 if(jsn.DBG()(0)) ij.DBG().severity(NDBG);

 for(const auto &br: ibv) {                                     // seq: <<..>>, <..>, >>..<<, >..<
  const std::string & skey = br.ITRP_OPN, & ekey = br.ITRP_CLS; // skey: start key, e.g/: '<<'
  for(size_t spos = tmp.find(skey), epos = tmp.find(ekey, spos);
             spos != std::string::npos and epos != std::string::npos;
             spos = tmp.find(skey, epos), epos = tmp.find(ekey, spos)) {
   if(any_of(irng.begin(), irng.end(),
             [spos](const auto &r){ return r.KEY <= spos and
                                           spos <= r.KEY + r.VALUE.first; })) continue;
   // lambda: jsonize found value
   auto jsonize = [&] (void) {
         if(not ij.is_string()) return false;                   // valid JSON but not a string
         try{ ij.parse( jsn.unquote_str(ij.str()) ); }          // now parse embedded JSON
         catch(Json::stdException & e) { return false; }        // not a valid JSON
         irng.emplace(spos, repl{epos - spos + skey.size(),
                                 ij.to_string(Jnode::PrettyType::Raw, 0)});
         return true;
        };

   try{ ij.parse(tmp.substr(spos + skey.size(), epos - spos - skey.size()),
                            ParseTrailing::Strict_trailing); }
   catch(Json::stdException & e) { continue; }                  // not a valid JSON
   switch(skey.front()) {
    case ITRP_JSY[0]:                                           // '<"..">' - jsonize from string
          if(not jsonize()) continue;
          break;
    case ITRP_JSY[1]:                                           // '>..<' - stringify from json
         irng.emplace(spos,
                      repl{epos - spos + skey.size(),
                           "\"" + jsn.inquote_str(ij.to_string(Jnode::PrettyType::Raw,
                                                               Jnode::endl_ == PRINT_PRT?
                                                                1: Jnode::tab_)) + "\""});
         break;
   }
   if(skey.size() == 1)
    if(skey.front() == ITRP_JSY[1] or
       ij.type() AMONG(Jnode::Jtype::Object, Jnode::Jtype::Array, Jnode::Jtype::String))
     { irng[spos].VALUE.erase(0, 1); irng[spos].VALUE.pop_back(); }    // strip JSON: <..>
   DBG(jsn, 5) DOUT(jsn) << (skey.front() == ITRP_JSY[0]? "jsonized: '":"stringified: '")
                         << irng[spos].VALUE << "'" << std::endl;
  }
 }

 signed_size_t offset = 0;
 for(auto &r: irng) {                                           // interpolate everything found
  tmp.replace(offset + r.KEY, r.VALUE.first, r.VALUE.second);
  offset += r.VALUE.second.size() - r.VALUE.first;
 }
 if(irng.empty()) tmp.clear();                                  // optimize return value
 return tmp;
}



size_t Json::utf8_adjusted(size_t start, const std::string &jsrc, size_t end) {
 // return string offset counted in unicode (UTF-8) characters
 size_t adj{0};
 if(end == SIZE_T(-1))
  end = jsrc.size();
 for(size_t i = start; i < end; ++i) {
  char c = jsrc[i];
  if(c >= 0) continue;                                          // non-unicode char
  if((c & 0x40) == 0) continue;                                 // UTF-8 non-initial byte
  c <<= 1;                                                      // must be an initial UTF-8 byte
  while((c & 0x80) > 0) { ++adj; c <<= 1; }                     // count leading bits but 1st one
 }
 return end - adj - start;
}




#undef XSFY
#undef SFY

#undef ARRAY_LMT
#undef WLK_SUCCESS
#undef SIZE_T
#undef SGNS_T
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
#undef CHR_SLDS
#undef STR_TRUE
#undef STR_FALSE
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
#undef QNT_OPN
#undef QNT_CLS
#undef TKN_EMP

#undef JSN_FBDN
#undef JSN_QTD
#undef JSN_TRL

#undef ITRP_BRC
#undef ITRP_JSY
#undef ITRP_OPN
#undef ITRP_CLS
#undef UTF8_ILL
#undef UTF8_RRM1
#undef UTF8_RRM2
#undef UTF8_RRM3

#undef ITRP_ADLM
#undef ITRP_GDLM
#undef ITRP_PDLM
#undef ITRP_ASPR
#undef ITRP_GSPR
#undef ITRP_PSPR
#undef ITRP_PJSN
#undef ITRP_PSTR
#undef GET_DLM_


#undef THROW_EXP
#undef __THROW_EXP_1_ARG__
#undef __THROW_EXP_2_ARG__
#undef __THROW_EXP_3RD_ARG__
#undef __THROW_EXP_CHOOSER__
#undef MAY_RETURN_ON_FAILURE















