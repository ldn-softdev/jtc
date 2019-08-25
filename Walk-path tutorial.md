
# [`jtc`](https://github.com/ldn-softdev/jtc). Walk-path easy. Tutorial (under construction)

`Walk-path` is a way to telling `jtc` how input JSON must be walked. 

1. [Walk-path Lexemes](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#walk-path-Lexemes)
2. [Subscripts](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#subscripts)
   * [Numerical offset `[n]`](https://github.com/ldn-softdev/jtc/blob/master/Walk-path%20tutorial.md#Numerical-offset)

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
There are few variants of _subscripts_, let's start with the most common one - _numerical offset_

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

- select 1st element in _JSON array_:
```
bash $ <<<$JSN jtc -w[0]
"abc"
bash $ 
```

\- select 5th element in _JSON array_:
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

If selected element is non-atomic (a.k.a. _iterable_), i.e., _Json array_, or _JSON object_, then you may continule digging
further the JSON tree:

\- select 5th element in _JSON array_ and then 3rd one:
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





