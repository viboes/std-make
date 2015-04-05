<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXX</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2015-03-08</td>
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

Experimental generic factories library for C++17. 

# Introduction

This paper presents a proposal for a generic factory `make` that allows to make generic 
algorithms that need to create an instance of a wrapped class from its underlying types.


# Motivation and Scope

All these types, `shared_ptr<T>`, `unique_ptr<T,D>`, `optional<T>`, `expected<T,E>` and 
`future<T>`, have in common that all of them have an underlying type `T'.

There are two kind of factories: 
* type constructor with the underlying types as parameter

 * ```make_optional```
 * ```make_ready_future```
 * ```make_expected```

* emplace construction of the underlying type given the constructor parameters

 * ```make_shared``` 
 * ```make_unique```

When writing an application, the user knows if the function to write should return a specific type,  
as `shared_ptr<T>`, `unique_ptr<T,D>`, `optional<T>`, `expected<T,E>` or `future<T>`. E.g.
when the user knows that the function must return a owned smart pointer it would use `unique_ptr<T>`.

```c++
template <class T>
unique_ptr<T> f() {
    T a,
    ...
    return make_unique(a);
}
```

If the user knows that the function must return a shared smart pointer

```c++
template <class T>
shared_ptr<T> f() {
    T a,
    ...
    return make_shared(a);
}
```

However when writing a library, the author doesn't always know which type the user wants as a result.
In these case the function library must take some kind of type constructor to let the user make the choice.

```c++
template <template <class> class TC, class T>
TC<T> f() {
    T a,
    ...
    return make<TC>(a);
}
```

In addition, we have factories for the product types such as `pair`and `tuple`

 * ```make_pair```
 * ```make_tuple```


We can use the template class name as a type constructor

```c++
int v=0;
auto x1 = make<shared_ptr>(v);
auto x2 = make<unique_ptr>(v);
auto x3 = make<optional>(v);
auto x4 = make<future>(v);
auto x5 = make<shared_future>(v);
auto x6 = make<expected>(v);
auto x7 = make<pair>(v, v);
auto x8 = make<tuple>(v, v, 1u);
```

or making use of reference_wrapper type deduction

```c++
int v=0;
future<int&> x4 = make<future>(std::ref(v));
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
int i;
auto x1 = make<future<long>>(i);
```

Sometimes the user wants that the underlying type be deduced from the parameter, but the 
type constructor needs more information. A type holder ```_t```can be used to mean any 
type `T`.

```c++
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
    return none<TC>();
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

For the `make` default constructor function, the class needs at least to have a 
default constructor

```c++
C();
```

For the `make` copy/move constructor function, the class needs at least to have a 
constructor from the underlying type.

```c++
C(X&&);
```


## How to customize an existing class

When the existing class doesn't provide the needed constructor as e.g. `future<T>`, the 
user needs to add the missing overloads so that they can be found by ADL.

```c++
namespace boost {
  future<void> make(meta::type<future<void>>) 
  { 
    return make_ready_future(); 
    }
  template <class T, class ...Args>
  future<T> make(meta::type<future<T>>, Args&& ...args)
  { 
    return make_ready_future<T>(forward<Args>(x)...); 
  }
}
```


## How to define a type constructor?

The simple case is when the class has a single template parameter as is the case for 
`future<T>`. 

```c++
namespace boost
{
  struct future_tc {
    template <class T>
    using apply = future<T>;
  };
}
```

When the class has two parameter and the underlying type is the first template parameter, 
as it is the case for `expected`, 

```c++
namespace boost
{
  template <class E>
  struct expected_tc<E> {
    template <class T>
    using apply = expected<T, E>;
  };
}
```

If the second template depends on the first one as it is the case of `unique_ptr<T, D>`, 
the rebind of the second parameter must be done explicitly. 

```c++
namespace boost
{
  namespace detail
  {
    template <class D, class T>
    struct rebind;
    template <template <class...> class TC, class ...Ts, class ...Us>
    struct rebind<TC<Ts...>, Us...>> {
      using type = TC<Us...>; 
    };
    template <class M, class ...Us>
    using rebind_t = typename rebind<M, Us...>>::type;
  }
  
  template <>
    struct default_delete<experimental::_t> 
  {
    template<class T>
    using apply = default_delete<T>;
  };
    
  template <class D>
    struct unique_ptr<experimental::_t, D>
  {
    template<class T>
    using apply = unique_ptr<T, detail::rebind_t<D, T>>;
  };
}
```

## Helper classes

Defining these type constructors is cumbersome. This task can be simplified with some helper classes. 

