> Authors: Timm Felden

> Reviewed for Tyr Version: 0.6.1

This tutorial is about arrays.
Arrays are contiguous sequences of elements of the same type.
Arrays serve as the foundation of most efficient data types.


## Regular Arrays

The array class ```tyr.lang.container.Array``` implements heap-allocated arrays.
This array class has a fixed size that has to be provided on creation.
The array class knows its length.
So, you don't have to remember it.
Example:
```
var arr = new container.Array[int](2)

2U == arr.size()

delete arr
```
Note: In Tyr, containers use Unsigned[64] as size type.
This is required to have arrays with more than 2^32 elements.
While this may look like a lot of elements at first glance, these days one could require even larger arrays.
If, for instance, an array of bytes is used to represent a file loaded to memory, one would be restricted to files smaller than 4GB.
Clearly, an inacceptable restriction.
Hence, we use larger indices and sizes.

Elements in the array can be accessed by index.
If an array has size n, indices including 0 until n-1 are valid.
An access is done using apply, i.e. by using the array like a function from index to element type.
Likewise, a new element can be stored at an index of an array using update, i.e. by assigning a new value to the result of accessing an element.
Example:
```
var arr = new container.Array[int](2)

arr(0) = 3
arr(1) = 5

8 == arr(0) + arr(1)

delete arr
```


## Variable Size Arrays

Sometimes, one does not know how many elements are going to be in an array when creating it.
In this case an ```tyr.lang.container.ArrayBuffer``` can be used.
In essence, this is an array that will automatically resize itself if an element is appended when there is no more space left.
Example:
```
var arr = new container.ArrayBuffer[int](2)

0U == arr.size()

arr += 3
arr += 5

2U == arr.size()

8 == arr(0) + arr(1)

delete arr
```

If an element is no longer needed, it can be removed again using its index.
Example:
```
var arr = new container.ArrayBuffer[int]

0U == arr.size()

arr += 3
arr.remove(0)

0U == arr.size()

delete arr
```
Note: removing an element requires moving all elements behind it.
Therefore, it is a lot faster to remove last elements first.

Both, ```Array``` and ```ArrayBuffer``` are ```Iterable```, i.e. they have binders allowing efficient and easy interaction with all elements.
The ```foreach``` binder binds an element and applys a block on each element of the array.
The ```count``` binder binds an element and applys a block yielding a condition on each element of the array and returns the number of elements satisfying that condition.
The ```forall``` binder binds an element and applys a block yielding a condition. If all elements fulfill the condition, ```true``` is returned. If one element does not fulfill the condition, no more elements will be inspected and ```false``` is returned.
The ```exists``` binder binds an element and applys a block yielding a condition. If an element fulfills the condition, ```true``` is returned immediately. If no element fulfills the condition, ```false``` is returned.
The ```fold``` binder takes an initial result and binds a running result and an element.
It applies a block for each element onto the running result and returns the running result afterwards.
Example:
```
type def sum(arr : container.ArrayBuffer[int]) = arr.fold(0) l,r do (l+r)
```


## Flat Arrays

Both, ```Array``` and ```ArrayBuffer``` are heap-allocated, know their size and their type.
This means that accessing them requires following a pointer which can be too expensive if done a lot.
Therefore, Tyr has efficient alternatives.
The types ```FlatArray``` and ```FlatSizedArray``` are both array types.
As types, they can be allocated anywhere.
```FlatArray``` gets its size on allocation.
```FlatSizedArray``` gets it size from a type parameter.
Example:
```
var arr = new container.FlatSizedArray[int, 2U]

arr(0) = 3
arr(1) = 5

8 == arr(0) + arr(1)
```

## Next steps

[TypeClassObject](https://github.com/tyr-lang/tutorials/tree/master/beginner/TypeClassObject): Learn about the three different member lifetimes in Tyr.
