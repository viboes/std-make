<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXX</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2014-10-09</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">Programming Language C++, Library Evolution Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escriba &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@wanadoo.fr</a>&gt;</td>
    </tr>
</table>

C++ generic factory 
===================

Experimental generic factory library for C++11/14/17. This code will eventualy form the basis of a formal proposal to add a generic factories to the C++ standard library.

# Introduction

This paper present a proposal for a generic factory `make`.


# Motivation and Scope


The goal is to be able to have a generic `make` factory that can be used in place of 
* ```make_shared``` 
* ```make_unique```
* ```make_optional```
* ```make_ready_future```
* ```make_expected```

All these types, `shared_ptr<T>`, `unique_ptr<T,D>`, `optional<T>`, `expected<T,E>` and `future<T>`, 
have in common that all of them have an underlying type `T'.

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
auto x1 = make<shared_ptr<A>>(v, v);
auto x2 = make<unique_ptr<A>>(v, v);
auto x3 = make<optional<A>>();
auto x4 = make<future<A>>(v);
auto x5 = make<shared_future<A>>(v, v);
auto x6 = make<expected<A>>(v, v);
```

We can also make use of the class name to avoid the type deduction

```c++
auto x1 = make<future<int&>>(v);
```

Sometimes the user wants that the underlying type be deduced from the parameter, but the type constructor need more information. A type  holder ```_t```can be used to mean any type

```c++
auto x1 = make<future<_t&>>(v);
auto x2 = make<expected<_t, E>>(v);
auto x2 = make<unique_ptr<_t, MyDeleter>>(v);
```

# Tutorial

## Type constructor factory


```c++
template <class TC>
  apply<TC, int> safe_divide(int i, int j)
{
  if (j == 0)
    return make_error<TC>(DivideByZero());
  else
    return make<TC>(i / j);
}
```

We can use this function with different type constructor as

```c++
auto x = safe_divide<optional<_t>>(1, 0);
```

or

```c++
auto x = safe_divide<expected<_t>>(1, 0);
```

  
## Emplace factory



## How to define a class that wouldn't need customization? 

For the `make` default constructor function, the class needs at least to have a constructor from the underlying type

```c++
C();
```

For the `make` copy/move constructor function, the class needs at least to have a constructor from the underlying type.

```c++
C(X&&);
```

For the `make` emplace constructor, the class needs at least to have an em_place constructor

```c++
C(in_place_t, ...);
```

## How to customize an existing class

When the existing class doesn't provide the needed constructor, the user needs to add the following overloads


```c++
  C<void> make(type<C<void>>);
  template <class X, class ...Args>
  C<X> make(type<C<X>>, X&& x);
  template <class X, class ...Args>
  C<X> make(type<C<X>>, in_place_t, Args&& ...args);
```

## How to customize the uses of type holder `_t`?


```c++
namespace std
{
namespace experimental
{
  template <>
    struct C<_t> : lift<C> ;
}
}
```

# Design rationale

## Why to have customization points?

The first factoy `make` uses defaut constructor to build a `C<void>`. 

The second factoy `make` uses conversion constructor from the underlying type. 

The thrid factory `make` is used to be able to do emplace construction given the specific type.

## Customization point

This proposal takes advatage of overloading the `make` functions adding a the tags `type<T>` and `in_place_t`.

We could rename the customization points `make` function to `custom_make` for example, to make more evident that these are customization points.


# Technical Specification

