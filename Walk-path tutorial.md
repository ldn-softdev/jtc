
# [`jtc`](https://github.com/ldn-softdev/jtc). Walk-path easy. Tutorial (under construction)

`Walk-path` is a way to telling `jtc` how input JSON must be walked. 

1. [Walk-path Lexemes](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#walk-path-Lexemes)
2. [Subscripts](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#subscripts)
   * [Numerical offset `[n]`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#numerical-offset)
   * [Literal subscript `[text]`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#literal-subscript)

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

---
in fact, `jtc` there complains due to a different reason: a second part of a walk (`three]`) is passed to `jtc` as a standalone argument,
which `jtc` treats as a _filename_. It tries opening and reading it, but because such file does not exist an empty result is returned. 
However, the empty input is _invalid JSON_ (by JSON standard) - that why it's a JSON parsing error is given.
here how walk-path parsing error looks like:
```
bash $ <<<$JSN jtc -w[4][2][number three] -
jtc json exception: walk_offset_missing_closure
bash $ 
```
---

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
That means that the order of elements (name/value pairs) within _JSON objects_ will be defined by a processing programm.
Some programms will retain the same order, others will reorder them - it all boils down to the internal implementation specifics.

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

That is serious enough reason to select elements in JSON objects by their keys/labels:
```
bash $ <<<$ANML jtc -w'[ANDEAN BEAR]'
"Bono"
bash $ 
```

























