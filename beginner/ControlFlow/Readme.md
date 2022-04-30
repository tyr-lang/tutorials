> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about control flow in Tyr.
It will teach you how to use variables, operators, blocks, conditional execution and loops.
You should be familiar with most of the concepts if you have programmed before.


## Values and Variables

First of all, we introduce variables and values.
A value can be created by using the ```val``` keyword, followed by a name, a ```=``` and an expression yielding a result.
Example: ```val x = 7``` creates the value x. Every usage of x results in 7.

Values can be accessed until they leave their scope, i.e. the block in which they have been declared.
Values must be initialized and can be given an explicit type like ```val x : int = 7```.
Values are immutable and they do not necessarily require memory.

If a value shall be modified, a variable has to be created using the ```var``` keyword.
Variables have an operator ```=``` allowing to update the value stored in them.
Local variables are backed by stack-allocated memory which can be optimized out.
In terms of lifetimen and access, variables share the behaviour of values.
Regarding initialization, variables must have either an explicit type or an initializer but may have both.
Example: ```var x : int; x = 7``` creates the int variable x. Then, it is set to the value 7.

Note: A field can be created using the ```var``` and ```val``` keywords within a type's body.


## If and Switch

If one desires to execute code only in certain cases, an ```if``` statement can be used.
```if``` is followed by an expression of type ```bool``` serving as condition.
It is followed by an expression that will be executed if the condition evaluates to ```true```.
Optionally, an ```else``` can follow which is followed by an expression that will be executed if the condition evaluates to ```false```.
Note that blocks are expressions.

Example:
```
var x = 7
// update x to 3 if x is 2 (which it is not)
if (2 == x) {
  x = 3
}
// x is still 7
7 == x
```


Another form of case distinction is the ```switch``` statement.
As of Tyr 0.6, it can be used to take an integer and continue execution based on its value.
This statement starts with the ```switch``` keyword and is followed by the expression yielding the value for the case distinction.
Afterwards, a block follows containing the case handling.
It can hold any number of cases of the form ```if```, literal integer, ```:```, expression.
It must end with an ```else``` followed by an expression.
The ```if``` branches are executed if the value equals the literal integer.
Otherwise, the ```else``` branch is executed.
After execution of a branch, the control flow continues after the ```switch``` statement.


Example:
```
var x = 3
switch x {
  if 1: { x = 42 }
  if 3: { x = 7 }
  else  { x = -1 }
}
// now, x is 7
7 == x
```


## While and Do-While

An expression can be executed repeatedly using a ```while``` loop.
It is created by using ```while``` followed by an expression of type bool forming the condition and another expression forming the body.
While the condition evaluates to ```true``` the body is executed another time.
Thus, the body is never executed if the condition is ```false``` after the first evaluation.

Example (print seven lines with "hello!"):
```
var x = 7
while (x > 0) {
  "hello!\n"!!;
  --x
}
```

Likewise, a do-while loop can be used.
In contrast to the while loop, the do-while loop executes the body before checking the condition.
It is created by using ```do``` followed by an expression forming the body and a ```while``` followed by an expression of type bool forming the condition.
While the condition evaluates to ```true``` the body is executed another time.
Thus, the body is executed once if the condition is ```false``` after the first evaluation.

Example (print three lines with "hello!"):
```
var x = 3
do {
  "hello!\n"!!;
  --x
} while (x != 3 && x > 0)
```


## Generalized Binders

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
So, we use the buffer's foreach function, bind a variable ```s``` to it and provide it with a block that is executed for each element within the buffer.
The implementation of foreach will update s to the current value before evaluating the block.
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
The buffer's fold function takes an int value, binds two variables and a block.
It starts with the provided int and uses it as current result.
Afterwards, for each element in the buffer, ```l``` is set to the current result and ```r``` is set to the current value.
Thereafter, the block is evaluated and its result is the new current result.
Fold, as used here, is a common form of expressing a sum over the elements of a collection.
As we can see, it returns 12 resulting from 0 + 3 + 4 + 5.

Note: The braces are required if used together with operators.
Both, () and {} can be used here.
This is because it is also legal to have binders not binding any variable.
This can result in syntactic ambiguities where the parser would not be able to decide if a do belongs to a do-while loop or to a binder.

In general, a binder call is an expression resulting in the binder followed by a list of variables followed by a ```do``` followed by a list of blocks.
If the binder expression has the form of a function call, its parameters are passed alongside the variables and blocks to the binder implementation.

Note: Binder calls are always realized by creating local control flow.
Bound variables are local variables of the calling function.
This is fundamentally different from higher order functions and closures in functional languages.
In Tyr, the resulting code is as efficient as if the user would have written it himself.
It can be fully optimized.
In fact, in the previous example, the optimizer will remove all code and replace it with ```true```.
This is, however, only possible if the content of the buffer is known to the optimizer.


## Next steps

[Namespaces](https://github.com/tyr-lang/tutorials/tree/master/beginner/Namespaces): Create and use name spaces and scopes.

[Advanced Control Flow](https://github.com/tyr-lang/tutorials/tree/master/advanced/ControlFlow): Learn about expression results and using and definition of binders

[Simple Expressions](https://github.com/tyr-lang/tutorials/tree/master/expert/SimpleExpression): Learn the difference between expressions and simple expressions.
