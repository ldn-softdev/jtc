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
 *     it will throw 'expected_number_type' exception)
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
 *  Json threes could be compared using '==' and '!=' operators.
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
 *  walk() is a class stateful feature, which let search entire Json and iterate
 *  over immediate children as well as over matches for provided search criteria
 *
 *  a) Walk path:
 *   walk() method accepts a string, which represents a walk path; path is made
 *   of lexemes, which instruct how to traverse Json tree:
 *
 *   1) Offset lexemes: enclosed into square braces '[', ']'; provide various offset
 *      functions. Following notation are possible:
 *      [], [text], [n], [-n], [^n], [+n], where n is an integer
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
 *         upon the next iteration a next child needs to be selected (i.e. 3rd, 4fth
 *        and so on)
 *
 *   2) search lexemes: enclosed into angular braces <>, or ><, instructs to perform
 *      a textual match everywhere under the given json's tree. Following notations
 *      are possible:
 *      <txt>, <txt>n, <txt>+n, <txt>S, <txt>Sn, <txt>S+n, and reverse notations:
 *      >txt<, >txt<n, >txt<+n, >txt<S, >txt<Sn, >txt<S+n,
 *      where: txt - is any text to search for,
 *             S - optional one letter suffix,
 *             n - optional quantifier
 *      - if lexeme is given using <> encasement, then forward search is applied
 *        otherwise (i.e. >< encasement), search is performed only among immediate
 *        children in the given node
 *      "<text>": performs search of the string "text" within json tree under
 *                the given node
 *      - optionally a one letter suffix (either of rlRLdbn) could be used:
 *        which affect search in the following way:
 *        r - apply exact match (default) while searching string values,
 *        R - same as r, but Regex search is applied
 *        l - apply exact match while searching labels only,
 *        L - same as l with Regex search
 *        d - match a number (i.e. searching only numeric json values),
 *        D - same as d with Regex search
 *        b - match a boolean (i.e. searching only boolean values), true/false/any
 *            must be fully spelled, e.g.: "<true>b",
 *        n - match null value (), the content within the encasement could be empty,
 *            or anything - it's ignored, e.g.: "<>n", ">null<n", etc
 *      - optionally a numeric quantifiers could follow search (or suffix):
 *      n - integer quantifier specifying match instance, e.g.: "<text>2"
 *          will match only upon a 3rd (i.e. zero based) encounter of the word
 *           "text"
 *      +n - makes the path iterable: produce a match upon n'th match encounter
 *           and indicates that upon the next iteration a next match instance to
 *           to be selected. e.g: "<text>+1" will match 2nd found instance of "text",
 *           then 3rd, 4fth and so on.
 *      - empty lexemes <> / >< may have either r suffix - matches an empty string,
 *        l - to match empty label, or n suffix, otherwise an exception is thrown
 *
 *   3) An offset lexeme ([...]) could be concatenated with a search lexeme (either
 *      <....> or >...<) over the colon sign (':'), e.g.: [some label]:<some text>
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
 *   Unlike Jnode iterator (which is bidirectional), walk interator (or Json::iterator)
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
 *  c) an optional second parameter for walk() is CacheAction, which defaults to
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
 * Json class is DEBUGGABLE - see dbg.hpp
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
#include <iomanip>              // std::setprecision
#include <initializer_list>
#include <regex>
#include "extensions.hpp"
#include "dbg.hpp"
#include "Outable.hpp"
//#include "Blob.hpp"             // SERDES interface



// Class design notes:
// Jnode represent a single JSON value of any kind (from null to object).
// 1. for storing atomic JSON values (null, bool, string, number) an std:string
//    is used - internally all JSON atomic values are stored like that, type/value
//    validation occurs only during parsing.
// 2. JSON's Arrays and Objects are recurrent structures, which need to be stored
//    in STL containers. Both arrays and objects could be stored using the same
//    container type. Considering all implications, std::map it seems the most
//    suitable container for JSON:
//    - std::map caters random point remove/insert operations efficiently,
//    - label search is O(log(n)) complexity; iteration is not much worse than
//      vector's
//    - for Array storage (whose values do not require JSON labels, map keys will
//      be auto-generated)


#define DBG_WIDTH 80                                            // max print len upon parser's dbg
#define ARRAY_LMT 4                                             // #bytes per array's index
#define KEY first                                               // semantic for map's pair
#define VALUE second                                            // instead of first/second
#define GLAMBDA(FUNC) [this](auto&&... arg) { FUNC(std::forward<decltype(arg)>(arg)...); }

#define PRINT_PRT '\n'                                          // pretty print separator
#define PRINT_RAW ' '                                           // raw print separator
#define CHR_TRUE 'T'                                            // designator for Json 'true' value
#define CHR_FALSE 'F'                                           // designator for Json 'false' val.
#define CHR_NULL '\0'                                           // end of string
#define CHR_RTRN '\n'                                           // end of line
#define STR_TRUE "true"                                         // Json true sting value
#define STR_FALSE "false"                                       // Json false sting value
#define STR_ANY "any"                                           // to match any boolean
#define STR_NULL "null"                                         // Json null sting value
#define JSN_OBJ_OPN '{'                                         // Json open object
#define JSN_OBJ_CLS '}'                                         // Json close object
#define JSN_ARY_OPN '['                                         // Json open array
#define JSN_ARY_CLS ']'                                         // Json close array
#define LXM_OFS_OPN '['                                         // Walk lexeme open offset
#define LXM_OFS_CLS ']'                                         // Walk lexeme close offset
#define LXM_SCH_OPN '<'                                         // Walk lexeme open search
#define LXM_SCH_CLS '>'                                         // Walk lexeme close offset
#define LBL_SPR ':'                                             // Walk lexeme label separator
#define PFX_ITR '+'                                             // prefix of iterable offset
#define PFX_WFR '^'                                             // walk from root offset
#define PFX_WFL '-'                                             // walk from end-leaf offset

#define JSN_FBDN "\b\f\n\r\t"                                   // forbidden JSON chars
#define JSN_QTD "/\"\\bfnrtu"                                   // chrs following quotation in JSON
// NOTE: solidus quotation is optional per JSON spec, a user will have an option
//       to chose between desired quotation behavior

// template meta to calculate number of bits required to represent an integer
template<uint64_t I>
struct bits { enum { num = 1 + bits<(I>>1)>::num }; };
template<>
struct bits<1> { enum { num = 1 }; };





class Json;
class Jnode {
    friend class Json;

  friend std::ostream & operator<<(std::ostream & os, const Jnode & jnode)
                         { int rl{0}; return print_json_(os, jnode, rl); }

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
                type_non_indexable, \
                type_non_subscriptable, \
                type_non_iterable, \
                expected_string_type, \
                expected_number_type, \
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
                \
                /* now define Json exceptions */ \
                unexpected_end_of_string, \
                unexpected_end_of_line, \
                unquoted_character, \
                unexpected_character_escape, \
                expected_valid_label, \
                missing_label_separator, \
                expected_json_value, \
                expected_enumeration, \
                invalid_number, \
                walk_offset_missing_closure, \
                walk_search_label_with_attached_label, \
                walk_expect_search_label, \
                walk_expect_lexeme, \
                walk_wrong_label_seprator_usage, \
                walk_unexpexted_suffix, \
                walk_bad_number, \
                walk_root_non_iterable, \
                walk_bad_suffix, \
                walk_bad_position, \
                walk_a_bug, \
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


