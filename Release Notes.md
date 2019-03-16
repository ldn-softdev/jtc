
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
  - `<..>k`: save into a namespace current JSON element's label/index
  - `<..>v`: save into a namespace current element's JSON value
  - `<..>z`: erase given name from the namespace (`<>z` will clear entire namespace)
  - `<..>s`: search recursively (off the current node) for the JSON matching one in the namespace  
  - `<..>t`: search recursively (off the current node) for the label matching one in the namespace  
#### Improvements, changes, fixes:
- improved parser debug alignments and aligned indentation for relevant calls/data structures
- made debugs unicode UTF-8 compatible
- debug locus now shows offset correctly and in UTF-8 unicode characters
- fixed issue, where a non-recursive array search (`>...<i`) might produce a false positive match for objects
