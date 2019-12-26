/*
 * Created by Dmitry Lyssenko, 2018, ldn.softdev@gmail.com
 *
 * this is a walk-path mini guide for Json class (Json.hpp)
 */

#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <regex>
#include <utility>





class GuideWp {
 // a singleton facilitating printing of a walk-path guide for json::iterator::walk()
 public:
    static GuideWp &    instance(void)
                         { static GuideWp guide; return guide; }
    void                print(void);

 private:
                        GuideWp(void) = default;
                        GuideWp(const GuideWp &) = delete;
     const GuideWp &    operator=(const GuideWp &) = delete;

    static std::string  wp_guide_;
};



void GuideWp::print(void) {
 // print walk path guide (substitute all required tokens)
 Json dummy;
 auto wp = dummy.walk("<>");
 auto & ws = wp.walk_path_().front();

 typedef std::pair<const char *, std::function<bool(void)>> replace_token;
 auto replace = [&ws] (const replace_token &rt) {
                 // build list of suffixes for a given replace token
                 std::string sfx;
                 for(int js = 0; js < Json::end_of_lexemes; ++js) {
                  ws.jsearch = static_cast<Json::Jsearch>(js);
                  if(rt.second()) sfx += ENUMS(Json::Jsearch, js)[0];
                 }
                 // replace token with the built suffix list
                 GuideWp::wp_guide_ = std::regex_replace(GuideWp::wp_guide_,
                                                         std::regex{rt.first}, sfx);
                };

 static const replace_token rtv[] = {                // replace-token vectors
               {"%ALL_SFX", [](void){ return true; } },
               {"%LXM_SRCH", [&ws](void){ return ws.is_not_directive(); } },
               {"%LXM_DRCT", [&ws](void){ return ws.is_directive(); } },
               {"%LXM_CTNT", [&ws](void){ return ws.is_lexeme_required(); } },
               {"%LXM_EMPT", [&ws](void){ return not ws.is_lexeme_required(); } },
               {"%LXM_NSPC", [&ws](void){ return ws.is_capturing_json(); } },
               {"%LXM_LABL", [&ws](void){ return ws.is_lbl_based(); } }
};

 for(const auto &rt: rtv)
  replace(rt);

 std::cout << GuideWp::wp_guide_ << std::endl;
}



std::string GuideWp::wp_guide_{R"(

  * Walk-path syntax:

Walk-path, made of various lexemes that instruct how to traverse JSON, allows selecting a single
or multiple elements from the input JSON

There are only 2 types of lexemes (though there're a few variants of each type):
 a) Subscript lexemes - provide addressing/subscripting JSON nodes
 b) Search lexemes - perform search down the JSON tree

