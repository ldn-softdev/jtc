
# [`jtc`](https://github.com/ldn-softdev/jtc). Examples and Use-cases

1. [Displaying JSON](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#displaying-json)
   * [Pretty printing (`-t`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#pretty-printing)
   * [Compact printing (`-r`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#compact-printing)
   * [Printing JSON size (`-z`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#printing-json-size)
   * [Validating JSON (`-d`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#validating-json)
   * [Forcing strict solidus parsing (`-q`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#forcing-strict-solidus-parsing)
   * [Unquoting JSON strings (`-qq`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#unquoting-JSON-strings)
   * [Stringifying JSON (`-rr`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying_json) 
2. [Walking JSON (`-w`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-json)
   * [Walking with subscripts (`[...]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-with-subscripts-offset-lexemes)
     * [Selecting multiple subscripted JSON elements (`[+n], [n:n]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#selecting-multiple-subscripted-json-elements)
   * [Searching JSON (`<...>`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#searching-json)
     * [Searching JSON with RE (`<...>R`,`<...>L`, `<...>D`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#searching-json-with-re)
     * [Search suffixes (`rRlLdDbnaoijwe`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-suffixes)
     * [Search quantifiers (`n`,`+n`,`n:n`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers)
     * [Scoped search `[...]:<...>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#scoped-search)
     * [Non-recursive search (`>...<`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#non-recursive-search)
   * [Addressing parents (`[-n]`, `[^n]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#addressing-parents)
   * [Walking multiple paths](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-multiple-paths)
     * [Sequential walk processing (`-n`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#sequential-walk-processing)
     * [Displaying walks with labels (`-l`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#displaying-walks-with-labels)
     * [Wrapping resulted walks to JSON (`-j`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-resulted-walks-to-json)
     * [Interleaved walk processing](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interleaved-walk-processing)
     * [Succinct walk-path syntax (`-x`,`-y`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#succinct-walk-path-syntax)
3. [Modifying JSON](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#modifying-json)
   * [In-place JSON modification (`-f`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#in-place-json-modification)
   * [Purging JSON (`-p`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#purging-json)
   * [Swapping JSON elements (`-s`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#swapping-json-elements)
   * [Insert operations (`-i`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-operations)
     * [Destination driven insertion](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#destination-driven-insertion)
     * [Inserting objects into objects](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#inserting-objects-into-objects)
     * [Insertion matrix without merging](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insertion-matrix-without-merging)
     * [Insertion matrix with merging (`-m`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insertion-matrix-with-merging)
   * [Update operations (`-u`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#update-operations)
     * [Update operations matrix](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#update-operations-matrix)
   * [Insert, Update with move (`-i`/`-u`,`-p`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-with-move)
   * [Insert, Update: argument shell evaluation (`-e`,`-i`/`-u`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation)
   * [Mixed use of arguments (`<JSON>, <walk-path>`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#mixed-use-of-arguments)
4. [Comparing JSONs (`-c`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#comparing-jsons)
5. [More Examples](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#more-examples)
   * [Working with templates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#working-with-templates)
     
     
## Displaying JSON
### Pretty printing
if no argument given, `jtc` will expect an input JSON from the `<stdin>`, otherwise JSON is read from the file pointed by the argument.
`jtc` will parse and validate input JSON and upon a successful validation will output:
```
bash $ cat ab.json | jtc
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
bash $ cat ab.json | jtc -t 10
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
bash $ cat ab.json | jtc -r
{ "Directory": [ { "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" }, "age": 25, "children": [ "Olivia" ], "name": "John", "phone": [ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ], "spouse": "Martha" }, { "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null }, { "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" } ] }
bash $
```

### Printing JSON size
JSON size is the total number of the JSON elements found within JSON, it could be printed using `-z`, the size appears after JSON
is printed:
```
bash $ cat ab.json | jtc -r -z
{ "Directory": [ { "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" }, "age": 25, "children": [ "Olivia" ], "name": "John", "phone": [ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ], "spouse": "Martha" }, { "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null }, { "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" } ] }
size: 56
bash $
```
if size only required, then use `tail` command:
```
bash $ cat ab.json | jtc -rz | tail -1
size: 56
bash $
```

### Validating JSON
When JSON is read (from a file, or from `stdin`), it get parsed and validated. If an invalid JSON is detected, the short exception
message will be displayed, e.g,:
```
bash $ cat ab.json | jtc
jtc json exception: unexpected_end_of_line
bash $
```
though the message let us know that there's a problem with the input JSON, it not very informative with regard whereabouts the
the problem. An easy way to see the spot where the problem and its locus is via debug (`-d`):
```
bash $ cat ab.json | jtc -d
.read_json(), start parsing json from file: <stdin>
.read_json(), exception locus: ...6213 E Colfax Ave",|          "state": "CO,|          "postal code": 80206|       },| ...
.read_json(), exception spot: ---------------------------------------------->| (offset: 1150)
jtc json exception: unexpected_end_of_line
bash $
```
the vertical pipe symbol `|` in the debug replaces new lines, thus it becomes easy to spot the problem. 
The offset (`1150`) is given in bytes from the beginning of the input/file. In that particular failure instance, `jtc`
found end of the line, while _JSON string_ `"Co,` is still open (JSON standard does permit multi-line strings).
To fix it, the missing quotation mark to be added

### Forcing strict solidus parsing
JSON specification allows quoting solidus (`/`) optionally. By default, `jtc` is relaxed w.r.t. solidus notation - it admits
both quoted and unquoted appearances:
```
bash $ echo '{ "quoted": "\/", "unquoted": "/" }' | jtc 
{
   "quoted": "\/",
   "unquoted": "/"
}
bash $
```
If there's a need for a strict solidus quoting parsing, option `-q` facilitates the need. It also will throw an exception 
upon facing a non-quoted notation:
```
bash $ echo '{ "quoted": "\/", "unquoted": "/" }' | jtc -q -d
.read_json(), start parsing json from <stdin>
.read_json(), exception locus: { "quoted": "\/", "unquoted": "/" }|
.location_(), exception spot: ------------------------------->| (offset: 31)
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
NOTE: _the option notation `-qq` will not engulf a single notation `-q`, if both behaviors are required the both variants have
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
- offset lexeme (`[...]`)
- search lexeme (`<...>`)

### Walking with subscripts (offset lexemes)
offsets are always enclosed into square brackets `[`,`]`, selecting of JSON elements always happens from the JSON root.
Both arrays and objects can be subscripted using numerical offset, though it's best to utilize literal offsets to subscript objects.
E.g. let's select `address` of the 2nd (all the indices in the walk-path are zero-based) record in the above JSON:
```
bash $ cat ab.json | jtc -w'[Directory][1][address]'
{
   "city": "Seattle",
   "postal code": 98104,
   "state": "WA",
   "street address": "5423 Madison St"
}
bash $
```
or, equally could be done like this, but the former syntax is preferable (for your own good):
```
bash $ cat ab.json | jtc -w'[0][1][0]'
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
bash $ cat ab.json | jtc -w'[Directory][+1][name]'
"Ivan"
"Jane"
bash $
```
Any numerical offset could be subscripted that way, and any number of such lexemes could appear in the walk-path, e.g.:
```
bash $ cat ab.json | jtc -w'[Directory][+0][phone][+1][number]'
"113-123-2368"
"223-283-0372"
"333-638-0238"
bash $
```
Here, all records (`[+0]`) from the `Directory` were selected and then in every record all `phone` sub-records selected starting from
the 2nd entry

The same way object elements could be subscripted, here's an example where all address entries staring from the 2nd one are printed,
each one stating from the 3rd entry: 
```
bash $ cat ab.json | jtc -w'[Directory][+1][address][+2]'
"WA"
"5423 Madison St"
"CO"
"6213 E Colfax Ave"
bash $
```

Another way to select multiple subscripts is to use range notation `[N:N]`. In that notation `N` could be either positive or negative, 
or entirely missed. First position designates beginning of the selection, the last position designates the end of the range exclusively
(i.e. not including the indexed element itself)
- positive `N` subscripts `N`th element from the beginning of the collection (whether it's array or an object)
- negative `N` subscripts the `N`th element from the end of the collection.
- empty (missed N) tells to address either from the the beginning of the collection (in the first position),
or from the end (last position) 

Thus, multiple notations with the same semantics are possible, e.g.:
- `[:]`, `[0:]` will select all the element in the collection and is equivalent of `[+0]` notation
- `[0:1]`, `[:1]` will select only the first element and is the same as `[0]`
- `[:-1]` will select all the elements except the last one
- `[-2:]` will select last 2 elements in the collection 

let's print all phone numbers for the last 2 records in the `Directory`:
```
bash $ cat ab.json | jtc -w'[Directory][-2:][phone][:][number]'
"273-923-6483"
"223-283-0372"
"358-303-0373"
"333-638-0238"
bash $
```

### Searching JSON
Walk-path lexemes enclosed into `<`,`>` braces instruct to perform a _recursive_ search of the value under a selected JSON node. 
I.e., if the search lexeme appears as the first one in the walk-path, then the search will be done from the root, otherwise
from the node in JSON where a prior lexeme stopped.

By default, if no modifying suffixes given, a search lexeme will search _JSON string_ values only (i.e. it won't match 
_JSON numerical_ or _JSON boolean_ or _JSON null_ values). E.g., following search find a match:
```
bash $ cat ab.json | jtc -w'[Directory][0]<New York>'
"New York"
bash $
```
while this one does not (the string value `New York` is found only in the first `Directory` record): 
```
bash $cat ab.json | jtc -w'[Directory][1:]<New York>'
bash $
```

#### Searching JSON with RE
Search lexemes support suffixes: an optional one letter following the closing bracket.
These suffixes alter the meaning of the search, e.g. suffix `R` instruct to perform a regex search among string values
(btw, RE in `jtc` are PCRE):
```
bash $ cat ab.json | jtc -w'<^N>R'
"New York"
bash $
```

#### Search suffixes
there are following suffixes to control search behavior: 
  * `r`: default (could be omitted), fully matches _JSON string_ value
  * `R`: the lexeme is a search RE, only _JSON sting_ values searched
  * `l`: fully matches _JSON label_
  * `L`: the lexeme is a search RE, only _JSON labels_ searched
  * `d`: fully matches _JSON number_
  * `D`: the lexeme is an RE, only _JSON numerical_ values searched
  * `b`: matches _JSON boolean_ value, the lexeme must be spelled as `<true>b`, `<false>b`, or `<any>b`
  * `n`: matches _JSON null_ value, the lexeme value is ignored, could be anything, like `<null>n`, or `<>n`, etc
  * `a`: matches any JSON atomic value, i.e. strings, numerical, boolean, null, the lexeme value is ignored
  * `o`: matches any JSON object - `{...}`, the lexeme value is ignored
  * `i`: matches any JSON array (iterable/indexable) `[...]`, , the lexeme value is ignored
  * `j`: matches specified JSON value, the lexeme must be a valid JSON, e.g.: `<[]>j` - finds an empty JSON array
  * `w`: matches any JSON value (wide range match): atomic values, objects, arrays, the lexeme value is ignored
  * `e`: matches end-nodes only: atomic values, `[]`, `{}`, the lexeme value is ignored

\- as you can see, capitalizing either of suffixes `r`, `l`, `d` promotes the respective search to the RE search

#### Search quantifiers
Optionally a quantifier may follow the search lexeme (if a lexeme has a suffix, then the quantifier must come after the suffix).
Quantifiers exist in the following formats:
- `n`, - a positive number - tells which instance of a match to pick. By default, a quantifier `0` is applied
(i.e. first match is selected)
- `+n` - selects all match instances starting from `n`th (zero based)
- `N:N` - range select: the notation rules for this quantifier the same as for subscript range (`[N:N]`), with just one understandable
caveat: `N` here cannot go negative (there's no way of knowing upfront how many matches would be produced, so it's impossible to select
a range/slice based off the last match), the rest of notation rules apply

To illustrate the quantifiers (with suffixes), let's dump all the JSON arrays in the `Directory`:
```
bash $ cat ab.json | jtc -w'<>i:' -r
[ { "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" }, "age": 25, "children": [ "Olivia" ], "name": "John", "phone": [ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ], "spouse": "Martha" }, { "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null }, { "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" } ]
[ "Olivia" ]
[ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ]
[]
[ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ]
[ "Robert", "Lila" ]
[ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ]
bash $
```
\- the final `:` in the walk path is the empty quantifier, which selects (prints) all the matches

#### Scoped search 
Search lexemes perform a _recursive_ search across the entire JSON tree off the point where it's invoked (i.e. the JSON node
selected by walking all the prior lexemes). However, sometime's there's a need to limit searching scope only to the specific label.
Here is the dump of all the _JSON strings_, where symbol `5` is sighted:
```
bash $ cat ab.json | jtc -w'<5>R:'
"599 Lafayette St"
"112-555-1234"
"5423 Madison St"
"358-303-0373"
bash $
```
Some of the values are `street address`, some are the phone `number`. Say, we want to dump only the phone records. Knowing the label
of the phone numbers (`"number"`), it's achievable via this notation:
```
bash $ cat ab.json | jtc -w'[number]:<5>R:'
"112-555-1234"
"358-303-0373"
bash $
```
I.e., once the label lexeme is attached to the search lexeme over `:`, it signifies a scoped search.

#### Non-recursive search
Sometimes there's a need to apply a non-recursive search onto collectable JSON nodes (arrays, objects) - i.e. find a value within
immediate children of the element and do not descend recursively. The notation facilitating such search is the same one, but
angular brackets to be put inside-out, i.e.: `>...<`. To illustrate that: say, we want to find all string values in the 1st `Directory`
record containing the letter `o`. If we do this using a recursive search, then all following entries will be found:
```
bash $ cat ab.json | jtc -w'[Directory] [0] <o>R:'
"New York"
"John"
"mobile"
"mobile"
bash $
```
To facilitate our ask - find all such entries within the immediate values of the 1st record, apply a non-recursive search notation:
```
bash $ cat ab.json | jtc -w'[Directory] [0] >o<R:'
"John"
bash $
```


### Addressing parents
One of the charming features of `jtc` is the ability to address parents (any ancestors up till the root) of the found JSON nodes.
Typically addressing parents would be needed after search lexemes (but really can occur anywhere in the walk-path). Parents can
be addressed using notation `[-n]`. This feature allows building queries that answer complex queries.

Let's dump all the names from the `Directory` whose records have a `home` phone entry:
```
bash $ cat ab.json | jtc -w'[type]:<home>: [-3] [name]'
"Ivan"
"Jane"
bash $
```
The magic which happens here (let's break down the walk-path into the lexemes) is revealed below:
  1. `[type]:<home>:` this lexeme instruct to find all (ending `:`) strings `home` scoped by label `"type"`, thus all records will
  be selected:
  ```
  bash $ cat ab.json | jtc -w'[type]:<home>:' -r
  "home"
  "home"
   bash $
  ```
  2. starting off those found JSON elements a 3rd ancestor will be selected. Let's see a parent selection in a slow-mo, one by one:
  ```
  bash $ cat ab.json | jtc -w'[type]:<home>: [-1]' -r
  { "number": "273-923-6483", "type": "home" }
  { "number": "333-638-0238", "type": "home" }
  bash $
  bash $ cat ab.json | jtc -w'[type]:<home>: [-2]' -r
  [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ]
  [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ]
  bash $
  bash $ cat ab.json | jtc -w'[type]:<home>: [-3]' -r
  { "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null   }
  { "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" }
  bash $
  ```
  3. now we can select (subscript) `[name]` of out those JSON nodes


Another example: who is the parent of a child `Lila`?
```
bash $ cat ab.json | jtc -w'<children>l: <Lila> [-2] [name]'
"Jane"
bash $
```
Explanation:
1. `<children>l:` - finds all records with the label `children`
2. `<Lila>` - in found records find string value `Lila`
3. `[-2] [name]` - go 2 levels (parents) up from found entry `"Lila"` and then subscript/offset by label `name`

Even more complex query: who of the parents, those who have children, have mobile numbers?
```
bash $ cat ab.json | jtc -w'<children>l: [0] [-2] [type]:<mobile> [-3] [name]'
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
bash $ cat ab.json | jtc -w'<children>l: [0] [^2] [type]:<mobile> [^2] [name]' -r
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
option `-n` ensures that all given walk-paths (`-w`) will be processed (and output) sequentially in the order they given:
```
bash $ cat ab.json | jtc -w '<name>l:' -w'<number>l:' -n
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
bash $ cat ab.json | jtc -w'<number>l:' -w'<name>l:' -n
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
bash $ cat ab.json | jtc -w '<name>l:' -w'<number>l:' -nl
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
`-j` does a quite simple thing - it wraps walked entries back to JSON, however predicated by `-l` and `-n` options the result will vary:
- `-j` without `-l` will just arrange walked entries into a JSON array:
  ```
  bash $ cat ab.json | jtc -w '<name>l:' -w'<number>l:' -nj 
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
  bash $ cat ab.json | jtc -w '<name>l:' -w'<number>l:' -n -j -l
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
bash $ cat ab.json | jtc -w '<name>l:' -w'<number>l:'
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
(hence they just interleaved one by one). Though if we provide walk-path relating each of those searches to their own record,
then magic happens:
```
bash $ cat ab.json | jtc -w '[Directory][:] <name>l:' -w'[Directory][:] <number>l:'
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
bash $ cat ab.json | jtc -w '[Directory][:] <name>l:' -w'[Directory][:] <number>l:' -jl
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

#### Succinct walk-path syntax
If you look at the prior example, you may notice that a common part of both walk-paths (`[Directory][:]`) we had to give twice. There's
a way to express it in more succinct syntax: options `-x` and `-y` allows rearrange walk-paths so that `-x` takes an initial common part
of the walk-path, where as `-y` would take care of the individuals trailing pars. Thus the same example cold have been written like:
```
bash $ cat ab.json | jtc -x '[Directory][:]' -y'<name>l:' -y'<number>l:'
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
bash $ cat ab.json | jtc -x '[Directory] [:' -y'] <name>l:' -y'] <number>l:'
...
```
However, if a reinstatement of the options results in a valid walk-path - that's all what matters.

It's possible to combine both syntaxes (i.e. `-w` with `-x` and `-y`), however, given that the processing of `-x` and `-y`
internally reinstates respective options `-w`, the former will be appended after any of given `-w` options (which will affect the 
order of processing/outputting) even though the order of their appearance is different:
```
bash $ cat ab.json | jtc -x '[Directory] [:]' -y'<name>l:' -y'<number>l:' -w '<children>l:' -rnl
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
is given in the priority order of operation selection). However, there are certain combination of options `-i`,`-u` and `-p`, which
facilitates _move_ semantic, those cases reviewed in the respective chapters.

Each of options requires one or multiple `-w` to be given to operate on. Options `-i` and `-u` require an argument, which comes in
different flavors, one of them is the walk-path per-se.

`jtc` will execute any of operations only once, if multiple operations required, then `jtc` needs to be invoked in a successive order
(e.g. daisy-chained through the pipe `|`)


### In-place JSON modification 
Whenever either of the option is executed, it will output entire resulting JSON to `stdout`. Option `-f` will apply (redirect)
the result of operation into the file (if the filename is given as an argument and input JSON is not read from `stdin`)


### Purging JSON
`-p` removes from JSON all walked elements (given by one or multiple `-w`). E.g.: let's remove from address book (for the sake
of an example) all the `home` and `office` phones records (effectively leaving only `mobile` types):
```
bash $ cat ab.json | jtc -w'[type]:<home>: [-1]' -w'[type]:<office>: [-1]' -p
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
         "phone": [],
         "spouse": "Chuck"
      }
   ]
}
bash $
```
Of course there's a more succinct syntax:
````
bash $ cat ab.json | jtc -x'[type]:' -y'<home>:[-1]' -y'<office>:[-1]' -p
````
or, using even a single walk-path:
````
cat ab.json | jtc -w'[type]:<(?:home)|(?:office)>R: [-1]' -p
````


Another use-case exist: remove all the JSON elements _except_ walked ones (while preserving original JSON structure) - that's
the feat for multiple option notation: `-pp`. Let's drop all entries (in all records) but the `name` and `spouse`:
````
bash $ cat ab.json | jtc -w'<(?:name)|(?:spouse)>L:' -pp
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
bash $ cat ab.json | jtc -w'<name>l:' -w'<spouse>l:' -s | jtc  -w'<(?:name)|(?:spouse)>L:' -l 
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
For the sake of an example, let's remove _array_ encapsulation from phone record, leaving only the last phone in it:
```
bash $ cat ab.json | jtc -w'<phone>l:' -w'<phone>l:[-1:]' -s | jtc -w'<phone>l:' -l
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
3. `<walk-path>` is assumed and parsed - if that fails too, an short exception message is displayed

_An attention is required when passing a `<walk-path>` argument: some walk-paths look exactly like JSON, e.g:
`[0]` - this is both a valid JSON array (made of single numeric value `0`) and a valid walk-path (addressing a first element
in an iterable), hence such argument will be treated as JSON. To pass this as a walk-path, modify it to a range-type of
walk, e.g.: `[0:1]` - that is still a valid walk-path (selecting only a first element) but is invalid JSON._

#### Destination driven insertion
The destination insertion point(s) (`-w`) controls how insertion is done:
- if a given destination insertion point (`-w`) is a single walk and non-iterable - i.e., if it's a single location - then all given
sources are attempted to get inserted into a single destination location:
```
bash $ cat ab.json | jtc -w'<children>l:' -lr
"children": [ "Olivia" ]
"children": []
"children": [ "Robert", "Lila" ]
bash $
bash $ cat ab.json | jtc -w'[name]:<Ivan>[-1][children]' -i'"Maggie"' -i'"Bruce"' | jtc -w'<children>l:' -lr
"children": [ "Olivia" ]
"children": [ "Maggie", "Bruce" ]
"children": [ "Robert", "Lila" ]
bash $
```
- if a given destination insertion point is iterable, or multiple are given, then all sources (`-i` arguments) are inserted one
by one in a circular fashion (if source runs out of JSON elements, but destination has more room to iterate, then source
is wrapped to the beginning element):
```
bash $ cat ab.json | jtc -w'<children>l:' -i'"Maggie"' -i'"Bruce"' | jtc -w'<children>l:' -lr
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
bash $ cat ab.json | jtc -w'[0][-1:][address]' -l
"address": {
   "city": "Denver",
   "postal code": 80206,
   "state": "CO",
   "street address": "6213 E Colfax Ave"
}
bash $
bash $ cat ab.json | jtc -w'[0][-1:][address]' -i'{ "PO box": null, "street address": null }' | jtc -w'[0][-1:][address]' -l
"address": {
   "PO box": null,
   "city": "Denver",
   "postal code": 80206,
   "state": "CO",
   "street address": "6213 E Colfax Ave"
}
bash $
```
\- as you can see, the `"PO box"` label was inserted, but the destination object's value in the `"street address"` was preserved

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

### Insert, Update with move
if a source argument for either `-i` or `-u` is given in the form of `<file>` or `<JSON>`, then those obviously cannot be moved.
The move semantic is only applicable when the argument is given in the form of a `<walk-path>`, then upon completing
the operation, the source elements (referred by the source walk-path) become possible to remove (purge).
This is achievable by letting specifying `-p` switch for such operations only.

Let's move `address` from the last `Directory` record into the first one:
```
bash $ cat ab.json | jtc -w'[Directory][0][address]' -u'[Directory][-1:][address]' -p
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
bash $ cat ab.json | jtc -w'<name>l:' 
"John"
"Ivan"
"Jane"
bash $
bash $ cat ab.json | jtc -w'<name>l:' -eu echo {} \| tr "[:lower:]" "[:upper:]" \; | jtc -w'<name>l:'
"JOHN"
"IVAN"
"JANE"
bash $
```
Once options `-e` and `-i`,`-u` used together, following rules must be observed:
- option `-e` must precede `-i`,`-u`
- cli sequence following option `-i`,`-u` must be terminated with escaped semicolon: `\;`
- any occurrence of `{}` will be interpolated with JSON entry being updated (or where it's inserted)
- the cli in argument do not require any additional escaping (except those which would normally be required by shell)
- if piping in the cli is required then pipe symbol itself needs to be escaped and spelled standalone: `\|`
- returned result of a shell evaluation still must be a valid JSON
- failed or empty results of the shell evaluations are ignored (JSON entry wont be updated/inserted, 
rather proceed to the next walked entry for another update attempt)

There're 2 variants of the substitution patterns:
- `{}` - will substitute JSON (pointed by `-w`) as it is
- `}{` - if substituted JSON is a _string_, then outer quotation marks are dropped, otherwise substitute as it is

### Mixed use of arguments
options `-c`, `-u`, `-i` allow two kinds of their arguments:
1. static JSONs (i.e., `<file>`, `<JSON>`)
2. walk-path (i.e., `<walk-path>`)
\- when those used together, namely a `<walk-path>` argument(s) follows either of statics  (e.g.: `jtc -u file.json -u'[Root][:]'`)
then all subsequent `<walk-path>` apply onto the first argument (here `file.json`). 
\- if `<walk-path>` arguments are given without preceding static JSON, then walk-path are applied onto the input (source) JSON

That rule is in play to facilitate a walking capability over the specified static JSONs. Though be aware: _all specified `<walk-path>`
will be processed in a consecutive order, one by one_.


## Comparing JSONs
`-c` allows comparing JSONs (or JSONs element pointed by walk-paths) - `jtc` will display JSON delta (diffs) between compared JSONs. 
Let's compare `phone` records from the first and  the second entries of the address book:
```
bash $ cat ab.json | jtc -w'[Directory][0][phone]' -c'[Directory][1][phone]' -l
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


## More Examples
### Working with templates
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
bash $ jtc -n -w'<Person>l:' -w'<Age>l:' -w'<Children>l:' -u ab.json -u'<name>l:' -u'<age>l:' -u'<children>l:' abc.json
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

