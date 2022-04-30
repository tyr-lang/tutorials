> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about the proxy property.
In Tyr, ```tyr.lang.proxy``` is used to let name binding continue its search in a different type.

While this is a very powerful feature, it's only useful in library programming to realize the [proxy pattern](https://en.wikipedia.org/wiki/Proxy_pattern) transparently.
Here, it adds a lot of usability to the resulting API.
However, it should be applied with great care.
An example is ```tyr.lang.Ref[T : Type] <: proxy[T]```.
In essence, this allows variables to be used like their base types.

The tutorial code shows Groovy-style delegates.
A C#-style delegate can currently not be implemented, because, as of Tyr 0.6, it is not allowed to bind a Block in a CT application on a type.
Therefore, there is no painless way to provide the delegate type the get and set implementations.
