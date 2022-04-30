> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about advanced parts of control flow in Tyr.
It will teach you how to use all aspects of expressions.


## Everything is an expression

Everything, even ```return```, is an expression.
Some expressions have obvious results.
Some results may not be that obvious.
For instance, the mentioned ```return``` expressions have the result type ```NoReturn```.
This might look very odd at first glance, but is exactly what happens.
Because, execution will not return from evaluation of a return.
Therefore, anything can be done with it.
It will never happen anyway.

Likewise, loops are expressions.
```while``` loops return ```void```, because they may not be executed once.
```do```-```while``` loops, however, return the result of their last iteration.
Also, branching expressions like ```if``` and ```switch``` return the result of the branch actually taken.
If an ```if``` expression lacks an ```else``` branch, ```void``` is returned.

A block returns it's last expression.
An empty block is of type ```void```.

Example:
```
var x = 3
x = switch x {
  if 1: 42
  if 3: 7
  else -1
}
```


## Generalized Binders and ```inline``` functions

A binder is defined like any other function in Tyr.
The bound variables are created using the ```BlockParameter``` type.
The bound blocks are created using the ```Block``` type.
A binder definition has to be ordered like a binder application.
The regular parameters come first.
Then the BlockParameters and the Blocks go last.



This is the coolest thing that Tyr introduced to programming languages.
Generalized binders allow types to define loop-like abstractions.

Example (print one, two and three each in a new line):
```
var buffer = new container.ArrayBuffer[StringLiteral]
buffer += "one"
buffer += "two"
buffer += "three"

buffer.foreach s do {
  s !!;
  "\n"!!
}
```
In this example, we add three strings to a buffer.
Afterwards, we want to do something with the strings.
So, we use the buffer's ```foreach``` function, bind a variable ```s``` to it and provide it with a block that is executed for each element within the buffer.
The implementation of ```foreach``` will update ```s``` to the current value before evaluating the block.
Foreach is, hence, basically what is known as generalized for loop in many other languages.
But, there are other binders.

Example:
```
var buffer = new container.ArrayBuffer[int]
buffer += 3
buffer += 4
buffer += 5

12 == (buffer.fold(0) l,r do { l + r })
```
The buffer's ```fold``` function takes an ```int``` value, binds two variables and a block.
It starts with the provided ```int``` and uses it as current result.
Afterwards, for each element in the buffer, ```l``` is set to the current result and ```r``` is set to the current value.
Thereafter, the block is evaluated and its result becomes the new current result.
Fold, as used here, is a common form of expressing a sum over the elements of a collection.
As we can see, it returns 12 resulting from 0 + 3 + 4 + 5.

Note: The braces are required if used together with operators.
Both, () and {} can be used here.
This is because it is also legal to have binders not binding any variable.
This can result in syntactic ambiguities where the parser would not be able to decide if a do belongs to a do-while loop or to a binder.

In general, a binder call is an expression resulting in the binder followed by a list of variables followed by a ```do``` followed by a list of blocks.
If the binder expression has the form of a function call, its parameters are passed alongside the variables and blocks to the binder implementation.

The binder definition itself looks like any other function definition.
The bound variables are parameters of the type ```tyr.lang.BlockParameter[T]```.
The bound blocks are parameters of the type  ```tyr.lang.Block[T]```.
The ```BlockParameter``` has an operator ```=``` that updates the bound variable.
The ```Block``` has a function ```eval``` that evaluates the argument block.

Example:
```
type Utils[T : Type] {
  type def sumIf(xs   : container.Iterable[T],
                 x    : BlockParameter[T],
                 cond : Block[bool],
                 map  : Block[int]) {
    var r = 0
    xs.foreach i do {
      // update the parameter to the current value
      x = i

      // if the condition evaluates to true
      if(cond.eval()) {
        // add the result of eval to the result returned by us
        r += map.eval()
      }
    }
    r
  }
}
```

Note: Returns behave like any other return in a function abstraction.
This means that a return from a binder definition returns to the caller.
Whereas a return passed in a block in a binder call will return from the function performing the binder call to its caller.
This is different to returns in lambda expressions in many languages which cannot return from the call of the surrounding function.
This is perfectly fine because lambdas result in function objects that can outlive the surrounding function.
The binder call cannot outlive the surrounding function and it is no value that can be stored in memory.
Blocks aren't either.

Note: Binder calls are always realized by creating local control flow.
Bound variables are local variables of the calling function.
This is fundamentally different from higher order functions and closures in functional languages.
In Tyr, the resulting code is as efficient as if the user would have written it himself.
It can be fully optimized.
In fact, in the previous example, the optimizer will remove all code and replace it with ```true```.
This is, however, only possible if the content of the buffer is known to the optimizer.

Note: Using a ```BlockParameter``` or ```Block``` parameter adds an implicit ```inline``` property.
If an inline function has ```Block``` but no ```BlockParamter``` parameters, it can be used like any other function or operator.
Example: [```Bool.||```](https://github.com/tyr-lang/stdlib/blob/master/lang/src/bool.tyr)

Exercise: Try to define a pair of binders that mimics C's ternary operator.
Hints: You need to define constants True and False or a conversion function because ```tyr.lang.bool``` cannot be extended.
Also, use ```?``` and ```::``` because ```:``` is a keyword.
Lastly, the property ```operator.precedence[100]``` sets an operator's precedence to 100.


## Next steps

???
