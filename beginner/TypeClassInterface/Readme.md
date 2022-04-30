> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about types, classes and interfaces.


## Types

Types represent flat data.
A type can have fields, i.e. components of other types.
And, it can have functions to implement operations on instances of a type.
An instance of a type is a piece of memory holding all its components.
An example of a predefined type is [int](https://github.com/tyr-lang/stdlib/blob/master/lang/src/integer.tyr).

Let's create a type ```point``` as an example.
We want a ```point``` to hold two integers.
Also, it should provide us with an operation plus adding two points and a comparison operator.
```
type point {
  // mutable field x of type int
  var x : int
  // mutable field y of type int
  var y : int

  // a function called plus operating on point taking a point "other" returning a point
  def plus(other : point) : point { /*..*/ }
  // an operator called == operating on point taking a point "other" returning a bool
  def == (other : point) : bool { /*..*/ }
}
```
Now, let's add some life to the plus function.
A new value of a type can be created using the keyword ```new```.
After creating a new object, we have to initialize x and y.
We do so by storing the sums of the current ```x``` and ```other```'s ```x``` in the resulting ```point```'s ```x``` field and do the same for ```y```.
After that, we are done and can return our result.
```
def plus(other : point) : point {
  var result = new point
  result.x = x + other.x
  result.y = y + other.y

  result
}
```
Likewise, we will implement the comparison operator by comparing ```x``` and ```y``` fields.
As this is just a single statement, we will use a little shorthand.
We let the Tyr compiler infer the result, because we know that comparisons are ```bool``` anyway.
Furthermore, we use ```=``` to start a function body that consists only of a single expression.
```
def == (other : point) = x == other.x &! y == other.y
```
Note that we use ```&!``` instead of ```&&``` to combine the two comparisons.
The version with the exclamation mark tells the compiler to do both comparisons irrespective of the firsts comparisons result.
As both comparisons are cheap, this results in faster overall code.

Now that we are done with our first type, we can add some tests to ensure that it does what we expect it to.
Tests belong to types and are, therefore, located in their bodies like members.
Unlike functions, tests are not present in your final executable.
Therefore, you can add as many tests as you like.
```
type point {
  /* ... */
  test "add eq" {
    var p = new point
    p.x = 3
    p.y = 7

    p = p.plus(p)

    var q = new point
    q.x = 6
    p.y = 14

    q == p
  }
}
```

Exercise: Try adding a function minus that subtracts two points from each other.
Have at least one test for minus.
A solution can be found at the end of the point implementation in the tutorial's sources.


In Tyr, types inherit properties from exactly one super type.
The only exception to this rule is ```any```, a type that has no members and no representation.
Any plays a crucial role in Tyr's type theory.
But, as it cannot be represented, it cannot be used to type fields, parameters or variables.
Any type that does not explicitly state a super type has any as implicit super type.
As of Tyr 0.6, a type inheriting another type must not introduce new fields except for types inheriting ```any```.
Inheritance is done by adding a ```<:``` after the name of the type followed by a ```,```-separated list of expressions evaluating to type, class, interface or property names.


## Classes

Classes are types with runtime type representation and a polymorphic representation.
This means that if one has a collection of fruits, the apples can be distinguished from oranges.
Using classes is about moving common properties and functionality into super classes.
In our example, apples and oranges have a weight and can be cut.
Therefore, an efficient way implementing these properties is by adding them to a class fruit and let apple and orange inherit them.
Additionally, this allows calculating the weight of a collection of fruits without having to check each and every fruit if it is an apple or an orange.

A consequence of this is that class instances are represented by pointers to memory.
I.e. an instances is allocated and the result is returned by new.
In contrast, calling new on a type will directly return the instance.
To some extend, this is actually the same behaviour, because classes are all about pointers to memory with common properties.
And, a new instance also creates a new pointer value.
As such, ```Object``` is a subtype of ```observablePointer```.

A class inherits one class and any number of interfaces.
If it does not inherit a class explicitly it implicitly inherits ```Object```.
Classes are highly efficient.
All operations on classes are constant time in Tyr.
Even type checks.
A type check, i.e. a statement that checks if an object is an instance of a class, can be performed using the operator ```?```.
Example:
```
val x = new Apple
Fruit ? x
```
A checked conversion can be performed using operator ```!```.
Example:
```
val x : Object = new Apple
if (Apple ? x) {
  // create a view onto the apple
  val a = Apple ! x
  // now, we can call cut on the view
  a.cut()
}
```


## Interfaces

Sometimes, a single super type is not enough.
For instance, when moddeling fruits, we can have a TastyFruit that expresses that humans would eat it.
All oranges are tasty, but some apples are not.
But, both, oranges and apples are fruits.
As being a fruit is our leading property set, we keep fruit a class and create an interface ```TastyFruit```.
Example:
```
class Fruit <: abstract {..}
interface TastyFruit <: Fruit {..}
class Orange <: TastyFruit {..}
class Apple <: Fruit {..}
// european apple that nobody would eat unless starving
class Holzapfel <: Apple {..}
// tasty sour apple
class Granny <: Apple, TastyFruit {..}
```

The distinction between classes and interfaces is mostly in costs.
This is caused by the fact that single inheritance allows the compiler to know the in-memory representation of an object without knowing its exact type.
Whereas, in unrestricted multiple inheritance, the in-memory representation cannot be known without using helper data structures.
Therefore, Tyr and almost all modern object oriented programming languages use this distinction.
The interface overhead is to be paid in two cases.
If a type check or cast is performed against an interface the runtime type information of the target is searched.
The worst case is that all interfaces implemented by the target have to be checked.
As this is usually a number between zero and five, you likely won't notice.
If a member, i.e. field or function, is accessed and the target's visible declaration stems from an interface, the runtime type information of the target is searched.
While this cannot fail, it can still be that out of bad luck all implemented members are compared.
As such, the cost of interface member access is linear in the number of the target's members inherited from interfaces.

Note that if a target has an interface type a type check that is performed against a class type is still constant.
Likewise, if a target has an interface type and a member is accessed that the interface inherits from a class, the access is constent.
This is true even if the interface overrides the inherited member.

The lesson here is that one should use the most precise type.
Use abstract classes over interfaces wherever you can.
If requirements change, in Tyr, it is always backward-compatible to change an abstract class into an interface.
Nonetheless, keep in mind that the most important thing in designing type hierarchies is that they should reflect the abstraction of the reality you are about to automate in code.


## Memory: Representation, Allocation and Release

Values of type instances are represented as a consecutive bytes of the type's field representations.
Values of class and interfaces instances are represented as pointers to the heap-allocated field-representing bytes.
This heap-allocated memory is managed by the programmer.
Nonetheless, types may also represent programmer-managed ressources like file handles.
Therefore, type-likes can be created with ```new``` and released with ```delete```.

Allocation is done by getting bytes to represent the fields.
Afterwards, a constructor is called.
Constructors are special functions defined with ```new```.
They are special in that they are the only functions that can modify value fields.
Classes and interfaces have always at least one constructor.
If they do not declare a constructor, an implicit constructor is created.
Types may represent things that cannot be created.
For instance ```void``` represent no bytes at all.
There cannot be a value in memory of type ```void```.
Likewise, numbers just are.
They are not created.
Any bit pattern forming a number has a useful meaning.
Thus, it does not make sense to create one explicitly.

Another special property of constructors are parameter placements.
A constructor parameter can be prefixed with a field name and a placement operator ```:=```.
This causes the parameter to be used as the field's initializer.

Finally, a constructor cannot be abstract.
Thus, a ```;``` instead of a body is used as an empty body.
In other function declarations, a semicolon results in the function being abstract.

Example:
```
class Fruit {
  val weight : int;
  new (weight := w : int);
}
```

After an instance is no longer used, it should be released.
For types, releasing an instance does not release the memory.
Deleting type instances is usually not required and often not even possible.

For classes and interfaces, releasing an instance releases the heap-allocated memory.
If the instance is stored in a variable, the variable is set to ```null```.
If ```null``` is released, nothing happens.
Before returning the memory to the operating system, the destructor is called.
Destructors are special functions defined with ```delete```.
Destructors cannot take parameters.

There are some classes whose instances cannot be released.
Examples are Class and StringLiteral.
Such classes are marked with the ```tyr.lang.mem.eternal```.
Deleting an instance with that property, as of Tyr 0.6, will result in an immediate crash.
Example:
```
class Array[T : Type] {
  private var data : FlatArray[T];
  public delete {
    // Release the backing flat array. If it is null, nothing happens.
    delete data;
  }
  /*..*/
}
```


Note: Variables are represented as pointers to stack-allocated values.
In most cases, they are optimized to registers.

Note: If the compiler can prove that memory usage is balanced, i.e. any allocated memory is also deleted, it may optimize out the heap-allocation or replace it by faster stack-allocation.

Note: Constructors are implicit type functions whereas destructors are implicit object functions.
They can be called explicitly using literal identifiers.
This intentionally allows objects to be reinitialized and is a safe alternative to C++'s placement new.


## Constructor and Destructor

Although constructors and destructors are functions and can even be used like other functions they have a very special nature.
Both use a stacked function call approach.
If you have a look at the tasty fruits example, calling ```eat``` on an ```Orange``` will result in ```TastyFruit```'s ```eat``` being executed.
However, if an ```Orange``` is created ```Orange```'s ```new``` is called which calls ```Fruit```'s ```new``` which calls ```Object```'s ```new```.
Likewise, deleting an ```Orange``` calls ```Orange```'s ```delete``` which calls ```Fruit```'s ```delete``` which calls ```Object```'s ```delete```.
This is because constructors and destructors are experts for their own members and deleget super members to super constructors and destructors.

Within the body of a constructor, an explicit super constructor call can be performed by using ```new``` like a function name on the super type.
For example Orange's constructor is ```new { Fruit.new(12) }```.

Within the body of a destructor, an explicit super destructor call can be performed by using ```delete``` like a function name on the super type.
A super destructor call does not allow ```()``` to be used as no parameters can be passed.
For example ```Fruit```'s destructor is ```delete { Object.delete }```.
This would also be the implicit implementation.


## Next steps

[Visibility](https://github.com/tyr-lang/tutorials/tree/master/beginner/Visibility): Learn which entities can be accessed and how you can control who can access your entities

adv operator overloading

adv function calls

adv type access