                        Jnode(void) = default;                  // DC
                        Jnode(const Jnode &jn): Jnode() {       // CC
                         #ifdef BG_CC
                          auto preserved = jn.is_pretty();
                          if(DBG()(0)) DOUT() << DBG_PROMPT(0) << "CC copying: " << jn.raw() << std::endl;
                          jn.pretty(preserved);
                         #endif
                         auto * volatile jnv = &jn.value();     // when walk iterator is copied its
                         if(jnv == nullptr)                     // supernode is empty, hence chck'n
                          { type_ = jn.type_; return; }         // supernode's type is parent type
                         // why volatile? compiler (mistakenly) believes that address of a returned
                         // reference can never be null, hence optimizes out above 2 lines, which
                         // leads to the crash eventually. "volatile" disables such optimization
                         type_ = jnv->type_;
                         value_ = jnv->value_;
                         descendants_ = jnv->descendants_;
                        }

                        Jnode(Jnode &&jn): Jnode() {            // MC
                         auto * volatile jnv = &jn.value();     // same here: moved jn could be an
                         if(jnv == nullptr)                     // empty supernoe, hence checking
                          { std::swap(type_, jn.type_); return; }
                         swap(*this, jn);                       // swap will resolve supernode
                        }

    Jnode &             operator=(Jnode jn) {                   // CA
                         swap(*this, jn);                       // CC takes care of an empty jn
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

                        template<typename T>
                        operator typename std::enable_if<std::is_floating_point<T>::value, double>
                                             ::type (void) const {
                         if(not is_number()) throw EXP(expected_number_type);
                         return num();
                        }
                        // concept ensures in-lieu application (avoid clashing with string type)

                        template<typename T>
                        operator typename std::enable_if<std::is_same<T, bool>::value, bool>
                                             ::type (void) const {
                         if(not is_bool()) throw EXP(expected_boolean_type);
                         return bul();
                        }
                        // concept ensures in-lieu application (avoid clashing with string type)

                        // class interface:
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

  virtual const Jnode & operator[](long i) const {
                         if(is_atomic()) throw EXP(type_non_indexable);
                         return iterator_by_idx_(i)->VALUE;
                        }

    Jnode &             operator[](const std::string & l) {
                         if(not is_object()) throw EXP(type_non_subscriptable);
                         return children_()[l];
                        }

    const Jnode &       operator[](const std::string & l) const {
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
                         if(not is_number()) throw EXP(expected_number_type);
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
    #ifdef BG_CC
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
  static std::ostream & print_json_(std::ostream & os, const Jnode & me, int & rl);
  static std::ostream & print_iterables_(std::ostream & os, const Jnode & me, int & rl);

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
 // this bidirectional iterator let iterate over children in given JSON iterable
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

    // Super node definition
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
    Iterator &          operator=(Iterator it)                  // CA
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

 if(is_array()) {                                               // array could be addressed directly
  size_t key = stoul(children_().rbegin()->KEY, nullptr, 16);
  if(key < children_().size()) {                                // if so, indices are non-tampered
   std::stringstream ss;
   ss << std::hex << std::setfill('0') << std::setw(ARRAY_LMT * 2) << idx;
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
   ss << std::hex << std::setfill('0') << std::setw(ARRAY_LMT * 2) << idx;
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
 ss << std::hex << std::setfill('0') << std::setw(ARRAY_LMT * 2) << key;

 return ss.str();
}


std::ostream & Jnode::print_json_(std::ostream & os, const Jnode & me, int & rl) {
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
        return os << '"' << my.str() << '"';
  default:
        return os;                                              // ignore unknown type
 }
 return print_iterables_(os, my, rl);
}


std::ostream & Jnode::print_iterables_(std::ostream & os, const Jnode & my, int & rl) {
 // process children in iterables (array or node)
 if(endl_ == PRINT_PRT) ++rl;                                   // if pretty print - adjust level

 for(auto & child: my.children_()) {                            // print all children:
  os << std::string(rl * tab_, ' ');                            // output current indent
  if(not my.is_array())                                         // if parent (me) is not Array
   os << '"' << child.KEY << "\": ";                            // print label
  print_json_(os, child.VALUE, rl)                              // then print child itself and the
   << (child.KEY != my.children_().rbegin()->KEY? ",": "")      // trailing comma if not the last
   << endl_;
 }

 if(rl > 1) os << std::string((rl-1)*tab_, ' ');                // would also signify pretty print
 if(my.is_array()) os << JSN_ARY_CLS;                           // close array, or
 if(my.is_object()) os << JSN_OBJ_CLS;                          // close node (object)
 if(endl_ == PRINT_PRT) --rl;                                   // if pretty print - adjust level

 return os;
}





class Json{
  friend std::ostream & operator<<(std::ostream & os, const Json & my)
                         { return os << my.root(); }

    #define PARSE_THROW \
                may_throw, \
                dont_throw
    ENUM(ParseThrow, PARSE_THROW)

 public:
    #define JS_ENUM \
                regular_match, \
                Regex_search, \
                digital_match, \
                Ditital_regex, \
                boolean_match, \
                null_match, \
                label_match, \
                Label_RE_search, \
                end_of_match, \
                text_offset, \
                generic_offset
    ENUMSTR(Jsearch, JS_ENUM)
    // Jsearch is defined so that its first letter corresponds to the suffix

    #define CACHE_ACTION \
                invalidate, \
                keep_cache
    ENUM(CacheAction, CACHE_ACTION)


                        Json(void) = default;
                        Json(const Jnode &jn): root_{jn.value()} { }
                        Json(Jnode &&jn): root_{std::move(jn.value())} { }
                        Json(const std::string &str) { parse(str); }

    // class interface:
    Jnode &             root(void) { return root_; }
    const Jnode &       root(void) const { return root_; }
    Json &              parse(const std::string & jstr);
    std::string::const_iterator
                        exception_point(void) { return ep_; }
    class iterator;
    iterator            walk(const std::string & walk_string = "", CacheAction = invalidate);

    // relayed Jnode interface
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
    Jnode &             operator[](size_t i) { return root()[i]; }
    const Jnode &       operator[](size_t i) const { return root()[i]; }
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

    // calling clear_cache is required once JSON was modified anyhow; it's called
    // anyway every time new walk is build, thus the end-user must call it only
    // when continue walking iterators (with search iterators) past JSON modification

    //SERDES(root_)                                             // not really needed (so far)
    DEBUGGABLE()
    EXCEPTIONS(Jnode::ThrowReason)

    static Jnode::Jtype json_number_definition(std::string::const_iterator & jsp);

 protected:
    // protected data structures
    Jnode               root_;
    std::string::const_iterator
                        ep_;                                    // exception pointer
    const char *        jsn_fbdn_{JSN_FBDN};                    // JSN_FBDN pointer
    const char *        jsn_qtd_{JSN_QTD};                      // JSN_QTD pointer

 private:
    // jsp: json string pointer
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

    typedef std::map<std::string, Jnode>::iterator iter_jn;
    typedef std::map<std::string, Jnode>::const_iterator const_iter_jn;
    typedef std::vector<std::string> v_str;

    void                compile_walk_(const std::string & wstr, iterator & it) const;
    void                parse_lexemes_(const std::string & wstr, iterator & it) const;
    std::string         parse_offset_(std::string::const_iterator &si, char closec_chr) const;
    void                parse_suffix_(std::string::const_iterator &si, iterator &it, v_str&) const;
    Jsearch             search_suffix_(char sfx) const;
    void                parse_quantifier_(std::string::const_iterator &si, iterator & it) const ;
    long                parse_index_(std::string::const_iterator &si, ParseThrow=may_throw) const;


    // Properties and structures used by walk iterator:

    // WalkStep:
    // - used by walk iterator and search cache key
    struct WalkStep {
        // WalkStep contains data describing one walk lexeme (subscript or search)
        // walk path is made of walk steps instructing how JSON tree to be traversed

                            WalkStep(void) = delete;
                            WalkStep(std::string && l, Jsearch js): // enable emplacement
                             lexeme(std::move(l)), jsearch(js) {}

        const char *        search_type() const                 // only for COUTABLE
                             { return ENUMS(Jsearch, jsearch); }
        const std::string   label() const                       // only for COUTABLE
                             { return stripped.size()==2? stripped.back(): "-"; }
        bool                operator<(const WalkStep &r) const {// only for SearchCacheKey
                             return stripped.back() != r.stripped.back()?
                                    stripped.back() < r.stripped.back():
                                    (jsearch != r.jsearch?
                                     jsearch < r.jsearch:
                                     stripped.front() < r.stripped.front());
                            }

        std::string         lexeme;                             // lexeme w/o suffix and quantifier
        Jsearch             jsearch;                            // Jsearch type
        long                offset{0};                          // current offset
        long                init{-1};                           // initial offset value
        // init -1: init is not used. offset can be positive or negative, e.g.: [-2], [3]
        // init -2: offset is used as absolute selector from root, like in [^n]
        // init >=0: offset is used as an iterable (e.g.: [+0]]), init keeps original offset
        v_str               stripped;
                            // stripped[0] -> a stripped lexeme (required)
                            // stripped[1] -> attached label match (optional)
        std::regex          re;

        COUTABLE(WalkStep, offset, init, search_type(), label(), lexeme)
    };

    // Itr (iterator-label pair):
    // - used by walk iterator (path_vector) and search cache key
    struct Itr {
        // path-vector is made of Itr - result of walking WalkStep vector (walk path)
        // last Itr in path-vector points to the found JSON element (via jit)
        // lbl keeps a copy of jit's key: this is required for validation - jit could
        // be invalidated due to JSON manipulation, preserved lbl ensures safe execution
        // of is_nested() and is_valid() methods
        // wsi (filled only when path is terminated with end() - out of iterations/non
        // iterable) used in increment_() facilitating walk path iterations

                            Itr(void) = default;                // for pv_.resize()
                            Itr(const iter_jn &it, const std::string &l, size_t i = 0):
                             jit(it), lbl(l), wsi(i) {}         // enable emplacement

        iter_jn             jit;                                // iterator pointing to JSON
        std::string         lbl;                                // preserved label for validation
        size_t              wsi;                                // walk step index (for increments)
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
        static bool 	    cmp(const SearchCacheKey &l, const SearchCacheKey &r)
                             { return l.jnp != r.jnp? l.jnp<r.jnp: l.ws<r.ws; }

        const Jnode *       jnp;
        WalkStep            ws;

        COUTABLE(SearchCacheKey, json_node(), ws)
    };

    typedef std::vector<Itr> path_vector;                       // used by iterator

    // parse_offset_type_() is dependent on WalkStep definition, hence moved down here
    void                parse_offset_type_(WalkStep & state) const;

 public:
    // Json::iterator: needs to be defined in-class to facilitate container storage
    // with the iterator (e.g. iterator-based callback)
    //
    class iterator: public std::iterator<std::forward_iterator_tag, Jnode> {
     // this forward iterator let iterate over *iterable* walk paths
     // once iterator is dereferenced it returns a reference to a Jnode's super-node,
     // super-node adds following powers to regular Jnode's:
     // - label() allows accessing node's label if node is nested by object
     // - index() allows accessing node's ordinal index if nested by array
     // - value() provides access to the Jnode itself (but this methods is quite
     //   redundant)
     // - is_root() let checking if the node is root() or not
     // - operator[] receives new capability: on a negative index it will address
     //              indexed levels up in the JSON's tree hierarchy (e.g." [-1] will
     //              address a parent of the dereferenced node, and so on)
        typedef signed long s_long;
        friend Json;
        friend void         swap(Json::iterator &l, Json::iterator &r) {
                             using std::swap;                   // enable ADL
                             swap(l.ws_, r.ws_);
                             swap(l.jp_, r.jp_);
                             swap(l.pv_, r.pv_);
                             swap(l.sn_.parent_type(), r.sn_.parent_type());// supernode requires
                            }                                   // swapping of type_ values only
        // Super node definition
        //
        class SuperJnode: public Jnode {
            friend Json::iterator;

         public:
            bool                has_label(void) const {
                                 return lbl_ != &Json::iterator::empty_ and
                                        parent_type() == Object;
                                }
            const std::string & label(void) const {
                                 if(type_ != Object)
                                  throw EXP(label_request_for_non_object_enclosed);
                                 return *lbl_;
                                }
            bool                has_index(void) const {
                                 return lbl_ != &Json::iterator::empty_ and
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
                                 // in addition to Json::iterator's, this one adds capability
                                 // to address supernode with negative index, e.g: [-1],
                                 // like in the walk-string to reference a parent
                                 if(i >= 0) return Jnode::operator[](i);
                                 if(-i >= static_cast<s_long>(jit_->pv_.size()))
                                  return jit_->json_().root();
                                 return jit_->pv_[jit_->pv_.size() + i -1].jit->VALUE;
                                }
            const Jnode &       operator[](long i) const {
                                 if(i >= 0) return Jnode::operator[](i);
                                 if(-i >= static_cast<s_long>(jit_->pv_.size()))
                                  return jit_->json_().root();
                                 return jit_->pv_[jit_->pv_.size() + i -1].jit->VALUE;
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
            SuperJnode &        operator()(Jnode &jn, Json::iterator * jit)
                                 { jnp_ = &jn; jit_ = jit; return *this; }

            const std::string * lbl_{&Json::iterator::empty_};  // lbl_ should never be nullptr
            Jnode *             jnp_{nullptr};                  // iterator's Jnode pointer
            Json::iterator *    jit_{nullptr};                  // back to iterator, for [-n]
        };
        //
        // end of Super node definition

     public:
                            iterator(void) = default;           // DC
                            iterator(const iterator &it):       // CC
                             ws_(it.ws_), jp_(it.jp_), pv_(it.pv_) {
                             sn_.type_ = it.sn_.type_;
                            }
                            iterator(iterator &&it) {           // MC
                             swap(*this, it);
                            }
        iterator &          operator=(iterator it) {            // CA
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
                             return pv_.empty()?
                              sn_(jp_->root(), this):
                              sn_(pv_.back().jit->KEY, pv_.back().jit->VALUE, this);
                            }
        Jnode *             operator->(void) {
                             return pv_.empty()?
                              &sn_(jp_->root(), this):
                              &sn_(pv_.back().jit->KEY, pv_.back().jit->VALUE, this);
                            }
        iterator &          operator++(void) { incremented(); return *this; }
        iterator &          begin(void) { return *this; }
        iterator            end(void) const {
                             if(not jp_->root().is_iterable())
                              throw jp_->EXP(Jnode::walk_root_non_iterable);
                             return iterator(jp_->root().children_().end());
                            }
        bool                is_valid(void) const {
                             // end iterator is considered invalid path
                             if(pv_.empty()) return true;
                             if(pv_.back().jit == jp_->root().children_().end()) return false;
                             return is_valid_(jp_->root(), 0);
                            }
        bool                is_nested(iterator & it) const;
        bool                incremented(void);
        size_t              walk_size(void) const { return ws_.size(); }
        long                counter(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].init >= 0? ws_[position].offset: -1;
                            }
        long                instance(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].init == -1? ws_[position].offset: -1;
                            }
        long                offset(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].offset;
                            }
        const std::string & lexeme(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].lexeme;
                            }
        Jsearch             type(size_t position) const {
                             if(position >= ws_.size()) throw jp_->EXP(Jnode::walk_bad_position);
                             return ws_[position].jsearch;
                            }

     protected:
                            iterator(Json *ptr): jp_{ptr} {};
                            iterator(const iter_jn & it) { pv_.emplace_back(it, ""); }

      std::vector<WalkStep> ws_;                                // walk state vector (walk path)
        Json *              jp_;                                // json pointer (for json().end())
        path_vector         pv_;                                // path_vector (result of walking)
        SuperJnode          sn_{Jnode::Neither};                // super node's type_ holds parent's

     private:
        bool                is_valid_(Jnode & jnp, size_t idx) const;
        auto &              walk_path_(void) { return ws_; }
        const auto &        walk_path_(void) const { return ws_; }
        Json &              json_(void) const { return *jp_; }
        auto &              cache_(void) const { return json_().sc_; }
        auto &              cnt_type_(void) { return sn_.type_; }   // original container type

        Json::iterator &    walk_(void);
        void                walk_step_(size_t wsi, Jnode *);
        void                show_built_pv_(std::ostream &out) const;
        void                walk_numeric_offset_(size_t wsi, Jnode *);
        void                walk_text_offset_(size_t wsi, Jnode *);
        void                walk_search_(size_t wsi, Jnode *);
        void                search_all_(Jnode *, const char *, const
                                        WalkStep &w, std::vector<path_vector> &);
        bool                search_successful_(Jnode *, const char *lbl, const WalkStep &, long &);
        void                lbl_callback_(const std::string &lbl, const Jnode *,
                                          const std::vector<path_vector> * = nullptr);
        void                itr_callback_(const Jnode *);
        bool                child_found_(Jnode *, const WalkStep &, long &);
        bool                label_matched_(const std::string &, const WalkStep &, long &) const;
        bool                atomic_matched_(const Jnode *, const char *l, const WalkStep &) const;
        bool                string_match_(const Jnode *, const char *lbl, const WalkStep &) const;
        bool                bull_matched_(const Jnode *, const char *lbl, const WalkStep &) const;
        bool                increment_(long l);
        long                next_iterable_ws(long idx) const;

        static std::string  empty_;
    };

