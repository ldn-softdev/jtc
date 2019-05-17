


# [`jtc`](https://github.com/ldn-softdev/jtc). Examples and Use-cases (_v1.67_)

1. [Displaying JSON](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#displaying-json)
   * [Pretty printing (`-t`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#pretty-printing)
   * [Compact printing (`-r`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#compact-printing)
   * [Printing JSON size (`-z`, `-zz`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#printing-json-size)
   * [Validating JSON (`-d`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#validating-json)
   * [Forcing strict solidus parsing (`-q`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#forcing-strict-solidus-parsing)
   * [Unquoting JSON strings (`-qq`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#unquoting-JSON-strings)
   * [Stringifying JSON (`-rr`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json) 
2. [Walking JSON (`-w`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-json)
   * [Walking with subscripts (`[..]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-with-subscripts-offset-lexemes)
     * [Selecting multiple subscripted JSON elements (`[+n], [n:n]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#selecting-multiple-subscripted-json-elements)
   * [Searching JSON (`<..>`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#searching-json)
     * [Searching JSON with RE (`<..>R`,`<..>L`, `<..>D`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#searching-json-with-re)
     * [Search suffixes (`rRdDbnlLaoicewjstqQ`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-suffixes)
     * [Directives and Namespaces (`vkzf`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#directives-and-namespaces)
     * [Fail-stop and Forward directives (`<..>f`, `<..>F`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#fail-stop-and-Forward-directives)
     * [RE generated namespaces (`$0`, `$1`, etc)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#re-generated-namespaces)
     * [Path namespaces (`$PATH`, `$path`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#path-namespaces)
     * [Search quantifiers (`n`,`+n`,`n:n`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers)
     * [Search quantifiers with relative offset semantic (`>..<t`, `>..<l`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers-with-relative-offset-semantic)
     * [Scoped search `[..]:<..>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#scoped-search)
     * [Non-recursive search (`>..<`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#non-recursive-search)
   * [Addressing parents (`[-n]`, `[^n]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#addressing-parents)
   * [Walking multiple paths](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-multiple-paths)
     * [Sequential walk processing (`-n`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#sequential-walk-processing)
     * [Displaying walks with labels (`-l`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#displaying-walks-with-labels)
     * [Wrapping resulted walks to JSON (`-j, -jj`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-resulted-walks-to-json)
     * [Interleaved walk processing](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interleaved-walk-processing)
     * [Succinct walk-path syntax (`-x`,`-y`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#succinct-walk-path-syntax)
3. [Modifying JSON](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#modifying-json)
   * [In-place JSON modification (`-f`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#in-place-json-modification)
   * [Purging JSON (`-p`, `-pp`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#purging-json)
   * [Swapping JSON elements (`-s`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#swapping-json-elements)
   * [Insert operations (`-i`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-operations)
     * [Destination driven insertion](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#destination-driven-insertion)
     * [Inserting objects into objects](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#inserting-objects-into-objects)
     * [Insertion matrix without merging](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insertion-matrix-without-merging)
     * [Insertion matrix with merging (`-m`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insertion-matrix-with-merging)
   * [Update operations (`-u`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#update-operations)
     * [Update operations matrix](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#update-operations-matrix)
     * [Updating labels](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#updating-labels)
   * [Insert, Update with move (`-i`/`-u`,`-p`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-with-move)
   * [Insert, Update: argument shell evaluation (`-e`,`-i`/`-u`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation)
   * [Mixed use of arguments for `-i`, `-u`, `-c` (e.g.: `jtc -u<JSON> -u<walk-path>`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#mixed-use-of-arguments-for--i--u--c)
   * [Mixed use of arguments with `-e`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#mixed-use-of-arguments-with--e)
   * [Summary of modes of operations](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#summary-of-modes-of-operations)
4. [Comparing JSONs (`-c`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#comparing-jsons)
   * [Comparing JSON schemas](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#comparing-json-schemas)
5. [Interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation)
   * [Search quantifiers interpolation (`<..>{..}`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers-interpolation)
6. [Templates (`-T`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates)
7. [Processing multiple input JSONs](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#processing-multiple-input-jsons)
   * [Process all input JSONs (`-a`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#process-all-input-jsons)
   * [Wrap all processed JSONs (`-J`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrap-all-processed-jsons)
8. [More Examples](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#more-examples)
   * [Generating new JSON (1)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#generating-new-json-1)
   * [Generating new JSON (2)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#generating-new-json-2)
   * [Taming duplicates (`<..>q`, `<..>Q`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#taming-duplicates)
     * [Remove duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#remove-duplicates)
     * [Remove all but duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#remove-all-but-duplicates)
     * [Leave only those which have no duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#leave-only-those-which-have-no-duplicates)
     * [Leave all duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#leave-all-duplicates)
---

## Displaying JSON
### Pretty printing
if no argument given, `jtc` will expect an input JSON from the `<stdin>`, otherwise JSON is read from the file pointed by the argument.
`jtc` will parse and validate input JSON and upon a successful validation will output:
```
bash $ <ab.json jtc
{
   "Directory": [
      {
         "address": {
            "city": "New York",
            "postal code": 10012,
            "state": "NY",
            "street address": "599 Lafayette St"
         },
         "age": 25,
         "children": [
            "Olivia"
         ],
         "name": "John",
         "phone": [
            {
               "number": "112-555-1234",
               "type": "mobile"
            },
            {
               "number": "113-123-2368",
               "type": "mobile"
            }
         ],
         "spouse": "Martha"
      },
      {
         "address": {
            "city": "Seattle",
            "postal code": 98104,
            "state": "WA",
            "street address": "5423 Madison St"
         },
         "age": 31,
         "children": [],
         "name": "Ivan",
         "phone": [
            {
               "number": "273-923-6483",
               "type": "home"
            },
            {
               "number": "223-283-0372",
               "type": "mobile"
            }
         ],
         "spouse": null
      },
      {
         "address": {
            "city": "Denver",
            "postal code": 80206,
            "state": "CO",
            "street address": "6213 E Colfax Ave"
         },
         "age": 25,
         "children": [
            "Robert",
            "Lila"
         ],
         "name": "Jane",
         "phone": [
            {
               "number": "358-303-0373",
               "type": "office"
            },
            {
               "number": "333-638-0238",
               "type": "home"
            }
         ],
         "spouse": "Chuck"
      }
   ]
}
bash $
```
option `-t` controls the indentation of the pretty-printing format (default is 3 white spaces):
```
bash $ <ab.json jtc -t 10
{
          "Directory": [
                    {
                              "address": {
                                        "city": "New York",
                                        "postal code": 10012,
                                        "state": "NY",
                                        "street address": "599 Lafayette St"
                              },
                              "age": 25,
                              "children": [
                                        "Olivia"
                              ],
...
```
Majority of the examples and explanations in this document are based on the above simplified version of the above address book JSON model.

### Compact printing
option `-r` will instruct to display JSON in a compact (one-line) format:
```
bash $ <ab.json jtc -r
{ "Directory": [ { "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" }, "age": 25, "children": [ "Olivia" ], "name": "John", "phone": [ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ], "spouse": "Martha" }, { "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null }, { "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" } ] }
bash $
```

### Printing JSON size
JSON size is the total number of the JSON elements found within JSON, it could be printed using `-z`, the size appears after JSON
is printed:
```
bash $ <ab.json jtc -r -z
{ "Directory": [ { "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" }, "age": 25, "children": [ "Olivia" ], "name": "John", "phone": [ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ], "spouse": "Martha" }, { "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null }, { "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" } ] }
size: 56
bash $
```
if size only required, then use `-zz` option:
```
bash $ <ab.json jtc -zz
56
bash $ 
```

### Validating JSON
When JSON is read (from a file, or from `stdin`), it get parsed and validated. If an invalid JSON is detected, the short exception
message will be displayed, e.g,:
```
bash $ <ab.json jtc
jtc json exception: unexpected_end_of_line
bash $
```
though the message let us know that there's a problem with the input JSON, it not very informative with regard whereabouts the
the problem. An easy way to see the spot where the problem and its locus is via debug (`-d`):
```
bash $ <ab.json jtc -d
.read_inputs(), reading json from <stdin>
.parsejson(), exception locus: ... Ave",|          "state": "CO,|          "postal code": 80206...
.location_(), exception spot: --------------------------------->| (offset: 1150)
jtc json exception: unexpected_end_of_line
bash $ 
```
the vertical pipe symbol `|` in the debug showing JSON replaces new lines, thus it becomes easy to spot the problem. 
The offset (`1150`) is given in _unicode UTF-8_ characters from the beginning of the input/file. In that particular failure 
instance, `jtc` found end of the line, while _JSON string_ `"Co,` is still open (JSON standard does not permit multi-line strings).
To fix that, the missing quotation mark to be added

### Forcing strict solidus parsing
JSON specification allows escaping solidus (`/`) optionally. By default, `jtc` is relaxed w.r.t. solidus notation - it admits
both unescaped and escaped appearances:
```
bash $ echo '{ "escaped": "\/", "unescaped": "/" }' | jtc 
{
   "escaped": "\/",
   "unescaped": "/"
}
bash $
```
If there's a need for a strict solidus parsing, option `-q` facilitates the need. It also will throw an exception upon facing
a non-escaped notation:
```
bash $ echo '{ "escaped": "\/", "unescaped": "/" }' | jtc -q -d
.read_json(), start parsing json from <stdin>
.read_json(), exception locus: { "escaped": "\/", "unescaped": "/" }|
.location_(), exception spot: --------------------------------->| (offset: 33)
jtc json exception: unquoted_character
bash $ 
```

### Unquoting JSON strings
If a JSON itself (or a result of JSON walking) is a single JSON string, then sometimes there's a need to unquote it
(especially handy it comes if the string itself is an embedded JSON). `-qq` allows unquoting it, here are a few examples:
```
bash $ echo '"{ \"JSON\": \"example of an embedded JSON\" }"' | jtc 
"{ \"JSON\": \"example of an embedded JSON\" }"
bash $
bash $ echo '"{ \"JSON\": \"example of an embedded JSON\" }"' | jtc -qq
{ "JSON": "example of an embedded JSON" }
bash $
bash $ echo '"{ \"JSON\": \"example of an embedded JSON\" }"' | jtc -qq | jtc
{
   "JSON": "example of an embedded JSON"
}
bash $
bash $ echo '"3.14"' | jtc
"3.14"
bash $
bash $ echo '"3.14"' | jtc -qq
3.14
bash $
```
NOTE: _the option notation `-qq` will not engulf a single notation `-q`, if both behaviors are required then both variants have
to be specified (e.g. `jtc -q -qq`, or `jtc -qqq`)_

### Stringifying JSON
An opposite request is to string-quote a JSON itself (e.g. if you like to embed JSON as a string into another JSON). This is
achieved with option notation `-rr`:
```
bash $ echo '[ "JSON", "example" ]' | jtc 
[
   "JSON",
   "example"
]
bash $
bash $ echo '[ "JSON", "example" ]' | jtc -rr
"[ \"JSON\", \"example\" ]"
bash $
```


## Walking JSON
Whenever there's a need to print only some or multiple JSON elements, walk-paths (`-w`) tell how to do it. A walk path
(an argument of `-w` option) is made of an arbitrary number of lexemes. Though there are only 2 types of the lexemes:
- offset lexeme (`[..]`)
- search lexeme (`<..>`)

### Walking with subscripts (offset lexemes)
offsets are always enclosed into square brackets `[`,`]`, selecting of JSON elements always happens from a root.
Both arrays and objects can be subscripted using numerical offset, though it's best to utilize literal offsets to subscript objects.
E.g. let's select `address` of the 2nd (all the indices in the walk-path are zero-based) record in the above JSON:
```
bash $ <ab.json jtc -w'[Directory][1][address]'
{
   "city": "Seattle",
   "postal code": 98104,
   "state": "WA",
   "street address": "5423 Madison St"
}
bash $
```
or, equally could be done like this, but the former syntax is preferable (for your own good - when giving indices you need 
to _guess_ the index of a labeled entry, which might be prone to mistakes):
```
bash $ <ab.json jtc -w'[0][1][0]'
{
   "city": "Seattle",
   "postal code": 98104,
   "state": "WA",
   "street address": "5423 Madison St"
}
bash $
```

#### Selecting multiple subscripted JSON elements
if a numerical subscript is prepended with `+`, then all following subscripted elements will be selected as well, e.g.,
a following example prints all the names out of the address book, starting from the 2nd record:
```
bash $ <ab.json jtc -w'[Directory][+1][name]'
"Ivan"
"Jane"
bash $
```
Any numerical offset could be subscripted that way, and any number of such lexemes could appear in the walk-path, e.g.:
```
bash $ <ab.json jtc -w'[Directory][+0][phone][+1][number]'
"113-123-2368"
"223-283-0372"
"333-638-0238"
bash $
```
Here, all records (`[+0]`) from the `Directory` were selected and then in every record all `phone` sub-records selected starting from
the 2nd entry (`[+1]`)

The same way object elements could be subscripted, here's an example where all address entries staring from the 2nd one are printed,
each one stating from the 3rd entry: 
```
bash $ <ab.json jtc -w'[Directory][+1][address][+2]'
"WA"
"5423 Madison St"
"CO"
"6213 E Colfax Ave"
bash $
```

Another way to select multiple subscripts is to use a range notation `[N:N]`. In that notation `N` could be either positive or negative, 
or entirely missed. First position designates beginning of the selection, the last position designates the end of the range exclusively
(i.e. not including the indexed element itself)
- positive `N` subscripts `N`th element from the beginning of the collection (whether it's array or an object)
- negative `N` subscripts the `N`th element from the end of the collection.
- empty (missed `N`) tells to address either from the the beginning of the collection (in the first position),
or from the end (last position) 

Thus, multiple notations with the same semantics are possible, e.g.:
- `[:]`, `[0:]` will select all the element in the collection and is equivalent of `[+0]` notation
- `[0:1]`, `[:1]` will select only the first element and is the same as `[0]`
- `[:-1]` will select all the elements except the last one
- `[-2:]` will select last 2 elements in the collection 

let's print all phone numbers for the last 2 records in the `Directory`:
```
bash $ <ab.json jtc -w'[Directory][-2:][phone][:][number]'
"273-923-6483"
"223-283-0372"
"358-303-0373"
"333-638-0238"
bash $
```

### Searching JSON
Walk-path lexemes enclosed into `<`,`>` braces instruct to perform a _recursive_ search of the value under a selected JSON node. 
I.e., if the search lexeme appears as the first one in the walk-path, then the search will be done from the root, otherwise
from the node in JSON where a prior lexeme has finished.

By default, if no modifying suffixes given, a search lexeme will search _JSON string_ values only (i.e. it won't match 
_JSON numerical_ or _JSON boolean_ or _JSON null_ values). E.g., following search find a match:
```
bash $ <ab.json jtc -w'<New York>'
"New York"
bash $
```
while this one does not (the string value `New York` is found only in the first `Directory` record): 
```
bash $ <ab.json jtc -w'[Directory][1:]<New York>'
bash $
```

#### Searching JSON with RE
Search lexemes may have various suffixes: an optional single character following the closing bracket.
These suffixes alter the meaning of the search, e.g. suffix `R` instruct to perform a regex search among string values
(btw, RE in `jtc` are PCRE):
```
bash $ <ab.json jtc -w'<^N>R'
"New York"
bash $
```

#### Search suffixes
there are following suffixes to control search behavior: 
  * `r`: default (could be omitted), fully matches _JSON string_ value
  * `R`: the lexeme is a search RE, only _JSON string_ values searched
  * `d`: fully matches _JSON number_
  * `D`: the lexeme is an RE, only _JSON numerical_ values searched
  * `b`: matches _JSON boolean_ value, must be spelled as `<true>b`, `<false>b`, `<>b`, or `<any>b` - the last two are the same
  * `n`: matches _JSON null_ value, the lexeme value must be empty:`<>n`
  * `l`: fully matches _JSON label_
  * `L`: the lexeme is a search RE, only _JSON labels_ searched
  * `a`: matches any JSON atomic value, i.e. strings, numerical, boolean, null, the lexeme value must be empty
  * `o`: matches any JSON object - `{..}`, the lexeme value must be empty
  * `i`: matches any JSON array (iterable/indexable) `[..]`, , the lexeme value must be empty
  * `c`: matches either arrays or objects; the content within the encasement must be empty
  * `e`: matches end-nodes only: atomic values, `[]`, `{}`, the lexeme value must be empty
  * `w`: matches any JSON value (wide range match): atomic values, objects, arrays; the lexeme value must be empty
  * `j`: matches specified JSON value, the lexeme must be a valid JSON, e.g.: `<[]>j` - finds an empty JSON array
  * `s`: matches a JSON value previously stored in the namespace by directives: `<..>k`, `<..>v`
  * `t`: matches a tag (label/index) previously stored in the namespace by directives `<..>k`, `<..>v`  
  * `q`: matches only unique JSON values; stores the matched elemetns in the namespace 
  * `Q`: matches only repeatitive (duplicate) JSON values; stores the matched elemetns in the namespace

\- as one can see, capitalization of either of suffixes `r`, `l`, `d` promotes the respective search to the RE search


#### Directives and Namespaces
there are a few of lexemes, that look like search, though they do not perform any matching, rather they apply certain actions
with the currently walked JSON elements:
  * `v`: saves the currently walked JSON value into a namespace under the name specified by the lexeme
  * `k`: instructs to reinterpret the key (label/index) of the currently walked JSON and treat it as a value (thus a label/index
  can be updated/extracted programmatically), if the lexeme's value is non-empty then it also saves a found key (label/index) into
  the corresponding namespace
  * `z`: erases namespace pointed by lexeme value; if lexeme is empty, erase entire namespace
  * `f`: fail-stop: if walking lexemes past the fail-stop directive fails, instead of progressing to the
        next iteration, a lexeme immediately preceeding the fail-stop will be matched

Thus a _namespace_ is a container within `jtc`, which allows storing JSON elements programmatically while walking JSON.
Stored in namespaces values could be reused later in the same or different walk-paths and 
[interpolated](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation) in 
[templates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates) and arguments
for a shell evaluation.

##### dynamic lookup using directives `-v` and `-k`
Say, we have a following JSON:
```
bash $ echo '{ "item": "bread", "list":{ "milk": 0.90, "bread": 1.20 } }' | jtc 
{
   "item": "bread",
   "list": {
      "bread": 1.20,
      "milk": 0.90
   }
}
bash $ 
```
the ask here is to retrieve a value from `list` defined by `item` - that would require a cross-reference look up.
Using namespaces it becomes a trivial task:
```
bash $ echo '{ "item": "bread", "list":[ { "milk": 0.90 }, { "bread": 1.20 } ] }' | jtc -w'[item]<lbl>v[^0][list]<lbl>t'
1.20
bash $ 
```
- `[item]<lbl>v` - will retrieve a value in `item` and store it in the namespace `lbl`
- `[^0][list]<lbl>t` - will reset the point of departure back to the root, then will select `list` and search the label
matching the value stored in the namespace `lbl` (which is `bread`)

Similar way labels/indices could be accessed and stored in the namespaces - using directive `k`. The directive lets reinterpreting
label/index of the currently walked JSON element and treat it as a _JSON string_ / _JSON number_ value respectively.
Say, we want to list all labels in the `address` record:
```
bash $ <ab.json jtc -w'<address>l[:]<>k'
"city"
"postal code"
"state"
"street address"
bash $ 
```
When the directive lexeme `<>k` is used w/o a value (like shown) then no saving in the namespaces occurs.

The directive `<..>v` allows user setting up custom JSON values (in lieu of currently walked JSON's) - if the directive's
value is given in the format:
- `<name:JSON_value>v`

then upon walking such syntax a user's `JSON_value` will be preserved in the namespace `name`


##### Fail-stop and Forward directives
All the lexemes in the _walk-path_ are bound by logical `AND` - only if all succeed then the path is successfully walked (and printed
or regarded for a respective operation). The _fail-stop_ directive make possible to introduce `OR` logic into the _walk-path_. 
Let's break it down:

Say, we want to list all `mobile` phone records (let's do it along with names of phone holders):
```
bash $ <ab.json jtc -x'[0][:]' -y'[name]' -y'[phone]<mobile>[-1]' -r
"John"
{ "number": "112-555-1234", "type": "mobile" }
"Ivan"
{ "number": "223-283-0372", "type": "mobile" }
"Jane"
bash $ 
```
As it appears, `Jane` has no mobile phone, but then our requirement is enhanced: for those who do not have a `mobile`, let's list
the first available phone from the records (there a `<>f` directive comes to a rescue):
```
bash $ <ab.json jtc -x'[0][:]' -y'[name]' -y'[phone][0]<>f[-1]<mobile>[-1]' -r
"John"
{ "number": "112-555-1234", "type": "mobile" }
"Ivan"
{ "number": "223-283-0372", "type": "mobile" }
"Jane"
{ "number": "358-303-0373", "type": "office" }
bash $ 
```
as the path is walked, as soon `<>f` directive is faced, it _memorizes_ the currently walked path and will _recall_ it shall
further walking fail, there
- we resolve the first entry in the `phone` records and memorize it's path location (`[phone][0]<>f`)
- then step back up and look for a `mobile` type of the record (`[-1]<mobile>`), then:
     * if it's found, we step back up (`[-1]`) to finish walking and disply the whole record
     * if not found (i.e. walking indeed fails), a fail-stop is engaged and preserved location is recalled and printed

A _walk-path_ may contain multiple _fail-stops_, only the respective fail-stop will be engaged (more specific one and closest 
one to the failing point)

The _fail_stop_ directive, (as well as `<..>v`) may carry a value (namespace), which will be populated with the currently walked
JSON element (for later interpolation), and, as well as `<..>v`, the _fail_stop_ is also allowed setting up custom JSON values 
(when used in the format: `<namespace:JSON_value>f`)

###### Here's another example sporting _fail-stops_ using namespaces and interpolation:
Say we want to list from the address book  all the record holders and indicate whether they have children or not in this format 
`{ "Name": true/false }`

Thus, we need to build a single path, which will find the `name`, then inspect sibling `children` records and transform it into
`true` if there at least 1 child, or `false` otherwise:

We can do it in steps:
1. let's get to the `name`s first and memorize those:
```
bash $ <ab.json jtc -x'[0][:][name]<person>v' 
"John"
"Ivan"
"Jane"
bash $ 
```
2. Now let's inspect sibling `children` records:
```
bash $ <ab.json jtc -x'[0][:][name]<person>v [-1][children]' 
[
   "Olivia"
]
[]
[
   "Robert",
   "Lila"
]
bash $ 
```
3. so far so good, but we need to engage _fail_stop_ to facilitate the requirement to classify those records as `true` / `false`:
```
bash $ <ab.json jtc -x'[0][:][name]<person>v [-1][children]<cn:false>f[0]<cn:true>v' 
"Olivia"
[]
"Robert"
bash $ 
```
- there namespace `cn` will be populated first with `false` and will stay put shall further walking fails.
- otherwise (i.e. upon a successful walk - addressing a first child `[0]`) the namespace `cn` will be overwritten with `true` value
4. finally, we need to interpolate preserved namespaces for our final / required output:
```
bash $ <ab.json jtc -x'[0][:][name]<person>v [-1][children]<cn:false>f[0]<cn:true>v' -T'{"{person}":{cn}}' -r
{ "John": true }
{ "Ivan": false }
{ "Jane": true }
bash $ 
```

Now, let's consider a following example. Say, we have a following JSON:
```
bash $ JSN=$(echo '[ { "ip": "1.1.1.1", "name": "server" }, { "ip": "1.1.1.100" }, { "ip": "1.1.1.2", "name": "printer" } ]')
bash $ echo "$JSN" | jtc 
[
   {
      "ip": "1.1.1.1",
      "name": "server"
   },
   {
      "ip": "1.1.1.100"
   },
   {
      "ip": "1.1.1.2",
      "name": "printer"
   }
]
bash $ 
```
How do we list only those records which don't have `name` and skip those which do? Well, one obvious solution then would be
to walk all those entries, which do have `name` labels and purge them:
```
bash $ echo "$JSN" | jtc -w'<name>l:[-1]' -p
[
   {
      "ip": "1.1.1.100"
   }
]
bash $ 
```
But what if we want to walk them instead of purging (e.g., for reason of template-interpolating the entries at the same time)?
The prior solution would require piping it to the next `jtc` cli, however, it's possible to achieve the same using this simple query:
```
bash $ echo "$JSN" | jtc -w'[:]<>f[name]<>F'
{
   "ip": "1.1.1.100"
}
bash $ 
```
Without `<>F` directive, the walk would look like:
```
bash $ echo "$JSN" | jtc -w'[:]<>f[name]'
"server"
{
   "ip": "1.1.1.100"
}
"printer"
bash $ 
```
Thus, `<>F` skips those (successfully) matched entries, leaving only one which fails - that's the one which we need in this query 
(the record(s) which does not have `name` in it)


#### RE generated namespaces
RE search lexemes (`R`, `L`, `D`) also auto-populate the namespaces with following names:
- `$0` is auto-generated for an entire RE match,
- `$1` for a first subgroup,
- `$2` for a second subgroup, and so on
```
bash $ <ab.json jtc -w'<^J(.*)>R:'
"John"
"Jane"
bash $ <ab.json jtc -w'<^J(.*)>R:' -T'"j{$1}"' 
"john"
"jane"
bash $ 
```


#### Path namespaces
There are also couple resrved namespaces to acqure a current JSON path:
- `$PATH`: that namespace contains a JSON array describing path to the currently selected element
- `$path`: this is a strigified represenation of the path
Here are both demonstrated:
```
bash $ <ab.json jtc -w'<Jane>' -T'{{$PATH}}' -r
[ "Directory", 2, "name" ]
bash $ <ab.json jtc -w'<NY>' -T'{{$path}}'
"Directory_0_address_state"
bash $ 
```
to play safe with the templates, always surrond them with single quotes (to dodge shell interpolation)





#### Search quantifiers
Optionally a quantifier may follow the search lexeme (if a lexeme has a suffix, then the quantifier must come after the suffix). 
Quantifiers in search lexemes allow selecting match instance (i.e. select first match, second one, etc, or a range of matches)
Quantifiers exist in the following formats:
- `n`, - a positive number - tells which instance of a match to pick. By default, a quantifier `0` is applied
(i.e. first match is selected)
- `+n` - selects all match instances starting from `n`th (zero based)
- `N:N` - range select: the notation rules for this quantifier the same as for subscript range (`[N:N]`), with just one understandable
caveat: `N` here cannot go negative (there's no way of knowing upfront how many matches would be produced, so it's impossible to select
a range/slice based off the last match), the rest of the notation rules apply

To illustrate the quantifiers (with suffixes), let's dump all the JSON arrays in the `Directory`, except the top one:
```
bash $ <ab.json jtc -w'<>i1:' -r
[ "Olivia" ]
[ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ]
[]
[ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ]
[ "Robert", "Lila" ]
[ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ]
bash $
```
\- the trailing `1:` in the walk path is the range quantifier, which selects (prints) all the matches (we are matching
_JSON arrays_ here - `i`) starting from second one (all quantifiers and indices are zero-based)


#### Search quantifiers with relative offset semantic 
There are two search lexemes where matching non-first instance does not make sense, namely: `>..<l` and `>..<t`.
Those are non-recursive searches will uniquely match label or index. Indeed, in a plain _JSON array_ or an _object_ it's possible
to address only one single label or index, there could not be any other, e.g. this JSON:
```
bash $ echo '{ "a": 1, "b":2, "c":3, "d":4, "e":6 }' | jtc -r
{ "a": 1, "b": 2, "c": 3, "d": 4, "e": 6 }
bash $ 
```
there could be only one label `"b"`, thus normally trying to match a second, third, etc instance of the label `"b"` would not
make much sense: `echo '{ "a": 1, "b":2, "c":3, "d":4, "e":6 }' | jtc -w'>b<l2'`

Thus the semantic of quantifiers only in those searches was changed (to extend use cases) - there, quantifiers provide a
relative offset meaning from a found label/index. So, for the notation like above: `'>b<l2'`, the label `"b"` will be matched and
once matched/found then its second (successive) neighbor value will be selected:
```
bash $ echo '{ "a": 1, "b":2, "c":3, "d":4, "e":6 }' | jtc -w'>b<l2' -l
"d": 4
bash $ 
```
Because of change in semantic, those are only search quantifiers (they also have to be non-recursive) which allow negative
values. Positive quantifiers let selecting next (successive) neighbors, while negative quantifiers let selective preceding
neighbors:
```
bash $ echo '{ "a": 1, "b":2, "c":3, "d":4, "e":6 }' | jtc -w'>b<l-1' -l
"a": 1
bash $ 
````


#### Scoped search 
Search lexemes perform a _recursive_ search across the entire JSON tree off the point where it's invoked (i.e. the JSON node
selected by walking all the prior lexemes). However, sometime's there's a need to limit searching scope only to the specific label.
Here is the dump of all the _JSON strings_, where symbol `5` is sighted:
```
bash $ <ab.json jtc -w'<5>R:'
"599 Lafayette St"
"112-555-1234"
"5423 Madison St"
"358-303-0373"
bash $
```
Some of the values are `street address`, some are the phone `number`. Say, we want to dump only the phone records. Knowing the label
of the phone numbers (`"number"`), it's achievable via this notation:
```
bash $ <ab.json jtc -w'[number]:<5>R:'
"112-555-1234"
"358-303-0373"
bash $
```
I.e., once the label lexeme is attached to the search lexeme over `:`, it signifies a scoped search.

#### Non-recursive search
Sometimes there's a need to apply a non-recursive search onto collectable JSON nodes (arrays, objects) - i.e. find a value within
immediate children of the element and do not descend recursively. The notation facilitating such search is the same one, but
angular brackets to be put inside-out, i.e.: `>..<`. To illustrate that: say, we want to find all string values in the 1st `Directory`
record containing the letter `o`. If we do this using a recursive search, then all following entries will be found:
```
bash $ <ab.json jtc -w'[Directory] [0] <o>R:'
"New York"
"John"
"mobile"
"mobile"
bash $
```
To facilitate our ask - find all such entries within the immediate values of the 1st record, apply a non-recursive search notation:
```
bash $ <ab.json jtc -w'[Directory] [0] >o<R:'
"John"
bash $
```


### Addressing parents
One of the charming features of `jtc` is the ability to address parents (any ancestors up till the root) of the found JSON nodes.
Typically addressing parents would be needed after search lexemes (but really can occur anywhere in the walk-path). Parents can
be addressed using notation `[-n]`. This feature allows building queries that answer complex queries.

Let's dump all the names from the `Directory` whose records have a `home` phone entry:
```
bash $ <ab.json jtc -w'[type]:<home>: [-3] [name]'
"Ivan"
"Jane"
bash $
```
The magic which happens here (let's break down the walk-path into the lexemes) is revealed below:
  1. `[type]:<home>:` this lexeme instruct to find all (ending `:`) strings `home` scoped by label `"type"`, thus all records will
  be selected:
  ```
  bash $ <ab.json jtc -w'[type]:<home>:' -r
  "home"
  "home"
   bash $
  ```
  2. starting off those found JSON elements a 3rd ancestor will be selected. Let's see a parent selection in a slow-mo, one by one:
  ```
  bash $ <ab.json jtc -w'[type]:<home>: [-1]' -r
  { "number": "273-923-6483", "type": "home" }
  { "number": "333-638-0238", "type": "home" }
  bash $
  bash $ <ab.json jtc -w'[type]:<home>: [-2]' -r
  [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ]
  [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ]
  bash $
  bash $ <ab.json jtc -w'[type]:<home>: [-3]' -r
  { "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null   }
  { "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" }
  bash $
  ```
  3. now we can select (subscript) `[name]` of out those JSON nodes


Another example: who is the parent of a child `Lila`?
```
bash $ <ab.json jtc -w'<children>l: <Lila> [-2] [name]'
"Jane"
bash $
```
Explanation:
1. `<children>l:` - finds all records with the label `children`
2. `<Lila>` - in found records find string value `Lila`
3. `[-2] [name]` - go 2 levels (parents) up from found entry `"Lila"` and then subscript/offset by label `name`

Even more complex query: who of the parents, those who have children, have mobile numbers?
```
bash $ <ab.json jtc -w'<children>l: [0] [-2] [type]:<mobile> [-3] [name]'
"John"
bash $
```
\-  that is correct answer (Ivan has no children and Jane has no mobile phone)

The walk-path break down:
1. `<children>l:` - find all records by label `"children"`
2. `[0]` - try addressing first element in the found records (that'll ensure that `children` is non-empty)
3. `[-2]` - go 2 parents up for those records which survived the prior step - that'll bring us to the `Directory` record level 
4. `[type]:<mobile>` - find recursively `mobile` string scoped by `type` (for those records which have children)
5. `[-3]`-  go 3 levels (parents) up (for those records which have `children` and have `mobile` types of phone records) - it'll bring
us again up to the `Directory` record level
6. `[name]` - finally select the name

There's another way to address parents - compare, the following walk-path achieves exactly the same ask:
```
bash $ <ab.json jtc -w'<children>l: [0] [^2] [type]:<mobile> [^2] [name]' -r
"John"
bash $
```
Note `[^2]` - this notation, like `[-n]` also selects a certain parent, however, while `[-n]` select the parent from the leaf
(i.e. from the currently selected node) `[^n]` notation selects the parent from the root.

When `jtc` walks lexemes, internally it maintains a path of the walked steps (it's visible via debugs `-dddd`). E.g., when the first
lexeme's match found (for `<children>l:`), the internal walked steps path would look like: `root [Directory] [0] [children]`,
then when the next lexeme is successfully applied, the internal path becomes: `root [Directory] [0] [children] [0]`
The meaning of `[-n]` and `[^n]` notation then is easy to see on this diagram:
```
                                                              etc.
                                                              [^5]
to address a parent from root: [^0]    [^1]    [^2]    [^3]   [^4]
                                |       |       |       |      |
                                v       v       v       v      v
       internally built path: root [Directory] [0] [children] [0]
                                ^       ^       ^       ^      ^
                                |       |       |       |      |
to address a parent from leaf: [-4]    [-3]    [-2]    [-1]   [-0]
                               [-5]
                               etc.                         
```


### Walking multiple paths
`jtc` allows a virtually unlimited number of walk-paths (`-w`), it would be limited only by the max size of the accepted string in
your shell. Though there are a few tweaks in `jtc` which let harnessing the order of displaying resulted walks. By default `jtc`
will be displaying resulted successful walks in an _interleaved_ manner, but first, let's take a look at

#### Sequential walk processing
option `-n` ensures that all given walk-paths (`-w`) will be processed (and printed) sequentially in the order they given:
```
bash $ <ab.json jtc -w '<name>l:' -w'<number>l:' -n
"John"
"Ivan"
"Jane"
"112-555-1234"
"113-123-2368"
"273-923-6483"
"223-283-0372"
"358-303-0373"
"333-638-0238"
bash $
bash $ <ab.json jtc -w'<number>l:' -w'<name>l:' -n
"112-555-1234"
"113-123-2368"
"273-923-6483"
"223-283-0372"
"358-303-0373"
"333-638-0238"
"John"
"Ivan"
"Jane"
bash $
```

#### Displaying walks with labels
if resulted walks have labels in the input JSON (i.e. they were inside _JSON objects_), then `-l` let dumping their labels too:
```
bash $ <ab.json jtc -w '<name>l:' -w'<number>l:' -nl
"name": "John"
"name": "Ivan"
"name": "Jane"
"number": "112-555-1234"
"number": "113-123-2368"
"number": "273-923-6483"
"number": "223-283-0372"
"number": "358-303-0373"
"number": "333-638-0238"
bash $
```

#### Wrapping resulted walks to JSON
`-j` does a quite simple thing - it wraps walked entries back into a _JSON array_, however predicated by `-l` and `-n` options
the result will vary:
- `-j` without `-l` will just arrange walked entries into a JSON array:
  ```
  bash $ <ab.json jtc -w '<name>l:' -w'<number>l:' -nj 
  [
     "John",
     "Ivan",
     "Jane",
     "112-555-1234",
     "113-123-2368",
     "273-923-6483",
     "223-283-0372",
     "358-303-0373",
     "333-638-0238"
  ]
  bash $
  ```
- once `-j` and `-l` given together, then entries which have labels (i.e. come from the _JSON objects_) will be wrapped into objects:
  ```
  bash $ <ab.json jtc -w '<name>l:' -w'<number>l:' -n -j -l
  [
     {
        "name": "John"
     },
     {
        "name": "Ivan"
     },
     {
        "name": "Jane"
     },
     {
        "number": "112-555-1234"
     },
     {
        "number": "113-123-2368"
     },
     {
        "number": "273-923-6483"
     },
     {
        "number": "223-283-0372"
     },
     {
        "number": "358-303-0373"
     },
     {
        "number": "333-638-0238"
     }
  ]
  bash $
  ```
Though even that behavior is influenced by the `-n` option. The above output looks dull and hardly will have many use-cases, a lot more
often it's required to group relevant walks together and then place them into respective JSON structures. For that, let's review

#### Interleaved walk processing
Interleaved walk processing (and outputting) occurs by default, though there's a certain way to control it. Let's take a look at the
above outputs dropping the option `-n` (i.e. print _interleaved_):
```
bash $ <ab.json jtc -w '<name>l:' -w'<number>l:'
"John"
"112-555-1234"
"Ivan"
"113-123-2368"
"Jane"
"273-923-6483"
"223-283-0372"
"358-303-0373"
"333-638-0238"
bash $
```
Those look interleaved though it does not appear that they relate to each other properly: e.g.: number `"113-123-2368"`
belong to `"John"` and preferably should be displayed before `"Ivan"` and so does apply to others. `jtc` is capable to process/print
relevant entries, though it needs a little hint from the walk-paths: the latter supposed to express the relevance between themselves.

Right now both paths (`<name>l:` and `<number>l:`) do not have common lexemes, thus it's unclear how to relate resulting walks
(hence they just interleaved one by one). Though if we provide walk-paths relating each of those searches to their own record,
then magic happens:
```
bash $ <ab.json jtc -w '[Directory][:] <name>l:' -w'[Directory][:] <number>l:'
"John"
"112-555-1234"
"113-123-2368"
"Ivan"
"273-923-6483"
"223-283-0372"
"Jane"
"358-303-0373"
"333-638-0238"
bash $
```
And now, applying options `-j` with `-l` gives a lot better result:
```
bash $ <ab.json jtc -w '[Directory][:] <name>l:' -w'[Directory][:] <number>l:' -jl
[
   {
      "name": "John",
      "number": [
         "112-555-1234",
         "113-123-2368"
      ]
   },
   {
      "name": "Ivan",
      "number": [
         "273-923-6483",
         "223-283-0372"
      ]
   },
   {
      "name": "Jane",
      "number": [
         "358-303-0373",
         "333-638-0238"
      ]
   }
]
bash $
```

`-jj` let wrapping walked JSON elements into a _JSON object_. All the elements in JSON object must have labels, thus any walked elements
which do not have labels (i.e. elements in _JSON array_ and root) will be ignored.

E.g., let's dump all values from `Jane`'s record and wrap them all into an object:
```
bash $ <ab.json jtc -w'<Jane>[-1]<>a:' -jj 
{
   "age": 25,
   "city": "Denver",
   "name": "Jane",
   "number": [
      "358-303-0373",
      "333-638-0238"
   ],
   "postal code": 80206,
   "spouse": "Chuck",
   "state": "CO",
   "street address": "6213 E Colfax Ave",
   "type": [
      "office",
      "home"
   ]
}
bash $ 
```
As one can see, even though `Jane` has 2 lovely children (`Robert` and `Lila`), they were not listed on the resulting output,
because they are enlisted as array and therefore have no attached labels (and hence were ignored).

Another point to note: the values with the clashing labels will be reassembled into an array

#### Succinct walk-path syntax
If you look at the prior example, you may notice that a common part of both walk-paths (`[Directory][:]`) we had to give twice. There's
a way to express it in more succinct syntax: options `-x` and `-y` allows rearrange walk-paths so that `-x` takes an initial common part
of the walk-path, where as `-y` would take care of the individuals trailing pars. Thus the same example cold have been written like:
```
bash $ <ab.json jtc -x '[Directory][:]' -y'<name>l:' -y'<number>l:'
"John"
"112-555-1234"
"113-123-2368"
"Ivan"
"273-923-6483"
"223-283-0372"
"Jane"
"358-303-0373"
"333-638-0238"
bash $
```
\- each occurrence of `-x` will be recombined with all subsequent `-y` (until next `-x` is faced). Options `-x`, `-y` is merely
a syntactical sugar and do not apply any walk-path parsing or validation, instead they just result in the respective `-w` options 
creations (internally), then the latter get processed. Thus, it's even possible to write it with what it seems a broken syntax at first:
```
bash $ <ab.json jtc -x '[Directory] [:' -y'] <name>l:' -y'] <number>l:'
...
```
However, if a reinstatement of the options results in a valid walk-path - that's all what matters.

It's possible to combine both syntaxes (i.e. `-w` with `-x` and `-y`), however, given that the processing of `-x` and `-y`
internally reinstates respective options `-w`, the former will be appended after any of given `-w` options (which will affect the 
order of processing/outputting) even though the order of their appearance is different:
```
bash $ <ab.json jtc -x '[Directory] [:]' -y'<name>l:' -y'<number>l:' -w '<children>l:' -rnl
"children": [ "Olivia" ]
"children": []
"children": [ "Robert", "Lila" ]
"name": "John"
"name": "Ivan"
"name": "Jane"
"number": "112-555-1234"
"number": "113-123-2368"
"number": "273-923-6483"
"number": "223-283-0372"
"number": "358-303-0373"
"number": "333-638-0238"
bash $
```


## Modifying JSON
there are few options which let modifying input JSON:
- `-i` - insert (copy, copy-merge, move) new elements to JSON
- `-u` - update (rewrite, rewrite-merge, move) elements to JSON
- `-s` - swap around pair(s) of JSON elements
- `-p` - purge (remove) elements from JSON

Typically, those options are mutually exclusive and if sighted together, only one type of operations will be executed (the above list
is given in the priority order of operation selection). However, there is a certain combination of options `-i`,`-u` and `-p`, which
facilitates _move_ semantic, those cases reviewed in the respective chapters.

Each of options requires one or multiple `-w` to be given to operate on. Options `-i` and `-u` require an argument, which comes in
different flavors, one of them is the `walk-path` per-se.

`jtc` will execute any of operations only once, if multiple operations required, then `jtc` needs to be invoked in a successive order
(e.g. daisy-chained through the pipe `|`)


### In-place JSON modification 
By default `jtc` will expect input from `stdin`. If the standalone argument `json_file` is given, then `jtc` will read
input from the file, see below:
```
bash $ cat file.json 
[
   "JSON",
   "in",
   "file"
]
bash $ 
bash $ echo '[ "input", "JSON" ]' | jtc
[
   "input",
   "JSON"
]
bash $ 
bash $ echo '[ "input", "JSON" ]' | jtc file.json
[
   "JSON",
   "in",
   "file"
]
bash $
```
If option `-f` is given (together with `json_file` argument) then `jtc` will apply (redirect) its output of the operations
into the file:
```
bash $ echo '[ "input", "JSON" ]' | jtc -f file.json
bash $ cat file.json 
[
   "JSON",
   "in",
   "file"
]
bash $ 
```
In the above example, JSON is read from `file.json` and redirected back into the file.

The bare hyphen (`-`) overrides a file input and ensured that the input is read from the `stdin`:
```
bash $ echo '[ "input", "JSON" ]' | jtc -f - file.json
bash $ cat file.json
[
   "input",
   "JSON"
]
bash $ 
```

### Purging JSON
`-p` removes from JSON all walked elements (given by one or multiple `-w`). E.g.: let's remove from address book (for the sake
of example) all the `home` and `office` phones records (effectively leaving only `mobile` types):
```
bash $ <ab.json jtc -w'[type]:<home>: [-1]' -w'[type]:<office>: [-1]' -p | jtc -w'<phone>l:' -l
"phone": [
   {
      "number": "112-555-1234",
      "type": "mobile"
   },
   {
      "number": "113-123-2368",
      "type": "mobile"
   }
]
"phone": [
   {
      "number": "223-283-0372",
      "type": "mobile"
   }
]
"phone": []
bash $
```
Of course there's a more succinct syntax:
````
bash $ <ab.json jtc -x'[type]:' -y'<home>:[-1]' -y'<office>:[-1]' -p
````
or, using even a single walk-path:
````
<ab.json jtc -w'[type]:<(?:home)|(?:office)>R: [-1]' -p
````


Another use-case exist: remove all the JSON elements _except_ walked ones (while preserving original JSON structure) - that's
the feat for multiple option notation: `-pp`. Let's drop all entries (in all records) but the `name` and `spouse`:
````
bash $ <ab.json jtc -w'<(?:name)|(?:spouse)>L:' -pp
{
   "Directory": [
      {
         "name": "John",
         "spouse": "Martha"
      },
      {
         "name": "Ivan",
         "spouse": null
      },
      {
         "name": "Jane",
         "spouse": "Chuck"
      }
   ]
}
bash $
````
Here, `(?:name)|(?:spouse)` is an RE (indicated by capitalized label search suffix `L`) matching labels containing either `"name"` or
`"spouse"`


### Swapping JSON elements
`-s` requires exactly two walk-paths (`-w`) to be given. Both walk-path will be walked concurrently (ensure they are consistent)
and resulted JSON elements will be swapped around.

E.g., here's an example of swapping around `name` and `spouse` for all records on the address book:
```
bash $ <ab.json jtc -w'<name>l:' -w'<spouse>l:' -s | jtc  -w'<(?:name)|(?:spouse)>L:' -l 
"name": "Martha"
"spouse": "John"
"name": null
"spouse": "Ivan"
"name": "Chuck"
"spouse": "Jane"
bash $
```
_\- for brevity, swapped elements only sorted out_


Possibly, a more frequent use-case for `-s` is when it's required to remove some extra/redundant nestedness in a JSON structure. 
For the sake of example, let's remove _array_ encapsulation from phone records, leaving only the last phone in it:
```
bash $ <ab.json jtc -w'<phone>l:' -w'<phone>l:[-1:]' -s | jtc -w'<phone>l:' -l
"phone": {
   "number": "113-123-2368",
   "type": "mobile"
}
"phone": {
   "number": "223-283-0372",
   "type": "mobile"
}
"phone": {
   "number": "333-638-0238",
   "type": "home"
}
bash $
```
_\- again, for brevity, only phone records are displayed_


### Insert operations
for insert (`-i`) operations, the destination points of insertion are given using `-w` option(s) while the argument under `-i`
itself is the source of the insertion (though multiple `-i` options could be given). The source of insertion must _always_ be
a valid JSON.

Insert operations never result in overwriting the destination JSON elements (though could be extended).
There are 3 different flavors of insertion arguments:
- `-i <file>` - JSON being inserted is read from the file
- `-i <JSON>` - the argument itself a JSON string
- `-i <walk-path>` - the argument is a walk-path in the input JSON

How `jtc` knows which argument is supplied? The disambiguation path is like this:
1. initially a `<file>` is assumed and attempted to be read, if that fails (i.e. file not found), then
2. `<JSON>` string is assumed and attempted to be parsed. If JSON parsing fails, then
3. `<walk-path>` is assumed and parsed - if that fails too, a short exception message is displayed

_An attention is required when passing a `<walk-path>` argument: some walk-paths look exactly like JSON, e.g:
`[0]` - this is both a valid JSON array (made of single numeric value `0`) and a valid walk-path (addressing a first element
in an iterable), hence such argument will be treated as JSON. To pass this as a walk-path, modify it to a range-type of
walk, e.g.: `[0:1]` - that is still a valid walk-path (selecting only a first element) but is invalid JSON._

#### Destination driven insertion
The destination insertion point(s) (`-w`) controls how insertion is done:
- if a given destination insertion point (`-w`) is a single walk and non-iterable - i.e., if it's a single location - then all given
sources are attempted to get inserted into a single destination location:
```
bash $ <ab.json jtc -w'<children>l:' -lr
"children": [ "Olivia" ]
"children": []
"children": [ "Robert", "Lila" ]
bash $
bash $ <ab.json jtc -w'[name]:<Ivan>[-1][children]' -i'"Maggie"' -i'"Bruce"' | jtc -w'<children>l:' -lr
"children": [ "Olivia" ]
"children": [ "Maggie", "Bruce" ]
"children": [ "Robert", "Lila" ]
bash $
```
- if a given destination insertion point is iterable, or multiple are given, then all sources (`-i` arguments) are inserted one
by one in a circular fashion (if source runs out of JSON elements, but destination has more room to iterate, then source
is wrapped to the beginning element):
```
bash $ <ab.json jtc -w'<children>l:' -i'"Maggie"' -i'"Bruce"' | jtc -w'<children>l:' -lr
"children": [ "Olivia", "Maggie" ]
"children": [ "Bruce" ]
"children": [ "Robert", "Lila", "Maggie" ]
bash $
```

#### Inserting objects into objects
while insertion into arrays is obvious (well, so far), insertion into object requires clarification:
- objects always merged recursively
- in case of clashing labels - destination is preserved while source is discarded

To illustrate: let's insert a JSON structure: `{ "PO box": null, "street address": null }` into the last record's `address`:
```
bash $ <ab.json jtc -w'[0][-1:][address]' -l
"address": {
   "city": "Denver",
   "postal code": 80206,
   "state": "CO",
   "street address": "6213 E Colfax Ave"
}
bash $
bash $ <ab.json jtc -w'[0][-1:][address]' -i'{ "PO box": null, "street address": null }' | jtc -w'[0][-1:][address]' -l
"address": {
   "PO box": null,
   "city": "Denver",
   "postal code": 80206,
   "state": "CO",
   "street address": "6213 E Colfax Ave"
}
bash $
```
\- as one can see, the `"PO box"` label was inserted, but the destination object's value in the `"street address"` was preserved

#### Insertion matrix without merging
Source (JSON being inserted) and destination (JSON point where insertion occurs) elements might represent different types:
_JSON array_, _JSON object_, _JSON atomic_. Thus there's a number of variants of insertions of one type of elements into others. 
All such variants are shown in the below matrix table:
```
  to \ from  |        [3,4]        |     {"a":3,"c":4}     |      "a":3,"c":4      |      3
-------------+---------------------+-----------------------+-----------------------+-------------
   [1,2]     |      [1,2,[3,4]     |  [1,2,{"a":3,"c":4}]  | [1,2,{"a":3},{"c":4}] |   [1,2,3]
{"a":1,"b":2}|    {"a":1,"b":2}    |  {"a":1,"b":2,"c":4}  |  {"a":1,"b":2,"c":4}  |{"a":1,"b":2}
    "a"      |         "a"         |          "a"          |          "a"          |     "a"
```
_\- the values in the 4th column header (namely `"a":3,"c":4`) do not look like valid JSON - those are JSON object's elements
when pointed to by the `-i <walk-path>`, i.e. they are JSON elements with labels_

as follows from the table:
- insertion cannot happen into the atomic JSON elements
- when inserting into array, whole JSON element is getting inserted (no array expansion occurs)
- labeled elements inserted as JSON objects

#### Insertion matrix with merging
One might wonder: if insertion of an array into another array happens without merging arrays, how to achieve then merged result upon
insertion? (or other similar questions may rise).

option `-m` (merge) alters the behavior of insert operation into following:
```
  to \ from  |        [3,4]        |     {"a":3,"c":4}     |      "a":3,"c":4      |      3
-------------+---------------------+-----------------------+-----------------------+-------------
   [1,2]     |      [1,2,3,4]      |       [1,2,3,4]       |       [1,2,3,4]       |   [1,2,3]
{"a":1,"b":2}|{"a":[1,3],"b":[2,4]}|{"a":[1,3],"b":2,"c":4}|{"a":[1,3],"b":2,"c":4}|{"a":1,"b":2}
    "a"      |      ["a",3,4]      |       ["a",3,4]       |       ["a",3,4]       |   ["a",3]
```
- merging option allows insertion into the atomic values, but it gets converted into an array first
- arrays are merged now
- clashing labels (for merged objects/object members) are also converted into arrays (if not yet) and merged 


### Update operations
update (`-u`) is similar to insert operation, but unlike _insert_, it overwrites the destination JSON element. Though both operations
share the same qualities:
- both are [destination driven operations](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#destination-driven-insertion)
- both merge _JSON objects_ recursively
- both support merging (`-m`) semantic
- both support _move_ (`-p`) semantic ([covered later](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-with-move))
- both support _shell evaluation_ (`-e`) of argument ([covered later](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluaton))

#### Update operations matrix
Here's the matrix table for update operations with and without merging:

- update without merging:
```
  to \ from  |        [3,4]        |     {"a":3,"c":4}     |         "a":3         |      3
-------------+---------------------+-----------------------+-----------------------+-------------
   [1,2]     |        [3,4]        |     {"a":3,"c":4}     |           3           |      3
{"a":1,"b":2}|        [3,4]        |     {"a":3,"c":4}     |           3           |      3
    "a"      |        [3,4]        |     {"a":3,"c":4}     |           3           |      3
```

- update with merging (`-m`):
```
  to \ from  |        [3,4]        |     {"a":3,"c":4}     |         "a":3         |      3
-------------+---------------------+-----------------------+-----------------------+-------------
   [1,2]     |        [3,4]        |         [3,4]         |         [3,2]         |    [3,2]
{"a":1,"b":2}|    {"a":3,"b":4}    |  {"a":3,"b":2,"c":4}  |      {"a":3,"b":2}    |{"a":3,"b":2}
    "a"      |        [3,4]        |     {"a":3,"c":4}     |         {"a":3}       |      3
```
- when updating without `-m`, the operation is straightforward - a source overwrites the destination
- when objects merge-updated, for clashing labels, a source does overwrite the destination (unlike with insertion)

#### Updating labels
A directive lexeme `<>k` allows accessing the label/index of the currently walked JSON element and even store it in the namespace.
Another function featured by the lexeme is that the label is reinterpreted as a _JSON string_ value, that allows rewriting labels
using update operation (insert into labels is not possible even semantically)

As the an exercise, let's capitalize all the labels within all `address`'es in `ab.json`:
```
bash $ <ab.json jtc -w'<address>l:[:]<>k' -eu echo '"{}"' \| tr '[:lower:]' '[:upper:]' \; | jtc -w'<address>l:' -rl
"address": { "CITY": "New York", "POSTAL CODE": 10012, "STATE": "NY", "STREET ADDRESS": "599 Lafayette St" }
"address": { "CITY": "Seattle", "POSTAL CODE": 98104, "STATE": "WA", "STREET ADDRESS": "5423 Madison St" }
"address": { "CITY": "Denver", "POSTAL CODE": 80206, "STATE": "CO", "STREET ADDRESS": "6213 E Colfax Ave" }
bash $ 
```

NOTE: _mind the caveat though - destination walk-path may become invalid (namely when altering labels of the nested elements after
the parent's label has been altered), in such case the update operation won't be applied due to invalidated destination_:
```
bash $ <ab.json jtc -x'[Directory][0][address]' -y'<>k' -y'[:]<>k'
"address"
"city"
"postal code"
"state"
"street address"
bash $ 
bash $ <ab.json jtc -x'[Directory][0][address]' -y'<>k' -y'[:]<>k' -eu echo '"{}"' \| tr '[:lower:]' '[:upper:]' \; | jtc -w'[Directory][0]'
error: destination walk became invalid, skipping update
error: destination walk became invalid, skipping update
error: destination walk became invalid, skipping update
error: destination walk became invalid, skipping update
{
   "ADDRESS": {
      "city": "New York",
      "postal code": 10012,
      "state": "NY",
      "street address": "599 Lafayette St"
   },
   "age": 25,
   "children": [
      "Olivia"
   ],
   "name": "John",
   "phone": [
      {
         "number": "112-555-1234",
         "type": "mobile"
      },
      {
         "number": "113-123-2368",
         "type": "mobile"
      }
   ],
   "spouse": "Martha"
}
bash $ 
```

to achieve what's intended, first the deepest labels have to be walked/processed and then the outers:
```
bash $ <ab.json jtc -x'[Directory][0][address]' -y'[:]<>k' -y'<>k' -eu echo '"{}"' \| tr '[:lower:]' '[:upper:]' \; | jtc -w'[Directory][0]'
{
   "ADDRESS": {
      "CITY": "New York",
      "POSTAL CODE": 10012,
      "STATE": "NY",
      "STREET ADDRESS": "599 Lafayette St"
   },
   "age": 25,
   "children": [
      "Olivia"
   ],
   "name": "John",
   "phone": [
      {
         "number": "112-555-1234",
         "type": "mobile"
      },
      {
         "number": "113-123-2368",
         "type": "mobile"
      }
   ],
   "spouse": "Martha"
}
bash $ 
```


### Insert, Update with move
if a source argument for either `-i` or `-u` is given in the form of `<file>` or `<JSON>`, then those obviously cannot be moved.
The move semantic is only applicable when the argument is given in the form of a `<walk-path>`, then upon completing
the operation, the source elements (referred by the source walk-path) become possible to remove (purge).
This is achievable by letting specifying `-p` switch for such operations only.

Let's move `address` from the last `Directory` record into the first one:
```
bash $ <ab.json jtc -w'[Directory][0][address]' -u'[Directory][-1:][address]' -p
{
   "Directory": [
      {
         "address": {
            "city": "Denver",
            "postal code": 80206,
            "state": "CO",
            "street address": "6213 E Colfax Ave"
         },
         "age": 25,
         "children": [
            "Olivia"
         ],
         "name": "John",
         "phone": [
            {
               "number": "112-555-1234",
               "type": "mobile"
            },
            {
               "number": "113-123-2368",
               "type": "mobile"
            }
         ],
         "spouse": "Martha"
      },
      {
         "address": {
            "city": "Seattle",
            "postal code": 98104,
            "state": "WA",
            "street address": "5423 Madison St"
         },
         "age": 31,
         "children": [],
         "name": "Ivan",
         "phone": [
            {
               "number": "273-923-6483",
               "type": "home"
            },
            {
               "number": "223-283-0372",
               "type": "mobile"
            }
         ],
         "spouse": null
      },
      {
         "age": 25,
         "children": [
            "Robert",
            "Lila"
         ],
         "name": "Jane",
         "phone": [
            {
               "number": "358-303-0373",
               "type": "office"
            },
            {
               "number": "333-638-0238",
               "type": "home"
            }
         ],
         "spouse": "Chuck"
      }
   ]
}
bash $
```
\- That leaves `Jane` without an address, while `John` "moves" into `Jane`'s place!

### Insert, Update: argument shell evaluation
An argument for _insert_ and _update_ operations (`-i`, `-u`) optionally may undergo a shell evaluation (predicated by `-e`). 
E.g., let's capitalize all the `name` entries in the address book:
```
bash $ <ab.json jtc -w'<name>l:' 
"John"
"Ivan"
"Jane"
bash $
bash $ <ab.json jtc -w'<name>l:' -eu echo '"{}"' \| tr "[:lower:]" "[:upper:]" \; | jtc -w'<name>l:'
"JOHN"
"IVAN"
"JANE"
bash $
```
Once options `-e` and `-i`,`-u` used together, following rules must be observed:
- option `-e` must precede first occurrence of `-i`,`-u`
- cli arguments sequence following option `-i`,`-u` must be terminated with escaped semicolon: `\;`
- the cli is also subjected for namespace / walk interpolation before it gets shell evaluated
- the cli in argument do not require any additional escaping (except those which would normally be required by bash shell)
- if pipeing in the cli is required then pipe symbol itself needs to be escaped and spelled standalone: `\|`
- returned result of a shell evaluation must be either a valid JSON, or non-empty and non-error, then it's
promoted to a _JSON string_ value
- failed (those returning non-zero exit code) or empty results of the shell evaluations are ignored 
(JSON entry wont be updated/inserted, rather proceed to the next walked entry for another update attempt)


if shell cli does not deliver expected result for some reason, it's easy to see why with `-dd` option, e.g, say we want to 
truncate all kid's names to 3 letters only:
```
bash $ <ab.json jtc -w'<children>l:[:]' -j | jtc -w[:] -eu sed -E 's/(...).*/\1/'<<<{} \; 
{}
bash $ <ab.json jtc -w'<children>l:[:]' -j | jtc -w[:] -eu sed -E 's/(...).*/\1/'<<<{} \; -dd
.read_inputs(), reading json from <stdin>
..demux_opt(), option: '-u', hits: 3
.parse_params_(), option '-u': total jsons: 0, total iterators: 0
..walk(), walk string: '[:]'
..walk(), dump of completed lexemes:
..walk(), [0]: WalkStep.. search_type():"numeric_offset", lexeme:"[:]", ws_type():"range_walk", ofst():"0", range():"[:]", label():"N/A", json():"N/A"
..walk(), initial walk requires iteration
..walk_interleaved_(), multi-walk: false
..walk_interleaved_(), walk-path instances: 1:
..walk_interleaved_(), instance: 0, iterators: 0
.write_json(), outputting json to <stdout>
{}
bash $ 
```

It did not work, because `jtc` recieved at the input only just this `{}`. Obviously `<<<{}` was interpolated by shell -  
`jtc` got that, thus, we have to quote it:
```
bash $ <ab.json jtc -w'<children>l:[:]' -j | jtc -w[:] -eu sed -E 's/(...).*/\1/<<<{}' \; -dd
.read_inputs(), reading json from <stdin>
..demux_opt(), option: '-u', hits: 3
.parse_params_(), option '-u': total jsons: 0, total iterators: 0
..walk(), walk string: '[:]'
..walk(), dump of completed lexemes:
..walk(), [0]: WalkStep.. search_type():"numeric_offset", lexeme:"[:]", ws_type():"range_walk", ofst():"0", range():"[:]", label():"N/A", json():"N/A"
..walk(), initial walk: successful match
..walk_interleaved_(), multi-walk: true
..walk_interleaved_(), walk-path instances: 1:
..walk_interleaved_(), instance: 0, iterators: 3
..reconcile_ui_(), interpolated & quoted string: 'sed \-E s\/(...).*\/\1\/<<<Olivia'
..system(), executing cmd 'sed \-E s\/(...).*\/\1\/<<<Olivia'
sh: -c: line 0: syntax error near unexpected token `('
sh: -c: line 0: `sed \-E s\/(...).*\/\1\/<<<Olivia'
..system(), return 512: ''
error: shell returned error (512)
..reconcile_ui_(), interpolated & quoted string: 'sed \-E s\/(...).*\/\1\/<<<Robert'
..system(), executing cmd 'sed \-E s\/(...).*\/\1\/<<<Robert'
sh: -c: line 0: syntax error near unexpected token `('
sh: -c: line 0: `sed \-E s\/(...).*\/\1\/<<<Robert'
..system(), return 512: ''
error: shell returned error (512)
..reconcile_ui_(), interpolated & quoted string: 'sed \-E s\/(...).*\/\1\/<<<Lila'
..system(), executing cmd 'sed \-E s\/(...).*\/\1\/<<<Lila'
sh: -c: line 0: syntax error near unexpected token `('
sh: -c: line 0: `sed \-E s\/(...).*\/\1\/<<<Lila'
..system(), return 512: ''
error: shell returned error (512)
.write_json(), outputting json to <stdout>
[
   "Olivia",
   "Robert",
   "Lila"
]
bash $ 

```
Now it did not work, because `jtc` received `sed`'s argument without single quotations (again those have been eaten by bash 
before pasing to `jtc` -  something `jtc` is unable to control - shell arguments evaluation of its arguments). Thus, 
let's double quote it now:

```
bash $ <ab.json jtc -w'<children>l:[:]' -j | jtc -w[:] -eu sed -E '"s/(...).*/\1/"<<<{}' \; -dd
.read_inputs(), reading json from <stdin>
..demux_opt(), option: '-u', hits: 3
.parse_params_(), option '-u': total jsons: 0, total iterators: 0
..walk(), walk string: '[:]'
..walk(), dump of completed lexemes:
..walk(), [0]: WalkStep.. search_type():"numeric_offset", lexeme:"[:]", ws_type():"range_walk", ofst():"0", range():"[:]", label():"N/A", json():"N/A"
..walk(), initial walk: successful match
..walk_interleaved_(), multi-walk: true
..walk_interleaved_(), walk-path instances: 1:
..walk_interleaved_(), instance: 0, iterators: 3
..reconcile_ui_(), interpolated & quoted string: 'sed \-E "s/(...).*/\1/"<<<Olivia'
..system(), executing cmd 'sed \-E "s/(...).*/\1/"<<<Olivia'
..system(), return 0: 'Oli
'
..walk(), walk string: ''
..walk(), dump of completed lexemes:
..walk(), invalidated search cache
..walk(), initial walk: successful match
..reconcile_ui_(), interpolated & quoted string: 'sed \-E "s/(...).*/\1/"<<<Robert'
..system(), executing cmd 'sed \-E "s/(...).*/\1/"<<<Robert'
..system(), return 0: 'Rob
'
..walk(), walk string: ''
..walk(), dump of completed lexemes:
..walk(), invalidated search cache
..walk(), initial walk: successful match
..reconcile_ui_(), interpolated & quoted string: 'sed \-E "s/(...).*/\1/"<<<Lila'
..system(), executing cmd 'sed \-E "s/(...).*/\1/"<<<Lila'
..system(), return 0: 'Lil
'
..walk(), walk string: ''
..walk(), dump of completed lexemes:
..walk(), invalidated search cache
..walk(), initial walk: successful match
.write_json(), outputting json to <stdout>
[
   "Oli",
   "Rob",
   "Lil"
]
bash $ 
```
Now it worked!

Actually the whole cli could have been quoted like this:
```
bash $ <ab.json jtc -w'<children>l:[:]' -j | jtc -w[:] -eu 'sed -E "s/(...).*/\1/"<<<{};'
[
   "Oli",
   "Rob",
   "Lil"
]
bash $
```



### Mixed use of arguments for `-i`, `-u`, `-c` 
options `-c`, `-u`, `-i` allow two kinds of their arguments:
1. static JSONs (i.e., `<file>`, `<JSON>`)
2. walk-path (i.e., `<walk-path>`)

\- when those used together, namely a `<walk-path>` argument(s) follows either of statics, e.g.: 
```
jtc -u file.json -u'[Root][:]'`
```
then all subsequent `<walk-path>` apply onto the first argument (here `file.json`). 
\- if `<walk-path>` arguments are given without preceding static JSON, then walk-path are applied onto the input (source) JSON

That rule is in play to facilitate a walking capability over the specified static JSONs. Though be aware: _all specified `<walk-path>`
will be processed in a consecutive order, one by one (i.e. interleaving occurs only with `-w` walks)_.


### Mixed use of arguments with `-e`
options `-u`, `-i` when used together with `-e` also allow specifying multiple instances of the option usage:
1. first option occurrence must prove a shell cli line, terminated with `;`
2. all the subsequent option usages must provide `<walk-path>` type of argument, which let specifying source(s) of interpolation.
Thus, in the case if mixed option arguments usage is detected (together with `-e`), then the semantic of the jtc arguments would be
like this (e.g., for option `-u`):
```
jtc  -w'<dst>' -e -u <shell cli ...> \; -u'<src>' 
```
That way it's possible to decouple source(s) (of interpolation) from the destination(s): all trailing (subsequent) arguments of `-u`
will be used in every shell evaluation (interpolating respective JSON elements), while arguments pointed by (all) `-w` option(s)
will point where returned/evaluated resulting JSONs should be placed.

The described argument behavior facilitates transformation of a JSON when a source location of transformation is not the same as
a destination

Hopefully this example will clarify:
- say (just for the sake of example), we want to add to every record's `children` the `name` of the person, but not just - we
want to add it in all capitals (i.e. transform the record).
```
bash $ <ab.json jtc -ei echo '"{}"' \| tr '[:lower:]' '[:upper:]' \; -i'<name>l:' -w'<children>l:' | jtc -lrw'<name>l:' -w'<children>l:'
"name": "John"
"children": [ "Olivia", "JOHN" ]
"name": "Ivan"
"children": [ "IVAN" ]
"name": "Jane"
"children": [ "Robert", "Lila", "JANE" ]
bash $ 
```
- there the source(s) of shell interpolation were `name` records (provided with `-i'<name>l:'`), while the destination were `children`
(specified with `-w'<children>l:'`)

In case if only a single option instance (`-u`/`-i`) is used, then both the source (of interpolation) and the destination
(of operation) would be provided with `-w` option argument


### Summary of modes of operations
`jtc` supports multiple update (and insertion) modes, at first it's easy to get confused, so let's recap here all possibilities:

###### _A._ 
| Update JSON from other locations of the same JSON: |
|:-:|
| `<file.json jtc -w<dst_wlk> -u<src_wlk>` |
- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from `src_wlk` 

###### _B._
| Update JSON with a static JSON (either from file, or a spelled literally): |
|:-:|
| `<file.json jtc -w<dst_wlk> -u<static.json>` |
- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from `static.json` (a file, or a literal JSON)

###### _C._
| Update JSON from some locations from a static JSON (either from file, or a spelled literally): |
|:-:|
| `<file.json jtc -w<dst_wlk> -u<static.json> -u<src_wlk>` |
- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from `src_wlk` walked `static.json`

###### _D._
| Update JSON with the transformed JSON elements via shell cli: |
|:-:|
| `<file.json jtc -w<dst_wlk> -e -u<cli>` |
- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from shell-evaluation of `cli`. `cli` here is subjected for
interpolation from namespaces and/or JSON elements pointed by `dst_wlk` itself. The namespaces here is also populated while
walking `dst_wlk`.

###### _E._
| Update JSON from some locations of the transformed JSON via shell cli: |
|:-:|
| `<file.json jtc -w<dst_wlk> -e -u<cli> -u<src_wlk>` |
- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from `src_wlk` walking shell-evaluated `cli`. 
`cli` here is subjected for interpolation from namespaces and/or JSON elements pointed by `dst_wlk` itself. The namespaces
here is also populated while walking `dst_wlk`.

_NOTE: if destination walk (`-w`) is pointing to the root of JSON then the switch can be entirely dismissed (in any of the above
examples)_

## Comparing JSONs
`-c` allows comparing JSONs (or JSONs element pointed by walk-paths) - `jtc` will display JSON delta (diffs) between compared JSONs. 
Let's compare `phone` records from the first and  the second entries of the address book:
```
bash $ <ab.json jtc -w'[Directory][0][phone]' -c'[Directory][1][phone]' -l
"json_1": [
   {
      "number": "112-555-1234",
      "type": "mobile"
   },
   {
      "number": "113-123-2368"
   }
]
"json_2": [
   {
      "number": "273-923-6483",
      "type": "home"
   },
   {
      "number": "223-283-0372"
   }
]
bash $
```
When both JSONs are equal, an empty set is displayed and return code is 0.
```
bash $ echo '123' | jtc -c'123' -l
"json_1": {}
"json_2": {}
bash $ echo $?
0
bash $
```
Otherwise (JSONs are different) a non-zero code is returned:
```
bash $ echo '[1,2,3]' | jtc -c'[2,3]' -lr
"json_1": [ 1, 2, 3 ]
"json_2": [ 2, 3 ]
bash $ echo $?
5
bash $
```
If multiple pairs of JSONs compared, zero code is returned only when all compared JSON pairs are equal. 

### Comparing JSON schemas
JSON schema essentially is a JSON structure (JSON containers, labels, indices) without leaf data. I.e., two JSONs may have
different contents (leaf data), while their structures could be the same (though the statement is rather loose - JSON schema does
include types of the leaf data as well).

E.g., if we add/insert a child into `Ivan`'s record, then the record would be different from the original:
```
bash $ <ab.json jtc -w'<Ivan>[-1] [children]' -i'"Norma"' | jtc -w'<Ivan>[-1]' -c'ab.json' -c'<Ivan>[-1]' -l
"json_1": {
   "children": [
      "Norma"
   ]
}
"json_2": {}
bash $ 
```

However, their schemas would be the same. To compare schemas of two JSONs (well, with applied exemption on checking leaves data types),
label directive `<>k` used together with `<>c` search suffix come handy:
```
bash $ <ab.json jtc -w'<Ivan>[-1] [children]' -i'"Norma"' | jtc -w'<Ivan>[-1] <>c: <>k' -c'ab.json' -c'<Ivan>[-1] <>c: <>k' -l
"json_1": {}
"json_2": {}
"json_1": {}
"json_2": {}
"json_1": {}
"json_2": {}
"json_1": {}
"json_2": {}
"json_1": {}
"json_2": {}
"json_1": {}
"json_2": {}
bash $ echo $?
0
bash $
```

NOTE: _usage of '<>k' is only restricted to JSON elements which have labels/indices. JSON `root` does not have any of those, thus
attempting to print a label of the root always results in the exception:_
```
bash $ <ab.json jtc -w'<>k'
jtc json exception: walk_root_has_no_label
bash $ 
```



## Interpolation
Interpolation may occur either for argument undergoing
[shell evaluation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation)
(`-e` in insert/update operations), or for template arguments (`-T`).

Interpolation occurs either from the namespaces, or from currently walked JSON element. Every occurrence (in the templates or in
shell cli) of tokens `{}` or `{{}}` will trigger interpolation:
- if the content under braces is empty (`{}`, `{{}}`) then interpolation happens from currently walked JSON element
- if the content is present (e.g.: `{val}`, `{{val}}`) then interpolation occurs from the relevant namespace.
The difference between single `{}` and double `{{}}` notation: upon interpolation of single notation when _JSON string_ is interpolated
then outer quote marks are dropped (other JSON elements interpolated w/o any change); when double notation is getting interpolated
then no exemption made - all JSON elements interpolated without any changes.

A string interpolation w/o outer quotes is handy when required altering an existing string, here's example of altering JSON label:
```
bash $ echo '{ "label": "value" }' | jtc 
{
   "label": "value"
}
bash $ echo '{ "label": "value" }' | jtc -w'<label>l<>k' -u'<label>l<lbl>k' -T'"new {lbl}"'
{
   "new label": "value"
}
bash $ 
```
Here's an illustration when double braces are handier (e.g., swapping around labels and values):
```
bash $ echo '{ "pi": 3.14, "type": "irrational" }' | jtc
{
   "pi": 3.14,
   "type": "irrational"
}
bash $ echo '{ "pi": 3.14, "type": "irrational" }' | jtc -i'[:]<key>k<val>v' -T'{ "{val}": {{key}} }' -p
{
   "3.14": "pi",
   "irrational": "type"
}
bash $ 
```

### Search quantifiers interpolation
Also interpolation may occur in quantifiers, say we have a following JSON, where we need to select an item from `list` by the
index value stored `item`:
```
bash $ echo '{ "item": 2, "list": { "milk": 0.90, "bread": 1.20, "cheese": 2.90 } }' | jtc 
{
   "item": 2,
   "list": {
      "bread": 1.20,
      "cheese": 2.90,
      "milk": 0.90
   }
}
bash $ 
```
To achieve that, we need to memorize the value of index in the namespace first, then select a value from the list by the index:
```
bash $ echo '{ "item": 2, "list": { "milk": 0.90, "bread": 1.20, "cheese": 2.90 } }' | jtc -w'[item]<idx>v[-1][list]><a{idx}' -l
"milk": 0.90
bash $ 
```
It should be quite easy to read/understand such walk path (predicated one is familiar with suffixes / directives). Let's see
how the walk-path works in a slow-mo:
1. `[item]` selects the value by label `item`:
```
bash $ J=$(echo '{ "item": 2, "list": { "milk": 0.90, "bread": 1.20, "cheese": 2.90 } }')
bash $ echo $J | jtc -w'[item]'
2
bash $ 
```
2. `<idx>v` the directive memorizes selected value (`2`) in the namespace `idx`
```
bash $ echo $J | jtc -w'[item]<idx>v'
2
bash $ 
```

3. `[-1]` steps up 1 level in the JSON tree off the current position (i.e. addresses the first parent of the `item` value) which is
the root of th input json:
```
bash $ echo $J | jtc -w'[item]<idx>v[-1]'
{
   "item": 2,
   "list": {
      "bread": 1.20,
      "cheese": 2.90,
      "milk": 0.90
   }
}
bash $ 
```
4. `[list]` selects the object value by label `list`:
```
bash $ echo $J | jtc -w'[item]<idx>v[-1][list]'
{
   "bread": 1.20,
   "cheese": 2.90,
   "milk": 0.90
}
bash $ 
```
5. `><a{idx}` - a non-recursive search of atomic values (`><a`) indexed by a quantifier with the stored in the namespace `idx`
(which is `2`) gives us the required value.



## Templates
Template is a literal JSON containing tokens for
[interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation). Templates can be used upon walking, 
insertion, updates and when comparing. The result of template interpolation still must be a valid JSON. If a template(s) is given
then it's a template (after interpolation) will be used for the operation, not the source walk.

When walking only is in process, then template interpolation occurs from the walk-path (`-w`):
```
bash $ <ab.json jtc -w'[0][0]<number>l:' 
"112-555-1234"
"113-123-2368"
bash $ <ab.json jtc -w'[0][0]<number>l:' -T'"+1 {}"'
"+1 112-555-1234"
"+1 113-123-2368"
bash $ 
```

For the rest of operations (`-i`, `-u`, `-c`) templates are getting interpolated from walk-path of the operation itself and
never from `-w`. The namespaces resulting from walking `-w` and any of operations (`-i`, `-u`, `-c`) do not intersect or clash,
as operation parameters walking (often referred as source walk) and template interpolation always occurs before starting walking `-w`
(and generally, namespaces for `-w` and source walks even reside in different containers).

Below is an example of updating the phone records for the first entry in the `Directory`:
```
bash $ <ab.json jtc -w'[0][0]<phone>l'
[
   {
      "number": "112-555-1234",
      "type": "mobile"
   },
   {
      "number": "113-123-2368",
      "type": "mobile"
   }
]
bash $ <ab.json jtc -w'[0][0]<phone>l[:]' -pi'[0][0]<number>l:<val>v' -T'{ "phone number": "+1 {val}" }' | jtc -w'[0][0]<phone>l'
[
   {
      "phone number": "+1 112-555-1234",
      "type": "mobile"
   },
   {
      "phone number": "+1 113-123-2368",
      "type": "mobile"
   }
]
bash $ 
```
Explanations:
- `-w'[0][0]<phone>l[:]'` - that's our destination which we will be updating (i.e. all phone records in the first `Directory` entry)
- `-pi'[0][0]<number>l:<val>v'` - we'll walk (synchronously with `-w`) all the `number` records and memorize number values in the
namespace `val`; option `-p` turns _insert_ operation into _move_
- `-T'{ "phone number": "+1 {val}" }'` after each walk (in `-i`) a template interpolations occurs here - a new JSON entry is generated
from the template and namespace `val` and the new entry is then used for insertion into the respective destination walk (`-w`).
Thus using templates it becomes easy to transmutate existing JSON into a new one.


## Processing multiple input JSONs
Normally `jtc` would process only a single input JSON if multiple input JSONs given - the fist JSON will be processed and the 
rest of the inputs will be silently ignored:
```
bash $ echo '[ "1st json" ] { "2nd": "json" } "3rd json"' | jtc
[
   "1st json"
]
bash $ 
```
Couple options allow altering the behavior and process all the input JSONs:

### Process all input JSONs
Option `-a` instructs to process all the input JSONS:
```
bash $ echo '[ "1st json" ] { "2nd": "json" } "3rd json"' | jtc -a
[
   "1st json"
]
{
   "2nd": "json"
}
"3rd json"
bash $ 
```
\- respected processing (of all given options) will occur for all of the input JSONs:
```
bash $ echo '[ "1st json" ] { "2nd": "json" } "3rd json"' | jtc -a -w'<json>R'
"1st json"
"json"
"3rd json"
bash $ 
```
All the input JSONs will be processed as long they are valid - processing will stops upon the parsing failure:
```
bash $ echo '[ "1st json" ] { "2nd": json" } "3rd json"' | jtc -ad
.read_inputs(), reading json from <stdin>
.write_json(), outputting json to <stdout>
[
   "1st json"
]
.parsejson(), exception locus: { "2nd": json" } "3rd json"|
.location_(), exception spot: --------->| (offset: 9)
jtc json exception: expected_json_value
bash $ 
```

### Wrap all processed JSONs
option `-J` let wrapping all processed inputs into JSON array (option `-J` assumes option `-a`, no need giving both):
```
bash $ echo '[ "1st json" ] { "2nd": "json" } "3rd json"' | jtc -J -w'<json>R'
[
   "1st json",
   "json",
   "3rd json"
]
bash $ 
```
option `-J` also implicitly imposes `-j` thus it could be used safely even with a single JSON at the input with the same effect. 
Though, when walking multiple input JSONs, each of the option would have its own effect, this example clarifies:
```
bash $ cat ab.json ab.json | jtc -w'[0][:][name]' -aj
[
   "John",
   "Ivan",
   "Jane"
]
[
   "John",
   "Ivan",
   "Jane"
]
bash $ 
bash $ cat ab.json ab.json | jtc -w'[0][:][name]' -J
[
   "John",
   "Ivan",
   "Jane",
   "John",
   "Ivan",
   "Jane"
]
bash $ 
bash $ cat ab.json ab.json | jtc -w'[0][:][name]' -Jj
[
   [
      "John",
      "Ivan",
      "Jane"
   ],
   [
      "John",
      "Ivan",
      "Jane"
   ]
]
bash $ 
```


## More Examples
### Generating new JSON (1)
Say, we need to craft a new JSON out of our address book, extracting only selected (and possibly renamed) fields. This is attainable
if 3 simple steps:
1. First let's prepare a template of our new
JSON:
```
bash $ echo '[{"Person":"", "Age":"", "Children":""}]' | jtc >abc.json
bash $ jtc abc.json
[
   {
      "Age": "",
      "Children": "",
      "Person": ""
   }
]
bash $
```
2. Now we need to multiply those records, to match number of records in the address book:
```
bash $ CNT=$(jtc -w'[Directory][:]' -r ab.json  | wc -l); # just count the number of entries
bash $ for i in $(seq 2 $CNT); do jtc -f -i'[0:1]' abc.json; done
bash $ jtc abc.json
[
   {
      "Age": "",
      "Children": "",
      "Person": ""
   },
   {
      "Age": "",
      "Children": "",
      "Person": ""
   },
   {
      "Age": "",
      "Children": "",
      "Person": ""
   }
]
bash $
```
in this step:
- `for` loop is arranged in the sequence from 2 to numbers of records in the `Directory` (`jtc -w'[Directory][:]' -r ab.json  | wc -l`)
- in each pass, file `abc.json` modified in-place (`-f`) by inserting the first entry from the book (`-i'[0:1]'`)
into the root (if no `-w` given, root is assumed)

3. Map (via update) required values from the address book (`ab.json`) onto the respective values in the template (`abc.json`):
```
bash $ jtc -nw'<Person>l:' -w'<Age>l:' -w'<Children>l:' -u ab.json -u'<name>l:' -u'<age>l:' -u'<children>l:' abc.json
[
   {
      "Age": 25,
      "Children": [
         "Olivia"
      ],
      "Person": "John"
   },
   {
      "Age": 31,
      "Children": [],
      "Person": "Ivan"
   },
   {
      "Age": 25,
      "Children": [
         "Robert",
         "Lila"
      ],
      "Person": "Jane"
   }
]
bash $
```
NOTE: because all `-u <walk-path>` options (which applied onto `ab.json`, rather than onto `abc.json` in this scenario) are being
process sequentially, option `-n` was used to ensure sequential execution of all `-w` options too (so that mapping would occur
onto respective entries).

### Generating new JSON (2)
The same operations could have been achieved in a different (probably a more concise) way:
1. purge JSON leaving only required records (namely `name`, `age`, `children`):
```
bash $ <ab.json jtc -x'[Directory][:]' -y'<name>l' -y'<age>l' -y'<children>l' -pp | jtc -w'[Directory]'
[
   {
      "age": 25,
      "children": [
         "Olivia"
      ],
      "name": "John"
   },
   {
      "age": 31,
      "children": [],
      "name": "Ivan"
   },
   {
      "age": 25,
      "children": [
         "Robert",
         "Lila"
      ],
      "name": "Jane"
   }
]
bash $ 
```

2. update all labels (in 3 successive updates) with required new labels:
```
bash $ <ab.json jtc -x'[Directory][:]' -y'<name>l' -y'<age>l' -y'<children>l' -pp | jtc -w'[Directory]' | jtc -w'<age>l:<>k' -u'"Age"' | jtc -w'<children>l:<>k' -u'"Children"' | jtc -w'<name>l:<>k' -u'"Person"'
[
   {
      "Age": 25,
      "Children": [
         "Olivia"
      ],
      "Person": "John"
   },
   {
      "Age": 31,
      "Children": [],
      "Person": "Ivan"
   },
   {
      "Age": 25,
      "Children": [
         "Robert",
         "Lila"
      ],
      "Person": "Jane"
   }
]
bash $ 
```

### Taming duplicates
Quite very common tasks (and requests) for JSON is to process duplicates. Say, we deal with the following JSON:
```
bash $ echo '[ "string", true, null, 3.14, "string", null ]' | jtc 
[
   "string",
   true,
   null,
   3.14,
   "string",
   null
]
bash $ 
```
So, let's
#### Remove duplicates
```
bash $ echo '[ "string", true, null, 3.14, "string", null ]' | jtc -w'<.>Q:' -p
[
   "string",
   true,
   null,
   3.14
]
bash $ 
```
Because switch `-p` is given, all the duplicate elements will be purged, thus leaving the list only with non-duplicate (unique) 
elements

But there's a reverse action:
#### Remove all but duplicates
```
bash $ echo '[ "string", true, null, 3.14, "string", null ]' | jtc -w'<.>Q:' -pp
[
   "string",
   null
]
bash $ 
```
That one is obvious - we just reversed the prior example.

How about:
#### Leave only those which have no duplicates
```
bash $ echo '[ "string", true, null, 3.14, "string", null ]' | jtc -w'<.>Q:[^0]<.>s:' -p 
[
   true,
   3.14
]
bash $ 
```
it's just a tiny bit more complex:
- `<.>Q:` - for each duplicate element, we'll memorize it into `.` namespace, then
- `[^0]<.>s:` reset the search path back to the root and now find all the elements (i.e. all duplicates).

that way all duplicates (and their origins) will be removed, leaving the array only with those which have no duplicates.

and finally
#### Leave all duplicates
```
bash $ echo '[ "string", true, null, 3.14, "string", null ]' | jtc -w'<.>Q:[^0]<.>s:' -pp
[
   "string",
   null,
   "string",
   null
]
bash $ 
```
it's just a reverse action.











