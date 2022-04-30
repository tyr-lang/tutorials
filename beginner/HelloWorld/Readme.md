> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This is a classical hello world tutorial.
It will teach you how to write a small program printing "Hello world!" to the console. If you have not installed Tyr yet, follow [this guide](https://github.com/tyr-lang/releases#requirements).

First, one creates a draupnir package definition file.
This file tells draupnir the name of the package and where to look for source code.
It can also be used to tell draupnir the name of the default entry point and the names of used Tyr or C libraries.
If the package definition file has a ```.draupnir``` file extension, draupnir will use it automatically when executed within the same folder.
Here, we create a file ```package.draupnir``` and add the two lines
```
src=src
name=HelloWorld
```

Now, we can start writing the code.
We told draupnir that our code is in ```src```, so, we have to create that folder.
In that folder, we create a file ```hello.tyr```.
When draupnir is asked to compile or run a package, it will use all ```.tyr``` files in the source folder.

Open ```hello.tyr``` and add these three lines
```
type hello {
  type def myMain : void <: entryPoint = "Hello World!\n"!!;
}
```

Tyr is type-oriented.
A consequence of this paradigm is that types are the only top-level entity.
The type ```hello``` owns the function ```myMain```.
```myMain``` is a stateless funciton taking no parameters and returning nothing.
It has an ```entryPoint``` property.
This property signals that the function can serve as the first function to be executed when running this package.
It is equivalent to ```main``` in C, Java or other languages.

We are done now and can run our first program by calling ```draupnir run```.
An executable can be created by calling ```draupnir build```.


## Tests

Testing is the most important activity in software development.
Therefore, Tyr makes testing easy.

Tests can be added to types just like functions.
Tests start with the keyword test, followed by a string providing the tests name and a block providing the test implementation.
```
type hello {
  type def myMain : void <: entryPoint = "Hello World!\n"!!;
  test "hello test" {
    "Hello World from Test!\n"!!;
    return true
  }
}
```
This test will print a short text.
After that, it will tell that everything worked as expected by returning true.
Try to run the test with ```draupnir test```.
It should print something like
```
running 1 tests
=== start test hello world ===
Hello World from Test!
=== end test: success ===
```

See what happens if you replace ```true``` with ```3 + 4 == 2``` or ```3 + 4 == 7```.


## Next steps

[Draupnir](https://github.com/tyr-lang/tutorials/tree/master/beginner/Draupnir): Explains the package manager

[Control Flow](https://github.com/tyr-lang/tutorials/tree/master/beginner/ControlFlow): Explains control-flow-related concepts