```c++
  // type holder
  struct _t {};
  
namespace meta
{
  // identity meta-function
  template<class T>
    struct id
    {
      using type = T;
    };

  // lift a template class to a type constructor
  template <template <class ...> class TC, class... Args>
    struct lift;

  // reverse lift a template class to a type constructor
  template <template <class ...> class TC, class... Args>
    struct reverse_lift;
    
  template <class M, class ...U>
  struct rebind : id<typename M::template rebind<U...>> {};

  template <template<class ...> class TC, class ...Ts, class ...Us>
  struct rebind<TC<Ts...>, Us...> : id<TC<Us...>> {};

  template <class M, class ...Us>
  using rebind_t = eval<rebind<M, Us...>>;
    
}
```

The previous type constructors could be rewritten using these helper classes as follows:

```c++
namespace boost
{
  template <> struct future<_t> : std::experimental::meta::lift<future> {};
}
```

```c++
namespace boost
{
  template <class E> struct expected<_t, E> : std::experimental::meta::reverse_lift<expected, E> {};
}
```

```c++
namespace boost
{
  
  template <>
    struct default_delete<_t> : std::experimental::meta::lift<default_delete> {};
    
  template <class D>
    struct unique_ptr<_t, D>
  {
    template<class T>
    using apply = unique_ptr<T, std::experimental::meta::rebind_t<D, T>>;
  };
}
```

# Design rationale

## Why to have customization points?

The first factoy `make` uses default constructor to build a `C<void>`. 

The second factoy `make` uses conversion constructor from the underlying type(s). 

The third factory `make` is used to be able to do emplace construction given the specific 
type.

## Customization point

This proposal takes advatage of overloading the `make` functions adding the tag 
`type<T>`.

We could rename the customization points `make` function to `make_custom` for example, to 
make more evident that these are customization points.

## `reference_wrapper<T>` overload to deduce `T&`

As it is the case for `make_pair` when the parameter is `reference_wrapper<T>`, 
the type deduced for the underlying type is `T&`.

## Product types factories

This proposal take in account also product type factories (as `std::pair` or `std::tuple`). 

```c++
  // make product factory overload: Deduce the resulting `Us` 
  template <template <class...> class T, class ...Ts>
    T<Us...> make(Ts&& ...args);
  // make product factory overload: Deduce the resulting `Us` 
  template <class TC, class ...Ts>
    apply<T, Us...> make(Ts&& ...args);
```

```c++
auto x = make<pair>(1, 2u);  
auto x = make<tuple>(1, 2u, string("a");  
```

## High order factory

It is simple to define a high order `maker<TC>` factory of factories that can be used in standard algorithms. 

For example

```c++
std::vector<X> xs;
std::vector<Something<X>> ys;
std::transform(xs.begin(), xs.end(), std::back_inserter(ys), maker<Something>{});
```

```c++
  template <template <class> class T>
  struct maker {
    template <typename ...X>
    constexpr auto
    operator()(X&& ...x) const
    {
        return make<T>(forward<X>(x)...);
    }     
  };
```

The main problem defining function objects is that we can not have the same class with different template parameters.
The `maker` template class has a template class parameter. 
We need an additional class that takes a meta-function class and a type.

```c++
  template <class MFC>
  struct maker_mfc {
    template <class ...Xs>
    constexpr auto
    operator()(Xs&& ...xs)
    {
      return make<MFC>(std::forward<Xs>(xs)...);
    }
  };
  
  template <class M>
  struct maker_t
  {
    template <class ...Args>
    constexpr M operator()(Args&& ...args) const
    {
      return make<M>(std::forward<Args>(args)...);
    }
  };
```

# Open points

The authors would like to have an answer to the following points:

## Should the customization be done with overloading or with traits?

The current proposal uses overloading as customization point. The alternative is to use traits.

If overloading is preferred, 

* should the customization function names be suffixed e.g. with  `_custom`?

* As `id` and `type` do the same, should the `type` be replaced by `id`?

## Should the namespace `meta` be used for the meta programming utilities `apply` and `type`?

## Should the function object factories be part of the proposal?

The function objects `make_tc`, `make_mfc` and `make_t` could be quite useful. 

What should be the default fo `maker`? 

## Is there an interest on the helper meta-functions `id`, `types`, `lift`, `lift_reverse` and `rebind`?

If yes, should them be part of this proposal?

## Should the customization of the standard classes `pair`, `tuple`,`optional`, `future`, `unique_ptr`, `shared_ptr` be part of this proposal?

# Technical Specification

