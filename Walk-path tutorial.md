

# [`jtc`](https://github.com/ldn-softdev/jtc). Walk-path easy. Tutorial (under construction)

`Walk-path` is a way to telling `jtc` how input JSON must be walked. 

1. [Walk-path Lexemes](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#walk-path-Lexemes)
   * [Grasping a gist of a walk-path in 1 minute](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#grasping-a-gist-of-a-walk-path-in-1-minute)
2. [Subscript lexemes (`[..]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#subscript-lexemes)
   * [Numerical offsets (`[n]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#numerical-offsets)
   * [Literal subscripts (`[text]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#literal-subscripts)
   * [Range subscripts (`[n:m:s]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#range-subscripts)
     * [Default range indices (`[:]`, `[n:]`, `[:n]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#default-range-indices)
       * [Alternative range notation (`[+n]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#alternative-range-notation)
     * [Ranges with positive indices (`[n:m]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#ranges-with-positive-indices)
     * [Ranges with negative indices (`[-n:-m]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#ranges-with-negative-indices)
   * [Addressing parents](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#addressing-parents)
     * [Offsetting path from a leaf (`[-n]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#offsetting-path-from-a-leaf)
     * [Offsetting path from the root (`[^n]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#offsetting-path-from-the-root)
3. [Search lexemes](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#search-lexemes)
   * [String searches (`<text>r`, `<RE>R`, `<>P`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#string-searches)
   * [Quantifiers (`n`, `n:`, `:n`, `:`, `n:N`, `{n}`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#quantifiers)
   * [Recursive vs Non-recursive search (`<..>` vs `>..<`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#recursive-vs-non-recursive-search)
   * [Numerical searches (`<n>d`, `<RE>D`, `<>N`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#numerical-searches)
   * [Boolean and Null searches (`<>b`, `<>n`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#boolean-and-null-searches)
   * [Json types searches (`<>P`,`<>N`,`<>b`,`<>n`,`<>a`,`<>o`,`<>i`,`<>c`,`<>e`,`<>w`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#json-types-searches)
   * [Arbitrary Json searches (`<JSON>j`,`<ns>s`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#arbitrary-json-searches)
   * [Original and Duplicate searches (`<..>q`,`<..>Q`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#original-and-duplicate-searches)
   * [Label searches (`<..>q`,`<..>Q`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#label-searches)
     * [Non-recursive behavior of label lexemes (`>..<l`,`>..<t`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#non-recursive-behavior-of-label-lexemes)
     * [Relative quantifiers (`>..<l`,`>..<t`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#relative-quantifiers)
   * [Scoped searches (`[label]:<..>`, `[label]:>..<`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#scoped-searches)
   * [Regex searches (`<..>R`, `<..>L`, `<..>D`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#regex-searches)
4. [Directives and Namespaces](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#directives-and-namespaces)
   * [Preserve a currently walked value in the namespace (`<..>v`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#preserve-a-currently-walked-value-in-the-namespace)
   * [Preserve a label of a currently walked (`<..>k`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#preserve-a-label-of-a-currently-walked)
   * [Erase namespace (`<..>z`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#erase-namespace)
   * [Walk branching](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#walk-branching)
     * [Fail-safe directive (`<..>f`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#fail-safe-directive)

---

## Walk-path Lexemes
`Walk-path` is an argument of `-w` option (but not only, other options may also accept `walk-path`s).

`Walk-path` is made of _lexemes_ (optionally separated with the white spaces).  
A _lexeme_ - is an atomic walk-step that `jtc` applies when traversing JSON tree. `jtc` always begins walking of a walk-path
starting from a _JSON root_.

If upon walking (i.e. applying _lexemes_, a.k.a. _walk-steps_) applying of a lexeme fails, such walk-path is considered to be empty
(non-existent) and therefore skipped (not displayed). Only _successfully finished_ walk-paths will be processed (displayed).  
_In order to succeed walking a walk-path, all its lexemes must be walked successfully_

There are only two types of lexemes:
- [_Subscripts_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#subscript-lexemes),
enclosed in square brackets `[..]`
- [_Searches_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#search-lexemes),
enclosed in angular brackets `<..>, >..<`

Though each type comes in several variants.

### Grasping a gist of a walk-path in 1 minute
1. Subscript lexemes `[..]` allow traversing JSON trees downwards and **upwards (!)**
2. Subscript lexemes allow iterating over an arbitrary number of values in any of JSON iterables
3. search lexemes `<..>, >..<` allow searching JSON trees recursively and non-recursively 
4. search lexemes allow iterating over a number of found matches in a controlled way 

E.g.: say, we have a JSON:
```bash
bash $ jsn='[{"number":"111-23456","type":"mobile"},{"number":"222-34567","type":"work"},{"number":"223-45678","type":"work"},{"number":"333-45678","type":"home"}]'
bash $ <<<$jsn jtc -tc
[
   { "number": "111-23456", "type": "mobile" },
   { "number": "222-34567", "type": "work" },
   { "number": "223-45678", "type": "work" },
   { "number": "333-45678", "type": "home" }
]
bash $ 
```
Say, we want to extract all `work` types of numbers? Applying the explained concepts to build a walk-path is easy: 
1. find recursively all `"work"` entries,
2. step 1 level up (from the each found entry) towards the root (i.e. traverse JSON tree _upwards_)
3. select (subscript) `number` value (for the each found record)

So, let's do it (build a walk-path) one step at a time:
```bash
# 1. find recursively all `"work"` entries:
bash $ <<<$jsn jtc -w'<work>:'
"work"
"work"
bash $ 

# 2. step 1 level up (from the each found entry) towards the root (i.e. traverse JSON tree _upwards_):
bash $ <<<$jsn jtc -w'<work>:[-1]'
{
   "number": "222-34567",
   "type": "work"
}
{
   "number": "223-45678",
   "type": "work"
}
bash $ 

# 3. select (subscript) `number` value (for the each found record)
bash $ <<<$jsn jtc -w'<work>:[-1][number]'
"222-34567"
"223-45678"
bash $ 
```
\- see how easy it becomes to accomplish such JSON query given ability to search JSON recursively and traverse it up and down?

**_That's it, you have it now!_**  
Well, there's more to it, but you've got the most frequent use case for sure. The rest of a tutorial
you may use it just as a reference to diffent types of lexemes and their uses


## Subscript lexemes
There are few variants of _subscripts_ (`[..]`):
- [_numerical offsets_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#numerical-offsets)
- [_literal subscripts_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#literal-subscripts)
- [_range (slice) subscripts_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#range-subscripts)
- [_parent selects_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#addressing-parents)

let's start with the most common one - _numerical offsets_

##
### Numerical offsets
`[n]` - like in most programming languages, in `jtc` numerical offsets let indexing `n`th instance in the currently selected
(walked) JSON, staring from `0` (indices are always zero-based):

Let's work with this JSON:
```bash
bash $ jsn='["abc", false, null, { "pi": 3.14}, [ 1,"two", {"number three": 3}] ]'
bash $ <<<$jsn jtc -tc
[
   "abc",
   false,
   null,
   { "pi": 3.14 },
   [
      1,
      "two",
      { "number three": 3 }
   ]
]
bash $ 
```

- select the _1st_ element from the top _JSON array_:
```bash
bash $ <<<$jsn jtc -w[0]
"abc"
bash $ 
```

\- select a _5th_ element in _JSON array_:
```bash
bash $ <<<$jsn jtc -w[4] -tc
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 
```

If the selected element is non-atomic (a.k.a. _iterable_), i.e., a _Json array_, or a _JSON object_, then you may continue digging
further the selected (walked, found) JSON tree:

\- select the _5th_ element in _JSON array_ and then the _3rd_ one:
```bash
bash $ <<<$jsn jtc -w[4][2] -tc
{ "number three": 3 }
bash $ 
```

If we try selecting a 2nd element from the resulted JSON (which has only a single element), walking will fail and the output
will be blank:
```bash
# indexing further the 2nd element fails:
bash $ <<<$jsn jtc -w[4][2][1]
bash $

# while indexing the 1st one yields the final result:
bash $ <<<$jsn jtc -w[4][2][0]
3
```
>_Note_: numerical offset is treated like one only if it's spelled like shown (`[n]`) - no white space allowed and `n` must be spelled
as a valid number, otherwise it's treated as a 
[_literal subscript_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#literal-subscripts).
E.g.: `[ 0 ]` will address an element with the label `" 0 "`. But that poses a question - how to address elements labeled with
a numerical literals, e.g.: `"0"`? Hold on, we'll get to it.


##
### Literal subscripts
`[text]` - literal subscripts allow addressing (indexing) elements within _JSON objects_ with their keys (labels)

There are two elements in the above JSON that are addressable with _literal subscripts_, let's get to them using _literal subscripts_.
First, let's get to `pi`'s value:
```bash
# first, get to to the object:
bash $ <<<$jsn jtc -w[3]
{
   "pi": 3.14
}
bash $ 

# then idex a value within the object by its label:
bash $ <<<$jsn jtc -w[3][pi]
3.14
bash $ 
```

Now, let's get to the `number three`'s value:
```bash
# dig the first level:
bash $ <<<$jsn jtc -w[4] -tc
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 
#
# dig the second level:
bash $ <<<$jsn jtc -w[4][2] -tc
{ "number three": 3 }
#
# dig the 3rd level:
bash $ <<<$jsn jtc -w[4][2][number three]
error: could not open file 'three]'
jtc json parsing exception (three]:0): unexpected_end_of_string
bash $ 
```
\- why? 
\- it happens because of a _shell interpolation_. Shell treats space as an argument separator, therefore option `-w`
ends up only with partial argument, namely with `[4][2][number`, while `three]` is treatated as a standalone (file) argument,
which is an invalid (empty) read, hence the exception.

To escape the shell interpolation, either the whole argument, or a space symbol must be quoted (the former variant is preferred, but
both will work):
```bash
# quote the entire walk-path (preferred)
bash $ <<<$jsn jtc -w'[4][2][number three]'
3
bash $ 

# quote the spacer
bash $ <<<$jsn jtc -w[4][2][number\ three]
3
bash $ 
```

The elements within objects also could be addressed using _numerical offsets_:
```bash
bash $ <<<$jsn jtc -w[4][2][0]
3
bash $ 
```
\- it seems _numerical notation_ is more succinct, why then bother using literal offsets? Because _**our assumptions of the order
of elements within _JSON objects_ are fragile**_.

Say, there's a following JSON:
```bash
bash $ anml='{ "ANDEAN BEAR": "Bono", "AMUR TIGER": "Shadow", "GRIZZLY BEAR": "Goofy" }'
```
And we want to get get the name of `ANDEAN BEAR`. Being lazy one can do it by a _numerical offset_, assuming here that the index
of the required entry would be `0` (indeed, it's listed first there in the object), let's see:
```bash
bash $ <<<$anml jtc -w[0]
"Shadow"
```
Bummer! Instead of selecting the name of `ANDEAN BEAR` we got the name of `AMUR TIGER`. That's because our assumption of the index
**was wrong**.

_JSON standard defines JSON objects as an **_unordered set_** of name/value pairs_.
That means that the order of elements (name/value pairs) within _JSON objects_ will be defined by a parser
(and not by user, like in _JSON arrays_). Some programs will retain the same order, others will reorder them -
it all boils down to the internal implementation specifics.

`jtc` always rearranges all the elements within the _JSON objects_ by their keys (labels) in the _alphabetical_ order, 
thus for `jtc` the above JSON looks like this:
```bash
bash $ <<<$anml jtc 
{
   "AMUR TIGER": "Shadow",
   "ANDEAN BEAR": "Bono",
   "GRIZZLY BEAR": "Goofy"
}
bash $ 
```
That is a serious enough reason to select elements in JSON objects by their keys/labels:
```bash
bash $ <<<$anml jtc -w'[ANDEAN BEAR]'
"Bono"
bash $ 
```
##
There's a curious case, when the label matches a numerical subscript, i.e. consider:
```bash
bash $ jsn='{ "0": 12345, "#": "abcd"}'
bash $ <<<$jsn jtc
{
   "#": "abcd",
   "0": 12345
}
bash $ 
```

Addressing _JSON root_ with `[0]` will return `"abcd"`:
```bash
bash $ <<<$jsn jtc -w[0]
"abcd"
bash $ 
````
- How to get to the value of the label `"0"`? For that we need to use a _non-recursive search_ lexeme (namely, `>0<l`).
> _NOTE_: there's a generic rule for all other types of subscripts: _If parsing of a subscript does not result in either types
(i.e. it's neither a numerical offsets, nor a range subscript, nor addressing parents), then it's treated as a literal subscript.

##
### Range subscripts
`[n:m:s]` - selects elements in a JSON _iterable_, starting from `n`th index and ending with `m`th - 1 with the step `s`, i.e. `m` 
is the index of the element following the last in the range. Both values `n`, `m` and `s` are optional and both could be omitted.

For those who are familiar with _Python addressing_, grasping this one is easy - it's matches Python's addressing concept.

Range subscript makes the walk-path iterable, i.e. it's like selecting multiple elements with just one _iterable walk_ 
instead of specifying multiple offsets, compare:
```bash
bash $ jsn='["abc", false, null, { "pi": 3.14}, [ 1,"two", {"number three": 3}] ]'

# select multiple values individually:
bash $ <<<$jsn jtc -w[0] -w[1] -w[2]
"abc"
false
null
bash $ 

# select the same values using range:
bash $ <<<$jsn jtc -w[0:3]
"abc"
false
null
bash $ 
```
##
#### Default range indices 
Either of indices in the _range subscript_ `n`, `m` and `s` could be omitted, then the index in the omitted position takes 
a _default_ value:
- a _default_ index in the first position (`n`) means: from the very first value in an _iterable_,  
- a _default_ index in the second position (`m`) means: till the last value in the _iterable_,
- a _default_ value in the third position (`s`) means: with the step of `1`

it's quite handy when we need to select only portion of the elements in an iterable either form its beginning, or till it's
last element, because sometimes we might not know upfront a number of elements in the iterable, e.g.:  
- select 2 elements from the beginning of the _JSON root's_  iterable:
```bash
bash $ <<<$jsn jtc -w[:2]
"abc"
false
bash $ 
```
- select _all_ elements staring from a 3rd one:
```bash
bash $ <<<$jsn jtc -w[2:] -tc
null
{ "pi": 3.14 }
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 
```
when both indices are missed `[:]` then each element in the iterable will be selected (_walked_):
```bash
bash $ <<<$jsn jtc -w[:] -tc
"abc"
false
null
{ "pi": 3.14 }
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 
```
##
The _range indices_ (as well as any lexemes) can appear in the walk-path _any number of times_. The above example shows iterating over
the _top iterable_ (or, the first tier) in JSON tree hierarchy, to iterate over _all_ iterables in the second tier of the JSON tree,
do this:
```bash
bash $ <<<$jsn jtc -w[:][:] -tc
3.14
1
"two"
{ "number three": 3 }
bash $ 
```
\- an each element in the _top iterable_ will be _walked_ and then attempted to walk the _children_ of the walked element itself, 
one by one.
Because first three elements are not iterable, they will not be shows (they cannot be iterated over):
```bash
bash $ <<<$jsn jtc -w[0][:]
bash $ 
```

If you like to see (print) both walks of the top iterable and then each of the iterable at the second tier, then provide two walk paths:
```bash
bash $ <<<$jsn jtc -w[:] -w[:][:] -tc
"abc"
false
null
{ "pi": 3.14 }
3.14
[
   1,
   "two",
   { "number three": 3 }
]
1
"two"
{ "number three": 3 }
bash $ 
```
>Note how `jtc` _interleaves_ the walks: it puts relevant walks in a good (relevant) order, rather than dumping results of
the first walk and then of the second. If one prefers seeing the latter behavior, option `-n` will do the trick, compare:
```bash
bash $ <<<$jsn jtc -w[:] -w[:][:] -tc -n
"abc"
false
null
{ "pi": 3.14 }
[
   1,
   "two",
   { "number three": 3 }
]
3.14
1
"two"
{ "number three": 3 }
bash $ 
```

##
##### Alternative range notation
`[+n]` is the alternative range notation for `[n:]`, they both do exactly the same thing - walk each element in the iterable starting
from `n`th element:
```bash
# dump all top entries starting from 4th using [+n] notation:
bash $ <<<$jsn jtc -w[+3] -tc
{ "pi": 3.14 }
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 

# dump all top entries starting from 4th using [n:] notation:
bash $ <<<$jsn jtc -w[3:] -tc
{ "pi": 3.14 }
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 
```
Using either of notations is a matter of personal preference and has no impact onto the way of walking JSON tree

##
#### Ranges with positive indices
Positive indices (and `0`) in the range notation (`[n:m]`) always refer to the index offset _from the beginning_ of the iterable.

When both `n` and `m` are positive, naturally `m` must be > `n`, if `m` <= `n`, it'll result in a blank output:
```bash
bash $ <<<$jsn jtc -w[2:1]
bash $ 
bash $ <<<$jsn jtc -w[2:2]
bash $ 
```

Case where `m` = `n` + 1, e.g., `[3:4]` is equal to spelling just a _numerical offset_ alone:
```bash
bash $ <<<$jsn jtc -w[3:4] -tc
{ "pi": 3.14 }
bash $ <<<$jsn jtc -w[3] -tc
{ "pi": 3.14 }
bash $ 
```

##
#### Ranges with negative indices
A negative index in the _range subscript_ refers to the offset _from the end_ of the iterable. In the range subscripts it's okay to
mix and match positive and negative indices in any position (except position `s`, which must stay positive).

- select _last 3 elements_ from the top array:
```bash
bash $ <<<$jsn jtc -w[-3:] -tc
null
{ "pi": 3.14 }
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 
```

- select _all_ elements in the range _2nd from the start_ till the _2nd from the end_:
```bash
bash $ <<<$jsn jtc -w[1:-1]
false
null
{
   "pi": 3.14
}
bash $ 
```

##
When either of indices is given outside of the _actual range_ of the iterable, `jtc` tolerates it fine re-adjusting respective range
indices properly to the beginning and the end of actual range of the iterable:
```bash
bash $ <<<$jsn jtc -w[-100:100] -tc
"abc"
false
null
{ "pi": 3.14 }
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 
```
However, when the range is unknown, it's best to use the notation with the
[_default range_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#default-range-indices)
values (i.e., `[:]`)

##
### Addressing parents
One of the nifty features that makes `jtc` very powerful when coming up with queries, is the ability to address parents off the walked
elements, i.e., those JSON elements higher up in the the JSON hierarchy tree (closer to the root) that make the path towards 
the currently walked element.

There are 2 ways to address parents:
- `[-n]` will address parent(s) in the path (made of offsets from the _JSON root_ to the currently walked element) offsetting it
from the currently walked element
- `[^n]` will do address parent(s) but offsetting it from the JSON _root_

Not sure if the definition above is easy to understand, but the concept is, so it's probably much easier to show it with the example.  
Let's see the walk path where we selected the JSON element `3`:
```bash
bash $ <<<$jsn jtc -w'[4][2][number three]'
3
bash $ 
```
The _**walk path**_ from the _JSON root_ towards the element `3` is `[4][2][number three]`.

##
In fact, every walk at any given step (even when it's done via _recursive search_ lexemes) internally always maintains a 
representation expressed via _subscripts and literal offsets_ only.  
E.g., the same number `3` could have been selected using a _recursive search_ walk:
```bash
bash $ <<<$jsn jtc -w'<3>d'
3
bash $ 
```
but internally, the path towards this JSON element would be built as:
```bash
bash $ <<<$jsn jtc -w'<3>d' -dddd 2>&1 | grep "built path vector"
....walk_(), built path vector: [00000004]->[00000002]->[number three]
....walk_(), finished walking: with built path vector: [00000004]->[00000002]->[number three]
bash $ 
```
i.e. it still would be `[4][2][number three]`. That's why `jtc` is known to be a _**`walk-path`**_ based utility.

##
#### Offsetting path from a leaf
Thus, if we list indices for the above walk-path starting from the leaf, it'll be like this:
```
Index from the leaf:   -3  -2 -1      -0
          walk-path: (root)[4][2][number three]
```
Thus in order to select either of parents, we just need to pick a respective index in the path. E.g.:
- `[-1]` will address an immediate parent of the value `3` 
- `[-2]` will address a parent of the parent of the value `3` 
- `[-3]` wil address the _JSON root_ itself.
>_Note_: `[-0]` will address the value `3` itself, so there's no much of a point to use such addressing, while indices greater _root's 
(in that example are `[-4]`, `[-5]`, etc will keep addressing the JSON root)_. Take a look:
```bash
# addressing an immediate parent:
bash $ <<<$jsn jtc -w'[4][2][number three][-1]' -tc
{ "number three": 3 }
bash $ 

# addressing a parent of a parent:
bash $ <<<$jsn jtc -w'[4][2][number three][-2]' -tc
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 

# addressing the next parent (which happens to be the root):
bash $ <<<$jsn jtc -w'[4][2][number three][-3]' -tc
[
   "abc",
   false,
   null,
   { "pi": 3.14 },
   [
      1,
      "two",
      { "number three": 3 }
   ]
]
bash $ 
```

##
#### Offsetting path from the root
Now, let's list all the indices for the same walk-path starting from the root:
```
Index from the root:    0   1  2       3
          walk-path: (root)[4][2][number three]
```
You must get already the idea: the addressing parent off the root takes those indices:
```bash
# addressing a root parent:
bash $ <<<$jsn jtc -w'[4][2][number three][^0]' -tc
[
   "abc",
   false,
   null,
   { "pi": 3.14 },
   [
      1,
      "two",
      { "number three": 3 }
   ]
]
bash $ 

# addressing a parent (of the walked) next after root:
bash $ <<<$jsn jtc -w'[4][2][number three][^1]' -tc
[
   1,
   "two",
   { "number three": 3 }
]
bash $ 

# addressing a next parent after root (which happens to be an immediate parent of the last walked):
bash $ <<<$jsn jtc -w'[4][2][number three][^2]' -tc
{ "number three": 3 }
bash $ 

# futher addressing has no effect:
bash $ <<<$jsn jtc -w'[4][2][number three][^3]'
3
bash $ 
```

Let's recap both addressing schemas (for the given walk in the example) on the same diagram:
```
                                                           etc.
                                                           [^4]
to address a parent from the root: [^0]   [^1]  [^2]       [^3]
                                    |      |     |          |
                                    v      v     v          v
                        walk-path: root > [4] > [2] > [number three]
                                    ^      ^     ^          ^
                                    |      |     |          |
to address a parent from the leaf: [-3]   [-2]  [-1]       [-0]
                                   [-4]
                                   etc.
```

Yes, agree, addressing parents when a walk-path is made of subscripts only is a dull idea (and here it's done only for
the instructive purposes) - indeed, we just walked that path from the root, why getting back using _parent addressing_
instead of stopping it at the required place?
Ergo, it makes sense to use parent addressing together with (after) _search lexemes_.


## Search lexemes
Search lexemes allow performing various searches across JSSON tree, there are two major notations for search lexemes:
 - `<expr>` - performs a _**recursive**_ search of `expr` from the currently selected JSON element
 - `>expr<` - performs a _**non-recrusive**_ search of `expr` for a currently selected _JSON iterable_

A complete notation for search lexemes (both, recursive and non-recursive), look like this:  
`<expr>SQ` (`>expr<SQ`), where:
- `expr` is a content of the lexeme, depending on the _lexeme suffix_, its semantic may vary: it could be either of: 
   - a value to match
   - a [Regular Expression](https://en.wikipedia.org/wiki/Regular_expression) to search for
   - a namespace (think of a _namespace_ as of a variable that can hold any JSON type/structure)
   - a template
   - empty
- `S` is an optional one-letter suffix that defines the behavior of the lexeme
- `Q` is a quantifier, whose function generally is analogous to the function of _numerical offset_ and _range subscripts_, but in some
cases also might vary, as per documentation. the quantifier must also follow the suffix (if one present).

Also, there's a few lexemes that look like _search lexemes_ but in fact they don't perform any type of search, 
instead they apply a certain action, they are known as _directives_, those are distinguishable from the searches only by the suffix

##
### String searches
`r`, `R`, `P` - these are suffixes to perform _JSON string_ searches. Suffix `r` is default and can be omitted:
- `<text>` - searches for the occurrence of exact match of `text` in the JSON tree (off the currently walked element)
- `<Regexp>R` - performs an RE search for the _regular expression_ `Regexp`
- `<>P`, `<namespace>P` - matches any JSON string value (a.k.a a _JSON string type_ match), similar to `<.*>R` but faster. 
The lexeme might be empty or hold the `namespace` where matched value will be stored

Examples:
- Find an exact string value:
```bash
bash $ <<<$jsn jtc -w'<two>'
"two"
```

- Find a string value matching _RE_:
```bash
bash $ <<<$jsn jtc -w'<^t>R'
"two"
```

- Find the first _JSON string_ value:
```bash
bash $ <<<$jsn jtc -w'<>P'
"abc"
```

##
### Quantifiers
By default any search lexeme is going to find only a first match occurrence. That is facilitated by a default quantifier `0`. 
If there's a need to find any other match instances (or range of instances) a quantifier must be given. 

Quantifiers may be given in either of following forms:
- `n` - search will find `n`th match
- `n:` - search will find all matches starting from `n`th till the last matched one
- `:N` - search will find all matches starting from the first (index `0`) till `N`th
- `n:N` - search will find all matches starting from `n`th till `N`th - 1
- `:` - search will find all matches

Observe following rules applied to all forms of quantifiers:
1) in any of the above notations indices (`n`, `N`) are zero based  
2) both indices `n`, `N` must be positive numbers (or `0`). There's only one case where quantifier may go negative (see 
[Relative quantifiers (`>..<l`,`>..<t`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#relative-quantifiers))  
3) either or both of indices `n`, `N` may take a form of `{Z}`, where `Z` is a namespace holding a JSON numeric value representing
an index

Some examples:
let's work with this JSON:
```bash
bash $ JSS='["one", "two", ["three", "four", {"5 to 7": [ "five", "six", "seven"], "second 1": "one"  } ] ]'
bash $ <<<$JSS jtc
[
   "one",
   "two",
   [
      "three",
      "four",
      {
         "5 to 7": [
            "five",
            "six",
            "seven"
         ],
         "second 1": "one"
      }
   ]
]
```

- among all _JSON strings_ find those from 2nd till 5th inclusive:
```bash
bash $ <<<$JSS jtc -w'<>P1:5'
"two"
"three"
"four"
"five"
```
##
As it was mentioned, the quantifier indices may take values from the namespaces. Namespaces will be covered later, 
when _directives_ covered, for now just take it: one way to set a value to the namespace is `<var:value>v`.

So, let's repeat the last example, but now using quantifier indices references in the namespaces:

- among all _JSON strings_ find those from 2nd till 5th inclusive:
```bash
bash $ <<<$JSS jtc -w'<Start:1>v<End:5>v <>P{Start}:{End}'
"two"
"three"
"four"
"five"
```
##

- find all the string occurrences where letter `e` is present:
```bash
bash $ <<<$JSS jtc -w'<e>R:'
"one"
"three"
"five"
"seven"
"one"
``` 

- find all the occurrences of string `"one"`:
```bash
bash $ <<<$JSS jtc -w'<one>:'
"one"
"one"
```

##
### Recursive vs Non-recursive search
In the last example, 2 instances of the string `"one"` were found. That's because a recursive search was applied
(and hence the entire JSON tree was searched). Sometimes there'a need to perform a non-recursive search, i.e. to look for a match
only among immediate children of a current _iterable_.

the JSON's root in the example is an _array_, so if we apply a non-recursive search on the root's array, only one match will be found:
```bash
bash $ <<<$JSS jtc -w'>one<:'
"one"
```

_NOTE_: the other subtle but a crucial difference is that a _non-recursive_ search_ could be applied only on _JSON iterables_
(i.e. _arrays_ and _objects_) and it will fail on any other (atomic) types. While a _recursive search_ could be applied onto
_any_ JSON type (even atomic).

The recursive search always begins from checking the currently selected (walked) entry, that's why it's possible to apply it
even onto atomic types and match those:
```bash
bash $ <<<$JSS jtc -w'[0]<one>'
"one"
```

- that feature of the recursive search comes handy when validating various JSON types (covered later)

##
### Numerical searches
`d`, `D`, `N` - these are numerical searches suffixes, they share the same relevant semantics as 
[_string searches_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#string-searches):
- `<number>d` - searches for the occurrence(s) of exact match of a `number` in the JSON tree
- `<Regexp>D` - performs an RE search for the _regular expression_ `Regexp` among _JSON numericals_
- `<>N`, `<namespace>N` - matches any JSON numerical value (a.k.a. _JSON numerical type match_), similar to `<.*>D` but faster.
The lexeme might be empty or hold the `namespace` where matched value will be preserved (upon a match)

```bash
bash $ <<<$jsn jtc -w'<[13]>D1:'
1
3
```
```bash
bash $ <<<$jsn jtc -w'<3.14>d:'
3.14
```

##
### Boolean and Null searches
`b` suffix stands for a _boolean_ match, while `n` is a null match.

A boolean lexeme can be given in the following forms:
- `<>b`, `<namespace>b` - in these forms, the search is performed among _JSON boolean_ values only and matched value will be preserved
in the `namespace` shall it be present in the lexeme
- `<true>b`, `<false>b` - when a _JSON boolean_ is spelled as a lexeme parameter, then it's not a _namespace_ reference,
but rather a spelled boolean value will be matched

```bash
bash $ <<<$jsn jtc -w'<>b:'
false
```

##
### Json types searches
There are quite a handful of lexemes that search and match _JSON types_, in fact there are lexemes to cover _all_ _JSON type matches_
and even more. Four of those already have been covered:
[_string type match_ `<>P`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#string-searches),
[_numerical type match_ `<>N`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#numerical-searches),
[_boolean type match_ `<>b` and _null type match_ `<>n`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#boolean-and-null-searches).
The others are:
- `<>a`: atomic match, will match _any_ of JSON atomic type (_string_, _numerical_, _boolean_, _null_)
- `<>o`: object match, will match a _JSON object type_ (`{..}`)
- `<>i`: array (indexable) match, will match a _JSON array type_ (`[..]`)
- `<>c`: container type match, will match either of _JSON iterable type_ (objects and/or arrays)
- `<>e`: end node (leaf) match type, will match any of atomic types, or _empty_ containers (`{}`, `[]`)
- `<>w`: wide type range match - will match _any_ JSON type/value 

All of those lexemes can stay empty, or hold the _namespace_ that will be filled upon a successful match.

```bash
bash $ <<<$jsn jtc -rw'<>c:'
[ "abc", false, null, { "pi": 3.14 }, [ 1, "two", { "number three": 3 } ] ]
{ "pi": 3.14 }
[ 1, "two", { "number three": 3 } ]
{ "number three": 3 }
```


##
### Arbitrary Json searches
lexeme with the suffix `j` can match any arbitrary JSON value:
```bash
bash $ <<<$jsn jtc -w'<{ "pi":3.14 }>j'
{
   "pi": 3.14
}
```

Even more, the parameter in the `j` lexeme can be a _templated JSON_:
```bash
bash $ <<<$jsn jtc -w'[4][2][0] <Nr3>v [^0] <{"pi": {Nr3}.14}>j [pi]'
3.14
```

That was the first complex walk-path shown, so, let's break it down:
- `'[4][2][0]` will get to the value of `"number three": 3` through _offset subscripts_
- `<Nr3>v` - _directive_ `v` will memorize the _JSON number_ `3` in the namespace `Nr3`
- `[^0]` will reset the walk path back to _JSON root_
- `<{"pi": {Nr3}.14}>j` will first interpolate number `3` from the namespace `Nr3` and then will find recursively
the resulted JSON (which will be `{"pi": 3.14}`)
- `[pi]` will address the value in found JSON by the label offset, resulting in the final value `3.14`

Obviously the `j` lexeme cannot be empty or result in an empty lexeme after template interpolation 
(as the empty space is not a valid JSON, as per spec). 

##
There's another search lexeme suffix - `s` - that one will find a JSON pointed by a _namespace_:
```bash
bash $  <<<$jsn jtc -w'<PI:{"pi": 3.14}>v <PI>s'
{
   "pi": 3.14
}
bash $ 
```

The `s` lexeme also cannot be empty (it always must point to some namespace).

##
### Original and Duplicate searches
`q` and `Q` lexemes allow finding original (first time seen) and duplicate elements respectively within the selected (walked) JSON tree. 
The lexemes cannot be empty - they point to a namespace which will be _**overwritten**_ during the search and will be set
to the found element (original or duplicate) once the match is found.

lexemes search for original or duplicate entries of any JSONs, not necessarily atomic types, here's an example:
```bash
bash $ JSD='{"Orig 1": 1, "Orig 2": "two", "list": [ "three", { "dup 1": 1, "dup 2": "two", "second dup 1": 1 } ]}' 
bash $ <<<$JSD jtc 
{
   "Orig 1": 1,
   "Orig 2": "two",
   "list": [
      "three",
      {
         "dup 1": 1,
         "dup 2": "two",
         "second dup 1": 1
      }
   ]
}
```

Let's see _all_ the original elements in the above JSON:
```bash
bash $ <<<$JSD jtc -lrw'<org>q:'
{ "Orig 1": 1, "Orig 2": "two", "list": [ "three", { "dup 1": 1, "dup 2": "two", "second dup 1": 1 } ] }
"Orig 1": 1
"Orig 2": "two"
"list": [ "three", { "dup 1": 1, "dup 2": "two", "second dup 1": 1 } ]
"three"
{ "dup 1": 1, "dup 2": "two", "second dup 1": 1 }
```
As you can see there were listed _all_ first seen JSON values (including the root itself)

Now, let's list _all_ the duplicates:
```bash
bash $ <<<$JSD jtc -lrw'<dup>Q:'
"dup 1": 1
"dup 2": "two"
"second dup 1": 1
```

_CAUTION_: both of the lexemes facilitate their functions by memorizing in the namespace _all_ the original values
(from walked JSON node), thus both of them are quite memory hungry - keep it in mind when walking huge JSONs

##
### Label searches
`l`, `L`, `t` - are suffixes to perform label based searches, they facilitate different kinds of label matching depending on the
type of a search:
- `<lbl>l` - finds recursively a JSON value with the label matching `"lbl"` exactly
- `<RE>L` - finds recursively a JSON value with the label matching a regular expression _RE_
- `<NS>t` - finds recursively a JSON value matching exactly the JSON string from the _namespace_ `NS`
- `>lbl<l` - _addresses_ an immediate child in the _JSON object_ with the label `"lbl"`
- `>RE<L` - _finds_ among immediate children in the JSON object a value with the label matching a regular expression _RE_
- `>NS<t` - _addresses_ an immediate child in the _JSON object_ with the label from the _namespace_ `NS`,
or _addresses_ an immediate child in the _JSON iterable_ with the numerical index from the _namespace_ `NS`

First two variants should not require much of a clarification, let's work with the following JSON:
```bash
bash $ JSL='{"One": 1, "obj": { "One": true, "Two": 2, "": 3 }, "45": "forty-five"}'
bash $ <<<$JSL jtc
{
   "45": "forty-five",
   "One": 1,
   "obj": {
      "": 3,
      "One": true,
      "Two": 2
   }
}
```
```bash
bash $ <<<$JSL jtc -rlw'<[oO]>L:'
"One": 1
"obj": { "": 3, "One": true, "Two": 2 }
"One": true
"Two": 2
```
```bash
bash $ <<<$JSL jtc -rlw'<One>l:'
"One": 1
"One": true
````

##
recursive form `<NS>t` will try matching a label from the specified namespace. The JSON type in the `NS` might be either _JSON string_, 
or _JSON numeric_, in the latter case, it's automatically converted to a _string value_ and also can match a label expressed as
a numerical value:
```bash
bash $ <<<$JSL jtc -lrw'<idx:45>v <idx>t'
"45": "forty-five"
```
```bash
bash $ <<<$JSL jtc -lrw'<idx:"45">v <idx>t'
"45": "forty-five"
```
All other _JSON types_ in the `NS` will be ignored, such search will always return _false_.

##

#### Non-recursive behavior of label lexemes
Normally, a non-recursive search will try matching a value among immediate children of the _JSON iterable_. 
But for matching a label or index, the actual search (i.e., iterating over children of an iterable) is a superfluous task:  
indeed, when we want to match a value by a label (in a _JSON object_) or by an index (in a _JSON array_), we should be able 
to do so just by addressing them.  

The non-recursive lexeme `>..<l` can match/address labels in _JSON objects_ only. _**That makes it (with a default quantifier) just
another variant of a
[literal subscripts](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#literal-subscripts)**_.
The latter lacks one ability - to address labels spelled as numerical value: in the above JSON, it won't be possible to address a JSON
value `"forty-five"` via literal subscript, but using `>..<l` lexeme it is:
```bash
bash $ <<<$JSL jtc -rlw'[45]'
bash $ 
bash $ <<<$JSL jtc -rlw'>45<l'
"45": "forty-five"
```

The lexeme `>..<t` can do both _JSON objects_ and _arrays_:  
\- if a _namespace_ referred by the lexeme has the type _JSON string_, then it will address the label (from the namespace)
in the JSON object:
```bash
bash $ <<<$JSL jtc -lw'<lbl:"45">v >lbl<t'
"45": "forty-five"
```
\- if the lexeme's _namespace_ is set to _JSON numeric_ type, then it can address _JSON iterables_ by the index:
```bash
bash $ <<<$JSL jtc -lw'<idx:2">v [obj]>idx<t'
"Two": 2
```

##
#### Relative quantifiers
There's another feature of how these lexemes operate. Think of a quantifier instance for the lexemes: 
any _parsed_ objects will hold only 1 unique label - there cannot be two equal labels among immediate children 
of the same _JSON object_. 

##
Even though it's possible to pass to a parser an object that will hold non-unique labels, the JSON RFC 
([8259](https://tools.ietf.org/html/rfc8259))
_**does not define**_ software behavior in that regard. `jtc` in such case retains the first parsed value:
```bash
bash $ <<<'{"abc":1, "abc":2}' jtc
{
   "abc": 1
}
```
_NOTE_: holding two non-unique labels would render such _JSON object_ non-addressable: indeed, in the above JSON, 
if the object were to hold both values, which value then to select when addressed by the label `"abc"`?
##

Getting back to the quantifiers: _any parsed objects will hold only **one** unique label, and so does array with its indices_. If so,
the usual semantic of quantifiers as a search instance in lexemes `>..<l` and `>..>t` is moot: we know that in the _parsed_ object
cannot be a second, third, forth (and so on) the same label, there can be only one. The same applies to arrays: there can be only one 
unique index in there, thus only instance `0` (the first and the only instance) makes sense.

Thus, a usual semantic of quantifier as a _match instance_ (except instance `0`) in these lexemes is meaningless. 

Therefore it was overloaded with a different and quite handy one: _a quantifier in these non-recursive lexemes allows
addressing neighbors (sibling) of the matched entry_. I.e., the quantifier here becomes _**relative**_ (to the matched entry)
and therefore can take a _negative value_ - that is the only case when a quantifier may go negative.

Observe a relative quantifier in action:
```bash
bash $ <<<$JSL jtc -w'[obj]'
{
   "": 3,
   "One": true,
   "Two": 2
}
```
```bash
bash $ <<<$JSL jtc -lw'[obj] >One<l'
"One": true
```
```bash 
bash $ <<<$JSL jtc -lw'[obj] >One<l-1'
"": 3
```
```bash
bash $ <<<$JSL jtc -lw'[obj] >One<l1'
"Two": 2
````

The relative quantifiers though are fully compatible with the quantifiers range-notation: 
```bash
bash $ <<<$JSL jtc -lw'[obj] >One<l:'
"": 3
"One": true
"Two": 2
```

##
### Scoped searches
When you would like to perform a search but only among values under a specific _label_, it's known as a _scoped search_. 
The syntax for a scoped search is rather symple: a 
[literal offset](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#literal-subscripts) is appeded wtih the 
[search lexeme](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#search-lexemes) over colon `:`, 
e.g.:  
`[some label]:<search lexeme>`  
All form of quantifiers and search suffixes are supported, except label searches: `l`, `L` and `t` - understandably,
a label cannot be scoped by a label.

For example:
```bash
bash $ <<<$JSL jtc -w'[One]:<org>q:'
1
true
```


##
### Regex searches
All search lexemes supporting _regex_ based searching have been already covered:
[`<..>R`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#string-searches),
[`<..>L`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#label-searches),
[`<..>D`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#numerical-searches),
they support (RE) matching of _strings_, _labels_ and _numerical_ JSON values respectively.

There's one more aspect to it though: the _regular expression_ also support _sub-groups_. Upon a successful match,
the subgroups will automatically setup respective namespaces, e.g.: the first subgroup will populate a namespace `$1`, 
the second will do `$2` and so on. Plus, the entire match will populate the namespace `$0`.

That way it's possible to extract any part(s) from the found JSON values for a later re-use.
```bash
bash $ <<<$JSL jtc -w'<(.*)[oO](.*)>L:' -T'{ "sub-group 1":{{$1}}, "sub-group 2":{{$2}}, "entire match":{{$0}} }'
{
   "entire match": "One",
   "sub-group 1": "",
   "sub-group 2": "ne"
}
{
   "entire match": "obj",
   "sub-group 1": "",
   "sub-group 2": "bj"
}
{
   "entire match": "One",
   "sub-group 1": "",
   "sub-group 2": "ne"
}
{
   "entire match": "Two",
   "sub-group 1": "Tw",
   "sub-group 2": ""
}
```

##
### Directives and Namespaces
##### Directives:
There're few lexemes that look like searches but they do not do any searching/matching, instead they apply certain actions
onto the currently walked JSONs elements or paths. They are known as _directives_. Directives are distinguishable from the search
lexemes only by the suffix.

Directives are typically agnostic to recursive or non-recursive forms of spelling, except one - `F`, 
where the spelling has a semantical meaning. Also, the directives do not support quantifiers (the quantifiers are parsed, 
but silently ignored).

##
##### Namespaces:
_Namespace_ is a way to facilitate _variables_ in `jtc`. The _namespace_ is implemented as a container (in the currently processed 
JSON) that holds all (sub) JSON values preserved during waling a walk-path.

The JSON values found in the _namespace_ could be re-used later either during the _interpolation_, or in other lexemes of the same
walk or in different (subsequent) walks.

The namespace could be populated (setup) by a lexeme either from currently walked JSON element, or with an _**arbitrary**_ JSON value 
(in the lexeme). e.g.:

In this walk-path example a search lexeme `a` will find (recursively) the first occurrence of any _atomic JSON_ and will 
populate the namespace _Atomic_ with a found JSON element:  
`-w'<Atomic>a'`  
In this walk-path example, the same search lexeme will populate the namespace _Atomic_ with the empty JSON array `[]`
instead of a found atomic JSON:  
`-w'<Atomic:[]>a'`  

The ability (and the form) to setup an arbitrary JSON value is _universal_ for all lexemes (and directives)  that are capable
of preserving values in the namespace.


##
#### Preserve a currently walked value in the namespace
The directive `<NS>v` preserves currently walked value in the _namespace_ `NS`. 
[Many search lexemes](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#json-types-searches)
are capable of doing the same on their own, but for others, as well as for the subscripts, it's still a useful feature.

```bash
bash $ <<<$jsn jtc
[
   "abc",
   false,
   null,
   {
      "pi": 3.14
   },
   [
      1,
      "two",
      {
         "number three": 3
      }
   ]
]
```
```bash
bash $ <<<$jsn jtc -w'[4][0]<Idx>v[-1]>Idx<t'
"two"
```

It's fun to see how `jtc` works in a slow-mo, building a walk-path step by step, one lexeme at a time:
```bash
bash $ <<<$jsn jtc -w'[4]'
[
   1,
   "two",
   {
      "number three": 3
   }
]
```
\- addressed there the 5th JSON element in the JSON root (always begin walking from the root)  
##
```bash
bash $ <<<$jsn jtc -w'[4][0]'
1
```
\-  addressed the 1st JSON value in the JSON iterable (found in the prior step)  
##
```bash
bash $ <<<$jsn jtc -w'[4][0]<Idx>v'
1
```
\- memorized a currently walked JSON in the namespace `Idx` (which is the _JSON numeric_ `1`)  
##
```bash
bash $ <<<$jsn jtc -w'[4][0]<Idx>v[-1]'
[
   1,
   "two",
   {
      "number three": 3
   }
]
```
\- stepped one level up (towards the root) from the last walked JSON  
##
```bash
bash $ <<<$jsn jtc -w'[4][0]<Idx>v[-1]>Idx<t'
"two"
```
\- using a value from the namespace `Idx` found an offset in the JSON iterable (the numeric value `1` stored in `Idx` points to a
2nd element in the JSON array, b/c all indices are _zero-based_)


##
### Preserve a label of a currently walked
The directive `<NS>k` functions pretty much like `<NS>v`, but instead of preserving a JSON value, it'll store in the namespace `NS`
its label (if currently walked element is a child of _JSON object_), or its index (if the currently walked element is a child
of a _JSON array_):

```bash
bash $ <<<$jsn jtc
[
   "abc",
   false,
   null,
   {
      "pi": 3.14
   },
   [
      1,
      "two",
      {
         "number three": 3
      }
   ]
]
```
```bash
bash $ <<<$jsn jtc -w'<{"pi":3.14}>j<idx>k' -T'{idx}'
3
```

If the lexeme is _**empty**_ (`<>k`) _AND_ is the last one in the walk-path, then it does not memorize (obviously) the label/index 
in the namespace, but instead re-interprets the label as the JSON value. That way it become possible to rewrite labels in update (`-u`)
operations, or re-use it in template interpolation.
```bash
bash $ <<<$jsn jtc -w'<{"pi":3.14}>j<>k'
3
```
```bash
bash $ <<<$jsn jtc -w'<{"pi":3.14}>j<>k' -T'{"idx": {{}}}' -r
{ "idx": 3 }
``` 
The described effect occurs only if the empty `<>k` lexeme appears the last in the walk-path, if the lexeme appears somewhere in the
middle of the walk-path, the lexeme is completely meaningless in that form and has no effect at all.


##
### Erase namespace
The directive `<NS>z` allows erasing the namespace `NS`. Mostly, this would be required when used together with 
_[walk branching](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#walk-branching)_.

For example, let's replace all even numbers in the array with their negative values:
```bash
bash $ <<<$'[1,2,3,4,5,6,7,8,9]' jtc -w'<Num>z[:]<>f<[02468]$>D:<Num>v' -T'-{Num}' -jr
[ 1, -2, 3, -4, 5, -6, 7, -8, 9 ]
```

If the walk began w/o initial lexeme erasing namespace `Num`, then the whole attempt would fail:
```bash
bash $ <<<$'[1,2,3,4,5,6,7,8,9]' jtc -w'[:]<>f<[02468]$>D:<Num>v' -T'-{Num}' -jr
[ 1, -2, -2, -4, -4, -6, -6, -8, -8 ]

```

Of course, knowing _how
[Regex lexemes](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#regex-searches)
work_, it's possible to rewrite the walk-path in a bit more succinct way:
```bash
bash $ <<<$'[1,2,3,4,5,6,7,8,9]' jtc -w'<$0>z[:]<>f<[02468]$>D:' -T'-{$0}' -jr
[ 1, -2, 3, -4, 5, -6, 7, -8, 9 ]
```

##
### Walk branching
Normally, all the lexemes in the _walk-path_ are contatenated with the logical operator _AND_ (i.e., a walk is suscessful only
if _all_ lexemes are).  
Directives `<..>f`, `<..>F` and `><F` introduce walk branching (the easiest way to think of it as of `if .. else ..`), 
i.e. they facilitate a control-flow logic of the walk-path exectution.

_Note_: the direcitve `F` is sensitive to the lexeme spelling (a recursive vs a non-recursive form) and provides different
reactions for each of the form (this is the only directive so far that is sensitive to the lexeme encasement, all others are not). 

##
#### Fail-safe directive
`<>f` is a _fail-safe_ directive (facilitating `if` part). Once walked, it memorizes the internally maintained path to the
currently walked JSON element and reinstate it, shall the walk _past_ `<>f` directive fails:
























