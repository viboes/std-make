std-make
========

C++ generic make factory 

Experimental generic make factory library for C++11/14/17. This code will eventualy form the basis of a formal proposal to add a generic make factory to the C++ standard library.

## Development Status

This code is undocumented, untested, constantly changing, and generally not fit for any use whatsoever.


# License

Most of the source code in this project are mine, and those are under the Boost Software License.

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

There are two kind of type constructors: The template class

```c++
int v=0;
auto x1 = make<shared_ptr>(v);
auto x2 = make<unique_ptr>(v);
auto x3 = make<optional>(v);
auto x4 = make<future>(v);
auto x5 = make<shared_future>(v);
auto x6 = make<expected>(v);
```

and the class itself to support emplace construction and avoid the type deduction

```c++
auto x1 = make<shared_ptr<A>>(v, v);
auto x2 = make<unique_ptr<A>>(v, v);
auto x3 = make<optional<A>>(v, v);
auto x4 = make<future<A>>(v, v);
auto x5 = make<shared_future<A>>(v, v);
auto x6 = make<expected<A>>(v, v);
auto x7 = make<future<int&>>(i);
```

A type holder ```_t```can be used to mean any type
```c++
auto x1 = make<future<_t&>>(v);
auto x2 = make<expected<_t, E>>(v);
```

# Reference

## Synopsis 

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
  // tag for template conctructor
  template <template <class ...> class M>
    struct tc {};

  // tag for type conctructor
  template <class T>
    struct c {};

  // type holde
  struct _t {};

  template <template <class ...> class M, class X>
    auto make(X&& x) -> decltype(make(tc<M>{}, std::forward<X>(x)));

  template <class M, class ...Args>
    auto make(Args&& ...args) -> decltype(make(c<M>{}, std::forward<Args>(args)...));

  template <template <class ...> class M, class X>
    M<decay_t<X>> make(tc<M>, X&& x);

  template <class M, class ...Args>
    M make(c<M>, Args&& ...args);

}
}
}

```

## Template function make 

### With template parameter

```c++
template <template <class ...> class M, class X>
  auto make(X&& x) -> decltype(make(tc<M>{}, std::forward<X>(x)));
```

*Effects:* Forwards to the customization point `make` with a template conctructor `tc<M>`. As if

```c++
return make(tc<M>{}, std::forward<X>(x));
```

### With type parameter

```c++
template <class M, class ...Args>
auto make(Args&& ...args) -> decltype(make(c<M>{}, std::forward<Args>(args)...));
```

*Effects:* Forwards to the customization point `make` with a type conctructor `c<M>`. As if

```c++
return make(c<M>{}, std::forward<Args>(args)...);
```

### template type constructor default customization


```c++
template <template <class ...> class M, class X>
  M<decay_t<X>> make(tc<M>, X&& x);
```

*Returns:* A `M<decay_t<X>>` constructed using the in place constructor `M<decay_t<X>>(in_place, std::forward<X>(x))`

*Throws:* Any exception thows by the constructor.

### template type constructor default customization


```c++
  template <class M, class ...Args>
  M make(c<M>, Args&& ...args);
```

*Returns:* A `M` constructed using the in place constructor `M(in_place, std::forward<Args>(args)...)`

*Throws:* Any exception thows by the constructor.
