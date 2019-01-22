
# jtc - cli tool to extract, manipulate and transform source JSON

jtc stand for: JSON test console, but it's a legacy name, don't get mislead. jtc offers a powerful way to select one
or multiple elements from a source JSON and apply various actions on the selected elements at once (wrap selected
elements into a new JSON, filter in/out, update elements, insert new elements or remove them or swap around).

#### Simple but efficient cli tool to manipulate JSON data

jtc offers following features:
  - simple user interface allowing applying bulk changes in one command
  - featured walk interface let extracting any combination of data from source JSON
  - extracted data is representable either as it found, or as a complete JSON format
  - support Regular Expressions when searching source JSON
  - fast and efficient processing very large JSON files (built-in search cache)
  - updates operations optionally undergo shell cli evaluation
  - written entirely in C++, no dependencies (STL only)
  - extensively debuggable
  - conforms JSON specification ([json.org](http://json.org/index.html))


Walk path is a feature easy to understand - it's only made of 2 types of lexemes:
  - subscripts - enclosed into `[`, `]`: subscripts let traversing JSON tree downwards **and upwards**
  - search lexemes - encased into `<`, `>`: search lexemes facilitate either full match or Regex search.

Both types of lexemes are iterable - subscript let iterating over children of currently addressed node,
while iterable search lexemes let iterating over all matches for a given search criteria.
A walk path is made of an arbitrary number of lexemes, while the tool accepts an unlimited number of walk
paths. See below more detailed explanation with examples



#### Linux and MacOS precompiled binaries are available for download

For compiling c++14 (or later) is required:
  - to compile under MacOS, use cli: `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - To compile under Linux, use cli: `c++ -o jtc -Wall -std=gnu++14 -static -Ofast jtc.cpp`

*pass `-DNDEBUG` flag if you like to compile w/o debugs, however it's unadvisable -
there's no performance gain from doing so*

or download the latest precompiled binary:
- [macOS 64 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-macos-64.v1.48)
- [macOS 32 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-macos-32.v1.48)
- [linux 64 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-linux-64.v1.48)
- [linux 32 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-linux-32.v1.48)


#### Compile and install instructions:

download `jtc-master.zip`, unzip it, descend into unzipped folder, compile using
an appropriate command, move compiled file into an install location.

here're the example steps (for MacOS):
  - say, `jtc-master.zip` has been downloaded to a folder and the terminal app is open in that
folder:
  - `unzip jtc-master.zip`
  - `cd jtc-master`
  - `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - `sudo mv ./jtc /usr/local/bin/`




## Quick usage guide:
*run `jtc -g` for walk path explanations and additional usage examples*

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




#### 1. let's start with a simple thing: list all URLs:
```
bash $ jtc -w "<url>l+0" Bookmarks
"https://www.nytimes.com/"
"https://www.huffingtonpost.co.uk/"
"https://www.dpreview.com/"
"https://stackoverflow.com/"
"https://en.cppreference.com/"
```
let's have a look at the walk-path `<url>l+0`:
- search lexemes are enclosed in angular brackets `<`, `>`
- suffix `l` instructs to search among labels only (all suffixes: `r`,`R`,`l`,`L`,`d`,`D`,`b`,`n`):
  * `r`: default (could be omitted), fully matches *JSON string* value
  * `R`: same as `r`, but the lexeme is a search RE
  * `l`: fully matches *JSON label*
  * `L`: same as `l`, but the lexeme is a search RE
  * `d`: fully matches *JSON number*
  * `D`: same as `d`, but the lexeme is an RE
  * `b`: matches JSON boolean value, the lexeme must be spelled as `<true>b`, `<false>b`, or `<any>b`
  * `n`: matches JSON null value, the lexeme value is ignored, could be something like `<null>n`, or `<>n`, etc
  * `a`: matches any JSON atomic value, i.e. strings, numerical, boolean, null
  * `o`: matches any JSON object - {...}
  * `i`: matches any JSON array (iterable) [...]
  * `j`: matches specified JSON value, the lexeme must be a valid JSON, e.g.: `<[]>j+0` - finds all empty arrays
- quantifier `+0` instructs to find all occurrences starting from the first (zero based),
such quantifiers (preceded with `+`) makes a path *iterable*




#### 2. dump all bookmark names from the `Work` folder:
```
bash $ jtc -w "<Work> [-1] [children] [+0] [name]" Bookmarks
"Stack Overflow"
"C++ reference"
```
here the walk path `<Work>[-1][children][+0][name]` is made of following lexemes (spaces separating lexemes are optional):

a. `<Work>`: find within a JSON tree the first location where the string value is matching `"Work"` exactly

b. `[-1]`: step up one tier in the JSON tree hierarchy (i.e. address an immediate parent of the found JSON element)

c. `[children]`: select/address a node whose label is `"children"` (it'll be a JSON array, at the same tier with `Work`)

d. `[+0]`: select an each node in the array (starting from the first one - indexes are always zero based)

e. `[name]`: select/address a node whose label is `"name"`
- subscript offsets are enclosed into square brackets `[`, `]` and may have different meaning:
  * simple numerical offsets (e.g.: `[0]`, `[5]`, etc) select/address a respective JSON immediate child in the addressed
node - a.k.a. numerical subscripts
  * numerical offsets proceeded with `+` make a path *iterable* - all children starting with the
given index will be selected (e.g.: [+2] will select/address all immediate children starting from 3rd one)
  * numerical negative offsets (e.g.`[-1]`, `[-2]`, etc ) will select/address a parent of currently
selected/found node, a parent of a parent, etc
  * textual offsets (e.g. `[name]`, `[children]`, etc) select/address nodes with corresponding labels among
immediate children (i.e. textual subscripts)
_*** there's more on offsets and search quantifiers below_

in order to understand better how a walk path works, let's run a series of cli, gradually adding lexemes
to the path, perhaps with the option `-l` to see also the labels (if any) of the selected elements:

```
bash $ jtc -w "<Work>" -l Bookmarks
"name": "Work"
```
```
bash $ jtc -w "<Work> [-1]" -l Bookmarks
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
bash $ jtc -w "<Work> [-1] [children]" -l Bookmarks
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
bash $ jtc -w "<Work> [-1] [children] [0]" -l Bookmarks
{
   "name": "Stack Overflow",
   "stamp": "2018-05-01, 12:05:19",
   "url": "https://stackoverflow.com/"
}
```
```
bash $ jtc -w "<Work>[-1][children][+0]" -l Bookmarks
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
bash $ jtc -w "<Work> [-1] [children] [+0] [name]" -l Bookmarks
"name": "Stack Overflow"
"name": "C++ reference"
```


#### 3. dump all URL's names:
```
bash $ jtc -w "<url>l+0 [-1] [name]" Bookmarks
"The New York Times"
"HuffPost UK"
"Digital Photography Review"
"Stack Overflow"
"C++ reference"
```
this walk path `<url>l+0 [-1] [name]`:

 - finds (encasement `<`, `>`) all (`+0`) JSON elements with a label (`l`) matching `"url"`

 - then for an each found JSON element, its parent (`[-1]`) is selected

 - then JSON element with label `"name"` is selected (encasement `[`, `]`) within parent's immediate children



#### 4. dump all the URLs and their corresponding names, preferably wrap found pairs in JSON:
```
bash $ jtc -w"<url>l+0" -w "<url>l+0 [-1] [name]" -jl Bookmarks
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
- if multiple walks (`-w`) are present, by default, walked results will be printed interleaved; option `-n` forces sequential
walk printing rather than interleaved.


#### 5. Subscripts (offsets) and Searches explained
##### _Subscript lexemes_
subscripts let addressing/selecting one or more JSON elements among immediate children (off the currently selected JSON element),
or address a parent(s). A full list of all types of subscripts supported by `jtc`:
- `[]` - addresses/selects an empty text label:
```
bash $ echo '{ "": "empty label", "label": "non-empty" }' | jtc -w'[]'
"empty lablel"
```
- `[text]` - addresses/selects a JSON value by label `text`:
```
bash $ echo '{ "": "empty label", "label": "non-empty" }' | jtc -w'[label]'
"non-empty"
```
- `[n]`, where n is a plain number - addresses/selects a JSON value by a numerical offset index, the index in jtc is always zero-based
(both JSON arrays and objects could be addressed, thought it's preferable to address objects by labels):
```
bash $ echo '{ "": "empty label", "label": "non-empty" }' | jtc -w'[1]'
"non-empty"
```
- `[-n]` - a negative offset let addressing parent(s) of the currently selected json element. `[-1]` will address immediate parent,
[-2] parent of a parent, etc:
```
bash $ echo '[ { "": "empty label", "label": "non-empty" } ]' | jtc -w'<non-empty> [-1]'
{
   "": "empty label",
   "label": "non-empty"
}
```
- `[^n]` - address a parent off the root (rather from the selected element). `[^0]` always select the root, `[^1]` will select
a first nested level, `[^2]` will do second, etc, up to the level of the selected JSON element.
Thus, the example above could be expressed then like this:
```
bash $ echo '[ { "": "empty label", "label": "non-empty" } ]' | jtc -w'<non-empty> [^1]'
{
   "": "empty label",
   "label": "non-empty"
}
```
- `[+n]` - numerical offset prepended with `+` will select all json elements in the iterable (array/object) starting off the
index `n` up till the end:
```
bash $ echo '[ 0, 1, 2, 3, 4, 5]' | jtc -w'[+3]'
3
4
5
```
- `[N:N]` - range select, allows selecting flexibly any slice of the json elements in the iterable. The first index denotes
beginning of the range/slice, the last one denotes the end index exclusively (i.e. not including the element denoted by
the last index itself)
  * `N` - positive `N` (could be prepended with '+') denotes an offset select _from the beginning_ of the iterable:
  ````
  bash $ echo '[ 0, 1, 2, 3, 4, 5]' | jtc -w'[2:4]'
  2
  3
  ````
  * `-N` - negative index denotes an offset select _from the end_ of the iterable. i.e. `-1` here addresses the last element,
  `-2` addressed 2nd element from the end, etc:
  ```
  bash $ echo '[ 0, 1, 2, 3, 4, 5]' | jtc -w'[-4:-2]'
  2
  3
  ```
  * either of indices could be omitted: first empty index indicates a beginning of the iterable, the last one indicates the end.
  Thus `[:]` will select all elements (it duplicates `[+0]` notation). Some more examples:
  ```
  bash $ echo '[ 0, 1, 2, 3, 4, 5]' | jtc -w'[:-4]'
  0
  1
  bash $ echo '[ 0, 1, 2, 3, 4, 5]' | jtc -w'[:2]'
  0
  1
  bash $ echo '[ 0, 1, 2, 3, 4, 5]' | jtc -w'[-2:]'
  4
  5
  bash $ echo '[ 0, 1, 2, 3, 4, 5]' | jtc -w'[4:]'
  4
  5
  ```
Finally, if any of numerical offsets does not result in a valid numerical value, such offset then is treated as
a text offset (btw, spaces within the offset also make the offset textual), e.g. following offsets is textual:
`[ +1 ]` - i.e., it will select an element with label `" +1 "` (if one exists of course)


##### _Search lexemes_
there are 2 types of search notations:
- enclosed like `<...>`: defines a _recursive_ search off the currently selected JSON element
- enclosed like `>...<`: defines a _non-recursive_ among immediate children of (the currently selected) JSON element

Compare:
```
bash $ echo '[ "a", "b", "c", ["d", "e", "f"]]' | jtc -w'<a>'
"a"
bash $ echo '[ "a", "b", "c", ["d", "e", "f"]]' | jtc -w'>a<'
"a"
bash $ echo '[ "a", "b", "c", ["d", "e", "f"]]' | jtc -w'<d>'
"d"
bash $ echo '[ "a", "b", "c", ["d", "e", "f"]]' | jtc -w'>d<'
bash $
```
Note, the last search yields no result, it's because there's no element `"d"` among root's immediate children.
The rest of the details applies to both search types, however notation `<...>` will be used only.

A full syntax for searches is: `<...>SN`, where:
- `S` is an _optional_ one letter suffix altering a search semantic (described earlier)
- `N` is an _optional_ quantifier, allowing selecting one or multiple successful search matches:
  * `n` - a plain number will instruct to select `n`th (zero based) encounter among all successful matches, by default `0` is assumed:
  ```
  bash $ echo '{ "a":"1", "b":"2", "c":"3", "d": { "e":"1", "f":"2", "g":"3" } }' | jtc -w'<1>' -l
  "a": "1"
  bash $ echo '{ "a":"1", "b":"2", "c":"3", "d": { "e":"1", "f":"2", "g":"3" } }' | jtc -w'<1>0' -l
  "a": "1"
  bash $ echo '{ "a":"1", "b":"2", "c":"3", "d": { "e":"1", "f":"2", "g":"3" } }' | jtc -w'<1>1' -l
  "e": "1"
  bash $ echo '{ "a":"1", "b":"2", "c":"3", "d": { "e":"1", "f":"2", "g":"3" } }' | jtc -w'<1>2' -l
  bash $
  ```
  * `+n` - explicitly marked a positive search allows selecting all successful matches starting from `n`'th one (zero based):
  ```
  bash $ echo '{ "a":"1", "b":"2", "c":"3", "d": { "e":"1", "f":"2", "g":"3" } }' | jtc -w'<1>+0' -l
  "a": "1"
  "e": "1"
  bash $ echo '{ "a":"1", "b":"2", "c":"3", "d": { "e":"1", "f":"2", "g":"3" } }' | jtc -w'<1>+1' -l
  "e": "1"
  bash $
  ```
  * `N:N` - allows expressing a range of selections (very much like in subscripts, but N cannot go negative here):
  ```
  bash $ echo '{ "a":"1", "b":"2", "c":"3", "d": { "e":"1", "f":"2", "g":"3" } }' | jtc -w'<>a2:5' -l
  "c": "3"
  "e": "1"
  "f": "2"
  bash $
  ```
  if index is not recognized as a valid in quantifier, the exception will be thrown
- optionally, a search could be combined with a label, that handles use-cases when a search is required to be performed among
specific labels, the notation then is like this `[label]:<...>` (emphasis is on the `:` between a subscript and a search lexemes),
e.g.:
  ```
  bash $ echo '{ "a":"1", "b":"2", "c":"3", "d": { "a":"4", "b":"5", "c":"6" } }' | jtc -w'[b]:<.*>R:' -l
  "b": "2"
  "b": "5"
  ```
in the above example `jtc` will search all JSON string values (with any content, as per RE), but match only those strings which
are directly attached to the label `"b"`


#### 6. There are 4 operations to modify source JSON:
- insert/merge JSON arrays/objects `-i`
- update existing entries `-u`
- swap around 2 entries `-s` in every pair or walked paths (thus `-s` requires exactly 2 walk paths)
- remove (purge) walked entry `-p` (if `-pp` given then purge all entries _except_ walked)

each of the above options would require a walk path (`-s` would require two) to operate on.


##### _Purge (delete) option:_
Say, let's delete (`-p`) all the stamps from the JSON:
```
bash $ jtc -w"<stamp>l+0" -p Bookmarks
{
   "Bookmarks": [
      {
         "children": [
            {
               "children": [
                  {
                     "name": "The New York Times",
                     "url": "https://www.nytimes.com/"
                  },
                  {
                     "name": "HuffPost UK",
                     "url": "https://www.huffingtonpost.co.uk/"
                  }
               ],
               "name": "News"
            },
            {
               "children": [
                  {
                     "name": "Digital Photography Review",
                     "url": "https://www.dpreview.com/"
                  }
               ],
               "name": "Photography"
            }
         ],
         "name": "Personal"
      },
      {
         "children": [
            {
               "name": "Stack Overflow",
               "url": "https://stackoverflow.com/"
            },
            {
               "name": "C++ reference",
               "url": "https://en.cppreference.com/"
            }
         ],
         "name": "Work"
      }
   ]
}
```
- option `-f` would enforce such modification into the source JSON file (unless the input was _stdin_), otherwise
the resulting JSON is only printed


let's do a reverse thing - delete everything but the time stamps from the JSON (i.e. display only walked JSON elements):
```
bash $ jtc -w"<stamp>l+0" -pp Bookmarks
{
   "Bookmarks": [
      {
         "children": [
            {
               "children": [
                  {
                     "stamp": "2017-10-03, 12:05:19"
                  },
                  {
                     "stamp": "2017-11-23, 12:05:19"
                  }
               ],
               "stamp": "2017-10-02, 12:05:19"
            },
            {
               "children": [
                  {
                     "stamp": "2017-02-27, 12:05:19"
                  }
               ],
               "stamp": "2017-02-27, 12:05:19"
            }
         ],
         "stamp": "2017-01-22, 12:05:19"
      },
      {
         "children": [
            {
               "stamp": "2018-05-01, 12:05:19"
            },
            {
               "stamp": "2018-06-21, 12:05:19"
            }
         ],
         "stamp": "2018-03-06, 12:07:29"
      }
   ]
}
bash $
```


##### _Update (replace) (`-u`) and insert (`-i`) options:_
Each of those options supports 4 types of parameters:
1. file (e.g.: `-i file.json`)
2. json-string (e.g.: `-i '{"pi": 3.14 }'`
3. cli line (e.g.: `-e -i date \| xargs -I% echo \"%\" \;` )
4. walk-path from source JSON (e.g.: `-i'<url>l+0 [-1] [name]'`)

\- every of those options parameters must represent a valid JSON.
\- the behavior for both options also could be modified by option `-m`
\- additionally, together with parameter type 4 an option `-p` could be used, which turns it into _move_ operation


##### _Insert (-i) option:_
By default, insert option allows inserting its parameters into arrays and objects only (indeed, one cannot _insert_
a value into an atomic value, for that overwrite operation is required, i.e. `-u`):
- new value will be pushed at the end of an array:
  ```
  bash $ echo '[ 1, 2 ]' | jtc -w'[-1]' -i'[3,4]' -r
  [ 1, 2, [ 3, 4 ] ]
  ```
  In the example, destination walk-path `-w'[-1]'` merely selects a root (other notations to address root could be:
  `-w' '`, `-w'[^0]'`), which is an array type, and the source JSON being inserted is a parameter of `-i`
- merging modifier (`-m`) alters that behavior allowing merging of destination and sources, it also allows merging
even into atomic types:
  * merging two arrays:
  ```
  bash $ echo '[ 1, 2 ]' | jtc -w'[-1]' -m -i'[3,4]' -r
  [ 1, 2, 3, 4 ]
  ```
  * merging object into array:
  ```
  bash $ echo '[ 1, 2 ]' | jtc -w'[-1]' -m -i'{"a":3,"b":4}' -r
  [ 1, 2, 3, 4 ]
  ```
  * merging array into (with) an atomic value:
  ```
  bash $ echo '[ 1, 2 ]' | jtc -w'[-1:]' -m -i'[3,4]' -r
  [ 1, [ 2, 3, 4 ] ]
  ```
  in the latter example JSON array `[3, 4]` is merged with the atomic element (`2`) pointed by the walk path `-w'[-1:]'`

- objects only could be merged, hence by default `-i` will merge two objects, however in case of clashing labels no
overwrite will occur (destination is preserved in that sense):
  * merging 2 objects with clashing labels:
  ```
  bash $ echo '{ "a":1, "b":2 }' | jtc -w'[-1]' -i'{ "b":3, "c":3 }' -r
  { "a": 1, "b": 2, "c": 3 }
  bash $
  ```
  * with `-m` modifier though, the values with clashing labels will be merged into a common array:
  ```
  bash $ echo '{ "a":1, "b":2 }' | jtc -w'[-1]' -mi'{ "b":3, "c":3 }' -r
  { "a": 1, "b": [ 2, 3 ], "c": 3 }
  bash $
  ```
- a real power adds ability to specify a walk-path in the source JSON as the parameter of the `-i` option - effectively it
let _copying_ portion of source JSON into another part of it, some examples:
  * duplicate all json entries:
  ```
  bash $ echo '[ null, 1, "2", { "a":1, "b":2 } ]' | jtc -w'[-1]' -i'[:]' -r
  [ null, 1, "2", { "a": 1, "b": 2 }, null, 1, "2", { "a": 1, "b": 2 } ]
  ```
  * extend the array with all its values (i.e. de-nest the object/arrays):
  ```
  bash $ echo '[ null, 1, "2", { "a":1, "b":2 } ]' | jtc -w'[-1]' -i'[:]' -r -m
  [ null, 1, "2", { "a": 1, "b": 2 }, null, 1, "2", 1, 2 ]
  ```
  * with `-p` added (while `-i`'s parameter is a walk-path), a _copy_ semantic turns into the _move_ - source-walked elements
  are removed, let's move all elements out of object into parent array:
  ```
  bash $ echo '[ null, 1, "2", { "a":1, "b":2 } ]' | jtc -w' ' -i[-1:][:]  -m -p -r
  [ null, 1, "2", {}, 1, 2 ]
  ```
  * the above step leaves an empty leftover from the object, which could be removed (purged) in a subsequent step:
  ```
  bash $ echo '[ null, 1, "2", { "a":1, "b":2 } ]' | jtc -w' ' -i[-1:][:]  -m -p | jtc -w'<{}>j:' -p -r
  [ null, 1, "2", 1, 2 ]
  ```


##### _Update (`-u`) option:_
Insert and update operations (`-i`, `-u`) optionally may undergo a shell evaluation (predicated by `-e`).
E.g., let's replace all the time-stamps in the original Bookmarks JSON with a number of
seconds since the epoch:
```
bash $ jtc -w'<stamp>l+0' -eu date -jf '"%F, %H:%M:%S"' {} +%s \; Bookmarks
{
   "Bookmarks": [
      {
         "children": [
            {
               "children": [
                  {
                     "name": "The New York Times",
                     "stamp": 1507025119,
                     "url": "https://www.nytimes.com/"
                  },
                  {
                     "name": "HuffPost UK",
                     "stamp": 1511435119,
                     "url": "https://www.huffingtonpost.co.uk/"
                  }
               ],
               "name": "News",
               "stamp": 1506938719
            },
            {
               "children": [
                  {
                     "name": "Digital Photography Review",
                     "stamp": 1488193519,
                     "url": "https://www.dpreview.com/"
                  }
               ],
               "name": "Photography",
               "stamp": 1488193519
            }
         ],
         "name": "Personal",
         "stamp": 1485083119
      },
      {
         "children": [
            {
               "name": "Stack Overflow",
               "stamp": 1525169119,
               "url": "https://stackoverflow.com/"
            },
            {
               "name": "C++ reference",
               "stamp": 1529575519,
               "url": "https://en.cppreference.com/"
            }
         ],
         "name": "Work",
         "stamp": 1520334449
      }
   ]
}
bash $
```
Once options `-e` and `-i`,`-u` used together following rules must be observed:
 - option `-e` must precede `-i`,`-u`
 - char sequence following option `-i`,`-u` must be terminated with escaped `;`
 - any occurrence of `{}` will be interpolated with JSON entry being updated (or where it's inserted)
 - the cli chars in argument do not require any additional escaping (except those which would normally be required by shell)
 - if piping in the cli is required then pipe symbol itself needs to be escaped: `\|`
 - returned result of a shell evaluation still must be a valid JSON
 - failed or empty results of the shell evaluations are ignored (JSON entry wont be updated, rather proceed to the next walked
 entry for another update attempt)

- Update operation overwrites the destination entirely:
  * even atomic value overwrites any json at destination:
  ```
  bash $ echo '[ 1, 2 ]' | jtc -w'[-1]' -u'3'
  3
  ```
  * objects by default are not merged:
  ```
  bash $ echo '{ "a":1, "b":2 }' | jtc -w'[-1]' -u'{ "c": 3 }' -r
  { "c": 3 }
  ```
- however merge option (`-m`) let altering that behavior (only for objects), now destination and source objects will get
merged, but the source will overwrite the destination:
  ```
  bash $ echo '{ "a":1, "b":2 }' | jtc -w'[-1]' -m -u'{ "b":3, "c":4 }' -r
  { "a": 1, "b": 3, "c": 4 }
  ```
- adding option `-p` (again, it's only useful while `-u`'s parameter is a walk-path) has the same effect as in case with `-i` option:
_copy_with_overwrite_ turns into _move_with_overwrite_:
  ```
  bash $ echo '[ 1, 2,{ "a":3, "b":4 }]' | jtc -w'[0]' -u'[-1:][-1:]' -r -p
  [ 4, 2, { "a": 3 } ]
  ```
  \- in that example, the first value of the root array (`1`), gets overwritten by a moved last value out of the last object
  (i.e. `"b": 4`), the label of course will be lost, as the array does not cater labels


##### _Swap option:_
Swap option requires strictly 2 walk paths (which in turn may be iterative) and will swap around all corresponding instances of
walk-paths (their iterations). In particular, swap operation is useful when it's required to reduce a (redundant) nestedness of
JSON structure:
```
bash $ echo '[ { "A": null }, { "A": true }, { "A": 2 }, { "A": "three" } ]' | jtc
[
   {
      "A": null
   },
   {
      "A": true
   },
   {
      "A": 2
   },
   {
      "A": "three"
   }
]
bash $ echo '[ { "A": null }, { "A": true }, { "A": 2 }, { "A": "three" } ]' | jtc -w'<A>l+0' -w'<A>l+0 [-1]' -s
[
   null,
   true,
   2,
   "three"
]
bash $
```


for more examples and a complete option list run *`jtc -h`* and *`jtc -g`*


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
for the complete description of Json class interface, refer to [Json.hpp](https://github.com/ldn-softdev/jtc/blob/master/lib/Json.hpp)

## jtc vs jq:
#### 1. tool positioning:
 - `jq` is a stateful processor with own DSL, variables, operations, control flow logic, IO system, etc, etc
 - `jtc` is a unixy utility confining its functionality (like most unix utilities do) only to operations with the data model only.
 The rest is out-tasked to unix cli tooling

that way `jtc` is never meant to surpass or even match `jq` in capabilities, but there is a flip side:

#### 2. learning curve:
 - `jq`: before you could come up with a query to handle even a relatively simple ask, you need to become an expert in jq's language,
 which will take some time. Coming up with the complex queries requires being an expert in jq, or spending lots of time on
 stackoverflow and similar
 - `jtc` on the other hand employs only a single (but powerful) concept of the _walk-path_ (which is made only of 2 types of lexemes,
 each type having several variants) which is easy to grasp. Once you learn it, you could achieve most of your asks with JSON
 very quickly, thus it's much easier and quicker to drive

#### 3. handling irregular JSONs:
 - `jq`: handling irregular JSONs for `jq` is not a challenge, building a query is! The more irregularities you need
 to handle the more challenging query becomes
 - `jtc` was incepted with the idea being capable of handling complex irregular JSONs with simplified interface - that all is fitted
 in the concept of the walk-path

#### 4. programming model
 - `jq` is written in _C_, which drags all intrinsic problems the language has dated its creation
 - `jtc` is written in idiomatic _c++14_ using STL only. Main json engine/library does not have a single `new` operator,
 nor it has a single naked pointer acting as a resource handler/owner, thus` jtc` is free of memory related bugs/issues
 (at least one class of the issues is off the table) - STL guaranty.

#### 5. performance
 - as per my benchmarking both `jq` and `jtc` have excellent performance (though `jtc` is a notch faster - if comparing apples to
 apples - capabilities that employ both tools)
- the rest of judgement is up to you!

##### Enhancement requests are more than welcome: *ldn.softdev@gmail.com*