## Synopsis 

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
namespace meta
{
  // apply a type constuctor TC to the type parameters Args
  template<class TC, class... Args>
    using apply = typename TC::template apply<Args...>;
    
  // tag type
  template <class T>
    struct type {};
}

  template <template <class ...> class TC>
  constexpr auto none();

  template <class TC>
  constexpr auto none();

  // make() overload
  template <template <class ...> class M>
    M<void> make();
  
  template <class TC>
    meta::apply<TC, void> make();
  
  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, class X>
    M<Y> make(X&& x);

    // make overload: requires a type construcor, deduce the underlying type
  template <class TC, class X>
    meta::apply<TC, Y> make(X&& x);
    
  // make overload: requires a type with a specific underlying type, 
  // don't deduce the underlying type from X
  template <class M, class X>
    M make(X&& x);
    
  // make emplace overload: requires a type with a specific underlying type, 
  // don't deduce the underlying type from Args
  template <class M, class ...Args>
    M make(Args&& ...args);

  // default customization point for TC<void> default constructor
  template <class M>
    M make(meta::type<M>);

  // default customization point for constructor from X
  template <class M, class X>
    M make(meta::type<M>, X&& x);

  // default customization point for in_place constructor
  template <class M, class ...Args>
    M make(meta::type<M>, in_place_t, Args&& ...args);
  
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

*Effects:* Forwards to the customization point `make` with a template constructor 
`type<M<void>>`. As if

```c++
    return make(meta::type<M<void>>{});
```

### template + deduced underlying type

```c++
template <template <class ...> class M, class T>
  M<V> make(T&& x);
```

where `V` is determined as follows: Let `U` be `decay_t<T>`. Then `V` is `X&`
if `U` equals `reference_wrapper<X>`, otherwise `V` is `U`.

*Effects:* Forwards to the customization point `make` with a template constructor 
`meta::type<M<V>>`. As if

```c++
    return make(meta::type<M<V>>{}, std::forward<T>(x));
```

### type constructor + deduced underlying type

```c++
  template <class TC, class T>
    meta::apply<TC, V> make(T&& x);
```

where `V` is determined as follows: Let `U` be `decay_t<T>`. Then `V` is `X&`
if `U` equals `reference_wrapper<X>`, otherwise `V` is `U`.

*Requires:* `TC` is a type constructor.

*Effects:* Forwards to the customization point `make` with a template constructor 
`meta::type<meta::apply<TC, V>>`. As if

```c++
    return make(meta::type<meta::apply<TC, V>>{}, std::forward<T>(x));
```

### type + non deduced underlying type

```c++
template <class M, class X>
  M make(X&& x);
```

*Requires:* `M` is not a type constructor and the underlying type of `M` is 
convertible from `X`.

*Effects:* Forwards to the customization point `make` with a template constructor 
`meta::type<M>`. As if

```c++
    return meta::make(meta::type<M>{}, std::forward<X>(x));
```

### type + emplace args 

```c++
template <class M, class ...Args>
  M make(Args&& ...args);
```

*Effects:* Forwards to the customization point `make` with a type constructor `meta::type<M>` 
and `in_place_t`. As if

```c++
    return make(meta::type<M>{}, in_place, std::forward<Args>(args)...);
```

### Template function `make` - default constructor customization point for void

```c++
  template <class M>
  M make(meta::type<M>)
```

*Returns:* A `M` constructed using the constructor `M()`

*Throws:* Any exception thrown by the constructor.

### copy constructor customization point

```c++
template <class M, class X>
  M make(meta::type<M>, X&& x);
```

*Returns:* A `M` constructed using the constructor `M(std::forward<X>(x))`

*Throws:* Any exception thrown by the constructor.

### `in_place_t` - emplace customization point

```c++
make <class M, class ...Args>
  M make(meta::type<M>, in_place_t, Args&& ...args);
```

*Returns:* A `M` constructed using the in place constructor 
`M(in_place, std::forward<Args>(args)...)`

*Throws:* Any exception thrown by the constructor.

# Example of customizations

Next follows some examples of customizations that could be included in the standard

## `optional`

```c++
namespace std {
namespace experimental {

  // Holder specialization
  template <>
  struct optional<_t>;

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
}
}
```

## `future`/`shared_future`

