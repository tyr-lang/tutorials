This document aims to provide experienced programmers with a fast path to Tyr mastery.

Reading [hello world](https://github.com/tyr-lang/tutorials/tree/master/beginner/HelloWorld) and [draupnir](https://github.com/tyr-lang/tutorials/tree/master/beginner/Draupnir) tutorials is still a good start as it teaches you how to layout projects and how to compile, run and test them.
Having a look at the examples in roadmap and the implementation of [tyr.lang](https://github.com/tyr-lang/stdlib/tree/master/lang/src) will likely give a good idea how to use the language.
The Tyr language is defined in Tyr source code, mostly in tyr.lang.

Tyr gives the programmer full control over the generated code and ressource usage while providing modern efficient abstractions.
It is closely related to languages like C, C++ and Rust.
It incorporates neither a JIT nor a GC.
This means that the resulting program is very efficient and will immediately start executing.
But, the programmer has to manage resources explicitly.
Lastly, Tyr is about effortless writing of fast and readable code.
Tyr has full static <= polymorphism on all Tyr types.

Here is a small cheat sheet that should explain most of the concepts.
 - Comments are ```// .. \n``` and ```/* .. */```
 - Sources have the form imports, type-like definitions.
 - ```type``` is the equivalent of a struct, flat array, integer or other flat memory without runtime type information
 - ```class``` can inherit one class and multiple interfaces; operations on classes have constant overhead
 - ```interface``` can inherit one class and multiple interfaces; operations on interfaces have linear overhead that is usually near-constant
 - Tyr uses ```{}``` braces for bodies and blocks
 - Tyr uses ```()``` braces for runtime parameters including array indexing
 - Tyr uses ```[]``` braces for compile time parameters aka template or generic parameters
 - As of version 0.6, all CT parameters are λ-polymporphic. I.e. type parameters behave like C++ templates
 - Tyr has local forward-flow single-level return type type inference. I.e. all parameters and the expected type is considered in overload resolution and the most precise result type is forwarded to users of the result. If an overload resolution fails, an error is reported; in contrast to global or n-level type resolution which would back track. All parameters require explicit type; in contrast to global type resolution where parameter types can result from function usage. Informally, this is what a human does if he can look only at single expressions when typing expressions.
 - Tyr has no predeclarations. The compiler will do it for you. He will try very hard. If you get an elaboration order error and are convinced that you could compile a similar piece of code in C++ with predeclarations, please create a bug ticket in the language conformance repository.
 - Tyr templates can be mutually recurisve as long as their individual representation results in finite data structures.
 
 - Entity types come after the the entity and are separated by ```:```, e.g. ```var x : int```
 - Variables and fields are created with ```var``` followed by a name an optional type and an optional initializer. Constant variables are created with the ```val``` keyword in the same way but require an initializer.
 - Functions are created with ```def``` followed by a name, a parameter list, an optional return type, an optional property list and a realization.
 - Functions can be realized by ```;``` (abstract function), ```=``` (single expression body), ```{..}``` (full body), ```:=``` (placement, i.e. use another function as realization) and ```extern``` (use a C symbol from a C library).
 - "Global" or "static" variables and functions are created by adding a ```type``` in front of the ```var```/```val```/```def```. If it has no prefix, the member is an instance member.
 - All Objects, i.e. instances of classes, are heap-allocated and accessed by pointer.
 - Constructors use ```new``` instead of defining a function of the type's name. Likewise destructors use ```delete``` instead of finalize (Java) or tilde type's name (C++). Both have no types, no properties and no ```def``` keyword.
 - ```type```/```class```/```interface``` instances have type ```tyr.lang.Type```/```tyr.lang.Class```/```tyr.lang.Interfaces``` respectively
 - Tyr has an assignment operator ```=``` and a placement operator ```:=```. Placements, in contrast to assignments do not copy a value to another location. They litteraly place it elswhere. Most uses of the placement operator have effects that cannot be achieved in other languages.
 - Omitted visibility is library-private. ```public``` grants access to other libraries. ```override``` is a visibility. Entities can be, both, ```private``` and ```protected``` and both may take parameters that can differ. Any type-like can access its members even if they are private to an inaccessible namespace.
 - Type likes can have test members. Tests are not compiled into released binaries. Tests count as succesful if they return true, e.g. ```test "add" { 1 + 2 == 3 }```
 
 - Operators and operator precedence is language-defined (see tyr.lang)
 - Operators can be defined by replacing the function name with an operator symbol
 
 - Integer literals are typed, e.g. ```0i24``` (zero as 24bit signed), ```0u7``` (0 as 7 bit unsigned), ```0``` (0 as 32 bit signed), ```0L``` (0 as 64 bit signed), ```0U``` (0 as 64 bit unsigned). There are no implicit conversions on Integer or IntegerLiteral.
 
 - Array is a Java-style array. FlatArray and FlatSizedArray are C-style arrays.
