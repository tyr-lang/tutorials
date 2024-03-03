> Authors: Timm Felden

> Reviewed for Tyr Version: 0.7.0

This tutorial is about the Tyr package manager ```draupnir```.
Draupnir makes creating and running tyr code easy.
It silently manages all the compilation steps for you when calling the ```tyr``` command.
The tutorial will teach you how to use its features and how to write package definitions.

## Package definitions

Package definition files have the file extension ```.draupnir```.
Package definitions are line-based.
Empty lines and lines starting with a ```#``` are ignored.


Every package has a ```name``` that identifies the package.
The name is used when collaborating with other packages.
Therefore, it should be unique and meaningful.

Example:
```
name=tutorials.beginner.Draupnir
```


The package's source folder can be defined with ```source``` or ```src```.
Only a single source folder can be defined.
The default source folder is the directory containing the package definition.
Note that subdirectories of the source folder are implicit namespace definitions.


Example:
```
src=.
```


Packages can use Tyr libraries using ```with```, ```using``` or ```import```.
Tyr libraries are identified by their package names.
Any number of libraries can be used.
```tyr.lang``` is always used implicitly.

Example:
```
with=tutorials.beginner.HelloWorld
with=tyr.io
```


Packages can use C libraries using ```extern``` Tyr name ```from``` lib name.
The Tyr name is used before the dot before an extern realization in Tyr code.
The lib name is the name of a shared object quiered using ldconfig.
Using C libraries is transitive and will be exported to other Tyr packages.
LibC is used by ```tyr.lang``` and, therefore, always available.

Example:
```
extern c from libc
```


The package can define a default entry point using ```entryPoint```.
Only one default entry point can be defined.
If the package contains only a single function with an entryPoint property, it is the implicit default entry point.
Note that, as of Tyr 0.6, only entryPoints from the current package are considered.

Example:
```
entryPoint=main
```


## Package repository

The only form of package in Tyr at the moment is a tyr library file (.tl).
A Tyr library is a compiled form of Tyr sources and the package definition.
All information required to compile dependent libraries are contained within the .tl file.

The user's package repository is located in ~/.draupnir/libs.
.tl files in this repository are used to fulfill dependencies.
They do not contain tests.

Note: if the package repository does not contain a required .tl-file but the sources are within the search path, most commands will implictly create it from source.
If, however, sources in the search path are changed, the user has to update the installed version manually.


## Command line interface

Draupnir offers a help if called with ```tyr -h```.
Commands have individual helps, e.g. ```tyr run -h```.

### Build, Run, Test

The most important command is ```tyr test```.
It compiles the current package and runs all its tests.
Tests can be filtered with the ```-t``` parameter.
Try running tests in this project with ```tyr test -t test``` and ```tyr test -t draupnir```.

An entry point can be executed with ```tyr run```.
This will compile and the current package and execute the default entry point.
If no default entry point exists, the target entry point can be provided with the ```-e``` parameter.
Try running this project with ```tyr run``` and ```tyr run -e alt```.
Note: run can be called from anywhere to execute .tl-files with default entry points from the package repository.
If you have a copy of the tutorials in your Tyr search path, you can run the hello world tutorial via ```tyr run -tl tutorials.beginner.HelloWorld```.

If you want to ship an executable use ```tyr build```.
By default, the executable will share the package's name.
It can be changed with the  ```-o``` parameter.
Try creating an executable with ```tyr run -e alt -o out```.


### Install, Build-deps, Version Guards

Once work on a library has been completed, it can be installed into the local repository using ```tyr install```.
Likewise, all libraries used by this library can be built with ```tyr build-deps```.
This is, however, usually not required as most commands will automatically build missing dependencies if the sources are in the Tyr search path.

Tyr library files have guards that ensure that they are used with the dependencies that were used to compile them.
This prevents a lot of very nasty bugs that plague most other programming languages like ODR violations in C++ or ClassLoader issues and module system runtime errors in Java.
Also, it allows .tl-files to hold pointers into other .tl-files saving a lot of time in linking and reducing the size of Tyr library files.


### Config

Lastly, there is the ```tyr config``` command.
Its main purpose is to configer the Tyr search path.
The parameter ```--clear-projects``` clears the search path.
The parameter ```--add-project``` adds a directory to the search path.
The parameter ```--list-projects``` shows to the search path.
Note: draupnir will look into subfolders of search path entries until it finds a .draupnir-file.


## Next steps

[Control Flow](https://github.com/tyr-lang/tutorials/tree/master/beginner/ControlFlow): Explains control-flow-related concepts

[Namespaces](https://github.com/tyr-lang/tutorials/edit/master/beginner/Namespaces): Create and use name spaces and scopes.
