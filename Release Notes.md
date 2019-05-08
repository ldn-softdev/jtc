
## `jtc` Release Notes
_Release Notes for `jtc` v.1.66_

#### New features:
- no new features, stability and improvements update 

#### Improvements, changes, fixes:
- fail-stop directive `<..>f` now is fully compatible and operational with any other walk lexemes, all prior limitations are lifted
(before it was limited by usage with `[-N]`, `[^N]` subscripts) and also now it may take an argument as a namespace (where a 
fail-stop'ed JSON will go upon engaging the lexeme)
- Extended directives `<..>v` and `<..>f` - if it's given in the format like `<name:json_value>v`, then `json_value` is put
into the namespace `name` ignoring currently walked JSON element - that allows setting up in the namespace custom JSON values;
`json_value` value must be a valid JSON though
- template behavior is finalized:
   * when template fails (i.e. its interpolation does not result in a valid JSON), then it's not engaged (source JSON is used instead), 
   except when the interpolation occurs for the shell evaluation - there template is used as it was given: in shell evaluation
   a template parameter it's indistinguishable from the cli, thus user must ensure a valid template interpolation
   * fixed/improved templates interpolation in shell evaluation (before only the last memorized item was interpolating)
   * lifted restriction on shell return result (if it's good one and not a shell error, or empty one) - if the result of a shell
   evaluation is not a valid JSON, then it's promoted to the _JSON string_ (before the result must always be a valid JSON)
   * fixed namespaces in shell interpolation with trailing options (e.g.: `jtc -eu ... \; -u...`) - now the namespace is tracked
   from the trailing option (before the namespace was tracked from the `-w` option, which in that syntax acts as the destination,
   which was illogical)
   * when performing a shell interpolation (whether a simple or an extended version - with trailed options) templating occurs during 
   expansion of shell cli itself, thus option -T has no effect here and entirely ignored

_all the behaviors and use-cases (and those coverd in the 
[User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md)) are completely UT'ed_

***


_Release Notes for `jtc` v.1.65_

#### New features:
- added `<>f` (fail-stop) directive: the directive "remembers" the currently walked JSON node (path) and shall the further walking 
fail, instead of proceeding to the next iteration, will stop and "recall" the node/path 

#### Improvements, changes, fixes:
- relative offsets with range quantifiers (`>..<t:` and `>..<l:`) now interprets default "head value" not as '0' but rather as 
'the beginning of the _JSON iterable_' - that extends use-cases base (i.e before, to address prior range entries user must know the
number of prior entries to be able to address, which is rarely the case)
- for search type `>..<t`, when a value in the NS represents a _JSON number_, index operation now applied onto both types 
of _JSON iterables_ - arrays and objects (before it was applied to arrays only, and for objects, label match was performed, 
which, for _JSON objects_ used to duplicate `>..<l` functionality)
- improved walk-step iteration logic (eliminated redundant walk-step in some cases - a cosmetic issue from functionality point of view)
***


_Release Notes for `jtc` v.1.64_
#### New features:
- added an option `-zz` to display JSON size only - it'll skip displaying JSON (unlike `-z` that will display JSON first
and then the size).

#### Improvements, changes, fixes:
- cacheing is extended to walking subscripts as well (before only search walk lexemes were covered). Now ranged subscripts are
cached (`[N:N]`) and static / numerical subscript when addressing _JSON objects_ (`[N]`); _JSON arrays_ addressed directly, thus
such operation is not prone to an exponential decaying and hence does not require cacheing.
***


_Release Notes for `jtc` v.1.63_
#### New features:
- added auto-generated name-spaces `$PATH` and `$path` - they represent a path to the currently selected JSON element. `$PATH`
represent it as a _JSON array_, while `$path` stores it as a _JSON string_ value
- added quantifier interpolation from the namespaces, i.e. a search in notation like `<..>{..}`, where a search
quantifier `{..}` resolved from the namespace (all types of quantifiers supported with all search suffixes, e.g.: `>blah<R:{lbl}`)
- quantifier semantic in search types `>..<l` and `>..<t` now let addressing values by relative offsets, i.e., selecting 
a neighbor of matched entry (either successive or preceding), so it becomes possible to select "neighbors" of the found entries

#### Improvements, changes, fixes:
- a big rework of the cache engine:
  - before it only was engaged for recursively ranged searches (`<..>n:n`) and was cacheing entire JSON (off a given node)
  - now, it's engaged for both search types recursive and non-recursive (`<..>`, `>..<`)
  - it caches up to (but not beyond) specified range, e.g. `>..<N`, - will build the cache up to N'th entry only
  - cache will be extended automatically if a next search (lexeme of walk) addresses beyond existing cache size, e.g.: `>..<:M`,
  where `M`>`N`
  - only non-cacheable suffixes are of dynamic types: [`stqQ`] (understandably, their result dynamically depends on the current
  state of the namespace) and searches with relative quantifiers (`>..<l` and `>..<t`)
- improved debugging for walk-step visualization, now it's UTF-8 compatible
- enforcing check on the empty types of lexemes (like `<>a` now must be always empty, otherwise exception is printed)
- added check for those which must be non-empty (e.g.: `<>R` - such empty lexeme is deemed invalid, exception printed)
- fixed a quite corner case bug where a too low subscript quantifier could result in an incomplete walking in a subsequent lexeme
- updated built-in user guide (`-g`)
***

_Release Notes for `jtc` v.1.62_
#### New features:
- added option `-a` process all input (and valid) JSONs (by default only first JSON is read and the rest of the input ignored)
- added option `-J` jsonize into a _JSON array_ all input (and processed) JSONs (option `-a` assumed); `-J` has similar relation
to input JSONs as option `-j` to provided walks (`-w`)
- added search lexeme `<..>q` - search and match only unique JSON elements; matched unique values recored into a provided namespace
- added search lexeme `<..>Q` - opposite to `q` lexeme: search and match all duplicate JSON elements, matched duplicates placed into a
provided namespace

#### Improvements, changes, fixes:
- option `-j` applies now not to walks only (as it was before), but to all operations
- fixed accidentally broken in-quoting (`-rr`, now UT'ed).
***


_Release Notes for `jtc` v.1.61_
#### New features:
A small, but very useful update: RE search suffixes (`R`, `L`, `D`) now auto-populate namespaces:
 * `$0` the namespace created for matched entire RE
 * `$1` is created for the 1st subgroup,
 * `$2` is created for the 2nd subgroup, and so on.

Interpolation of such namespaces occurs usually, e.g.: `{$1}`

#### Improvements, changes, fixes:
 * minor correction in the `jtc` guide (`-g`)
***


_Release Notes for `jtc` v.1.60_
#### New features:
- added `-jj` option to jsonize into JSON object (in addition to `-j` which jsonizes into JSON array)
- added support for JSON templates, option `-T` let specifying template(s); templating work for walking, insert/update/compare operations
(and when shell-evaluated as well)
- settled on unified interpolation semantic for templates and shell-evaluation
- added namespaces (allow storing JSON elements in the named space while walking paths); namespaces are reused in templates and in
walk-paths
- added new search keys and directives facilitating namespaces: `<..>k`, `<..>v`, `<..>z`, `<..>s`, `<..>t`
  - `<..>k`: save current JSON element's **k**ey (label/index); it also reinterprets label/index as the value
  - `<..>v`: save into a namespace current element's JSON **v**alue
  - `<..>z`: **z**ip (erase) given name from the namespace (`<>z` will clear entire namespace)
  - `<..>s`: **s**earch (off the current node) for the JSON matching one in the namespace  
  - `<..>t`: search (off the current node) for the **t**ag (label) matching one in the namespace
#### Improvements, changes, fixes:
- improved parser debug alignments and aligned indentation for relevant calls/data structures
- made debugs unicode UTF-8 compatible
- debug locus now shows offset correctly and in UTF-8 unicode characters
- fixed issue, where a non-recursive array search (`>...<i`) might produce a false positive match for objects
- changed semantic for directive `<>v`: in the previous version (1.59) it used to facilitate the same function as `<>k` now
- substitution operation now replaced with interpolation: removed parsing of `{-}`,`[]`,`[-]`; interpolation now occurs
with `{}` and `{{}}`, the latter one interpolates JSON as it is, the former one affects string interpolation only - the string value
is interpolated without outer quote marks

