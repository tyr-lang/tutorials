> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about using Tyr functions, function calls and function objects.


## Defaults and named parameters

The authors of Tyr have significantly invested into the expressiveness and flexibility of the language.
One part of it is allowing functions to define default values for parameters.
For example, if a cursor should step to the next element, the implementation may take an arbitrary stride but by default, we would like to step to the next element.
We can do this by defining that:
```
type cursor {
  ..
  def step(stride : int = 1) {..}
}
```
In this example, calling ```step()``` will tell the compiler to insert a copy of ```1``` into the missing parameter slot.

For functions taking several parameters, it can be more readable if the call explicitly mentions the parameter names.
This can be done by adding the parameter name and a placement operator before the value in a call.
Example:
```
val c : cursor = ..
c.step(stride := 2);
```
Here, the compiler will look at the definition of step and insert 2 at the correct position for you.


## ```apply``` and ```update```

We have seen already that datatypes in Tyr are used like functions.
In Tyr, a function object can be created by implementing a function ```apply```.
More specifically, if a function call is performed on a value that itself is not a function, the compiler will search for a function ```apply``` and call it for you.
So, an expression ```f(..)``` is implicitly expanded to ```f.apply(..)```.
Note that ```f``` can also be a type and apply can be a type def.

If the compiler encounters an expanded apply followed by an asignment, it will try to replace it with an ```update```.
Thus, an expression ```f.apply(..) = v``` is implicitly replaced by ```f.update(.., v)```.
This allows us to implement mutable function-like data structures like arrays.
To some extent, arrays are just mutable partial functions from integers to their base types.
Note that the ```update``` replacement is only done if ```apply``` is defined in the same type.
Furthermore, ```update``` must have the same parameters as the corresponding ```apply``` plus an extra parameter for the right hand side of the assignment.


Exercise: Create a type Pair taking a parameter T that defaults to int.
It should have two fields left and right that can be initialized from a constructor.
Also, the type should implement apply and update making it usable like an Array with two elements.


## ```inline``` and ```native```

Finally, there are two interesting function properties.
One is ```inline```.
In contrast to what one may be used to from other languages, ```inline``` in Tyr is "must inline".
I.e. the compiler will always inline the function in order to realize a call.
As such, inline functions must not be recursive.
In fact, any function that will be implemented by an inline call will implicitly get an ```inline``` property.
This includes functions taking ```tyr.lang.Block``` or ```tyr.lang.BlockParameter``` parameters.

The second property is ```native```.
When looking at code in the standard library, one will eventually notice that some functions have this property and an abstract realization.
This tells the Tyr compiler that the function is somehow special and that it should know it and handle it differently if a call is encountered.
For instance, Integer.+ cannot be implemented without the compiler's help.
Note that it is not allowed to take a function pointer from a native function because it has no adress.
In fact, ```native``` can be used on any entity.
For instance, Integer cannot be expressed if the compiler does not know what an Integer is.


## Next steps

???
