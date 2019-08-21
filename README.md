# `jtc` - cli tool to extract, manipulate and transform source JSON

`jtc` stand for: _JSON test console_, but it's a legacy name, don't get mislead.  
`jtc` offers a powerful way to select one or multiple elements from a source JSON and apply various actions on the selected elements
at once (wrap selected elements into a new JSON, filter in/out, update elements, insert new elements, remove, copy, move, compare,
transform and swap around).

### Content:
1. [Short description](https://github.com/ldn-softdev/jtc#short-description)
    * [Linux and MacOS precompiled binaries](https://github.com/ldn-softdev/jtc#linux-and-macos-precompiled-binaries-are-available-for-download)
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
  - simple user interface allowing applying a bulk of changes in one command
  - featured walk-path interface lets extracting any combination of data from source JSON
  - extracted data is representable either as it found, or encapsulated in JSON array/object
  - support Regular Expressions when searching source JSON
  - fast and efficient processing very large JSON files (built-in search cache)
  - insert/updates operations optionally may undergo _shell cli_ evaluation
  - features namespaces, interpolation from namespaces and templates 
  - supports buffered and streamed modes of input reads
  - written entirely in C++14, no dependencies (STL only, idiomatic C++, no memory leaks)
  - extensively debuggable
  - conforms JSON specification ([json.org](http://json.org/index.html))

Walk path feature is easy to understand - it's only made of 2 types of lexemes:
  - subscripts - enclosed into `[`, `]`: subscripts let traversing JSON tree downwards and **upwards**
  - search lexemes - encased into `<`, `>`: search lexemes facilitate either full match or Regex search.

Both types of lexemes are iterable - subscript let iterating over children of currently addressed iterables node (array/object),
while iterable search lexemes let iterating over all matches for a given search criteria.
A walk path is made of an arbitrary number of lexemes, while the tool accepts a virtually unlimited number of walk
paths. See below more detailed explanation with examples


### Linux and MacOS precompiled binaries are available for download

For compiling, `c++14` (or later) is required:
  - to compile under MacOS, use cli:  
      `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - To compile under Linux, use cli:  
      `c++ -o jtc -Wall -std=gnu++14 -static -Ofast jtc.cpp`

*pass `-DNDEBUG` flag if you like to compile w/o debugs, however it's unadvisable -
there's no performance gain from doing so*

or download the latest precompiled binary:
- _latest_ [macOS](https://github.com/ldn-softdev/jtc/releases/download/1.72a/jtc-macos-64.v1.72a)
- _latest_ [linux 64 bit](https://github.com/ldn-softdev/jtc/releases/download/1.72a/jtc-linux-64.v1.72a)
- _latest_ [linux 32 bit](https://github.com/ldn-softdev/jtc/releases/download/1.72a/jtc-linux-32.v1.72a)


### Compile and install instructions:

download `jtc-master.zip`, unzip it, descend into unzipped folder, compile using
an appropriate command, move compiled file into an install location.

here're the example steps for **MacOS**:
  - say, `jtc-master.zip` has been downloaded to a folder and the terminal app is open in that
folder:
  - `unzip jtc-master.zip`
  - `cd jtc-master`
  - `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - `sudo mv ./jtc /usr/local/bin/`

For **Linux** you'd have to compile using this line:
- `c++ -o jtc -Wall -std=gnu++14 -static -Ofast jtc.cpp`

### Release Notes
See the latest [Release Notes](https://github.com/ldn-softdev/jtc/blob/master/Release%20Notes.md)


## Quick-start guide:
*run `jtc -g` for walk path explanations, usage notes and additional usage examples*

Consider a following JSON (a mockup of a bookmark container), stored in a file `Bookmarks`:
```
{
   "Bookmarks": [
      {
         "children": [
            {
               "children": [
                  {
                     "name": "The New York Times",
                     "stamp": "2017-10-03, 12:05:19",
                     "url": "https://www.nytimes.com/"
                  },
                  {
                     "name": "HuffPost UK",
                     "stamp": "2017-11-23, 12:05:19",
                     "url": "https://www.huffingtonpost.co.uk/"
                  }
               ],
               "name": "News",
               "stamp": "2017-10-02, 12:05:19"
            },
            {
               "children": [
                  {
                     "name": "Digital Photography Review",
                     "stamp": "2017-02-27, 12:05:19",
                     "url": "https://www.dpreview.com/"
                  }
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
   ]
}
```


### 1. let's start with a simple thing - list all URLs:
```
bash $ jtc -w'<url>l:' Bookmarks
"https://www.nytimes.com/"
"https://www.huffingtonpost.co.uk/"
"https://www.dpreview.com/"
"https://stackoverflow.com/"
"https://en.cppreference.com/"
```
The walk-path (an argument of `-w`) is a combination of lexemes. There are only 2 types of lexemes:
1. subscript lexemes -  enclosed in `[`,`]`
2. search lexemes - enclosed in `<`,`>`
\- the walk-paths may contain any number of lexemes

let's take a look at the walk-path `<url>l:`:
- search lexemes are enclosed in angular brackets `<`, `>` - that style provides a **recursive search**
- suffix `l` instructs to search among **labels** only
- quantifier `:` instructs to find **all occurrences**, such quantifiers makes a path *iterable*


### 2. dump all bookmark names from the `Work` folder:
```
bash $ jtc -w'<Work>[-1][children][:][name]' Bookmarks
"Stack Overflow"
"C++ reference"
```
here the walk path `<Work>[-1][children][:][name]` is made of following lexemes (spaces separating lexemes are optional):

a. `<Work>`: find within a JSON tree the **first** occurrence where the **JSON string** value is matching `"Work"` exactly
b. `[-1]`: **step up** one tier in the JSON tree structure (i.e. address an immediate parent of the found JSON element)
c. `[children]`: **select/address** a node whose label is `"children"` (it'll be a JSON array, at the same tier with `Work`)
d. `[:]`: select an **each node** in the array
e. `[name]`: select/address a node whose label is `"name"`

- subscript offsets are enclosed into square brackets `[`, `]` and may have different meaning:
  * simple numerical offsets (e.g.: `[0]`, `[5]`, etc) select/address a respective JSON immediate child in the addressed
node - a.k.a. numerical subscripts
  * slice/range offsets, expressed as `[N:N]` let selecting any slice/range of element in the array/object (any of `N` could be
  omitted in that notation)
  * numerical offsets proceeded with `+` make a path *iterable* - all children starting with the
given index will be selected (e.g.: [+2] will select/address all immediate children starting from 3rd one) - such notation is equivalent
of `[N:]`
  * numerical negative offsets (e.g.`[-1]`, `[-2]`, etc ) will select/address a parent of currently
selected/found node, a parent of a parent, etc
  * textual offsets (e.g. `[name]`, `[children]`, etc) select/address nodes with corresponding labels among
immediate children (i.e. textual subscripts)

_*** there's more on offsets and search quantifiers below_

in order to understand better how a walk path works, let's run a series of cli in a slow-motion, gradually adding lexemes
to the path, perhaps with the option `-l` to see also the labels (if any) of the selected elements:

```
bash $ jtc -w'<Work>' -l Bookmarks
"name": "Work"
```
```
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
```
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
```
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
```
bash $ jtc -w'<Work>[-1][children][:][name]' -l Bookmarks
"name": "Stack Overflow"
"name": "C++ reference"
```


### 3. dump all URL's names:
```
bash $ jtc -w'<url>l:[-1][name]' Bookmarks
"The New York Times"
"HuffPost UK"
"Digital Photography Review"
"Stack Overflow"
"C++ reference"
```
this walk path `<url>l:[-1][name]`:

 - finds recursively (encasement `<`, `>`) all (`:`) JSON elements with a label (`l`) matching `url`

 - then for an each found JSON element, select its parent (`[-1]`)

 - then, select a JSON element with the label `"name"` (encasement `[`, `]`)


### 4. dump all the URLs and their corresponding names, preferably wrap found pairs in JSON:
```
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
    - addressing children (by index/label) in _JSON iterables_ (_arrays_ and _objects_) - i.e. traverse JSON structure downward
    from the root (toward leaves), e.g.: `[2]`, `[id]` 
    - addressing parents (immediate and distant) - i.e. traverse JSON structure upwards, toward the the root (from leaves),
    e.g.:  `[-1]` (tier offset from the walked element), `[^2]` (tier offset from the root)
    - select ranges and slices of JSON elements in _JSON iterables_, e.g.: `[+2]`, `[:]`, `[:3]`, `[-2:]`, `[1:-1]` 
- Search lexemes (`<..>`, `>..<`) facilitate:
    - recursive (`<..>`) and non-recursive (`>..<`) matches
    - there're optional one-letter suffixes that may follow the lexemes (e.g.: `<..>Q`) which define type of search: (REGEX) string 
    search, (REGEX) label search, (REGEX) numerical, boolean, null, atomic, objects, arrays (or either), arbitrary JSONs, 
    unique, duplicates, etc.
    - there're also optional quantifiers to lexemes (must take the last position, after the suffix if one present) - let selecting
    match instance, or range of matches (e.g.: `<id>l3`- will match 4th (zero based) label `"id"`; if no quantifier present `0` 
    is assumed - first match)
- subscript lexemes could be joined with search lexemes over ':' to facilitate _scoped search_, e.g.: `[id]:<value>` is a single
   lexeme which will match recursively the first occurrence of the string `"value"` with the label `"id"` - i.e. `"id": "value"`
- Directives: there are a few suffixes which turn a search lexeme into a directive:
    - directives do not do any matching, instead they facilitate a certain action/operation with the currently walked JSON element,
    like: memorize it in the _namespace_, or erase from it, or memorize its label, or perform a _shell cli_ evaluation
    - couple directives (`<>f` and `<>F`) facilitate also walk branching

Refer to 
[`jtc` User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-json)
for the detailed explanation of the subscripts, search lexemes and directives.

### 6. Debugability / JSON validation
`jtc` is extensively debuggable: the more times option `-d` is given the more debugs will be produced (currently debug depth may go
as deep as 7: `-ddddddd`).
Enabling too many debugs might be overwhelming, though one specific case many would find extremely useful - when validating a failing JSON:
```
bash $ <addressbook-sampe.json jtc 
jtc json exception: expected_json_value
```
If JSON is big, it's desirable to locate the parsing failure point. Specifying just one `-d` let easily spotting the
parsing failure point and its locus:
```
bash $ <addressbook-sampe.json jtc -d
.read_inputs(), reading json from <stdin>
.parsejson(), exception locus: ...       ],|       "children": [,],|       "spouse": null|    }...
.location_(), exception spot: --------------------------------->| (offset: 967)
jtc json exception: expected_json_value
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
```
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

 cout << jin["AddressBook"].clear().push_back( move(srt) ) << endl;              // put back into the original container and print
}
```

Address Book JSON:
```
bash $ cat addressbook-sample.json
{
  "AddressBook": [
    {
       "Name": "John",
       "age": 25,
       "address": {
          "city": "New York",
          "street address": "599 Lafayette St",
          "state": "NY",
          "postal code": "10012"
       },
       "phoneNumbers": [
          {
             "type": "mobile",
             "number": "212 555-1234"
          }
       ],
       "children": [],
       "spouse": null
    },
    {
       "Name": "Ivan",
       "age": 31,
       "address": {
          "city": "Seattle",
          "street address": "5423 Madison St",
          "state": "WA",
          "postal code": "98104"
       },
       "phoneNumbers": [
          {
             "type": "home",
             "number": "3 23 12334"
          },
          {
             "type": "mobile",
             "number": "6 54 12345"
          }
       ],
       "children": [],
       "spouse": null
    },
    {
       "Name": "Jane",
       "age": 25,
       "address": {
          "city": "Denver",
          "street address": "6213 E Colfax Ave",
          "state": "CO",
          "postal code": "80206"
       },
       "phoneNumbers": [
          {
             "type": "office",
             "number": "+1 543 422-1231"
          }
       ],
       "children": [],
       "spouse": null
    }
  ]
}
bash $
```

Output result:
```
bash$ cat addressbook-sample.json | sort_ab
[
   [
      {
         "Name": "Ivan",
         "address": {
            "city": "Seattle",
            "postal code": "98104",
            "state": "WA",
            "street address": "5423 Madison St"
         },
         "age": 31,
         "children": [],
         "phoneNumbers": [
            {
               "number": "3 23 12334",
               "type": "home"
            },
            {
               "number": "6 54 12345",
               "type": "mobile"
            }
         ],
         "spouse": null
      },
      {
         "Name": "Jane",
         "address": {
            "city": "Denver",
            "postal code": "80206",
            "state": "CO",
            "street address": "6213 E Colfax Ave"
         },
         "age": 25,
         "children": [],
         "phoneNumbers": [
            {
               "number": "+1 543 422-1231",
               "type": "office"
            }
         ],
         "spouse": null
      },
      {
         "Name": "John",
         "address": {
            "city": "New York",
            "postal code": "10012",
            "state": "NY",
            "street address": "599 Lafayette St"
         },
         "age": 25,
         "children": [],
         "phoneNumbers": [
            {
               "number": "212 555-1234",
               "type": "mobile"
            }
         ],
         "spouse": null
      }
   ]
]
bash $
```
for the complete description of Json class interface, refer to [Json.hpp](https://github.com/ldn-softdev/jtc/Json.hpp)


## `jtc` vs **jq**:
`jtc` was _inspired_ by the complexity of **jq** interface (and its 
[DSL](https://en.wikipedia.org/wiki/Domain-specific_language)),
aiming to provide a user tool which would let attaining the desired result in a more feasible way

### utility ideology:
 - **jq** is a stateful processor with own DSL, variables, operations, control flow logic, IO system, etc, etc
 - `jtc` is a unix utility confining its functionality to operation types with its data model only (as per unix ideology). `jtc`
 performs one operation at a time and if successive operations required, then _cli_ to be daisy-chained

**jq** is non-idiomatic in a _unix way_, e.g., one can write a program in **jq** language that even has nothing to do with JSON.
Most of the requests (if not all) to manipulate JSONs are _ad hoc_ type of tasks, and learning **jq**'s DSL for _ad hoc_ type of tasks 
is an overkill (that purpose is best facilitated with
[GPL](https://en.wikipedia.org/wiki/General-purpose_language)).  
The number of asks on the
[stackoverflow](https://stackoverflow.com/) 
to facilitate even simple queries for **jq** is huge - that's the proof in itself that for many people feasibility of attaining their 
asks with **jq** is a way too low, hence they default to posting their questions on the forum.

`jtc` on the other hand is a utility (not a language), which employs a novel but powerful concept, which "embeds" the ask right into the
walk-path. That facilitates a much higher feasibility of attaining a desired result: building a walk-path a lexeme by lexeme, 
one at a time, provides an immediate visual feedback and let coming up with the desired result quite quickly.

### learning curve:
 - **jq**: before you could come up with a query to handle even a relatively simple ask, you need to become an expert in 
 **jq**'s language, which will take some time. Coming up with the complex queries requires it seems having a PhD in **jq**, or spending 
 lots of time on stackoverflow and similar forums
 - `jtc` employs only a single (but powerful) concept of the _walk-path_ (which is made only of 2 types of lexemes,
 each type though has several variants) which is easy to grasp.

### handling irregular JSONs:
 - **jq**: handling irregular JSONs for **jq** is not a challenge, building a query is! The more irregularities you need
 to handle the more challenging the query (**jq** program) becomes
 - `jtc` was conceived with the idea of being capable of handling complex irregular JSONs with a simplified interface - that all is
 fitted into the concept of the _walk-path_, while daisy-chaining multiple `jtc` operations it's possible to satisfy almost every ask. 


### programming model
 - **jq** is written in _C_, which drags all intrinsic problems the language has dated its creation
 - `jtc` is written in idiomatic _C++14_ using STL only. Main JSON engine/library does not have a single `new` operator,
 nor it has a single naked pointer acting as a resource holder/owner, thus` jtc` is guaranteed to be **free of memory leaks** 
 (at least one class of the problems is off the table) - STL guaranty.  
 Also, `jtc` is written in a very portable way, it should not cause any problems compiling it under any unix like system.
 
### performance:
here's 4+ million node JSON:
```
bash $ jtc -zz standard.json 
4329975
```
Table compares similar `jtc` and jq operations (using `TIMEFORMAT="user %U"`):

`jtc` | jq
---: | :---
parsing JSON | parsing JSON
`bash $ time jtc standard.json \| wc -l` | `bash $ time jq . standard.json \| wc -l`
` 7091578` | ` 7091578`
`user 11.195` | `user 24.685`



Refer to a complete [User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md) for further examples and guidelines.

##### Enhancement requests are more than welcome: *ldn.softdev@gmail.com*





