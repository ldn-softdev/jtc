# User Guide: [`jtc`](https://github.com/ldn-softdev/jtc). Examples and Use-cases (_v.1.76_, being updated)

1. [Displaying JSON](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#displaying-json)
   * [Pretty printing (`-t`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#pretty-printing)
   * [Compact printing (`-r`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#compact-printing)
   * [Semi-compact printing (`-tNc`,`-tc`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#semi-compact-printing)   
   * [Printing JSON size (`-z`, `-zz`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#printing-json-size)
   * [Validating JSON (`-d`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#validating-json)
   * [Forcing strict solidus parsing (`-q`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#forcing-strict-solidus-parsing)
   * [Unquoting JSON strings (`-qq`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#unquoting-JSON-strings)
   * [Stringifying JSON (`-rr`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json)
   * [Summary of display options](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#summary-of-display-options)
2. [Walking JSON (`-w`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-json)
   * [Walking with subscripts (`[..]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-with-subscripts-offset-lexemes)
     * [Selecting multiple subscripted JSON elements (`[+n]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#selecting-multiple-subscripted-json-elements)
     * [Subscript slice notation (`[n:m:s]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#subscript-slice-notation)
   * [Searching JSON (`<..>`, `>..<`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#searching-json)
     * [Searching JSON with RE (`<..>R`,`<..>L`, `<..>D`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#searching-json-with-re)
     * [Search suffixes (`rRPdDNbnlLaoicewjstqQgG`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-suffixes)
       * [Cached Search](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#-cached-search)
     * [Directives (`vkzfFuIZW`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#directives)
     * [Setting a custom JSON value into a namespace](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#setting-a-custom-JSON-value-into-a-namespace)
     * [Fail-safe and Forward-Stop directives (`<..>f`, `<..>F`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#fail-safe-and-forward-stop-directives)
       * [Examples illustrating _fail-safe_ using namespaces and interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#-examples-illustrating-fail-safe-using-namespaces-and-interpolation)
       * [Uses of `Fn` directive with non-default quantifiers (`<>Fn`, `><Fn`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#-uses-of-fn-directive-with-non-default-quantifiers)
     * [RE generated namespaces (`$0`, `$1`, etc)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#re-generated-namespaces)
     * [Search quantifiers (`n`,`+n`,`n:m:s`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers)
       * [Search quantifiers with relative offset semantic (`>..<t`, `>..<l`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#-search-quantifiers-with-relative-offset-semantic)
     * [Scoped search `[..]:<..>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#scoped-search)
     * [Non-recursive search (`>..<`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#non-recursive-search)
   * [Addressing parents (`[-n]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#addressing-parents)
     * [Addressing parents offsetting from root (`[^n]`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#addressing-parents-offsetting-from-root)
   * [Walking multiple walk-paths](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-multiple-walk-paths)
     * [Sequential walk processing (`-n`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#sequential-walk-processing)
     * [Displaying walks with labels (`-l`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#displaying-walks-with-labels)
     * [Wrapping resulted walks to JSON array (`-j`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-resulted-walks-to-json-array)
     * [Interleaved walk processing](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interleaved-walk-processing)
     * [Aggregating walks (`-nn`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#aggregating-walks)
     * [Wrapping walked entries into a JSON object (`-jj`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-walked-entries-into-a-json-object)
     * [Extracting labeled values (`-ll`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#extracting-labeled-values)
     * [Succinct walk-path syntax (`-x`,`-y`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#succinct-walk-path-syntax)
     * [Controlling displayed walks (`-xn/N`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#controlling-displayed-walks)
   * [Summary of walk lexemes](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#summary-of-walk-lexemes)
   * [Summary of walk options](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#summary-of-walk-options)
3. [Interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation)
   * [Interpolation token types (`{}` vs `{{}}`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation-token-types)
      * [String interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#string-interpolation)
      * [Interpolation of iterables](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation-of-iterables)
        * [Interpolation of iterables into a string template](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#-interpolation-of-iterables-into-a-string-template)
   * [Namespace](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespace)
       * [Cross-lookups using namespace (`<>s`, `<>t`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#cross-lookups-using-namespace)
       * [Path namespace example (`$PATH`, `$path`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#path-namespace-example)
       * [Prior walk token (`$?`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#prior-walk-token)
       * [Iterables auto tokens (`$a`, `$A`, `$b`, etc)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#iterables-auto-tokens)
   * [Namespaces with interleaved walks](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespaces-with-interleaved-walks)
   * [Search quantifiers interpolation (`<...>S{..}`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers-interpolation)
   * [Templates (`-T`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates)
     * [Multiple templates and walks](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#multiple-templates-and-walks)
     * [Stringifying JSON, Jsonizing stringified(`>{{}}<`, `>>{{}}<<`, `<{{}}>`, `<<{{}}>>`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json-jsonizing-stringified)
   * [Summary of interpolation token types](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#summary-of-interpolation-token-types) 
   * [Summary of namespace tokens](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#summary-of-namespace-tokens) 
4. [Modifying JSON](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#modifying-json)
   * [In-place JSON modification (`-f`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#in-place-json-modification)
     * [Ensuring input read from `stdin` (`-`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#ensuring-input-read-from-stdin)
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
   * [Use of mixed argument types for `-i`, `-u`, `-c` (e.g.: `jtc -u<JSON> -u<walk-path>`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#use-of-mixed-argument-types-for--i--u--c)
   * [Use of mixed argument types with `-e`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#use-of-mixed-argument-types-with--e)
   * [Cross-referenced insert, update](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#Cross-referenced-insert-update)
     * [Cross-referenced purge](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#cross-referenced-purge)
   * [Summary of modification options](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#summary-of-modification-options)
5. [Comparing JSONs (`-c`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#comparing-jsons)
   * [Comparing JSON schemas](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#comparing-json-schemas)
6. [Processing input JSONs](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#processing-input-jsons)
   * [Parse ill-formed JSONs with clashing labels (`-mm`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#parse-ill-formed-JSONs-with-clashing-labels)
   * [A word on ordering within JSON objects](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#a-word-on-ordering-within-JSON-objects)
   * [Process all input JSONs (`-a`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#process-all-input-jsons)
   * [Wrap all processed JSONs (`-J`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrap-all-processed-jsons)
   * [Buffered vs Streamed read](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#buffered-vs-streamed-read)
   * [Chaining option sets (`/`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#Chaining-option-sets)
8. [Some Examples](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#some-examples)
   * [Generating CSV from JSON](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#generating-csv-from-json)
   * [Taming duplicates (`<..>q`, `<..>Q`)](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#taming-duplicates)
     * [Remove duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#remove-duplicates)
     * [Remove all but duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#remove-all-the-elements-but-the-ones-which-have-duplicates)
     * [Leave only those which have no duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#leave-only-those-which-have-no-duplicates)
     * [Leave all duplicates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#leave-all-duplicates)
   * [Counting with `jtc`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#counting-with-jtc)
   * [Transposing a matrix](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#transposing-a-matrix)
   * [Sorting JSONs](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#sorting-jsons)
   * [More examples and case studies](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#more-examples-and-case-studies)

---

## Displaying JSON
### Pretty printing
If no argument given, `jtc` will expect an input JSON from the `<stdin>`, otherwise JSON is read from the file(s) pointed by the 
argument(s). `jtc` will parse and validate input JSON and upon a successful validation will output:
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
bash $ <ab.json jtc -t10
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

By default, the compact printing view will use a single spacer between all tokens, that also could be controlled if `-r` and `-t`
used together, e.g., to print the above JSON w/o spacer:
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
.init_inputs(), reading json from <stdin>
.exception_locus_(), ...e": 80206,|            "state": "CO,|            "street address": "6213...
.exception_spot_(), --------------------------------------->| (offset: 1214)
jtc json parsing exception (<stdin>:1214): unexpected_end_of_line
bash $ 
```
the vertical pipe symbol `|` in the debug showing JSON locus replaces new lines, thus it becomes easy to spot the problem. 
The offset (`1214` in the example) is given in _unicode UTF-8_ characters from the beginning of the input/file/stream.
In that particular failure instance, `jtc` found the end of a line, while _JSON string_ `"Co,` is still open (JSON standard does not
permit multi-line strings). To fix that, the missing quotation mark to be added

### Forcing strict solidus parsing
JSON specification allows escaping solidus (`/`) optionally. By default, `jtc` is relaxed w.r.t. parsing solidus notation - it admits
both unescaped and escaped appearances:
```bash
bash $ <<<'{ "escaped": "\/", "unescaped": "/" }' jtc
{
   "escaped": "/",
   "unescaped": "/"
}
bash $ 
```
If there's a need for a strict solidus parsing, option `-q` facilitates the need. It also will throw an exception upon facing
a non-escaped notation:
```bash
bash $ <<<'{ "escaped": "\/", "unescaped": "/" }' jtc -q -d
.display_opts(), option set[0]: -q -d (internally imposed: )
.init_inputs(), reading json from <stdin>
.exception_locus_(), { "escaped": "\/", "unescaped": "/" }
.exception_spot_(), --------------------------------->| (offset: 33)
jtc json parsing exception (<stdin>:33): unquoted_character
bash $ 
```

### Unquoting JSON strings
If a JSON itself (or a result from walking JSON) is a single JSON string, then sometimes there's a need to unquote it
(especially it comes handy if the string itself is an embedded JSON). `-qq` allows unquoting it, here are a few examples:
```bash
bash $ jsn='"{ \"JSON\": \"example of an embedded JSON\" }"'
bash $ <<<$jsn jtc 
"{ \"JSON\": \"example of an embedded JSON\" }"
bash $ 
# unquote (jsonize) embedded json:
bash $ <<<$jsn jtc -qq
{ "JSON": "example of an embedded JSON" }
bash $ 
# unquote (jsonize) embedded json and re-parse it:
bash $ <<<$jsn jtc -qq | jtc
{
   "JSON": "example of an embedded JSON"
}
bash $
```

When unquoting empty _JSON strings_ (`""`) the resulted blank lines are not even printed:
```bash
bash $ <<<'[null, "", true]' jtc -w[:] -qq
null
true
bash $ 
```

If the source string contains Unicode code points, those will be correctly translated into
respective UTF-8 characters:
```bash
bash $ <<<'"Unicode char: \u1234"' jtc -qq
Unicode char: ሴ
bash $ 
bash $ <<<'"Surrogate pair: \uD834\uDD1E"' jtc -qq
Surrogate pair: 𝄞
bash $ 
bash $ <<<'"Invalid surrogate: \uDD1E"' jtc -qq
jtc json exception: invalid_surrogate_code_pair
bash $ 
```


> NOTE: _the option notation `-qq` will not engulf a single option notation `-q`, if both behaviors are required then both variants have
to be spelled (e.g. `jtc -q -qq`, or `jtc -qqq`)_  
> Also, `-qq` is incompatible with `-j`, `-J` options, because of a risk of forming an ill-formed JSON, thus, when sighted together
option `-qq` is silently ignored


### Stringifying JSON
An opposite request is to string-quote a JSON itself (e.g. if you like to embed JSON as a string into another JSON). This is
achieved with the option notation `-rr`:
```bash
bash $ jsn='[ "JSON", "example" ]'
bash $ <<<$jsn jtc
[
   "JSON",
   "example"
]
bash $
bash $ <<<$jsn jtc -rr
"[ \"JSON\", \"example\" ]"
bash $
```

The spacer for stringification is also controlled via `-t`, e.g.: to stringify the above JSON w/o space use:
```bash
bash $ <<<$jsn jtc -rr -t0
"[\"JSON\",\"example\"]"
bash $ 
```

> B.t.w., both _string unquoting_ and _JSON stringification_ also could be achieved via 
[template operations](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json-jsonizing-stringified).


### Summary of display options
- [`-tN`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#pretty-printing):
pretty printing using indentation with N spaces (by default `N` is `3`)
- [`-r`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#compact-printing):
compact (single row) printing using with a default spacer (1 white space) 
- [`-rtN`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#compact-printing):
compact printing using `N` white spaces as a spacer
- [`-tNc`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#semi-compact-printing):
semi-compact printing with indentation of `N` white spaces (default is `3`)
- [`-z`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#printing-json-size):
additionally print a size of an entire JSON (or of each walked element) at the bottom of a printed JSON (each walked element)
- [`-zz`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#printing-json-size):
print a size instead of a JSON (walked elements)
- [`-q`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#forcing-strict-solidus-parsing):
force parsing of a solidus `/` as strictly quoted (i.e., `\/`)
- [`-qq`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#unquoting-json-strings):
unquote printed (walked) JSON string (drop the outer quotation marks and unqote all embedded quoted characters)
- [`-rr`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json):
inquote (embed) JSON (walked elements) into a JSON string values
- [`-rrtN`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json):
inquote (embed) JSON (walked elements) into a JSON string values with a spacer `N`


## Walking JSON
Whenever there's a requirement to select (extract) only one or multiple JSON elements, a walk-path (`-w`) tells how to do it.
A walk path (an argument of `-w` option) can be made of an arbitrary number of lexemes. Though there are only 2 kinds of the
lexemes:
- offset lexeme (`[..]`)
- search lexeme (`<..>`, `>..<`)

### Walking with subscripts (offset lexemes)
Offsets are always enclosed into square brackets `[..]`. Selecting JSON elements always begins from a JSON root.
Both arrays and objects can be subscripted using numerical offsets, though it's best to utilize literal offsets to subscript objects.
E.g., let's select `address` of the 2nd (all the indices in the walk-path are zero-based) record in the above JSON:
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
or equally, it could be done like in the example below, but the former syntax is preferable (for your own good - when giving indices
you'd need to _guess_ the index of a labeled entry, which might be prone to mistakes):
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
(it makes the lexeme _iterable_), e.g., a following example prints all _names_ out of the address book, starting from the 2nd record:
```bash
bash $ <ab.json jtc -w'[Directory][+1][name]'
"Ivan"
"Jane"
bash $
```
Any JSON iterable could be subscripted that way, and any number of such lexemes could appear in the walk-path, e.g.:
```bash
bash $ <ab.json jtc -w'[Directory][+0][phone][+1][number]'
"113-123-2368"
"223-283-0372"
"333-638-0238"
bash $
```
There, all records (`[+0]`) from the `Directory` have been selected and then in every record all `phone` sub-records selected
starting from the 2nd entry (`[+1]`)

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

#### Subscript slice notation
Another way to select multiple JSON elements is to use a slice notation `[N:M:S]`. In that notation `N` and `M` could be either
positive or negative, while `S` must be strictly positive. Any of positions (as well as position separator `:`) may be omitted.

The first position (`N`) designates a beginning of the slice selection, the second position (`M`) designates an end of the  slice
exclusively (i.e., not including the indexed element `M` itself), the last position (`S`) designates a step.
- positive `N` (and `M`) refers to `N`th element offset from the _beginning_ of a collection (an array or an object)
- negative `N` (and `M`) refers to the `N`th element offset from the _end_ of the collection.
- empty (missed `N` and `M`) index tells to address either from the the beginning of the collection (in the first position),
or from the end (last position) 
- `S` position indicates a step value when iterating over the selected slice, the default value is obviously `1`

Thus, multiple notations with the same semantics are possible, e.g.:
- `[:]`, `[0:]`, `[0::]` will select all the element in the collection and is equivalent of `[+0]` notation
- `[0:1]`, `[:1]`, will select only the first element and is the same as `[0]`
- `[:-1]`, `[:-1:]` will select all the elements except the last one
- `[-2:]`, `[-2::1]` will select at most 2 last elements in the collection 
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
Walk-path lexemes enclosed into `<..>` braces instruct to perform a _recursive_ search off the value under a currently selected
JSON node. I.e., if a search lexeme appears as the first one in the walk-path, then the search will be performed off the root,
otherwise off the node in JSON where a prior lexeme has stopped.
 
By default (if no suffix is given), a search lexeme will perform a search among _JSON string_ values only (i.e., it won't match 
_JSON numerical_ or _JSON boolean_ or _JSON null_ values). E.g., following search produces a match:
```bash
bash $ <ab.json jtc -w'<New York>'
"New York"
bash $
```
while this one doesn't produce a match (the string value `"New York"` is found only in the first `Directory` record): 
```bash
bash $ <ab.json jtc -w'[Directory][1:]<New York>'
bash $
```

#### Searching JSON with RE
Optionally, search lexemes may accept _one-letter suffixes_: a single character following the lexeme's closing bracket.
These suffixes alter the search behaviors.  
There 3 suffixes that facilitate _REGEX_ types of search:
- `R`: performs a _REGEX search_ only among JSON _string_ values
- `D`: performs a _REGEX search_ only among JSON _numerical_ values
- `L`: performs a _REGEX search_ only among JSON _labels_

E.g., a following query finds recursively the first _string_ entry with the RE:
```bash
bash $ <ab.json jtc -w'<^N>R'
"New York"
bash $
```

REGEX lexemes optionally may have flags altering RE search behavior
([cppreference](https://en.cppreference.com/w/cpp/regex/syntax_option_type)):
- `I` (icase): - character matching should be performed without regard to case 
- `N` (nosubs): - sub-expressions `(expr)` are treated as non-marking sub-expressions `(?:expr)`
- `O` (optimize): - make matching faster, with the potential cost of making construction slower
- `C` (collate): - character ranges of the form "[a-b]" will be locale sensitive
> option _multiline_ is unsupported due to JSON not permitting multiline strings

Also, following REGEX grammars are supported:
- `E`: modified `ECMAScript` regular expression grammar (default)
- `S`: basic POSIX regular expression grammar
- `X`: extended POSIX regular expression grammar
- `A`: grammar used by the `awk` utility in POSIX
- `G`: grammar used by the `grep` utility in POSIX
- `P`: regular expression grammar used by the `egrep` utility

All of the above flags may be passed as _quoted trailing characters_ in the lexeme:
```bash
bash $ <ab.json jtc -w'<^new york\I>R'
"New York"
bash $ 
```
> Multiple options could be passed within the lexeme, however, if multiple grammars specified, only the first one will take the effect, 
e.g.: `<...\G\A>R` - between `awk` and `grep` grammars the latter wins, because it's given first

All REGEX lexemes also support template/namespace
[interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation). The ineterpolation is applied before
regex search performed.

_NOTE: the namespace tokens usage in REGEX lexemes is restricted to alphabetical names only (e.g.: `{abc}`):  
\- numerical namespaces (e.g., `{123}` ) might be clashing with REGEXP quantifiers and hence not supported,  
\- the auto-tokens (e.g.: '$abc') are also unsupported, because at the time of walking the iterator is yet unresolved_


#### Search suffixes
This is the complete list of suffixes that control _search_ behavior: 
  * `r`: default (could be omitted), fully matches _JSON string_ values (e.g.: `<CO>`, `<CO>r`)
  * `R`: the lexeme is a search RE, only _JSON string_ values searched (e.g.: `<^N.*>R`)
  * `P`: matches _any_ string values, same like `<.*>R`, just faster (e.g.: `<>P`)
  * `d`: matches _JSON numericals_ (e.g.: `<3.14>d`)
  * `D`: the lexeme is an RE, only _JSON numerical_ values searched (e.g.: `<^3\.+*>D`)
  * `N`: matches _any_ JSON numerical value, same like `<.*>D`, just faster (e.g.: `<>N`)
  * `b`: matches _JSON boolean_ values; to match the exact boolean, it must be spelled as `<true>b`, `<false>b`; when the lexeme is empty
  (`<>n`) it matches any boolean
  * `n`: matches _JSON null_ value (e.g.: `<>n`)
  * `l`: fully matches _JSON labels_ (e.g.: `<address>l`)
  * `L`: the lexeme is a search RE, only _JSON labels_ searched (e.g. `<^[a-z]>L`)
  * `a`: matches any JSON atomic value, i.e., _strings_, _numerical_, _boolean_, _null_ (e.g.: `<>a`)
  * `o`: matches any JSON object `{..}` (e.g.: `<>o`)
  * `i`: matches any JSON array `[..]` (e.g.: `<>i`)
  * `c`: matches any container type - either arrays or objects (e.g.: `<>c`) 
  * `e`: matches end-nodes only, which is either atomic values, or empty iterables `[]`, `{}` (e.g.: `<>e`)
  * `w`: matches any JSON value (wide range match): atomic values, objects, arrays (e.g. `<>w`)
  * `j`: matches a JSON value; the lexeme can be either a valid JSON (e.g.: `<[]>j` - finds an empty JSON array), or a
  [template](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates)
  resulting in a valid JSON after 
  [interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation)
  (e.g.: `<"{str}">j` - finds a _JSON string_ whose value is in
  [namespace](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespace)
  `str`)
  * `s`: matches a JSON value previously stored in the namespace (e.g.: `<Val>v ... <Val>s`)
  * `t`: matches a tag (label/index) previously stored in the namespace (e.g. `<Lbl>k ... <Lbl>t`)  
  * `q`: matches only original JSON values, i.e. selects non-duplicated values only (e.g.: `<>q`)  
  * `Q`: matches only repetitive (duplicating) JSON values (e.g.: `<>Q`)
  * `g`: matches all JSON values in the ascending order (e.g.: `<>g`)
  * `G`: matches all JSON values in the descending order (e.g.: `<>G`)


Some search lexemes (and
[directives](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#directives))
require their content is set and be **non-empty** (`R`,`d`,`D`,`L`,`j`,`s`,`t`,`v`,`z`,`u`,`I`,`Z`,`W`,`S`), otherwise an exception 
(_`walk_empty_lexeme`_) will be thrown, e.g.:
```bash
bash $ <ab.json jtc -w'<name>L'
"John"
bash $ <ab.json jtc -w'<>L'
jtc json exception: walk_empty_lexeme
bash $ 
```

The walk-path might be quite long containing multiple lexemes and it might not be obvious which one throws the exception. 
To figure that, run the command with `-ddd` - the throwing lexeme will be displayed:
```bash
bash $ <ab.json jtc -w'<>L' -ddd
.display_opts(), option set[0]: -w'<>L' -d -d -d (internally imposed: )
.init_inputs(), reading json from <stdin>
..ss_init_(), initializing mode: buffered_cin
..ss_init_(), buffer (from <stdin>) size after initialization: 1674
..run_decomposed_optsets(), pass for set[0]
...parse(), finished parsing json
..demux_opt(), option: '-w', hits: 1
.walk_json(), copying input json for integrity check (debug only)
..walk(), walk string: '<>L'
...parse_lexemes_(), walked string: <>L
...parse_lexemes_(), parsing here: >|
...extract_lexeme_(), parsed lexeme: <>
...parse_lexemes_(), walked string: <>L
...parse_lexemes_(), parsing here: -->|
...parse_suffix_(), search type sfx: Label_RE_search
..main(), exception raised by: file: './lib/Json.hpp', func: 'parse_suffix_()', line: 3573
jtc json exception: walk_empty_lexeme
bash $ 
```


A few of search lexemes might be left empty, but then they cary a semantic of an **empty match** (`r`,`l`):
  - `<>r` (same as `<>`) - will match an empty _JSON string_
  - `<>l` - will match an entry with the empty _JSON label_  

E.g.:
```bash
bash $ <<<'{"":"empty label"}' jtc -w'<>l'
"empty label"
bash $ 
```


The rest of the lexemes (search: `P`,`N`,`b`,`n`,`a`,`o`,`i`,`c`,`e`,`w`,`q`,`Q`,`g`,`G` and directives: `k`,`f`,`F`) also might
be left empty - all those search lexemes carry semantic of **any match**. However, if those lexemes are non-empty, then their content
points to a [_namespace_](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespace)
where the found value (result of a match - for search lexemes, or currently walked JSON - for directives) will be stored, e.g.: 
```bash
bash $ <ab.json jtc -w'<array>i1' -T'[{array}, "Sophia"]'
[
   "Olivia",
   "Sophia"
]
bash $ 
bash $ <ab.json jtc -w'<array:["Mia"]>i1' -T'[{array}, "Sophia"]'
[
   "Mia",
   "Sophia"
]
bash $ 
```


##### \* Cached Search
`jtc` is super efficient searching recursively even huge JSONs structures - normally no exponential search decay will be observed
(which is very typical for such kind of operations). The decay is avoided because `jtc` builds a cache for **_all_** searches (whenever
cacheing is required, both recursive and non-recursive) and thus all subsequent matches are taken from the cache.  


#### Directives
There are a few lexemes that look like searches, though they do not perform any matching, rather they apply certain actions
for the currently walked JSON elements, these are _directives_:

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
    without ending the walk (like _`continue`_ loop operator in some programming languages)
    * `<>FN`, where `N` is a non-zero quantifier - implements a _"jump"_ logic: it will skip over N lexemes
    starting with the `F` lexeme itself; thus `<>F1` just continues walk with the next lexeme, `<>F2` will skip over the next lexeme,
    etc.
    * `><F` - when the directive is reached, the walk successfully ends for the output processing (similar to _`break`_ loop operator)
    * `><FN`, where `N` is a non-zero quantifier - implements "_repeat"_ logic: repeats walked path N times, e.g.:
    `><F1` will produce 2 identical walk results (original one, and one repeated) 
  * `u`: user evaluation of the walk-path: the lexeme is the _`shell cli`_ sequence which affects walking: if a returned result of the
  shell evaluation is `0` (success) then walk continues, otherwise the walk fails; the lexeme is subjected for template
  interpolation
  * `I`: increment/multiply lexeme; the lexeme lets incrementing and/or multiplying the namespace value pointed by the lexeme, 
  e.g.: `<val>I3:2` - a JSON numerical stored in the namespace `val` will be incremented by 3 and then multiplied by 2.
  * `Z`: saved into a provided namespace a size of a currently walked JSON (recursive and non-recursive notations produces different
  effects - the former calculates the entire JSON size, while the latter does only the number of children); with the quantifier of `1`
  (i.e., `<StrSize>Z1`) saves into the namespace a size of the currently walked _JSON string_, otherwise (if not a string) `-1`
  * `W` saves into the provided namespace a currently walked JSON's walk-path as a _JSON array_ (e.g.: `<wp>W`)
  * `S` restores the point of walk (if it can be restored) previously saved by `W` lexeme (e.g.: `<wp>S`)


#### Setting a custom JSON value into a namespace
There's a set of lexemes (search and directives) which may reference a name in the
[_namespace_](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespace) for capturing a currently walked JSON elements:
`P`,`N`,`b`,`n`,`a`,`o`,`i`,`c`,`e`,`w`,`q`,`Q`,`g`,`G`,`v`,`k`,`f`,`F`.
All of those lexemes also allow capturing a custom JSON value in lieu of currently walked JSON - if the lexeme's value is given in
the format, e.g.:  
\- `<name:JSON_value>v`  
then upon walking such syntax the `JSON_value` will be preserved in the namespace `name` instead of a currently walked JSON
> Normally, `JSON_value` must be a valid JSON, otherwise it'll be promoted to a JSON string. If it still fails then an exception 
will be thrown (`json_lexeme_invalid`)


#### Fail-safe and Forward-Stop directives
All the lexemes in the _walk-path_ are bound by a logical `AND` - only once all succeed then the walk-path succeeds too (and printed
or passed for a respective operation). The _fail-safe_ and _Forward-Stop_ directives make possible to introduce branching logic 
into the _walk-path_. Let's break it down:

When directive `F` is paired with `<>f`, together they cover all cases of walk-paths branching:
  * ... `<>f` {if this path fails, then a walk ends reinstating the walk at `<>f` point}
  * ... `<>f` {if this path succeeds, then skip the result} `<>F` {otherwise keep walking this path starting from `<>f` point} ...
  * ... `<>f` {if this path path succeeds, then end walking} `><F` {otherwise walk this path} ...
  * ... `<>f` {if this path succeeds, then end walking} `><F <>F` # otherwise skip it (i.e., skip the failed path/result)
  * etc. (there's unlimited number of times `<>f` and `<>F`/`><F` pairs could be present in the walk)

Say, we want to list all `mobile` phone records, let's do it along with names of phone holders:
```bash
bash $ <ab.json jtc -w'[0][:][name]' -w'[0][:][phone]<mobile>[-1]' -r
"John"
{ "number": "112-555-1234", "type": "mobile" }
"Ivan"
{ "number": "223-283-0372", "type": "mobile" }
"Jane"
bash $ 
```
As it appears, `Jane` has no mobile phone, but then our requirement is enhanced: for those who do not have a `mobile`, let's list
the first available phone from the records, there a `<>f` directive comes to a rescue:
```bash
bash $ <ab.json jtc -w'[0][:][name]' -w'[0][:][phone][0]<>f [-1]<mobile>[-1]' -r
"John"
{ "number": "112-555-1234", "type": "mobile" }
"Ivan"
{ "number": "223-283-0372", "type": "mobile" }
"Jane"
{ "number": "358-303-0373", "type": "office" }
bash $ 
```
as the path is walked, as soon `<>f` directive is faced, `jtc` _memorizes_ the currently walked path point and will _reinstate_ it shall
further walking fails, there:
- we resolve the first entry in the `phone` records and memorize its path location (`[phone][0] <>f`)
- then step back up and look for a `mobile` type of the record (`[-1]<mobile>`), then:
     * if it's found, we step back up (`[-1]`) again to finish walking and display the whole record
     * if not found (i.e., walking indeed fails), a fail-safe is engaged and preserved location is recalled and displayed

> A _walk-path_ may contain multiple _fail-safe_, only the respective fail-safe will be engaged (more specific one and closest 
one to the failing point)


##### \* Examples illustrating _fail-safe_ using namespaces and interpolation:
Say we want to list from the address book all the record holders and indicate whether they have any children or not in 
this format:  
  `<Name> has children: true/false`

Thus, we need to build a single path, which will find the `name`, then inspect `children` record and transform it into
`true` if there at least 1 child, or `false` otherwise:

We can do it in steps:
1. let's get to the `name`s first and memorize those:
```bash
bash $ <ab.json jtc -w'<name>l:<N>v' 
"John"
"Ivan"
"Jane"
bash $ 
```
2. Now let's inspect a sibling record `children`:
```bash
bash $ <ab.json jtc -w'<name>l:<N>v [-1][children]' -r
[ "Olivia" ]
[]
[ "Robert", "Lila" ]
bash $ 
```
3. so far so good, now, we need to engage _fail-safe_ to facilitate the requirement to classify those records as `true` / `false`:
```bash
bash $ <ab.json jtc -w'<name>l:<N>v[-1][children]<C:false>f[0]<C:true>v' 
"Olivia"
[]
"Robert"
bash $ 
```
- there namespace `C` will be populated first with JSON value `false` and will stay put shall further walking fails;
- otherwise (i.e., upon a successful walk - addressing a first child `[0]`) the namespace `C` will be overwritten
with the value `true`

4. lastly, we need to interpolate preserved namespaces for our final / required output using a template:
```bash
bash $ <ab.json jtc -w'<name>l:<N>v[-1][children]<C:false>f[0]<C:true>v' -T'"{N} has children: {C}"' -qq
John has children: true
Ivan has children: false
Jane has children: true
bash $ 
```
##
Let's consider another example, say, we have a following JSON:
```bash
bash $ jsn='[{"ip":"1.1.1.1", "name":"server"}, {"ip":"1.1.1.100"}, {"ip":"1.1.1.2", "name":"printer"}, {"ip":"1.1.1.101"}]'
bash $ <<<$jsn jtc -tc
[
   { "ip": "1.1.1.1", "name": "server" },
   { "ip": "1.1.1.100" },
   { "ip": "1.1.1.2", "name": "printer" },
   { "ip": "1.1.1.101" }
]
bash $ 
```
How do we list only those records which **don't have** `name` and skip those which do? Well, one apparent solution then would be
to walk all those entries, which do have `name` labels and purge them:
```bash
bash $ <<<$jsn jtc -pw'<name>l:[-1]' -tc
[
   { "ip": "1.1.1.100" },
   { "ip": "1.1.1.101" }
]
bash $ 
```
But what if we want to walk entries rather than purge (e.g., for reason of template-interpolating the entries at the output)?
The prior solution would require
[chaining the output](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#Chaining-option-sets)
to the next option set (which is quite a reasonable solution too, e.g.: `<<<$jsn jtc -pw'<name>l:[-1]' / -w[:] -tc`),
however, it's possible to achieve the same using this simple query:
```bash
bash $ <<<$jsn jtc -w'[:]<>f[name]<>F' -tc
{ "ip": "1.1.1.100" }
{ "ip": "1.1.1.101" }
bash $ 
```
Without `<>F` directive at the end, the walk would look like this:
```bash
bash $ <<<$jsn jtc -w'[:]<>f[name]' -tc
"server"
{ "ip": "1.1.1.100" }
"printer"
{ "ip": "1.1.1.101" }
bash $ 
```
Thus, `<>F` skips those (successfully) matched entries, leaving only ones which fail - that's what we need in this query 
(the records which do not have `name` in it)

Now, what if in the example above (one with `<>F` directive) we want to process *failed* JSON further, say, to display `ip` only, 
rather than the whole record? That is easy - walking of the *failed* path continues past the `F` directive:
```bash
bash $ <<<$jsn jtc -w'[:]<>f[name]<>F[ip]' -qq
1.1.1.100
1.1.1.101
bash $ 
```


##### \* Uses of `Fn` directive with non-default quantifiers
there are couple other uses for `Fn` lexeme with a non-zero (non-default) quantifiers:
  - `<>Fn` - that variant of the lexeme implements a **_jump_** logic for the walk path - i.e., once walked, it will jump to the `n`th 
  lexeme (counting from the lexeme `<>F` itself) and continues walking from there.
  E.g.: `<>F1` does not do anything - it continues walking from the 1st lexeme after `<>F1`,
  `<>F2` will jump over the very next lexeme and continues walking from the 2nd one, and so on and so forth.  
  - `><Fn` - this variant will **_repeat_** the same walk up to the lexeme additionally `n` times - that is useful when there's a need to
  _repeat_ the path additionally `n` times

For example, to duplicate all the found address records, use `<>Fn`:
```bash
# find all names who's spouse is not set to null:
bash $ <ab.json jtc  -w'<spouse>l:<>f<>n<>F[-1][name]'
"John"
"Jane"
bash $ 
# duplicate the same logic once:
bash $ <ab.json jtc  -w'<spouse>l:<>f<>n<>F[-1][name]><F1'
"John"
"Jane"
"John"
"Jane"
bash $ 
```


#### RE generated namespaces
RE search lexemes (`R`, `L`, `D`) also auto-populate the namespace with following names:
- `$0` is auto-generated for an entire RE match,
- `$1` for the first RE captured subgroup,
- `$2` for the second RE subgroup, and so on (_predicated no `\N` flag was given_)
```bash
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


#### Search quantifiers
Optionally, a quantifier may follow the search lexeme (if a lexeme has a suffix, then the quantifier must come after the suffix). 
Quantifiers in search lexemes allow selecting match instance (i.e., select first match, second one, etc, or a range of matches)
Quantifiers exist in the following formats:
- `n`, - a positive number - tells which instance of a match to pick. By default, a quantifier `0` is applied
(i.e., first match selected)
- `+n` - selects all match instances starting from `n`th (zero based)
- `n:m:s` - slice select: the notation rules for this quantifier the same as for 
[subscript slices](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#subscript-slice-notation)
(`[n:m:s]`), with just one understandable caveat: `n`,`m` here cannot go negative (there's no way of knowing upfront how many
matches would be produced, so it's impossible to select a range/slice based off the last match), the rest of the notation rules apply

To illustrate the quantifiers (with suffixes), let's dump all the _JSON arrays_ in the `Directory`, except the top one:
```bash
bash $ <ab.json jtc -w'<>i1:' -tc
[ "Olivia" ]
[
   { "number": "112-555-1234", "type": "mobile" },
   { "number": "113-123-2368", "type": "mobile" }
]
[]
[
   { "number": "273-923-6483", "type": "home" },
   { "number": "223-283-0372", "type": "mobile" }
]
[ "Robert", "Lila" ]
[
   { "number": "358-303-0373", "type": "office" },
   { "number": "333-638-0238", "type": "home" }
]
bash $
```
\- the trailing `1:` in the walk path is the slice quantifier, which selects (prints) all the matches (we are matching
_JSON arrays_ here - suffix `i`) starting from second one (all quantifiers and indices are zero-based)


##### \* Search quantifiers with relative offset semantic 
There are two search lexemes types where matching non-first instance does not make sense, namely: `>..<l` and `>..<t`.
Those 
[non-recursive searches](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#non-recursive-search)
will uniquely match label or index. Indeed, in a plain _JSON array_ or an _object_ it's possible
to address only one **single** label or index, there could not be any other, e.g., in this JSON:
```bash
bash $ jsn='{ "a": 1, "b":2, "c":3, "d":4, "e":6 }'
bash $ <<<$jsn jtc -r
{ "a": 1, "b": 2, "c": 3, "d": 4, "e": 6 }
bash $ 
```
there could be only one label `"b"`, thus normally trying to match a second, third, etc instance of the label `"b"` would not
make much of a sense: `<<<'{ "a": 1, "b":2, "c":3, "d":4, "e":6 }' jtc -w'>b<l2'`

Thus, the semantic of quantifiers only in those searches was enhanced (to extend use cases) - there, the quantifiers provide a
_relative offset_ from a found label/index. So, for the notations like above: `'>b<l2'`, the label `"b"` will be matched and
then its second (successive) neighbor value will be selected:
```bash
bash $ <<<$jsn jtc -w'>b<l2' -l
"d": 4
bash $ 
```
Because of a change in semantic, those are the only search quantifiers (in the _non-recursive_ lexemes `>..<l`, `>..<t`) 
which allow negative values. Positive quantifiers let selecting next (successive) neighbors, while negative quantifiers let
selecting preceding neighbors:
```bash
bash $ <<<$jsn jtc -w'>d<l' -l
"d": 4
bash $ <<<$jsn jtc -w'>d<l-2:1' -l
"b": 2
"c": 3
"d": 4
bash $ 
```


#### Scoped search
Search lexemes perform a _recursive_ search across the entire JSON tree off the point where it's invoked (i.e., the JSON node
selected by walking all the prior lexemes). However, sometimes there's a need to limit searching scope only to the specific label.
Here is the dump of all the _JSON strings_ containing at least one digit:
```bash
bash $ <ab.json jtc -w'<\d>R:' -l
"street address": "599 Lafayette St"
"number": "112-555-1234"
"number": "113-123-2368"
"street address": "5423 Madison St"
"number": "273-923-6483"
"number": "223-283-0372"
"street address": "6213 E Colfax Ave"
"number": "358-303-0373"
"number": "333-638-0238"
bash $ 
```
Some of the values are `street address`es, some are the phone `number`s. Say, we want to dump only the phone records using
the same search criteria (lexeme). Knowing the label of the phone numbers (`"number"`), it's achievable via this notation:
```bash
bash $ <ab.json jtc -w'[number]:<\d>R:' -l
"number": "112-555-1234"
"number": "113-123-2368"
"number": "273-923-6483"
"number": "223-283-0372"
"number": "358-303-0373"
"number": "333-638-0238"
bash $ 
```
I.e., once the literal subscript lexeme is attached to the search lexeme over `:`, it makes a scoped search:  
  (`[..]:<..>`).


#### Non-recursive search
Sometimes there's a requirement to apply a _non-recursive_ search onto iterable JSON nodes (arrays, objects) - i.e., find a value
within immediate children of the node and do not descend recursively. The notation facilitating such search is the same one, but
angular brackets to be placed inside-out: `>..<`.  
To illustrate that, say, we want to find all string values in the 1st `Directory` record containing the letter `o`. 
If we do this using a recursive search, then all following entries will be found:
```bash
bash $ <ab.json jtc -w'[Directory][0]<o>R:'
"New York"
"John"
"mobile"
"mobile"
bash $
```
To facilitate our ask (find all such entries within the immediate values of the 1st record only), apply a non-recursive search notation:
```bash
bash $ <ab.json jtc -w'[Directory][0]>o<R:'
"John"
bash $
```
> **One subtle but crucial difference between _recursive_ `<..>` and _non-recursive_ `>..<` searches:  
> \- the former starts a recursive search from a currently walked/selected element itself, i.e., if a currently selected JSON is `"pi"`,
then this walk-path still matches: `<pi><pi><pi><pi>` and so on. For the latter case the _non-recursive_ search performs matching
strictly among currently walked JSON iterable's children**


### Addressing parents
One of the charming features of `jtc` is the ability to address parents (any ancestors up till the root) off the found JSON nodes.
Typically, addressing parents would be required after search lexemes (but may occur anywhere in the walk-path). Parents can
be addressed using notation `[-n]`. This feature allows building queries that answer quite complex queries.

Let's dump all the names from the `Directory` whose records have a `home` phone entry:
```bash
bash $ <ab.json jtc -w'[type]:<home>:[-3][name]'
"Ivan"
"Jane"
bash $
```
The magic which happens here (let's break down the walk-path into the lexemes):
  1. `[type]:<home>:` - this lexeme instructs to find _each_ (ending quantifier `:`) string `home` scoped by the label `"type"`
  (`[type]:` - is attached scope), thus all such phone records values will be selected:
  ```bash
  bash $ <ab.json jtc -w'[type]:<home>:'
  "home"
  "home"
   bash $
  ```
  2. `[-3]` - starting off an each found JSON element a 3rd ancestor will be selected. Let's see a parent selection in a slow-mo,
  one by one:
  ```bash
  # select 1st immediate parent:
  bash $ <ab.json jtc -w'[type]:<home>: [-1]' -tc
  { "number": "273-923-6483", "type": "home" }
  { "number": "333-638-0238", "type": "home" }
  bash $ 

  # select 2nd parent:
  bash $ <ab.json jtc -w'[type]:<home>: [-2]' -tc
  [
     { "number": "273-923-6483", "type": "home" },
     { "number": "223-283-0372", "type": "mobile" }
  ]
  [
     { "number": "358-303-0373", "type": "office" },
     { "number": "333-638-0238", "type": "home" }
  ]
  bash $ 

  # select 3rd parent:
  bash $ <ab.json jtc -w'[type]:<home>: [-3]' -tc
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
  }
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
  bash $ 
  ```
  3. `[name]` - now we can select (subscript) `[name]` of out those selected JSON nodes
##
Another example: who is the parent of a child `Lila`?
```bash
bash $ <ab.json jtc -w'<children>l:<Lila>[-2][name]'
"Jane"
bash $
```
Explanation:
1. `<children>l:` - finds each record with the label `children`
2. `<Lila>` - in the each found record find a string value `Lila`, and once/if found
3. `[-2][name]` - go 2 levels (parents) up from the found entry `"Lila"` and then subscript/offset by label `name`
##
Even more complex query: who of the parents, who have children, have mobile numbers?
```bash
bash $ <ab.json jtc -w'<children>l:[0][-2][type]:<mobile>[-3][name]'
"John"
bash $
```
\-  that is the only correct answer (because Ivan has no children and Jane has no `mobile` phone)

The walk-path break down:
1. `<children>l:` - find each record by label `"children"`
2. `[0]` - try addressing first element in the found records (that'll ensure that `children` is non-empty)
3. `[-2]` - go 2 parents up for those records which survived the prior step - that'll bring us to the person's record level 
4. `[type]:<mobile>` - find recursively `mobile` string scoped by `type` (already only for those records which have children)
5. `[-3]`-  go 3 levels (parents) up (for those records which have `children` and have `mobile` types of phone records) - it'll bring
us again up to the person's record level
6. `[name]` - finally select the name


#### Addressing parents offsetting from root
There's another way to address parents - through `[^n]` notation, compare: the following walk-path achieves exactly the same ask:
```bash
bash $ <ab.json jtc -w'<children>l:[0][^2][type]:<mobile>[^2][name]'
"John"
bash $
```
Note `[^2]` - this notation, likewise `[-n]` also selects a certain parent, however, while `[-n]` select the parent off the leaf
(i.e., from the currently selected node) `[^n]` notation does it off the root.

When `jtc` walks lexemes (traverse JSON tree), internally it maintains a path to the walked steps (it's visible via debugs `-dddd`).
E.g., when the first lexeme's match found (for `<children>l:`), the internal walked steps path would look like:  
 `root -> [Directory] -> [0] -> [children]`,  
then when the next lexeme is successfully applied, the internal path becomes:  
 `root -> [Directory] -> [0] -> [children] -> [0]`  
The meaning of `[-n]` and `[^n]` notation then is easy to observe on this diagram:
```
                                                                             etc.
                                                                             [^5]
to address a parent from the root: [^0]      [^1]       [^2]      [^3]       [^4]
                                    |          |          |         |          |
                                    v          v          v         v          v
           internally built path: root -> [Directory] -> [0] -> [children] -> [0]
                                    ^          ^          ^         ^          ^
                                    |          |          |         |          |
to address a parent from the leaf: [-4]      [-3]       [-2]      [-1]       [-0]
                                   [-5]
                                   etc.                         
```


### Walking multiple walk-paths
`jtc` allows a virtually unlimited number of walk-paths (`-w`), it would be limited only by the max size of the accepted string in
your shell. Though there are a few tweaks in `jtc` which let harnessing the order of displaying resulted walks. By default `jtc`
will be displaying resulted successful walks in an _interleaved_ manner, but first, let's take a look at


#### Sequential walk processing
option `-n` ensures that all given walk-paths (`-w`) will be processed (and printed) sequentially in the order they given:
```bash
# dump names first and then phone numbers:
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
# dump names first the phone numbers and then the names:
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
if resulted walks have labels in the input JSON (i.e., they were inside _JSON objects_), then `-l` lets dumping their labels too:
```bash
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
`-j` does a quite simple thing - it wraps all walked entries back into a _JSON array_, however, predicated by `-l` and `-n` options
the result will vary:
- `-j` without `-l` will just arrange walked entries into a JSON array:
  ```bash
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
- once `-n`,`-j` and `-l` given together, then entries which have labels be wrapped into own objects
(array items won't be wrapped into objects):
  ```bash
  bash $ <ab.json jtc -w'<name>l:' -w'<number>l:' -tc -njl
  [
     { "name": "John" },
     { "name": "Ivan" },
     { "name": "Jane" },
     { "number": "112-555-1234" },
     { "number": "113-123-2368" },
     { "number": "273-923-6483" },
     { "number": "223-283-0372" },
     { "number": "358-303-0373" },
     { "number": "333-638-0238" }
  ]
  bash $ 
  ```
Though even that behavior is influenced by the `-n` option, the above output looks dull and hardly will have many use-cases, a lot more
often it's required to group relevant walks together and then place them into respective JSON structures. For that, let's review


#### Interleaved walk processing
Interleaved walk processing (and outputting) occurs by default, though there's a certain way to control it. Let's take a look at the
above outputs dropping the option `-n` (i.e., print walks _interleaved_):
```bash
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
Those look interleaved, though it does not appear that they relate to each other properly: e.g.: a number `"113-123-2368"`
belong to `"John"` and preferably should be displayed before `"Ivan"` and so does apply to others. `jtc` is capable of 
processing/printing relevant entries, though it needs a little hint from the walk-paths: the latter supposed to express the
relation between themselves.

Right now both paths (`<name>l:` and `<number>l:`) do not have common base lexemes, thus it's unclear how to relate resulting walks
(hence they just interleaved one by one). Though if we provide walk-paths relating each of those searches to their own record,
then the magic happens:
```bash
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
And now, applying options `-j` together with `-l` gives a lot better result (we achieve grouping of relevant walks):
```bash
bash $ <ab.json jtc -w'[Directory][:]<name>l:' -w'[Directory][:]<number>l:' -jl -tc
[
   {
      "name": "John",
      "number": [ "112-555-1234", "113-123-2368" ]
   },
   {
      "name": "Ivan",
      "number": [ "273-923-6483", "223-283-0372" ]
   },
   {
      "name": "Jane",
      "number": [ "358-303-0373", "333-638-0238" ]
   }
]
bash $ 
```
B.t.w., the relation between walks could be also expressed in a _relative_ (rather than via _absolute_, as above) path way:
```bash
bash $ <ab.json jtc -w '<name>l:' -w'<name>l:[-1]<number>l:'
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
# and now groping relevant walks:
bash $ <ab.json jtc -w '<name>l:' -w'<name>l:[-1]<number>l:' -jl -tc
[
   {
      "name": "John",
      "number": [ "112-555-1234", "113-123-2368" ]
   },
   {
      "name": "Ivan",
      "number": [ "273-923-6483", "223-283-0372" ]
   },
   {
      "name": "Jane",
      "number": [ "358-303-0373", "333-638-0238" ]
   }
]
bash $ 
```
>Note: such grouping is only possible with labeled values (obviously), it won't be possible to group array elements that easily,
> e.g., let's break array into pairs:
>```bash
>bash $ array='[0,1,2,3,4,5,6,7,8,9]'
>bash $ <<<$array jtc -w[::2] -w[1::2] -j -tc
>[ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ]
>bash $ 
>```
> it won't work even if we try relating walks:
>```bash
>bash $ <<<$array jtc -w[::2] -w'[::2]<I>k[-1]>I<t1' -j -tc
>[ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ]
>bash $ 
>```
>Thus grouping here should be achieved differently. One way is to use only a single walk collecting required elements of the group into
>the namespaces and then using template interpolating the latter:
>```bash
>bash $ <<<$array jtc -w'[::2]<I>k<V>v[-1]>I<t1' -T'[{{V}},{{}}]' -j -tc
>[
>   [ 0, 1 ],
>   [ 2, 3 ],
>   [ 4, 5 ],
>   [ 6, 7 ],
>   [ 8, 9 ]
>]
>bash $
>```
>Another way is to transofrm the walks into objects assigning labels from the first walk's index:
>```bash
>bash $ <<<$array jtc -w'[::2]<I>k' -w[1::2] -T'{"{I}":{{}}}' -ll / -tc
>{
>   "0": [ 0, 1 ],
>   "2": [ 2, 3 ],
>   "4": [ 4, 5 ],
>   "6": [ 6, 7 ],
>   "8": [ 8, 9 ]
>}
>bash $ 
>```
>and then re-walk dropping labels and encapsulating into the outer array:
>```bash
>bash $ <<<$array jtc -w'[::2]<I>k' -w[1::2] -T'{"{I}":{{}}}' -ll / -jw[:] -tc
>[
>   [ 0, 1 ],
>   [ 2, 3 ],
>   [ 4, 5 ],
>   [ 6, 7 ],
>   [ 8, 9 ]
>]
>bash $ 


#### Aggregating walks
the walks results that have labels also could be aggregated (per label), option `-nn` facilitates the ask:
```bash
bash $ <ab.json jtc -w'[Directory][:]<name>l:' -w'[Directory][:]<number>l:' -jlnn
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


#### Wrapping walked entries into a JSON object
While `-j` wraps walks into a _JSON array_, `-jj` does into a _JSON object_. All the elements in JSON object must have labels,
thus any walked elements which do not have labels (i.e., elements in _JSON array_ and root) will be ignored.

E.g., let's dump all values from `Jane`'s record and wrap them all into an object:
```bash
bash $ <ab.json jtc -w'<Jane>[-1]<>a:' -jj
{
   "age": 25,
   "city": "Denver",
   "name": "Jane",
   "number": "333-638-0238",
   "postal code": 80206,
   "spouse": "Chuck",
   "state": "CO",
   "street address": "6213 E Colfax Ave",
   "type": "home"
}
bash $ 
```
Well, the above output though does not keep all the atomic entries from the Jane's record (e.g.: Jane has 2 phone numbers, while only
one is displayed). That is because clashing labels will override each other (as of version _1.75_). To ensure aggregation of clashing 
labels, use `-m` option:
```bash
bash $ <ab.json jtc -w'<Jane>[-1]<>a:' -jjm
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
that is because they are enlisted in _JSON array_ and therefore have no labels (and hence ignored in `-jj` option, which considers
values with labels only).


#### Extracting labeled values
Sometimes, when displaying outputs wrapped into an object, it's desirable to extract the labeled value from the object
(i.e., reach inside the object and use inner label rather than outer). This become especially handy when dealing with templates.

Let's consider a following query:  
Say, the ask here is to extract all names of all the people from `ab.json` and group them with newly crafted record indicating if a person
has children or not, like this:
```bash
[
   { "name": "John", "has children": "yes" },
   ...
]
```

1\. Extracting names is easy:
```bash
bash $ <ab.json jtc -w'<name>l:'
"John"
"Ivan"
"Jane"
bash $ 
```

2\. Crafting a new record would require knowing 
[templates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates),
[namespace](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespace) and
[interpolation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation)
, for now let's just construct a walk which creates the required namespace:
```bash
bash $ <ab.json jtc -w'<name>l:' -w'<children>l: <C:no>f[0]<C:yes>v'
"John"
"Olivia"
"Ivan"
[]
"Jane"
"Robert"
bash $ 
```
\- the second walk above features couple concepts:
- [branching](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#fail-safe-and-forward-stop-directives)
(`<..>f`) _fail-safe_ lexeme: ensures that the walk is reinstated at the placement of the lexeme if/once the 
subsequent walk fails
- [namespaces](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespace)
(`<C:no>f`, `<C:yes>v`): both lexemes setup the _namespace_ `C`, initially to value `"no"` then to value `"yes"`; 
the latter value will override the former only if walking `[0]` was successful (i.e., if a person indeed has at least one child,
b/c if array `children` were empty, that walk would fail)

3\. by now, each time when second walk finishes iteration, the namespace `C` should be correctly populated with the respective values 
reflecting if a person has children or not, but to see that, we'd need to interpolate that namespace using a template:
```bash
bash $ <ab.json jtc -w'<name>l:' -w'<children>l: <C:no>f[0]<C:yes>v' -TT -T'{"has children": {{C}}}' -tc
"John"
{ "has children": "yes" }
"Ivan"
{ "has children": "no" }
"Jane"
{ "has children": "yes" }
bash $ 
```
4. okay, we're getting closer, but now we want to display all records with labels:
```bash
bash $ <ab.json jtc -w'<name>l:' -w'<children>l:<C:no>f[0]<C:yes>v' -T'{"has children": {{C}}}' -l
"name": "John"
jtc jnode exception: label_accessed_not_via_iterator
bash $ 
```
Bummer! The exception (rightfully) occurs here because trying to find an outer label (with `-l`) of an interpolated JSON 
`{ "has children": "yes" }` fails - indeed it's a standalone JSON, and root does not have any label attached - hence the 
exception. In the situations like this, we'd rather want to reach out inside the object for a labeled value rather than 
finding an outer label. The option `-ll` facilitates that requirement:
```bash
bash $ <ab.json jtc -w'<name>l:' -TT -w'<children>l: <C:no>f[0]<C:yes>v' -T'{"has children": {{C}}}' -llj -tc
[
   { "has children": "yes", "name": "John" },
   { "has children": "no", "name": "Ivan" },
   { "has children": "yes", "name": "Jane" }
]
bash $ 
```
Finally, what's `-TT` in there? That's a dummy template (one which surely fails). It's needed because if it wasn't here, then 
a single template would apply to both walks (and we don't want our template to apply onto the first walk). So, we'd rather
provide a dummy one so that each 
[template would relate to own walk](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#multiple-templates-and-walks).
If template fails (and `-TT` surely does) then no interpolation applied and walk iteration result is used as it is.

If that dummy template discomforts you, there's a way to go without one: keep in mind that template gets applied only once it
results in a legit JSON after the interpolation occurs (shall one occurs). Let's see what the result would be w/o that dummy template:
```bash
bash $ <ab.json jtc -w'<name>l:' -w'<children>l: <C:no>f[0]<C:yes>v' -T'{"has children": {{C}}}' -tc
"John"
{ "has children": "yes" }
{ "has children": "yes" }
{ "has children": "no" }
{ "has children": "no" }
{ "has children": "yes" }
bash $ 
```
Now, the same template (`-T'{"has children": {{C}}}'`) gets applied to each walk, and while for the 1st walk the template
interpolation definitely fails (while walking the 1st walk iteration for the 1st time, the name `C` does not exist yet in
the namespace) and therefore the very 1st walk is getting displayed as it is, for all the subsequent iterations of the 1st
walk the template interpolation would succeed (because  the namespace `C` now holds the value from the 2nd walk) and therefore
unwanted template substitution occurs.  
To fix that problem easily, we can erase the namespace `C` right at the beginning of the 1st walk:
```bash
bash $ <ab.json jtc -w'<C>z<name>l:' -w'<children>l: <C:no>f[0]<C:yes>v' -T'{"has children": {{C}}}' -tc
"John"
{ "has children": "yes" }
"Ivan"
{ "has children": "no" }
"Jane"
{ "has children": "yes" }
bash $ 
```
And adding `-llj` provides the desired effect now:
```bash
bash $ <ab.json jtc -w'<C>z<name>l:' -w'<children>l: <C:no>f[0]<C:yes>v' -T'{"has children": {{C}}}' -tc -llj
[
   { "has children": "yes", "name": "John" },
   { "has children": "no", "name": "Ivan" },
   { "has children": "yes", "name": "Jane" }
]
bash $ 
```

> all the above examples just illustrate capabilities of the options for instructional purpose. Practically,
the same ask would be easier to achive using just a single walk:  
>```bash
>bash $ <ab.json jtc -w'<name>l:<N>v[-1][children]<C:no>f[0]<C:yes>v' -T'{"name":{{N}}, "has children": {{C}}}' -jtc
>[
>   { "has children": "yes", "name": "John" },
>   { "has children": "no", "name": "Ivan" },
>   { "has children": "yes", "name": "Jane" }
>]
>bash $ 
>```


#### Succinct walk-path syntax
If you look at the prior example
([Aggregating walks](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#aggregating-walks)),
you may notice that a common part of both walk-paths (`[Directory][:]`) had been given twice. There's a way to express it in a more 
succinct syntax: options `-x` and `-y` allow rearranging walk-paths so that `-x` takes an initial common part of the walk-path, 
whereas `-y` would take care of the varying trailing pars. Thus the same example cold have been written like this:
```bash
bash $ <ab.json jtc -x'[Directory][:]' -y'<name>l:' -y'<number>l:' -jlnn
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
```
\- each occurrence of `-x` will be reconciled with all subsequent `-y` (until next `-x` is faced). Options `-x`, `-y` is merely
a syntactical sugar and do not apply any walk-path parsing or validation, instead they just reconcile into respective `-w` options 
created internally, then the latter get processed. Thus, it's even possible to write it with what it seems a broken syntax at first:
```bash
bash $ <ab.json jtc -x'[Directory][:' -y']<name>l:' -y']<number>l:' -jlnn
...
```
However, if a reinstatement of the options results in a valid walk-path - that's all what matters.

It's possible to combine both syntaxes (e.g.: `-w` with `-x` and `-y`), however, given that the processing of `-x` and `-y`
internally reinstates respective options `-w`, the former will be appended after any of given `-w` options (which will affect the 
order of processing/outputting) even though the order of their appearance is different:
```bash
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
the entire JSON will be displayed). However, there's a way to control which ones are displayed - option `-x` is overloaded to provide
such capability.

If argument of option `-x` is given in any of notations: `-xn`, `-xn/N`, `-x/N` - where `n` and `N` are numbers, then it controls
a frequency and offset of the displayed walks (and then `-x` does not represent a common portion of a walk-path).  
The first number `n` in that notation tells to display every `n`th walk. if `n` is `0` it tells to display `N`th walk once (and in such
case `0` - a default value - can be omitted resulting in the syntax `-x/N`)
The second (optional) number `N` tells to begin displaying walks starting from `N`th one (`N` is an index and thus is zero based, default
value is `0`).

Both `n` and `N` are generally positive numbers, though there's a special notation `-x/-1` (or equally `-x0`) - in such case 
_the last walk_ is _ensured_ to be displayed (only if it wasn't displayed yet)

Say, we want to display _every 4th walk_ of the below JSON:
```bash
bash $ jsn='[1,2,3,4,5,6,7,8,9]'
bash $ <<<$jsn jtc -w[:]
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
```bash
bash $ <<<$jsn jtc -w[:] -x4
4
8
bash $ 
 ```
To display _every 4th walk starting from 3rd one_, use this notation:
```bash
bash $ <<<$jsn jtc -w[:] -x4/2
3
7
bash $ 
```
> remember: the second number in the option is an index and thus is zero based

Lets add to the output the very first walk and the last one:
```bash
bash $ <<<$jsn  jtc -w[:] -x4/2 -x/0 -x/-1
1
3
7
9
bash $ 
```


### Summary of walk lexemes
- [`[]`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-with-subscripts-offset-lexemes):
matches an _empty label_ in a currently walked JSON element (e.g.: like in `{ "": "empty label" }`)
- [`[text]`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-with-subscripts-offset-lexemes):
matches an _exact label_ in a currently walked JSON element (e.g.: like in `{ "text": "is a label" }`)
- [`[N]`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-with-subscripts-offset-lexemes):
matches _`N`th_ child (all indices are zero based in `jtc`) in a currently walked JSON _iterable_
- [`[-N]`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#addressing-parents):
selects _`N`th_ parent for a currently walked JSON node/element referring from the node itself
- [`[^N]`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#addressing-parents-offsetting-from-root):
selects _`N`th_ parent for a currently walked JSON node though referring the parent from the JSON root (rather than from a node)
- [`[+N]`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#selecting-multiple-subscripted-json-elements):
matches _every child_ in a currently walked JSON _iterable_ starting from _`N`th_ child
- [`[N:M:S]`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#subscript-slice-notation):
matches _every child_ in a currently walked JSON _iterable_ starting from _`N`th_ child up till, but not including _`M`th_
with the step of _`S`_ (`N`,`M` could be negative defining the start/end of the slice from the end of the _iterable_ rather than from
the start, while _`S`_ can only be positive); either of positional parameters could be omitted (e.g.: 
`[:]`, `[::]`, `[1:]`, `[:-2]`, `[::3]`, etc)
- [`<>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#searching-json):
finds recursively the first _empty string_ (e.g.: like in `{ "empty string": "" }`)
- [`<text>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#searching-json):
finds _recursively_ the first string _`"text"`_ (e.g.: like in `[ "text" ]`)
- [`<..>X`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-suffixes):
_`X`_ is an optional one-letter suffix altering the behavior of the lexeme:  
  * if _`X`_ is any of _`rRPdDNbnlLaoicewjstqQgG`_ - then it's a _search_ matching a first occurrence of the lexeme, as per the 
  suffix description      
  * if _`X`_ is any of _`vkzfFuIZWS`_ then it's a _directive_ and applies the respective behavior as per the suffix description
- [`<text>n`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers):
finds recursively _`n`th_ occurrence of _`"text"`_ in a currently walked JSON element
- [`<text>+n`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers):
finds recursively _each occurrence_ of _`"text"`_ in a currently walked JSON element starting from _`n`th_ occurrence
- [`<text>n:m:s`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers):
finds recursively _each occurrence_ of `"text"` in a currently walked JSON element for the selected slice,
where _`n`_,_`m`_,_`s`_ parameters comply with all `[N:M:S]` rules with an additional limitation: _`n`_ and _`m`_ cannot go negative
and one additional liberation: either of parameters _`n`_,_`m`_,_`s`_ could be _interpolated_ from the namespace
- [`<..>Xn`, `<..>X+n`, `<..>Xn:m:s`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#search-quantifiers): 
  * if _`X`_ is a search lexeme suffix, then quantifier notations apply exactly the same way as per in above `text` search quantifiers
  * if _`X`_ is a directive lexeme suffix, then the quantifier behavior is either ignored (like in directives `vkzW`), 
  or is specific for the given directive (refer to the relevant description of directives: `fFuIZ`)
- [`[label]:<..>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#scoped-search):
_scoped_ recursive search - the search and match is performed in the currently selected JSON element only _among 
values with the specified label_; directives lexemes ignore scoping; all search suffixes and quantifiers are applicable in the scoped
searches
- [`>..<`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#non-recursive-search):
a _non-recursive_ search notation - the search is performed strictly _among children_ of the currently selected JSON _iterable_
without any descend into children's children; all search suffixes and quantifiers applied here the same way, with the following
suffixes exemptions:
  * `>NS<sn`: _`NS`_ is the referred namespace being searched and matched against in the currently selected _JSON iterable_
  * [`>NS<tn`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#-search-quantifiers-with-relative-offset-semantic):
  the same definitions applies as above, but the match is performed agains a label (for objects), or index (for arrays),
  plus this lexeme cannot have a _scoped_ notation (obviously), however,
  a quantifier _`n`_ here refers to a relative offset from the found match (hence _`n`_ may be negative, effectively allowing selecting
  a _sibling_ of the found element)
  * [`>..<l`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#-search-quantifiers-with-relative-offset-semantic):
  the _non-recursive_ exact label match _cannot_ be scoped as well, however _`n`_ also may go negative here
  * [`>..<L`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#searching-json-with-re):
  the _non-recursive_ REGEX label match _cannot_ be scoped as well
- [`<NS>X`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#setting-a-custom-json-value-into-a-namespace):
some search lexemes and some directives allow _capturing_ a currently walked/matched JSON into a namespace _`NS`_:
  * if _`X`_ is a suffix any of _`PNbnaoicewqQgGvfF`_, then the namespace _`NS`_ will be populated upon a successful match (for searches)
  or upon walking (for directives)
  * for the rest of the searches (_`rRdDlLjst`_), the lexeme defines a _search context_ (rather than the namespace reference), 
  * for  for the rest of directives (_`kzuIZWS`_) the behavior varies - refer to a respective directive description
- [`<NS:JSON_value>X`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#setting-a-custom-json-value-into-a-namespace):
the same searches and directives allowing _capturing_ JSON values, allow setting custom _`JSON_value`s_ 
_instead_ of capturing (the same rules apply)


### Summary of walk options
- [`-w`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#walking-json):
defines a _walk-path_ (multiple walks supported), by default the results produced from multiple walks are _interleaved_
- [`-l`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#displaying-walks-with-labels):
when used w/o `-j` just _prints labels_ for those walked JSONS which have one
- [`-ll`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#extracting-labeled-values):
when used w/o `-j` prints all JSON objects _"naked"_, i.e. _removes JSON object encasement_ and then uses _inner_ labels; 
for the rest of JSON types the behavior is the same like with the single `-l`
- [`-n`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#sequential-walk-processing):
turns on a _sequential_ behavior for walk-paths (process first all the results from the first walk, then from then second, etc),
- [`-nn`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#aggregating-walks):
when used together with `-j` and `-l` allows _aggregated_ behavior for values with clashing labels, see below
- [`-nn`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#multiple-templates-and-walks):
also triggers a _round-robin templates_ application when a number of templates (`-T`) and walks (`-w`) is the same but 
the _round_robin_ template application must be favored over the _per-walk_'s in this situation
- [`-j`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-resulted-walks-to-json-array):
arranges all walked elements (from all walk-paths) into a _JSON array_
- [`-jl`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-resulted-walks-to-json-array):
arranges all walked elements (from all walk-paths) into a _JSON array_, puts labeled nodes into _separate JSON objects_, 
any clashing labels will be aggregated (into _JSON arrays_) within those objects
- [`-jln`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-resulted-walks-to-json-array):
arranges all walked elements into a _JSON array_, each labeled element will be placed into _own JSON object_, thus dodging
possible label clashing
- [`jlnn`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#aggregating-walks):
arranges all walked elements into a _JSON array_, _all labeled values_ placed into a _single JSON object_, all clashing
labels are aggregated
- [`-jj`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-walked-entries-into-a-json-object):
arranges all walked elements into a _JSON object_ (i.e. all walked elements which do not have labels will be ignored), the
values with clashing labels will _override the prior_ values (_note:_ `-l`,`-n` and `-nn` with `-jj` have no effect)
- [`-jjm`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#wrapping-walked-entries-into-a-json-object):
alters behavior of `-jj` by enabling the aggregation of the values with the clashing labels into _JSON arrays_ 
- `-jjll`: combined behavior of `-jj` and `-ll`
- `-jjllm`: combined behavior of `-jj` and `-ll` and `-m`
- [`-x`,`-y`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#succinct-walk-path-syntax):
facilitates breaking walk-paths (`-w`) into a common (`-x`) and variables parts (`-y`); i.e. an argument of each `-x` will be
combined with every subsequent argument of `-y` (if any), e.g.: `-xA` `-yB` `-yC` `-xD` `-yE` `-yF` result in 4 walk-paths:
`-wAB` `-wAC` `-wDE` `-wDF` - thus `-x`/`-y` notation provides more succinct notation (than `-w`) for multiple walk-paths with 
a common head (and varying tails)
- [`-xN/M`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#controlling-displayed-walks):
controls which walk result(s) get(s) printed: print each _`N`th_ walk starting with _`M`th_ (_`M`_ is an index and hence is
zero based); `-xN` prints every _`N`th_ walk result (same as `-xN/M`, where `M`=`N`-`1`), `-x/M` prints a single _`M`th_ walk
(same as `-x0/M`); special handling for `-x0` or `-x/-1` - it _ensures_ that the very last walk result is always gets printed
(but not duplicated)


## Interpolation
Interpolation may occur in any of the following cases:
- in templates (`-T ...`) for walk/update/insert/compare type of operations
- for the argument undergoing a
[shell evaluation](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation)
(`-e` + insert/update operation, e.g.: `-eu ... \;`),
- in the lexeme `<..>u` applying a shell evaluation on its content
- in the lexeme `<..>j` where lexeme is expressed as a template
- for any search lexeme where a quantifier is expressed through interpolation, e.g.: `<>a{N}`

Interpolation occurs either from the namespace, or from a currently walked JSON element. Every occurrence of tokens `{..}` or `{{..}}`
will trigger the interpolation attempt:
- if the content under braces is empty (`{}`, `{{}}`) then the currently walked JSON element is getting substituted
- if the content is non-empty (e.g.: `{val}`, `{{val}}`) then interpolation is attempted from the referred namespace

Whenever template interpolation (`-T ...`, `<..>j`) occurs, the result of interpolation must always be a valid JSON,
otherwise the template will fail to get applied


### Interpolation token types
The difference between single `{}` and double `{{}}` notations: 
- double notation (a.k.a. _dressed notation_) `{{..}}` interpolates JSON elements exactly, so it's always a safe type
- a single notation (a.k.a. _naked notation_) `{..}` "strips" the interpolated object and interpolates
_JSON strings_, _JSON arrays_, _JSON objects_ differently:
  * when interpolating _JSON string_, the outer quotation marks are dropped, e.g., instead of `"blah"`, it will be interpolated as 
  `blah`. Thus, it makes sense to use this interpolation inside double quotation marks
  * when interpolating _JSON array_, then enclosing brackets `[`, `]` are dropped (allows extending arrays); e.g., `[1,"2",{}]` 
  will be interpolated as `1,"2",{}` (which is invalid JSON), thus, to keep it valid the outer brackets must be provided,
  e.g.: `-T'[ 0, {}, 4, [] ]'` after becomes `[ 0, 1, "2", {}, 4, [] ]`
  * when interpolating _JSON object_, then enclosing braces `{`, `}` are dropped (allows extending objects), e.g., `{"pi":3.14}` 
  will be interpolated as `"pi": 3.14`, so to keep it valid the outer braces must be provided, e.g., `-T'{ {}, "type": "irrational" }'`
  * if you meant to insert in a template an empty object `{}` rather than a _naked_ token (which has the same notation), 
  then spell it over space: `{ }`

#### String interpolation
A _string_ interpolation using a _naked notation_ is handy when there's a requirement to alter/extend an existing string,
here's example of altering JSON label:
```bash
bash $ jsn='{ "label1": "value1", "label2": "value2" }'
bash $ <<<$jsn jtc -w'<label>L:<>k' -u'<label>L:<>k' -T'"new {}"'
{
   "new label1": "value1",
   "new label2": "value2"
}
bash $ 
```
There, in the example above, the template token `{}` refers to the result of walking `-u` rather than `-w` (the same holds
true for insert `-i` and compare `-c` operators: themplate operation (`-T`) refers to the option argument rather than to the walk).
The walk `-w` points to the destination location(s) for the update (which is the label,  as per description of the lexeme `<>k`),
while source is pointed by `-u` walk.  
Alternatively, the same could be achieved like this, in a bit more succinct way:
```bash
bash $ <<<$jsn jtc -w'<label>L:<T>k<>k' -u0 -T'"new {T}"'
{
   "new label1": "value1",
   "new label2": "value2"
}
bash $ 
```
The update argument (`-u0`) in this case is a dummy JSON (`0` is taken just as one of the shortest JSON values), its value will be
unused in the template and hence is irrelevant to the operation. The template now will refer to the value `T` from  the namespace,
which will be  populated when destination `-w` is walked (which, btw, always occurs before any other walks, as per design logic).  
Why `<..>k` lexeme is used twice? As per the lexeme design, when it's empty (and only then) it lets reinterpreting currently walked 
element's label as a value (which is being updated in this case), Thus, the first lexeme `<L>k` only preserves the label in the
namespace, while the second lexeme allows pointing to the label as the destination point for the update - which will be the
template-interpolated JSON value `'"new label"'`.  
  
Starting from `jtc` _`v1.176`_, it's possible to achive the same interpolation even in a more efficient and laconic way:
```bash
bash $ <<<$jsn jtc -w'<label>L:<>k' -u'"new {}";'
{
   "new label1": "value1",
   "new label2": "value2"
}
bash $ 
```
options `-u`/`-i`/`-c` may accept now an argument in various forms: _JSON_, _walk_, _template_. In the latter case the template
token `{}` will refer to the walk (`-w`) result.  
\- Why trailing `;` is there?  
\- It boils down to how argument disambiguation for the options occurs:
among various forms of an argument (_JSON_, _walk_, _template_) it's possible that the argument semantic can be ambigous.
In our case, if the template string was like `"new {}"` it would be impossible to tell if it's a _JSON_ argument, or a _template_
(indeed, in your great design `{}` might represent either a token, or an empty object, or just a literal value - there's no way for `jtc`
to know that).  
So, `jtc` needs a little hint from you how to treat that argument (in case if there's an ambiguity).
_JSON_ argument does not tolerate any trailing symbols, while _template_ argument disregards any trailing symbols after
interpolation and parsing. Thus symbol `;` only ensures that the argument is treated as a _template_ rater than a _JSON_ (in fact,
it could be any combinations of any trailing symbols)


Here's an illustration when a _naked notation_ is required:
```bash
bash $ jsn='{ "pi": 3.14, "type": "irrational" }'
bash $ <<<$jsn jtc
{
   "pi": 3.14,
   "type": "irrational"
}
bash $ 
# swap around values and labels:
bash $ <<<$jsn jtc -i'[:]<Key>k<Val>v' -T'{ "{Val}": {{Key}} }' -p
{
   "3.14": "pi",
   "irrational": "type"
}
bash $ 
```
there, values getting into the namespace `Val` will be different types in each pass: the first time it's a numeric value `3.14`,
in the second pass it'll be a string `"irrational"`. Therefore, in the template, where `Val` is used with the label semantic,
we have to ensure that the interpolation occurs of the naked value.  

Let's consider both scenarios where interpolation of the namespace `Val` uses a _dressed_ notation:

  - `<<<$jsn jtc -rpi'[:]<Key>k<Val>v' -T'{ {{Val}}: {{Key}} }'` results in `{ "irrational": "type" }` - the interpolation of `{{VAL}}`
for the first record fails here, it becomes `{ 3.14: "pi" }`, which is invalid JSON and hence not getting inserted (but purged)
  - `<<<$jsn jtc -rpi'[:]<Key>k<Val>v' -T'{ "{{Val}}": {{Key}} }'` results in `{ "3.14": "pi" }` - the interpolation of `"{{VAL}}"`
for the second record fails here - it becomes `{ ""irrational"": "type" }` - which is also an invalid JSON

The `Key` token notation could have been spelled either way, e.g.: `"{Key}"` - would work as well. 


#### Interpolation of iterables
An array interpolation using a naked notation (`{..}`) is handy when there's a requirement to extend the array during
template interpolation. 
There's a special case though - template-extending of an empty array. Let's consider a following example:
```bash
bash $ jsn='[ {"args": [123],"Func": "x + y"}, { "args":[], "Func":"a * b" }  ]'
bash $ <<<$jsn jtc
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
And the ask here would be to extend all arrays in each `args` with the arguments from the respective `Func`:
```bash
bash $ <<<$jsn jtc -w'[:][args]' -u'[:][Func]<(\w+)[ +*]+(\w+)>R[-1][args]' -T'[{}, {{$1}}, {{$2}}]' -tc
[
   {
      "Func": "x + y",
      "args": [ 123, "x", "y" ]
   },
   {
      "Func": "a * b",
      "args": [ "a", "b" ]
   }
]
bash $ 
```
When interpolating the second record (the one with `"Func": "a * b"`), the interpolation in fact, would result in the invalid JSON:
the _array_ in `args` is empty initially (`"args": []`), thus when it gets interpolated via template `[{}, {{$1}}, {{$2}}]` it
becomes `[, "a", "b"]` - which is an invalid JSON array. However, `jtc` is aware of such empty iterables and handles them properly, 
allowing extending even empty arrays and objects without producing failures.

All the same applies when interpolating _JSON objects_ and _JSON strings_.


##### \* Interpolation of iterables into a string template
An array could be converted into a string _literally_ using a _dressed_ token notation as long it does not hold any string literals,
e.g.:
```bash
bash $ <<<'[null, true, 123, {}, []]' jtc -T'"array stringified: {{}}"'
"array stringified: [ null, true, 123, {}, [] ]"
bash $ 
```
\- presence of any string value (or a label) would fail the interpolation (b/c of quotation marks)

Any _iterable_ can be interpolated into a string template using the _naked_ token notation: then _all its atomic_ values 
get _recursively_ enumerated within the string:
```bash
# array:
bash $ jsn='{"array":[null,1,true,{"root":["five"]}]}'
bash $ 
bash $ <<<$jsn jtc -w[array] -T'"stringified array: {}"'
"stringified array: null, 1, true, five"
bash $ 

# object:
bash $ <<<$jsn jtc -T'"stringified object: {}"'
"stringified object: null, 1, true, five"
bash $ 
```
Note, string values (`"five"`) also get naked (because during such kind of interpolation a naked notation token `{}` gets
applied onto each value of the iterable one by one).


By default, for such kind of interpolations (stringifying iterables) the enumeration separator used is held in the namespace `$#` 
(default value `", "`), which means, it could be altered by a user:
```json
bash $ <<<'[1,2,3,4,5]' jtc -w'<$#:\t>v' -qqT'"good for TSV conversion:\n{}"'
good for TSV conversion:
1       2       3       4       5
bash $ 
```


### Namespace
A _namespace_ is a container within `jtc`, which allows storing JSON elements (nodes) programmatically while walking JSON.

Stored in the namespace values could be reused later in the same or different walk-paths and 
[interpolated](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation) in 
[templates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#templates) and arguments
for a shell evaluation.

Beside user provided names, `jtc` caters a number of internally generated/supported tokens and names that have various applications:
  - `$N` namespace, where `N` is a number - typically that would be a reference to the result of a REGEX matched group
  (however, could be (re)used by a user as well)
  - `$a`, `$b`, `$c`, etc - auto generated tokens when the interpolated value is an iterable (an array or an object),
  where `$a` refers to the first top element in the iterable, `$b` to the second top, etc
  - `$A`, `$B`, `$C`, etc - auto generated tokens when the interpolated value is an iterable,
  where `$A` refers to the first top element's label/index, `$B` to the second top's label/index, etc
  - `$PATH` - an auto generated token, used in templates when requires to interpolate a path (set of indices/labels)
  to the walked point as a JSON array
  - `$path` - same as `$PATH` but interpolation occurs as a _JSON string_
  - `$file` - a namespace, holding currently processed filename (if one given, otherwise empty)
  - `$_` - a namespace, holding a string value that is used when the elements during `{$path}` interpolation are getting concatenated 
  (default value `"_"`) 
  - `$#` - a namespace, holding a string value that is used as a separator when a _JSON array_ or _object_ is getting
  template-interpolated into a string (default value `", "`)
  - `$?` - a token referring to the result of a prior successful walk, thus it's used to expand multiple walks into a string
  or an array
  - `$$?` - a namespace holding a string separator considered when expanding walks using `{$?}` token into a string
  (default value is `","`) 


#### Cross-lookups using namespace 
Directives `<>v`, `<>k` (as well as all other lexemes allowing capturing and setting namespace) and search lexemes `<>s`, `<>t` 
let facilitating cross-lookups. Say, we have a following JSON:
```bash
bash $ jsn='{ "item": "bread", "list":{ "milk": 0.90, "bread": 1.20 } }'
bash $ <<<$jsn jtc -tc
{
   "item": "bread",
   "list": { "bread": 1.20, "milk": 0.90 }
}
bash $ 
```
the ask here would be to retrieve a value from `list` given the label is in `item` - that would require a cross lookup.
Using namespaces it becomes a trivial task:
```bash
bash $ <<<$jsn jtc -w'[item]<Itm>v[^0]<Itm>t' -l
"bread": 1.20
bash $ 
```
- `[item]` - selects a value by label `item`
- `<Itm>v` - stores currently walked (selected) value (`bread`) it in the namespace `Itm`
- `[^0]` - resets the walk path back to the root
- `<Itm>t` - searches (recursively) for a (first) label matching the value stored in the namespace `Itm` (which is `bread`)

The similar way (like in `<Itm>v`) labels/indices could be stored and accessed in the namespace using directive `<>k`.
The empty directive lets reinterpreting label/index of the currently walked JSON element and treat it as a _JSON string_ / _JSON number_ 
value respectively.  
E.g., say, we want to list all labels in the `address` record:
```bash
bash $ <ab.json jtc -w'<address>l[:]<>k'
"city"
"postal code"
"state"
"street address"
bash $ 
```
and we want to remap some of the labels, e.g.: `postal code` -> `zip`, `street address` -> `street`.
Here's a way to do it:
```bash
bash $ map='{"postal code":"zip","street address":"street"}'
bash $ 
bash $ <ab.json jtc -w'<address>l[:]<Lbl>k<>k' -u"$map" -u'>Lbl<t' / -w'<address>l'
{
   "city": "New York",
   "state": "NY",
   "street": "599 Lafayette St",
   "zip": 10012
}
bash $ 
```
\- it was already explained why there are 2 x `k`-lexemes in the walk-path, but here's once again: when `k`-lexeme stores the label
into a namespace it does not re-interpret the label (it ony does storing), while the emplty `k`-lexeme does. 

#### Path namespace example
Here are both of the path tokens demonstrated:
```bash
bash $ <ab.json jtc -w'<Jane>' -T'{{$PATH}}' -r
[ "Directory", 2, "name" ]
bash $ <ab.json jtc -w'<NY>' -T'{{$path}}'
"Directory_0_address_state"
bash $ 
```
_to play safe with the templates, always surround them with single quotes (to dodge shell interpolation)_  

here's an example how to join path tokens using a custom separator:
```bash
bash $ <ab.json jtc -w'<$_:\t>v<NY>' -qqT'{{$path}}'
Directory       0       address state
bash $ 
```
Equally, the same could be achived with the `$PATH` token:
```bash
bash $ <ab.json jtc -w'<$#:\t>v<NY>' -qqT'"{$PATH}"'
Directory       0       address state
bash $ 
```


#### Prior walk token
A prior last walk could be referred (during interpolation) using an auto-generated token `$?`. It comes handy when it's required
to build/join up JSON records:
```bash
bash $ <<<'["a","b","c"]' jtc -w[:]
"a"
"b"
"c"
bash $ <<<'["a","b","c"]' jtc -w[:] -T'[{$?}, {{}}]' -r
[ "a" ]
[ "a", "b" ]
[ "a", "b", "c" ]
bash $ 
```
When interpolation of the token `$?` occurs the first time (i.e. there was no prior walk, or when interpolation of `$?` fails, or the 
token was reset by user by setting the namespace '$?' to any value) then the value of this token is reset to an empty string (`""`). 

When expanding values into a string (rather than into an array), the separator used by a user is arbitrary, e.g.:
```bash
bash $ <<<'["a","b","c"]' jtc -w[:] -T'"{$?} | {}"' 
" | a"
" | a | b"
" | a | b | c"
bash $ 
```
The first separator appearing as an artifact of the first interpolation is undesirable and it seems superfluous. To rid of this artifact 
the namespace `$$?` holds the value which `jtc` considers as a separator (default is `,`):
```bash
bash $ <<<'["a","b","c"]' jtc -w'<$$?:|>v[:]' -T'"{$?} | {}"'
"a"
"a | b"
"a | b | c"
bash $ 
```

#### Iterables auto tokens
When a JSON iterable is being interpolated, then it generates auto-tokens which could be used in a template-interpolation. 
Each value in the iterable could be referred by a respective token: first value referred by `$a`, second by `$b`, and so on. In the 
unlikely event of running out of all letters (a - z), the next tokens would be `$aa`, `$ab`, and so on. If the interpolated iterable
is a JSON object, then its labels also could be referred using capital letters notations: `$A`, `$B`, ... `$Z`, `$AA`, `$AB`, etc.:
```bash
bash $ <<<'["This", "is", "example"]' jtc -T'"{$a} {$b} an {$c}!"'
"This is an example!"
bash $ 
```

The auto-generated tokens for interpolated iterables can go as far as 3 characters in size. Say, there's a big flat JSON array
(for the simplicity of explanation) like this: `[1, 2, 3, 4, 5 ... ]`. The first token (`$a`, `$A`) will refer to the number `1` 
(and its index `0` respectively), `b$` will refer `2`, `$aa` will do `27`, `$aaa` will do `703`, etc.
The highest element (index) in such array could be referred by auto-token `$zzž` (`$ZZZ`), which is `18278` (index `18277`)  

Here's a proof:
```bash
bash $ 
bash $ <<<0 jtc -jw'<c>I1><F19999' -T{c} / -T'{$a}'
1
bash $ <<<0 jtc -jw'<c>I1><F19999' -T{c} / -T'{$b}'
2
bash $ <<<0 jtc -jw'<c>I1><F20999' -T{c} / -T'{$aa}'
27
bash $ <<<0 jtc -jw'<c>I1><F19999' -T{c} / -T'{$aaa}'
703
bash $ <<<0 jtc -jw'<c>I1><F19999' -T{c} / -T'{$zzz}'
18278
bash $ 
```
\- the first _option chain-set_ (`-jw'<c>I1><F19999' -T{c}`) generates such a big flat array: `[1, 2, 3, ... 19999, 20000]`,
the second part (`-T'{$..}'`) picks the respective element from the array


### Namespaces with interleaved walks
When multiple _interleaved_ walks (`-w`) present (obviously there must be multiple walks - a single one cannot be _interleaved_), 
they populate namespaces in the order the walks appear:
```bash
bash $ <ab.json jtc -x[0][:] -y'[name]<pnt>v' -y'[children][:]<chld>v' -T'{ "Parent": {{pnt}}, "child": {{chld}} }' -r
"John"
{ "Parent": "John", "child": "Olivia" }
{ "Parent": "Ivan", "child": "Olivia" }
{ "Parent": "Jane", "child": "Olivia" }
{ "Parent": "Jane", "child": "Robert" }
{ "Parent": "Jane", "child": "Lila" }
bash $ 
```
That is a correct result (though might not reflect what possibly was intended), let's review it:
1. first line contains only result `"John"` - because template interpolation fails here (namespace `chld` does not yet exist yet,
thus the resulting template is _invalid JSON_) hence source walk is used / printed w/o interpolation
2. upon next (_interleaved_) walk, we see a correct result of a template interpolation: `Parent`'s and `child`'s records are filled
right (template is a _valid JSON_ here)
3. in the third is also corline, the result rect, albeit might not be the expected one - upon next _interleaved_ walk, the 
namespace `pnt` is populated with `"Ivan"`, but the namespace `chld` still carries the old result.
4. _etc._

By now it should be clear why is such result.

Going by the notion of the provided template, apparently, the expected result were to have all records pairs for each person with 
each own child. That way, for example, `Ivan` should not be even listed (he has no children), `John`'s record should appear only
once and `Jane` should have 2 records (she has 2 kids).

The situation could be easily rectified if for each walk we use own template and assign a dummy one for the first one: 
```bash
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
```bash
bash $ <ab.json jtc -x[0][:] -y'[name]<pnt>v' -T'""' -y'[children][:]<chld>v' -T'{ "Parent": {{pnt}}, "child": {{chld}} }' -rqq
{ "Parent": "John", "child": "Olivia" }
{ "Parent": "Jane", "child": "Robert" }
{ "Parent": "Jane", "child": "Lila" }
bash $ 
```
\- that's a neat, though a [documented](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#unquoting-JSON-strings) trick

Yet again, the same could have been achieved even a simpler way (using just one walk):
```bash
bash $ <ab.json jtc -w'<name>l:<pnt>v[-1][children][:]' -T'{ "Parent": {{pnt}}, "child": {{}} }' -r
{ "Parent": "John", "child": "Olivia" }
{ "Parent": "Jane", "child": "Robert" }
{ "Parent": "Jane", "child": "Lila" }
bash $ 
```


### Search quantifiers interpolation
Interpolation may also occur in quantifiers, say we have a following JSON, where we need to select an item from `list` by the
index value stored `item`:
```bash
bash $ jsn='{ "item": 2, "list": { "milk": 0.90, "bread": 1.20, "cheese": 2.90 } }'
bash $ <<<$jsn jtc
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
To achieve that, we need to memorize the value of `item` in the namespace first, then select a value from the list by the index:
```bash
bash $ <<<$jsn jtc -w'[item]<idx>v[-1][list]><a{idx}' -l
"milk": 0.90
bash $ 
```
It should be quite easy to read/understand such walk path (predicated one is familiar with suffixes / directives). Let's see
how the walk-path works in a slow-mo:
1. `[item]`: selects the value by label `item`:
```bash
bash $ <<<$jsn jtc -w'[item]'
2
bash $ 
```
2. `<idx>v`: the directive memorizes selected value (`2`) in the namespace `idx`
```bash
bash $ <<<$jsn jtc -w'[item]<idx>v'
2
bash $ 
```
3. `[-1]`: steps up 1 level in the JSON tree off the current position (i.e., addresses the first parent of the `item` value) which is
the root of the input JSON:
```bash
bash $ <<<$jsn jtc -w'[item]<idx>v[-1]'
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
4. `[list]`: selects the object value by label `list`:
```bash
bash $ <<<$jsn jtc -w'[item]<idx>v[-1][list]'
{
   "bread": 1.20,
   "cheese": 2.90,
   "milk": 0.90
}
bash $ 
```
5. `><a{idx}` - a non-recursive search of atomic values (`><a`) indexed by a quantifier with the stored in the namespace `idx`
(which is `2`) gives us the required value.

**_Alternatively_**, the same ask could have been achieved using a slightly different query:
```bash
bash $ <<<$jsn jtc -w'[item]<idx>v[-1][list]>idx<t' -l
"milk": 0.90
bash $ 
```
\- `>idx<t` lexeme here will use namespace `idx` to produce the offset (index).

There's a subtle difference how the lexeme `t` treats and uses referred namespace:

- in a recursive `<..>t` notation, the lexeme always treats the value in the namespace as _JSON string_ and will try searching
(recursively) a  respective label. I.e., even if the value in the namespace is numerical value `0`, it will search for a label `"0"`
- in a non-recursive `>..<t` notation, if the namespace holds a literal (i.e., a _JSON string_) value, then the lexeme will try
matching the label (as expected);  however, if the namespace holds a numerical value (_JSON number_), then the value (its integral part) 
is used as a direct offset in the searched JSON node


### Templates
Template (an argument to `-T`, or in lexeme `<..>j`) is a literal JSON optionally containing tokens for interpolation. 
Templates can be used upon walking, insertion, updates and when comparing. The result of template interpolation still must
be a valid JSON. If a template (`-T`) is given then it's a template value (after interpolation) will be used for the operations,
not the source walk (unless the resulting template is invalid JSON, in such case the source walk will be used).

When walking is a standalone operation, then template interpolation occurs from the walk-path (`-w`):
```bash
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
[cross-referenced insertions and updates](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#cross-referenced-insert-update)
are possible. Logically, for each destination walk (`-w`) there will be a respective subsequent source walk 
(e.g.: `-c <src-walk>`), thus source walk may utilize the namespaces populated during destination walk (`-w`). 
Template-interpolation will be attempted only once source walk is successful

Below is an example of updating the phone records for the first entry in the `Directory` (appending a country-code and 
altering the `phone` label at the same time via template):
```bash
# dump phones of the 1st record in the Directory 
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
bash $
# transform phone records: append country-code and update labels at the same time
bash $ <ab.json jtc -w'[0][0]<phone>l[:]' -pi'[0][0]<number>l:<V>v' -T'{ "phone number": "+1 {V}" }' / -w'[0][0]<phone>l'
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
- `-w'[0][0]<phone>l[:]'` - that's our destinations which we will be updating (i.e., all phone records in the first `Directory` entry)
- `-pi'[0][0]<number>l:<val>v'` - we'll walk (logically, synchronously with `-w`) all the `number` records and memorize `number`
values in the namespace `V`; option `-p` turns _insert_ operation into _move_
- `-T'{ "phone number": "+1 {val}" }'` after each source walk argument in `-i` a template interpolations occurs - a new 
JSON entry is generated from the template and namespace `V`, and the new entry is then used for insertion into the respective
destination walk (`-w`). Thus using templates it becomes easy to transmute existing JSON entry into a new one.

> there might be a confusion how purge (`-p`) is applied when used together with `-i`, `-u`, `-c`: 
>- when the argument of the options is a source walk and not a static JSON and/or a file (i.e. when options `-i`,`-u`,`-c` are walking
the  source/input JSON), then the purge is applied to the source walked elements
>- when the argument of the options is either a static JSON and/or a file, then the purge is applied to the destination walked 
(`-w`) elements


#### Multiple templates and walks
When _multiple_ templates given and a number of walks (`-w<walk>`, or `-u<walk>`, `-i<walk>` to which templates applied)
_is the same_, then templates are pertain to each walk. In all other cases templates are applied in a round-robin fashion.  
In the case where a round-robin behavior is required while a number of templates and walks matches, use `-nn` notation -
it will ensure round-robin templates application onto sequential walks

Compare:
```bash
# templates are pertain to walks:
bash $ <ab.json jtc -x[0][:] -y[name] -T'{"Person":{{}}}' -y[age] -T'{"Age":{{}}}' -r
{ "Person": "John" }
{ "Age": 25 }
{ "Person": "Ivan" }
{ "Age": 31 }
{ "Person": "Jane" }
{ "Age": 25 }
bash $ 

# templates are pertain to walks, while walks go sequential (non-interleaved):
bash $ <ab.json jtc -x[0][:] -y[name] -T'{"Person":{{}}}' -y[age] -T'{"Age":{{}}}' -rn
{ "Person": "John" }
{ "Person": "Ivan" }
{ "Person": "Jane" }
{ "Age": 25 }
{ "Age": 31 }
{ "Age": 25 }
bash $ 

# templates applied round-robin, while walks go sequential (non-interleaved):
bash $ <ab.json jtc -x[0][:] -y[name] -T'{"Person":{{}}}' -y[age] -T'{"Age":{{}}}' -rnnn
{ "Person": "John" }
{ "Age": "Ivan" }
{ "Person": "Jane" }
{ "Age": 25 }
{ "Person": 31 }
{ "Age": 25 }
bash $ 

```
The mess in the above last example is explained by `-nn` usage: templates are forced to get applied in the round-robin fashion while
walks are sequential.


One use-case of multiple round-robin templates would be this example:
```bash
bash $ <<<'[1,2,3,4,5,6,7,8,9,10]' jtc -w[:] -T'""' -T{} -T'""' -qq
2
5
8
bash $ 
```
\- in the above example printed every 3rd element from source JSON starting from the 2nd one (recall: when unquoting an empty
JSON string ("") the resulted blank lines are not getting printed). Though a much handier way to achieve the same is to use `-xn/N`
option.


#### Stringifying JSON, Jsonizing stringified
There's one more token combination in templates allowing _stringification_ and _jsonization_ of values:
- `<..>`, `<<..>>` will attempt "expanding" a string value into a JSON
- `>..<`, `>>..<<` will take a current JSON value and stringify it

The token notation follows the same rule as for regular tokens (`{}`, `{{}}`):
- single angular bracket notation (`<..>`, `>..<`) will result in a "naked" JSON value (without quotation marks, curly braces
or square brackets for strings, objects and arrays respectively)
- double token notation (`<<..>>`, `>>..<<`) is always a safe type and the result of operation while be a complete JSON type.

This little demo illustrates the tokens usage:
```bash
bash $ <<<'["a", "b"]' jtc -T'">{{}}<"'
"[ \"a\", \"b\" ]"
```
\- because the single form of angular token notation was used, the outer quotation marks were necessary for a successful interpolation.
The same could have been achieved with the template: `-T'>>{{}}<<'`

That was the example of _stringification_ of a JSON value, now let's do a reverse thing - _jsonize_ previously stringified value:
```bash
bash $ <<<'["a", "b"]' jtc -T'>>{{}}<<' / -T'[ <{{}}>, "c"]' -r
[ "a", "b", "c" ]
bash $ 
```
\- the above example sports _jsonization_ of the previously _stringified JSON_ while extending resulting JSON array at the same time


### Summary of interpolation token types
- [`{}`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation-token-types):
a token used in _templates_ resulting in a "naked" type of interpolation from the currently walked JSON
- [`{NS}`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation-token-types):
same as `{}`, but interpolation occurs from the _namespace_ `NS`
- [`{{}}`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation-token-types):
a token used in _templates_ resulting in a "dressed" type of interpolation from the currently walked JSON
- [`{{NS}}`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#interpolation-token-types):
same as `{{}}`, but interpolation occurs from the _namespace_ `NS`
- [`<json_str>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json-jsonizing-stringified):
a token notation for a jsonization request of the stringified JSON `json_str`, the result is a "naked" JSON value
- [`<<json_str>>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json-jsonizing-stringified):
same as `<json_str>`, however  the result is a complete ("dressed") JSON value
- [`>json<`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json-jsonizing-stringified):
a token notation for a stringification request of a JSON `json`, the result is a "naked" _JSON string_ value
- [`>>json<<`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#stringifying-json-jsonizing-stringified):
same as `>json<`, however the result is a complete ("dressed") _JSON string_ value


### Summary of namespace tokens
- [`Namespace`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespace):
a user can use any spelling (including spaces) to define a name within the namespace, as long it's compatible with
the _JSON string_ definition
- [`?..`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespace):
all internally generated names and tokens always begin with symbol `$`
- [`?N`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#namespace):
where N is a number - these names are generated by matching subgroups in REGEX lexemes (`<..>R`, `<..>D`, `<..>L`)
- [`$a`, `$b` .. `$z`, `$aa`, `$ab`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#iterables-auto-tokens), .. :
auto generated tokens when the interpolated JSON is an iterable, each token corresponds
to a respective ordinal value in the iterable
- [`$A`, `$B` .. `$Z`, `$AA`, `$AB`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#iterables-auto-tokens), .. :
auto generated tokens when the interpolated JSON is an object, each token corresponds
to a label of each respective ordinal value in the object
- [`$PATH`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#path-namespace-example):
a token expanding into a _JSON array_ holding a path (set of labels and indices) towards currently walked JSON element
- [`$path`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#path-namespace-example):
a token expanding into a _JSON string_ representing a path towards currently walked JSON element
- [`$_`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#path-namespace-example):
a name holding a separator used when expanding token `$path`, default value is `"_"`
- [`$#`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#path-namespace-example):
a name holding a separator used when a _JSON iterable_ is expanded into a _JSON string_, default value is `", "`
- [`$?`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#prior-walk-token):
a token referring to the result of a prior last successful walk
- [`$$?`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#prior-walk-token):
a namespace holding a string separator considered when expanding walks using `$?` token, default value is `","`


## Modifying JSON
there are a few options that let modifying input JSON:
- `-i` - insert (copy-insert, copy-merge, move) new elements to JSON
- `-u` - update (rewrite, rewrite-merge, move) elements to JSON
- `-s` - swap around pair(s) of JSON elements
- `-p` - purge (remove) elements from JSON

Typically, those options are mutually exclusive and if sighted together only one operation will be executed (the above list
is given in the priority order of operation selection). However, there is a combination of options `-i`,`-u` and `-p`, which
facilitates _move_ semantic, those cases reviewed in the respective chapters.

Each of options requires one or multiple `-w` to be given to operate on (a.k.a. destination walk). Options `-i` and `-u` require
an argument, which comes in different flavors, one of them is the `walk-path` per-se (a.k.a. source walk)

`jtc` will execute any of those operations only once, if multiple operations required, then those could be combined in multiple
option chain sets, daisy-chained through the separator `/`.

Once the modification operation is complete, the entire resulting JSON is displayed.

### In-place JSON modification 
By default `jtc` expects the input from `stdin`. If the standalone argument(s) `args` is given then `jtc` will read input from the 
file (ignoring `stdin`), see below:
```bash
# show content of the file:
bash $ cat file.json 
[ "JSON", "in", "file" ]
bash $ 
# both input sources present: stdin and file 
bash $ <<<'[ "<stdin>", "JSON" ]' jtc file.json
[
   "JSON",
   "in",
   "file"
]
bash $
```
The option `-f` (together with a _single_ argument) redirects (forces) the output of the operation into the file (instead of `stdout`):
```bash
bash $ <<<'[ "<stdin>", "JSON" ]' jtc -f file.json
bash $ cat file.json 
[
   "JSON",
   "in",
   "file"
]
bash $ 
```
In the above example, JSON is read from `file.json` and output back into the file (`stdin` input is ignored) - note the altered
format of the file.

#### Ensuring input read from `stdin`
The bare hyphen (`-`) overrides file _input_ and ensures that the input is read from the `stdin`:
```bash
bash $ <<<'[ "input", "JSON" ]' jtc -f - file.json
bash $ cat file.json
[
   "input",
   "JSON"
]
bash $ 
```


### Purging JSON
`-p` removes from JSON all walked elements (given by one or multiple `-w`). E.g.: let's remove from the address book (for the sake
of an example) all the `home` and `office` phones records (effectively leaving only `mobile` phone records):
```bash
bash $ <ab.json jtc -w'[type]:<home>:[-1]' -w'[type]:<office>:[-1]' -p / -w'<phone>l:' -l -tc
"phone": [
   { "number": "112-555-1234", "type": "mobile" },
   { "number": "113-123-2368", "type": "mobile" }
]
"phone": [
   { "number": "223-283-0372", "type": "mobile" }
]
"phone": []
bash $ 
```
Of course there's a bit more succinct syntax:
```bash
bash $ <ab.json jtc -x[type]: -y'<home>:[-1]' -y'<office>:[-1]' -p  / -w'<phone>l:' -ltc
```
or, using even a single walk-path:
```bash
bash $ <ab.json jtc -w'[type]:<home|office>R:[-1]' -p / -w'<phone>l:' -ltc
```

Another use-case example: remove all the JSON elements _except_ walked ones, while preserving original JSON structure - that's
a feat for a plural option notation: `-pp`. E.g.: let's drop all the entries (in all records) but `name` and `spouse`:
```bash
bash $ <ab.json jtc -w'<name|spouse>L:' -pp -tc
{
   "Directory": [
      { "name": "John", "spouse": "Martha" },
      { "name": "Ivan", "spouse": null },
      { "name": "Jane", "spouse": "Chuck" }
   ]
}
bash $ 
```
Here, `name|spouse` is the RE (indicated by the RE label search suffix `L`) matching labels containing either `"name"` or
`"spouse"`


### Swapping JSON elements
`-s` requires walk-paths (`-w`) to be given in pairs. Paired walk-path will be walked concurrently (so, ensure they are consistent)
and resulted JSON elements will be swapped around.

E.g., here's the way of swapping around `name` and `spouse` for all records on the address book:
```bash
bash $ <ab.json jtc -w'<name>l:' -w'<spouse>l:' -s / -w'<name|spouse>L:' -l
"name": "Martha"
"spouse": "John"
"name": null
"spouse": "Ivan"
"name": "Chuck"
"spouse": "Jane"
bash $
```
> _\- for the sake of output brevity, swapped elements only displayed_


Probably, a more frequent use-case for `-s` is when it's required to remove some extra/redundant nestedness in a JSON structure. 
E.g., let's remove _array_ encapsulation from phone records, leaving only _the last phone record_ in it:
```bash
bash $ <ab.json jtc -w'<phone>l:' -w'<phone>l:[-1:]' -s / -w'<phone>l:' -l
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
> _\- again, for the brevity, only phone records are displayed_

Finally, more than just one pair of walks (-w) could be swapped out. In fact, as many *pairs* of walks given will be swapped
(predicated walks did not become invalid during prior walk pair swap operations)


### Insert operations
when either of insert (`-i`) of update (`-u`) operation is carried, there 2 types of walks exist:
 - one facilitating insert/update points, a.k.a. _destination walks_ (always facilitated with `-w` options)
 - one facilitating points (elements) being inserted/updated, a.k.a. _source walks_ (facilitated with respective `-i`, `-u` options)

The _destination points_ of insertion are always given using `-w` option(s), while the argument under `-i` designates the source 
of the insertions (multiple `-i` options could be given). The source of insertion must _always_ be a valid JSON.

Insert operations never result in overwriting destination JSON elements (though the destination could be extended).
There are 5 different flavors of insertion arguments (i.e., sources of insertion):
- `-i <static_json> ...`: a JSON being inserted is either read from a file or spelled literally, multiple of such insert options allowed 
- `-i <static_json> -i<walk-path> ...`: here the `walk-path` actually walks `static_json` rather than the input (source) JSON; only one
option with `static_json` argument is allowed (will be processed) while multiple options with `walk-path` may be given
- `-i <walk-path> ...`: the argument `walk-path` walks the input (source) JSON, multiple allowed
- `-ei <shell_cli> \;`: `shell_cli` is the shell command sequence terminated with `\;` destination walks (`-w`) to be shell evaluated, 
optionally containing interpolation tokens; tokens `{}`,`{{}}` will be referring to JSONs pointed by `-w` (destination) walk; the 
returned value (predicated the evaluation was a success) has to be a valid JSON, otherwise it'll be promoted to a _JSON string_.
- `-ei <shell_cli> \; -i<walk-path>` ...: `walk-path` here walks the input (source) JSON and tokens `{}`,`{{}}` will be referring
to `-i<walk-path>`, the shell evaluation occurs for the each of `-i<walk-path>` rather than for destination walks (`-w`).

How does `jtc` know which argument is supplied? The disambiguation path is like this:
1. initially a `file` argument is assumed and attempted to be open/read, if that fails (i.e., file not found), then
2. a literally spelled JSON is assumed and attempted to be parsed. If JSON parsing fails, then
3. a `walk-path` is assumed and parsed - if that fails too, a short exception message is thrown (`walk_expect_lexeme`)

> _Attention is required when passing a `<walk-path>` type argument: some walk-paths look exactly like JSON, e.g:
`[0]` - this is both a valid JSON array (made of a single numeric value `0`) and a valid walk-path (addressing the first element
in an iterable), hence such argument will be treated as JSON.  
> To pass it as a walk-path, modify it to a range-type of walk, e.g.: `[0:1]` - that is still a valid walk-path (selecting only the
first element) but is invalid JSON.  
> **Alternatively**, add a trailing space at the end of the walk-lexeme: `'[0] '` - then the argument will be treated as a walk-path (in 
options `-i`, `-u`, `-c` the JSON argument is expected to have no trailing white spaces or other characters)_


#### Destination driven insertion
The destination insertion point(s) (`-w`) controls how insertion is done:
- if a given destination insertion point (`-w`) is a single walk and non-iterable - i.e., if it's a single point location - then 
_all_ the supplied sources are attempted to get inserted into a _single_ destination location:
```bash
# list all children records
bash $ <ab.json jtc -w'<children>l:' -lr
"children": [ "Olivia" ]
"children": []
"children": [ "Robert", "Lila" ]
bash $
# make couple insertions in a single destination record
bash $ <ab.json jtc -w'[name]:<Ivan>[-1][children]' -i'"Maggie"' -i'"Bruce"' / -w'<children>l:' -lr
"children": [ "Olivia" ]
"children": [ "Maggie", "Bruce" ]
"children": [ "Robert", "Lila" ]
bash $
```

- if a given destination insertion point is iterable or multiple are given, then all sources (`-i` arguments) are inserted one
by one in a round-robin fashion (if source runs out of JSON elements, but destination has more room to iterate, then source
is wrapped to the first element):
```bash
# make insertion in a round-robin fashion
bash $ <ab.json jtc -w'<children>l:' -i'"Maggie"' -i'"Bruce"' / -w'<children>l:' -lr
"children": [ "Olivia", "Maggie" ]
"children": [ "Bruce" ]
"children": [ "Robert", "Lila", "Maggie" ]
bash $
```


#### Inserting objects into objects
while insertion into arrays is obvious (well, so far), insertion into objects requires clarification:
- objects always merged recursively
- in case of the clashing labels, by default, the destination is preserved while source of insertion is discarded

To illustrate, let's insert a JSON structure: `{ "PO box": null, "street address": null }` into the last record's `address`:
```bash
# dump the first address record in Directory  
bash $ <ab.json jtc -w'[0][-1:][address]' -l
"address": {
   "city": "Denver",
   "postal code": 80206,
   "state": "CO",
   "street address": "6213 E Colfax Ave"
}
bash $
# insert custom entries into the first address record 
bash $ <ab.json jtc -w'[0][-1:][address]' -i'{ "PO box": null, "street address": null }' / -w'[0][-1:][address]' -l
"address": {
   "PO box": null,
   "city": "Denver",
   "postal code": 80206,
   "state": "CO",
   "street address": "6213 E Colfax Ave"
}
bash $
```
\- the `"PO box"` label got inserted, but the destination object's value in the `"street address"` has been preserved


#### Insertion matrix without merging
The source (a JSON being inserted) and the destination (a JSON point where insertion occurs) elements might represent different types:
_JSON array_, _JSON object_, _JSON atomic_. Thus there's a number of variants of insertions of one type of elements into others. 
All such variants are shown in the below matrix table:
```
  to \ from  |        [3,4]        |     {"a":3,"c":4}     |      "a":3,"c":4      |      3
-------------+---------------------+-----------------------+-----------------------+-------------
   [1,2]     |      [1,2,[3,4]     |  [1,2,{"a":3,"c":4}]  | [1,2,{"a":3},{"c":4}] |   [1,2,3]
{"a":1,"b":2}|    {"a":1,"b":2}    |  {"a":1,"b":2,"c":4}  |  {"a":1,"b":2,"c":4}  |{"a":1,"b":2}
    "a"      |         "a"         |          "a"          |          "a"          |     "a"
```
> _\- the values in the 4th column header (namely `"a":3,"c":4`) do not look like valid JSON - those are JSON object's elements
when pointed to by the `-i <walk-path>`, i.e., they are JSON values in objects (the values with labels_)

as follows from the table:
- insertion cannot occur into the atomic JSON elements
- when inserting into an array, the whole JSON value is getting inserted (no array expansion occurs)
- labeled values are getting inserted into arrays as standalone JSON objects
- when inserting objects into objects, upon label clashing the destination's label is preserved (source's ignored)

#### Insertion matrix with merging
if insertion of an array into another array happens without merging arrays, how then to achieve the merged result upon
insertion?

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
- both support [_move_](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-with-move-semantic)
(`-p`) semantic
- both support [_shell evaluation_](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation)
(`-e`) of argument 

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
- when objects are merge-updated, for clashing labels the source does overwrite the destination (unlike with insertion)


#### Updating labels
The directive lexeme `<>k` allows accessing the label/index of the currently walked JSON element and even store it in the namespace.

Another function featured by the lexeme is that the label could be is reinterpreted as a _JSON string_ value, that allows rewriting
labels using update operation (insert into labels is not possible even semantically). However, that only applies if `<>k` lexeme 
is the last lexeme in the walk-path and if the lexeme is empty.

As the an exercise, let's capitalize all the labels within all `address`'es in `ab.json`:
```bash
bash $ <ab.json jtc -w'<address>l:[:]<>k' -eu '<<<{{}}' tr '[:lower:]' '[:upper:]' \; / -w'<address>l:' -rl
"address": { "CITY": "New York", "POSTAL CODE": 10012, "STATE": "NY", "STREET ADDRESS": "599 Lafayette St" }
"address": { "CITY": "Seattle", "POSTAL CODE": 98104, "STATE": "WA", "STREET ADDRESS": "5423 Madison St" }
"address": { "CITY": "Denver", "POSTAL CODE": 80206, "STATE": "CO", "STREET ADDRESS": "6213 E Colfax Ave" }
bash $ 
```

The destination walk-path will not become invalid after the parent's label has been altered, thus allowing altering labels even 
of the nested elements in the same recursive update: 
```bash
# list all the labels of the John's record:
bash $ <ab.json jtc -w'[name]:<John>[-1]<.*>L:<>k'
"address"
"city"
"postal code"
"state"
"street address"
"age"
"children"
"name"
"phone"
"number"
"type"
"number"
"type"
"spouse"
bash $ 
# capitalize all the labels in it:
bash $ <ab.json jtc -w'<John>[-1]<.*>L:<>k' -eu '<<<{{}}' tr '[:lower:]' '[:upper:]' \; / -w'<John>[-1]' -tc
{
   "ADDRESS": { "CITY": "New York", "POSTAL CODE": 10012, "STATE": "NY", "STREET ADDRESS": "599 Lafayette St" },
   "AGE": 25,
   "CHILDREN": [ "Olivia" ],
   "NAME": "John",
   "PHONE": [
      { "NUMBER": "112-555-1234", "TYPE": "mobile" },
      { "NUMBER": "113-123-2368", "TYPE": "mobile" }
   ],
   "SPOUSE": "Martha"
}
bash $ 
```


### Insert, Update with move semantic 
if a source argument for either `-i` or `-u` is given in the form of `<file>` or `<JSON>`, then those obviously cannot be moved.
The move semantic is only applicable when the argument is given only in the form of a `<walk-path>` (i.e. it refers to the 
input/source JSON), then upon completing the operation, the source elements (referred by the source walk-path) becomes possible
to remove (purge). This is achievable by adding the option `-p`.

Let's move `address` from the last `Directory` record into the first one:
```bash
bash $ <ab.json jtc -w'[Directory][0][address]' -u'[Directory][-1:][address]' -p -tc
{
   "Directory": [
      {
         "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" },
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
\- That leaves `Jane` "homeless", while `John` "moves" into `Jane`'s place!


### Insert, Update: argument shell evaluation
An argument for _insert_ and _update_ operations (`-i`, `-u`) optionally may undergo a shell evaluation (predicated by preceding 
option `-e`).  
E.g., let's capitalize all the `name` entries in the address book:
```bash
# dump all the names:
bash $ <ab.json jtc -w'<name>l:' 
"John"
"Ivan"
"Jane"
bash $
# capitalize the names through update using shell evaluation:
bash $ <ab.json jtc -w'<name>l:' -eu '<<<{{}}' tr "[:lower:]" "[:upper:]" \; / -w'<name>l:'
"JOHN"
"IVAN"
"JANE"
bash $
```
Once options `-e` and `-u`(`-i`) are used together, following rules must be observed:
- option `-e` must precede first occurrence of `-i`/`-u`
- cli arguments sequence following option `-i`/`-u` must be terminated with the _standalone_ escaped semicolon: `\;`
- the cli is also subjected for namespace interpolation before it gets shell evaluated
- the cli in the argument do not require any additional escaping (except those which would normally be required by a shell)
- if piping in the cli is required then pipe symbol itself has to be escaped and spelled _standalone_: ` \| `
- returned result of a shell evaluation must be either a valid JSON, or non-empty and non-error (in the latter case it'll
be promoted to a _JSON string_ value)
- failed (those returning non-zero exit code) or empty results of the shell evaluations are ignored
(then JSON entry wont be updated/inserted, rather proceed to the next walked entry for another/next update attempt)
- templates (`-T`) / template-interpolation occurs and applies after shell evaluation returns the result

>if shell cli does not deliver expected result for some reason, it's debuggable with `-dd` options.


### Use of mixed argument types for `-i`, `-u`, `-c` 
options `-i`, `-u`, `-c` allow two *kinds* of their arguments:
1. static JSONs (i.e., `<file>`, `<JSON>`)
2. walk-path (i.e., `<walk-path>`)

\- when those used together, namely a `<walk-path>` argument(s) follows either of statics, e.g.:  
`jtc -u file.json -u'[Root][:]'`

then all `<walk-path>` arguments (here `[Root][:]`) apply onto the static argument (here `file.json`).
\- When both kinds of arguments are used together, then only one (the first) static JSON argument is accepted, while 
multiple walk-path may be given
\- if `<walk-path>` arguments are given without preceding static JSON, then walk-path are applied onto the input (source) JSON

That rule is in play to facilitate a walking capability over the specified static JSONs. Though be aware: in any case _all specified 
`<walk-path>` arguments will be processed in a consecutive order, one by one (i.e., interleaving never occurs)_.

(Also, see 
[operations with cross referenced lookups](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#cross-referenced-insert-update))


### Use of mixed argument types with `-e`
options `-u`, `-i` when used together with `-e` also allow specifying multiple instances of the option usage:
1. first option occurrence must prove a shell cli line, terminated with the standalone spelling of a semicolon `\;`
2. all the subsequent option usages must provide `<walk-path>` type of argument, which let specifying source(s) of interpolation
(occurring before shell evaluation happens). However, in the case if mixed option arguments usage is detected (in presence of `-e`),
then the semantic of the `jtc` arguments would be like this (e.g., for option `-u`):  
`jtc -w'<dst>' -eu <shell cli ...> \; -u'<src>'`  
Here, both `<dst>` and `<src>` walk the same input JSON. _Shell cli_ evaluation / interpolation occurs from walking `<src>`
That way it's possible to decouple source(s) (of interpolation) from the destination(s): all trailing (subsequent) arguments of `-u`
will be used in every shell evaluation (interpolating respective JSON elements), while arguments pointed by (all) `-w` option(s)
will point where returned/evaluated resulting JSONs should be placed.

The described argument behavior facilitates transformation of a JSON when a source location of transformation is not the same as
a destination

Hopefully this example will clarify:
- say (just for the sake of example), we want to add to every record's `children` the `name` of the person, but not just - we
want to add it in all capitals (i.e., transform the record).
```bash
bash $ <ab.json jtc -w'<children>l:' -ei '<<<{{}}' tr '[:lower:]' '[:upper:]' \; -i'<name>l:' / -lrw'<name>l:' -w'<children>l:'
"name": "John"
"children": [ "Olivia", "JOHN" ]
"name": "Ivan"
"children": [ "IVAN" ]
"name": "Jane"
"children": [ "Robert", "Lila", "JANE" ]
bash $ 
```
- there, the source(s) of shell interpolation were `name` records (provided with `-i'<name>l:'`), while the destinations were
`children` (given with `-w'<children>l:'`)

In case if a single option instance (`-eu`/`-ei`) is used (w/o trailing options with walk arguments), then both the source
(of interpolation) and the destination (of operation) would be provided with `-w` option argument


### Cross-referenced insert, update
One use-case that namespaces facilitate quite nicely, is when insert/update/purge/compare operation refer to different JSONs 
(i.e., in
[Use of mixed argument types](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#use-of-mixed-argument-types-for--i--u--c)) 
but one requires a reference from another.

Say, we have 2 JSONs:
1. `main.json`:
```bash
bash $ <main.json jtc -tc
[
   { "name": "Abba", "rec": 1, "songs": [] },
   { "name": "Deep Purple", "rec": 3, "songs": [] },
   { "name": "Queen", "rec": 2, "songs": [] }
]
bash $ 
```
2. `id.json`:
```bash
bash $ <id.json jtc -tc
[
   { "id": 3, "title": "Smoke on the Water" },
   { "id": 1, "title": "The Winner Takes It All" },
   { "id": 2, "title": "The Show Must Go On" }
]
bash $ 
```

The ask here is to insert songs titles from `id.json` into `main.json` cross-referencing respective `rec` to `id` values.  
The way to do it:
- first walk `main.json` finding and memorizing (each) `rec` value 
- then, walk up to the `song` entry  (so that will be a destination pointer, where song needs to be inserted).

The insert operation (`-i`) here would need to find `id` record in `id.json` using memorized (in the destination walk) namespace and 
insert respective `title`:
```bash
bash $ <main.json jtc -w'<rec>l:<R>v[-1][songs]' -mi id.json -i'[id]:<R>s[-1][title]' -tc
[
   {
      "name": "Abba",
      "rec": 1,
      "songs": [ "The Winner Takes It All" ]
   },
   {
      "name": "Deep Purple",
      "rec": 3,
      "songs": [ "Smoke on the Water" ]
   },
   {
      "name": "Queen",
      "rec": 2,
      "songs": [ "The Show Must Go On" ]
   }
]
bash $ 
```
For each destination walk (`-w`) here, there will be a respective insert-walk (`-i`) (`-w` is always walked first). When dst. 
walk  finishes walking, the namespace will be populated with a respective value from the `rec` entry. That value will be reused
by insert-walk when walking its source JSON (`id.json`) with the lexeme `[id]:<R>s` that will find a respective `id`. 
The rest should be obvious by now.


#### Cross-referenced purge
`jtc` does not have a "walk" argument for `-p` (purge) operation (`-p` is a standalone option, when it's used only with `-w`
it will purge every resulted/walked entry).  
So, how to facilitate a cross-referenced purge then? (i.e., when purging ids are located in a separate file)  

The trick is to use update/insert `-u`/`-i` operation together with `-p`. When the cli is given in this notation:  
`<<<dst.json jtc -w... -u <src.json> -u... -p`,  
purging will be applied to walked destinations, but only predicated by a successful source walk:
```bash
bash $ <main.json jtc -w'<rec>l:<R>v[-1]' -u'[{"id":1}, {"id":3}]' -u'[id]:<R>s' -p
[
   {
      "name": "Queen",
      "rec": 2,
      "songs": []
   }
]
bash $ 
```

The "complemented" purge operation (i.e. when we want to delete everything except referenced) is facilitated using `-pp`:
```bash
bash $ <main.json jtc -w'[rec]:<R>N:[-1]<E>v' -u'[1, 3]' -u'<R>s' -T'{{E}}' -pp
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
\- memorizing the whole entry (in `E`) is required because update operation w/o the template only replaces records (and 
purge everything else), but that's not the goal - the goal is to retain all the entries, hence replacing the updated entries
with the template for the entire entry.


### Summary of modification options
- [`-`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#ensuring-input-read-from-stdin):
bare qualifier (hyphen), ensures that input read occurs from `stdin` regardless of present filename arguments
- [`-f`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#in-place-json-modification):
ensures that the final output is redirected to the filename (if one given) instead of `stdout`
- [`-p`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#purging-json):
purges all walked (`-w`) JSON elements from a JSON tree
- [`-pp`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#purging-json):
purges all JSON elements _except_ walked ones (`-w`) from a JSON tree
- [`-s`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#swapping-json-elements):
swaps around JSON elements in a JSON tree pointed by the _pairs_ of walks (i.e. at least 2 -w must be given)
- [`-i<static_json>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-operations):
inserts `static_json` (which is either a file, or a literally spelled JSON) into the destinations pointed by `-w`; 
multiple options with such arguments allowed
- [`-i<static_json> -i<walk-path>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-operations):
inserts JSON elements walked `static_json` with `walk-path` into the destinations pointed by `-w`; 
only a single option with `static_json` and multiple options with `walk-path` arguments are supported
- [`-i<walk-path>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-operations):
insert-copies JSON elements from the input (source) JSON pointed by `walk-path` into the destinations pointed by `-w`;
multiple options with such arguments allowed
- [`-pi<walk-path>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-with-move-semantic):
insert-moves JSON elements from the input (source) JSON pointed by `walk-path` into the destinations pointed by `-w`;
multiple options with such arguments allowed
- [`-ppi<walk-path>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-with-move-semantic):
inserts JSON elements from the input (source) JSON pointed by `walk-path` into the destinations pointed by `-w`, 
while purging all other (non-walked) elements from a JSON tree
- [`-ei <shell_cli> \;`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation):
inserts a JSON element resulted from a shell evaluation running `shell_cli` into the destinations pointed
by `-w`; `shell_cli` is run for every successful destination walk (`-w`) iteration; only a single invocation _per the option chain-set_ is
supported
- [`-ei <shell_cli> \; -i<walk-path>`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation):
inserts a JSON element resulted from a running `shell_cli` into the destinations pointed
by `-w`; `shell_cli` is run for every successful source `walk-path` iteration walking input (source) JSON; multiple options with
`walk-path` argument are supported
- [`u...`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#update-operations):
update (rewrite) operations, all the same option modes and combinations as for `-i` are applied
- [`-m`](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insertion-matrix-with-merging):
modifier option, when used together with `-i`, `-u` toggles insert/update behavior allowing "merging" behavior


## Comparing JSONs
`-c` allows comparing JSONs (or JSONs element pointed by walk-paths) - `jtc` will display JSON delta (diffs) between compared JSONs. 
Let's compare `phone` records from the first and the second entries of the address book:
```bash
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
```bash
bash $ <<<'123' jtc -c'123' -l
"json_1": {}
"json_2": {}
bash $ echo $?
0
bash $
```
Otherwise (JSONs are different) a non-zero code is returned:
```bash
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
```bash
bash $ <ab.json jtc -w'<Ivan>[-1][children]' -i'"Norma"' / -w'<Ivan>[-1]' -c ab.json -c'<Ivan>[-1]' -l
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
```bash
bash $ <ab.json jtc -w'<Ivan>[-1][children]' -i'"Norma"' / -w'<Ivan>[-1]<>c:<>k' -c'ab.json' -c'<Ivan>[-1]<>c:<>k' -l
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
> NOTE: _usage of '<>k' is only restricted to JSON elements which have labels/indices. JSON `root` does not have any of those, thus
attempting to print a label of the root always results in the exception:_
>```bash
>bash $ <ab.json jtc -w'<>k'
>jtc json exception: walk_root_has_no_label
>bash $ 
>```


## Processing input JSONs

### Parse ill-formed JSONs with clashing labels 
[ECMA-404](http://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf) is relaxed w.r.t. the uniqueness of labels:
> _The JSON syntax does not impose any restrictions on the strings used as names, does not require that name strings be unique_ ...

While JSON [RFC 7158](https://tools.ietf.org/html/rfc7158) is strict on the label uniqueness:
> _... The names within an object SHOULD be unique._

`jtc` follows the RFC (and so considers JSONs with clashing labels to be ill-formed), because logically, labels must provide
an addressing mechanism within objects and non-unique (clashing) labels break the addressing. In case if the source JSON holds
duplicate labels, by default `jtc` parses and retains the first label only:
```bash
bash $ cat ill.json 
{
 "label": "first entry",
 "label": "second entry"
}
bash $ 
# parse ill-formed json:
bash $ <ill.json jtc 
{
   "label": "first entry"
}
bash $ 
```
However, sometimes there's a requirement to parse in such ill-formed JSONs and retain all the values. Option `-mm` allows
merging the values with clashing labels into a JSON array:
```bash
bash $ <ill.json jtc -mm
{
   "label": [
      "first entry",
      "second entry"
   ]
}
bash $ 
```
> Note: option `-mm` will not engulf a single one `-m` and thus, if both behaviors are required then both to be provided (e.g.: `-mmm`)


### A word on ordering within JSON objects
both _`ECMA-404`_ and _`RFC 7158`_ agree that JSON spec does not assign any significance to the ordering of name/value pairs within
objects (unlike with the arrays, which are ordered sequences) - thus, it's free up to a JSON parser to handle it in any preferable way.

`jtc`, while being relaxed upon parsing in object values coming in any order, will always re-arrange objects by labels sorted
in the descending order - that provides some benefits when handling JSON manipulations queries, e.g.: auto-generated tokens when
an object gets interpolated into a template-string allows operating with object values deterministically, another benefit is that 
subscripting object elements with numerical indices also becomes more deterministic, etc.


### Process all input JSONs
Normally `jtc` would process only a _single_ input JSON. If multiple input JSONs given - the fist JSON will be processed and the 
rest of the inputs will be silently ignored:
```bash
bash $ <<<'[ "1st json" ] { "2nd": "json" } "3rd json"' jtc -r
[ "1st json" ]
bash $ 
```
Couple options allow altering the behavior and process all the input JSONs:

Option `-a` instructs to process each of the input JSONS:
```bash
bash $ <<<'[ "1st json" ] { "2nd": "json" } "3rd json"' jtc -ar
[ "1st json" ]
{ "2nd": "json" }
"3rd json"
bash $ 
```
\- respected processing (of all given options) will occur for all of the input JSONs:
```bash
bash $ <<<'[ "1st json" ] { "2nd": "json" } "3rd json"' jtc -a -w'<json>R'
"1st json"
"json"
"3rd json"
bash $ 
```
All the input JSONs will be processed as long they are valid - processing will stops upon parsing failure:
```bash
bash $ <<<'[ "1st json" ] { "2nd": json" } "3rd json"' jtc -ad
.display_opts(), option set[0]: -a -d (internally imposed: )
.read_inputs(), reading json from <stdin>
.write_json(), outputting json to <stdout>
[
   "1st json"
]
.location_(), exception locus: [ "1st json" ] { "2nd": j
.location_(), exception spot: ------------------------>| (offset: 24)
jtc json parsing exception (<stdin>:24): expected_json_value
bash $ 
```

### Wrap all processed JSONs
option `-J` allows wrapping all processed input JSONs into a super JSON array (option `-J` assumes option `-a`, no need giving both):
```bash
bash $ <<<'[ "1st json" ] { "2nd": "json" } "3rd json"' jtc -J -w'<json>R'
[
   "1st json",
   "json",
   "3rd json"
]
bash $ 
```
option `-J` also implicitly imposes `-j` thus it could be used safely even with a single JSON at the input with the same effect.  
Though, when walking multiple input JSONs, each of the option would have its own effect, this example clarifies:
```bash
# process and wrap each input JSON into an array:
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
# process all input JSONs and wrap them into an array:
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
# process and wrap each input JSON into an array and then wrap all the processed into a super array:
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
> _Note: `jtc` supports an unlimited number of files that can be supplied via standalone arguments (after all options given). 
When multiple input files are given, options `-a` is assumed_ automatically.


### Buffered vs Streamed read
`jtc` may read inputs 2 ways:
- _**buffered read**_
- _**streamed read**_

In the _buffered read_ mode (which is default), the entire file (or `<stdin>`) input is read into memory and only then JSON parsing is
attempted (with all subsequent due processing).  
In the _streamed read_ mode JSON parsing begins immediately as the the first character is read (so, no memory wasted to hold input
literal JSON).

The _streamed read_ is activated when:
- option `-a` given **_AND_** input source is `<stdin>`  

The option `-J` overrides _streamed read_ (reverting to _buffered_): the _streamed read_ might be endless, while option `-J`
assumes a finite number of inputs to be processed and then displayed

From the JSON result point of view there's no difference between _buffered_ and _streamed_ reads - the result will be 100% consistent
across both types of reads. However, _streamed read_ finds its application when the streamed data are there (typically would be
a network-based streaming)

We can see the difference in the parsing when debugging `jtc`:
\- in a _buffered read_ mode, the debug will show the _parsing point_ with the data following behind it:
```bash
bash $ <ab.json jtc -dddddd 
.display_opts(), option set[0]: -d -d -d -d -d -d (internally imposed: )
.read_inputs(), reading json from <stdin>
..ss_init_(), initializing: buffered_cin
..ss_init_(), read file: <stdin> (1674 bytes)
..run_decomposed(), pass for set[0]
......parse_(), parsing point ->{|   "Directory": [|      {|         "address": {|        ...
......parse_(), parsing point ->"Directory": [|      {|         "address": {|            "...
......parse_(), parsing point ->[|      {|         "address": {|            "city": "New Y...
......parse_(), parsing point ->{|         "address": {|            "city": "New York",|  ...
...
```
\- in a _streamed read_ mode, the _parsing point_ would point to the last read character from the `<stdin>`:
```bash
bash $ <ab.json jtc -dddddd -a
.display_opts(), option set[0]: -d -d -d -d -d -d -a (internally imposed: )
.read_inputs(), reading json from <stdin>
..ss_init_(), initializing: streamed_cin
..run_decomposed(), pass for set[0]
......parse_(), {<- parsing point
......parse_(), {|   "<- parsing point
......parse_(), {|   "Directory": [<- parsing point
......parse_(), {|   "Directory": [|      {<- parsing point
......parse_(), {|   "Directory": [|      {|         "<- parsing point
...
```

Here's an example of how _streamed read_ works in `jtc`:
```bash
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


### Chaining option sets
Like it was mentioned before, `jtc` performs one major operation at a time: _standalone walking_, _insertion_, _update_, _purging_, 
_swapping_, _comparison_. There's a number of supplementary operations that might complement the major operations like: wrapping results 
into JSON arrays and objects, toggling various viewing and parsing modes, etc.

If multiple major operations are required, one way to achieve it would be piping an output of the prior operation into the input
of the next one, e.g:  
- `jtc <insert...> | jtc <swap...> | etc`  

However, such approach is quite suboptimal - with every piping operation a serialization (outputting) and deserialization (parsing)
of JSON occurs and those are quite expensive (CPU cycles-wise) operations.

`jtc` permits chaining multiple operations using solidus separator `/`. The above example could be collapsed into this:  
- `jtc <insert...> / <swap...> / etc`  

without any affect to the result. The sets of all options in between separators are known as _options sets_.

The advantage of such approach is huge: processed JSONs now are passed from one option set to the next one in a compiled binary
form (no CPU cycles wasted on printing / parsing). Another additional benefit is that the _namespace_ now is shared across all 
_options sets_.

There's a few options (mostly viewing and parsing) which are non-transient and may occur only in the first or in the last _option set_:
- `-r`: compact printing - may occur only in the last option set
- `-rr`: stringifying output JSON - may occur only in the last option set; if such operation is required in the interim operation -
use template stringification instead
- `-t`: output indentation - may occur only in the last option set
- `-q`: parse input with a strict solidus quoting - may occur only in the initial option set
- `-qq`: unquoting _JSON strings_, jsonizing stringified JSONs - may occur only in the last option set; if such operation is required
in the interim operation - use template jsonizing instead
- `-z`: additionally printing size for the each walked JSON - may occur only in the last option set
- `-zz`: printing size instead of JSON - may occur only in the last option set
- `-f`: forcing (redirecting) outputs into a file  - may occur only in the last option set
- `-`: ensuring input is read from `stdin` - may occur in any of the option sets, but affects only first one (where parsing occurs)


## Some Examples
### Generating CSV from JSON
`CSV` stands for _comma separated values_, thus to convert a JSON into a `csv` file, it's required dumping all relevant JSON walks
line by line, while separating JSON values either with `,` or with `;` (`CSV` format admits both)

There are couple tricks required to do so, but not difficult ones, so, let's walk it.

Say, we want to dump into `csv` format following data from the `ab.json`:  
`name, city, postal code, state, street address`

1. First let's build a walk going over all the names (memorizing them) and all the addresses:
```bash
bash $ <ab.json jtc -rw'<name>l:<N>v[-1][address]'
{ "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" }
{ "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" }
{ "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" }
bash $ 
```
\- not that difficult

2. Now, using template it's possible to arrange all the data in the required format:
```bash
bash $ <ab.json jtc -rw'<name>l:<N>v[-1][address]' -qqT'"{N}, {$a}, {$b}, {$c}, {$d}"'
John, New York, 10012, NY, 599 Lafayette St
Ivan, Seattle, 98104, WA, 5423 Madison St
Jane, Denver, 80206, CO, 6213 E Colfax Ave
bash $ 
```
The template above is demo'ing iterable auto tokens - those are good when it's required to extract iterables members individually, 
or in a different order. In our case the query's order of items matches those in the array, hence it's possible to simplify our
template:
```bash
bash $ <ab.json jtc -rw'<name>l:<N>v[-1][address]' -qqT'"{N}, {}"'
John, New York, 10012, NY, 599 Lafayette St
Ivan, Seattle, 98104, WA, 5423 Madison St
Jane, Denver, 80206, CO, 6213 E Colfax Ave
bash $ 
```


If the header is required it could be added either using unix `echo` command:
```bash
bash $ hdr='name, city, postal code, state, street address'
bash $ echo -e "$hdr\n$(<ab.json jtc -rw'<name>l:<N>v[-1][address]' -qqT'"{N}, {}"')"
name, city, postal code, state, street address
John, New York, 10012, NY, 599 Lafayette St
Ivan, Seattle, 98104, WA, 5423 Madison St
Jane, Denver, 80206, CO, 6213 E Colfax Ave
bash $ 
```

Another way to add a header is to use additional template with `jtc`: 
```bash
bash $ <ab.json jtc -nqqw' ' -T"\"$hdr\"" -w'<name>l:<N>v[-1][address]' -qqT'"{N}, {}"'
name, city, postal code, state, street address
John, New York, 10012, NY, 599 Lafayette St
Ivan, Seattle, 98104, WA, 5423 Madison St
Jane, Denver, 80206, CO, 6213 E Colfax Ave
bash $ 
```
DONE.


### Taming duplicates
Quite a very common query for JSON is to process duplicates. Say, we deal with the following JSON:
```bash
bash $ jsn='[ "string", true, null, 3.14, "string", null ]'
bash $ <<<$jsn jtc
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
```bash
bash $ <<<$jsn jtc -w'<>Q:' -p
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

If the JSON structure is as simple as shown, then the same could be achieved differently: walk only unique elements and jsonize 
the output:
```bash
bash $ <<<$jsn jtc -w'><q:' -j
[
   "string",
   true,
   null,
   3.14
]
bash $ 
```

But there's a reverse action:
#### Remove all the elements but the ones which have duplicates
```bash
bash $ <<<$jsn jtc -w'<>Q:' -pp
[
   "string",
   null
]
bash $ 
```
That one is obvious - we just reversed the prior example.

Another way to achieve the same:
```bash
bash $ <<<$jsn jtc -w'><q:' -p
[
   "string",
   null
]
bash $ 
```

How about:
#### Leave only those which have no duplicates
```bash
bash $ <<<$jsn jtc -w'<Dup>Q:[^0]<Dup>s:' -p
[
   true,
   3.14
]
bash $ 
```
it's just a tiny bit more complex:
- `<Dup>Q:` - for each duplicate element, we'll memorize it into `Dup` namespace, then
- `[^0]<Dup>s:` reset the search path back to the root and now find all the elements (i.e., all duplicates).

that way all duplicates (and their origins) will be removed, leaving the array only with those which have no duplicates.

and finally
#### Leave all duplicates
```bash
bash $ <<<$jsn jtc -w'<Dup>Q:[^0]<Dup>s:' -pp
[
   "string",
   null,
   "string",
   null
]
bash $ 
```
it's just a reverse action.


### Counting with `jtc`
Counting any number of properties is JSON could be done using external `wc` unix utility. E.g., let's count all `number`s in `ab.json`:
```bash
bash $ <ab.json jtc -w'<number>l:' | wc -l
       6
bash $ 
```

However, the same is possible to achieve using `jtc` capability - with `<..>I` lexeme:
```bash
bash $ <ab.json jtc -w'<number>l:<cnt>I1' -T{cnt} -x/-1
6
bash $ 
```
- `<cnt>I1` will arrange a namespace var `cnt` counting values starting from `0` with increment of `1` upon each walk pass (iteration)
- `-T{cnt}` will interpolate it
- `-x/-1` will display the last walk only

Say, now we want to count the same phone numbers, but for some reason starting from the offset `100`:
```bash
bash $ <ab.json jtc -w'<number>l:<cnt:100>I1' -T{cnt} -x/-1
106
bash $ 
```

Finally, let's count home numbers and mobile numbers separately:
```bash
bash $ <ab.json jtc -x'<phone>l:' -y'<home>:<hn>I1' -y'<mobile>:<mn>I1' -T'{"total home numbers":{hn},"total mobile numbers":{mn}}' -x/-1
{
   "total home numbers": 2,
   "total mobile numbers": 3
}
bash $ 
```


### Transposing a matrix
Say, we have a matrix to transpose:
```bash
bash $ mtx='[[0,1,2,3,4],["a","b","c","d","e"],[null,true,2,"3",[4]]]'
bash $ <<<$mtx jtc -tc
[
   [ 0, 1, 2, 3, 4 ],
   [ "a", "b", "c", "d", "e" ],
   [
      null,
      true,
      2,
      "3",
      [ 4 ]
   ]
]
bash $ 
```

We can arrange walking through each slice while memorizing incremental index within the slice, it's trivial:
```bash
bash $ <<<$mtx jtc -w'[:][:]<I>k' -jr
[ 0, 1, 2, 3, 4, "a", "b", "c", "d", "e", null, true, 2, "3", [ 4 ] ]
bash $ 
```

However, we need to re-arrange such output per each new, transposed matrix with number of columns `<->` rows.
That could be facilitated if we label each value with the row index:
```bash
bash $ <<<$mtx jtc -w'[:][:]<I>k' -T'{"{I}":{{}}}' -r
{ "0": 0 }
{ "1": 1 }
{ "2": 2 }
{ "3": 3 }
{ "4": 4 }
{ "0": "a" }
{ "1": "b" }
{ "2": "c" }
{ "3": "d" }
{ "4": "e" }
{ "0": null }
{ "1": true }
{ "2": 2 }
{ "3": "3" }
{ "4": [ 4 ] }
bash $ 
```

The last step is to reach out for labels inside each object (`-ll`) and then regroup the output per each new group:
```bash
bash $ <<<$mtx jtc -w'[:][:]<I>k' -T'{"{I}":{{}}}' -ll / -jw[:] -tc
[
   [ 0, "a", null ],
   [ 1, "b", true ],
   [ 2, "c", 2 ],
   [ 3, "d", "3" ],
   [
      4,
      "e",
      [ 4 ]
   ]
]
bash $ 
```
DONE.


### Sorting JSONs
Search Lexemes `<>g`, `<>G` allow walk and thus sort entries in an ascending or descending order respectively. Let's sort
entries in the Directory of `ab.json` by `city` in the ascending order.

First, let's walk `city`es in the ascending order:
```bash
bash $ <ab.json jtc -w'[city]:<>g:'
"Denver"
"New York"
"Seattle"
bash $ 
```

Sorting is achieved with the insert-move operations of the entries in the sorted order:
```bash
bash $ <ab.json jtc -w'[Directory]' -pi'[city]:<>g:[-2]' -tc
{
   "Directory": [
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
      },
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
      }
   ]
}
bash $ 
```

When walking lexemes `<>g`, `<>G` the sorting order occurs for JSONs using the following priority resolution order:  
_null JSON_ < _boolean JSON_ < _numerical JSON_ < _string JSON_ < _JSON array_ < _JSON object_  
E.g.: an empty _JSON object_ `{}` wins (has a better weight) over any array of any size, and so on.

When comparing the same iterable types (comparing atomic values is trivial) the following priority resolution order is applied:
- an iterable of the bigger size wins, otherwise (sizes are the same):
- deeper JSON wins over the shallower one, otherwise (both have the same nestedness):
- compared values child-by-child defines a winner, otherwise (all children values are the same):
- if it's an object then the labels are compared, otherwise (if it's an array, or all the labels are the same):
- JSON values are equal

### More examples and case studies
[Here](https://github.com/ldn-softdev/stackoverflow-json/blob/master/README.md)
you could find some answers using **`jtc`** for JSON queries taken from
[stackoverflow.com](https://stackoverflow.com/questions/tagged/jq+or+bash+json)








