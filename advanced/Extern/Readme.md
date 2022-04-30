> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about using C functions and global variables from Tyr.


## Importing shared objects into projects

Before one can use symbols from C libraries, we have to declare them in our package.draupnir file.
With a line ```extern``` name ```from``` library, a new library can be made available.
Example:

```
extern c from libc
```
Names are case sensitive.
Names must be unique.
Extern library definitions are inherited from imported Tyr packages.
Note that ```tyr.lang``` will always make libc availble under the name ```c```.
Thus, every Tyr package can use extern symbols from libc.

The library name is automatically matched to existing libraries by draupnir.
For instance, if libLLVM-12 and libLLVM-10 are present, ```extern llvm from libLLVM``` matches against either version.
As of Tyr 0.6, the actual match is not defined.
However, ```extern llvm from libLLVM-12``` would match against libLLVM-12.
It would fail if only libLLVM-10 were present.


## Using symbols from a shared object

Type members can be realized with C symbols.
To do so, the initializer or realization is replaced by ```extern```, an identifier serving as library name, a ```.``` and an identifier serving as symbol name within that library.
Example:
```
public type CString <: FlatArray[byte] {
  // either use a type def and the explicit type
  public type def print (this : CString) : int extern c.puts;
  // or use an implicit this which is perfectly fine in this case
  public def print2 : int extern c.puts;
}
```

Note: The same symbol can be bound multiple times.
This prevents issues with inherited bindings from parent libraries.
Also, binding a C symbol creates no runtime overhead of any kind.
However, binding the same symbol twice requires its definitions to have the exact same effective Tyr signature.

Note: Neither the name nor the type of the Tyr member has to match that of the C symbol.
However, the Tyr compiler will realize an access to the Tyr member by delegating to the C symbol.
Thus, it is the duty of the programmer to ensure that correct types are chosen.
Also, as of Tyr 0.6, if the same symbol is bound multiple times, the storage types must be identical.

Note: Some "functions" in C do not compile to functions.
This can be either because they are CPU instructions or macros.
Both do not exist in shared objects and can, hence, not be imported.


## Opaque pointer

In C, it is sometimes the case that you get a pointer to a type but nobody tells you what that type looks like.
This can be reflected in Tyr using the type ```tyr.lang.mem.OpaquePointer```.
For example, the C FILE pointer stdout can be realized as
```
type CFile <: mem.OpaquePointer["FILE"] {
  type val out : CFile extern c.stdout;
}
```

Exercise: Write "Hello Tyr!" into a file hello.txt without using tyr.io.CFile.
Hint: You should use tyr.io.CString to save you a lot of work.


## Next steps

???