## Synopsis 

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
  // apply a type constuctor TC to the type parameters Args
  template<class TC, class... Args>
  using apply = typename TC::template apply<Args...>;

  // identity meta-function
  template<class T>
  struct identity;

  // tag type
  template <class T>
    struct type 
    {
      using underlying_type = T;
    };

  // type holder
  struct _t {};

  // type constructor tag type
  struct type_constructor_tag {};

  // lift a template class to a type constructor
  template <template <class ...> class TC, class... Args>
  struct lift;

  // reverse lift a template class to a type constructor
  template <template <class ...> class TC, class... Args>
  struct reverse_lift;

  // type constructor customization point. Default implementation make use of nested type type_constructor
  template <class M >
  struct type_constructor;

  // type constructor meta-function
  template <class M >
  using type_constructor_t = typename type_constructor<M>::type;

  // type trait based on inheritance from type_constructor_tag
  template <class TC >
  struct is_type_constructor;
  template <class TC >
  using is_type_constructor_t = typename is_type_constructor<TC>::type;

  // rebinds a type having a underlying type with another underlying type
  template <class M, class U>
  using rebind = apply<type_constructor_t<M>, U>;


  // make() overload
  template <template <class ...> class M>
  M<void> make();
  
  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, class X>
    M<typename std::decay<X>::type> make(X&& x);

    // make overload: requires a type construcor, deduce the underlying type
  template <class TC, class X>
    apply<TC, decay_t<X>> make(X&& x);
    
  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from X
  template <class M, class X>
    M make(type<M>, X&& x);
    
  // make emplace overload: requires a type with a specific underlying type, don't deduce the underlying type from X
  template <class M, class ...Args>
    M make(Args&& ...args);

  // default customization point for TC<void> default constructor
  template <class M>
  M make(type<M>);

  // default customization point for constructor from X
  template <class M, class X>
  M make(type<M>, X&& x);

  // default customization point for in_place constructor
  template <class M, class ...Args>
  M make(type<M>, in_place_t, Args&& ...args);
  
}
}
}

```

## Template function `make` 

### template + void

```c++
  template <template <class ...> class M>
  M<void> make();
```

*Effects:* Forwards to the customization point `make` with a template conctructor `type<M<void>>`. As if

```c++
    return make(type<M<void>>{});
```

### template + deduced underlying type

```c++
template <template <class ...> class M, class X>
  M<decay_t<X>> make(X&& x);
```

*Effects:* Forwards to the customization point `make` with a template conctructor `type<M<decay_t<X>>>`. As if

```c++
    return make(type<M<decay_t<X>>>{}, std::forward<X>(x));
```


### type constructor + deduced underlying type

```c++
  template <class TC, class X>
    apply<TC, decay_t<X>> make(X&& x);
```

*Requires:* `TC`is a type constructor.

*Effects:* Forwards to the customization point `make` with a template conctructor `type<M<void>>`. As if

```c++
    return make(type<apply<TC, decay_t<X>>>{}, std::forward<X>(x));
```

### type + non deduced underlying type


```c++
template <class M, class X>
  M make(X&& x);
```

*Requires:* `M`is not a type constructor and the underlying type of `M`is convertible from `X`.

*Effects:* Forwards to the customization point `make` with a template conctructor `type<M>`. As if

```c++
    return make(type<M>{}, std::forward<X>(x));
```

### type + emplace args 

```c++
template <class M, class ...Args>
  M make(Args&& ...args);
```

*Effects:* Forwards to the customization point `make` with a type conctructor `type<M>` and `in_place_t`. As if

```c++
    return make(type<M>{}, in_place, std::forward<Args>(args)...);
```

### Template function `make` - default constructor customization point for void

```c++
  template <class M>
  M make(type<M>)
```

*Returns:* A `M` constructed using the constructor `M()`

*Throws:* Any exception thows by the constructor.

### copy constructor customization point

```c++
template <class M, class X>
  M make(type<M>, X&& x);
```

*Returns:* A `M` constructed using the constructor `M(std::forward<X>(x))`

*Throws:* Any exception thows by the constructor.

### `in_place_t` - emplace customization point

```c++
make <class M, class ...Args>
  M make(type<M>, in_place_t, Args&& ...args);
```

*Returns:* A `M` constructed using the in place constructor `M(in_place, std::forward<Args>(args)...)`

*Throws:* Any exception thows by the constructor.

# Example of customizations

## `optional`

```c++
namespace std {
namespace experimental {