 private:

    // data structures (cache storage and callbacks)
    // cache:
    std::map<SearchCacheKey, std::vector<path_vector>, decltype(&SearchCacheKey::cmp)>
                        sc_{SearchCacheKey::cmp};           // search cache itself
                        // search cache is the array of all path_vector's for given
                        // search key (combination of jnode and walk step)

    // There are 2 types of callbacks:
    // 1. callback hooked onto the JSON labels (lbl_callback_map)
    // 2. callback hooked onto Json::iterators (itr_callback_vec)
    struct ItrCallback {
     // this class facilitates itr_callback_vec type storage, binding Json::iterator
     // and callback itself together
                            ItrCallback(iterator &&itr, std::function<void(const Jnode &)> &&cb):
                             iter{std::move(itr)}, callback(std::move(cb))
                              {}                            // facilitate emplacement for vector
        iterator            iter;
        std::function<void(const Jnode &)>
                            callback;
    };
    typedef std::map<std::string, std::function<void(const Jnode &)>> lbl_callback_map;
    typedef std::vector<ItrCallback> itr_callback_vec;

    lbl_callback_map    lcb_;                               // label callbacks storage
    itr_callback_vec    icb_;                               // iterator-based callback storage
    bool                ce_{false};                         // callbacks engaged? flag

