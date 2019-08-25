## `jtc` Release Notes
_Release Notes for `jtc` v.1.73_

#### New features:
- No new features, some enhancements and stability improvements


#### Improvements, changes, fixes:
- lifted _label update_ operation when `-u` is used to update a label (when a walk-path is ending with an empty `...<>k` lexeme): 
now it's possible to update/rewrite recursively even nested labels w/o failures
- converted walking (walk iteration) to a non-recursive loop, now walks are virtually endless (i.e. able to walk JSONs of virtually
ANY size and depth) and not restricted by a depth of a stack
- `-T` processing for `-i<walk>` and `-u<walk>` operations is enhanced to match the same behavior as for `-w<walk>`:
templates are interpolated per walks now (if a count of templates and walks matches), or round-robin fashion otherwise
(before, for some weird reasons *all* templates were applied for each such walk)
- fixed insertion (`-i`) when the last lexeme of a walk is non-empty `<..>k` then no _label reinterpretation_ occurs
(so it's consistent now with the same behavior of `-u`)
- removed support for the empty `<>z` notation form of the lexeme: erasing entire _namespace_ is idiomatically inconsistent with the
_walk design_ (and might lead to confusion or misunderstanding of the expected behavior), so only non-empty lexemes `<..>z` are 
supported now (and restricted to)
- fixed a crash when debugging is on (quite a corner case though)
- fixed a programmatic error (rarely occurs only in API calls) where Json class would falsely expect `<stdin>` in the event when 
   parsing constructor throws
***


_Release Notes for `jtc` v.1.72_

#### New features:
- introduced a new directive `<>I` which let incrementing/decrementing numerical JSONs preserved in the namespace (and ignore other
JSON types), e.g.: `<var>I3`, `<var>I-1`. If `var` wasn't defined before, the iteration begins with `0`; 
however, it's possible to initialize it with other than `0` values - see User Guide
- introduced an auto-namespace variable `$?` to reference the last processed walk, this facilitates use-cases when converting
input JSON to `.csv` format; see User Guide for more
- introduced new lexemes `<..>P`, `<..>N` to match any _JSON strings_ and _JSON numerical_ types respectively. Before, to facilitate the
same, REGEX lexemes were used: `<.*>R` and `<.*>D` respectively, but new lexemes work faster and allow capturing matched values in
the namespace)
- Template-interpolation was enhanced with new capability to _jsonize JSON strings_ (containing embedded JSONs) and _stringify JSONs_ - 
similar to respective options `-qq` and `-rr` but now programmatically. See User Guide for the syntax and examples
- added a new semantic to `-x` option: `-xn[/N]` notation lets specifying a frequency of walks to be displayed - (every `n`th walk) 
staring from the optional offset `N` (zero based); e.g.: `-x4` - display every _4th_ walk, while `-x4/1` will do the same starting
from the 2nd (`N` index is zero based) walk.
Also, note a special notation case: `-x0/N` - will display `Nth` (zero based) walk only _once_, this could be truncated to `-x/N`; 
`N` is positive, but also supported `-1` value - to display _the last_ walk

