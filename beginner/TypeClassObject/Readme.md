> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about member lifetimes.
In other tutorials, we have used type and object lifetimes to associate members with types and instances respectively.
Here, we will explain all three lifetimes and explain how access is done.
Also, we will explain explicit ```this``` used commonly to restrict mutating functions to mutable targets.


## Object lifetime

Object lifetime is the most common form in object-oriented programming.
It is called object, because fields are stored in the object and functions operate on an object.
In other languages, this is referred to as instance field or instance method.

If the member stems from a ```type```, mutating ```var``` fields requires the object to be stored in a ```var``` itself.
If it is stored in a ```class``` or ```interface```, mutating ```var``` fields is always allowed.

Functions are not stored anywhere.
They exist at a fixed location in the executable irrespective of the lifetime.
However, functions with object lifetime are conceptually applied onto an instance of the defining type.
As such, any such function takes part in static overriding.
If the function is defined in a ```class``` or ```interface```, it also takes part in dynamic overriding.

So, what does this mean?
Static overriding means that the override of the function closest to the instance's static type is picked.
An example is contained in this tutorial's Tyr project (static.tyr).
Dynamic overriding means that the ```Class``` instance representing on object's type at runtime contains a pointer to the most specific implementation.
This pointer is retrieved at runtime and used for the call.
We have already seen this when deleting objects.

Object functions always have a parameter ```this``` serving as the representation of the target object.
A member with object lifetime is created by adding neither ```type``` nor ```class``` in front of the ```var```, ```val``` or ```def```.


## ```type``` lifetime

Some values just exist.
For instance, the global output stream ```tyr.io.System.out``` is just there.
It is provided once by the operating system.
It does not start to exist and cannot cease to exist.
Nonetheless, it is associated with ```tyr.io.System``` and exists within the context of that type alongside other input and output streams provided by the OS.

Likewise, some functions are associated with types, but do not require an instance of it.
A common case is the factory method pattern, which serves as an alternative to a constructor.
Another common form are helper methods that would not require dynamic dispatching because they are not expected to get overriden.
For instance, ```tyr.lang.container.Seq.empty``` is a shorthand of comparing the argument sequence's size with 0.
Functions with type lifetime take part in static overriding.
They cannot take part in dynamic overriding as there is no dynamic target.

In some languages, this is referred to as static or global lifetime or member.
In Tyr, we use the term type lifetime, because the member is associated with the enclosing type-like definition.
Hence, a member with type lifetime is created by adding ```type``` in front of the ```var```, ```val``` or ```def```.


## ```class``` lifetime

We have learned that object functions come from the Class instance at runtime.
So, in Tyr, we added a way of storing other objects and functions there, too.
This is a mix between object and type lifetime.
A class field exists indefinitely.
But, it exists once per class.
Thus, if a subclass is created, that subclass get's its own copy that can be modified individually.
Access to these copies is subject to dynamic dispathing.

For fields, applications are values like human-readable display names that differ by subclass but are identical for each instance of the same subclass.
Storing such values in a class field is much cheaper than using an object function returning the value for that type.
Another application would be an instance counter for a type hierarchy that is aware of subtypes.

For functions, applications are validation, construction or copy methods.
Anything that one would implement using reflection in a language that offers an reflection API.
Note that reflection is a lot less safe and a lot less efficient compared to class functions.

Using a class member requires a Class pointer to perform the dynamic access.
If applied to an instance of a class, an implicit access to the respective Class pointer is performed.
Nonetheless, class functions do not get an implicit this.

Note: Capitalization in this section is intended.
Class is the instance of ```tyr.lang.Class```, i.e. the runtime representation of type-like defined using the ```class``` keyword.
Whereas, class the lifetime of a member defined using the ```class``` keyword.


## Explicit ```this```

In Tyr, functions can have an explicit ```this``` parameter.
Starting with Tyr 0.6, this parameter has to be the first paramater to ensure readability.
The pramater can have any type.
However, for functions with object or class lifetime, an implicit conversion to the Class pointer has to exist.
Moreover, an implicit conversion to the enclosing type should exist, because the user will think about target objects as instances of that type.

An example of this is ```Integer.++```.
This takes a ```Ref[Integer[T]]``` and increments it by one.
Ref is by far used the most as type of an explicit ```this```.
Another common use case is adding type functions to classes that behave like object function but do not dispatch dynamically.


## ```type``` function duality

Tyr has representational transparency as a core concept.
It does not follow philosphical approaches like Smalltalk or Simula.
Tyr is about efficient organization of memory and operations on memory.
A consequence is, besides placement realizations and explicit overrides, that, both, object and class functions can be used like type functions.
Likewise, type functions with an explicit this can be used like object or class functions except for the dynamic dispathing part.

Why is this?
As stated above, a function just is.
A Class instance holds a pointer to it.
But, there is no reason preventing us from calling the function directly.
If an implicit ```this``` is present, the dual type function will have an explicit ```this```.
Because, this is how the function is realized.

A consequence of this is that static and dynamic dispatching can be disabled on call site.
Example:
```
var arr = new Array[int](17)

// 17 is the size
17 == arr.size()
// using Array's count iterates over the array using an index
17 == arr.count x do (true)
// using Iterable's count iterates over the array using an Iterator
17 == Iterable[int].count(arr) x do (true)

```

## Next steps

[Templates](https://github.com/tyr-lang/tutorials/edit/master/beginner/Templates): Learn about how to use and write types that work on arbitrary other types.
