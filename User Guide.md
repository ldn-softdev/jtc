# User Guide: [`jtc`](https://github.com/ldn-softdev/jtc). Examples and Use-cases (_v.1.75, being updated_)

1. [Displaying JSON](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#displaying-json)
   * [Pretty printing (`-t`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#pretty-printing)
   * [Compact printing (`-r`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#compact-printing)
   * [Semi-compact printing (`-tNc`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#semi-compact-printing)   
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
     * [Search suffixes (`rRPdDNbnlLaoicewjstqQ`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-suffixes)
     * [Directives (`vkzfFuI`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#directives)
     * [Fail-safe and Forward-Stop directives (`<..>f`, `<..>F`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#fail-safe-and-forward-stop-directives)
     * [RE generated namespaces (`$0`, `$1`, etc)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#re-generated-namespaces)
     * [Path namespaces (`$PATH`, `$path`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#path-namespaces)
     * [Last Walk namespace (`$?`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#last-walk-namespace)
     * [Search quantifiers (`n`,`+n`,`n:n`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers)
     * [Search quantifiers with relative offset semantic (`>..<t`, `>..<l`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers-with-relative-offset-semantic)
     * [Scoped search `[..]:<..>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#scoped-search)
     * [Non-recursive search (`>..<`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#non-recursive-search)
   * [Addressing parents (`[-n]`, `[^n]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#addressing-parents)
   * [Walking multiple walk-paths](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-multiple-walk-paths)
     * [Sequential walk processing (`-n`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#sequential-walk-processing)
     * [Displaying walks with labels (`-l`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#displaying-walks-with-labels)
     * [Wrapping resulted walks to JSON array (`-j`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-resulted-walks-to-json-array)
     * [Interleaved walk processing](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interleaved-walk-processing)
     * [Aggregating walks](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#aggregating-walks)
     * [Wrapping walked entries into JSON object (`-jj`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-walked-entries-into-json-object)
     * [Extracting labeled value (`-ll`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#extracting-labeled-value)
     * [Succinct walk-path syntax (`-x`,`-y`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#succinct-walk-path-syntax)
     * [Controlling displayed walks (`-xn/N`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#controlling-displayed-walks)
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
   * [Insert, Update with move semantic (`-i`/`-u`,`-p`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-with-move-semantic)
   * [Insert, Update: argument shell evaluation (`-e`,`-i`/`-u`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation)
   * [Use of mixed arguments for `-i`, `-u`, `-c` (e.g.: `jtc -u<JSON> -u<walk-path>`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#use-of-mixed-arguments-for--i--u--c)
   * [Use of mixed arguments with `-e`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#use-of-mixed-arguments-with--e)
   * [Summary of modes of operations](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#summary-of-modes-of-operations)
4. [Comparing JSONs (`-c`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#comparing-jsons)
   * [Comparing JSON schemas](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#comparing-json-schemas)
5. [Interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation)
   * [Interpolation types (`{}` vs `{{}}`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation-types)
   * [Namespaces with interleaved walks](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespaces-with-interleaved-walks)
   * [Search quantifiers interpolation (`<..>{..}`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers-interpolation)
   * [Cross-referenced lookups](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#cross-referenced-lookups)
     * [Cross-referenced purge](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#cross-referenced-purge)
6. [Templates (`-T`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates)
   * [Multiple templates and walks](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#multiple-templates-and-walks)
   * [Stringifying JSON, Jsonizing stringified(`>{{}}<`, `>>{{}}<<`, `<{{}}>`, `<<{{}}>>`) ](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json-jsonizing-stringified)
7. [Processing multiple input JSONs](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#processing-multiple-input-jsons)
   * [Process all input JSONs (`-a`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#process-all-input-jsons)
   * [Wrap all processed JSONs (`-J`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrap-all-processed-jsons)
   * [Buffered vs Streamed read](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#buffered-vs-streamed-read)
8. [More Examples](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#more-examples)
   * [Generating CSV from JSON](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#generating-csv-from-json)
   * [Taming duplicates (`<..>q`, `<..>Q`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#taming-duplicates)
     * [Remove duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#remove-duplicates)
     * [Remove all but duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#remove-all-but-duplicates)
     * [Leave only those which have no duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#leave-only-those-which-have-no-duplicates)
     * [Leave all duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#leave-all-duplicates)
   * [Counting with `jtc`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#counting-with-jtc)

---

## Displaying JSON
### Pretty printing
If no argument given, `jtc` will expect an input JSON from the `<stdin>`, otherwise JSON is read from the file(s) pointed by the 
argumen(t). `jtc` will parse and validate input JSON and upon a successful validation will output:
```bash
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
```bash
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
Option `-r` will instruct to display JSON in a compact (single row) format:
```bash
bash $ <ab.json jtc -r
{ "Directory": [ { "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" }, "age": 25, "children": [ "Olivia" ], "name": "John", "phone": [ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ], "spouse": "Martha" }, { "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null }, { "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" } ] }
bash $
```

By default, the compact prniting view will use a single space as a spacer between all tokens, that also could be controlled if `-r` and `-t` used together, e.g. to print the above JSON w/o spacer:
```bash
bash $ <ab.json jtc -rt0
{"Directory":[{"address":{"city":"New York","postal code":10012,"state":"NY","street address":"599 Lafayette St"},"age":25,"children":["Olivia"],"name":"John","phone":[{"number":"112-555-1234","type":"mobile"},{"number":"113-123-2368","type":"mobile"}],"spouse":"Martha"},{"address":{"city":"Seattle","postal code":98104,"state":"WA","street address":"5423 Madison St"},"age":31,"children":[],"name":"Ivan","phone":[{"number":"273-923-6483","type":"home"},{"number":"223-283-0372","type":"mobile"}],"spouse":null},{"address":{"city":"Denver","postal code":80206,"state":"CO","street address":"6213 E Colfax Ave"},"age":25,"children":["Robert","Lila"],"name":"Jane","phone":[{"number":"358-303-0373","type":"office"},{"number":"333-638-0238","type":"home"}],"spouse":"Chuck"}]}
bash $ 
```


### Semi-compact printing
A semi-compact view is a middle ground between pretty and compact views. The semi-compact view is engaged with the suffix -`c` appended
to the indent value in `-t` option (e.g.: `-t5c`) . In the semi-compact view all _JSON iterables_ made of only _atomic values_ and/or
empty iterables (`[]`, `{}`) will be printed in a single line, the rest if pretty-printed, compare:
```bash
bash $ <ab.json jtc -tc
{
   "Directory": [
      {
         "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" },
         "age": 25,
         "children": [ "Olivia" ],
         "name": "John",
         "phone": [
            { "number": "112-555-1234", "type": "mobile" },
            { "number": "113-123-2368", "type": "mobile" }
         ],
         "spouse": "Martha"
      },
      {
         "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" },
         "age": 31,
         "children": [],
         "name": "Ivan",
         "phone": [
            { "number": "273-923-6483", "type": "home" },
            { "number": "223-283-0372", "type": "mobile" }
         ],
         "spouse": null
      },
      {
         "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" },
         "age": 25,
         "children": [ "Robert", "Lila" ],
         "name": "Jane",
         "phone": [
            { "number": "358-303-0373", "type": "office" },
            { "number": "333-638-0238", "type": "home" }
         ],
         "spouse": "Chuck"
      }
   ]
}
bash $ 
```


### Printing JSON size
JSON size is the total number of the JSON elements found within JSON, it could be printed using `-z`, the size appears after input JSON
is printed (starting from version 1.75b the size is printed in a JSON format):
```bash
bash $ <ab.json jtc -rz
{ "Directory": [ { "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" }, "age": 25, "children": [ "Olivia" ], "name": "John", "phone": [ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ], "spouse": "Martha" }, { "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null }, { "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" } ] }
{ "size": 56 }
bash $ 
```
if size only required (i.e., w/o printing the input JSON), then use `-zz` option:
```bash
bash $ <ab.json jtc -zz
56
bash $ 
```


### Validating JSON
When JSON is read (from a file, or from `stdin`), it get parsed and validated. If an invalid JSON is detected, a short exception
message will be displayed, e.g,:
```bash
bash $ <ab.json jtc
jtc json parsing exception (<stdin>:1214): unexpected_end_of_line
bash $ 
```
and though the message lets us knowing that there's a problem with the input JSON, it not very informative with regards whereabouts the
the problem. To visualize the spot where the problem is, as well as its locus pass a single debug option (`-d`):
```bash
bash $ <ab.json jtc -d
.display_opts(), option set[0]: -d (internally imposed: )
.read_inputs(), reading json from <stdin>
.location_(), exception locus: ...206,|            "state": "CO,|            "street address": ...
.location_(), exception spot: --------------------------------->| (offset: 1214)
jtc json parsing exception (<stdin>:1214): unexpected_end_of_line
bash $ 
```
the vertical pipe symbol `|` in the debug showing JSON replaces new lines, thus it becomes easy to spot the problem. 
The offset (`1214` in the example) is given in _unicode UTF-8_ characters from the beginning of the input/file/stream.
In that particular failure instance, `jtc` found the end of a line, while _JSON string_ `"Co,` is still open (JSON standard does not
permit multi-line strings). To fix that, the missing quotation mark to be added

### Forcing strict solidus parsing
JSON specification allows escaping solidus (`/`) optionally. By default, `jtc` is relaxed w.r.t. solidus notation - it admits
both unescaped and escaped appearances:
```bash
bash $ <<<'{ "escaped": "\/", "unescaped": "/" }' jtc
{
   "escaped": "\/",
   "unescaped": "/"
}
bash $
```
If there's a need for a strict solidus parsing, option `-q` facilitates the need. It also will throw an exception upon facing
a non-escaped notation:
```bash
bash $ <<<'{ "escaped": "\/", "unescaped": "/" }' jtc -q -d
.display_opts(), option set[0]: -q -d (internally imposed: )
.read_inputs(), reading json from <stdin>
.location_(), exception locus: { "escaped": "\/", "unescaped": "/" }|
.location_(), exception spot: --------------------------------->| (offset: 33)
jtc json parsing exception (<stdin>:33): unquoted_character
bash $ 
```

### Unquoting JSON strings
If a JSON itself (or a result from walking JSON) is a single JSON string, then sometimes there's a need to unquote it
(especially it comes handy if the string itself is an embedded JSON). `-qq` allows unquoting it, here are a few examples:
```bash
bash $ <<<'"{ \"JSON\": \"example of an embedded JSON\" }"' jtc
"{ \"JSON\": \"example of an embedded JSON\" }"
bash $ 
bash $ <<<'"{ \"JSON\": \"example of an embedded JSON\" }"' jtc -qq
{ "JSON": "example of an embedded JSON" }
bash $ 
bash $ <<<'"{ \"JSON\": \"example of an embedded JSON\" }"' jtc -qq | jtc
{
   "JSON": "example of an embedded JSON"
}
bash $
bash $ <<<'"3.14"' jtc
"3.14"
bash $
bash $ <<<'"3.14"' jtc -qq
3.14
bash $
```

When unquoting an empty _JSON string_ (`""`) the resulted blank lines are not even printed:
```bash
bash $ <<<'[null, "", true]' jtc -w[:] -qq
null
true
bash $ 
```

NOTE: _the option notation `-qq` will not engulf a single option notation `-q`, if both behaviors are required then both variants have
to be spelled (e.g. `jtc -q -qq`, or `jtc -qqq`)_  

Also, `-qq` is incompatible with `-j`, `-J` options, because of a risk of ill-formed JSON, thus, when sighted together
option `-qq` is silently ignored


### Stringifying JSON
An opposite request is to string-quote a JSON itself (e.g. if you like to embed JSON as a string into another JSON). This is
achieved with the option notation `-rr`:
```bash
bash $ <<<'[ "JSON", "example" ]' jtc
[
   "JSON",
   "example"
]
bash $
bash $ <<<'[ "JSON", "example" ]' jtc -rr
"[ \"JSON\", \"example\" ]"
bash $
```

B.t.w, both _string unquoting_ and _JSON stringification_ also could be achieved via 
[template operations](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json-jsonizing-stringified).


## Walking JSON
Whenever there's a need to select only one or multiple JSON elements, walk-paths (`-w`) tell how to do it. A walk path
(an argument of `-w` option) can be made of an arbitrary number of lexemes. Though there are only 2 types of the lexemes:
- offset lexeme (`[..]`)
- search lexeme (`<..>`, `>..<`)

### Walking with subscripts (offset lexemes)
Offsets are always enclosed into square brackets `[..]`. Selecting JSON elements always begins from the root.
Both arrays and objects can be subscripted using numerical offset, though it's best to utilize literal offsets to subscript objects.
E.g. let's select `address` of the 2nd (all the indices in the walk-path are zero-based) record in the above JSON:
```bash
bash $ <ab.json jtc -w'[Directory][1][address]'
{
   "city": "Seattle",
   "postal code": 98104,
   "state": "WA",
   "street address": "5423 Madison St"
}
bash $
```
or, equally could be done like in below example, but the former syntax is preferable (for your own good - when giving indices you'd need 
to _guess_ the index of a labeled entry, which might be prone  to mistakes):
```bash
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
if a numerical subscript index is prepended with `+`, then all the subsequent subscripted elements will be selected as well
(a.k.a _iterable_ lexeme), e.g., a following example prints all the names out of the address book, starting from the 2nd record:
```bash
bash $ <ab.json jtc -w'[Directory][+1][name]'
"Ivan"
"Jane"
bash $
```
Any numerical offset could be subscripted that way, and any number of such lexemes could appear in the walk-path, e.g.:
```bash
bash $ <ab.json jtc -w'[Directory][+0][phone][+1][number]'
"113-123-2368"
"223-283-0372"
"333-638-0238"
bash $
```
Here, all records (`[+0]`) from the `Directory` have been selected and then in every record all `phone` sub-records selected starting from
the 2nd entry (`[+1]`)

The same way object elements could be subscripted, here's an example where all address entries starting from the 2nd one are printed,
each one stating from the 3rd entry: 
```bash
bash $ <ab.json jtc -w'[Directory][+1][address][+2]'
"WA"
"5423 Madison St"
"CO"
"6213 E Colfax Ave"
bash $
```

##### Subscript slice notation
Another way to select multiple subscripts is to use a slice notation `[N:M:S]`. In that notation `N` and `M` could be either positive
or negative, while `S` must be strictly positive. Any of positions (as well as position separator `:`) could be omitted.
The first position (`N`) designates a beginning of the slice selection, the last position (`M`) designates the end of the 
slice exclusively (i.e., not including the indexed element itself)
- positive `N` (and `M`) refers to `N`th element offset from the beginning of the iterable (whether it's array or an object)
- negative `N` (and `M`) refers to the `N`th element offset from the end of the collection.
- empty (missed `N` and `M`) tells to address either from the the beginning of the collection (in the first position), or from the end
(last position) 
- `S` position indicates a step value when iterating over the selected slice, the default value is obviously `1`

Thus, multiple notations with the same semantics are possible, e.g.:
- `[:]`, `[0:]`, `[0::]` will select all the element in the collection and is equivalent of `[+0]` notation
- `[0:1]`, `[:1]`, will select only the first element and is the same as `[0]`
- `[:-1]` will select all the elements except the last one
- `[-2:]` will select last 2 elements in the collection 
- `[::2]` -will select every other element in the collection

E.g., let's print all phone numbers for the last 2 records in the `Directory`:
```bash
bash $ <ab.json jtc -w'[Directory][-2:][phone][:][number]' -l
"number": "273-923-6483"
"number": "223-283-0372"
"number": "358-303-0373"
"number": "333-638-0238"
bash $ 
```

### Searching JSON
Walk-path lexemes enclosed into `<..>` braces instruct to perform a _recursive_ search off the value under a currently selected JSON node. 
I.e., if a search lexeme appears as the first one in the walk-path, then the search will be done from the root, otherwise from the node
in JSON where a prior lexeme has stopped.

By default (if no suffix is given), a search lexeme will perform a search among _JSON string_ values only (i.e., it won't match 
_JSON numerical_ or _JSON boolean_ or _JSON null_ values). E.g., following search produces a match:
```bash
bash $ <ab.json jtc -w'<New York>'
"New York"
bash $
```
while this one doesn't have a match (the string value `New York` is found only in the first `Directory` record): 
```bash
bash $ <ab.json jtc -w'[Directory][1:]<New York>'
bash $
```

#### Searching JSON with RE
Optionally, search lexemes may accept _one-letter suffixes_: a single character following the lexeme's closing bracket.
These suffixes alter the meaning of the search, e.g. suffix `R` instruct to perform a regex search among string values:
```bash
bash $ <ab.json jtc -w'<^N>R'
"New York"
bash $
```

#### Search suffixes
This is the list of suffixes to control search behavior: 
  * `r`: default (could be omitted), fully matches _JSON string_ values (e.g.: `<CO>`, `<CO>r`)
  * `R`: the lexeme is a search RE, only _JSON string_ values searched (e.g.: `<^N+*>R`)
  * `P`: matches _any_ string values, same like `<.*>R`, just faster (e.g.: `<>P`)
  * `d`: matches _JSON numericals_ (e.g.: `<3.14>d`)
  * `D`: the lexeme is an RE, only _JSON numerical_ values searched (e.g.: `<^3\.+*>D`)
  * `N`: matches _any_ JSON numerical value, same like `<.*>D`, just faster (e.g.: `<>N`)
  * `b`: matches _JSON boolean_ values; to match the exact boolean, it must be spelled as `<true>b`, `<false>b`; when the lexeme is empty
  (`<>n`) it matches any boolean
  * `n`: matches _JSON null_ value (e.g.: `<>n`)
  * `l`: fully matches _JSON label_ (e.g.: `<address>l`)
  * `L`: the lexeme is a search RE, only _JSON labels_ searched (e.g. `<^[a-z]>L`)
  * `a`: matches any JSON atomic value, i.e., _strings_, _numerical_, _boolean_, _null_ (e.g.: `<>a`)
  * `o`: matches any JSON object `{..}` (e.g.: `<>o`)
  * `i`: matches any JSON array `[..]` (e.g.: `<>i`)
  * `c`: matches containers - either arrays or objects (e.g.: `<>c`) 
  * `e`: matches end-nodes only, which is either atomic values, or an empty iterables `[]`, `{}` (e.g.: `<>e`)
  * `w`: matches any JSON value (wide range match): atomic values, objects, arrays (e.g. `<>w`)
  * `j`: matches a JSON value; the lexeme can be either a valid JSON (e.g.: `<[]>j` - finds an empty JSON array), or a
  [template](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates)
  resulting in a valid JSON after 
  [interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation)
  (e.g.: `<"{str}">j` - finds a _JSON string_ whose value is in
  [namespace](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespaces)
  `str`)
  * `s`: matches a JSON value previously stored in the namespace by directives `<..>k`, `<..>v` (e.g.: `<Val>v ... <Val>s`)
  * `t`: matches a tag (label/index) previously stored in the namespace by directives `<..>k`, `<..>v` (e.g. `<Lbl>k ... <Lbl>t`)  
  * `q`: matches only original JSON values, i.e. selects non-duplicated values (e.g.: `<>q`)  
  * `Q`: matches only repetitive (duplicating) JSON values (e.g.: `<>Q`)
  * `g`: matches all JSON values in the ascending order (e.g.: `<>g`)
  * `G`: matches all JSON values in the descending order (e.g.: `<>G`)


Some search lexemes (and
[directives](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#directives))
require their content is set and be **non-empty** (`R`,`d`,`D`,`L`,`j`,`s`,`t`,`v`,`z`,`u`,`I`,`Z`,`W`), otherwise an exception 
_`walk_empty_lexeme`_ will be thrown

A few of search lexemes might be left empty, but then they cary a semantic of an **empty search** (`r`,`l`,`b`):
  - `<>` (same as `<>r`) - will match an empty _JSON string_
  - `<>l` - will match an entry with the empty _JSON label_
  - `<>b` will match any boolean value.

The rest of the lexemes (search and directives - `P`,`N`,`n`,`a`,`o`,`i`,`c`,`e`,`w`,`q`,`Q`,`g`,`G`,`k`,`f`,`F`) also might be left
empty. However, if those lexemes are non-empty, then the content points to a
[**namespace**](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespaces)
where the found value (result of a match) will be stored, e.g.: `<array>i` - upon a match will preserve found _JSON array_ in the
namespace `array`


##### Cached Search
`jtc` is super efficient searching recursively even huge JSONs structures - normally no exponential search decay will be observed
(which is very typical for such kind of operations). The decay is avoided because `jtc` builds a cache for all searches (whenever
cacheing is required, both recursive and non-recursive) and thus all subsequent matches are taken from the cache.  
Though, there couple cases when search could not be _cached_ in principle - when the search lexeme is a _dynamic_ type, i.e., when
resolution of the search is dependent on the _namespace_ value.  
Here's the list of such search types:
  * recursive `<..>s`,`<..>t`: the lexemes are using _namespaces_ when performing search and hence could not cached
  * JSON match when the lexeme is a template, e.g.: `<{ "label": {{val}} }>j`: templates typically require _namespace_ for interpolation
  and hence are also non-cacheable, though `<..>j` searches with static JSONs will be cached - e.g.: `<{"label": "val" }>j`)

\- all the above cases are exempt from cacheing and hence the exponential decay might become noticeable, so be aware when building a 
query for very large JSON structures (order of hundred thousands of nodes)


#### Directives

There are a few lexemes that look like searches, though they do not perform any matching, rather they apply certain actions
for the currently walked JSON elements, these are _directives_:

ZW

  * `v`: saves the currently walked JSON value into a namespace under the name specified by the lexeme (e.g.: `<Jsn>v`)
  * `k`: instructs to reinterpret the key (label/index) of the currently walked JSON and treat it as a value (thus a label/index
         can be updated/extracted programmatically) - e.g.: `<>k`; if the lexeme is non-empty then it saves a found key 
         (label/index) into the corresponding namespace and **cancels reinterpretation** of the label as a value (e.g.: `<Lbl>k`)
  * `z`: erases namespace pointed by the lexeme's; the lexeme must not be empty (e.g.: `<Jsn>z`)
  * `f`: fail-safe (branching): if walking **past the fail-safe** lexeme fails then, instead of progressing to the next iteration
         (a typical behavior), the walk for the lexeme immediately preceding the fail-safe will be reinstated;
         walking (of the same walk-path) may continue if `><F` directive is present (past the failing point)
  * `F`: Forward-Stop: behavior of the directive is dependent on spelling:
    * `<>F` - when the directive is reached, the currently walked path is skipped and silently proceeds to the next walk iteration
    without ending the walk;
    * `><F` - when the directive is reached, the walk successfully stops for the output processing    
  * `u`: user evaluation of the walk-path: the lexeme is the _`shell cli`_ sequence which affects walking: if a returned result of the
  shell evaluation is `0` (success) then walk continues, otherwise the walk fails; the lexeme is subjected for template
  interpolation
  * `I`: increment/multiply lexeme.
  if the namespace value pointed by a lexeme is a _JSON number_ then it's incremented by the specified offset
  (e.g. `<var>I-3` will decrement `var` by `3`), if the pointed value is not a _JSON number_ then it's ignored
   
The use of `F` directive makes only sense paired with `<>f`. Together they cover all cases of walk-paths branching:
  * ... `<>f` {if this path does not fail, then skip it} `<>F` {otherwise walk this path (starting from `<>f` point)} ...
  * ... `<>f` {if this path does not fail, then end current walk} `><F` {otherwise walk this path} ...
  * ... `<>f` {if this path does not fail, then end current walk} `><F <>F` # otherwise skip it (i.e., skip the failed path)
  * ... `<>f` {if this path does not fail, then end current walk} `><F <>f` {otherwise walk this path, end it if walked successfully} 
  `><F` {otherwise, if second branching fails, walk this one } ...
  * etc

##### Namespaces

A _namespace_ is a container(s) within `jtc`, which allows storing JSON elements programmatically while walking JSON.
Stored in the namespace values could be reused later in the same or different walk-paths and 
[interpolated](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation) in 
[templates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates) and arguments
for a shell evaluation.

##### Dynamic lookup using directives `-v` and `-k`
Say, we have a following JSON:
```
bash $ <<<'{ "item": "bread", "list":{ "milk": 0.90, "bread": 1.20 } }' jtc
{
   "item": "bread",
   "list": {
      "bread": 1.20,
      "milk": 0.90
   }
}
bash $ 
```
the ask here is to retrieve a value from `list` defined by `item` - that would require a cross-reference lookup.
Using namespaces it becomes a trivial task:
```
bash $ JSN='{ "item": "bread", "list":{ "milk": 0.90, "bread": 1.20 } }'
bash $ <<<$JSN jtc -w'[item]<itm>v[^0]<itm>t'
1.20
bash $ 
```
- `[item]<itm>v` - will retrieve a value in `item` and store it in the namespace `itm`
- `[^0]<itm>t` - will reset the point of departure back to the root, then will search for a label matching the value stored
in the namespace `itm` (which is `bread`)

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
When the directive lexeme `<>k` is given w/o a value (like shown) then no saving in the namespaces occurs.

The lexeme `<..>v` (and all others which allow _namespaces_ - `naoicewkfF`) allow setting up a custom JSON value (in lieu of
currently walked JSON) - if the lexeme's value is given in the format:  
- `<name:JSON_value>v`  
then upon walking such syntax a user's `JSON_value` will be preserved in the namespace `name`


#### Fail-safe and Forward-Stop directives
All the lexemes in the _walk-path_ are bound by logical `AND` - only if all succeed then the path is successfully walked (and printed
or regarded for a respective operation). The _fail-safe_ and _Forward-Stop_ directives make possible to introduce branching logic 
into the _walk-path_.  
Let's break it down:

Say, we want to list all `mobile` phone records (let's do it along with names of phone holders):
```
bash $ <ab.json jtc -w'[0][:][name]' -w'[0][:][phone]<mobile>[-1]' -r
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
bash $ <ab.json jtc -w'[0][:][name]' -w'[0][:][phone][0] <>f [-1]<mobile>[-1]' -r
"John"
{ "number": "112-555-1234", "type": "mobile" }
"Ivan"
{ "number": "223-283-0372", "type": "mobile" }
"Jane"
{ "number": "358-303-0373", "type": "office" }
bash $ 
```
as the path is walked, as soon `<>f` directive is faced, `jtc` _memorizes_ the currently walked path point and will _recall_ it shall
further walking fails, there:
- we resolve the first entry in the `phone` records and memorize its path location (`[phone][0] <>f`)
- then step back up and look for a `mobile` type of the record (`[-1]<mobile>`), then:
     * if it's found, we step back up (`[-1]`) again to finish walking and display the whole record
     * if not found (i.e., walking indeed fails), a fail-safe is engaged and preserved location is recalled and displayed

A _walk-path_ may contain multiple _fail-safe_, only the respective fail-safe will be engaged (more specific one and closest 
one to the failing point)

The _fail-safe_ directive, (as well as `<..>v`) may carry a value (namespace), which will be populated with the currently walked
JSON element (for later interpolation), and, as well as `<..>v`, the _fail-safe_ is also allowed setting up custom JSON values 
(when used in the format: `<namespace:JSON_value>f`)

##### Here's another example sporting _fail-safe_ using namespaces and interpolation:
Say we want to list from the address book all the record holders and indicate whether they have any children or not in 
this format 
`{ "Name has children": true/false }`

Thus, we need to build a single path, which will find the `name`, then inspect `children` record and transform it into
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
2. Now let's inspect a sibling record `children`:
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
3. so far so good, but we need to engage _fail-safe_ to facilitate the requirement to classify those records as `true` / `false`:
```
bash $ <ab.json jtc -x'[0][:][name]<person>v [-1][children]<kids:false>f[0]<kids:true>v' 
"Olivia"
[]
"Robert"
bash $ 
```
- there namespace `kids` will be populated first with JSON value `false` and will stay put shall further walking fails;
- otherwise (i.e., upon a successful walk - addressing a first child `[0]`) the namespace `kids` will be overwritten
with `true` value

4. finally, we need to interpolate preserved namespaces for our final / required output:
```
bash $ <ab.json jtc -x'[0][:][name]<person>v [-1][children]<kids:false>f[0]<kids:true>v' -T'{"{person} has children":{kids}}' -r
{ "John has children": true }
{ "Ivan has children": false }
{ "Jane has children": true }
bash $ 
```


Now, let's consider another example. Say, we have a following JSON:
```
bash $ JSN='[ { "ip": "1.1.1.1", "name": "server" }, { "ip": "1.1.1.100" }, { "ip": "1.1.1.2", "name": "printer" } ]'
bash $ <<<$JSN jtc
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
How do we list only those records which don't have `name` and skip those which do? Well, one apparent solution then would be
to walk all those entries, which do have `name` labels and purge them:
```
bash $ <<<$JSN jtc -w'<name>l:[-1]' -p
[
   {
      "ip": "1.1.1.100"
   }
]
bash $ 
```
But what if we want to walk them instead of purging (e.g., for reason of template-interpolating the entries at the output)?
The prior solution would require piping the output to the next `jtc` cli, however, it's possible to achieve the same using this 
simple query:
```
bash $ <<<$JSN jtc -w'[:]<>f[name]<>F'
{
   "ip": "1.1.1.100"
}
bash $ 
```
Without `<>F` directive, the walk would look like:
```
bash $ <<<$JSN jtc -w'[:]<>f[name]'
"server"
{
   "ip": "1.1.1.100"
}
"printer"
bash $ 
```
Thus, `<>F` skips those (successfully) matched entries, leaving only one which fails - that's the one which we need in this query 
(the record(s) which does not have `name` in it)


Now, what if in the latter example above (one with `<>F` directive) we want to process *failed* JSON further, say, to display ip only, 
w/o object itself? That is easily achievable - walking of the *failed* path continues past the `<>F` directive:
```
bash $ <<<$JSN jtc -w'[:]<>f[name]<>F [ip]'
"1.1.1.100"
bash $
```
So, the walking sequence (for the failed, second entry) would be like this:
- upon walking `[name]` lexeme, path would fail (the second record does not have one), so the path vector would be reinstated 
to the exact point when it was right at `<>f` directive (which is the second entry itself)
- then walking continues past the next `<>F` directive, which is `[ip]`


#### RE generated namespaces
RE search lexemes (`R`, `L`, `D`) also auto-populate the namespaces with following names:
- `$0` is auto-generated for an entire RE match,
- `$1` for a first RE subgroup,
- `$2` for a second RE subgroup, and so on
```
bash $ <ab.json jtc -w'<^J(.*)>R:'
"John"
"Jane"
bash $ <ab.json jtc -w'<^J(.*)>R:' -T'"j{$1}"'
"john"
"jane"
bash $ 
```
(coverage of REGEX is entirely out of scope of this document, rather refer to this external link: 
[Regular Expression](https://www.regular-expressions.info/))


#### Path namespaces
There are also couple reserved namespaces to acquire a current JSON path:
- `$PATH`: that namespace contains a _JSON array_ describing path to the currently selected element
- `$path`: this is a stringified representation of the path
Here are both demonstrated:
```
bash $ <ab.json jtc -w'<Jane>' -T'{{$PATH}}' -r
[ "Directory", 2, "name" ]
bash $ <ab.json jtc -w'<NY>' -T'{{$path}}'
"Directory_0_address_state"
bash $ 
```
to play safe with the templates, always surround them with single quotes (to dodge shell interpolation)


#### Search quantifiers
Optionally a quantifier may follow the search lexeme (if a lexeme has a suffix, then the quantifier must come after the suffix). 
Quantifiers in search lexemes allow selecting match instance (i.e., select first match, second one, etc, or a range of matches)
Quantifiers exist in the following formats:
- `n`, - a positive number - tells which instance of a match to pick. By default, a quantifier `0` is applied
(i.e., first match is selected)
- `+n` - selects all match instances starting from `n`th (zero based)
- `N:N` - slice select: the notation rules for this quantifier the same as for 
[subscript slices](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#subscript-slice-notation)
(`[N:N]`), with just one understandable caveat: `N` here cannot go negative (there's no way of knowing upfront how many
matches would be produced, so it's impossible to select a range/slice based off the last match), the rest of the notation rules apply

To illustrate the quantifiers (with suffixes), let's dump all the _JSON arrays_ in the `Directory`, except the top one:
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
\- the trailing `1:` in the walk path is the slice quantifier, which selects (prints) all the matches (we are matching
_JSON arrays_ here - `i`) starting from second one (all quantifiers and indices are zero-based)


#### Search quantifiers with relative offset semantic 
There are two search lexemes where matching non-first instance does not make sense, namely: `>..<l` and `>..<t`.
Those are 
[non-recursive searches](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#non-recursive-search)
will uniquely match label or index. Indeed, in a plain _JSON array_ or an _object_ it's possible
to address only one **single** label or index, there could not be any other, e.g. in this JSON:
```
bash $ <<<'{ "a": 1, "b":2, "c":3, "d":4, "e":6 }' jtc -r
{ "a": 1, "b": 2, "c": 3, "d": 4, "e": 6 }
bash $ 
```
there could be only one label `"b"`, thus normally trying to match a second, third, etc instance of the label `"b"` would not
make much of a sense: `<<<'{ "a": 1, "b":2, "c":3, "d":4, "e":6 }' jtc -w'>b<l2'`

Thus the semantic of quantifiers only in those searches was altered (to extend use cases) - there, quantifiers provide a
_relative offset_ meaning from a found label/index. So, for the notation like above: `'>b<l2'`, the label `"b"` will be matched and
once match found then its second (successive) neighbor value will be selected:
```
bash $ <<<'{ "a": 1, "b":2, "c":3, "d":4, "e":6 }' jtc -w'>b<l2' -l
"d": 4
bash $ 
```
Because of change in semantic, those are the only search quantifiers (they also have to be non-recursive spelling: `>..<l`, `>..<t`) 
which allow negative values. Positive quantifiers let selecting next (successive) neighbors, while negative quantifiers let
selecting preceding neighbors:
```
bash $ <<<'{ "a": 1, "b":2, "c":3, "d":4, "e":6 }' jtc -w'>b<l-1' -l
"a": 1
bash $ 
```

#### Last Walk namespace
Any last walk could be referred (during interpolation) using an auto-generated namespace `$?`. It comes handy when is required
to build up JSON 'historical' records:
```
bash $ <<<'["a","b","c"]' jtc -w[:]
"a"
"b"
"c"
bash $ <<<'["a","b","c"]' jtc -w[:] -T'[{$?}, {{}}]' -r
[ "a" ]
[ "a", "b" ]
[ "a", "b", "c" ]
```
When interpolation of $? occurs first time (i.e. there was no prior walk), or when interpolation of $? fails, then the value of this
namespace is reset to an empty string (`""`).  
The use of that variable comes handy when converting JSON to a `csv` format.


#### Scoped search 
Search lexemes perform a _recursive_ search across the entire JSON tree off the point where it's invoked (i.e., the JSON node
selected by walking all the prior lexemes). However, sometimes there's a need to limit searching scope only to the specific label.
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
Sometimes there's a need to apply a non-recursive search onto collectable JSON nodes (arrays, objects) - i.e., find a value within
immediate children of the node and do not descend recursively. The notation facilitating such search is the same one, but
angular brackets to be put inside-out, i.e.: `>..<`. To illustrate that: say, we want to find all string values in the 1st `Directory`
record containing the letter `o`. If we do this using a recursive search, then all following entries will be found:
```
bash $ <ab.json jtc -w'[Directory][0]<o>R:'
"New York"
"John"
"mobile"
"mobile"
bash $
```
To facilitate our ask (find all such entries within the immediate values of the 1st record only), apply a non-recursive search notation:
```
bash $ <ab.json jtc -w'[Directory][0]>o<R:'
"John"
bash $
```


### Addressing parents
One of the charming features of `jtc` is the ability to address parents (any ancestors up till the root) of the found JSON nodes.
Typically addressing parents would be needed after search lexemes (but can occur anywhere in the walk-path). Parents can
be addressed using notation `[-n]`. This feature allows building queries that answer quite complex queries.

Let's dump all the names from the `Directory` whose records have a `home` phone entry:
```
bash $ <ab.json jtc -w'[type]:<home>:[-3][name]'
"Ivan"
"Jane"
bash $
```
The magic which happens here (let's break down the walk-path into the lexemes) is revealed:
  1. `[type]:<home>:` - this lexeme instruct to find all (ending quantifier `:`) strings `home` scoped by label `"type"` (`[type]:` -
  attached scope), thus all such phone records values will be selected:
  ```
  bash $ <ab.json jtc -w'[type]:<home>:' -r
  "home"
  "home"
   bash $
  ```
  2. `[-3]` - starting off those found JSON elements a 3rd ancestor will be selected. Let's see a parent selection in a slow-mo,
  one by one:
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
  3. `[name]` - now we can select (subscript) `[name]` of out those selected JSON nodes


Another example: who is the parent of a child `Lila`?
```
bash $ <ab.json jtc -w'<children>l:<Lila>[-2][name]'
"Jane"
bash $
```
Explanation:
1. `<children>l:` - finds all records with the label `children`
2. `<Lila>` - in found records find string value `Lila`, and once/if found
3. `[-2][name]` - go 2 levels (parents) up from found entry `"Lila"` and then subscript/offset by label `name`


Even more complex query: who of the parents with children, have mobile numbers?
```
bash $ <ab.json jtc -w'<children>l:[0][-2][type]:<mobile>[-3][name]'
"John"
bash $
```
\-  that is the only correct answer (because Ivan has no children and Jane has no `mobile` phone)

The walk-path break down:
1. `<children>l:` - find all records by label `"children"`
2. `[0]` - try addressing first element in the found records (that'll ensure that `children` is non-empty)
3. `[-2]` - go 2 parents up for those records which survived the prior step - that'll bring us to the person's record level 
4. `[type]:<mobile>` - find recursively `mobile` string scoped by `type` (already only for those records which have children)
5. `[-3]`-  go 3 levels (parents) up (for those records which have `children` and have `mobile` types of phone records) - it'll bring
us again up to the person's record level
6. `[name]` - finally select the name

There's another way to address parents - through `[^n]` notation, compare: the following walk-path achieves exactly the same ask:
```
bash $ <ab.json jtc -w'<children>l:[0][^2][type]:<mobile>[^2][name]' -r
"John"
bash $
```
Note `[^2]` - this notation, likewise `[-n]` also selects a certain parent, however, while `[-n]` select the parent from the leaf
(i.e., from the currently selected node) `[^n]` notation does it from the root.

When `jtc` walks lexemes, internally it maintains a path of the walked steps (it's visible via debugs `-dddd`). E.g., when the first
lexeme's match found (for `<children>l:`), the internal walked steps path would look like: `root -> [Directory] -> [0] -> [children]`,
then when the next lexeme is successfully applied, the internal path becomes: `root -> [Directory] -> [0] -> [children] -> [0]`
The meaning of `[-n]` and `[^n]` notation then is easy to observe on this diagram:
```
                                                                         etc.
                                                                         [^5]
to address a parent from root: [^0]      [^1]       [^2]      [^3]       [^4]
                                |          |          |         |          |
                                v          v          v         v          v
       internally built path: root -> [Directory] -> [0] -> [children] -> [0]
                                ^          ^          ^         ^          ^
                                |          |          |         |          |
to address a parent from leaf: [-4]      [-3]       [-2]      [-1]       [-0]
                               [-5]
                               etc.                         
```


### Walking multiple walk-paths
`jtc` allows a virtually unlimited number of walk-paths (`-w`), it would be limited only by the max size of the accepted string in
your shell. Though there are a few tweaks in `jtc` which let harnessing the order of displaying resulted walks. By default `jtc`
will be displaying resulted successful walks in an _interleaved_ manner, but first, let's take a look at


#### Sequential walk processing
option `-n` ensures that all given walk-paths (`-w`) will be processed (and printed) sequentially in the order they given:
```
bash $ <ab.json jtc -w'<name>l:' -w'<number>l:' -n
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
if resulted walks have labels in the input JSON (i.e., they were inside _JSON objects_), then `-l` let dumping their labels too:
```
bash $ <ab.json jtc -w'<name>l:' -w'<number>l:' -nl
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


#### Wrapping resulted walks to JSON array
`-j` does a quite simple thing - it wraps all walked entries back into a _JSON array_, however predicated by `-l` and `-n` options
the result will vary:
- `-j` without `-l` will just arrange walked entries into a JSON array:
  ```
  bash $ <ab.json jtc -w'<name>l:' -w'<number>l:' -nj 
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
- once `-j` and `-l` given together, then entries which have labels (i.e., come from the _JSON objects_) will be wrapped into objects:
  ```
  bash $ <ab.json jtc -w'<name>l:' -w'<number>l:' -n -j -l
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
above outputs dropping the option `-n` (i.e., print walks _interleaved_):
```
bash $ <ab.json jtc -w'<name>l:' -w'<number>l:'
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
Those look interleaved, though it does not appear that they relate to each other properly: e.g.: number `"113-123-2368"`
belong to `"John"` and preferably should be displayed before `"Ivan"` and so does apply to others. `jtc` is capable of 
processing/printing relevant entries, though it needs a little hint from the walk-paths: the latter supposed to express the
relevance between themselves.

Right now both paths (`<name>l:` and `<number>l:`) do not have common base lexemes, thus it's unclear how to relate resulting walks
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
And now, applying options `-j` together with `-l` gives a lot better result:
```
bash $ <ab.json jtc -w '[Directory][:]<name>l:' -w'[Directory][:] <number>l:' -jl
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

#### Aggregating walks
the walks also could be aggregated (per label), option `-nn` facilitates the ask:
```
bash $ <ab.json jtc -w '[Directory][:]<name>l:' -w'[Directory][:] <number>l:' -jl -nn
[
   {
      "name": [
         "John",
         "Ivan",
         "Jane"
      ],
      "number": [
         "112-555-1234",
         "113-123-2368",
         "273-923-6483",
         "223-283-0372",
         "358-303-0373",
         "333-638-0238"
      ]
   }
]
bash $ 
```


#### Wrapping walked entries into JSON object
`-jj` let wrapping walked JSON elements into a _JSON object_. All the elements in JSON object must have labels, thus any walked elements
which do not have labels (i.e., elements in _JSON array_ and root) will be ignored.

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
because they are enlisted in _JSON array_ and therefore have no attached labels (and hence ignored).

Another point to note: the values with the clashing labels will be reassembled into a _JSON array_


#### Extracting labeled value
Sometimes, when displaying outputs wrapped into array or object, it's desirable to extract the the labeled value from the object.
This become especially useful when dealing with templates. Let's consider a following exercise:

Say, the ask here is to extract names of all the people from `ab.json` and group them with newly crafted record indicating if a person
has children or not, like this:
```
[
   {
       "name": "John",
       "has children": "yes"
   },
   ...
]
```

1\. Extracting names is easy:
```
bash $ <ab.json jtc -w'[0][:][name]'
"John"
"Ivan"
"Jane"
bash $ 
```

2\. Crafting a new record would require knowing 
[templates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates)
and
[namespace interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation)
, for now let's just construct a walk which create required namespace:
```
bash $ <ab.json jtc -w'[0][:][name]' -w'[0][:][children] <C:"no">f[0]<C:"yes">v'
"John"
"Olivia"
"Ivan"
[]
"Jane"
"Robert"
bash $ 
```
\- the second walk above features a couple concepts:
- [branching](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#fail-safe-and-forward-stop-directives)
(`<..>f`) _fail-safe_ lexeme: ensures that the walk is reinstated at the placement of the lexeme if/once the 
subsequent walk fails
- [namespaces](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespaces)
(`<C:"no">f`, `<C:"yes">v`): both lexemes setup the _namespace_ `C`, initially to value `"no"` then to value `"yes"`; 
the latter value will override the former only if walking `[0]` was successful (i.e., if the person indeed has at least one child,
b/c if array `children` were empty, that walk would fail)

3\. by now, each time when second walk finishes iteration, the namespace `C` should be correctly set to the respective values reflecting
if a person has children or not, but to see that, we'd need to interpolate that namespace using a template:
```
bash $ <ab.json jtc -w'[0][:][name]' -w'[0][:][children] <C:"no">f[0]<C:"yes">v' -TT -T'{"has children": "{C}"}'
"John"
{
   "has children": "yes"
}
"Ivan"
{
   "has children": "no"
}
"Jane"
{
   "has children": "yes"
}
bash $ 
```
4. okay, we're getting closer, but now we want to display all records with labels:
```
bash $ <ab.json jtc -w'[0][:][name]' -w'[0][:][children] <C:"no">f[0]<C:"yes">v' -TT -T'{"has children": "{C}"}' -l
"name": "John"
jtc jnode exception: label_accessed_not_via_iterator
bash $ 
```
Bummer! The exception (rightfully) occurs here because trying to find an outer label of an interpolated JSON 
`{ "has children": "yes" }` fails - indeed it's a standalone JSON, and root does not have any label attached - hence the exception.
In the situations like this, we'd rather want to reach out inside the object for the labeled value rather than find an outer label.
The option `-ll` facilitates that need:
```
bash $ <ab.json jtc -w'[0][:][name]' -w'[0][:][children] <C:"no">f[0]<C:"yes">v' -TT -T'{"has children": "{C}"}' -llj
[
   {
      "has children": "yes",
      "name": "John"
   },
   {
      "has children": "no",
      "name": "Ivan"
   },
   {
      "has children": "yes",
      "name": "Jane"
   }
]
bash $ 
```
Finally - what's `-TT` in there? It's a dummy template (one which surely fails). It needed because if it wasn't here, then 
a single template would apply to both walks (and we don't want our template to apply onto the first walk). So, we'd need to provide 
a dummy one so that each 
[template would relate to own walk](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#multiple-templates-and-interleaved-walks)
. If templates fails (and `-TT` surely does) then no interpolation applied and walked iteration result is used as it is.


#### Succinct walk-path syntax
If you look at the prior example
([Interleaved walk processing](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interleaved-walk-processing))
, you may notice that a common part of both walk-paths (`[Directory][:]`) we had to give twice. There's a way to express it in more 
succinct syntax: options `-x` and `-y` allows rearrange walk-paths so that `-x` takes an initial common part of the walk-path, 
whereas `-y` would take care of the individuals trailing pars. Thus the same example cold have been written like this:
```
bash $ <ab.json jtc -x'[Directory][:]' -y'<name>l:' -y'<number>l:'
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
\- each occurrence of `-x` will be reconciled with all subsequent `-y` (until next `-x` is faced). Options `-x`, `-y` is merely
a syntactical sugar and do not apply any walk-path parsing or validation, instead they just result in the respective `-w` options 
creations (internally), then the latter get processed. Thus, it's even possible to write it with what it seems a broken syntax at first:
```
bash $ <ab.json jtc -x'[Directory][:' -y']<name>l:' -y']<number>l:'
...
```
However, if a reinstatement of the options results in a valid walk-path - that's all what matters.

It's possible to combine both syntaxes (i.e., `-w` with `-x` and `-y`), however, given that the processing of `-x` and `-y`
internally reinstates respective options `-w`, the former will be appended after any of given `-w` options (which will affect the 
order of processing/outputting) even though the order of their appearance is different:
```
bash $ <ab.json jtc -x'[Directory][:]' -y'<name>l:' -y'<number>l:' -w '<children>l:' -rnl
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
- here `children` walked first, because `name` and `number` walks undergo reconciliation (internally) and inserted *after* all options 

#### Controlling displayed walks
By default all walks (`-w`) will be displayed (unless `jtc` carries any of modification operations like insert/update/swap/purge, then
the entire JSON will be displayed). However, there's a way to control which ones will be displayed - option `-x` is overloaded to provide
such capability.

If argument of option `-x` is given in any of notations `-xn`, `-xn/N`, `-x/N` - where `n` and `N` are numbers, then it controls
a frequency of the displayed walks (and does not represent a common portion of a walk-path).  
The first number `n` in that notation tells to display every `n`th walk. if `n` is `0` it tells to display `N`th walk once (and in such
case `0` - a default value - can be omitted resulting in the syntax `-x/N`)
The second (optional) number `N` tells to begin displaying walks starting from `N`th one (`N` is an index and thus is zero based, default
value is `0`).

Both `n` and `N` are generally positive numbers, though there's a special notation `-x/-1` - in such case _the last walk_ is ensured 
to be displayed

Say, we want to display _every 4th walk_ of the below JSON:
```
bash $ <<<'[1,2,3,4,5,6,7,8,9]' jtc -w[:]
1
2
3
4
5
6
7
8
9
bash $ 
```
One way to achieve that would be to use templates (the trick is shown in
[Multiple templates and walks](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#multiple-templates-and-walks)
section), but it's quite impractical. Much easier is to use `-x` option here:
```
bash $ <<<'[1,2,3,4,5,6,7,8,9]' jtc -w[:] -x4
4
8
bash $ 
 ```
To display _every 4th walk starting from 3rd one_, use this notation:
```
bash $ <<<'[1,2,3,4,5,6,7,8,9]' jtc -w[:] -x4/2
3
7
bash $ 
```
\- reminder: the second number in the option is an index and thus is zero based.  
Lets add to the output the very first walk and the last one:
```
bash $ <<<'[1,2,3,4,5,6,7,8,9]' jtc -w[:] -x4/2 -x/0 -x/-1
1
3
7
9
bash $ 
```


## Modifying JSON
there are a few options which let modifying input JSON:
- `-i` - insert (copy-insert, copy-merge, move) new elements to JSON
- `-u` - update (rewrite, rewrite-merge, move) elements to JSON
- `-s` - swap around pair(s) of JSON elements
- `-p` - purge (remove) elements from JSON

Typically, those options are mutually exclusive and if sighted together, only one type of operations will be executed (the above list
is given in the priority order of operation selection). However, there is a certain combination of options `-i`,`-u` and `-p`, which
facilitates _move_ semantic, those cases reviewed in the respective chapters.

Each of options requires one or multiple `-w` to be given to operate on. Options `-i` and `-u` require an argument, which comes in
different flavors, one of them is the `walk-path` per-se.

`jtc` will execute any of operations only once, if multiple operations required, then `jtc` needs to be invoked in a successive order
(e.g., daisy-chained through the pipe `|`)


### In-place JSON modification 
By default `jtc` will expect input from `stdin`. If the standalone argument `json_file` is given, then `jtc` will read
input from the file (ignoring `stdin`), see below:
```
bash $ cat file.json 
[
   "JSON",
   "in",
   "file"
]
bash $ 
bash $ <<<'[ "<stdin>", "JSON" ]' jtc
[
   "<stdin>",
   "JSON"
]
bash $ 
bash $ <<<'[ "<stdin>", "JSON" ]' jtc file.json
[
   "JSON",
   "in",
   "file"
]
bash $
```
If option `-f` is given (together with a _single_ `json_file` argument) then `jtc` will apply (redirect) its output of the operations
into the file (instead of `stdout`):
```
bash $ <<<'[ "<stdin>", "JSON" ]' jtc -f file.json
bash $ cat file.json 
[
   "JSON",
   "in",
   "file"
]
bash $ 
```
In the above example, JSON is read from `file.json` and output back into the file (`stdin` input is ignored).

The bare hyphen (`-`) overrides a file input and ensured that the input is read from the `stdin`:
```
bash $ <<<'[ "input", "JSON" ]' jtc -f - file.json
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
bash $ <<<$(<ab.json jtc -w'[type]:<home>:[-1]' -w'[type]:<office>:[-1]' -p) jtc -w'<phone>l:' -l
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
bash $ <ab.json jtc -x[type]: -y'<home>:[-1]' -y'<office>:[-1]' -p
````
or, using even a single walk-path:
````
bash $ <ab.json jtc -w'[type]:<(?:home)|(?:office)>R: [-1]' -p
````


Another use-case exist: remove all the JSON elements _except_ walked ones (while preserving original JSON structure) - that's
the feat for plural option notation: `-pp`. Let's drop all entries (in all records) but the `name` and `spouse`:
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
`-s` requires walk-paths (`-w`) to be given in pairs. Paired walk-path will be walked concurrently (thus, ensure they are consistent)
and resulted JSON elements will be swapped around.

E.g., here's an example of swapping around `name` and `spouse` for all records on the address book:
```
bash $ <<<$(<ab.json jtc -w'<name>l:' -w'<spouse>l:' -s) jtc -w'<(?:name)|(?:spouse)>L:' -l
"name": "Martha"
"spouse": "John"
"name": null
"spouse": "Ivan"
"name": "Chuck"
"spouse": "Jane"
bash $
```
_\- for the sake of example brevity, swapped elements only sorted out_


Possibly, a more frequent use-case for `-s` is when it's required to remove some extra/redundant nestedness in a JSON structure. 
For the sake of example, let's remove _array_ encapsulation from phone records, leaving only the last phone in it:
```
bash $ <<<$(<ab.json jtc -w'<phone>l:' -w'<phone>l:[-1:]' -s) jtc -w'<phone>l:' -l
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

Finally, using `-s` more than one pair of walks (-w) could be swapped out. In fact, as many *pairs* of walks given will be swapped
(predicated walks did not become invalid during prior walk pair swap operations)


### Insert operations
when either of insert (`-i`) of update (`-u`) operation is carried, there could exist 2 types of walks:
 - one facilitating insert/update points, a.k.a destination walks
 - one facilitating points (elements) being inserted/updated, a.k.a. source walks 

for insert (`-i`) operations, the destination points of insertion are given using `-w` option(s) while the argument under `-i`
itself is the source of the insertion (multiple `-i` options could be given). The source of insertion must _always_ be
a valid JSON.

Insert operations never result in overwriting destination JSON elements (though the destination could be extended).
There are 3 different flavors of insertion arguments:
- `-i <file>` - JSON being inserted is read from the file
- `-i <JSON>` - the argument itself a JSON string
- `-i <walk-path>` - the argument is a walk-path in the input JSON
all 3 flavors of of insert arguments could be mixed and used together (with some limitation, covered in 
[use of mixed arguments](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#use-of-mixed-arguments-for--i--u--c) section)
)  
If option arguments types are **not** mixed, then for the first two cases (`<file>` and `<JSON>`) source of the operation are external
to the input JSON. In the latter case (`<walk-path>`), both destination (`-w`) and source walk the same input JSON.  



How does `jtc` know which argument is supplied? The disambiguation path is like this:
1. initially a `<file>` argument is assumed and attempted to be open/read, if that fails (i.e., file not found), then
2. `<JSON>` string is assumed and attempted to be parsed. If JSON parsing fails, then
3. `<walk-path>` is assumed and parsed - if that fails too, a short exception message is displayed (`walk_expect_lexeme`)

_Attention is required when passing a `<walk-path>` type argument: some walk-paths look exactly like JSON, e.g:
`[0]` - this is both a valid JSON array (made of a single numeric value `0`) and a valid walk-path (addressing the first element
in an iterable), hence such argument will be treated as JSON.  
To pass it as a walk-path, modify it to a range-type of walk, e.g.: `[0:1]` - that is still a valid walk-path (selecting only the
first element) but is invalid JSON.  
Alternatively, add a trailing space at the end of the walk-lexeme: `[0] ` - then the argument will be treated as a walk-path (in 
`-i`, `-u` arguments the `<JSON>` argument is expected to have no trailing white spaces)_


#### Destination driven insertion
The destination insertion point(s) (`-w`) controls how insertion is done:
- if a given destination insertion point (`-w`) is a single walk and non-iterable - i.e., if it's a single point location - then 
all the supplied sources are attempted to get inserted into a single destination location:
```
bash $ <ab.json jtc -w'<children>l:' -lr
"children": [ "Olivia" ]
"children": []
"children": [ "Robert", "Lila" ]
bash $
bash $ <<<$(<ab.json jtc -w'[name]:<Ivan>[-1][children]' -i'"Maggie"' -i'"Bruce"') jtc -w'<children>l:' -lr
"children": [ "Olivia" ]
"children": [ "Maggie", "Bruce" ]
"children": [ "Robert", "Lila" ]
bash $
```
- if a given destination insertion point is iterable or multiple are given, then all sources (`-i` arguments) are inserted one
by one in a circular fashion (if source runs out of JSON elements, but destination has more room to iterate, then source
is wrapped to the beginning element):
```
bash $ <<<$(<ab.json jtc -w'<children>l:' -i'"Maggie"' -i'"Bruce"') jtc -w'<children>l:' -lr
"children": [ "Olivia", "Maggie" ]
"children": [ "Bruce" ]
"children": [ "Robert", "Lila", "Maggie" ]
bash $
```

#### Inserting objects into objects
while insertion into arrays is obvious (well, so far), insertion into object requires clarification:
- objects merged always recursively
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
bash $ <<<$(<ab.json jtc -w'[0][-1:][address]' -i'{ "PO box": null, "street address": null }') jtc -w'[0][-1:][address]' -l
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
when pointed to by the `-i <walk-path>`, i.e., they are JSON elements in objects, i.e., with labels_

as follows from the table:
- insertion cannot occur into the atomic JSON elements
- when inserting into array, whole JSON element is getting inserted (no array expansion occurs)
- labeled elements inserted as JSON objects
- when inserting objects into object, upon label clashing the destination's label is preserved (source's ignored)

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
- merging option allows insertion into the atomic values, but it gets converted into an _JSON array_ first
- arrays are merged now
- clashing labels (for merged objects/object members) are also converted into arrays (if not yet) and merged 


### Update operations
update (`-u`) is similar to insert operation, but unlike _insert_, it rewrites the destination JSON element. Though both operations
share the same qualities:
- both are [destination driven operations](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#destination-driven-insertion)
- both merge _JSON objects_ recursively
- both support merging (`-m`) semantic
- both support _move_ (`-p`) semantic (
[covered later](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-with-move)
)
- both support _shell evaluation_ (`-e`) of argument 
([covered later](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluaton)
)

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
using update operation (insert into labels is not possible even semantically). however, that only applies if `<>k` lexeme is the
last lexeme in the walk-path and if it's empty.

As the an exercise, let's capitalize all the labels within all `address`'es in `ab.json`:
```
bash $ <<<$(<ab.json jtc -w'<address>l:[:]<>k' -eu '<<<{{}}' tr '[:lower:]' '[:upper:]' \;) jtc -w'<address>l:' -rl
"address": { "CITY": "New York", "POSTAL CODE": 10012, "STATE": "NY", "STREET ADDRESS": "599 Lafayette St" }
"address": { "CITY": "Seattle", "POSTAL CODE": 98104, "STATE": "WA", "STREET ADDRESS": "5423 Madison St" }
"address": { "CITY": "Denver", "POSTAL CODE": 80206, "STATE": "CO", "STREET ADDRESS": "6213 E Colfax Ave" }
bash $ 
```

NOTE: _mind the caveat - destination walk-path may become invalid (namely, when altering labels of the nested elements after
the parent's label has been altered), in such case the update operation won't be applied due to invalidated destination_:
```
bash $ <ab.json jtc -x[Directory][0][address] -y'<>k' -y'[:]<>k'
"address"
"city"
"postal code"
"state"
"street address"
bash $ 
bash $ <<<$(<ab.json jtc -x[Directory][0][address] -y'<>k' -y'[:]<>k' -eu '<<<{{}}' tr '[:lower:]' '[:upper:]' \;) jtc -w'[Directory][0]'
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

to achieve what's intended, first the most inner labels have to be walked/processed and then the outers:
```
bash $ <<<$(<ab.json jtc -x[Directory][0][address] -y'[:]<>k' -y'<>k' -eu '<<<{{}}' tr '[:lower:]' '[:upper:]' \;) jtc -w'[Directory][0]'
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

**The above caveat is applicable only when label is being updated using _cli evaluation_.**  
When labels updated without _cli evaluation_ then recursive update is not a problem:
```
bash $ <<<$(<ab.json jtc -x[Directory][0][address] -y'<L>k<>k' -y'[:]<L>k<>k' -u0 -T'"NEW-{L}"') jtc -w'[Directory][0]'
{
   "NEW-address": {
      "NEW-city": "New York",
      "NEW-postal code": 10012,
      "NEW-state": "NY",
      "NEW-street address": "599 Lafayette St"
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
\- double lexeme notation `<L>k<>k` is required because `<L>k` will only memorize the label in the namespace `L` but will not trigger
label re-interpretation like a value, while second lexeme (`<>k`) - does.



### Insert, Update with move semantic 
if a source argument for either `-i` or `-u` is given in the form of `<file>` or `<JSON>`, then those obviously cannot be moved.
The move semantic is only applicable when the argument is given only in the form of a `<walk-path>`, then upon completing
the operation, the source elements (referred by the source walk-path) become possible to remove (purge).
This is achievable by specifying option `-p`.

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
An argument for _insert_ and _update_ operations (`-i`, `-u`) optionally may undergo a shell evaluation (predicated by preceding `-e`). 
E.g., let's capitalize all the `name` entries in the address book:
```
bash $ <ab.json jtc -w'<name>l:' 
"John"
"Ivan"
"Jane"
bash $
bash $ <<<$(<ab.json jtc -w'<name>l:' -eu '<<<{{}}' tr "[:lower:]" "[:upper:]" \;) jtc -w'<name>l:'
"JOHN"
"IVAN"
"JANE"
bash $
```
Once options `-e` and `-u`(`-i`) are used together, following rules must be observed:
- option `-e` must precede first occurrence of `-i`/`-u`
- cli arguments sequence following option `-i`/`-u` must be terminated with the escaped semicolon: `\;`
- the cli is also subjected for namespace interpolation before it gets shell evaluated
- the cli in the argument do not require any additional escaping (except those which would normally be required by bash shell)
- if piping in the cli is required then pipe symbol itself has to be escaped and spelled standalone: ` \| `
- returned result of a shell evaluation must be either a valid JSON, or non-empty and non-error (in the latter case it's
promoted to a _JSON string_ value)
- failed (those returning non-zero exit code) or empty results of the shell evaluations are ignored
(then JSON entry wont be updated/inserted, rather proceed to the next walked entry for another/next update attempt)
- templates (`-T`) are ignored (unused) - template-interpolation already occurs during the _cli evaluation_

if shell cli does not deliver expected result for some reason, it's easy to see why with `-dd` option, e.g, say, we want to 
truncate all kid's names to 3 letters only (just for fun):
```
bash $ <<<$(<ab.json jtc -w'<children>l:[:]' -j) jtc -w[:] -eu <<<{} sed -E 's/(...).*/\1/' \; -dd
.read_inputs(), reading json from <stdin>
..demux_opt(), option: '-u', hits: 3
..ready_params_(), total jsons: 0, ordinal position of option -u walks: [], jit source: src_input
..walk(), walk string: '[:]'
..walk(), dump of completed lexemes:
..walk(), [0]: WalkStep.. search_type():"numeric_offset", lexeme:"[:]", ws_type():"range_walk", ofst():"0", range():"[:]", label():"N/A", json():"N/A"
..walk(), initial walk requires iteration
..walk_interleaved_(), multi-walk: false
..walk_interleaved_(), interleaved walk-path instances (-w: 1):
..walk_interleaved_(), instance: 0, iterators: 0
.write_json(), outputting json to <stdout>
{}
bash $ 
```
\- It did not work, because `jtc` received at the input only just this `{}`. Obviously `<<<{}` was interpolated by shell -  
`jtc` got that, thus, we have to quote it:
```
bash $ <ab.jsonbash $ <<<$(<ab.json jtc -w'<children>l:[:]' -j) jtc -w[:] -eu '<<<{}' sed -E 's/(...).*/\1/' \; -dd
.read_inputs(), reading json from <stdin>
..demux_opt(), option: '-u', hits: 4
..ready_params_(), total jsons: 0, ordinal position of option -u walks: [], jit source: src_input
..walk(), walk string: '[:]'
..walk(), dump of completed lexemes:
..walk(), [0]: WalkStep.. search_type():"numeric_offset", lexeme:"[:]", ws_type():"range_walk", ofst():"0", range():"[:]", label():"N/A", json():"N/A"
..walk(), initial walk: successful match
..walk_interleaved_(), multi-walk: true
..walk_interleaved_(), interleaved walk-path instances (-w: 1):
..walk_interleaved_(), instance: 0, iterators: 3
..reconcile_ui_(), interpolated & quoted string: '<<<Olivia sed \-E s\/(...).*\/\1\/'
..system(), executing cmd '<<<Olivia sed \-E s\/(...).*\/\1\/'
sh: -c: line 0: syntax error near unexpected token `('
sh: -c: line 0: `<<<Olivia sed \-E s\/(...).*\/\1\/'
..system(), return 512: ''
error: shell returned error (512)
..reconcile_ui_(), interpolated & quoted string: '<<<Robert sed \-E s\/(...).*\/\1\/'
..system(), executing cmd '<<<Robert sed \-E s\/(...).*\/\1\/'
sh: -c: line 0: syntax error near unexpected token `('
sh: -c: line 0: `<<<Robert sed \-E s\/(...).*\/\1\/'
..system(), return 512: ''
error: shell returned error (512)
..reconcile_ui_(), interpolated & quoted string: '<<<Lila sed \-E s\/(...).*\/\1\/'
..system(), executing cmd '<<<Lila sed \-E s\/(...).*\/\1\/'
sh: -c: line 0: syntax error near unexpected token `('
sh: -c: line 0: `<<<Lila sed \-E s\/(...).*\/\1\/'
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
\- Now it did not work, because `jtc` received `sed`'s argument without single quotations (again those have been eaten by bash 
before passing to `jtc` -  something `jtc` is unable to control - shell arguments evaluation of its own arguments). Thus, 
let's double quote it now:
```
bash $ <<<$(<ab.json jtc -w'<children>l:[:]' -j) jtc -w[:] -eu '<<<{}' sed -E "'s/(...).*/\1/'" \; -dd
.read_inputs(), reading json from <stdin>
..demux_opt(), option: '-u', hits: 4
..ready_params_(), total jsons: 0, ordinal position of option -u walks: [], jit source: src_input
..walk(), walk string: '[:]'
..walk(), dump of completed lexemes:
..walk(), [0]: WalkStep.. search_type():"numeric_offset", lexeme:"[:]", ws_type():"range_walk", ofst():"0", range():"[:]", label():"N/A", json():"N/A"
..walk(), initial walk: successful match
..walk_interleaved_(), multi-walk: true
..walk_interleaved_(), interleaved walk-path instances (-w: 1):
..walk_interleaved_(), instance: 0, iterators: 3
..reconcile_ui_(), interpolated & quoted string: '<<<Olivia sed \-E 's/(...).*/\1/''
..system(), executing cmd '<<<Olivia sed \-E 's/(...).*/\1/''
..system(), return 0: 'Oli
'
..walk(), walk string: ''
..walk(), dump of completed lexemes:
..walk(), invalidated search cache
..walk(), initial walk: successful match
..reconcile_ui_(), interpolated & quoted string: '<<<Robert sed \-E 's/(...).*/\1/''
..system(), executing cmd '<<<Robert sed \-E 's/(...).*/\1/''
..system(), return 0: 'Rob
'
..walk(), walk string: ''
..walk(), dump of completed lexemes:
..walk(), invalidated search cache
..walk(), initial walk: successful match
..reconcile_ui_(), interpolated & quoted string: '<<<Lila sed \-E 's/(...).*/\1/''
..system(), executing cmd '<<<Lila sed \-E 's/(...).*/\1/''
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

Actually, the whole _cli_ could have been coded like this:
```
bash $ <<<$(<ab.json jtc -w'<children>l:[:]' -j) jtc -w[:] -eu '<<<{} sed -E "s/(...).*/\1/";' 
[
   "Oli",
   "Rob",
   "Lil"
]
bash $ 
```
Of course, the above way of solving the ask is entirely academic - it's always best to avoid using _shell cli_ evaluation and 
default to it only as a last resort.  
A much better (efficient) way achieving the same ask would be to use RE and template interpolation:
```
bash $ <<<$(<ab.json jtc -w'<children>l:[:]' -j) jtc -w'<(...)>R:' -T'"{$1}"' -j
[
   "Oli",
   "Rob",
   "Lil"
]
bash $ 
```


### Use of mixed arguments for `-i`, `-u`, `-c` 
options `-c`, `-u`, `-i` allow two *kinds* of their arguments:
1. static JSONs (i.e., `<file>`, `<JSON>`)
2. walk-path (i.e., `<walk-path>`)

\- when those used together, namely a `<walk-path>` argument(s) follows either of statics, e.g.: 
```
jtc -u file.json -u'[Root][:]'`
```
then all `<walk-path>` arguments (here `[Root][:]`) apply onto the static argument (here `file.json`).
\- When both kinds of arguments are used together, then only one (the first) static JSON argument is accepted, while 
multiple walk-path may be given
\- if `<walk-path>` arguments are given without preceding static JSON, then walk-path are applied onto the input (source) JSON

That rule is in play to facilitate a walking capability over the specified static JSONs. Though be aware: in any case _all specified 
`<walk-path>` arguments will be processed in a consecutive order, one by one (i.e., interleaving never occurs)_.

(Also, see 
[operations with cross referenced lookups](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#cross-referenced-lookups)
)


### Use of mixed arguments with `-e`
options `-u`, `-i` when used together with `-e` also allow specifying multiple instances of the option usage:
1. first option occurrence must prove a shell cli line, terminated with `;`
2. all the subsequent option usages must provide `<walk-path>` type of argument, which let specifying source(s) of interpolation.
However, in the case if mixed option arguments usage is detected (in presence of `-e`), then the semantic of the `jtc` arguments would
be like this (e.g., for option `-u`):  
`jtc -w'<dst>' -eu <shell cli ...> \; -u'<src>'`  
Here, both `<dst>` and `<src>` walks the same input JSON. _Shell cli_ evaluation / interpolation occurs from walking `<src>`
That way it's possible to decouple source(s) (of interpolation) from the destination(s): all trailing (subsequent) arguments of `-u`
will be used in every shell evaluation (interpolating respective JSON elements), while arguments pointed by (all) `-w` option(s)
will point where returned/evaluated resulting JSONs should be placed.

The described argument behavior facilitates transformation of a JSON when a source location of transformation is not the same as
a destination

Hopefully this example will clarify:
- say (just for the sake of example), we want to add to every record's `children` the `name` of the person, but not just - we
want to add it in all capitals (i.e., transform the record).
```
bash $ <<<$(<ab.json jtc -ei '<<<{{}}' tr '[:lower:]' '[:upper:]' \; -i'<name>l:' -w'<children>l:') jtc -lrw'<name>l:' -w'<children>l:'
"name": "John"
"children": [ "Olivia", "JOHN" ]
"name": "Ivan"
"children": [ "IVAN" ]
"name": "Jane"
"children": [ "Robert", "Lila", "JANE" ]
bash $ 
```
- there, the source(s) of shell interpolation were `name` records (provided with `-i'<name>l:'`), while the destination were `children`
(given with `-w'<children>l:'`)

In case if only a single option instance (`-eu`/`-ei`) is used, then both the source (of interpolation) and the destination
(of operation) would be provided with `-w` option argument


### Summary of modes of operations
`jtc` supports multiple update (and insertion) modes, at first it's easy to get confused, so let's recap here all the options:

##### 1. _Update JSON from other locations of the same JSON:_  
`<file.json jtc -w<dst_wlk> -u<src_wlk>`  
\- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from `src_wlk` of the same file (JSON) 

##### 2. _Update JSON with a static JSON (either from file, or a spelled literally):_
`<file.json jtc -w<dst_wlk> -u<static_json>`  
\- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from `static_json` (a file, or a literal JSON)

##### 3. _Update JSON from some locations of a static JSON (either from file, or a spelled literally):_
`<file.json jtc -w<dst_wlk> -u<static_json> -u<src_wlk>`  
\- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from `src_wlk` walked `static_json`

##### 4. _Update JSON with the transformed JSON elements via shell cli:_
`<file.json jtc -w<dst_wlk> -eu <cli> \;`  
\- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from shell-evaluation of `cli`. `cli` here is subjected for
interpolation from namespaces and/or JSON elements pointed by `dst_wlk` itself.

##### 5. _Update JSON from some locations of the transformed JSON via shell cli:_
`<file.json jtc -w<dst_wlk> -e -u <cli> \; -u<src_wlk>`  
\- destination(s) (in `file.json` pointed by `dst_wlk`) is updated from shell-evaluated `cli`. `cli` here is subjected for 
interpolation from namespaces and/or JSON elements pointed by `src_wlk` walking `file.json`.

_NOTE: if destination walk (`-w`) is pointing to the root of JSON then it can be entirely dismissed (in any of the above
examples) - root is always assumed_

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
bash $ <<<'123' jtc -c'123' -l
"json_1": {}
"json_2": {}
bash $ echo $?
0
bash $
```
Otherwise (JSONs are different) a non-zero code is returned:
```
bash $ <<<'[1,2,3]' jtc -c'[2,3]' -lr
"json_1": [ 1, 2, 3 ]
"json_2": [ 2, 3 ]
bash $ echo $?
4
bash $
```
If multiple pairs of JSONs compared, zero code is returned only when all compared JSON pairs are equal. 


### Comparing JSON schemas
JSON schema essentially is a JSON structure (JSON containers, labels, indices) without leaf data. I.e., two JSONs may have
different contents (leaf data), while their structures could be the same (though the statement is rather loose - JSON schema does
validate types of the leaf data as well).

E.g., if we add/insert a child into `Ivan`'s record, then the record would be different from the original:
```
bash $ <<<$(<ab.json jtc -w'<Ivan>[-1] [children]' -i'"Norma"') jtc -w'<Ivan>[-1]' -c'ab.json' -c'<Ivan>[-1]' -l
"json_1": {
   "children": [
      "Norma"
   ]
}
"json_2": {}
bash $ 
```

However, their schemas would be the same. To compare schemas of two JSONs (loosely, with applied exemption on checking leaves data types),
label directive `<>k` used together with `<>c` search suffix come handy:
```
bash $ <<<$(<ab.json jtc -w'<Ivan>[-1] [children]' -i'"Norma"') jtc -w'<Ivan>[-1]<>c:<>k' -c'ab.json' -c'<Ivan>[-1]<>c:<>k' -l
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
Interpolation may occur in following cases:
- for template arguments (`-T ...`) in walk/update/insert/compare type of operations
- for the argument undergoing a
[shell evaluation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation)
(`-e` + insert/update operation, e.g.: `-eu ... \;`),
- in the lexeme `<..>u` applying a shell evaluation on its content
- in the lexeme `<..>j` where lexeme content is expressed as a template
- for any search lexeme where a quantifier is expressed through interpolation, e.g.: `<>a{N}`

Interpolation occurs either from the namespaces, or from currently walked JSON element. Every occurrence (in the templates or in
shell cli) of tokens `{}` or `{{}}` will trigger interpolation:
- if the content under braces is empty (`{}`, `{{}}`) then the currently walked JSON element is getting interpolated
- if the content is present (e.g.: `{val}`, `{{val}}`) then interpolation occurs from the respective namespace


### Interpolation types
The difference between single `{}` and double `{{}}` notations: 
- double notation `{{..}}` interpolate JSON elements exactly, so it's always a safe type
- a single notation `{..}` interpolate _JSON strings_, _JSON arrays_, _JSON objects_ differently:
  * when interpolating _JSON string_, the outer quotation marks are dropped, e.g., instead of `"blah"`, it will be interpolated as 
  `blah`. Thus, it makes sense to use this interpolation inside double quotations (the interpolated value still has to be a valid JSON)
  * when interpolating _JSON array_, then enclosing brackets `[`, `]` are dropped (allows extending arrays); e.g., `[1,2,3]` 
  will be interpolated as `1,2,3` (which is invalid JSON), thus to keep it valid the outer brackets must be provided - `-T'[ {} ]'`
  * when interpolating _JSON object_, then enclosing braces `{`, `}` are dropped (allows extending objects), e.g., `{"pi":3.14}` 
  will be interpolated as `"pi": 3.14`, so to keep it valid the outer braces must be provided, e.g., `-T{ {}, "key": "new" }`

A string interpolation w/o outer quotes is handy when it's required altering an existing string, here's example of altering JSON label:
```
bash $ <<<'{ "label": "value" }' jtc
{
   "label": "value"
}
bash $ <<<'{ "label": "value" }' jtc -w'<label>l<>k' -u'<label>l<L>k' -T'"new {L}"'
{
   "new label": "value"
}
bash $ 
```
Here's an illustration when double braces are handier (e.g., swapping around labels and values):
```
bash $ JSN='{ "pi": 3.14, "type": "irrational" }'
bash $ <<<$JSN jtc
{
   "pi": 3.14,
   "type": "irrational"
}
bash $ <<<$JSN jtc -i'[:]<Key>k<Val>v' -T'{ "{Val}": {{Key}} }' -p
{
   "3.14": "pi",
   "irrational": "type"
}
bash $ 
```

An array interpolation using a single brace notation `{..}` is handy when there's a need extending the array _during_
interpolation. 
There's a special case though - template-extending of an empty array. Let's consider a following example:
```
bash $ JSN='[ {"args": [123],"Func": "x + y"}, { "args":[], "Func":"a * b" }  ]'
bash $ <<<$JSN jtc
[
   {
      "Func": "x + y",
      "args": [
         123
      ]
   },
   {
      "Func": "a * b",
      "args": []
   }
]
bash $ 
```
And the ask here would be to augment all arrays in each `args` with the arguments from respective `Func`:
```
bash $ <<<$JSN jtc -w'[:][args]' -u'[:][Func]<(\w+)[ +*]+(\w+)>R[-1][args]' -T'[{}, {{$1}}, {{$2}}]'
[
   {
      "Func": "x + y",
      "args": [
         123,
         "x",
         "y"
      ]
   },
   {
      "Func": "a * b",
      "args": [
         "a",
         "b"
      ]
   }
]
bash $ 
```

When interpolating the second record, the interpolation in fact, would result in the invalid JSON: the _array_ in `args` is empty
initially (`[]`), thus when it gets interpolated via template `[{}, {{$1}}, {{$2}}]` it becomes `[, "a", "b"]` - which is an invalid
JSON. However, `jtc` is aware of such empty iterables and handles them properly, allowing extending even empty arrays and objects
without producing failures.

All the same applies for when interpolating _JSON objects_ and _JSON strings_. For _JSON objects_ such null-interpolation is aware
of enumerations occurring over of either of `,` or `;`


### Namespaces with interleaved walks
When multiple _interleaved_ walks (`-w`) present (obviously there must be multiple walks - a single one cannot be _interleaved_), 
they populate namespaces in the order the walks appear:
```
bash $ <ab.json jtc -x[0][:] -y'[name]<pnt>v' -y'[children][:]<chld>v' -T'{ "Parent": {{pnt}}, "child": {{chld}} }' -r
"John"
{ "Parent": "John", "child": "Olivia" }
{ "Parent": "Ivan", "child": "Olivia" }
{ "Parent": "Jane", "child": "Olivia" }
{ "Parent": "Jane", "child": "Robert" }
{ "Parent": "Jane", "child": "Lila" }
bash $ 
```
That is a correct result (though might not reflect what possibly was intended), let review the result:
1. first line contains only result `"John"` - because template interpolation here failed (namespace `chld` does not yet exist yet,
thus the resulting template is _invalid JSON_) hence source walk is used / printed last walked JSON value
2. upon next (_interleaved_) walk, we see a correct result of template interpolation: `Parent`'s and `child`'s records are filled right
(template is a _valid JSON_ here)
3. in the third line, the result is also correct, albeit might be not the expected one - upon next _interleaved_ walk, the 
namespace `pnt` is populated with `"Ivan"`, but the namespace `chld` still carries the old result.
4. _etc._

By now it should be clear why is such result.

Going by the notion of the provided template, apparently, the expected result were to have all records pairs for each person with 
each own child. That way, for example, `Ivan` should not be even listed (he has no children), `John`'s record should appear only
once and `Jane` should have 2 records (she has 2 kids).

The situation could be easily rectified if for each walk we use own template and assign a dummy one for the first one: 
```
bash $ <ab.json jtc -x[0][:] -y'[name]<pnt>v' -T'""' -y'[children][:]<chld>v' -T'{ "Parent": {{pnt}}, "child": {{chld}} }' -r
""
{ "Parent": "John", "child": "Olivia" }
""
""
{ "Parent": "Jane", "child": "Robert" }
{ "Parent": "Jane", "child": "Lila" }
bash $ 
```
Now the result looks closer to the intended one (no records for `Ivan`, one for `John` and 2 for `Jane`, as expected). But what about
those annoying empty _JSON strings_ `""`? Those will be gone if `-qq` option is thrown in:
```
bash $ <ab.json jtc -x[0][:] -y'[name]<pnt>v' -T'""' -y'[children][:]<chld>v' -T'{ "Parent": {{pnt}}, "child": {{chld}} }' -rqq
{ "Parent": "John", "child": "Olivia" }
{ "Parent": "Jane", "child": "Robert" }
{ "Parent": "Jane", "child": "Lila" }
bash $ 
```
\- that's a neat, though a [documented](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#unquoting-JSON-strings) trick

Yet, the same could have been achieved even a simpler way (using just one walk):
```
bash $ <ab.json jtc -w'[0][:][name]<pnt>v[-1][children][:]' -T'{ "Parent": {{pnt}}, "child": {{}} }' -r
{ "Parent": "John", "child": "Olivia" }
{ "Parent": "Jane", "child": "Robert" }
{ "Parent": "Jane", "child": "Lila" }
bash $ 
```


### Search quantifiers interpolation
Interpolation may also occur in quantifiers, say we have a following JSON, where we need to select an item from `list` by the
index value stored `item`:
```
bash $ JSN='{ "item": 2, "list": { "milk": 0.90, "bread": 1.20, "cheese": 2.90 } }'
bash $ <<<$JSN jtc
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
bash $ <<<$JSN jtc -w'[item]<idx>v[-1][list]><a{idx}' -l
"milk": 0.90
bash $ 
```
It should be quite easy to read/understand such walk path (predicated one is familiar with suffixes / directives). Let's see
how the walk-path works in a slow-mo:
1. `[item]` selects the value by label `item`:
```
bash $ <<<$JSN jtc -w'[item]'
2
bash $ 
```
2. `<idx>v` the directive memorizes selected value (`2`) in the namespace `idx`
```
bash $ <<<$JSN jtc -w'[item]<idx>v'
2
bash $ 
```
3. `[-1]` steps up 1 level in the JSON tree off the current position (i.e., addresses the first parent of the `item` value) which is
the root of the input JSON:
```
bash $ <<<$JSN jtc -w'[item]<idx>v[-1]'
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
bash $ <<<$JSN jtc -w'[item]<idx>v[-1][list]'
{
   "bread": 1.20,
   "cheese": 2.90,
   "milk": 0.90
}
bash $ 
```
5. `><a{idx}` - a non-recursive search of atomic values (`><a`) indexed by a quantifier with the stored in the namespace `idx`
(which is `2`) gives us the required value.

_Alternatively_, the same ask could be achieved using a slightly different query:
```
bash $ <<<$JSN jtc -w'[item]<idx>v[-1][list]>idx<t' -l
"milk": 0.90
bash $ 
```
- `>idx<t` lexeme here will utilize namespace `idx` to find the offset (index).

There's a subtle difference how the lexeme `t` treats and uses referred namespace:
- in `<..>t` notation, the lexeme always treats the value in the namespace as _JSON string_ and will try searching (recursively) a 
respective label. I.e., even if the value in the namespace is numerical value `0`, it will search for a label `"0"` instead
- in `>..<t` notation, if the namespace holds a literal (i.e., a _JSON string_) value, then the lexeme will try matching the label
(as expected);  however, if the namespace holds a numerical value (_JSON number_), then the value is used as a direct offset
in the searched JSON node


### Cross-referenced lookups
One use-case that namespaces facilitate quite neatly, is when insert/update/purge/compare operation refer to different JSONs 
(i.e., in [Use of mixed arguments](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#use-of-mixed-arguments-for--i--u--c) 
types of operations) but one requires a reference from another.

Say, we have 2 JSONs:
1. `main.json`:
```
bash $ <main.json jtc
[
   {
      "name": "Abba",
      "rec": 1,
      "songs": []
   },
   {
      "name": "Deep Purple",
      "rec": 3,
      "songs": []
   },
   {
      "name": "Queen",
      "rec": 2,
      "songs": []
   }
]
bash $ 
```
2. `id.json`:
```
bash $ <id.json jtc
[
   {
      "id": 3,
      "title": "Smoke on the Water"
   },
   {
      "id": 1,
      "title": "The Winner Takes It All"
   },
   {
      "id": 2,
      "title": "The Show Must Go On"
   }
]
bash $ 
```

The ask here is to insert songs titles from `id.json` into main.json cross-referencing respective `rec` to `id` values.  
The way to do it:
- first walk `main.json` finding and memorizing (each) `rec` value 
- then, walk up to the `song` entry  (so that will be a destination pointer, where song needs to be inserted).

The insert operation (`-i`) here would need to find `id` record in `id.json` using memorized (in the destination walk) namespace and 
insert respective `title`:
```
bash $ <main.json jtc -w'[:][rec]<Record>v[-1][songs]' -mi id.json -i'[id]:<Record>s[-1][title]'
[
   {
      "name": "Abba",
      "rec": 1,
      "songs": [
         "The Winner Takes It All"
      ]
   },
   {
      "name": "Deep Purple",
      "rec": 3,
      "songs": [
         "Smoke on the Water"
      ]
   },
   {
      "name": "Queen",
      "rec": 2,
      "songs": [
         "The Show Must Go On"
      ]
   }
]
bash $ 
```

For each destination walk (`-w`) here, there will be a respective insert-walk (`-i`) (`-w` is walked first). When dst. walk 
finishes walking, the namespace will be populated with respective value from the `rec` entry. That value will be reused by insert-walk
when walking its source JSON (`id.json`) with the lexeme `[id]:<Record>s` - that will find a respective `id`. The rest should be obvious
by now.


#### Cross-referenced purge
`jtc` does not have a "walk" argument for `-p` (purge) operation (`-p` is a standalone option, when it's used only with `-w`
it will purge every resulted/walked entry).  
So, how to facilitate a cross-referenced purge then? (i.e., when purging ids are located in a separate file)  

The trick is to use a dummy `-u`/`-i` operation and apply `-p`.  
When the cli is given in this notation:  
`<<<dst.json jtc -w... -u <src.json> -u... -p`,  
purging will be applied to walked destinations, but only predicated by a successful source walk:
```
bash $ <main.json jtc -w'[:][rec]<Record>v[-1]' -u'[{"id":1}, {"id":3}]' -u'[id]:<Record>s' -p
[
   {
      "name": "Queen",
      "rec": 2,
      "songs": []
   }
]
bash $ 
```

The "complemented" purge operation (i.e. when you want to delete everything except referenced) is facilitated using `-pp`:
```
bash $ <main.json jtc -w'[rec]:<Record>N:[-1]<Entry>v' -u'[1, 3]' -u'<Record>s' -T'{{Entry}}' -pp
[
   {
      "name": "Abba",
      "rec": 1,
      "songs": []
   },
   {
      "name": "Deep Purple",
      "rec": 3,
      "songs": []
   }
]
bash $ 
```
\- memorizing the whole `Entry` is required because update operation w/o the template only replaced records (and purge everything else),
but that's not the goal - goal is to retain all the entries, hence replacing the updating entries with the template for the entire entry.



## Templates
Template, an argument to `-T` is a literal JSON optionally containing tokens for
[interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation). Templates can be used upon walking, 
insertion, updates and when comparing. The result of template interpolation still must be a valid JSON. If a template (`-T`) is given
then it's a template value (after interpolation) will be used for the operations, not the source walk (unless the resulting template is
invalid JSON, in such case the source walk will be used).

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

For the rest of operations (`-i`, `-u`, `-c`) templates are getting interpolated from walk-path of the operation argument itself and
never from `-w`. The namespaces resulting from walking destinations (`-w`) are shared with source walks in operations (`-i`, `-u`, `-c`)
\- that way 
[cross-referenced lookups](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#cross-referenced-lookups)
 are possible.
Logically, for each destination walk (`-w`) there will be a respective subsequent source walk (e.g.: `-c<src-walk>`), thus source walk
may utilize the namespaces populated during destination walk (`-w`). Template-interpolation will be attempted only once source walk is
successful

Below is an example of updating the phone records for the first entry in the `Directory` (appending a country-code and 
altering the `phone` label at the same time via template):
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
bash $ <<<$(<ab.json jtc -w'[0][0]<phone>l[:]' -pi'[0][0]<number>l:<val>v' -T'{ "phone number": "+1 {val}" }') jtc -w'[0][0]<phone>l'
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
- `-w'[0][0]<phone>l[:]'` - that's our destination which we will be updating (i.e., all phone records in the first `Directory` entry)
- `-pi'[0][0]<number>l:<val>v'` - we'll walk (synchronously with `-w`) all the `number` records and memorize `number` values in the
namespace `val`; option `-p` turns _insert_ operation into _move_
- `-T'{ "phone number": "+1 {val}" }'` after each source walk argument in `-i`, a template interpolations occurs - a new 
JSON entry is generated from the template and namespace `val` and the new entry is then used for insertion into the respective
destination walk (`-w`). Thus using templates it becomes easy to transmute existing JSON into a new one.


### Multiple templates and walks
When multiple templates given and a number of walks (`-w<walk>`, or `-u<walk>`, `-i<walk>`) **is matching** the number of templates
(i.e., more than one walk is present) then templates are pertain per each walk. In all other cases templates are applied in a
round-robin fashion.  
In the case when round-robin behavior is required when number of templates and walks matches, use `-nn` notation - it will ensure
round-robin templates application onto sequential walks

Compare:
```
bash $ <ab.json jtc -x[0][:] -y[name] -T'{"Person":{{}}}' -y[age] -T'{"Age":{{}}}' -r
{ "Person": "John" }
{ "Age": 25 }
{ "Person": "Ivan" }
{ "Age": 31 }
{ "Person": "Jane" }
{ "Age": 25 }
bash $ 
bash $ <ab.json jtc -x[0][:] -y[name] -T'{"Person":{{}}}' -y[age] -T'{"Age":{{}}}' -rn
{ "Person": "John" }
{ "Person": "Ivan" }
{ "Person": "Jane" }
{ "Age": 25 }
{ "Age": 31 }
{ "Age": 25 }
bash $ <ab.json jtc -x[0][:] -y[name] -T'{"Person":{{}}}' -y[age] -T'{"Age":{{}}}' -rnn
{ "Person": "John" }
{ "Age": "Ivan" }
{ "Person": "Jane" }
{ "Age": 25 }
{ "Person": 31 }
{ "Age": 25 }
bash $ 

```
The mess in the above's last example is explained by `-nn` usage: templates are forced to apply in the round-robin fashion


One handy use-case of multiple round-robin templates would be this example:
```
bash $ <<<'[1,2,3,4,5,6,7,8,9,10]' jtc -w[:] -T'""' -T{} -T'""' -qq
2
5
8
bash $ 
```
\- in the above example printed every 3rd element from source JSON starting from the 2nd one (recall: when unquoting an empty
JSON string ("") the resulted blank lines are not getting printed).


### Stringifying JSON, Jsonizing stringified
There's one more token combination in templates allowing _stringification_ and _jsonization_ of values:
- `<..>` will try "expanding" a string value into a JSON
- `>..<` will take a current JSON value and stringify it

The token notation follows the same rule as for regular tokens (`{}`, `{{}}`), i.e. single angular bracket notation will result
in a "naked" JSON value (i.e. without quotation marks, curly braces or square brackets for strings, objects and arrays
respectively). While double token notation is always a safe type and result of operation while be a complete JSON type.

This little demo illustrates the tokens usage:
```
bash $ <<<'["a", "b"]' jtc -T'">{{}}<"'
"[ \"a\", \"b\" ]"
```
\- because a single form of angular token notation was used, the outer quotation marks were necessary for a successful interpolation.
The same could have been achieved with the template: `-T'>>{{}}<<'`

That was an example of _stringification_ of a JSON value, now let's do a reverse thing - _jsonize_ previously stringified value:
```
bash $ <<<'["a", "b"]' jtc -T'>>{{}}<<' | jtc -T'[ <{{}}>, "c"]' -r
[ "a", "b", "c" ]
bash $ 
```
\- the above example sports _jsonization_ of the previously _stringified JSON_ while extending resulting JSON array at the same time


## Processing multiple input JSONs
Normally `jtc` would process only a single input JSON. If multiple input JSONs given - the fist JSON will be processed and the 
rest of the inputs will be silently ignored:
```
bash $ <<<'[ "1st json" ] { "2nd": "json" } "3rd json"' jtc -r
[ "1st json" ]
bash $ 
```
Couple options allow altering the behavior and process all the input JSONs:


### Process all input JSONs
Option `-a` instructs to process all the input JSONS:
```
bash $ <<<'[ "1st json" ] { "2nd": "json" } "3rd json"' jtc -ar
[ "1st json" ]
{ "2nd": "json" }
"3rd json"
bash $ 
```
\- respected processing (of all given options) will occur for all of the input JSONs:
```
bash $ <<<'[ "1st json" ] { "2nd": "json" } "3rd json"' jtc -a -w'<json>R'
"1st json"
"json"
notice: input atomic JSON is non-iterable, ignoring all walk-paths (-w)
"3rd json"
bash $ 
```
All the input JSONs will be processed as long they are valid - processing will stops upon parsing failure:
```
bash $ <<<'[ "1st json" ] { "2nd": json" } "3rd json"' jtc -ad
.read_inputs(), reading json from <stdin>
.write_json(), outputting json to <stdout>
[
   "1st json"
]
.parsejson(), exception locus: [ "1st json" ] { "2nd": j
.location_(), exception spot: ------------------------>| (offset: 24)
jtc json exception: expected_json_value
bash $ 
```

### Wrap all processed JSONs
option `-J` let wrapping all processed input JSONs into a super JSON array (option `-J` assumes option `-a`, no need giving both):
```
bash $ <<<'[ "1st json" ] { "2nd": "json" } "3rd json"' jtc -J -w'<json>R'
notice: option -J cancels streaming input
notice: input atomic JSON is non-iterable, ignoring all walk-paths (-w)
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
bash $ jtc -w'[0][:][name]' -aj ab.json ab.json
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
bash $ jtc -w'[0][:][name]' -J ab.json ab.json
[
   "John",
   "Ivan",
   "Jane",
   "John",
   "Ivan",
   "Jane"
]
bash $ 
bash $ jtc -w'[0][:][name]' -Jj ab.json ab.json
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

_Note: `jtc` supports unlimited number of files that can be supplied as standalone arguments (after all options given). When 
multiple input files are given, options `-a` is assumed_


### Buffered vs Streamed read
`jtc` may read inputs 2 ways:
- _**buffered read**_
- _**streamed read**_

In the _buffered read_ mode (which is default), entire file (or `<stdin>`) input is read into memory and only then JSON parsing is
attempted (with all subsequent due processing).  
In the _streamed read_ mode JSON parsing begins immediately as the the first character is read (so, no memory wasted to hold input JSON).

The _streamed read_ is activated when:
- option `-a` given *AND* input source is `<stdin>`  
- option `-J` overrides _streamed read_ (reverting to _buffered_): _streamed read_ might be endless, while option `-J` assumes a finite
number of inputs to be processed and then displayed

From the JSON result point of view there's no difference between _buffered_ and _streamed_ reads - the result will be 100% consistent
across those types of reads. However, _streamed read_ finds its application when streamed data are there (typically would be
a network-based streaming)

We can see the difference in the parsing when debugging `jtc`:
\- in a _buffered read_ mode, the debug will show the _parsing point_ with data after:
```
bash $ <ab.json jtc -dddddd 
.read_inputs(), reading json from <stdin>
......parse_(), parsing point ->{|  "Directory": [|    {|       "name": "John",|       "ag...
......parse_(), parsing point ->"Directory": [|    {|       "name": "John",|       "age": ...
......parse_(), parsing point ->[|    {|       "name": "John",|       "age": 25,|       "a...
......parse_(), parsing point ->{|       "name": "John",|       "age": 25,|       "address...
...
```
\- in a _streamed read_ mode, the _parsing point_ would point to the last read character from the `<stdin>`:
```
bash $ <ab.json jtc -dddddd -a
.read_inputs(), reading json from <stdin>
..ss_init_(), initializing: stream
......parse_(), {<- parsing point
......parse_(), {|  "<- parsing point
......parse_(), {|  "Directory": [<- parsing point
......parse_(), {|  "Directory": [|    {<- parsing point
...
```

Here's an example of how _streamed read_ works in `jtc`:
```
|                       Screen 1                       |                       Screen 2                       |
| ---------------------------------------------------- | ---------------------------------------------------- |
| bash $ nc -lk localhost 3000 | jtc -ra               | bash $ <ab.json jtc -w'<address>l:' | nc localhost 3 |
| { "city": "New York", "postal code": 10012, "state": | 000                                                  |
| "NY", "street address": "599 Lafayette St" }         | bash $                                               |
| { "city": "Seattle", "postal code": 98104, "state":  | bash $ <ab.json jtc -w'<name>l:' -w'<name>l:[-1][pho | 
| "WA", "street address": "5423 Madison St" }          | ne]' | nc localhost 3000                             |
| { "city": "Denver", "postal code": 80206, "state": " | bash $                                               |
| CO", "street address": "6213 E Colfax Ave" }         | bash $ <ab.json jtc -w'<name>l:<N>v[-1][children]' - |
| "John"                                               | T'{"Parent":{{N}}, "progeny": {{}} }' | nc localhost |
| [ { "number": "112-555-1234", "type": "mobile" }, {  | 3000                                                 |
| "number": "113-123-2368", "type": "mobile" } ]       | bash $                                               |
| "Ivan"                                               |                                                      |
| [ { "number": "273-923-6483", "type": "home" }, { "n |                                                      |
| umber": "223-283-0372", "type": "mobile" } ]         |                                                      |
| "Jane"                                               |                                                      |
| [ { "number": "358-303-0373", "type": "office" }, {  |                                                      |
| "number": "333-638-0238", "type": "home" } ]         |                                                      |
| { "Parent": "John", "progeny": [ "Olivia" ] }        |                                                      |
| { "Parent": "Ivan", "progeny": [] }                  |                                                      |
| { "Parent": "Jane", "progeny": [ "Robert", "Lila" ]  |                                                      |
| }                                                    |                                                      |
|                                                      |                                                      |
```
 
In the `Screen 1`, `jtc` listens to the stream data coming from `netcat` utility and process-prints (in a compact format) all
the input JSONs. It will stop once `<stdin>` is closed, but `netcat` is run using `-k` option, which means _endlessly_.

In the `Screen 2`, `jtc` sends to `netcat` a few walks (JSONs), which `netcat` relays to its counterpart in the `Screen1`.


## More Examples
### Generating CSV from JSON
`CSV` stands for _comma separated values_, thus to convert a JSON into a `csv` file, it's required dumping all relevant JSON walks
line by line, while separating JSON values either with `,` or with `;` (`csv` format admits both)

There are couple tricks required to do so, but not difficult ones, so, let's walk it.

Say, we want to dump into `csv` format following data from the `ab.json`:  
`name, city, postal code, state, street address`

1. First let's just dump all those entries:
```
bash $ <ab.json jtc -rx[0][:] -y[name] -y[address][:]
"John"
"New York"
10012
"NY"
"599 Lafayette St"
"Ivan"
"Seattle"
98104
"WA"
"5423 Madison St"
"Jane"
"Denver"
80206
"CO"
"6213 E Colfax Ave"
bash $ 
```
\- not that difficult

2. then we need to reconcile all the walks into lines, here `$?` auto-namespace comes handy:
```
bash $ <ab.json jtc -rx[0][:] -y[name] -y[address][:] -T'"{$?}, {}"'
"John"
"John, New York"
"John, New York, 10012"
"John, New York, 10012, NY"
"John, New York, 10012, NY, 599 Lafayette St"
"John, New York, 10012, NY, 599 Lafayette St, Ivan"
"John, New York, 10012, NY, 599 Lafayette St, Ivan, Seattle"
"John, New York, 10012, NY, 599 Lafayette St, Ivan, Seattle, 98104"
"John, New York, 10012, NY, 599 Lafayette St, Ivan, Seattle, 98104, WA"
"John, New York, 10012, NY, 599 Lafayette St, Ivan, Seattle, 98104, WA, 5423 Madison St"
"John, New York, 10012, NY, 599 Lafayette St, Ivan, Seattle, 98104, WA, 5423 Madison St, Jane"
"John, New York, 10012, NY, 599 Lafayette St, Ivan, Seattle, 98104, WA, 5423 Madison St, Jane, Denver"
"John, New York, 10012, NY, 599 Lafayette St, Ivan, Seattle, 98104, WA, 5423 Madison St, Jane, Denver, 80206"
"John, New York, 10012, NY, 599 Lafayette St, Ivan, Seattle, 98104, WA, 5423 Madison St, Jane, Denver, 80206, CO"
"John, New York, 10012, NY, 599 Lafayette St, Ivan, Seattle, 98104, WA, 5423 Madison St, Jane, Denver, 80206, CO, 6213 E Colfax Ave"
bash $ 
```
\- hmmm, but we need such line one per record and not like above

3. To ensure that we reconcile only each record (and not all of them), let's add one more walk which will fail interpolation of `$?`
in the template (that will reset the value of `$?` back to `""`)
```
bash $ <ab.json jtc -rx[0][:] -y[name] -y[address][:] -y' ' -T'"{$?}, {}"'
"John"
"John, New York"
"John, New York, 10012"
"John, New York, 10012, NY"
"John, New York, 10012, NY, 599 Lafayette St"
{ "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" }, "age": 25, "children": [ "Olivia" ], "name": "John", "phone": [ { "number": "112-555-1234", "type": "mobile" }, { "number": "113-123-2368", "type": "mobile" } ], "spouse": "Martha" }
"Ivan"
"Ivan, Seattle"
"Ivan, Seattle, 98104"
"Ivan, Seattle, 98104, WA"
"Ivan, Seattle, 98104, WA, 5423 Madison St"
{ "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }, "age": 31, "children": [], "name": "Ivan", "phone": [ { "number": "273-923-6483", "type": "home" }, { "number": "223-283-0372", "type": "mobile" } ], "spouse": null }
"Jane"
"Jane, Denver"
"Jane, Denver, 80206"
"Jane, Denver, 80206, CO"
"Jane, Denver, 80206, CO, 6213 E Colfax Ave"
{ "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }, "age": 25, "children": [ "Robert", "Lila" ], "name": "Jane", "phone": [ { "number": "358-303-0373", "type": "office" }, { "number": "333-638-0238", "type": "home" } ], "spouse": "Chuck" }
bash $ 
```
\- that's better, now every 5th walk out of every 6 is what we need.

4. Display only required walks:
```
bash $ <ab.json jtc -rx[0][:] -y[name] -y[address][:] -y' ' -T'"{$?}, {}"' -x6/4
"John, New York, 10012, NY, 599 Lafayette St"
"Ivan, Seattle, 98104, WA, 5423 Madison St"
"Jane, Denver, 80206, CO, 6213 E Colfax Ave"
bash $ 
```
\- that is our required `CSV` format! Well, almost - every line is still a JSON string, but we can undress it with `-qq` option

5. just a sugar topping: let's add a header line, which could be done using `echo` unix cli:
```
bash $ echo -e "name, city, postal, street\n$(<ab.json jtc -x[0][:] -y[name] -y'[address][:]' -y' ' -T'"{$?}, {}"' -x6/4 -qq)"
name, city, postal, street
John, New York, 10012, NY, 599 Lafayette St
Ivan, Seattle, 98104, WA, 5423 Madison St
Jane, Denver, 80206, CO, 6213 E Colfax Ave
bash $ 
```
DONE.


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
- `[^0]<.>s:` reset the search path back to the root and now find all the elements (i.e., all duplicates).

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

**CAUTION:** _be aware that directives `<..>q`, `<..>Q` are dynamic types, which means they are exempted for cacheing,
implying that on very big JSONs an **exponential decay** will become noticeable. Plus, both directives are quite memory hungry._  
Thus use them cautiously when dealing with big JSONs (around or above hundreds of thousand of nodes - subjected to a spec of your machine)


### Counting with `jtc`
Counting any number of properties is JSON could be done using exteral `wc` unix utility. E.g., let's count all `number`s in `ab.json`:
```
bash $ <ab.json jtc -w'<number>l:' | wc -l
       6
bash $ 
```

The same is possible to achieve using only `jtc` capability - using `<..>I` lexeme:
```
bash $ <ab.json jtc -w'<number>l:<cnt>I1' -T{cnt} -x/-1
6
bash $ 
```
- `<cnt>I1` will arrange a namespace var `cnt` counting values starting from `0` with increment of `1` upon each walk pass (iteration)
- `-T{cnt}` will interpolate it
- `-x/-1` will display on the last walk

Say, now we want to count the same phone numbers, but for some reason starting from `100`:
```
bash $ <ab.json jtc -w'<cnt:100>f[]<>F<number>l:<cnt>I1' -T{cnt} -x/-1
106
bash $ 
```
- `<cnt:100>f` will setup a fail-safe point at the same time initializing namespace `cnt` with value `100`
- `[]` is a walk lexeme which is guaranteed to fail here (there are no empty lables), so, it will trigger a fail-stop, which will
continue walking past `<>F` lexeme - that will ensure that initial offset of the `cnt` is set to `100` instead of default `0`


Finally, let's count home numbers and mobile numbers separately:
```
bash $ <ab.json jtc -x'<phone>l:' -y'<home>:<hn>I1'  -y'<mobile>:<mn>I1' -T'{"total home numbers": {hn}, "total mobile numbers": {mn}}' -x/-1
{
   "total home numbers": 2,
   "total mobile numbers": 3
}
bash $ 
```







