/*
 * Created by Dmitry Lyssenko, 2018, ldn.softdev@gmail.com
 *
 * this is a mini guide to Jtc, includes:
 *   o mini walk-path guide (wp_guide.hpp)
 *   o short usage notes
 *   o a limited set of examples
 */
#pragma once

#include <iostream>
#include "wp_guide.hpp"

// facilitate option materialization
#define XSTR(X) #X
#define STR(X) XSTR(X)
#define XCHR(X) *#X
#define CHR(X) XCHR(X)





class GuideJtc {
 // a singleton facilitating printing of a JTC guide (wp_guide, jtc_usage, jtc_examples)
 public:
    static GuideJtc &   instance(void)
                         { static GuideJtc guide; return guide; }
    void                print(void);

 private:
                        GuideJtc(void) = default;
                        GuideJtc(const GuideJtc &) = delete;
     const GuideJtc &   operator=(const GuideJtc &) = delete;

    static std::string  jtc_usage_;
    static std::string  jtc_examples_;
};



void GuideJtc::print(void) {
 // print walk path guide (substitute all required tokens)

 typedef std::pair<const char *, const char *> replace_token;
 static const replace_token rtv[] = {                // replace-token vectors
               {"%w", STR(OPT_WLK) }, {"%j", STR(OPT_JSN) }, {"%n", STR(OPT_SEQ) },
               {"%c", STR(OPT_CMP) }, {"%s", STR(OPT_SWP) }, {"%l", STR(OPT_LBL) },
               {"%i", STR(OPT_INS) }, {"%u", STR(OPT_UPD) }, {"%p", STR(OPT_PRG) },
               {"%e", STR(OPT_EXE) }, {"%m", STR(OPT_MDF) }, {"%x", STR(OPT_CMN) },
               {"%y", STR(OPT_PRT) }, {"%T", STR(OPT_TMP) }, {"%f", STR(OPT_FRC) },
               {"%a", STR(OPT_ALL)}
 };
 auto replace = [] (const replace_token &rt) {
                 GuideJtc::jtc_usage_ = std::regex_replace(GuideJtc::jtc_usage_,
                                                           std::regex{rt.first}, rt.second);
                 GuideJtc::jtc_examples_ = std::regex_replace(GuideJtc::jtc_examples_,
                                                              std::regex{rt.first}, rt.second);
                };

 for(const auto &rt: rtv)
  replace(rt);

 GuideWp::instance().print();
 std::cout << GuideJtc::jtc_usage_ << std::endl;
 std::cout << GuideJtc::jtc_examples_ << std::endl;
}



