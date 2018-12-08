# jtc - cli tool to extract, manipulate and transform source JSON

jtc stand for: JSON test console, but it's a legacy name, don't get misleaded. jtc offers a powerful way to select one
or multiple elements from a source JSON and apply various actions on the selected elements at once (wrap selected elments into a new JSON, filter in/out, update elements, insert new elemetns or remove them or swap around). 

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
  - search lexems - encased into `<`, `>`: search lexems facilate either full match or Regex search.

Both types of lexemes are iterable - subscrips let iterating over children of currently addressed node,
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
- [macOS 64 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-macos-64.v1.41)
- [macOS 32 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-macos-32.v1.41)
- [linux 64 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-linux-64.v1.41)
- [linux 32 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-linux-32.v1.41)


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


#### Quick usage guide:
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




1. let's start with a simple thing: list all URLs:
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
- quantifier `+0` instructs to find all occurrences starting from the first (zero based),
such quantifiers (preceded with `+`) makes a path *iterable*




2. dump all bookmark names from the `Work` folder:
```
bash $ jtc -w "<Work> [-1] [children] [+0] [name]" Bookmarks
"Stack Overflow"
"C++ reference"
```
here the walk path `<Work>[-1][children][+0][name]` is made of following lexemes (spaces separating lexems are optional):

a. `<Work>`: first, find within a JSON tree a location where a string value is matching "Work" exactly

b. `[-1]`: step up one tier in JSON tree hierarchy (i.e. address an immediate parent of the found JSON element)

c. `[children]`: select a node whose label is "children" (it'll be a JSON array, at the same tier with `Work`)

d. `[+0]`: select each node in the array (starting from the first one - indexes are always zero based)

e. `[name]`: select a node whose label is "name"
- offsets are enclosed into square brackets `[`, `]` and may have different meaning:
  * numerical offsets  (e.g.: `[0]`, `[5]`, etc) select a respective JSON immediate child in the addressed
node - a.k.a. numerical subscripts
  * numerical offsets proceeded with `+` make a path *iterable* - all children starting with the
given index will be selected
  * numerical negative offsets (e.g.`[-1]`, `[-2]`, etc ) will select parent of currently
selected/found node, parent of a parent, etc
  * textual offsets (e.g. `[name]`, `[children]`, etc) select nodes with corresponding labels among
immediate children (i.e. textual subscripts)

in order to understand better how walk path works, let's run the series of cli, gradually adding lexemes
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




3. dump all URL's names:
```
bash $ jtc -w "<url>l+0 [-1] [name]" Bookmarks
"The New York Times"
"HuffPost UK"
"Digital Photography Review"
"Stack Overflow"
"C++ reference"
```
this walk path `<url>l+0 [-1] [name]`: 

 - finds (`<url>`) all (`+0`) JSON elements with label (`l`) matching `"url"`
 
 - then for each found JSON element its parent (`[-1]`) is selected

 - then JSON element with label `"name"` is selected (`[name]`) within parent's immediate children



4. dump all the URLs and their corresponding names, preferably wrap found pairs in JSON:
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
- option `-j` will wrap the walked outputs back into a JSON, but not just,
- option `-l` together with `-j` will ensure relevant walks are grouped together (try without `-l`)




5. There are 4 operations to modify source JSON:
- insert JSON entry `-i`
- update existing entries `-u` (if `-e` preceeds, update is subjected to shell interpolation)
- swap around 2 entries `-s` in every pair or walked paths (thus `-s` requires 2 walk paths) 
- remove (purge) walked entry `-p` (if mulitple `-p` given (e.g. `-pp`) then purge all entries except walked)

each of the above options would require a walk path (`-s` would require two) to operate on.

Say, let's delete all the stamps from the JSON:
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
- option `-f` would force such modification into the source JSON file, otherwise
the resulting JSON is only printed
- options `-i` and `-u` require an argument in a fully qualified JSON notation


let's do a reverse thing - delete everyting but the time stamps from the JSON (i.e. display only walked JSON elements):
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



Update operation (`-u`) optionally may undergo a shell evaluation (predicated by `-e`).
E.g., let's replace all the time-stamps in the original Bookmarks JSON with a number of
seconds since epoch:
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

Once options `-e` and `-u` used together following rules must be observed:
 - option `-e` must precede `-u`
 - char sequence following option `-u` must be terminated with escaped `;`
 - any occurrence of `{}` will be interpolated with JSON entry being updated
 - the cli chars in argument do not require any additional escaping (except those which would normally be required by shell)
 - if piping in the cli is required then pipe simbol itself needs to be escaped: `\|`
 - returned result of a shell evaluation still must be a valid JSON
 - failed or empty results of the shell evaluations are ignored (JSON entry wont be updated, rather
proceed to the next walked entry for another update attempt)




for more examples and a complete option list run *`jtc -h`* and *`jtc -g`*


#### A tiny example of class usage and its interface (c++14):
Say, we want to accomplish a following task:
1. read Address Book JSON from `<stdin>`
2. sort all records by `Name` (for simplicity, assume all records have that label)
3. output resulting Address Book JSON

Below is the code sample how that could be achieved using `Json.hpp` class and the srouce JSON - Address Book:
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
bash $ cat addressbook-sampe.json
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
bash$ cat addressbook-sampe.json | sort_ab
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
for the complete description of Json class interace, refer to [Json.hpp](https://github.com/ldn-softdev/jtc/blob/master/lib/Json.hpp) 




##### Enhancement requests are more than welcome: *ldn.softdev@gmail.com*



