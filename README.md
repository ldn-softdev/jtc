# jtc - JSON test console

#### Simple offline cli tool to manipulate JSON data format

jtc features following qualities:
  - simple user interface allowing applying bulk changes in one command
  - featured walk interface let extracting any combination of data from source JSON
  - extracted data is representable either as it found, or as a complete JSON format
  - support Regular Expressions when searching source JSON
  - fast and efficient processing very large JSON files (built-in search cache)
  - updates operations optionally undergo shell cli evaluation
  - written entirely in C++, no dependencies
  - extensively debuggable
  - conforms JSON specification ([json.org](http://json.org/index.html))


#### Linux and MacOS precompiled binaries are available for download

For compiling c++14 (or later) is required:
  - to compile under MacOS, use cli: `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - To compile under Linux, use cli: `c++ -o jtc -Wall -std=gnu++14 -static -Ofast jtc.cpp`

*pass `-DNDEBUG` flag if you like to compile w/o debugs, however it's unadvisable -
there's no performance gain from doing so*

or download latest precompiled binary:
- [macOS 64 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-macos-64.v1.27)
- [macOS 32 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-macos-32.v1.27)
- [linux 64 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-linux-64.v1.27)
- [linux 32 bit](https://github.com/ldn-softdev/jtc/raw/master/jtc-linux-32.v1.27)


#### Compile and install instructions:

download `jtc-master.zip`, unzip it, descend into unzipped folder, compile using
an appropriate command, move compiled file into an install location.

here's the example steps:
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
- search lexemes are enclosed in angular brackets `<`, `>`
- suffix `l` instructs to search among labels only (all suffixes: `r`,`R`,`l`,`L`,`d`,`D`,`b`,`n`)
  * `r`: default (could be omitted), fully matches *JSON string* value
  * `R`: same as `r`, but lexeme is a search RE
  * `l`: fully matches *JSON label*
  * `L`: same as `l`, but lexeme is a search RE
  * `d`: fully matches *JSON number*
  * `D`: same as `d`, but lexeme is an RE
  * `b`: matches JSON boolean value, lexeme must be spelled as `<true>b`, `<false>b`, or `<any>b`
  * `n`: matches JSON null value, lexeme value is ignored, could be like `<null>n`, or `<>n`
- quantifier `+0` instructs to find all occurrences starting from the first (zero based),
such quantifier (preceded with `+`) makes the path *iterable*




2. dump all bookmark names from the `Work` folder:
```
bash $ jtc -w "<Work>[-1][children][+0][name]" Bookmarks
"Stack Overflow"
"C++ reference"
```
a. `<Work>`: first find within JSON a location where a string is matching "Work"

b. `[-1]`: step up one tier in JSON tree hierarchy

c. `[children]`: select a node with the label "children" (it'll be a JSON array, at the same tier with `Work`)

d. `[+0]`: select each node in the array (stating from the first one, indexes are always zero
based)

e. `[name]`: select a node whose label is "name"
- offsets enclosed into square brackets `[`, `]` and may have different meaning:
  * numerical offsets  (e.g.: `[0]`, `[5]`, etc) select a respective JSON child in the addressed
node - a.k.a. numerical subscripts
  * numerical offsets proceeded with `+` make the path *iterable* - all children starting with the
given index will be selected
  * numerical negative offsets (e.g.`[-1]`, `[-2]`, etc ) will select parent of currently
selected/found node, parent of a parent, etc
  * text offsets (e.g. `[name]`, `[children]`, etc) select nodes with corresponding labels among
immediate children (i.e. textual subscripts)

in order to understand better how walk path works, run the series of cli gradually adding lexemes
to the path (perhaps with the option `-l` to see also the labels (if any) of the selected elements:

```
bash $ jtc -w "<Work>" -l Bookmarks
"name": "Work"
bash $ jtc -w "<Work>[-1]" -l Bookmarks
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
bash $ jtc -w "<Work>[-1][children]" -l Bookmarks
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
bash $ jtc -w "<Work>[-1][children][0]" -l Bookmarks
{
   "name": "Stack Overflow",
   "stamp": "2018-05-01, 12:05:19",
   "url": "https://stackoverflow.com/"
}
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
bash $ jtc -w "<Work>[-1][children][+0][name]" -l Bookmarks
"name": "Stack Overflow"
"name": "C++ reference"
```




3. dump all URL's names:
```
bash $ jtc -w "<url>l+0[-1][name]" Bookmarks
"The New York Times"
"HuffPost UK"
"Digital Photography Review"
"Stack Overflow"
"C++ reference"
```




4. dump all URLs and their corresponding names, preferably wrap found pairs in JSON:
```
bash $ jtc -w"<url>l+0" -w "<url>l+0[-1][name]" -jl Bookmarks
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
- option `-j` will wrap the walked outputs into a JSON, but not just
- option `-l` together with `-j` will ensure relevant walks are grouped together (try without `-l`)




5. There are 4 operations to modify source JSON:
- insert  entry `-i`
- update existing entry `-u`
- swap around 2 entries `-s`
- remove (purge) entry `-p`

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




Update operation (`-u`) optionally may undergo shell evaluation (predicated by `-e`).
E.g., let's replace all the time-stamps in the original Bookmarks JSON with a number of
seconds since epoch:
```
bash $ jtc -w'<stamp>l+0' -eu date -jf "'\"%F, %H:%M:%S\"'" {} "'+%s'" \; Bookmarks
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
 - all special characters subjected to shell interpretation (like `|`, `;`, `"`, `'`, etc)
 must be either escapted or quoted
 - returned result of shell evaluation still must be a valid JSON
 - failed or empty result of shell evaluations are ignored (JSON entry wont be updated, rather
proceed to the next walked entry)






for more examples and a complete option list run *`jtc -h`* and *`jtc -g`*


##### Enhancement requests are more than welcome: *ldn.softdev@gmail.com*