#### Improvements, changes, fixes:
- improved `-jl` options combination behavior: in some cases it wasn't robust and failed providing the expected result. 
Plus, introduced a new merge format: `-jlnn` - all clashing values will be aggregated (disrespecting JSON structured grouping vs. as in 
the case of `-jl`)
- lifted handling of _atomic JSONs_ - simplified the code allow applying _walk-paths_ now even onto the atomic JSON values
- extended null-interpolation to _JSON strings_ as well: before it was applied for _JSON arrays_ and _JSON objects_ only). 
Now, the empty variable interpolation in the string, following either of `,`, `;` will be taken into account,
e.g.: `-T'"{}, "'` - if `{}` is empty, then result of interpolation will be empty too: `""`
- improved buffered file read speed (3 times faster) and _stdin_ buffered speed (1.5-2 times faster), improved handling of 
non-existent/bad file-arguments (when multiple given)
- enhanced _move_ semantic of `-u`, `-i` operations, so that when used together with `-pp` it also works as expected with those options 
(before it was only working for `-p` and `-pp` notation was ignored)
- a last in the walk `k` lexeme (e.g.: `-w'... <>k'`) is only subjected for re-interpretation of the label if it's empty (`<>k`) now;
the non-empty `<..>k` lexeme then doesn't (the value is preserved in the namespace, so no need to re-interpret it then)
- _template pertain per walk_ feature used to work only for _interleaved_ walks and `-n` was cancelling it (template then were applied 
round-robin). Now, even with `-n` (i.e., for sequenced walk processing) templates are also pertained per walks, 
in the unlikely event when round robin behavior is required `-nn` notation will support it.
Also, _template pertain per walk_ feature is enhanced to be engaged only when number of _templates_ (`-T`)
matches the number of provided walks (`-w`), otherwise _round-robin_ template application behavior is engaged.
- put a hard cutoff on a too deep recursion shall any unforeseen case (while walking) occurs in the future;
the same enhancement has fixed a case of too deep recursion (with subsequent stack overflow) for a corner case of lexeme `<>F`
usage occurring in processing really huge JSONs only
- the message "_notice: option -J cancels streaming input_" is printed now only when `-a` + `<stdin>` were selected explicitly together
with `-J` (and not when `-a` is implicitly imposed upon `-J`)
- fixed parsing debug (offset for a _streamed_ read now shows a correct value - from the beginning of a stream, instead of
the beginning of an internal circular buffer, other read debugs (_buffered_, _cin_) are unaffected)
- fixed empty `<>b` lexeme - it was not working (as documented), now it matches any boolean JSON value (UT'ed) 
***


_Release Notes for `jtc` v.1.71_

#### New features:
- new lexeme: `<..>u` - user evaluation of the current walk-path via shell cli - walking will continue if cli return `0` (success) 
and fails otherwise. The cli in the lexeme is subjected for namespaces interpolation
- full support of `if` ... `else` walk branching via pairing `f` and `F` lexemes:
   - ... `<>f` {if path does not fail, then skip it} `<>F` {otherwise execute this path} ...
   - ... `<>f` {if path does not fail, then end walking} `><F` {otherwise execute this path} ...
   - ... `<>f` {if path does not fail, then end walking} `><F` `<>F` # otherwise skip it (i.e. skip the failed path)
- `jtc` now supports _STREAMED_ type of read, engaged with `-a` and selected `<stdin>`; so `jtc` could be used in between pipes
allowing transforming JSONS "on the fly": `... | jtc -a ... | ...`
- multiple files could be fed to `jtc` now (before it used to accept only one input file)

#### Improvements, changes, fixes:
- search lexemes and some directives (`naoicewvkfF`) now can write currently walked JSON values into the namespaces, e.g.: `<var>a`, 
  though `<var:"lbl">k` directive supports only JSON string types (understandably, labels could be represented only as JSON strings); 
  thus a walk path previously written as `<>a<var>v` now could be collapsed into `<var>a`
- extend `<..>j` search lexeme, allowing the lexeme value to be a _templated_ JSON, e.g.: `<{ "{lbl}": {{val}} }>j`; however, it makes
the search lexeme `j` a _dynamic type_ when used with interpolation template (unlike one with a _static_ JSON), therefore, like
all dynamic lexemes it is an exempt for caching, hence it's prone to exponential decay noticeable upon big recursive searches - be aware
when building a query for huge input JSONs
- for insert/update operations (`-i`, `-u`), destination walks' (`-w`) are related now to the source walk of operations (in 
`-i`, `-u`) and so are their namespaces; now it becomes possible to make an insert/update based on the cross references from the 
destination walks using namespaces
- swap option `-s` now accept any number of walk _pairs_ to be swapped (before it was only swapping first 2 specified), e.g.: 
`jtc -s -w<1_swap_with_next> -w<1_swap_with_prior> -w<2_swap_with_next> -w<2_swap_with_prior> ...`
- extended option `-l`: once given as `-ll` - if the record does not have a label (i.e. it's a root, or inside an array) **and** the 
record is the _JSON object_, then it takes the (first) label from the object and its value and uses as a labeled record
(allows stripping templated values with labels), e.g.: `jtc ... -T'{"label": {{value}} }' -ll` lets interpolated template to be treated
as the value with the label rather than as a JSON object
- option `-p` is enhanced to work with cases of mixed args, eg: `jtc ... -u<static> -u<walk> -p` before it wasn't in use, but now
it will purge all walked _destinations_ - it's done in order to facilitate a cross-referenced purging (so here `-u`/`-i` is just
a dummy operation only providing ability to cross-reference purge points)
- fixed auto-generated REGEX namespaces to be properly pertained per walk when cached 
***