```c++
namespace std {

  // customization point for template 
  // (needed because std::experimental::future doesn't has a default constructor)
  future<void> make(experimental::meta::type<future<void>>);

  // customization point for template
  // (needed because std::experimental::future doesn't has a conversion constructor)
  template <class DX, class X>
    future<DX> make(experimental::meta::type<future<DX>>, X&& x);

  // customization point for template 
  // (needed because std::experimental::future doesn't uses experimental::in_place_t)
  template <class X, class ...Args>
    future<X> make(experimental::meta::type<future<X>>, experimental::in_place_t, Args&& ...args);


  // customization point for template 
  // (needed because std::experimental::shared_future doesn't has a default constructor)
  shared_future<void> make(experimental::meta::type<shared_future<void>>);

  // customization point for template 
  // (needed because std::experimental::shared_future<X> doesn't has a constructor from X)
  template <class DX, class X>
    shared_future<DX> make(experimental::meta::type<shared_future<DX>>, X&& x);
  
  // customization point for template 
  // (needed because std::experimental::shared_future doesn't use experimental::in_place_t)
  template <class X, class ...Args>
    shared_future<X> make(experimental::meta::type<shared_future<X>>, experimental::in_place_t, Args&& ...args);
    
  // Holder specializations
  template <>
    struct future<experimental::_t>;
  template <>
    struct future<experimental::_t&>;
  template <>
    struct shared_future<experimental::_t>;
  template <>
    struct shared_future<experimental::_t&>;   
}
```

## `unique_ptr`


```c++
namespace std {

  // customization point for template 
  // (needed because std::unique_ptr doesn't has a conversion constructor)
  template <class DX, class X>
    unique_ptr<DX> make(experimental::meta::type<unique_ptr<DX>>, X&& x);

  // customization point for template 
  // (needed because std::unique_ptr doesn't uses experimental::in_place_t)
  template <class X, class ...Args>
  unique_ptr<X> make(experimental::meta::type<unique_ptr<X>>, experimental::in_place_t, Args&& ...args);


  // Holder customization
  template <class D>
  struct unique_ptr<experimental::_t, D>;

  template <>
  struct default_delete<experimental::_t>;

}
```

## `shared_ptr`

```c++
namespace std {

  // customization point for template 
  // (needed because std::shared_ptr doesn't has a conversion constructor)
  template <class DX, class X>
  shared_ptr<DX> make(experimental::meta::type<shared_ptr<DX>>, X&& x);

  // customization point for template 
  // (needed because std::shared_ptr doesn't uses experimental::in_place_t)
  template <class X, class ...Args>
  shared_ptr<X> make(experimental::meta::type<shared_ptr<X>>, experimental::in_place_t, Args&& ...args);

  // Holder customization
  template <>
  struct shared_ptr<experimental::_t>;
  
}
```

## Implementation

There is an implementation at https://github.com/viboes/std-make.

# Acknowledgements 

Many thanks to Agustín K-ballo Bergé from which I learn the trick to implement the different overloads. 
Scott Pager helped me to identify a minimal proposal, making optional 
the helper classes and of course the addition high order functional factory and 
the missing reference_wrapper<T> overload.

# References

## Hana

## Pure

# Appendix - Non Mandatory Helper Classes

In the original proposal there were some helper classes as 
`lift`, `reverse_lift`, `_t` and `id`  that are not mandatory for this proposal. 
If the committee has interest, a specific proposal can be written. 

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
  // type holder
  struct _t {};
  
namespace meta
{
  // identity meta-function
  template<class T>
    struct id
    {
      using type = T;
    };

  // lift a template class to a type constructor
  template <template <class ...> class TC, class... Args>
    struct lift;

  // reverse lift a template class to a type constructor
  template <template <class ...> class TC, class... Args>
    struct reverse_lift;

  template <class M, class ...U>
  struct rebind : id<typename M::template rebind<U...>> {};

  template <template<class ...> class TC, class ...Ts, class ...Us>
  struct rebind<TC<Ts...>, Us...> : id<TC<Us...>> {};

  template <class M, class ...Us>
  using rebind_t = typename rebind<M, Us...>::type;

}}}}
```

# History

## v0.1 Creation

## v0.2 Take in account comments from the ML

 * Moved `apply` and `type` to `meta` namespace.
 * Added `constexpr`.
 * Added product type factory overload `make` to support `pair/tuple` types.
 * Fix the signature of `make` to support `reference_wrapper` types.
 * Added factory function object `maker`.
 * Added `none` factory.
 * Removed the emplace `make` factory specialization.
 * Remove `type_constructor` as out of the scope of the proposal. It was used by `unique_ptr<_t, D>` specialization, but this can be seen as an implementation detail.
 * Remove `type_constructor_tag` as this was an implementation detail.
 * Refactored `rebind`.
 * Moved `rebind`, `lift`, `reverse_lift`, `_t` and `id` to appendix Non Mandatory Helper Classes and to to `meta` namespace. 
