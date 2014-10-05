std-make
========

C++ generic make factory 

Experimental generic make factory library for C++11/14/17. This code will eventualy form the basis of a formal proposal to add a generic make factory to the C++ standard library.

Development Status
------------------
This code is undocumented, untested, constantly changing, and generally not fit for any use whatsoever.


License
-------
Most of the source code in this project are mine, and those are under the Boost Software License.

Supported Compilers
-------------------
The code is known to work on the following compilers:
* Clang 3.2 -std=c++11 
* Clang 3.5 -std=c++11 -std=c++1y
* GCC 4.8.2 -std=c++11
* GCC 4.9.0 -std=c++11 -std=c++1y

Introduction
------------

The goal is to be able to have a generic make factory that can be used in place of 
* ```make_shared``` 
* ```make_unique```
* ```make_optional```
* ```make_ready_future```

There are two kind of type constructors: The template class

```c++
int v=0;
make<shared_ptr>(v);
make<unique_ptr>(v);
make<optional>(v);
make<future>(v);
make<shared_future>(v);
make<expected>(v);
```

and the class itself

```c++
make<shared_ptr<A>>(v, v);
make<unique_ptr<A>>(v, v);
make<optional<A>>(v, v);
make<future<A>>(v, v);
make<shared_future<A>>(v, v);
make<expected<A>>(v, v);
make<expected<_, E>>(v);
```

A type holder ```_t```can be used to mean any type
```c++
make<future<_t&>>(v);
make<expected<_t, E>>(v);
```

