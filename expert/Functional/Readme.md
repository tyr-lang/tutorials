> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about the relation between functional programming and generalized binders.
At first glance, the two concepts might look very similar.
Like templates and generics.
But, although they may feel similar to casual users, there are important differences.

Functional programming is based on a compiler generated function object called [closure](https://en.wikipedia.org/wiki/Closure_(computer_programming)).
In essence, the compiler checks which local variables and values are used in the local function and creates an object holding them.
This has two consequences.
The one that makes this idea popular is that it allows definition of local functions that can be passed to different scopes.
This pattern can be used to create APIs that allow very productive programming wrt. time required to implement a functional requirement.

However, there is a second consequence.
The implicit creation of the closure can have undesired consequences on local variables.
For instance if an index is used with a foreach loop, the index variable is lifted to the closure.
Also, memory management code for it could be required ensuring that it is released once it is no longer used.
This causes it to be way more expensive in both memory and cpu time.
Additionally, the lifted index variable can likely not be optimized out.

Binders on the other hand do not suffer this performance penalty.
This is not because Tyr is so much better than functional programming languages.
It is because binders are fundamentally different.
By taking away the "passing around" part of functions, the lifting of local variables is no longer required.
Thus, similar APIs can be created without paying the ten plus times overhead of closures.

If the closure is required in Tyr code, it can be created explicitly using a function object.
Furthermore, this has the advantage of ensuring that only intended lifting happens ensuring the resulting programming to be efficient.