_Release Notes for `jtc` v.1.70_

#### New features:
- no new features, quick stability update

#### Improvements, changes, fixes:
- updated code base to make `c++17` compatible
- fixed (and UT'ed) a peculiar template interpolation bug, where sometimes namespace interpolation might not work as expected
- dropped macOS 32-bit binary, as Apple deprecates 32-bit support past Mojave
***


_Release Notes for `jtc` v.1.69_

#### New features:
- no new features, mostly improved behavior for namespaces and templating

#### Improvements, changes, fixes:
- namespaces, in case of *multiple interleaved walks* (`-w`), respect now the correct order of iterations (before namespaces
were reflecting only a correct order from the last given walk-path `-w`)
- template behavior for *interleaved walks* only (i.e. when templating (`-T`) is used for walks only,
not with `-i`, `-u`) is enhanced:
   * if *multiple* templates are given, then each template corresponds/attributes to each relevant walk (`-w`)
   * in all other cases (one template multiple walks, one walk multiple templates, multiple walks and multiple templates but
   sequential / non-interleaved processing - `-n` ) then templates are getting applied as before - in a round-robin fashion
- when unquoting (`-qq`) is used on an empty string (`""`) then resulting blank line is not printed
***


_Release Notes for `jtc` v.1.68_

#### New features:
- directive `<..>F` is liberated: now it can be not the last one (explanations in the 
[User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#fail-stop-and-forward-directives))
- extended walking use-cases for `<..>f`: the lexeme also will act upon failed iterable subscripts too (before it used to 
engage on failing atomic values only), plus walking continues past `<..>F` directive if the latter is present - in such 
case the failing domain is only covered within `<>f` ... `<>F` lexemes
- interpolation with stripping (`{}`) now extended to _JSON iterables_ (`[..]` and `{..}`), allows extending _JSON iterables_
during interpolation (before `{}` was stripping only _JSON strings_ only)

#### Improvements, changes, fixes:
- improved interleaved walking relevance (removed directives out of considerations when relating walks)
- improved quotation for shell cli interpolated values (fixed issue when unexpected quoting might occur in literals 
followed quoting character present in the latter)
- fixed recursive interpolation (when the interpolated JSON value accidentally may undergo interpolation over again)
- fixed a corner case usage for `<..>f` directive with ranged lexemes when it could have caused an infinite loop
***


_Release Notes for `jtc` v.1.67_

#### New features:
- added `<>F` directive - upon walking this one (which is predicated by successful walk of all prior steps) makes walking to proceed 
to the next iteration without producing a match or a fail-stop; the lexeme is supposed to be only the last lexeme in the walk path
(all further walk lexeme will be discarded) and is meant to be used with (after) `<>f` lexeme. Both lexemes make negative match 
queries to handle with ease - see [User Guide](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md)) for examples

#### Improvements, changes, fixes:
- binary search lexeme `<>b` now may be empty, semantically it's equal to `<any>b` (the latter form is deprecated)
- drastically improved characters quotations and interpolation for shell evaluation parameters (i.e.: `-e -u ... \;`). Unfortunately,
quite lame implementation of symbol quoting in prior versions might break some corner cli syntax, thus,

_NOTE: if you use `jtc` with `-e` option, please check that it still works before migrating to the new version.
It might require a bit of tinkering of an old
[cli syntax](https://github.com/ldn-softdev/jtc/blob/master/User%20Guide.md#insert-update-argument-shell-evaluation)_

_Also, the cli chars quotation is tuned to be used with *`bash`* shell, however for compatibility with all others the entire cli
could just quoted_
***


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
- added namespaces (allow capturing JSON elements in the named space while walking paths); namespaces are reused in templates and in
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