 public:

    #define CALLBACK_TYPE \
                label_based, \
                iterator_based, \
                status
    ENUM(CallbackType, CALLBACK_TYPE)

    // user callbacks interface:
    bool                callbacks_engaged(CallbackType ct = status) const {
                         if(ct == label_based) return ce_ and not lcb_.empty();
                         if(ct == iterator_based) return ce_ and not icb_.empty();
                         return ce_;
                        }
    Json &              engage_callbacks(bool x=true)           // engage/disengage all callbacks
                         { ce_ = x; return *this; };
    Json &              callback(const std::string &lbl,        // plug-in label-callback
                                 std::function<void(const Jnode &)> &&cb) {
                         lcb_.emplace(std::move(lbl), std::move(cb));
                         return *this;
                        }
    Json &              callback(iterator itr,                  // plug-in iter-callback
                                 std::function<void(const Jnode &)> &&cb) {
                         if(itr != itr.end())                   // don't plug ended iterators
                          icb_.emplace_back(std::move(itr), std::move(cb));
                         return *this;
                        }
    Json &              clear_callbacks(void)
                         { lcb_.clear(); icb_.clear(); return *this; }
    lbl_callback_map &  lbl_callbacks(void) { return lcb_; }    // access to labeled callbacks
    itr_callback_vec &  itr_callbacks(void) { return icb_; }    // access to iterator callbacks
};

// class static definitions
STRINGIFY(Json::Jsearch, JS_ENUM)

#undef JS_ENUM
#undef CACHE_ACTION
#undef CALLBACK_TYPE
#undef PARSE_THROW



// Jnode methods requiring Json definition:
Jnode::Jnode(Json j)                                            // type conversions Json -> Jnode
 { swap(*this, j.root()); }



// Json definitions:
Json operator ""_json(const char *c_str, std::size_t len) {
 // raw string parsing
 Json x;
 return x.parse(c_str);
}


Json & Json::parse(const std::string & jstr) {
 // parse input string. this is a wrapper for parse_(), where actual parsing occurs
 root() = OBJ{};

 auto jsp = jstr.begin();                                       // json string pointer
 parse_(root_, jsp);

 if(root_.type() == Jnode::Neither)
  { ep_ = jsp; throw EXP(Jnode::expected_json_value); }

 return *this;
}


void Json::parse_(Jnode & node, std::string::const_iterator &jsp) {
 // parse JSON from string
 skip_blanks_(jsp);
 node.type_ = classify_jnode_(jsp);

 DBG(4) {                                                       // print currently parsed point
   static const char* pfx{"parsing point ->"};
   bool truncate = std::strlen(&*jsp) > (DBG_WIDTH-sizeof(pfx));
   std::string str {jsp, jsp + (truncate? DBG_WIDTH - sizeof(pfx) - 3: strlen(&*jsp))};
   for(auto &c: str)
    if(c == CHR_RTRN) c = '|';
   DOUT() << pfx << str << (truncate? "...":"") << std::endl;
 }

 if(node.type_ == Jnode::Neither) return;
 DBG(4) DOUT() << "classified as: " << ENUMS(Jnode::Jtype, node.type()) << std::endl;

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
 auto sp = jsp;                                                 // copy, for work-around
 auto ep = find_delimiter_('"', jsp);
 node.value_ = std::string{sp, ep};                             // work around gnu's compiler bug
 //node.value_ = std::string{jsp, find_delimiter_('"', jsp)};   // this is broken in linux gnu c++
 ++jsp;
}


