
## `jtc` Release Notes
_Relese Notes for `jtc` v.1.60_
#### New features:
- added `-jj` option to jsonize into JSON object (in addition to `-j` which jsonizes into JSON array)
- added support for JSON templates, option `-T` let specifying template(s); templating work for walking, insert/update/compare operations
(and when shell-evaluated as well)
- settled on unified interpolation semantic for templates and shell-evaluation
- added namespaces (allow storing JSON elements in the named space while walking paths); namespaces are reused in templates and in
walk-paths
- added new search keys facilitating namespaces: `<..>k`, `<..>v`, `<..>z`, `<..>s`, `<..>t`
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
