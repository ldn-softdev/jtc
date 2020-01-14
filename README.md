# `jtc` - cli tool to extract, manipulate and transform source JSON

`jtc` stand for: **_JSON transformational chains_** (used to be _JSON test console_).  
  
`jtc` offers a powerful way to select one or multiple elements from a source JSON and apply various actions on the selected
elements at once (wrap selected elements into a new JSON, filter in/out, sort elements, update elements, insert new elements,
remove, copy, move, compare, transform, swap around and many other operations).

### Content:
1. [Short description](https://github.com/ldn-softdev/jtc#short-description)
    * [Linux and MacOS precompiled binaries](https://github.com/ldn-softdev/jtc#linux-and-macos-precompiled-binaries-are-available-for-download)
    * [Installing via MacPorts](https://github.com/ldn-softdev/jtc#installing-via-macports)
    * [Compile and install instructions](https://github.com/ldn-softdev/jtc#compile-and-install-instructions)
    * [Release Notes](https://github.com/ldn-softdev/jtc/blob/master/Release%20Notes.md)

2. [Quick-start guide](https://github.com/ldn-softdev/jtc#quick-start-guide)
    * [list all URLs](https://github.com/ldn-softdev/jtc#1-lets-start-with-a-simple-thing---list-all-urls)
    * [dump all bookmark names](https://github.com/ldn-softdev/jtc#2-dump-all-bookmark-names-from-the-work-folder)
    * [dump all URL's names](https://github.com/ldn-softdev/jtc#3-dump-all-urls-names)
    * [dump all the URLs and corresponding names](https://github.com/ldn-softdev/jtc#4-dump-all-the-urls-and-their-corresponding-names-preferably-wrap-found-pairs-in-json)
    * [Subscripts (offsets) and Searches explained](https://github.com/ldn-softdev/jtc#5-subscripts-offsets-and-searches-explained)
    * [Debugging and validating JSON](https://github.com/ldn-softdev/jtc#6-debugability--json-validation)
3. [Complete User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md)
4. [Class usage and c++14 interface](https://github.com/ldn-softdev/jtc#a-tiny-example-of-class-usage-and-its-interface-c14)
5. [`jtc` vs **jq**](https://github.com/ldn-softdev/jtc#jtc-vs-jq)

## Short description
\- `jtc` is simple but efficient cli utility tool to manipulate JSON data

`jtc` offers following features (a short list of main features):
  - simple user interface allowing applying a bulk of changes in a single or chained sets of commands 
  - featured _walk-path_ interface lets extracting any combination of data from source JSON
  - extracted data is representable either as found, or could be encapsulated in JSON array/object or transfored using _templates_
  - support _Regular Expressions_ when searching source JSON (various _RE grammars_ supported)
  - fast and efficient processing of very large JSON files (built-in search cache)
  - insert/update operations optionally may undergo _shell cli_ evaluation
  - features namespaces, interpolation from namespaces in templates 
  - supports _buffered_ and _streamed_ modes of input reads
  - written entirely in _C++14_, no dependencies (STL only, idiomatic C++, **_no memory leaks_**)
  - extensively debuggable
  - conforms JSON specification ([json.org](http://json.org/index.html))

The _walk-path_ feature is easy to understand - it's only made of 2 types of lexemes:
  - subscripts - enclosed into `[`, `]`, subscripts let traversing JSON tree downwards and **upwards (!)**
  - search lexemes - encased as `<..>` or `>..<` (for a recursive and non-recursive search respectively); search lexemes facilitate various match criteria defined by an optional suffix and/or quantifier.

Both types of lexemes cab be _iterable_ - **subscripts** let iterating over children of currently addressed JSON iterables
nodes (arrays/objects), while iterable **search lexemes** let iterating over all matches for a given search criteria.
A _walk-path_ may have an arbitrary number of lexemes, while the tool accepts a virtually unlimited number of walk
paths. See below more detailed explanation with examples


### Linux and MacOS precompiled binaries are available for download

For compiling, **`c++14`** (or later) is required. To compile under different platforms:
  - MacOS/BSD: `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - Linux: `c++ -o jtc -Wall -std=gnu++14 -static -Ofast jtc.cpp`
  - Debian: `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp` (ensure `c++` poits to `clang++-6.0` or above)

*pass `-DNDEBUG` flag if you like to compile w/o debugs, however it's unadvisable -
there's no performance gain from doing so*

or download the latest **precompiled binary**:
- _latest_ [macOS](https://github.com/ldn-softdev/jtc/releases/download/1.75c/jtc-macos-64.v1.75c)
- _latest_ [linux 64 bit](https://github.com/ldn-softdev/jtc/releases/download/1.75c/jtc-linux-64.v1.75c)
- _latest_ [linux 32 bit](https://github.com/ldn-softdev/jtc/releases/download/1.75c/jtc-linux-32.v1.75c)

### Installing via MacPorts

On MacOS, you can install `jtc` via the [MacPorts](https://macports.org) package manager:

``` ShellSession
$ sudo port selfupdate
$ sudo port install jtc
```


### Compile and install instructions:

download [`jtc-master.zip`](https://github.com/ldn-softdev/jtc/archive/master.zip),
unzip it, descend into unzipped folder, compile using an appropriate command, move compiled file into an install location.

here're the example steps for **MacOS**:
  - say, `jtc-master.zip` has been downloaded to a folder and the terminal app is open in that
folder:
  - `unzip jtc-master.zip`
  - `cd jtc-master`
  - `c++ -o jtc -Wall -std=c++17 -Ofast jtc.cpp`
  - `sudo mv ./jtc /usr/local/bin/`


### Release Notes
See the latest [Release Notes](https://github.com/ldn-softdev/jtc/releases)


## Quick-start guide:
*run `jtc -g` for walk-path explanations, usage notes and additional usage examples*
##
Consider a following JSON (a mockup of a bookmark container), stored in a file `Bookmarks`:
```json
{
   "Bookmarks": [
      {
         "children": [
            {
               "children": [
                  { "name": "The New York Times", "stamp": "2017-10-03, 12:05:19", "url": "https://www.nytimes.com/" },
                  { "name": "HuffPost UK", "stamp": "2017-11-23, 12:05:19", "url": "https://www.huffingtonpost.co.uk/" }
               ],
               "name": "News",
               "stamp": "2017-10-02, 12:05:19"
            },
            {
               "children": [
                  { "name": "Digital Photography Review", "stamp": "2017-02-27, 12:05:19", "url": "https://www.dpreview.com/" }
               ],
               "name": "Photography",
               "stamp": "2017-02-27, 12:05:19"
            }
         ],
         "name": "Personal",
         "stamp": "2017-01-22, 12:05:19"
      },
      {
         "children": [
            { "name": "Stack Overflow", "stamp": "2018-05-01, 12:05:19", "url": "https://stackoverflow.com/" },
            { "name": "C++ reference", "stamp": "2018-06-21, 12:05:19", "url": "https://en.cppreference.com/" }
         ],
         "name": "Work",
         "stamp": "2018-03-06, 12:07:29"
      }
   ]
}

```


### 1. let's start with a simple thing - list all URLs:
```bash
bash $ jtc -w'<url>l:' Bookmarks
"https://www.nytimes.com/"
"https://www.huffingtonpost.co.uk/"
"https://www.dpreview.com/"
"https://stackoverflow.com/"
"https://en.cppreference.com/"
```
The **_walk-path_** (an argument of `-w`) is a combination of lexemes. There are only 2 types of lexemes:
1. subscript lexemes -  enclosed in `[..]`
2. search lexemes - enclosed in `<..>` for a _recursive_, or in `>..<` for a _non-recursive_ type of search
\- the walk-paths may contain any number of lexemes, optionally separated with space(s)

let's take a look at the walk-path `<url>l:`:
- search lexemes are enclosed in angular brackets `<`, `>` - that style provides a **recursive search** throughout JSON 
- suffix `l` instructs to search among **labels** only
- quantifier `:` instructs to find **all occurrences**, such quantifiers makes a path *iterable*


### 2. dump all bookmark names from the `Work` folder:
```bash
bash $ jtc -w'<Work>[-1][children][:][name]' Bookmarks
"Stack Overflow"
"C++ reference"
```
here the walk-path `<Work>[-1][children][:][name]` is made of following lexemes:

a. `<Work>`: find within a JSON tree the **first** occurrence where the **JSON string** value is matching `"Work"` exactly  
b. `[-1]`: **step up** one tier in the JSON tree structure (i.e., address an immediate parent of the found JSON element)  
c. `[children]`: **select/address** a node whose label is `"children"` (it'll be a JSON array, at the same tier with `Work`)  
d. `[:]`: select **each node** in the array  
e. `[name]`: select/address a node whose label is `"name"` 


in order to understand better how the walk-path works, let's run a series of cli in a slow-motion, gradually adding lexemes
to the path one by one, perhaps with the option `-l` to see also the labels (if any) of the selected elements:

```bash
bash $ jtc -w'<Work>' -l Bookmarks
"name": "Work"
```
```bash
bash $ jtc -w'<Work>[-1]' -l Bookmarks
{
   "children": [
      {
         "name": "Stack Overflow",
         "stamp": "2018-05-01, 12:05:19",
         "url": "https://stackoverflow.com/"
      },
      {
         "name": "C++ reference",
         "stamp": "2018-06-21, 12:05:19",
         "url": "https://en.cppreference.com/"
      }
   ],
   "name": "Work",
   "stamp": "2018-03-06, 12:07:29"
}
```
```bash
bash $ jtc -w'<Work>[-1][children]' -l Bookmarks
"children": [
   {
      "name": "Stack Overflow",
      "stamp": "2018-05-01, 12:05:19",
      "url": "https://stackoverflow.com/"
   },
   {
      "name": "C++ reference",
      "stamp": "2018-06-21, 12:05:19",
      "url": "https://en.cppreference.com/"
   }
]
```
```bash
bash $ jtc -w'<Work>[-1][children][:]' -l Bookmarks
{
   "name": "Stack Overflow",
   "stamp": "2018-05-01, 12:05:19",
   "url": "https://stackoverflow.com/"
}
{
   "name": "C++ reference",
   "stamp": "2018-06-21, 12:05:19",
   "url": "https://en.cppreference.com/"
}
```
```bash
bash $ jtc -w'<Work>[-1][children][:][name]' -l Bookmarks
"name": "Stack Overflow"
"name": "C++ reference"
```


### 3. dump all URL's names:
```bash
bash $ jtc -w'<url>l:[-1][name]' Bookmarks
"The New York Times"
"HuffPost UK"
"Digital Photography Review"
"Stack Overflow"
"C++ reference"
```
this walk-path `<url>l:[-1][name]`:

 - finds recursively (encasement `<`, `>`) each (`:`) JSON element with a label (`l`) matching `url`
 - then for an each found JSON element, select its parent (`[-1]`)
 - then, select a JSON element with the label `"name"` (encasement `[`, `]`)


### 4. dump all the URLs and their corresponding names, preferably wrap found pairs in JSON:
```bash
bash $ jtc -w'<url>l:' -w'<url>l:[-1][name]' -jl Bookmarks
[
   {
      "name": "The New York Times",
      "url": "https://www.nytimes.com/"
   },
   {
      "name": "HuffPost UK",
      "url": "https://www.huffingtonpost.co.uk/"
   },
   {
      "name": "Digital Photography Review",
      "url": "https://www.dpreview.com/"
   },
   {
      "name": "Stack Overflow",
      "url": "https://stackoverflow.com/"
   },
   {
      "name": "C++ reference",
      "url": "https://en.cppreference.com/"
   }
]
```

- yes, multiple walks (`-w`) are allowed
- option `-j` will wrap the walked outputs into a JSON array, but not just,
- option `-l` used together with `-j` will ensure relevant walks are grouped together (try without `-l`)
- if multiple walks (`-w`) are present, by default, walked results will be printed interleaved


### 5. Subscripts (offsets) and Searches explained
In short:
- Subscript lexemes (`[..]`) facilitate:
    - addressing children (by index/label) in _JSON iterables_ (_arrays_ and _objects_) - i.e., traverse JSON structure downward
    from the root (toward leaves), e.g.: `[2]`, `[id]` 
    - addressing parents (immediate and distant) - i.e., traverse JSON structure **upwards**, toward the the root (from leaves),
    e.g.:  `[-1]` (tier offset from the currently walked/found element), `[^2]` (tier offset from the root towards walked/found element)
    - select ranges and slices of JSON elements in _JSON iterables_, e.g.: `[+2]`, `[:]`, `[:3]`, `[-2:]`, `[1:-1:2]` 
- Search lexemes (`<..>`, `>..<`) facilitate:
    - recursive (`<..>`) and non-recursive (`>..<`) matches
    - there're optional one-letter suffixes that may follow the lexemes (e.g.: `<..>Q`) which define type of search: (REGEX) string 
    search, (REGEX) label search, (REGEX) numerical, boolean, null, atomic, objects, arrays (or either), arbitrary JSONs, 
    unique, duplicates, sorted match, etc.
    - there're also optional quantifiers to search lexemes (must take the last position in the search lexeme, after the suffix
    if one present) - let selecting match instance, or range of matches (e.g.: `<id>l3`- will match 4th (zero based) label `"id"`;
    if no quantifier present `0` is assumed - first match)
- a subscript lexeme could be grouped with a search lexeme over ':' to facilitate a **_scoped search_**, e.g.: `[id]:<value>` is a single
   lexeme which will match recursively the first occurrence of the string `"value"` with the label `"id"` - i.e., `"id": "value"`
- Directives: there are a few suffixes which turn a search lexeme into a directive:
    - directives do not do any matching, instead they facilitate a certain action/operation with the currently walked JSON element,
    like: memorize it in the _namespace_, or erase from it, or memorize its label, or perform a _shell cli_ evaluation
    - couple directives (`<>f` and `<>F`) facilitate also walk branching

Refer to 
[`jtc` User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-json)
for the detailed explanation of the subscripts, search lexemes and directives.

### 6. Debugability / JSON validation
`jtc` is extensively debuggable: the more times option `-d` is passed the more debugs will be produced.
Enabling too many debugs might be overwhelming, though one specific case many would find extremely useful - when validating
a failing JSON:
```bash
bash $ <addressbook-sample.json jtc 
jtc json exception: expected_json_value
```
If JSON is big, it's desirable to locate the parsing failure point. Passing just one `-d` let easily spotting the
parsing failure point and its locus:
```bash
bash $ <addressbook-sample.json jtc -d
.display_opts(), option set[0]: -d (internally imposed: )
.read_inputs(), reading json from <stdin>
.location_(), exception locus: ...      }|       ],|       "children": [,],|       "spouse": null|    },|    {...
.location_(), exception spot: ----------------------------------------->| (offset: 967)
jtc json parsing exception (<stdin>:967): expected_json_value
bash $ 
```

## Complete User Guide
Refer to a complete [User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md) for further examples and guidelines.


## A tiny example of class usage and its interface (c++14):
Say, we want to accomplish a following task:
1. read Address Book JSON from `<stdin>`
2. sort all records by `Name` (for simplicity, assume all records have that label)
3. output resulting Address Book JSON

Below is the code sample how that could be achieved using `Json.hpp` class and the source JSON - Address Book:
```c++
#include <iostream>
#include <fstream>
#include "lib/Json.hpp"

// compile with: c++ -o sort_ab -Wall -std=c++14 sorting_ab.cpp
using namespace std;


int main(int argc, char *argv[]) {
 Json jin( {istream_iterator<char>(cin>>noskipws), istream_iterator<char>{}} );  // read and parse json from cin
 vector<string> names(jin.walk("[AddressBook][+0][Name]"), jin.walk().end());    // get all the names
 sort(names.begin(), names.end());                                               // sort the names

 Json srt = ARY{};                                                               // rebuild AB with sorted records
 for(const auto &name: names)
  srt.push_back( move( *jin.walk("[AddressBook][Name]:<" + name + ">[-1]") ) );

 jin["AddressBook"].clear().push_back( move(srt) );                             // put back into the original container
 cout << jin.tab(3) << endl;                                                    // and print using indentation 3
}
```

Address Book JSON:
```bash
bash $ jtc -tc addressbook-sample.json 
{
   "AddressBook": [
      {
         "Name": "John",
         "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" },
         "age": 25,
         "children": [ "Olivia" ],
         "phoneNumbers": [
            { "number": "212 555-1234", "type": "mobile" },
            { "number": "213 123-2368", "type": "mobile" }
         ],
         "spouse": "Martha"
      },
      {
         "Name": "Ivan",
         "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" },
         "age": 31,
         "children": [],
         "phoneNumbers": [
            { "number": "573 923-6483", "type": "home" },
            { "number": "523 283-0372", "type": "mobile" }
         ],
         "spouse": null
      },
      {
         "Name": "Jane",
         "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" },
         "age": 25,
         "children": [ "Robert", "Lila" ],
         "phoneNumbers": [
            { "number": "358 303-0373", "type": "office" },
            { "number": "333 638-0238", "type": "home" }
         ],
         "spouse": "Chuck"
      }
   ]
}
bash $ 
```

Output result:
```bash
bash $ <addressbook-sample.json sort_ab | jtc -tc      # using jtc here only for a compact view
{
   "AddressBook": [
      [
         {
            "Name": "Ivan",
            "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" },
            "age": 31,
            "children": [],
            "phoneNumbers": [
               { "number": "573 923-6483", "type": "home" },
               { "number": "523 283-0372", "type": "mobile" }
            ],
            "spouse": null
         },
         {
            "Name": "Jane",
            "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" },
            "age": 25,
            "children": [ "Robert", "Lila" ],
            "phoneNumbers": [
               { "number": "358 303-0373", "type": "office" },
               { "number": "333 638-0238", "type": "home" }
            ],
            "spouse": "Chuck"
         },
         {
            "Name": "John",
            "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" },
            "age": 25,
            "children": [ "Olivia" ],
            "phoneNumbers": [
               { "number": "212 555-1234", "type": "mobile" },
               { "number": "213 123-2368", "type": "mobile" }
            ],
            "spouse": "Martha"
         }
      ]
   ]
}
bash $ 
```
for the complete description of Json class interface, refer to [Json.hpp](https://github.com/ldn-softdev/jtc/blob/master/lib/Json.hpp)

Btw, the same sorting is achievable using `<>g` lexeme:
```bash
bash $ <addressbook-sample.json jtc -tc -w'[0]' -pi'[Name]:<>g:[-1]'
{
   "AddressBook": [
      {
         "Name": "Ivan",
         "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" },
         "age": 31,
         "children": [],
         "phoneNumbers": [
            { "number": "573 923-6483", "type": "home" },
            { "number": "523 283-0372", "type": "mobile" }
         ],
         "spouse": null
      },
      {
         "Name": "Jane",
         "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" },
         "age": 25,
         "children": [ "Robert", "Lila" ],
         "phoneNumbers": [
            { "number": "358 303-0373", "type": "office" },
            { "number": "333 638-0238", "type": "home" }
         ],
         "spouse": "Chuck"
      },
      {
         "Name": "John",
         "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" },
         "age": 25,
         "children": [ "Olivia" ],
         "phoneNumbers": [
            { "number": "212 555-1234", "type": "mobile" },
            { "number": "213 123-2368", "type": "mobile" }
         ],
         "spouse": "Martha"
      }
   ]
}
bash $
```

## `jtc` vs **jq**:
`jtc` was _inspired_ by the complexity of **jq** interface (and its 
[DSL](https://en.wikipedia.org/wiki/Domain-specific_language)),
aiming to provide a user tool which would let attaining the desired result in a more feasible and succinct way


### utility ideology:
 - **jq** is a stateful processor with own DSL, variables, operations, control flow logic, IO system, etc, etc
 - `jtc` is a unix utility confining its functionality to operation types with its data model only (as per unix ideology). `jtc`
 performs one major operation at a time (like insertion, update, swap, etc), however multiple operations could be chained
 using `/` delimiter

**jq** is non-idiomatic in a _unix way_, e.g.: one can write a program in **jq** language that even has nothing to do with JSON.
Most of the requests (if not all) to manipulate JSONs are _ad hoc_ type of tasks, and learning **jq**'s DSL for _ad hoc_ type of tasks 
is an overkill (that purpose is best facilitated with
[GPL](https://en.wikipedia.org/wiki/General-purpose_language)).  
The number of asks on the
[stackoverflow](https://stackoverflow.com/) 
to facilitate even simple queries for **jq** is huge - that's the proof in itself that for many people feasibility of attaining their 
asks with **jq** is a way too low, hence they default to posting their questions on the forum.

`jtc` on the other hand is a utility (not a language), which employs a novel but powerful concept, which "embeds" the ask right into the
_walk-path_. That facilitates a much higher feasibility of attaining a desired result: building a walk-path a lexeme by lexeme, 
one at a time, provides an immediate visual feedback and let coming up with the desired result rather quickly.


### learning curve:
 - **jq**: before you could come up with a query to handle even a relatively simple ask, you need to become an expert in 
 **jq** language, which will take some time. Coming up with the complex queries requires what it seems having a PhD in **jq**, or spending 
 lots of time on stackoverflow and similar forums
 - `jtc` employs only a simple (but powerful) concept of the _walk-path_ (which is made only of 2 types of lexemes,
 each type though has several variants) which is quite easy to grasp.


### handling irregular JSONs:
 - **jq**: handling irregular JSONs for **jq** is not a challenge, building a query is! The more irregularities you need
 to handle the more challenging the query (**jq** program) becomes
 - `jtc` was conceived with the idea of being capable of handling complex irregular JSONs with a simplified interface - that all is
 fitted into the concept of the _walk-path_, while daisy-chaining multiple operations is possible to satisfy almost every ask. 


### programming model
 - **jq** is written in _C_, which drags all intrinsic problems the language has dated its creation
 ([here's what I mean](https://github.com/ldn-softdev/cpluspus-vs-c))
 - `jtc` is written in the idiomatic _C++14_ using STL only. `jtc` does not have a single naked memory allocation operator
 (those few `new` operators required for legacy interface are implemented as _guards_),
 nor it has a single naked pointer acting as a resource holder/owner, thus `jtc` is guaranteed to be **free of memory/resourses leaks** 
 (at least one class of the problems is off the table) - **STL guaranty**.  
 Also, `jtc` is written in a very portable way, it should not cause problems compiling it under any unix like system.


### solutions input invariance
\- most of `jtc` solutions are input invariant (hardly the same could be stated for **jq**). Not that it's impossible to come up with
invariant solutions in **jq**, it's just a lot more harder, while `jtc` with its walk-path model prompts for invariant solutions.
I.e., the invariant solution will keep working even once the JSON outer format changes (invariant solution only would stop working once 
the relationship between walked JSON elements changes).  
E.g.: consider a following query, extract format `[ "name", "surname" ]` from 2 types of JSON:
```bash
bash $ case1='{"Name":"Patrick", "Surname":"Lynch", "gender":"male", "age":29}'
bash $ case2='[{"Name":"Patrick", "Surname":"Lynch", "gender":"male", "age":29},{"Name":"Alice", "Surname":"Price", "gender":"female", "age":27}]'
```
a natural, idiomatic `jtc` solution would be:
```bash
bash $ <<<$case1 jtc -w'<Name>l:<N>v[-1][Surname]' -rT'[{{N}},{{}}]'
[ "Patrick", "Lynch" ]
bash $ <<<$case2 jtc -w'<Name>l:<N>v[-1][Surname]' -rT'[{{N}},{{}}]'
[ "Patrick", "Lynch" ]
[ "Alice", "Price" ]
```
While one of the most probable **jq** solution would be:
```bash
bash $ <<<$case1 jq -c 'if type == "array" then .[] else . end | [.Name, .Surname]'
["Patrick","Lynch"]
bash $ <<<$case2 jq -c 'if type == "array" then .[] else . end | [.Name, .Surname]'
["Patrick","Lynch"]
["Alice","Price"]
```
The both solutions work correctly, however, any change in the outer encapsulation will break **jq**'s solution , 
while `jtc` will keep working even if JSON is reshaped into an _irregular_ structure, e.g.:
```bash
#jtc:
bash $ case3='[{"Name":"Patrick", "Surname":"Lynch", "gender":"male", "age":29}, {"closed circle":[{"Name":"Alice", "Surname":"Price", "gender":"female", "age":27}, {"Name":"Rebecca", "Surname":"Hernandez", "gender":"female", "age":28}]}]'
bash $ <<<$case3 jtc -w'<Name>l:<N>v[-1][Surname]' -rT'[{{N}},{{}}]'
[ "Patrick", "Lynch" ]
[ "Alice", "Price" ]
[ "Rebecca", "Hernandez" ]

#jq:
bash $ <<<$case3 jq -c 'if type == "array" then .[] else . end | [.Name, .Surname]'
["Patrick","Lynch"]
[null,null]
```
The same property makes `jtc` solutions resistant to cases of incomplete data, e.g.: if we  drop `"Name"` entry from one of the 
entries in case 2, `jtc` solution still works correctly:
```bash
#jtc:
bash $ case2='[{"Surname":"Lynch", "gender":"male", "age":29},{"Name":"Alice", "Surname":"Price", "gender":"female", "age":27}]'
bash $ <<<$case2 jtc -w'<Name>l:<N>v[-1][Surname]' -rT'[{{N}},{{}}]'
[ "Alice", "Price" ]

#jq:
bash $ <<<$case2 jq -c 'if type == "array" then .[] else . end | [.Name, .Surname]'
[null,"Lynch"]
["Alice","Price"]
```
\- i.e., `jtc` will not assume that user would require some default substitution in case of incomplete data (but if such handling is 
required then the walk-path can be easily enhanced)


### JSON numerical fidelity:
 - **jq** is not compliant with JSON numerical definition. What jq does, it simply converts a symbolic numerical representation to an 
 internal binary and keeps it that way. That approach:
     - is not compliant with JSON definition of the numerical values
     - it has problems retaining required precision
     - might change original representation of numericals
 - `jtc` validates all JSON numericals per JSON standard and keep numbers internally in their original literal format, so it's free of
 all the above caveats, compare:
 
 Handling | `jtc` | **jq 1.6** 
 --- | ---: | :---
Invalid Json: `[ 00 ]` | `<<<'[00]' jtc` | `<<<'[00]' jq -c .`
_Parsing result_ | `jtc json exception: missed_prior_enumeration` | `[0]`
Precision test: | `<<<'[0.99999999999999999]' jtc -r` | `<<<'[0.99999999999999999]' jq -c .`
_Parsing result_ | `[ 0.99999999999999999 ]` | `[1]`
Retaining original format: | `<<<'[0.00001]' jtc -r` | `<<<'[0.00001]' jq -c .`
_Parsing result_ | `[ 0.00001 ]` | `[1e-05]`


### performance:
here's a 4+ million node JSON file [standard.json](https://github.com/ldn-softdev/jtc/releases/download/standard.json/standard.json):
```
bash $ time jtc -zz standard.json 
4329975
user 5.537 sec
```
The table below compares `jtc` and jq performance for similar operations (using `TIMEFORMAT="user %U sec"`):

`jtc` 1.75 | jq 1.6
---: | :---
_**`parsing JSON:`**_ | _**`parsing JSON:`**_
`bash $ time jtc -t2 standard.json \| md5` | `bash $ time jq -M . standard.json \| md5`
`d3b56762fd3a22d664fdd2f46f029599` | `d3b56762fd3a22d664fdd2f46f029599`
`user 8.679 sec` | `user 19.570 sec`
_**`removing by key from JSON:`**_ | _**`removing by key from JSON:`**_
`bash $ time jtc -t2 -pw'<attributes>l:' standard.json \| md5` | `bash $ time jq -M 'del(..\|.attributes?)' standard.json \| md5`
`0624aec46294399bcb9544ae36a33cd5` | `0624aec46294399bcb9544ae36a33cd5`
`user 9.442 sec` | `user 28.624 sec`
_**`updating JSON recursively by label:`**_ | _**`updating JSON recursively by label:`**_
`bash $ time jtc -t2 -w'<attributes>l:[-1]' -i'{"reserved": null}' standard.json \| md5` | `bash $ time jq -M 'walk(if type == "object" and has("attributes") then . + { "reserved" : null } else . end)' standard.json \| md5`
`6c86462ae6b71e10e3ea114e86659ab5` | `6c86462ae6b71e10e3ea114e86659ab5`
`user 12.292 sec` | `user 30.255 sec`

Machine spec used for testing:
```
  Model Name:                 MacBook Pro
  Model Identifier:           MacBookPro15,1
  Processor Name:             Intel Core i7
  Processor Speed:            2,6 GHz
  Number of Processors:       1
  Total Number of Cores:      6
  L2 Cache (per Core):        256 KB
  L3 Cache:                   12 MB
  Hyper-Threading Technology: Enabled
  Memory:                     16 GB 2400 MHz DDR4
```

Refer to a complete [User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md) for further examples and guidelines.

##### Enhancement requests are more than welcome: *ldn.softdev@gmail.com*