void Json::parse_number_(Jnode & node, std::string::const_iterator &jsp) {
 // parse number, as per JSON number definition
 auto sp = jsp;                                                 // copy, for work-around
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
   if(*jsp == ',')
    if(not comma_read and node.has_children())
     { ++jsp; comma_read = true; continue; }                    // interleaving comma: .. 2, 3, ..
   // here: either a double comma: " ... ,,", or leading comma: "[ , ...
   ep_ = jsp; throw EXP(Jnode::expected_json_value);            // e.g.: "[ , ...", or "[ 123,, ]"
  }
  if(not comma_read and node.has_children())                    // e.g.: [ "abc" 3.14 ]
   { ep_ = jsp; throw EXP(Jnode::expected_enumeration); }

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
    if(*jsp == ',')
     if(not comma_read and node.has_children())
      { ++jsp; comma_read = true; continue; }                   // interleaving comma
   }
   ep_ = lsp; throw EXP(Jnode::expected_valid_label);
  }

  if(skip_blanks_(jsp) != ':')                                  // label was read, expecting ':'
   { ep_ = jsp; throw EXP(Jnode::missing_label_separator); }

  Jnode child;
  parse_(child, ++jsp);
  if(child.type() == Jnode::Neither)                            // after 'label:' there must follow
   { ep_ = jsp; throw EXP(Jnode::expected_json_value); }        // a valid JSON value

  if(not comma_read and node.has_children())                    // e.g.: [ "abc" 3.14 ]
   { ep_ = jsp; throw EXP(Jnode::expected_enumeration); }

  node.children_().emplace(std::move(label.str()), std::move(child));
  comma_read = false;
 }
}


