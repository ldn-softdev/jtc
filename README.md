# `jtc` - cli tool to extract, manipulate and transform source JSON

`jtc` stand for: **_JSON transformational chains_** (used to be _JSON test console_).  
  
`jtc` offers a powerful way to select one or multiple elements from a source JSON and apply various actions on the selected
elements at once (wrap selected elements into a new JSON, filter in/out, sort elements, update elements, insert new elements,
remove, copy, move, compare, transform, swap around and many other operations).

##### Enhancement requests and/or questions are more than welcome: *ldn.softdev@gmail.com*


### Content:
1. [Short description](https://github.com/ldn-softdev/jtc#short-description)
2. [Compilation and installation options](https://github.com/ldn-softdev/jtc#compilation-and-installation-options)
    * [Linux and MacOS precompiled binaries](https://github.com/ldn-softdev/jtc#linux-and-macos-precompiled-binaries-are-available-for-download)
    * [Installing via MacPorts](https://github.com/ldn-softdev/jtc#installing-via-macports)
    * [Installation on Linux distributions](https://github.com/ldn-softdev/jtc#installation-on-linux-distributions)
    * [Manual installation](https://github.com/ldn-softdev/jtc#manual-installation)
    * [Release Notes](https://github.com/ldn-softdev/jtc/releases)

2. [Quick-start guide](https://github.com/ldn-softdev/jtc#quick-start-guide)
    * [list all URLs](https://github.com/ldn-softdev/jtc#1-lets-start-with-a-simple-thing---list-all-urls)
    * [dump all bookmark names](https://github.com/ldn-softdev/jtc#2-dump-all-bookmark-names-from-the-work-folder)
    * [dump all URL's names](https://github.com/ldn-softdev/jtc#3-dump-all-urls-names)
    * [dump all the URLs and corresponding names](https://github.com/ldn-softdev/jtc#4-dump-all-the-urls-and-their-corresponding-names-preferably-wrap-found-pairs-in-json-array)
    * [Debugging and validating JSON](https://github.com/ldn-softdev/jtc#5-debugging-and-validating-json)
3. [Complete User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md)
4. [C++ class and interface usage primer](https://github.com/ldn-softdev/jtc/blob/master/Class%20usage%20primer.md)
5. [`jtc` vs **jq**](https://github.com/ldn-softdev/jtc#jtc-vs-jq)
     * [utility ideology](https://github.com/ldn-softdev/jtc#utility-ideology)
     * [learning curve](https://github.com/ldn-softdev/jtc#learning-curve)
     * [handling irregular JSONs](https://github.com/ldn-softdev/jtc#handling-irregular-jsons)
     * [solutions input invariance](https://github.com/ldn-softdev/jtc#solutions-input-invariance)
     * [programming model](https://github.com/ldn-softdev/jtc#programming-model)
     * [JSON numerical fidelity](https://github.com/ldn-softdev/jtc#json-numerical-fidelity)
     * [performance](https://github.com/ldn-softdev/jtc#performance)
     * [compare `jtc` based solutions with *jq*'s](https://github.com/ldn-softdev/jtc#compare-jtc-based-solutions-with-jqs)

## Short description
\- `jtc` is a simple yet very powerful and efficient cli utility tool to process and manipulate JSON data

**`jtc` offers following features (a short list of main features)**:
  - simple user interface allowing applying a **_bulk of changes_** in a single or chained sets of commands 
  - featured **_walk-path_** interface lets extracting any combination of data from sourced JSON trees
  - extracted data is representable either as found, or could be encapsulated in JSON array/object or transformed using **_templates_**
  - support **_Regular Expressions_** when searching source JSON
  - fast and efficient processing of very large JSON files (**_various built-in search caches_**)
  - insert/update operations optionally may undergo **_shell cli evaluation_**
  - support **_in-place modifications_** of the input/source JSON file
  - features **_namespaces_**, facilitating interpolation of preserved JSON values in templates
  - supports **_buffered_** and **_streamed_** modes of input read
  - sports **_concurrent_** input JSON reading/parsing (on multi-core CPU)
  - written entirely in **_C++14_**, no dependencies (STL only, idiomatic C++, **_no memory leaks_**)
  - extensively debuggable
  - conforms JSON specification ([json.org](http://json.org/index.html))

The _walk-path_ feature is easy to understand - it's only made of 2 kinds of lexemes traversing JSON tree,
which could be mixed up in any order:
  - **_subscripts_** - enclosed into `[`, `]`, subscripts let traversing JSON tree downwards (towards the leaves) and 
  **upwards** (towards the root)
  - **_search lexemes_** - encased as `<..>` or `>..<` (for a recursive and non-recursive searches respectively); search lexemes
  facilitate various match criteria defined by an optional suffix and/or quantifier
>There's also a 3rd kind of lexemes - **_directives_**: they typically facilitate other functions like working with _namespaces_,
controlling walk-path execution, etc; _directives_ are syntactically similar to the _search lexemes_    

All lexemes can be _iterable_:
  - iterable **_subscripts_** let iterating over children of currently addressed JSON iterables nodes (arrays/objects), 
  - while iterable **_search lexemes_** let iterating over all (recursive) matches for a given search criteria

A _walk-path_ may have an arbitrary number of lexemes -the tool accepts a virtually unlimited number of walk
paths. See below more detailed explanation with examples

## Compilation and installation options
For compiling, **`c++14`** (or later) is required. To compile under different platforms:
  - **MacOS/BSD**:
    - `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - **Linux**:
    - non-relocatable (_dynamically_ linked) image:
      - `c++ -o jtc -Wall -std=gnu++14 -Ofast -pthread -lpthread jtc.cpp`
    - relocatable (_statically_ linked) image: 
      - `c++ -o jtc -Wall -std=gnu++14 -Ofast -static -Wl,--whole-archive -lrt -pthread -lpthread -Wl,--no-whole-archive jtc.cpp`
  - **Debian**:
    - `c++ -o jtc -Wall -std=c++14 -pthread -lpthread -Ofast jtc.cpp` (ensure `c++` poits to `clang++-6.0` or above)

Following debug related flags could be passed to `jtc` when compiling:
- `-DNDEBUG`: compile w/o debugs, however it's unadvisable - there's no performance gain from doing so
- `-DNDBG_PARSER`: disable debugs coming from parsing JSON (handy when deep debugging huge JSONs and want to skip parsing debugs) 
- `-DBG_FLOW`: all debuggable function/method calls will disply an entry and exit points
- `-DBG_mTS`, `-DBG_uTS`: display absolute time-stamps in the debug: with millisecond accuracy and with microsecond accuracy respectively
- `-DBG_dTS`: used with either of 2 previous flags: makes time-stamp to display delta (since last debug message) instead of absolute stamp
- `-DBG_CC`: every call to a copy-constructor in `Jnode` class will reveal itself (handy for optimization debugging)

#### Linux and MacOS precompiled binaries are available for download
Choose the latest **precompiled binary**:
- _latest_ [macOS](https://github.com/ldn-softdev/jtc/releases/download/LatestBuild/jtc-macos-64.latest)
- _latest_ [linux 64 bit](https://github.com/ldn-softdev/jtc/releases/download/LatestBuild/jtc-linux-64.latest)
- _latest_ [linux 32 bit](https://github.com/ldn-softdev/jtc/releases/download/LatestBuild/jtc-linux-32.latest)

Rename the downloaded file and give proper permissions. E.g., for the latest macOS:
```
mv jtc-macos-64.latest jtc
chmod 754 jtc
```

#### Packaged installations:
##### Installing via MacPorts
On MacOS, you can install `jtc` via the [MacPorts](https://macports.org) package manager:
``` ShellSession
$ sudo port selfupdate
$ sudo port install jtc
```
##### Installation on Linux distributions
`jtc` is packaged in the following Linux distributions and can be installed via the package manager.
  - **Fedora**: `jtc` is present in Fedora 31 and later:
``` ShellSession
$ dnf install jtc
```
  - **openSUSE**: `jtc` can be installed on openSUSE Tumbleweed via `zypper`:
``` ShellSession
$ zypper in jtc
```
or on Leap 15.0 and later by adding the
[utilities](https://build.opensuse.org/project/show/utilities) repository and installing `jtc` via zypper.

#### Manual installation:
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

* run the command `jtc -g` to read the mini USER-GUIDE, with walk path syntax, usage notes, short examples
* read the examples just below
* see [stackoverflow-json](https://github.com/ldn-softdev/stackoverflow-json) for lots of worked examples
  based on Stack Overflow questions
* refer to the complete [User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md) for further examples and guidelines.

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

#### 1. let's start with a simple thing - list all URLs:
```bash
bash $ jtc -w'<url>l:' Bookmarks
"https://www.nytimes.com/"
"https://www.huffingtonpost.co.uk/"
"https://www.dpreview.com/"
"https://stackoverflow.com/"
"https://en.cppreference.com/"
```
Let's take a look at the walk-path **`<url>l:`**:
- search lexemes are enclosed in angular brackets `<`, `>` - that style provides a **recursive search** throughout JSON 
- suffix **`l`** instructs to search among **labels** only
- quantifier **`:`** instructs to find **all occurrences**, such quantifiers makes a path *iterable*


#### 2. dump all bookmark names from the `Work` folder:
```bash
bash $ jtc -w'<Work>[-1][children][:][name]' Bookmarks
"Stack Overflow"
"C++ reference"
```
Here the walk-path **`<Work>[-1][children][:][name]`** is made of following lexemes:

a. **`<Work>`**: find within a JSON tree the **first** occurrence where the **JSON string** value is matching **`"Work"`** exactly  
b. **`[-1]`**: **step up** one tier in the JSON tree structure (i.e., address an immediate parent of the found JSON element)  
c. **`[children]`**: **select/address** a node whose label is **`"children"`** (it'll be a JSON array, at the same tier with `Work`)  
d. **`[:]`**: select **each node** in the array  
e. **`[name]`**: select/address a node with the label **`"name"`** 


in order to understand better how the walk-path works, let's run that series of cli in a slow-motion, gradually adding lexemes
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
> B.t.w., a better (a bit faster and more efficient) walk-path achieving the same query would be this:  
>  - `jtc -w'<Work>[-1][children]<name>l:' Bookmarks`

#### 3. dump all URL's names:
```bash
bash $ jtc -w'<url>l:[-1][name]' Bookmarks
"The New York Times"
"HuffPost UK"
"Digital Photography Review"
"Stack Overflow"
"C++ reference"
```
this walk-path **`<url>l:[-1][name]`**:

 - finds recursively (encasement *`<`*, *`>`*) each (*`:`*) JSON element with a label (*`l`*) matching **`url`**
 - then for an each found JSON element, select its parent (**`[-1]`**)
 - then, select a JSON (sub)element with the label **`"name"`**


#### 4. dump all the URLs and their corresponding names, preferably wrap found pairs in JSON array:
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
- option **`-j`** will wrap the walked outputs into a JSON array, but not just,
- option **`-l`** used together with `-j` will ensure relevant walks are grouped together (try without `-l`)
- if multiple walks (**`-w`**) are present, by default, walked results will be printed interleaved (if it can be interleaved)

#### 5. Debugging and validating JSON
`jtc` is extensively debuggable: the more times option `-d` is passed the more debugs will be produced.
Enabling too many debugs might be overwhelming, though one specific case many would find extremely useful - when validating
a failing JSON:
```bash
bash $ <addressbook-sample.json jtc 
jtc json parsing exception (<stdin>:263): expected_json_value
```
If JSON is big, it's desirable to locate the parsing failure point. Passing just one `-d` let easily spotting the
parsing failure point and its locus:
```bash
bash $ <addressbook-sample.json jtc -d
.display_opts(), option set[0]: -d (internally imposed: )
.init_inputs(), reading json from <stdin>
.exception_locus_(), ...    "age": 31,|        "children": [,],|        "phoneNumbers": [|     ...
.exception_spot_(), --------------------------------------->| (offset: 263)
jtc json parsing exception (<stdin>:263): expected_json_value
bash $ 
```

## Complete User Guide
there's a lot more under the hood of `jtc`:
- various viewing options, 
- directives allowing controlling walks, preserving parts of whole JSONs in namespaces, walking with various criteria, etc
- interpolating namespaces and walk results in templates and lexemes
- amending input JSONs via purge/swap/update/insert/move/merge operations
- comparing JSONs (or their parts) or their schemas
- various processing modes (streamed, buffered, concurrent parsing, chaining operations, etc)
- and more ...

Refer to a complete [User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md) for further examples and guidelines.

## C++ class and interface usage primer
Refer to a [Class usage primer](https://github.com/ldn-softdev/jtc/blob/master/Class%20usage%20primer.md) document.


## `jtc` vs **jq**:
`jtc` was _inspired_ by the complexity of **jq** interface (and its 
[DSL](https://en.wikipedia.org/wiki/Domain-specific_language)),
aiming to provide users a tool which would let attaining the desired JSON queries in an easier, more feasible and succinct way


#### utility ideology:
 - **jq** is a stateful processor with own DSL, variables, operations, control flow logic, IO system, etc, etc
 - `jtc` is a unix utility confining its functionality to operation types with its data model only (as per unix ideology). `jtc`
 performs one major operation at a time (like insertion, update, swap, etc), however multiple operations could be chained
 using `/` delimiter

**jq** is non-idiomatic in a _unix way_, e.g.: one can write a program in **jq** language that even has nothing to do with JSON.
Most of the requests (if not all) to manipulate JSONs are _ad hoc_ type of tasks, and learning **jq**'s DSL for _ad hoc_ type of tasks 
is an overkill (that purpose is best facilitated with
[GPL](https://en.wikipedia.org/wiki/General-purpose_language), e.g.: _Python_).  
The number of asks on the
[stackoverflow](https://stackoverflow.com/) 
to facilitate even simple queries for **jq** is huge - that's the proof in itself that for many people feasibility of attaining their 
asks with **jq** is a way too low, hence they default to posting their questions on the forum.

`jtc` on the other hand is a utility (not a language), which employs a novel but powerful concept, which "embeds" the ask right into the
_walk-path_. That facilitates a much higher feasibility of attaining a desired result: building a walk-path a lexeme by lexeme, 
one at a time, provides an immediate visual feedback and let coming up with the desired result rather quickly.


#### learning curve:
 - **jq**: before you could come up with a query to handle even a relatively simple ask, you need to become an expert in 
 **jq** language, which will take some time. Coming up with the complex queries requires what it seems having a PhD in **jq**, or spending 
 lots of time on stackoverflow and similar forums
 - `jtc` employs only a simple (but powerful) concept of the _walk-path_ (which is made only of 2 types of search lexemes,
 each type though has several variants) which is quite easy to grasp.


#### handling irregular JSONs:
 - **jq**: handling irregular JSONs for **jq** is not a challenge, building a query is! The more irregularities you need
 to handle the more challenging the query (**jq** program) becomes
 - `jtc` was conceived with the idea of being capable of handling complex irregular JSONs with a simplified interface - that all is
 fitted into the concept of the _walk-path_, while daisy-chaining multiple operations is possible to satisfy almost every ask. 


#### solutions input invariance
\- most of `jtc` solutions would be input invariant (hardly the same could be stated for **jq**). Not that it's impossible to come up
with invariant solutions in **jq**, it's just a lot more harder, while `jtc` with its walk-path model prompts for invariant solutions.
I.e., the invariant solution will keep working even once the JSON outer format changes (the invariant solution only would stop working 
once the relationship between walked JSON elements changes).  
E.g.: consider a following query, extract format `[ "name", "surname" ]` from 2 types of JSON:
```bash
bash $ case1='{"Name":"Patrick", "Surname":"Lynch", "gender":"male", "age":29}'
bash $ case2='[{"Name":"Patrick", "Surname":"Lynch", "gender":"male", "age":29},{"Name":"Alice", "Surname":"Price", "gender":"female", "age":27}]'
```
a natural, idiomatic `jtc` solution would be:
```bash
bash $ <<<$case1 jtc -w'<Name>l:<N>v[-1]' -rT'[{{$a}},{{$b}}]'
[ "Patrick", "Lynch" ]
bash $ <<<$case2 jtc -w'<Name>l:<N>v[-1]' -rT'[{{$a}},{{$b}}]'
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
bash $ case3='{"root":[{"Name":"Patrick", "Surname":"Lynch", "gender":"male", "age":29}, {"closed circle":[{"Name":"Alice", "Surname":"Price", "gender":"female", "age":27}, {"Name":"Rebecca", "Surname":"Hernandez", "gender":"female", "age":28}]}]}'
bash $ 
bash $ <<<$case3 jtc -w'<Name>l:<N>v[-1]' -rT'[{{$a}},{{$b}}]'
[ "Patrick", "Lynch" ]
[ "Alice", "Price" ]
[ "Rebecca", "Hernandez" ]
#
#jq:
bash $ <<<$case3 jq -c 'if type == "array" then .[] else . end | [.Name, .Surname]'
[null,null]
```
The same property makes `jtc` solutions resistant to cases of incomplete data, e.g.: if we  drop `"Name"` entry from one of the 
entries in case 2, `jtc` solution still works correctly:
```bash
#jtc:
bash $ case2='[{"Surname":"Lynch", "gender":"male", "age":29},{"Name":"Alice", "Surname":"Price", "gender":"female", "age":27}]'
bash $ 
bash $ <<<$case2 jtc -w'<Name>l:<N>v[-1]' -rT'[{{$a}},{{$b}}]'
[ "Alice", "Price" ]
#
#jq:
bash $ <<<$case2 jq -c 'if type == "array" then .[] else . end | [.Name, .Surname]'
[null,"Lynch"]
["Alice","Price"]
```
\- i.e., `jtc` will not assume that user would require some default substitution in case of incomplete data (but if such handling is 
required then the walk-path can be easily enhanced)


#### programming model
 - **jq** is written in _C_ language, which drags all intrinsic problems the language has dated its creation
 ([here's what I mean](https://github.com/ldn-softdev/cpluspus-vs-c))
 - `jtc` is written in the idiomatic _C++14_ using STL only. `jtc` does not have a single naked memory allocation operator
 (those few `new` operators required for legacy interface are implemented as _guards_),
 nor it has a single naked pointer acting as a resource holder/owner, thus `jtc` is guaranteed to be **free of memory/resources leaks** 
 (at least one class of the problems is off the table) - **STL guaranty**.  
 Also, `jtc` is written in a very portable way, it should not cause problems compiling it under any unix like system.


#### JSON numerical fidelity:
 - **jq** is not compliant with JSON numerical definition. What jq does, it simply converts a symbolic numerical representation to an 
 internal binary and keeps it that way. That approach:
     - is not compliant with JSON definition of the numerical values
     - it has problems retaining required precision
     - might change original representation of numericals
     - leads to incorrect processing of some JSON streams
 - `jtc` validates all JSON numericals per JSON standard and keep numbers internally in their original literal format, so it's free of
 all the above caveats, compare:
 
 Handling | `jtc` | **jq 1.6** 
 --- | ---: | :---
Invalid Json: `[00]` | `<<<'[00]' jtc` | `<<<'[00]' jq -c .`
_Parsing result_ | `jtc json parsing exception (<stdin>:3): missed_prior_enumeration` | `[0]`
Precision test: | `<<<'[0.99999999999999999]' jtc -r` | `<<<'[0.99999999999999999]' jq -c .`
_Parsing result_ | `[ 0.99999999999999999 ]` | `[1]`
Retaining original format: | `<<<'[0.00001]' jtc -r` | `<<<'[0.00001]' jq -c .`
_Parsing result_ | `[ 0.00001 ]` | `[1e-05]`
Stream of atomic JSONs: | `<<<'{}[]"bar""foo"00123truefalsenull' jtc -Jr` | `<<<'{}[]"bar""foo"00123truefalsenull' jq -sc`
_Parsing result_ | `[ {}, [], "bar", "foo", 0, 0, 123, true, false, null ]` | `parse error: Invalid numeric literal at line 2, column 0`


#### performance:
 - **jq** is a single-threaded process
 - `jtc` engages a _concurrent_ (multi-threaded) reading/parsing when multiple files given (the advantage could be observed on multi-core
 CPU, though it become noticeable only with relatively big JSONs or with relatively big number of files processed)

**_Comparison of single-threaded performance:_**  
Here's a 4+ million node JSON file [standard.json](https://github.com/ldn-softdev/jtc/releases/download/standard.json/standard.json):
```
bash $ time jtc -zz standard.json 
4329975
user 6.085 sec
```
The table below compares `jtc` and jq performance for similar operations (using `TIMEFORMAT="user %U sec"`):

`jtc` 1.76 | **jq** 1.6
---: | :---
_**`parsing JSON:`**_ | _**`parsing JSON:`**_
`bash $ time jtc -t2 standard.json \| md5` | `bash $ time jq -M . standard.json \| md5`
`d3b56762fd3a22d664fdd2f46f029599` | `d3b56762fd3a22d664fdd2f46f029599`
`user 9.110 sec` | `user 18.853 sec`
_**`removing by key from JSON:`**_ | _**`removing by key from JSON:`**_
`bash $ time jtc -t2 -pw'<attributes>l:' standard.json \| md5` | `bash $ time jq -M 'del(..\|.attributes?)' standard.json \| md5`
`0624aec46294399bcb9544ae36a33cd5` | `0624aec46294399bcb9544ae36a33cd5`
`user 10.027 sec` | `user 27.439 sec`
_**`updating JSON recursively by label:`**_ | _**`updating JSON recursively by label:`**_
`bash $ time jtc -t2 -w'<attributes>l:[-1]' -i'{"reserved": null}' standard.json \| md5` | `bash $ time jq -M 'walk(if type == "object" and has("attributes") then . + { "reserved" : null } else . end)' standard.json \| md5`
`6c86462ae6b71e10e3ea114e86659ab5` | `6c86462ae6b71e10e3ea114e86659ab5`
`user 12.715 sec` | `user 29.450 sec`


**_Comparison of `jtc` to `jtc` (single-threaded to multi-threaded parsing performance):_**
```bash SKIP
bash $ unset TIMEFORMAT
bash $ 
bash $ # concurrent (multi-threaded) parsing:
bash $ time jtc -J / -zz  standard.json standard.json standard.json standard.json standard.json 
21649876

real	0m10.995s     # <- compare these figures
user	0m34.083s
sys	0m3.288s
bash $ 
bash $ # sequential (single-threaded) parsing:
bash $ time jtc -aJ / -zz  standard.json standard.json standard.json standard.json standard.json 
21649876

real	0m31.717s     # <- compare these figures
user	0m30.125s
sys	0m1.555s
bash $ 
```


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

### compare `jtc` based solutions with *jq*'s:
[Here](https://github.com/ldn-softdev/stackoverflow-json/blob/master/README.md)
are published some answers for JSON queries using `jtc`, you may compare those with `jq`'s, as well as study the 
feasibility of the solutions, test relevant performance, etc


Refer to a complete [User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md) for further examples and guidelines.