a. Subscript lexemes: enclosed into square braces '[', ']', their meaning depends on the content;
   following notation variants are possible:
   '[]', '[text]', '[n]', '[-n]', '[^n]', '[+n]', '[N:M]', '[N:M:S]'
   - []: an empty offset, matches an empty label, like one in the following JSON:
        { "": "empty label example" }
   - [text]: the offset selects a child in the node by the label, e.g.: '[number]' selects the
     element 3.14 in the following JSON:
        { "number": 3.14, "false": true }
   - [n]: plain numerical offset selects indexed element within an iterable (an iterable is either
     a JSON array or an object), e.g.: '[1]' - selects a 2nd child of an array (or an object) node
     (all indices and quantifiers in a walk-path are zero based); in the above JSON, the element
     the label "false" will be selected (i.e. the value 'true'')
   - [-n]: a negative numerical offset, backs off n levels up the JSON tree (from a given node);
     e.g.: a path like '[0][0][-2]' digs 2 levels down the JSON tree and then selects back the
     root node (i.e. ascends 2 levels/tiers up)
   - [^n] numerical offsets with preceding circumflex do a similar thing as negative offsets, but
     instead descends from the root; e.g.: following walk-paths results are equal: '[0][1]',
     '[0][1][2][-1]', '[0][1][2][^2]'; number n in this notation must be signless (cannot go
     negative in this type of offset) preceding '+' sign is also not allowed;
   - [+n]: a positive numerical offset makes the path iterable, e.g.: [+1] selects all immediate
     children within a given JSON node (JSON array or object) staring from the 2nd child
   - [N:M]: a range offset (same notation like in Python) - selects all elements in the specified
     range. e.g.: [-3:] selects last 3 elements among JSON iterable's children; like in Python,
     the '+','-' signs as well as the numerical indices of a range are optional, thus all of the
     following notations are valid: '[:]', '[1:]', '[-2:]', '[+2:]', '[:3]', '[:-3]', etc
   - [N:M:S]: a range offset with the step increment; S may not be zero or negative; by default
     S is set to 1; all following notations are valid: '[::]', '[::2]', '[1::2]', '[:-1:3]', etc

   Some notations may duplicate each other, e.g.:
     '[+0]' and '[:]' and '[::]', or '[+2]' and '[2:]' and '[2::1]' have the same effect

b. search lexemes: enclosed into angular braces '<', '>', instruct to perform a recursive (or a
   non-recursive) search under a given JSON tree point; following notation forms are possible:
   '<txt>', '<txt>S', '<txt>N', '<txt>SN'
   and the respective non-recursive search lexeme forms:
   '>txt<', '>txt<S', '>txt<N', '>txt<SN'
   where txt - is any text to search for, S - is an optional one letter suffix, N - is an optional
   quantifier, which comes in several variants
   - if a lexeme is given using '<..>' encasement, then a *recursive* search applied off the
     current JSON node, otherwise (i.e. '>..<' encasement given) - a *non-recursive* search is
     performed among immediate JSON node's children only
   - '<a text>': performs a search of "a text" under a JSON tree off the given node among JSON
     strings values only - it's a default behavior, which could be altered with an optional suffix
   S: an optional one letter suffix, either of these: [%ALL_SFX], each one
      altering the search in the following way:
     r: apply exact match (default, may be omitted) while searching JSON string values only
     R: same as r, but the lexeme expression is a Regex (regex match applied)
     P: match any string, equals '<.*>R' but faster and let saving the value into a namespace
     d: match a specific number (i.e. searches numeric JSON values only)
     D: same as d, but the lexeme expression is a Regex (the value is treated as a string here)
     N: numerical match, same as '<.*>D' but faster and let saving the value in a namespace
     b: match a boolean (i.e. searching only boolean values); if lexeme value spelled fully, i.e.:
       'true', or 'false' then matched a respective boolean, while any other (including empty)
       lexeme spelling will memorize (any) found  boolean value in the respective namespace,
       e.g.: '<true>b', '<false>b', '<>b', '<val>b'
     n: match null values only, the content within the encasement may be empty, e.g.: '><n'; if a
        lexeme is non-empty (e.g.: '<val>n), then the found null value is preserved into a
        respective namespace
     l: apply exact match while searching objects' labels only
     L: same as l, but expression in braces is a Regex (a regex match applied)
     a: match any atomic JSON value (string, numeric, boolean, null); the content within the
        encasements may be empty (e.g.: '<>a')
     o: match any object JSON value, the lexeme's content may be empty
     i: match any array (indexable) JSON value; the content within the encasement may be empty
     c: match arrays and objects (containers); the content within the encasement may be empty
     e: end-node match (matches leaves only) - matches either of: atomics, {}, []
     w: wide range match - matches any JSON value (atomics, objects, arrays)
     j: match a user specific JSON value; the content within the encasement should be a valid
        literal JSON value, e.g.: '<{ "pi": 3.14 }>j' - will find a given JSON object; it's also
        possible to specify a templated JSON, e.g.: '<{ "{lbl}": {{val}} }>j' to match JSON
        dynamically based on the current namespace values, the result of template interpolation
        still must be a valid JSON
     s: match a JSON value previously stored in a namespace, the lexeme content points to the
        namespace, e.g.: '<val>s', will find a JSON value previously stored in the 'val' namespace
     t: match a label/index value previously saved in a namespace, the lexeme must points to the
        namespace, which should be a JSON string or a numeric type only
     q: match only original JSON elements, every match is stored in the namespace, the lexeme
        may be empty
     Q: match all non-unique (duplicate) elements, every match is stored in the namespace, the
        lexeme may be empty
     g: matches JSON elements in the ascending order, the lexeme may stay empty
     G: matches JSON elements in the descending order, the lexeme may stay empty

     - Following suffixes define lexemes as directives, which do not perform any search, instead,
       directive allow performing certain actions with currently walked JSON elements:
     v: saves the currently found (or user-defined) JSON value into a namespace
     k: preserves a currently found/walked JSON element's label (index) into a given namespace;
        if the lexeme is empty, then the label (index) can be reinterpreted as a JSON value
     z: erase a namespace pointed by a lexeme's value; the lexeme cannot be empty
     f: fail-safe (or fork): if walking lexemes past the fail-safe fails, instead of progressing
        to the next iteration, a resolved JSON node (and its path) at the fail-safe point will be
        restored; when used together with 'F' directive, the walking may either continues past
        '<>F' directive, or stop at '><F' directive
     F: when spelled as '<>F' then upon walking in instructs to skip to the next iteration; if
        spelled as '><F' then instructs to stop walking (despite further path present); a non-zero
        quantifier extends behavior for both of the lexeme notations: '<>Fn' will let to continue
        walking the path at the next n'th lexeme; '><Fn' will re-iterate the walk path (up to the
        lexeme) additionally n times
     u: application-specific directive (might be even undefined), further walking is gated by the
        call-back's return status: `true` let continue walking, `false` fails walking
     I: increment (and optionally multiply) a namespace: in the notation '<val>In:m', increments
        the value 'val' by n and optionally multiplies by m (if given); the value 'val' must be
         a numeric JSON.
     Z: saves into the provided namespace a size of the currently walked JSON; recursive notation
        calculates size recursively, while non-recursive notation provides only a number of
        immediate children; with the quantifier 1 ('Z1') saves into the namespace a size of the
        currently walked JSON string (or -1 if not a string)
     W: saves into the provided namespace a current walk-path as a JSON array

   N: an integer quantifier specifying search match instance/range, comes in several variants
     n - a number (index), e.g. '<a text>3' - matches 4th encounter of a string "a text" within
       a JSON tree (off a given search point); all quantifiers, as well as numerical subscripts
       are zero based
     +n - collects all matched encounters staring from index n, e.g.: '<a text>+2' will match 3rd
       encounter of "a text", 4th, 5th and so on until all matches found
     n:m - once ':' is sighted within a quantifier, then it specifies a range; the range
       quantifiers follow the same notation/rules as subscript's range
     n:m:s - same notation rules like for [N:M:S] subscript range; if after interpolation from the
       namespace 's' happens to be negative (or zero) value, then the default value of 1 applied
   - the indices in quantifiers typically cannot go negative, with exceptions for search types
     '>..<t' and '>..<l', where they signify a relative offset, so that it's possible to select
     siblings of the found matches (see full user guide for explanation and use cases)

   All the lexemes could be bucketed by different properties:
   o  search lexemes [%LXM_SRCH]
   o  directive lexemes [%LXM_DRCT];
   o  lexemes that must not be empty [%LXM_CTNT]
   o  lexemes that may stay empty [%LXM_EMPT]
   o  lexemes that catch walked JSON values into namespace [%LXM_NSPC]
   o  search lexemes that look among labels (or indices) only [%LXM_LABL]

All lexeme types allow specifying inner brackets, however, the closing one has to be quoted with
the preceding backslash, e.g.: '[case[0\]]' - specifies an offset lexeme to the label "case[0]";
'<tag<a\>>' - denotes a search lexeme for the text "tag<a>"

Spaces *between* lexemes are allowed and ignored; for the subscript lexemes even a more generic
rule is in play: if a subscript lexeme violates any of the described above rules and does not
represent a valid offset, then it's treated as a literal type of offset; e.g.: '[ 1]' and '[1 ]'
are in fact respective literal offsets (labels) " 1" and "1 " rather than a numerical offset 1;
'[^-3]', '[+-2]', are also examples of literal subscripts

There're cases when JSON values have to be searched where they attached only to a specific label
(a.k.a. scoped search), a following lexeme syntax facilitates such case: '[a label]:<some text>'
- in that example "some text" will be searched (and matched) only among JSON values which are
attached to the label "a label"; in that notation all suffixes (and quantifiers) are allowed
except 'l', 'L' and 't', e.g.: '[age]:<25>j:' matches all JSON numerical values 25, where it's
attached to the label "age" only
)"};



























