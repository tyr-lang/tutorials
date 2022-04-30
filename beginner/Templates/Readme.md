> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about templates.
Templates allow a programmer to define a type-like with parameters and have the compiler insert them once they are known.
We have seen that ```Array``` takes a parameter.
This allows us to have one ```Array``` implementation for all types.
This saves a lot of our lifetime.
It, clearly, does not make sense to implement an ```Array_int``` for ```int``` and an ```Array_float``` for ```float``` and an ```Array_String``` for ```String``` and so on.

From a programming languages perspective, templates in Tyr are λ-polymorphic.
This means that templates are functions on the type system.
The arguments are the ones provided in square brackets.
The result is a Type for types, Class for classes, and so on.


## Use

Using templates is very easy.
Just use the type like a function, but provide the arguments in square brackets.
The application happens at compile time.
Therefore, using a template is as efficient as using a non-template type.
Actually, templates in Tyr are so efficient that even ```int``` is a template type (see [Integer](https://github.com/tyr-lang/stdlib/blob/master/lang/src/integer.tyr)).

Example:
```
Ref[int]
container.Array[int]
Ref[container.Array[container.Array[int]]]
container.Array[container.Array[container.Array[Ref[Integer[32]]]]]
```


## Define

Defining templates is also pretty easy.
In fact, it works the same way as defining other type-likes.
The only difference is that a parameter list in square brackets is added behind the name and before the property list.
These parameters can be used within the body like fields.
Example:
```
class Vector[D : container.size_t] {
  private var data : FlatSizedArray[float, D]
  
  public def apply(dimension : int) = data(dimension)
  public def update(dimension : int, value : float) = data(dimension) = value
  
  public def + (other : Vector[D]) {
    val r = new Vector[D]
    data.foreach i, x do {
      r.data(i) = x + other.data(i)
    }
    r
  }
}
```

Note that Tyr is a strictly typed language.
If a type is defined, the compiler will check if that type makes sense.
This applies to types taking parameters, too.
In consequence, one cannot just use members of an argument type and hope that the user will provide one that has these members.
Unfortunately, in Tyr 0.6, there is no workaround for this restriction.
I.e., in Tyr 0.6, argument types are completely transparent.
However, this is not much of an issue, as most templates work like this anyway.


## CT knowledge and type equivalence

If we want the compiler to create a type for us based on some parameters, we have to give the compiler the argument values.
This means that only constants and expressions that the compiler can turn into constants immediately are allowed.

This rule has another aspect which is type equivalence.
Whenever a type is used with identical parameters, we want the identical resulting type to be returned.
This reflects that types just are.
As such, functions on types behave like mathmatical functions rather than mutable functions.
This is only achievable, if they are restricted to the mathmatical subset of the language.


## Felden type theory: recursion and ODR

Tyr stands for TYpe Research.
One of the original research questions was if a template type theory could be created in which template classes could use each other without having the user to predeclare types or members.
For example, ```tyr.lang.container.Iterator[T].toBuffer``` converts the remaining elements of an iterator into an ```ArrayBuffer```.
If you have a look at the implementation of [```Iterator```](https://github.com/tyr-lang/stdlib/blob/master/lang/src/container/iterator.tyr) and [```ArrayBuffer```](https://github.com/tyr-lang/stdlib/blob/master/lang/src/container/arrayBuffer.tyr), you may notice two things.
One is that no predeclarations exist.
Obviously, because Tyr does not even have syntax for it.
The other more important observation is that an ```ArrayBuffer``` is an ```Iterable``` which uses the type ```Iterator``` which uses the type ```ArrayBuffer```.
This is a cycle.
And, it is a cylce in a template type, i.e. the compiler has to do all the argument insertion in a way that results in types that work with each other.
Note that, from a research perspective, this is even a lot more fun when the whole "compiled templates from other library" thing comes into place.
For a programmer, the simple demand is that he would not have to care.
And in fact, the underlying type theory fulfills this demand.
Even if types have complex cyclic dependencies.
Even if types have members with complex implementations and inline each others bodies.

In programming languages, there is a thing called one definition rule (ODR).
Basically, this means that a difinition can only happen once.
If multiple definitions exist, all of them have to be identical.
In todays most-used programming languages like C++ and Java, this rule has to be fulfilled by the programmer.
It is not enforced by the compiler or virtual machine.
This can lead to very very complex bugs.
They alone would be a reason to have the compiler do it for you in Tyr.
However, having cyclic templates is actually only possible in languages where the compiler enforces ODR.
Because, otherwise, it could be that cycles are broken by ODR violations.
A consequence is that, in contrast to Java, a compiled library cannot be patched afterwards.
It can only be compiled again.


### Excursion on C++ and Java

Because some people may not believe that Tyr offers functionality that neither C++ nor Java can emulate, let me go into details here.

A simple form of recursive templates is two different pair structs that can be converted into each other.
There is a [test](https://github.com/tyr-lang/test.conformance/blob/master/0.6.0/accept/qswap/mar.tyr) in the Tyr conformance test suite that will serve as an example, here.

In C++, templates are λ-polymorphic, i.e. they work like Tyr templates.
A single swap type that swaps itself can be easily defined in C++.
However, C++ has top-down elaboration.
Thus, cum granu salis, type definitions are evaluated from top to bottom.
Also, as of today, the bare members of a template cannot be predefined.
Therefore, there is no way getting our example to work in C++.

In Java, generics are ∀-polymoprhic, i.e. they do not work like Tyr templates as described here.
In Java, generics do not create new types.
It is more like a function that always returns the same result called raw type in Java.
Therefore, one can define the two types in Java.
However, there are severe restrictions and consequences.
In Tyr, any constant value is allowed as a template argument, e.g. integers.
In Java, only types and moreover only subtypes of java.lang.Object are allowed.
Therefore, the example does not type in Java, because int and float cannot be used as type arguments.


Also, let me explain the ODR part a bit more.

In C++, this is commonly experienced if, for whatever reason, two different definitions for the same template exist.
This can, for example, happen if a fixed version and an earlier version of a template is compiled into the same binary.
It can be very hard to undererstand, because as a developer, you will see the fixed version in front of you, but the code will just not do what you expect.
And there might not be any warning about it.

In Java, this happens if a class exists more than once in the class path.
It has basically the same effect as in C++.
In Java, this is often caused by so-called fat jars.
Admittedly, this is mostly mittigated by using maven and Java 9 module definitions.


## Excursion Ada-style and C++-style templates

There are basically two ways of implementing templates.
One is the original C++ way where a template is basically just a piece of code that will generate a type or a function when used.
Pretty much like a macro.
The other one is the Ada way where it is perceived as a function on code doing the same thing.
The main difference here is that in C++, most of the checking happens when the template is instantiated.
Whereas, in Ada, the checking happens when the template is defined.

This has two consequences.
Obviously, not having to declare and restrict template parameters increases flexibility and expressiveness.
Also, the language design and implementation is a lot easier this way, because there is no need to define means of declaring and checking template argument properties.
For example, if a Vector should be defined over arbitrary base types, one needs to know that certain operations like + or * exits on that base type.
Also, some constants like 0 and 1 might be required.
So, there needs to be syntax and semantics to provide that information.
And, if the language should feel good to a user, there must be means of inferring these values for both Vector[int] and Vector[float] usages.
Because the user of Vector will, for sure, not want to provide all the functions and constants manually.

However, the second consequence is that error reporting will respect the abstraction.
This means that if a Vector implementation usses some Operator & by mistake, with Ada-style templates, the error is reported to the author of Vector.
Whereas, with C++-style templates the error is reported to the user of Vector.
This is a violation of the abstraction.
Clearly, the Vector implementation should encapsulate all the details and should habe been checked to work for any parameter.
Obviously, this does not imply that any valid parameter will also make sense or result in behaviour that one would consider correct.
But, this is also the case for ordinary functions.

Interestingly, C++ later introduced a concept called [concepts](https://en.wikipedia.org/wiki/Concepts_(C++)).
To some extent, this brings templates closer to the function abstraction.
So, for Tyr, we decided to learn from that lesson and go for Ada-style templates from the very beginning.


## Next steps

[Extern](https://github.com/tyr-lang/tutorials/tree/master/advanced/Extern): Learn about C integration
