std-make
========

C++ generic factories 

Experimental generic factories library for C++11/14/17. This code will eventualy form the basis of a formal proposal to add a generic factories to the C++ standard library.

## Development Status

This code is undocumented, untested, constantly changing, and generally not fit for any use whatsoever.


# License

Most of the source code in this project are mine, and those are under the Boost Software License. I have borrowed the `hana::type` class.

# Supported Compilers

The code is known to work on the following compilers:

* Clang 3.2 -std=c++11 
* Clang 3.5 -std=c++11 -std=c++1y
* GCC 4.8.2 -std=c++11
* GCC 4.9.0 -std=c++11 -std=c++1y

# Motivation

The goal is to be able to have a generic make factory that can be used in place of 
* ```make_shared``` 
* ```make_unique```
* ```make_optional```
* ```make_ready_future```

There are two kind of factories: 
* type constructor with the underlying type as parameter
* emplace construction of the underlying type given the constructor parameters

We can use the template class name as a type constructor

```c++
int v=0;
auto x1 = make<shared_ptr>(v);
auto x2 = make<unique_ptr>(v);
auto x3 = make<optional>(v);
auto x4 = make<future>(v);
auto x5 = make<shared_future>(v);
auto x6 = make<expected>(v);
```

or use the class name to build to support in place construction

```c++
auto x1 = emplace<shared_ptr<A>>(v, v);
auto x2 = emplace<unique_ptr<A>>(v, v);
auto x3 = emplace<optional<A>>();
auto x4 = emplace<future<A>>(v);
auto x5 = emplace<shared_future<A>>(v, v);
auto x6 = emplace<expected<A>>(v, v);
```

We can also make use of the class name to avoid the type deduction

```c++
auto x1 = make<future<int&>>(v);
```

Sometimes the user wants that the underlying type be deduced from the parameter, but the type constructor need more information. A type  holder ```_t```can be used to mean any type

```c++
auto x1 = make<future<_t&>>(v);
auto x2 = make<expected<_t, E>>(v);
```

# Tutorial

## Type constructor factory

## Emplace factory

## How to define a class that wouldn't need customization? 

For the `make` function, the class needs at least to have constructor from the underlying type

```c++
C(X&&);
```

For the `emplace` function, the class needs at least to have a in_place constructor

```c++
C(in_place_t, ...);
```

## How to customize an existing class

When the existing class doesn't provide the needed constructor, the user needs to add the following overloads


```c++
  template <class X, class ...Args>
  C<X> make(type<C<X>>, X&& x);
  template <class X, class ...Args>
  C<X> emplace(type<C<X>>, Args&& ...args);
```

## How to customize the uses of type holder `_t`?

# Design rationale

## Why to have two customization points?

The first factoy `make` uses conversion constructor from the underlying type. 

The second factory is used to be able to do emplace construction given the specific type.

## Customization point

This proposal takes advatage of overloading the `make`/`emplace` functions adding a the tags `type<T>`.

We could rename the customization points `make`/`emplace` function to `custom_make`/`custom_emplace` for example, to make more evident that these are customization points.


# Reference

## Synopsis 

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
  // tag for type conctructor
  template <class T>
    struct type {};

  // type holder
  struct _t {};

  template <template <class ...> class M, class X>
    auto make(X&& x) -> decltype( make(type<M<decay_t<X>>{}, std::forward<X>(x)) );

  template <class M, class X>
    auto make(X&& x) -> decltype( make(type<M>{}, std::forward<X>(x)) );
    
  template <class M, class X>
    M make(type<M>, X&& x);
    
  template <class M, class ...Args>
    auto emplace(Args&& ...args) -> decltype(make(type<M>{}, std::forward<Args>(args)...));

  template <class M, class ...Args>
    M emplace(type<M>, Args&& ...args);

}
}
}

```

## Template function `make` 

```c++
emplate <template <class ...> class M, class X>
  auto make(X&& x) -> decltype( make(type<M<decay_t<X>>>{}, std::forward<X>(x)) );
```

*Effects:* Forwards to the customization point `make` with a template conctructor `type<M<decay_t<X>>>`. As if

```c++
    return make(type<M<decay_t<X>>>{}, std::forward<X>(x));
```


```c++
emplate <class M, class X>
  auto make(X&& x) -> decltype( make(type<M>{}, std::forward<X>(x)) );
```

*Effects:* Forwards to the customization point `make` with a template conctructor `type<M>`. As if

```c++
    return make(type<M>{}, std::forward<X>(x));
```


## Template function `make` - default customization point

```c++
template <class M, class X>
  M make(type<M>, X&& x);
```

*Returns:* A `M` constructed using the constructor `M(std::forward<X>(x))`

*Throws:* Any exception thows by the constructor.

## Template function `emplace` 

```c++
template <class M, class ...Args>
  auto emplace(Args&& ...args) -> decltype(emplace(type<M>{}, std::forward<Args>(args)...));
```

*Effects:* Forwards to the customization point `emplace` with a type conctructor `type<M>`. As if

```c++
    return emplace(type<M>{}, std::forward<Args>(args)...);
```

## Template function `emplace` - default customization

```c++
emplate <class M, class ...Args>
  M emplace(type<M>, Args&& ...args);
```

*Returns:* A `M` constructed using the in place constructor `M(in_place, std::forward<Args>(args)...)`

*Throws:* Any exception thows by the constructor.
