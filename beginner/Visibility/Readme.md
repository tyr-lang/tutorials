> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about visibility of entities.
Visibility is a concept to hide implementation details from users.
Its main purpose is to make modularization of software more efficient.
It is a core concept of object-oriented programming.
There, it is known as information hiding.
The idea is that the implementer of a type or library knows all the details.
But a user will only make use of certain aspects so that he can ignore all the implementation details.
For instance, if you use a buffer, you care about adding elements to it.
But you do not care about arrays or counters or whatever is used to realize this.


## default and public

In Tyr, types, classes, interfaces, fields, functions, properties and type aliases have a visibility.
If the visibility is omitted, the default visibility is used.
The default visibility allows all entities within the same library to access the entity.
This is the default visibility because it is usually fine to not care about visibility as long as one is within the same code base.

If an entity should be accessible to anybody, ```public``` visibility has to be used.


## private protected

If visibility should be restricted based on scopes, ```private``` can be used.
```private``` takes an optional parameter in square brackets.
If the parameter is provided, any member of that scope can access the entity.
If no parameter is provided, only the member's enclosing type can access the entity.

In an object-oriented design, it is often required to allow subtypes to access an entity.
This can be achieved using ```protected``` visibility.
```protected``` can be used on fields and functions, only.
```protected``` grants access to any member of a subtype.
Like ```private```, ```protected``` takes an optional parameter.
If the parameter is provided, only members of subtypes in the provided package can access the entity.
If no parameter is provided, an implicit ```_root_``` is used, i.e. any subtype has access.
Furthermore, functions and fields may only be overridden if a protected access is granted.

In contrast to basically any other language, private and protected can be used together.
This allows defining public access to a type while preventing subtypes in other libraries using, for instance, ```private[_root_] protected[some.package]```.
If a member has a ```protected``` but no ```private```, an implicit ```private``` without parameters is added.
If a member has a ```private``` but no ```protected```, an implicit ```protected``` with the parameter of ```private``` is added.

Note that it is not required for the defining type to have either private or protected access.
A type and its members can always access itself and its members.

## override

Members can have an ```override``` visibility.
Let's, for a second, step back and assume that this were not the case.
If an overriden member could have its own visibility two interesting cases arise.
The first is that the super member has more visibility than the overriding member.
In this case, a user would get access from the super member.
Thus, this does not make sense and should be forbidden.
The second is that the super member has less visibility than the overriding member.
This means that the overriding member exposes internal state of a super type.
Thereby, the architecture underlying the super type's implementation would be broken.
Hence, the only useful visibility is the exact visibility of the super member.
Therefore, Tyr eliminates the risk of getting this wrong by using ```override``` as visibility.
Access to an ```override```-visible member is granted if access to one of its super members is granted.

Furthermore, ```override``` can take a list of arguments in square brackets.
The arguments are used as super members.
This is a list because super members can be inherited from, both, the super class and several super interfaces.

Example:
```
public class ArrayBuffer[T : Type] <: Seq[T] {
  /*..*/
  override [Iterable.forall]
  def forall (x : BlockParameter[T], predicate : Block[bool]) : bool {..}
}
```

Note that a member with default visibility implicitly overriding another member has implicit ```override``` visibility.
This is exactly what one would expect to happen.
The sole alternative to this rule is to create an error message forcing the user to add ```override``` to the code.


## Tests

From within tests a special rule exists.
If an access from test fails, it is still granted if the accessed entity is defined in the same package as the test.
Or in other words, for tests, any entity has an additional default visibility.

This makes writing tests a lot easier, because private members can be accessed to ensure that some change has happened as intended.
Likewise, it makes writing testable code easier, because one does not have to write package private helper methods to check such criteria.
Also, as this does not affect tests from other packages, the public API can still be tested using tests in a test package compiled separately.


## Type instances and other substitution results

Access rules apply only to explicit code.
This means that if a library instantiates e.g. ```ArrayBuffer``` from tyr.lang, the instance may still access its private fields.
Actually, this applies to all forms of substitutions or implicit inlining.
For example, usage of the binder ```ArrayBuffer.foreach``` results in an access to the buffer's backing array getting inlined into your code.
Still, no access grant is required to do so.
The only thing required is access to the entity that causes the substitution to happen.
In this example, this is ```ArrayBuffer.foreach``` which is public.


## Next steps

[Tests](https://github.com/tyr-lang/tutorials/tree/master/beginner/Tests): Learn about embedding tests into your code