  // Holder specialization
  template <>
  struct optional<_t>;
  template <class T>
    struct type_constructor<optional<T>>;

}
}
```

## `expected`

```c++
namespace std {
namespace experimental {

  // Holder specialization
  template <class E>
  struct expected<_t, E>;
  template <class T, class E>
    struct type_constructor<expected<T,E>>;
}
}
```

## `future`/`shared_future`

```c++
namespace std {
namespace experimental {

  // customization point for template 
  // (needed because std::experimental::future doesn't has a default constructor)
  future<void> make(type<future<void>>);

  // customization point for template
  // (needed because std::experimental::future doesn't has a conversion constructor)
  template <class DX, class X>
    future<DX> make(type<future<DX>>, X&& x);

  // customization point for template 
  // (needed because std::experimental::future doesn't uses experimental::in_place_t)
  template <class X, class ...Args>
    future<X> make(type<future<X>>, experimental::in_place_t, Args&& ...args);

  // Holder specializations
  template <>
    struct future<_t>;
  template <>
    struct future<_t&>;

  // customization point for template 
  // (needed because std::experimental::shared_future doesn't has a default constructor)
  shared_future<void> make(type<shared_future<void>>);

  // customization point for template 
  // (needed because std::experimental::shared_future<X> doesn't has a constructor from X)
  template <class DX, class X>
    shared_future<DX> make(type<shared_future<DX>>, X&& x);
  
  // customization point for template 
  // (needed because std::experimental::shared_future doesn't use experimental::in_place_t)
  template <class X, class ...Args>
    shared_future<X> make(type<shared_future<X>>, experimental::in_place_t, Args&& ...args);
    
  // Holder specializations
  template <>
    struct shared_future<_t>;
  template <>
    struct shared_future<_t&>;
  template <class T>
    struct type_constructor<shared_future<T>>;

}
}
```

## `unique_ptr`

```c++
namespace std {
namespace experimental {

  // customization point for template 
  // (needed because std::unique_ptr doesn't has a conversion constructor)
  template <class DX, class X>
    unique_ptr<DX> make(experimental::type<unique_ptr<DX>>, X&& x);

  // customization point for template 
  // (needed because std::unique_ptr doesn't uses experimental::in_place_t)
  template <class X, class ...Args>
  unique_ptr<X> make(experimental::type<unique_ptr<X>>, experimental::in_place_t, Args&& ...args);


  // Holder customization
  template <class D>
  struct unique_ptr<_t, D>;

  template <>
  struct default_delete<_t>;


  // type_constructor customization
  template <class T, class D>
  struct type_constructor<unique_ptr<T,D>>;
  template <class T>
  struct type_constructor<default_delete<T>>;


}}
```

## `shared_ptr`

```c++
namespace std {
namespace experimental {

  // customization point for template 
  // (needed because std::shared_ptr doesn't has a conversion constructor)
  template <class DX, class X>
  shared_ptr<DX> make(type<shared_ptr<DX>>, X&& x);

  // customization point for template (needed because std::shared_ptr doesn't uses experimental::in_place_t)
  template <class X, class ...Args>
  shared_ptr<X> make(type<shared_ptr<X>>, in_place_t, Args&& ...args);

  // Holder customization
  template <>
  struct shared_ptr<_t>;
  
  // type_constructor customization
  template <class T>
  struct type_constructor<shared_ptr<T>>;

}}
```

## Development Status

This code is undocumented, untested, constantly changing, and generally not fit for any use whatsoever.


# License

Most of the source code in this project are mine, and those are under the Boost Software License. I have borrowed the `hana::type` class.

# Supported Compilers

The code is known to work on the following compilers:

* Clang 3.2 -std=c++11 
* Clang 3.5 -std=c++11 -std=c++1y
* GCC 4.7.2, 4.8.0, 4.8.1, 4.8.2 -std=c++11
* GCC 4.9.0 -std=c++11 -std=c++1y

# Acknowledgements 

Many thanks to Agustín K-ballo Bergé. 

# References

