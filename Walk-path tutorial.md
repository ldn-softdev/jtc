
# [`jtc`](https://github.com/ldn-softdev/jtc). Walk-path easy. Tutorial (under construction)

`Walk-path` is a way to telling `jtc` how input JSON must be walked. 

1. [Walk-path Lexemes](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#walk-path-Lexemes)
2. [Subscripts](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#subscripts)
   * [Numerical offset (`[n]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#numerical-offset)
   * [Literal subscript (`[text]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#literal-subscript)
   * [Range subscripts (`[n:N]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#range-subscripts)
     * [Default range indices (`[:]`, `[n:]`, `[:n]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#default-range-indices)
       * [Alternative range notation (`[+n]`)](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#alternative-range-notation)
     * [Ranges with positive indices](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#ranges-with-positive-indices)
     * [Ranges with negative indices](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#ranges-with-negative-indices)

---

## Walk-path Lexemes
`Walk-path` is an argument of `-w` option (but not only, other options may also accept `walk-path`s).

`Walk-path` is made of _lexemes_.  
A _lexeme_ - is an atomic walk-step which `jtc` applies when traversing JSON tree. `jtc` always begins walking of any walk-path
starting from the _JSON root_.

If upon walking (i.e. applying _lexemes_, a.k.a. _walk-steps_) applying of a lexeme fails, such walk-path is considered to be empty
(non-existent) and therefore not displayed. Only _successfully finished_ walk-paths will be displayed.  
_In order to succeed walking a walk-path, all its lexemes must be walked successfully_

There are only two types of lexemes:
- _Subscript_, enclosed in square brackets `[`, `]`
- _Searches_, enclosed in angular brackets `<`,`>`
Though each type comes in several variants.


## Subscripts
There are few variants of _subscripts_:
- _numerical offsets_
- _textual (literal) subscripts_
- _range (slice) subscript_
- _parent selects_

let's start with the most common one - _numerical offset_

##
### Numerical offset
`[n]` - as like in most programming languages, in `jtc` numerical offsets let selecting `n`th instance in the currently selected
(walked) JSON, staring from `0` (indices are always zero-based):

Let's work with this JSON:
```
bash $ JSN='["abc", false, null, { "pi": 3.14}, [ 1,"two", {"three": 3}] ]'
bash $ <<<$JSN jtc
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
bash $ 
```

- select _1st_ element in _JSON array_:
```
bash $ <<<$JSN jtc -w[0]
"abc"
bash $ 
```

\- select _5th_ element in _JSON array_:
```
bash $ <<<$JSN jtc -w[4]
[
   1,
   "two",
   {
      "number three": 3
   }
]
bash $ 
```

If the selected element is non-atomic (a.k.a. _iterable_), i.e., _Json array_, or _JSON object_, then you may continule digging
further the selected (walked) JSON tree:

\- select _5th_ element in _JSON array_ and then _3rd_ one:
```
bash $ <<<$JSN jtc -w[4][2]
{
   "number three": 3
}
bash $ 
```

If we try selecting a 2nd element from the resulted JSON (which has only single element), walking will fail and the output
will be blank:
```
bash $ <<<$JSN jtc -w[4][2][1]
bash $ 
bash $ <<<$JSN jtc -w[4][2][0]
3
bash $ 
```
##
_Note_: numerical offset is treated like one only if spelled like shown (`[n]`) - no white space allowed and `n` must be spelled
as a valid number, otherwise it's treated as a 
[_literal subscript_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#literal-subscript).
E.g.: `[ 0 ]` will address an element with the label `" 0 "`.

##
### Literal subscript
`[text]` - literal subscripts allow addressing (selecting) elements within _JSON objects_ by their key (label)

There are two elements in the above JSON that are addressable with _literal subscripts_, let's get to them using _literal subscripts_.
First, let's get to `pi`'s value:
```
bash $ <<<$JSN jtc -w[3]
{
   "pi": 3.14
}
bash $ <<<$JSN jtc -w[3][pi]
3.14
bash $ 
```

Now let's get to the `number three`'s value:
```
bash $ <<<$JSN jtc -w[4]
[
   1,
   "two",
   {
      "number three": 3
   }
]
bash $ <<<$JSN jtc -w[4][2]
{
   "number three": 3
}
bash $ <<<$JSN jtc -w[4][2][number three]
jtc json exception: unexpected_end_of_string
bash $ 
```
\- why? 
\- it happens because of a _shell interpolation_. Shell treats space ('` `') as an argument separator, therefore option `-w`
ends up only with partial argument, namely with `[4][2][number`, which is an invalid walk.

##
in fact, `jtc` there complains due to a different reason: a second part of a walk (`three]`) is passed to `jtc` as a standalone argument,
which `jtc` treats as a _filename_. It tries opening and reading it, but because such file does not exist an empty result is returned. 
However, the empty input is an _invalid JSON_ (by JSON standard) - that why it's a JSON parsing error is given.  
Here how walk-path parsing error looks like:
```
bash $ <<<$JSN jtc -w[4][2][number three] -
jtc json exception: walk_offset_missing_closure
bash $ 
```
##

To escape shell interpolation, either the whole argument must be quoted, or a space symbol (the former varian is preferred, but
both will work):
```
bash $ <<<$JSN jtc -w'[4][2][number three]'
3
bash $ <<<$JSN jtc -w[4][2][number\ three]
3
bash $ 
```

The elements within objects also could be addressed using _numerical offsets_:
```
bash $ <<<$JSN jtc -w[4][2][0]
3
bash $ 
```
\- it seems _numerical notation_ is more succinct, why then bother using literal offsets?  
\- it's because _**our assumptions of the order of elements within _JSON objects_ are fragile**_.

Say, there's a following JSON:
```
bash $ ANML='{ "ANDEAN BEAR": "Bono", "AMUR TIGER": "Shadow", "GRIZZLY BEAR": "Goofy" }'
```
And we want to get get the name of `ANDEAN BEAR`. Being lazy one can do it by a _numerical offset_, assuming here that the index
of the required entry would be `0` (indeed, it's listed first there in the object), let's see:
```
bash $ <<<$ANML jtc -w[0]
"Shadow"
bash $ 
```
Bummer! Instead of selecting the name of `ANDEAN BEAR` we got the name of `AMUR TIGER`. That's because our assumption of the index
**was wrong**.

_JSON standard defines JSON objects as ***unordered set*** of name/value pairs_.
That means that the order of elements (name/value pairs) within _JSON objects_ will be defined by a processing programm
(and not by user, like in _JSON arrays_). Some programms will retain the same order, others will reorder them - it all boils
down to the internal implementation specifics.

`jtc` always rearranges all the elements within the _JSON objects_ by their keys (labels) in  the _alphabetical_ order, 
thus for `jtc` the above JSON looks like this:
```
bash $ <<<$ANML jtc 
{
   "AMUR TIGER": "Shadow",
   "ANDEAN BEAR": "Bono",
   "GRIZZLY BEAR": "Goofy"
}
bash $ 
```

That is a serious enough reason to select elements in JSON objects by their keys/labels:
```
bash $ <<<$ANML jtc -w'[ANDEAN BEAR]'
"Bono"
bash $ 
```
##
There's a curious case, when the label matches a numerical subscript, i.e. consider:
```
bash $ <<<'{ "0": 12345, "#": "abcde"}' jtc 
{
   "#": "abcde",
   "0": 12345
}
bash $ 
```

Addressing _JSON root_ with `[0]` will return `"abcde"`:
```
bash $ <<<'{ "0": 12345, "#": "abcde"}' jtc -w[0]
"abcde"
bash $ 
````
- How to get to the value of the label `"0"`? For that we need to use a _non-recursive search_ lexeme (namely, `>0<l`).

##
### Range subscripts
`[n:N]` - selects each element in the _iterable_, starting from `n`th index and ending with `N`th - 1, i.e. `N` 
is the index of the element following the last in the range. Both values `n` and `N` are optional and both could be omited

For those who are familiar with _Python addressing_, grasping this one is easy - it's matches Python's addressing concept entirely.

Range subscript makes the walk-path iterable, i.e. it's like selecting multiple elements with just one _iterable walk_ 
instead of specifying multiple offsets, compare:
```
bash $ <<<$JSN jtc -w[0] -w[1] -w[2]
"abc"
false
null
bash $ 
bash $ <<<$JSN jtc -w[0:3]
"abc"
false
null
bash $ 
```
##
#### Default range indices 
Either of indices in the _range subscirpt_ `n` or `N` could be missed, then the index in the ommited position takes a _defalut_ value. 

i.e. a _default_ index in the first position means: from the very first value in the _iterable_,  
while a _default_ index in the second position means: till the last value in the _iterable_

it's quite handy when we need to select only portion of the elements in the iterable either starting form its beginning, or till it's
last element, because sometimes we might not know upfront a number of elements in the iterable.
- select 2 elements from the beginning of the _JSON root's_  iterable:
```
bash $ <<<$JSN jtc -w[:2]
"abc"
false
bash $ 
```
- select _all_ elements staring from 3rd one:
```
bash $ <<<$JSN jtc -w[2:]
null
{
   "pi": 3.14
}
[
   1,
   "two",
   {
      "three": 3
   }
]
bash $ 
```


when both indices are missed `[:]` then each element in the iterable will be selected (_walked_):
```
bash $ <<<$JSN jtc -w[:]
"abc"
false
null
{
   "pi": 3.14
}
[
   1,
   "two",
   {
      "three": 3
   }
]
bash $ 
```
##
The _range indices_ (as well as any lexemes) can appear in the walk-path _any number of times_. The above example shows iterating over
the _top iterable_ (or, the first tier) in JSON tree hierarchy, to iterate over _all_ iterables in the second tier of the JSON tree,
do this:
```
bash $ <<<$JSN jtc -w[:][:]
3.14
1
"two"
{
   "three": 3
}
bash $ 
```
\- an each element in the _top iterable_ will be _walked_ and then attmpted to walk the _children_ of the walked element itself, 
one by one.
Because first three elements are not iterable, they will not be shows (they cannot be iterated over):
```
bash $ <<<$JSN jtc -w[0][:]
bash $ 
```

If you like to see (print) both walks of the top iterable and then each of the iterable at the second tier, then provide two walk paths:
```
bash $ <<<$JSN jtc -w[:] -w[:][:]
"abc"
false
null
{
   "pi": 3.14
}
3.14
[
   1,
   "two",
   {
      "three": 3
   }
]
1
"two"
{
   "three": 3
}
bash $ 
```
\- Note how `jtc` _iterleaves_ the walks - it puts relevant walkings in a good (relevant) order, rather than dumping results of
the first walk and then of the second. If one prefers seeing the latter behavior, option `-n` will do the trick, compare:
```
bash $ <<<$JSN jtc -w[:] -w[:][:] -n
"abc"
false
null
{
   "pi": 3.14
}
[
   1,
   "two",
   {
      "three": 3
   }
]
3.14
1
"two"
{
   "three": 3
}
bash $ 
```
##
##### Alternative range notation
`[+n]` is the alternative range notation for `[n:]`, they both do exactly the same thing - walk each element in the iterable starting
from `n`th element:
```
bash $ <<<$JSN jtc -w[+3]
{
   "pi": 3.14
}
[
   1,
   "two",
   {
      "three": 3
   }
]
bash $ <<<$JSN jtc -w[3:]
{
   "pi": 3.14
}
[
   1,
   "two",
   {
      "three": 3
   }
]
bash $ 
```
Using either of notations is a matter of personal preferrence and has no impact onto the way of walking JSON tree


##
#### Ranges with positive indices
Positive indices (and `0`) in range notation (`[n:N]`) always refer to the index offset _from the beginning_ of the range (iterable). 

When both `n` and `N` are positive, naturally `N` must be > `n`, if `N` <= `n`, it'll result in a blank output:
```
bash $ <<<$JSN jtc -w[2:1]
bash $ 
bash $ <<<$JSN jtc -w[2:2]
bash $ 
```

Case where `N` = `n` + 1, e.g., `[3:4]` is equal to spelling just a _numerical offset_ alone:
```
bash $ <<<$JSN jtc -w[3:4]
{
   "pi": 3.14
}
bash $ <<<$JSN jtc -w[3]
{
   "pi": 3.14
}
bash $ 
```

##
#### Ranges with negative indices
A negative index in the _range subscript_ refers to the offset _from the end_ of the iterable. In the range subscripts it's okay to
mix and match positive and negative indices in any position.

- select _last 3 elements_ from the top array:
```
bash $ <<<$JSN jtc -w[-3:]
null
{
   "pi": 3.14
}
[
   1,
   "two",
   {
      "three": 3
   }
]
bash $ 
```

- select _all_ elements in the range _from the 2nd_ till the one _before the last one_:
```
bash $ <<<$JSN jtc -w[1:-1]
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
```
bash $ <<<$JSN jtc -w[-100:100]
"abc"
false
null
{
   "pi": 3.14
}
[
   1,
   "two",
   {
      "three": 3
   }
]
bash $ 
```
However, when the range is unknown, it's best to use the notation with the
[_default range_](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#default-ranges)
values (i.e, `[:]`) 








    










