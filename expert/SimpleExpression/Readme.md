> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1




This tutorial is about the actual expression grammar.
In Tyr, almost anything is an expression.
To keep the language parseable simple expressions and full expressions exist.
Long story short, a simple expression is an expression where, in any context, the end of the expression can be determined by the parser.
Any expression can be turned into a simple expression by embracing it with ```()```.


## Simple expressions

Simple expressions start with a base and contain an arbitrary number of suffices.
The base can be one of:
 - static access, e.g. ```x```
 - literal, e.g. ```7``` or ```null```
 - new (allocation), e.g. ```new T```
 - delete (deallocation), e.g. ```delete x```
 - switch, e.g. ```switch x { .. }```
 - atomic, e.g. ```( .. )```
 - block, e.g. ```{ .. }```
 - try (not used in 0.6)

A suffix can be:
 - member access, e.g. ```.f```
 - runtime time application, e.g. ```(x)```
 - compile time application, e.g. ```[x]```
 - braced generalized binder, e.g. ```i do (i)```
 - super constructor call, e.g. ```.new(x)```
 - super destructor call, e.g. ```.delete```
 - static type access, e.g. ```.type```
 - dynamic type access, e.g. ```.class```

A simple expression may end with a single type expectation like ```x : int```.
A type expectation is a ```:``` followed by an expression yielding the expected type.


## Locations where simple expressions can be used

A simple expression must be used in these cases to ensure parsability of the language:
 - non-operators in operator applications, e.g. ```- 0 == 3 + 2 * 2 - 7u.sign()```
 - the condition after ```if```, e.g. ```if true {..}```
 - the condition after ```while```, e.g. ```while true {..}```
 - any expression occuring in a switch expression, e.g. ```7``` in ```switch 7 {if 7: 7 else 7}```
 - the target of a regular binder expression, e.g. ```arr.foreach``` in ```arr.foreach x do {..}```
 - the target of an allocation , e.g. ```T``` in ```new T```
 - the target of a deallocation , e.g. ```x``` in ```delete x```

A simple expression must be used in these cases to ensure readable code:
 - import after the ```with```, e.g. ```with tyr.lang```
 - expressions in visibility, simply because static and member access is the only useful combination here, e.g. ```private[tyr.lang]```
 - all forms of types provided after an ```:```, e.g. types in ```def f(x : int) : int = x : int```
 - the target in placement realizations, e.g. ```g``` in ```def f(x : int) : int := g```
 - type extensions, i.e. anything provided to a type-like definition after the ```g```, e.g. ```Object``` in ```class T <: Object {}```