std::string::const_iterator & Json::find_delimiter_(char c, std::string::const_iterator & jsp) {
 // find next occurrence of character (actually it's used only to find `"')
 while(*jsp != c) {
  if(*jsp AMONG(CHR_NULL, CHR_RTRN))                            // JSON string does not support
   { ep_ = jsp; throw EXP(Jnode::unexpected_end_of_line); }     // multiline, hence throwing
  if(strchr(jsn_fbdn_, *jsp) != nullptr)                        // i.e. found illegal JSON control
   { ep_ = jsp; throw EXP(Jnode::unquoted_character); }

  if(*jsp == '\\') {
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
 if(*jsp == '-') ++jsp;
 if(not isdigit(*jsp)) return Jnode::Neither;                   // digit must follow '-'
 if(*jsp > '0')
  while(isdigit(*jsp)) ++jsp;
 else                                                           // next could be only [.eE] or end
  ++jsp;                                                        // skip leading 0
 // here it could be either of [.eE] or end
 if(*jsp == '.') {
  if(not isdigit(*++jsp)) return Jnode::Neither;                // digit must follow '.'
  while(isdigit(*jsp)) ++jsp;
 }
 // here could be [eE] or end
 if(*jsp AMONG('e', 'E')) {
  ++jsp;                                                        // skip [eE]
  if(*jsp AMONG('+', '-')) ++jsp;                               // skip [+-]
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
 if(*jsp == '"') return Jnode::String;
 if(isdigit(*jsp) or (*jsp == '-' and isdigit(*(jsp+1)))) return Jnode::Number;

 const char * str = &*jsp;
 if(std::strncmp(str, STR_TRUE, sizeof(STR_TRUE)-1) == 0) return Jnode::Bool;
 if(std::strncmp(str, STR_FALSE, sizeof(STR_FALSE)-1) == 0) return Jnode::Bool;
 if(std::strncmp(str, STR_NULL, sizeof(STR_NULL)-1) == 0) return Jnode::Null;
 return Jnode::Neither;
}


char Json::skip_blanks_(std::string::const_iterator & jsp) {
 // skip_blanks_() sets pointer to the first a non-blank character
 while(*jsp <= ' ') {
  if(*jsp == CHR_NULL)
   { ep_ = jsp; throw EXP(Jnode::unexpected_end_of_string); }
  ++jsp;
 }
 return *jsp;
}



// Walk path is a stateful feature. The path is a string, always refers from
// the root and made of lexemes
//
// Walk path design notes:
// 1. offset lexemes:
//    [], [n], [text], [-n], [^n], [+n], where n is an integer
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
// 2. search lexemes:
//    <text>S+n
//    - <text>: perform search of the entire node to match the text
//    - <...> perform forward search, >...< perform search among immediate children
//      only
//    - optional suffix S could be either of [rlRLdb]:
//       r - default, performs exact match of the string,
//       R - expression in brackets is treated as RE and RE search is applied
//       l - performs exact match of the label
//       L - same as R but RE search is applied on labels
//       d - matches digital JSONs only
//       b - matches boolean JSONs only (value must be fully spelled, e.g.: <true>b)
//       n - finds null values (value in brackets is ignored, could be empty).
//    - n optional quantifier (must take the last position):
//       n - search find' n'th (zero based) match instance
//      +n - indicates that the path is iterable (starting from n'th match)
//    - empty lexeme "<>" may have either r, l, or n suffixes (otherwise exception
//      is thrown)
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


std::string Json::iterator::empty_;


Json::iterator Json::walk(const std::string & wstr, CacheAction action) {
 // return Json::iterator to the first element pointed by the walk string.
 // otherwise end-iterator returned
 DBG(0) DOUT() << "walk string: '" << wstr << "'" << std::endl;

 iterator it = this, &itr = it;                                 // itr - for return by reference
 compile_walk_(wstr, it);
 it.pv_.reserve(it.walk_path_().size());                        // iter's performance optimization

 DBG(0) {
  DOUT() << "dump parsed lexemes:" << std::endl;
  for(auto &v: it.walk_path_()) DOUT() << DBG_PROMPT(0) << v << std::endl;
 }

 if(action == invalidate) {                                     // talking a conservative approach:
  clear_cache();                                                // user must specify "keep" himself
  DBG(0) DOUT() << "invalidated search cache" << std::endl;
 }

 it.walk_();
 if(it.pv_.empty()) return itr;                                 // must resolve reference
 if(it.pv_.back().jit != root().children_().end()) return itr;  // must resolve reference
 // here walk_() returned end(), but it might not be a true end, an increment step is required
 // in order to confirm that it's either a true end; return then incremented reference
 it.pv_.clear();                                                // here walk_() returned end(), so
 return ++it;                                                   // advance to next, or true end
}


void Json::compile_walk_(const std::string & wstr, iterator & it) const {
 // parse walk string and compile all all parts for ws_;
 parse_lexemes_(wstr, it);
 std::for_each(it.walk_path_().begin(), it.walk_path_().end(), GLAMBDA(parse_offset_type_) );
}


void Json::parse_lexemes_(const std::string & wstr, iterator & it) const {
 // parse full lexemes: offsets/search lexemes + possible suffixes in search lexemes
 auto & ws = it.walk_path_();
 v_str req_label;                                               // would hold stripped [label]:

 for(auto si = wstr.begin(); si != wstr.end();) {               // si: input string iterator
  DBG(1) {
   DOUT() << "walked string: " << wstr << std::endl;
   DOUT() << DBG_PROMPT(1) << "parsing here: "
          << std::string(si-wstr.begin(), '-') << ">|" << std::endl;
  }
  switch(*si) {
   case ' ': ++si; continue;                                    // space in between lexemes allowed
   case LXM_OFS_OPN:                                            // offset opening: '['
        if(not req_label.empty())                               // only search <...> allowed
         throw EXP(Jnode::walk_expect_search_label);            // after [label]:
        ws.emplace_back(parse_offset_(si, LXM_OFS_CLS), generic_offset);
        break;
   case LXM_SCH_OPN:                                            // opening search fwd '<'
        ws.emplace_back(parse_offset_(si, LXM_SCH_CLS), regular_match);
        break;
   case LXM_SCH_CLS:                                            // opening immediate search '>'
        ws.emplace_back(parse_offset_(si, LXM_SCH_OPN), regular_match);
        break;
   default:
        parse_suffix_(si, it, req_label);                       // outside of offset there could be
        if(req_label.empty()) parse_quantifier_(si, it);        // only suffixes and quantifiers
        continue;
  }
  // store a stripped offset lexeme here (after parsing one)
  ws.back().stripped.push_back(ws.back().lexeme.substr(1, ws.back().lexeme.size() - 2));
  if(not req_label.empty()) {
   ws.back().stripped.push_back( std::move(req_label.front()) );
   req_label.clear();
  }
 }
}


std::string Json::parse_offset_(std::string::const_iterator &si, char close_chr) const {
 // offset lexemes: [], [n], [+n], [-n], [any text, possibly with \]]
 // search lexemes w/o suffix: <>, <text<\>>
 auto begin = si;
 while(*si != close_chr) {                                      // advance till closing bracket
  if(*si == CHR_NULL)
   throw EXP(Jnode::walk_offset_missing_closure);
  if(*si == '\\')
   if(*++si != close_chr) --si;
  ++si;
 }

 std::string lexeme{begin, ++si};                               // lexeme with encasement
 std::string quoted{"\\"};                                      // replace quoted closing bracket
 quoted += close_chr;
 for(size_t p = lexeme.find(quoted); p != std::string::npos; p = lexeme.find(quoted))
  lexeme.replace(p, 2, 1, close_chr);

 DBG(1) DOUT() << "parsed lexeme: " << lexeme << std::endl;
 return lexeme;
}


void Json::parse_suffix_(std::string::const_iterator &si, iterator & it, v_str &req_label) const {
 // parse suffix following a text offset, e.g.: ...>r, ...>d, ...>L and suffix ':' in [..]:<...>
 if(it.walk_path_().empty())                                    // expect some elements already in
  throw EXP(Jnode::walk_expect_lexeme);                         // walks may start with lexeme only

 if(*si == LBL_SPR) {                                           // it's separator (':')
  if(not req_label.empty() or it.walk_path_().back().jsearch != generic_offset)
   throw EXP(Jnode::walk_wrong_label_seprator_usage);
  req_label.push_back( std::move(it.walk_path_().back().stripped.back()) );
  it.walk_path_().pop_back();                                   // label belongs to the next lexeme
  ++si;
  return;
 }

 auto & last_step = it.walk_path_().back();
 if(last_step.lexeme.front() == LXM_OFS_OPN)                    // i.e. it's '['
  throw EXP(Jnode::walk_unexpexted_suffix);                     // suffix may only follow search

 Jsearch sfx = search_suffix_(*si);                             // see if a search suffix valid
 if(sfx < end_of_match) {
  last_step.jsearch = sfx;
  if(sfx AMONG(Regex_search, Label_RE_search, Ditital_regex))
   last_step.re = last_step.stripped.front();                   // it's a RE, book RE then
  DBG(1) DOUT() << "search type sfx: " << ENUMS(Jsearch, sfx) << std::endl;

  if(last_step.stripped.front().empty())                        // i.e. search is "<>Sfx"
   if( not(last_step.jsearch AMONG(regular_match, label_match, null_match)) )
    throw EXP(Jnode::walk_bad_suffix);
  ++si;
 }
}


Json::Jsearch Json::search_suffix_(char sfx) const {
 // Jsearch is defined so that its first letter corresponds to the suffix
 for(int i = regular_match; i < end_of_match; ++i)
  if(sfx == ENUMS(Jsearch, i)[0])
   return static_cast<Jsearch>(i);
 return end_of_match;                                           // when no match found
}


void Json::parse_quantifier_(std::string::const_iterator &si, iterator & it) const {
 auto & lastStep = it.walk_path_().back();
 if(*si == '+' or isdigit(*si)) {                               // must be a quantifier
  auto front = *si;
  lastStep.offset = parse_index_(si);
  if(front == '+')
   lastStep.init = lastStep.offset;
  DBG(1) DOUT() << "quantifier: " << lastStep.offset << std::endl;
 }

 while(*si == ' ') ++si;
 if(not(*si AMONG(LXM_OFS_OPN, LXM_SCH_OPN, LXM_SCH_CLS, CHR_NULL)))
  throw EXP(Jnode::walk_expect_lexeme);
}


long Json::parse_index_(std::string::const_iterator &si, ParseThrow throwing) const {
 // validate if iterator points to a valid number
 char *endptr;
 const char * str{&*si};

 long idx{ strtol(str, &endptr, 10) };
 if(endptr == str) {
  if(throwing == may_throw) throw EXP(Jnode::walk_bad_number);
  else return 0;
 }
 si += endptr - str;
 return idx;
}


void Json::parse_offset_type_(WalkStep & ws) const {
 // parse offset types: separates numerical offsets (e.g. [1]) from text offsets (e.g. [ 1])
 DBG(1) DOUT() << "step: " << ws << std::endl;

 if(ws.lexeme.front() != LXM_OFS_OPN) {                         // it's search: either '<' or '>'
  if(ws.stripped.size() == 2)                                   // there's an attached label offset
   if(ws.jsearch AMONG(label_match, Label_RE_search))           // label search type cannot have
    throw EXP(Jnode::walk_search_label_with_attached_label);    // attached label offset
  return;
 }

 auto si = ++ws.lexeme.cbegin();
 char pfx = '\0';                                               // collect prefix '-','+', or '^'
 if(*si AMONG(PFX_ITR, PFX_WFR, PFX_WFL))
  pfx = *(si++);
 if(not isdigit(*si))
  { ws.jsearch = text_offset; return; };                        // it's a text lexeme

 long idx = parse_index_(si, dont_throw);                       // parse number, but don't throw
 if(*si != LXM_OFS_CLS)
  { ws.jsearch = text_offset; return; }                         // parsing failed - it's a text
 // PFX behavior explained:
 // PFX_ITR: init = offset; // offset >= 0
 // PFX_WFL: init = -1; // offset < 0
 // PFX_WFR: init = -2 ; // offset >= 0;
 ws.offset = pfx == PFX_WFL? -idx: idx;                         // either [-n] or [n]
 if(pfx == PFX_ITR) ws.init = ws.offset;                        // [+n]
 if(pfx == PFX_WFR) ws.init = -2;                               // e.g. [^1] - walk from root
}



//
//
// Json::iterator methods
//
//


bool Json::iterator::is_nested(iterator & it) const {
 // check if we're nesting iterator. end() iterator considered to be non-nested
 if(pv_.empty()) return true;                                   // root always nest
 if(pv_.back().jit == jp_->root().children_().end())
  return false;
 for(size_t i = 0; i<pv_.size() and i<it.pv_.size(); ++i)
  if(pv_[i].lbl != it.pv_[i].lbl)
   return false;
 return true;
}


bool Json::iterator::is_valid_(Jnode & jn, size_t idx) const {
 // check if all labels in path-vector are present
 if(idx >= pv_.size())
  return true;
 auto it = jn.children_().find(pv_[idx].lbl);
 if(it != jn.children_().end())
  return is_valid_(it->VALUE, idx+1);
 return false;
}


Json::iterator & Json::iterator::walk_(void) {
 // walk 'ws' structure from the root building a path vector
 // empty pv_ addresses json.root().
 Jnode * jnp = & json_().root();
 pv_.clear();                                                   // path-vector being built

 for(size_t i = 0; i < ws_.size(); ++i) {
  walk_step_(i, jnp);                                           // walkStep builds up a path-vector
  if(pv_.empty())
   jnp = & json_().root();
  else {
   if(pv_.back().jit == json_().root().children_().end())
    { DBG(json_(), 2) show_built_pv_(DOUT(json_())); return *this; }
   jnp = &pv_.back().jit->VALUE;
  }
  DBG(json_(), 2) show_built_pv_(DOUT(json_()));
 }

 if(pv_.empty())
  cnt_type_() = jp_->root().type();
 else
  if(pv_.back().jit != json_().root().children_().end())
   cnt_type_() = pv_.size()>1? pv_[pv_.size()-2].jit->VALUE.type(): jp_->type();
 DBG(json_(), 2) DOUT(json_()) << "finished walking" << std::endl;
 return *this;
}


void Json::iterator::show_built_pv_(std::ostream &out) const {
 out << "built path vector:";
 for(auto &it: pv_)
  out << (&it == &pv_.front()? " ":"-> ")
      << (it.jit == json_().root().children_().end()? "(end)": it.lbl);
 out << std::endl;
}


void Json::iterator::walk_step_(size_t idx, Jnode *jn) {
 DBG(json_(), 3) DOUT(json_()) << "walking step: [" << idx << "]" << std::endl;
 // walk a single lexeme
 switch(ws_[idx].jsearch) {
  case generic_offset:
        return walk_numeric_offset_(idx, jn);
  case text_offset:
        return walk_text_offset_(idx, jn);
  default:
        return walk_search_(idx, jn);
 }
}


void Json::iterator::walk_numeric_offset_(size_t idx, Jnode *jn) {
 // walk a numerical offset, e.g.: [3], [+0], [-1]
 auto &ws = ws_[idx];

 if(ws.init == -2) {                                            // e.g.: [^2]
  pv_.resize(ws.offset > static_cast<s_long>(pv_.size())? pv_.size(): ws.offset);// shrinking path
  return;
 }

 if(ws.offset >= 0) {                                           // [0], [+1], etc
  if(ws.offset >= static_cast<s_long>(jn->children_().size()))  // address beyond children's size
   { pv_.emplace_back(json_().root().children_().end(), "", idx); return; }
  auto it = jn->iterator_by_idx_(ws.offset);
  pv_.emplace_back(it, it->KEY);
  return;
 }

 // negative offset, ws.offset < 0, e.g.: [-2]
 pv_.resize(-ws.offset <= static_cast<s_long>(pv_.size())? pv_.size() + ws.offset: 0);
}


void Json::iterator::walk_text_offset_(size_t idx, Jnode *jn) {
 // walk a text offset, e.g.: [label]
 auto &ws = ws_[idx];
 auto it = jn->children_().find(ws.stripped.front());           // see if label exist
 if(it == jn->children_().end())
  pv_.emplace_back(json_().root().children_().end(), "", idx);
 else
  pv_.emplace_back(it, it->KEY);                                // if so, add to the path-vector
}


void Json::iterator::walk_search_(size_t idx, Jnode *jn) {
 // if search is iterable, build a cache (if not yet), otherwise do a one-time search
 auto &ws = ws_[idx];
 auto i = ws.offset;                                            // needed stateful, for recursion

 if(ws.lexeme.front() == LXM_SCH_CLS) {                         // '>': search immediate children
  if(not child_found_(jn, ws, i))
   pv_.emplace_back(json_().root().children_().end(), "", idx);
  return;
 }

 if(ws.init < 0) {                                              // non-iterable, find once
  if(not search_successful_(jn, nullptr, ws, i))
   pv_.emplace_back(json_().root().children_().end(), "", idx);
  return;
 }

 auto & cache = cache_();                                       // search is iterable: build cache
 SearchCacheKey skey{jn, ws};
 if(cache.count(skey) == 0) {                                   // cache does not exits
  DBG(json_(), 1) DOUT(json_()) << skey << std::endl;
  cache[skey].resize(1);                                        // prepare place for 1st pathvector
  search_all_(jn, nullptr, ws, cache[skey]);                    // build cache
  cache[skey].pop_back();                                       // last entry is redundant
  DBG(json_(), 1) DOUT(json_()) << "cached " << cache[skey].size() << " searches" << std::endl;
 }

 if(i >= static_cast<s_long>(cache[skey].size()))               // offset outside of cache:
  pv_.emplace_back(json_().root().children_().end(), "", idx);  // return end() iterator
 else                                                           // otherwise augment the path
  for(auto &path: cache[skey][i])
   pv_.push_back(path);
}


void Json::iterator::search_all_(Jnode *jn, const char *lbl,
                       const WalkStep &ws, std::vector<path_vector> & vpv) {
 // find all matches from given json node, cache them into current vector of paths:
 // cache is the vector of all found path-vectors
 if(jn->is_iterable()) {
  for(auto it = jn->children_().begin(); it != jn->children_().end(); ++it) {
   vpv.back().emplace_back(it, it->KEY);
   if(json_().callbacks_engaged(iterator_based))
    itr_callback_(jn);
   if(jn->is_object()) {
    if(json_().callbacks_engaged(label_based))
     lbl_callback_(it->KEY, jn, &vpv);
    if(ws.jsearch AMONG(label_match, Label_RE_search)) {
     long i = 0;
     if(label_matched_(it->KEY, ws, i)) vpv.push_back(vpv.back());  // if found, preserve path
    }
   }
   search_all_(&it->VALUE, jn->is_object()? it->KEY.c_str(): nullptr, ws, vpv);
   vpv.back().pop_back();
  }
  return;
 }
 // it's atomic then
 if(json_().callbacks_engaged(iterator_based))
  itr_callback_(jn);
 if(atomic_matched_(jn, lbl, ws))                               // if mathed
  vpv.push_back(vpv.back());                                    // preserve the path then
}



bool Json::iterator::search_successful_(Jnode *jn, const char *lbl, const WalkStep &ws, long &i) {
 // search current Jnode tree forward, return true/false if found,
 // if found pv_.back() must contain an iterator to the found node
 if(jn->is_iterable()) {
  for(auto it = jn->children_().begin(); it != jn->children_().end(); ++it) {
   pv_.emplace_back(it, it->KEY);
   if(json_().callbacks_engaged(iterator_based))
    itr_callback_(jn);
   if(jn->is_object()) {
    if(json_().callbacks_engaged(label_based))
     lbl_callback_(it->KEY, jn);
    if(ws.jsearch AMONG(label_match, Label_RE_search))
     if(label_matched_(it->KEY, ws, i)) return true;
   }
   if(search_successful_(&it->VALUE, jn->is_object()? it->KEY.c_str(): nullptr, ws, i))
    return true;
   pv_.pop_back();
  }
  return false;
 }
 // it's not iterable
 if(json_().callbacks_engaged(iterator_based))
  itr_callback_(jn);
 if(not atomic_matched_(jn, lbl, ws)) return false;
 return --i < 0? true: false;
}



void Json::iterator::lbl_callback_(const std::string &label, const Jnode *jn,
                               const std::vector<path_vector> *vpv) {
 // invoke callback attached to the label (if there's one)
 if(json_().lbl_callbacks().count(label) == 0) return;          // label not registered?

 if(vpv != nullptr)                                             // callback from search_all_()
  for(auto &path: vpv->back())                                  // then need to augment path-vector
   pv_.push_back(path);                                         // from last vpv path

 cnt_type_() = Jnode::Object;                                   // ensure supernode's correct type
 json_().lbl_callbacks()[label]( operator*() );                 // call back passing super node

 if(vpv != nullptr)
  pv_.resize(pv_.size() - vpv->back().size());                  // restore path
}



void Json::iterator::itr_callback_(const Jnode *jn) {
 // invoke callbacks matching given iterator
 for(auto &ic: json_().itr_callbacks()) {
  if(ic.iter == ic.iter.end()) continue;                        // iterations are over
  if(&ic.iter->value() != jn) continue;                         // it's a different node

  ic.callback( *ic.iter );                                      // call back passing super node
  json_().engage_callbacks(false);
  ++ic.iter;
  json_().engage_callbacks(true);
 }
}



bool Json::iterator::child_found_(Jnode *jn, const WalkStep &ws, long &i) {
 // search current iterable Jnode immediate children, return true/false if found,
 // if found pv_.back() must contain an iterator to the found node
 if(not jn->is_iterable())                                      // only iterables have children
  return false;

 for(auto it = jn->children_().begin(); it != jn->children_().end(); ++it) {
  if(jn->is_object() and (ws.jsearch AMONG(label_match, Label_RE_search))) {
   if(label_matched_(it->KEY, ws, i)) {
    pv_.emplace_back(jn->children_().find(it->KEY), it->KEY);
    return true;
   }
   continue;
  }
  if(it->VALUE.is_atomic())                                     // try to match str/num/bool/null
   if(atomic_matched_(&it->VALUE, it->KEY.c_str(), ws) and --i < 0) {
    pv_.emplace_back(jn->children_().find(it->KEY), it->KEY);
    return true;
   }
 }
 return false;
}


bool Json::iterator::label_matched_(const std::string &lbl, const WalkStep &ws, long &i) const {
 if(ws.jsearch == label_match) {
  if(lbl != ws.stripped.front()) return false;
  return --i < 0? true: false;
 }
 if(not std::regex_search(lbl, ws.re)) return false;            // ws.jsearch == Label_RE_match
 return --i < 0? true: false;
}


bool Json::iterator::atomic_matched_(const Jnode *jn, const char *lbl, const WalkStep &ws) const {
 // see if string/number/bool/null value matches
 if(ws.jsearch AMONG(regular_match, Regex_search, digital_match, Ditital_regex))
  return string_match_(jn, lbl, ws);
 if(ws.jsearch AMONG(label_match, Label_RE_search))
  return false;
 return bull_matched_(jn, lbl, ws);
}


bool Json::iterator::string_match_(const Jnode *jn, const char *lbl, const WalkStep &ws) const {
 // match numbers and strings
 switch(ws.jsearch) {
  case digital_match:
        if(ws.stripped.size() > 1)                              // label present: try matching
         if(lbl == nullptr or ws.stripped.back() != lbl) return false;
        return jn->is_number() and jn->val() == ws.stripped.front();
  case Ditital_regex:
        if(ws.stripped.size() > 1)                              // label present: try matching
         if(lbl == nullptr or ws.stripped.back() != lbl) return false;
        return jn->is_number() and std::regex_search(jn->val(), ws.re);
  case regular_match:
        if(ws.stripped.size() > 1)                              // label present: try matching
         if(lbl == nullptr or ws.stripped.back() != lbl) return false;
        return jn->is_string() and jn->val() == ws.stripped.front();
  case Regex_search:
        if(ws.stripped.size() > 1)                              // label present: try matching
         if(lbl == nullptr or ws.stripped.back() != lbl) return false;
        return jn->is_string() and std::regex_search(jn->val(), ws.re);
  default:                                                      // should never reach here.
        throw json_().EXP(Jnode::walk_a_bug);                   // covering compiler's warning
 }
}


bool Json::iterator::bull_matched_(const Jnode *jn, const char *lbl, const WalkStep &ws) const {
 // match bool and null (hence bull)
 switch(ws.jsearch) {
  case boolean_match:
        if(ws.stripped.size() > 1)                              // label attached: try matching
         if(lbl == nullptr or ws.stripped.back() != lbl) return false;
        return jn->is_bool() and
               (ws.stripped.front() == STR_ANY or (jn->bul()?
                                                   ws.stripped.front() == STR_TRUE:
                                                   ws.stripped.front() == STR_FALSE));
  case null_match:
        if(ws.stripped.size() > 1)                              // label attached: try matching
         if(lbl == nullptr or ws.stripped.back() != lbl) return false;
        return jn->is_null();
  default:                                                      // should never reach here.
        throw json_().EXP(Jnode::walk_a_bug);                   // covering compiler's warning
 }
}


bool Json::iterator::incremented(void) {
 // produce a next iterator per walk string (this call is just a wrapper for increment_)
 if(not pv_.empty())
  if(pv_.back().jit == json_().root().children_().end())        // don't attempt incrementing end()
   return false;

 long i = next_iterable_ws(walk_path_().size());
 if(i < 0) {
  pv_.emplace_back(json_().root().children_().end(), "");
  DBG(json_(), 2) DOUT(json_()) << "path is non-iterable" << std::endl;
  return false;
 }

 return increment_(i);
}


bool Json::iterator::increment_(long l) {
 // increment walk step and re-walk: returns true / false upon successful / unsuccessful walk
 auto & ws = walk_path_()[ l ];

 ++ws.offset;
 DBG(json_(), 2) DOUT(json_()) << "next increment: [" << l << "] " << ws << std::endl;
 walk_();
 if(pv_.empty()) return true;                                   // successful walk
 if(pv_.back().jit != json_().root().children_().end()) return true;    // successful walk

 // unsuccessful walk (out of iterations)
 if(l < static_cast<s_long>(pv_.back().wsi)) {                  // it's not my position, plus
  DBG(json_(), 2)                                               // it's in less significant place
   DOUT(json_()) << "walk [" << pv_.back().wsi << "] is out of iterations" << std::endl;
  // even if walk_ does not yield a result for a given idx, if walk failed because of
  // other walk step (index), then next walk still might yield a match in the next record.
  // That logic is required to handle irregular JSON
  long n = next_iterable_ws(walk_path_().size());
  return n < 0? false: increment_( n );
 }

 l = next_iterable_ws(l);
 if(l < 0) return false;
 ws.offset = ws.init;
 return increment_(l);
}


long Json::iterator::next_iterable_ws(long idx) const {
 // get next least significant position (index within walk-path) of iterable offset,
 // otherwise (whole path non-iterable) return -1
 while(--idx >= 0)
  if(walk_path_()[idx].init >= 0) break;
 return idx;
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
#undef CHR_RTRN
#undef STR_TRUE
#undef STR_FALSE
#undef STR_ANY
#undef STR_NULL
#undef JSN_OBJ_OPN
#undef JSN_OBJ_CLS
#undef JSN_ARY_OPN
#undef JSN_ARY_CLS
#undef LXM_OFS_OPN
#undef LXM_OFS_CLS
#undef LXM_SCH_OPN
#undef LXM_SCH_CLS

#undef LBL_SPR
#undef PFX_ITR
#undef PFX_WFR
#undef PFX_WFL

#undef JSN_FBDN
#undef JSN_QTD











