> Authors: Timm Felden

> Reviewed for Tyr Version: 0.7.0

This tutorial is about tests.
Tests are key to creating high-quality software.
Hence, testing is an important task in software development.
Adding automated tests to your code is a good way to ensure that your code does what you intend it to.
Moreover, it is the cheapest way to ensure that it stays that way.
Therefore, in Tyr, writing automated tests is really easy.


## Regular tests

A common form of testing is to execute some code.
Afterwards, a test criterion is checked.
If the criterion is true, the test counts as passed.

A test can be defined within the body of a type-like.
It consists of the word ```test```,
 followed by string literal serving as name,
  followed by an optional list of test properties and,
  finally, a block serving as the test implementation.
Tests are evaluated in the scope of the enclosing type.
Thus, they can access type members without prefix.
To some extent, a test is a type function taking no parameters returning a ```bool``` to signal if the test was successful.
However, the test can access all private entities within the same library.
Also, tests are not part of a released library or executable.

Example:
```
type T {
  public type val global = 42

  test "use global" {
    42 == global
  }
}
```

## Test properties

Tests, like most entity definitions in Tyr, can have properties.
Tests, however, have special language-defined properties.
As of Tyr 0.6, there is ```noCompile``` and ```noWarning```.

```noCompile``` ensures that an error happens during the evaluation of the test body.
This is most useful to ensure that a certain API that is currently inaccessible stays that way.
Also, it can be used that certain implicit conversions do not happen.

```noWarning``` ensures that no warning occurs during the evaluation of the test body.
This can be used if project members do not care for warnings.

Since Tyr 0.7, there are test categories.
The entire list can be viewed with ```tyr test --explain-message-categories```.
The parameter ```--print-message-categories``` adds the test categories to compiler messages.
Since Tyr 0.7, ```noCompile``` takes a mandatory category in square brackets as parameter.
If the exact category does not matter or may change, a substring can beused like ```noCompile[NAM_]``` to match all name resolution errors.

Note that expected errors are usually suppressed.
They can be made visible with the ```-v``` option.

Note that only errors that can be recovered from can be used in no compile tests.
The ```SYN``` and ```EXT``` categories will not work here.
Intended candidates are ```NAM```, ```TYP``` and the configurable categories.


## Test execution

Tests can be executed with ```tyr test```.
This will execute all tests in the current project.
Tests can be filtered with ```-t```.
So, for example, ```tyr test -t global``` will execute all tests with "global" in their name.
If the ```-v``` option is provided, draupnir will print the expected error messages as well.
This can be used to check if the issues correspond to the intended errors.

Tests are always executed in isolation.
That means using the test as single entry point.
Thus, global state is always the initial state when the test starts.
Also, this allows running tests in parallel.
The number of test execution jobs can be controlled with the ```-j``` parameter.
By default, one test per virtual CPU core is executed.


## Jenkins integration

Draupnir can generate JUnit-compatible xml files as test results.
This is done when the ```-x``` parameter is used with an optional output directory.
This feature can be used to publish Tyr test results in Jenkins.

The ```--all-recursive``` parameter can be used to instruct draupnir to look for Tyr projects in subdirectories.
In combination, ```tyr test --all-recursive -x xml-dir``` will result in execution of all tests in all subdirectories and the results getting written into the subfolder xml-dir.
Thus, test execution via Jenkins can be implemented with a single command.


## Next steps

[Arrays](https://github.com/tyr-lang/tutorials/tree/master/beginner/Arrays): Learn about how to work with many instances of the same type.
