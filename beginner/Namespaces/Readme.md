> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about the creation and usage of namespaces and scopes.

A scope is, essentially, a collection of definitions and scopes.
Scopes form a tree.
In some way, scopes are like folders in file systems.
They are used to structure definitions and to make using them clearer.
Also, they make accessing them significantly shorter in large code bases.

Namespaces are scopes that can contain scopes and type-like definitions.
Types are scopes that contain members.


## Scope Access

Scope access is done using a dot (```.```).
There is no distinction between namespaces, types, members or the way accessed entities are stored in memory.
It is always a dot.
If an open access is performed, e.g. accessing the name ```lang``` from ```tyr.io```, the result is ```tyr.lang```.
This is, because in Tyr, namespaces are a tree and the lookup is retried one level higher until either an entity is found or the tree's root has been reached.
This search strategy is called open access.

Note: A common form of an open access is resolution of the entity behind an identifier such as a field name in an expression.

Note: Closed access, that is an explicit scope access using a dot, will only check members in that scope ignoring parent scopes.

Note: If an open access resolution fails in ```_root_``` a closed resolution in ```tyr.lang``` is performed as last resort.
This rule makes all members of ```tyr.lang``` available without imports.
Such members are ```int```, ```bool``` and many others.


## Namespace Definitions

All namespaces are created implicitly.
This means that they cannot be created by any form of statement within a source file.
The first part of any namespaces comes from the name of a package.
Here, a ```.``` is used to split parents.

Example:
```name=tutorials.beginner.HelloWorld``` and ```name=tutorials.beginner.Draupnir``` will result in the namespace ```tutorials.beginner``` containing the namespaces ```HelloWorld``` and ```Draupnir```.

The other way of creating namespaces is by creating folders in the source directory.
This will create a namespace of the exact same name in the namespace of the parent folder.


## Importing Namespaces

In a file or type, one is usually using a specific set of definitions from other namespaces.
This can be eased by using a ```with``` keyword, followed by a scope access.
This will make the accessed entity available under its name.
Example (three equivalent imports):
```
with tyr.lang.container.Array
with container.Array
with _root_.tyr.lang.container.Array
```

An explicit name can be provided using a ```with``` followed by an identifier, followed by a ```:=``` followed by the scope access.
Example:
```
with Array := tyr.lang.container.Array // same as without :=
with Arr   := tyr.lang.container.Array
```

The scope access expression is just any expression.
As such, the first part is an open lookup that may lead to an undesired result if not fully qualified.
It can be fully qulified by starting with ```_root_```, i.e. the name of the global root scope.


## Type Aliases

Similar to imports, a type from another package can be introduced locally with a type alias.
In contrast to an import, the type is actually created and can be accessed from other files or packages.
A type alias definition consists of a visibility followed by ```type``` followed by an identifier serving as the new type's name followed by ```:=``` followed by an expression resulting in a type, class or interface.

Example:
```
// create type Arr that is identical to tyr.lang.container.Array
type Arr := tyr.lang.container.Array
```
Note: An example type aliased used a lot because it is in ```tyr.lang``` is
```
public type int   := Integer[32];
```

Note: Type alias can be used to create a migration path when moving type definitions to other namespaces.
This happens occasionally when libraries grow a lot over time.


## Next steps

[Type, Class and Interface](https://github.com/tyr-lang/tutorials/tree/master/beginner/TypeClassInterface): Defining data types and operations on them