std::string GuideJtc::jtc_usage_{R"(

  * Usage notes:

multiple -%w usage:
 - if switch -%n is given, then all walk paths will be processed sequentially, otherwise, results
   would be grouped by relevance and walks interleaved; the order of provided walks will be (if
   can be) honored

options -%j and -%l usage:
 - when -%j is given w/o -%l, then each walked elements will be encapsulate into a JSON array; when
   used together, each walked elements will be grouped into relevant objects within a parent array
 - when -%j%j is given, walked elements with labels only (thus elements in arrays and the root are
   ignored) will be collected into JSON object; if used together with -%l%l then instead of
   collecting walked elements by outer label, inner labels (and respective) elements within walked
   objects will be collected

mutually exclusive primary operations:
 - options -%c -%i, -%u, -%s, -%p normally are mutually exclusive; if sighted together, only one type
   will be executed (selected in the priority of the listed order); though there's a case usage
   of -%p together with options -%i, -%u (facilitating move semantic), see notes below

options -%i, -%u when used with -%e:
 - the options accept one parameter which may be treated differently: initially a file name
   assumed; if the file is not found, then the parameter is treated as a JSON and its parsing is
   attempted; if parsing fails then a walk-path is assumed and if it fails an exception is thrown
 - when used together with  option -%e, the latter must precede option -%i or option -%u; in such
   case the parameter is subjected a for shell evaluation, but before that an interpolation occurs
   (see notes on the interpolation); the interpolated entry is completely escaped, thus does not
   require any additional quoting; all shell-specific chars (e.g.: '|', ';', '\"', etc) have to be
   quoted or escaped; terminate the cli line with trailing semicolon (which has to be escaped): \;

option -%m usage with -%i, -%u:
 - option -%m modifies behaviors of the these options:
   for -%i: toggles "merge" semantic; by default -%i can insert its parameter only into iterables,
           while with -%m, the parameter is merged with the destined element(s), which could be any
           JSON type
   for -%u: toggles "merge" semantic instead of overwrite when updating;
 - due to a variety of combinations of sources -%i, -%u and destination -%w, the number of various
   operation possibilities is big, therefore it's easier to track it in the following table:

 * insert operation (-%i) without merge usage examples:
   to \ from  |        [3,4]        |     {"a":3,"c":4}     |      "a":3,"c":4      |      3
 -------------+---------------------+-----------------------+-----------------------+-------------
    [1,2]     |      [1,2,[3,4]     |  [1,2,{"a":3,"c":4}]  | [1,2,{"a":3},{"c":4}] |   [1,2,3]
 {"a":1,"b":2}|    {"a":1,"b":2}    |  {"a":1,"b":2,"c":4}  |  {"a":1,"b":2,"c":4}  |{"a":1,"b":2}
     "a"      |         "a"         |          "a"          |          "a"          |     "a"
 * insert (-%i) with the merge (-%m):
   to \ from  |        [3,4]        |     {"a":3,"c":4}     |      "a":3,"c":4      |      3
 -------------+---------------------+-----------------------+-----------------------+-------------
    [1,2]     |      [1,2,3,4]      |       [1,2,3,4]       |       [1,2,3,4]       |   [1,2,3]
 {"a":1,"b":2}|{"a":[1,3],"b":[2,4]}|{"a":[1,3],"b":2,"c":4}|{"a":[1,3],"b":2,"c":4}|{"a":1,"b":2}
     "a"      |      ["a",3,4]      |       ["a",3,4]       |       ["a",3,4]       |   ["a",3]

 * update operation (-%u) without merge usage examples:
   to \ from  |        [3,4]        |     {"a":3,"c":4}     |         "a":3         |      3
--------------+---------------------+-----------------------+-----------------------+-------------
    [1,2]     |        [3,4]        |     {"a":3,"c":4}     |           3           |      3
 {"a":1,"b":2}|        [3,4]        |     {"a":3,"c":4}     |           3           |      3
     "a"      |        [3,4]        |     {"a":3,"c":4}     |           3           |      3
 * update (-%u) with the merge (-%m):
   to \ from  |        [3,4]        |     {"a":3,"c":4}     |         "a":3         |      3
--------------+---------------------+-----------------------+-----------------------+-------------
    [1,2]     |        [3,4]        |         [3,4]         |         [3,2]         |    [3,2]
 {"a":1,"b":2}|    {"a":3,"b":4}    |  {"a":3,"b":2,"c":4}  |     {"a":3,"b":2}     |{"a":3,"b":2}
     "a"      |        [3,4]        |     {"a":3,"c":4}     |         {"a":3}       |      3

option -%p usage with -%i, -%u:
  - if option -%p is sighted together with -%i, or -%u when the latter use a walk-path as a parameter
    then the insert/update operations effectively turn into *move*: the source entries (parameters
    of the -%i, -%u) will be purged at the end of operation; if -%p%p is sighted, then everything else
    is removed besides the destination locations (-%w)

options -%x and -%y usage:
 - these options should be given together: one -%x per any number of -%y options; each argument of
   -%y will be prepended with preceding -%x, together they will form an equivalent of -%w, e.g.:
   -%x1 -%yA -%yB -%x2 -%x3 -%yC is converted into: -%w1A -%w1B -%w2 -%w3C; there's no syntax validation
   in -%x and -%y until they converted into the respective -%w options, then walk validation occurs

interpolation:
 - interpolation may occur when using templates (-%T), or in shell cli argument; the notation for
   an interpolation is expressed in by tokens like {name}, or {{name}}. In the latter notation
   form the token gets interpolated from the namespace pointed by the token 'name' - the JSON
   element (in the namespace) being interpolated is preserved; in the former notation format, if
   JSON is a string, then outer quotation marks are dropped, if it's a JSON array or object, then
   the respective encasements ('[', ']', or '{', '}') are dropped, so the user must specify those
 - if an empty token is given (e.g.: {}, {{}}), then the interpolation of the currently selected
   (walked) JSON element occurs (the same interpolation rules apply)

operations chaining:
 - jtc normally performs one (primary) operation at a time; if multiple required those could be
   chained up using shell piping, e.g.:
      <stream.json jtc -J | jtc <..insert operation..> | jtc <..swap operation..> | etc
   however, such approach is less preferable due to printing/parsing JSON in between each step,
   the same could be performed using option set separator '/':
      <stream.json jtc -J / <..insert operation..> / <..swap operation..> / etc
   here, all operations are performed by a single jtc invocation, resulting in a much faster
   performance, plus that way the namespace is common across all the steps, which allows achieving
   operations with JSON otherwise unattainable in the former notation

single/multiple JSONs and sources:
 - if a single JSON source is given (file, or <std-in>), then jtc will process only the first JSON
   and then quit. If the single source holds multiple JSONs and all to be processed, then option
   -%a is required.
 - if multiple sources given (i.e., multiple files) then option -%a is assumed and is not required,
   jtc then performs reading and parsing from multiple files concurrently (in multiple threads).
   However, if option -%a is given, then it disables concurrent JSON reading/parsing and forces
   processing of all the input files sequentially (in a single thread)
)"};



std::string GuideJtc::jtc_examples_{R"(

  * Some examples and explanations:
  * - for a complete user guide visit:
  *   https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md

 Consider following source JSON (stored in the file example.json):
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
    jtc -%w[Relation][0][children] example.json
    [
       "Sophia",
       "Olivia"
    ]
  the above example shows drilling through JSON structure, but the same could be achieved via
  recursive search:
    jtc -%w'<children>l' example.json


- to select all children records individually (for the first record), run:
    jtc -%w[Relation][0][children][:] example.json
    "Sophia"
    "Olivia"
  or:
    jtc -%w'<children>l[:]' example.json
  or:
    jtc -%w'<children>l<>P:' example.json


- to select all children (for all records) and their parents as well, run:
    jtc -%w[Relation][:][parent] -%w[Relation][:][children][:] example.json
    "John Smith"
    "Sophia"
    "Olivia"
    "Anna Johnson"
    "John"
  or:
    jtc -w'<parent>l:' -w'<parent>l:[-1][children][:]' example.json

  the same could be achieved through a bit more succinct syntax:
    jtc -%x'[Relation][:]' -%y[parent] -%y '[children][:]' example.json
  or:
    jtc -%x'<parent>l:' -%y' ' -%y'[-1][children][:]' example.json

  Here, all concatenations of an option -%x with every option -%y is transformed into -%w then
  compile and executed. That syntax is supposed to save the input space when a common path is
  present in multiple walks


- to select all children, from the node whose parent's name starts with "John", run:
    jtc -%w '<^John>R[-1][children][:]' example.json
    "Sophia"
    "Olivia"

  However, the above walk syntax might be prone to false positives, as it finds any string
  starting with "John"; to improve it, we have to ensure that the search is attached to the label
  (a.k.a. scoped search), i.e., '"parent": "John..."':
    jtc -%w '[parent]:<^John>R[-1][children][:]' example.json


- to add (insert) a child "James" to a parent whose name starts with "John" and reflect the
  changes right into the source file, run:
    jtc -%f%w'[parent]:<^John>R[-1][children]' -%i'"James"' example.json
    jtc -tc example.json
    {
       "Relation": [
          {
             "age": 31,
             "children": [ "Sophia", "Olivia", "James" ],
             "city": "New York",
             "parent": "John Smith"
          },
          {
             "age": 28,
             "children": [ "John" ],
             "city": "Chicago",
             "parent": "Anna Johnson"
          }
       ]
    }


- to alter (rewrite) the parent's name from "John Smith" into "Jane Smith" run:
    jtc -%f%w '<John Smith>' -%u '"Jane Smith"' example.json
    jtc -tc example.json
    {
       "Relation": [
          {
             "age": 31,
             "children": [ "Sophia", "Olivia", "James" ],
             "city": "New York",
             "parent": "Jane Smith"
          },
          {
             "age": 28,
             "children": [ "John" ],
             "city": "Chicago",
             "parent": "Anna Johnson"
          }
       ]
    }


- to add a new record into a JSON node where parent is '"Jane Smith"':
    jtc -tc -w'[parent]:<Jane Smith>[-1]' -i'{"gene": "Y"}' example.json
    {
       "Relation": [
          {
             "age": 31,
             "children": [ "Sophia", "Olivia", "James" ],
             "city": "New York",
             "gene": "Y",
             "parent": "Jane Smith"
          },
          {
             "age": 28,
             "children": [ "John" ],
             "city": "Chicago",
             "parent": "Anna Johnson"
          }
       ]
    }

  However, that would add a record only to "Jane Smith" rendering the whole JOSN irregular.
  It's possible to update all records conditionally: say, update all records with
  '"gene": "X"', but the records containing word "Smith" with '"gene": "Y"':
    jtc -%f%w'[Relation][:]<g:"X">f<\bSmith\b>R<g:"Y">v[-1]' -%i0 -%T'{"gene": "{g}"}' example.json
    jtc -tc example.json
    {
       "Relation": [
          {
             "age": 31,
             "children": [ "Sophia", "Olivia", "James" ],
             "city": "New York",
             "gene": "Y",
             "parent": "Jane Smith"
          },
          {
             "age": 28,
             "children": [ "John" ],
             "city": "Chicago",
             "gene": "X",
             "parent": "Anna Johnson"
          }
       ]
    }

    That conditional insertion is facilitated using a few tricks:
    1. directive `<g:"X">f` facilitates 2 things at once: it sets up a namespace `g` to the value
       "X" and provides walk branching - if subsequent walking fails, then currently walked
       point will be restored
    2. directive <g:"Y">v will rewrite namespace `g` predicated previous walk step was a success
    3. Given we would need to apply template interpolation (to utilize the namespace `g`) the
       insertion argument is irrelevant, hence '-i0' is given
    4. Finally the template `-%T'{"gene": "{g}"}`` will expand into a relevant entry (before
       insertion) facilitating interpolation of a namespace `g`


- it's possible to wrap walked results back into JSON, with help of -%j option:
    jtc -%w'[Relation][:][parent]' -%j example.json
    [
       "Jane Smith",
       "Anna Johnson"
    ]


- if we throw in an option -%l, then output JSON format ensures that entries with labels will be
  grouped and displayed accordingly:
    jtc -%w'[Relation][:][parent]' -%j%l example.json
    [
       {
          "parent": [
             "Jane Smith",
             "Anna Johnson"
          ]
       }
    ]
  or:
    jtc -w'<parent>l:' -jl example.json


- an insert and update options could be subjected for a shell cli evaluation, e.g., say we want to
  capitalize all parent names in our JSON:
    jtc -tc -w'<parent>l:' -eu echo {{}} \| tr "[:lower:]" "[:upper:]" \; example.json
    {
       "Relation": [
          {
             "age": 31,
             "children": [ "Sophia", "Olivia", "James" ],
             "city": "New York",
             "gene": "Y",
             "parent": "JANE SMITH"
          },
          {
             "age": 28,
             "children": [ "John" ],
             "city": "Chicago",
             "gene": "X",
             "parent": "ANNA JOHNSON"
          }
       ]
    }


- to add a child "Victoria" into each record, but as the first child use update operation with
  templating:
    jtc -tc -w'<children>l:' -u'<children>l:' -T'["Victoria", {}]' example.json
    {
       "Relation": [
          {
             "age": 31,
             "children": [ "Victoria", "Sophia", "Olivia", "James" ],
             "city": "New York",
             "gene": "Y",
             "parent": "Jane Smith"
          },
          {
             "age": 28,
             "children": [ "Victoria", "John" ],
             "city": "Chicago",
             "gene": "X",
             "parent": "Anna Johnson"
          }
       ]
    }
  or:
    jtc -tc -w'<children>l:' -u'["Victoria", {}];' example.json

  in the 1st example, the destination point of an update (-%w), as well as the source point of the
  update (-%u) are in the same input - `example.json`. In the template (-%T), the interpolation
  of `{}` token will result in the naked array of "children", making resulting array carrying
  "Victoria" as the first element. -%T (and its interpolation token) here refers to the -%u walk;
  in the 2nd example, -%u holds a template itself, which then refers to the -%w walk


* for a complete user guide visit https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md
)"};





#undef XSTR
#undef STR
#undef XCHR
#undef CHR
