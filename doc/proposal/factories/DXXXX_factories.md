<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXX</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2016-05-11</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">ISO/IEC JTC1 SC22 WG21 Programming Language C++</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Audience:</td>
        <td width="435">Library Evolution Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escrib&aacute; &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@nokia.com</a>&gt;</td>
    </tr>
</table>

C++ generic factories 
=====================

**---------------------- DRAFT ----------------------**


**Abstract**

This paper presents a proposal for a generic factory `make<TC>(args...)` that allows to make generic algorithms that need to create an instance of a wrapped class `TC` from the underlying types.

[P0091R0] extends template parameter deduction for functions to constructors of template classes. With this feature, it would seam clear that this proposal lost most of its added value but this is not the case.

# Table of Contents

1. [Introduction](#introduction)
2. [Motivation and scope](#motivation-and-scope)
3. [Proposal](#proposal)
4. [Design rationale](#design-rationale)
5. [Proposed wording](#proposed-wording)
6. [Implementability](#implementability)
7. [Open points](#open-points)
8. [Acknowledgements](#acknowledgements)
9. [References](#references)

# Introduction

This paper presents a proposal for a family of generic factories `make<TC>(args...)` that create an instance of a wrapping class from a *type constructor* and his underlying types as well as emplace factories `make<T>(args...)` that creates an instance of a wrapping class by emplace constructing the underlying type from the provided arguments. 

[P0091R0] extends template parameter deduction for functions to constructors of template classes. With this feature, it would seam that this proposal has lost most of its added value but this is not the case, as [P0091R0] is a feature for the user that cannot be used in generic code.

# Motivation and scope

## Possible valued types

All these types, `shared_ptr<T>`, `unique_ptr<T,D>`, `optional<T>`, `expected<T,E>` (see [P0323R0]) and `future<T>`, have in common that all of them have an underlying type `T`.

There are two kind of factories: 

* type constructor with the underlying types as parameter

 * ```back_inserter```
 * ```make_optional```
 * ```make_ready_future```
 * ```make_expected```

* emplace construction of the underlying type given the constructor parameters

 * ```make_shared``` 
 * ```make_unique```

When writing an application, the user knows if the function to write should return a specific type, as `shared_ptr<T>`, `unique_ptr<T,D>`, `optional<T>`, `expected<T,E>` or `future<T>`. E.g. when the user knows that the function must return an owned smart pointer it would use `unique_ptr<T>`.

```c++
template <class T>
unique_ptr<T> f() {
    T a,
    ...
    return make_unique(a);
    //return unique_ptr(a); // this should be correct with [P0091R0]
}
```

If the user knows that the function must return a shared pointer

```c++
template <class T>
shared_ptr<T> f() {
    T a,
    ...
    return make_shared(a);
    //return shared_ptr(a); // this should be correct with [P0091R0]
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
    //return TC(a); // This should not work even with [P0091R0] 
}
```

Another generic example: *PossiblyValued* types define 

```c++
template <class Callable, class PossiblyValued>
// requires PossiblyValued<FT> and TypeConstructible<FT>
auto functor_map f(Callable&& c, PossiblyValued && f) 
    -> rebind_t<PossiblyValued, decltype(c(declval<value_type_t<PossiblyValued>>()))>
{
    using namespace possibly_valued;
    using namespace type_constructible;
    
    if (has_value(forward<PossiblyValued>(pv)))
        // This should not work even with [P0091R0] 
        return make<type_constructor_t<PossiblyValued>>(c(value(forward<PossiblyValued>(pv)))); 
    else 
        return none<type_constructor_t<PossiblyValued>>(); 
}
```

## *Product types*

In addition, we have factories for the *product types* such as `pair` and `tuple`

 * ```make_pair```
 * ```make_tuple```

## Comparaison

<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td align="left" valign="top"> WITHOUT proposal </td>
        <td align="left" valign="top"> WITH proposal </td>
    </tr>
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>
int v=0;
auto x1  = make_shared&lt;int>(v);
auto x2  = make_unique&lt;int>(v);
auto x3  = make_optional(v);
auto x4v = make_ready_future();
auto x4  = make_ready_future(v);
auto x5v = make_ready_future().share();
auto x5  = make_ready_future(v).share();
auto x6v = make_expected();
auto x6  = make_expected(v);
auto x7  = make_pair(v, v);
auto x8  = make_tuple(v, v, 1u);

future&lt;int&> x4r = make_ready_future(std::ref(v));

auto x1  = make_shared&lt;A>(v, v);
auto x2  = make_unique&lt;A>(v, v);
auto x3  = make_optional&lt;A>(v,v);
auto x4  = make_ready_future&lt;A>(v,v);
auto x5  = make<shared_future<A>>(v, v);
auto x6  = make_expected&lt;A>(v, v);

        </b></td>
        <td align="left" valign="top"><pre class='brush: cpp'>
int v=0;
auto x1  = make&lt;shared_ptr>(v);
auto x2  = make&lt;unique_ptr>(v);
auto x3  = make&lt;optional>(v);
auto x4v = make&lt;future>();
auto x4  = make&lt;future>(v);
auto x5v = make&lt;shared_future>();
auto x5  = make&lt;shared_future>(v);
auto x6v = make&lt;expected>();
auto x6  = make&lt;expected>(v);
auto x7  = make&lt;pair>(v, v);
auto x8  = make&lt;tuple>(v, v, 1u);

future&lt;int&> x4r = make&lt;future>(std::ref(v));

auto x1  = make&lt;shared_ptr&lt;A>>(v, v);
auto x2  = make&lt;unique_ptr&lt;A>>(v, v);
auto x3  = make&lt;optional&lt;A>>(v,v);
auto x4  = make&lt;future&lt;A>>(v,v);
auto x5  = make&lt;shared_future&lt;A>>(v, v);
auto x6  = make&lt;expected&lt;A>>(v, v);

        </b></td>
    </tr>   

</table>

We can use the class template name as a type constructor

```c++
vector<int> vi1 = { 0, 1, 1, 2, 3, 5, 8 };
vector<int> vi2;
copy_n(vi1, 3, maker<back_insert_iterator>(vi2));

int v=0;
auto x1 = make<shared_ptr>(v);
auto x2 = make<unique_ptr>(v);
auto x3 = make<optional>(v);
auto x4v = make<future>();
auto x4 = make<future>(v);
auto x5v = make<shared_future>();
auto x5 = make<shared_future>(v);
auto x6v = make<expected>();
auto x6 = make<expected>(v);
auto x7 = make<pair>(v, v);
auto x8 = make<tuple>(v, v, 1u);
```

or making use of `reference_wrapper` type deduction

```c++
int v=0;
future<int&> x4 = make<future>(std::ref(v));
```

or use the class name to build to support in place construction

```c++
auto x1 = make<shared_ptr<A>>(v, v);
auto x2 = make<unique_ptr<A>>(v, v);
auto x3 = make<optional<A>>(v,v);
auto x4 = make<future<A>>(v,v);
auto x5 = make<shared_future<A>>(v, v);
auto x6 = make<expected<A>>(v, v);
```

Note, with [P0091R0], the following is already possible

```c++
int v=0;
auto x3 = optional(v);
auto x7 = pair(v, v);
auto x8 = tuple(v, v, 1u);
```

We can also make use of the class name to avoid the type deduction

```c++
int i;
auto x1 = make<future<long>>(i);
```

Sometimes the user wants that the underlying type be deduced from the parameter, but the type constructor needs more information. A type holder ```_t```can be used to mean any type `T`.

```c++
auto x2 = make<expected<_t, E>>(v);
auto x2 = make<unique_ptr<_t, MyDeleter>>(v);
```


# Proposal

## Type constructor factory


```c++
template <class TC>
  invoke<TC, int> safe_divide(int i, int j)
{
  if (j == 0)
    return {};
  else
    return make<TC>(i / j);
}
```

We can use this function with different type constructor as

```c++
auto x = safe_divide<optional<_t>>(1, 0);
```
  
## Emplace factory

**TBC**

## How to define a class that wouldn't need customization? 

For the `make` default constructor function, the class needs at least to have a 
default constructor

```c++
C();
```

For the `make` copy/move constructor function, the class needs at least to have a constructor from the underlying types.

```c++
C(Xs&&...);
```

## How to customize an existing class

When the existing class doesn't provide the needed constructor as e.g. `future<T>`, the user needs to add the missing overloads for `make_custom` so that they can be found by ADL.

```c++
namespace boost {
  future<void> make_custom(meta::id<future<void>>) 
  { 
    return make_ready_future(); 
  }
  template <class T, class ...Args>
  future<T> make_custom(meta::id<future<T>>, Args&& ...args)
  { 
    return make_ready_future<T>(forward<Args>(args)...); 
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
    using invoke = future<T>;
  };
}
```

When the class has two parameters and the underlying type is the first template parameter, as it is the case for `expected`, 

```c++
namespace boost
{
  template <class E>
  struct expected_tc<E> {
    template <class T>
    using invoke = expected<T, E>;
  };
}
```

If the second template depends on the first one as it is the case of `unique_ptr<T, D>`, the rebinding of the second parameter must be done explicitly. 

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
    using invoke = default_delete<T>;
  };
    
  template <class D>
    struct unique_ptr<experimental::_t, D>
  {
    template<class T>
    using invoke = unique_ptr<T, detail::rebind_t<D, T>>;
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
  // eval a type-trait expression
  template <class T>
    using eval = typename T::type;
    
  // invoke a type constructor TC to the type parameters Xs
  template<class TC, class... Xs>
    using invoke = typename TC::template invoke<Xs...>;
    
    
  // identity trait
  template <class T>
    struct id
    {
      using type = T;
    };
  template <class T>
    using id_t = eval<id, T>;
    
  // constant meta-function that returns always its argument T
  template <class T>
    struct always
    {
        template <class...>
        using invoke = T;
    };

  // lift a class template to a type constructor
  template <template <class ...> class TC>
    struct quote
    {
        template <class... Xs>
        using invoke = TC<Xs...>;
    };

  // lift a class template to a type constructor binding the arguments first
  template <template <class ...> class TC, class... Args>
    struct bind_front
    {
        template <class... Xs>
        using invoke = TC<Args..., Xs...>;
    };

  // reverse lift a class template to a type constructor binding the arguments last
  template <template <class ...> class TC, class... Args>
    struct bind_back
    {
        template <class... Xs>
        using invoke = TC<Xs..., Argss...>;
    };
    
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
  template <> struct future<_t> : std::experimental::meta::quote<future> {};
}
```

```c++
namespace boost
{
  template <class E> struct expected<_t, E> : std::experimental::meta::bind_back<expected, E> {};
}
```

```c++
namespace boost
{
  
  template <>
    struct default_delete<_t> : std::experimental::meta::quote<default_delete> {};
    
  template <class D>
    struct unique_ptr<_t, D>
  {
    template<class T>
    using invoke = unique_ptr<T, std::experimental::meta::rebind_t<D, T>>;
  };
}
```

# Design rationale

## Customization point

This proposal takes advantage of overloading the `make_custom` functions adding the tag `id<T>`.

We have named the customization point `make_custom` to make more evident that these are customization point.

Alternatively, we could use a trait


```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
template <class T>
struct make_traits 
{
    template <class ...Xs>
    constexpr auto make(Xs&& xs) 
    {
        return T{forward<Xs>(xs)...};
    }
    
};
}}}
```


```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <>
struct make_traits<future<void>> 
{
    constexpr future<void> make()
    { 
        return make_ready_future(); 
    }
};

template <class T>
struct make_traits<future<T>> 
{
    template <class ...Xs>
    future<T> make(Xs&& ...xs)
    { 
        return make_ready_future<T>(forward<Xs>(xs)...); 
    }   
};

}}}
```

## Why a default customization point?

The first `make` factory  uses default constructor to build a `C<void>`. 

The second `make` factory uses conversion constructor from the underlying type(s). 

The third `make` factory is used to be able to do emplace construction given the specific 
type.

## `reference_wrapper<T>` overload to deduce `T&`

As it is the case for `make_pair` when the parameter is `reference_wrapper<T>`, 
the type deduced for the underlying type is `T&`.

## Product types factories

This proposal takes into account also *product type* factories (as `std::pair` or `std::tuple`). 

```c++
  // make product factory overload: Deduce the resulting `Us` 
  template <template <class...> class TC, class ...Xs>
    TC<decay_unwrap_t<Xs>...> make(Xs&& ...xs);
  // make product factory overload: Deduce the resulting `Us` 
  template <class TC, class ...Xs>
    invoke<TC, decay_unwrap_t<Xs>...> make(Xs&& ...xs);
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

where

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

The main problem defining function objects is that we cannot have the same class with different template parameters. The previous `maker` class template has a template class parameter. We need an additional class that takes a type constructor or a type.

```c++
  template <template <class> class Tmpl>
  struct maker_tmpl {
    template <typename ...X>
    constexpr auto
    operator()(X&& ...x) const
    {
        return make<Tmpl>(forward<X>(x)...);
    }     
  };

  template <class TC>
  // requires  TC is a type constructor
  struct maker_tc {
    template <typename ...Args>
    constexpr auto
    operator()(Args&& ...args) const
    {
        return make<TC>(forward<Args>(args)...);
    }     
  };  
 
  template <class T>
  // requires  T is not a type constructor
  struct maker_t
  {
    template <class ...Args>
    constexpr auto 
    operator()(Args&& ...args) const 
    {
      return make<T>(std::forward<Args>(args)...);
    }
  };
```

Now we can define a `maker` factory for high-order `make` functions as follows

```c++
template <class T>
// requires is_type_constructor<T>()==false
maker_t<T> maker() { return maker_t<T>{}; }

template <class TC>
// requires is_type_constructor<TC>()==false
maker_tc<TC> maker() { return maker_tc<TC>{}; }

template <template <class ...> class TC>
maker_tmpl<TC> maker() { return maker_tmpl<TC>{}; }

```

The previous example would be instead

```c++
std::vector<X> xs;
std::vector<Something<X>> ys;
std::transform(xs.begin(), xs.end(), std::back_inserter(ys), maker<Something>());
```

Note the use of `()` instead of `{}`

# Impact on the standard

These changes are entirely based on library extensions and do not require any language features beyond what is available in C++14. 


# Proposed wording

The proposed changes are expressed as edits to [N4564] the Working Draft - C++ Extensions for Library Fundamentals V2.

The current wording make use of `decay_unwrap_t` as proposed in [P0318R0], but if this is not accepted the wording can be changed without too much troubles.


## General utilities library

***-------------------------------------------------------***
Insert a new section.
***-------------------------------------------------------***

**X.Y Factories [functional.factorires]**

**X.Y.1 In General**

**X.Y.2 Header <experimental/factories> synopsis**

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace meta
{
  // invoke a type constructor TC to the type parameters Xs
  template<class TC, class... Xs>
    using invoke = typename TC::template invoke<Xs...>;
    
  // identity meta-function
  template <class T>
    struct id { using type = T; };
}

  // make() overload
  template <template <class ...> class M>
    M<void> make();
  
  // requires a type constructor
  template <class TC>
    meta::invoke<TC, void> make();
  
  // make overload: requires a template class parameter, deduce the underlying type
  template <template <class ...> class TC, class ...Xs>
    TC<decay_unwrap<Xs>...> make(Xs&& ...xs);

  // make overload: requires a type constructor, deduce the underlying types
  template <class TC, class ...Xs>
    meta::invoke<TC, decay_unwrap<Xs>...> make(Xs&& ...xs);
    
  // make overload: don't deduce the underlying types, 
  // don't deduce the underlying type from Xs
  // requires M is not a type constructor

  template <class M, class ...Xs>
    M make(Xs&& ...xs);
    
    
  template <class TC>
  struct maker_tc;
  
  template <template <class> class T>
  struct maker_tmpl;  
 
  template <class T>
  struct maker_t;
  
  // requires a type constructor
  template <class TC>
    maker_tc<TC> maker();
    
  // requires T is not a type constructor
  template <class T>
    maker_t<T> maker();

  template <template <class ...> class TC>
    maker_tmpl<TC> maker();

    

namespace meta
{
  // default customization point for TC<void> default constructor
  template <class M>
    M make_custom(meta::id<M>);

  // default customization point for constructor from Xs
  template <class M, class ...Xs>
    M make_custom(meta::id<M>, Xs&& xs);
}  
}
}
}

```


**X.Y.3 Template function `make`**

**X.Y.4 template + void**

###########################################################################
```c++
  template <template <class ...> class M>
  M<void> make();
```

*Effects:* Forwards to the customization point `make` with a template constructor 
`id<M<void>>`. As if

###########################################################################
```c++
    return make_custom(meta::id<M<void>>{});
```

**X.Y.5 template + deduced underlying type**

###########################################################################
```c++
template <template <class ...> class M, class ...Xs>
  M<decay_unwrap<Xs>...> make(Xs&& ...xs);
```

*Effects:* Forwards to the customization point `make_custom` with a template constructor 
`meta::id<M<decay_unwrap<Xs>...>>`. As if

```c++
    return make_custom(meta::id<M<decay_unwrap<Xs>...>>{}, std::forward<T>(x));
```

**X.Y.6 type constructor + deduced underlying type**

###########################################################################
```c++
  template <class TC, class ...Xs>
    meta::invoke<TC, decay_unwrap<Xs>...> make(Xs&& ...xs);
```

*Requires:* `TC` is a type constructor.

*Effects:* Forwards to the customization point `make_custom` with a template constructor 
`meta::id<meta::invoke<TC, decay_unwrap<Xs>>>`. As if

```c++
    return make_custom(meta::id<meta::invoke<TC, decay_unwrap<Xs>>>{}, std::forward<Xs>(xs));
```

**X.Y.7 type + non deduced underlying type**

###########################################################################
```c++
template <class M, class ...Xs>
  M make(Xs&& ...xs);
```

*Requires:* `M` is not a type constructor.

*Effects:* Forwards to the customization point `make_custom` with a template constructor 
`meta::id<M>`. As if

```c++
    return make_custom(meta::id<M>{}, std::forward<Xs>(xs)...);
```

**X.Y.8 constructor customization point**

###########################################################################
```c++
template <class M, class ...Xs>
  M make_custom(meta::id<M>, Xs&& ...xs);
```

*Returns:* A `M` constructed using the constructor `M(std::forward<Xs>(xs)...)`

*Throws:* Any exception thrown by the constructor.

# Example of customizations

Next follows some examples of customizations that could be included in the standard

## `optional`

```c++
namespace std {
namespace experimental {

  // Holder specialization
  template <>
  struct optional<_t>: meta::quote<optional> {};

}
}
```

## `expected`

```c++
namespace std {
namespace experimental {

  // Holder specialization
  template <class E>
  struct expected<_t, E>: meta::bind_back<expected, E> {};
}
}
```

## `future`/`shared_future`

```c++
namespace std {

  // customization point for template 
  future<void> make_custom(experimental::meta::id<future<void>>) {
    return make_ready_future();
  }

  // customization point for template
  template <class DX, class ...Xs>
    future<DX> make_custom(experimental::meta::id<future<DX>>, Xs&& ...xs) {
    return make_ready_future<DX>(forward<Xs>(xs)...);
  }

  // customization point for template 
  shared_future<void> make_custom(experimental::meta::id<shared_future<void>>)  {
    return make_ready_future().share();
  }

  // customization point for template 
  template <class DX, class ...Xs>
    shared_future<DX> make_custom(experimental::meta::id<shared_future<DX>>, Xs&& ...xs)  {
    return make_ready_future<DX>(forward<Xs>(xs)...).share();
  }
     
  // Holder specializations
  template <>
    struct future<experimental::_t> : experimental::meta::quote<future> {};;
  template <>
    struct future<experimental::_t&>;
  template <>
    struct shared_future<experimental::_t> : experimental::meta::quote<shared_future> {};;
  template <>
    struct shared_future<experimental::_t&>;   
}
```

## `unique_ptr`


```c++
namespace std {

  // customization point for template 
  template <class DX, class ...Xs>
    unique_ptr<DX> make_custom(experimental::meta::id<unique_ptr<DX>>, Xs&& ...xs) {
    return make_unique<T>(forward<Xs>(xs)...);
  }

  // Holder customization
  template <class D>
  struct unique_ptr<experimental::_t, D> {
      template <class ...T>
      using invoke = unique_ptr<T..., experimental::meta::eval<experimental::meta::rebind<D, T...>>>;
  };

}
```

## `shared_ptr`

```c++
namespace std {

  // customization point for template 
  template <class DX, class ...Xs>
  shared_ptr<DX> make_custom(experimental::meta::id<shared_ptr<DX>>, Xs&& ...xs) {
    return make_shared<DX>(forward<Xs>(xs)...);
  }

  // Holder customization
  template <>
  struct shared_ptr<experimental::_t> : experimental::meta::quote<shared_ptr> {};
  
}
```


## `pair`


```c++
namespace std {


  // Holder customization
  template <>
  struct pair<experimental::_t, experimental::_t>
  {
    template <class ...Ts>
    using invoke = pair<Ts...>;
  };
  
}
```

## `tuple`

```c++
namespace std {

  // customization point for template 
  template <class DX, class ...Xs>
  shared_ptr<DX> make_custom(experimental::meta::id<shared_ptr<DX>>, Xs&& ...xs) {
    return make_shared<DX>(forward<Xs>(xs)...);
  }

  // Holder customization
  template <>
  struct tuple<experimental::_t> : experimental::meta::quote<tuple>  {  };  
}
```


# Implementability

This proposal can be implemented as pure library extension, without any compiler magic support, in C++14.

There is an implementation at https://github.com/viboes/std-make.

# Open points

The authors would like to have an answer to the following points if there is at all an interest in this proposal:

* Is there an interest on the `make` functions?

* Should the customization be done with overloading or with traits?

    The current proposal uses overloading as customization point. 
    The alternative is to use traits as e.g. the library [Boost.Hana] uses.

    If overloading is preferred, 

    * should the customization function names be suffixed e.g. with  `_custom`?

* Should the namespace `meta` be used for the meta programming utilities `invoke` and `id`?

* Should the high-order function factory `maker` be part of the proposal?

* Should the function factories `make` be function objects?

    [N4381] proposes to use function objects as customized points, so that ADL is not involved.

    This has the advantages to solve the function and the high order function at once.

    The same technique is used a lot in other functional libraries as [Range-V3], [Fit] and [Pure]. 

* Is there an interest on placeholder type `_t`?

    While not need absolutely, it helps to define friendly the type constructors.

* Is there an interest on the helper meta-functions `id`, `quote`, `bind_front`, `bind_back` and `rebind`?

    If yes, should them be part of a separated proposal?

    There is much more on meta-programming utilities as show on the [Meta] library.

* Should the customization of the standard classes `pair`, `tuple`, `optional`, `future`, `unique_ptr`, `shared_ptr` be part of this proposal? Are there others standard types to customize?


# Acknowledgements 

Many thanks to Agustín K-ballo Bergé from which I learn the trick to implement the different overloads. Scott Pager helped me to identify a minimal proposal, making optional the helper classes and of course the addition high order functional factory and the missing reference_wrapper<T> overload.

Thanks to Mike Spertus for its [P0091R0] proposal that would even help to avoid the factories in the common cases. 

# References


[N4381]: http://open-std.org/JTC1/SC22/WG21/docs/papers/2015/n4381.html "Suggested Design for Customization Points"

[N4480]: http://open-std.org/JTC1/SC22/WG21/docs/papers/2015/n4480.html "Programming Languages — C++ Extensions for Library Fundamentals"

[P0091R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0091r0.html "Template parameter deduction for constructors (Rev. 3)"

[P0318R0]: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0318r0.pdf "`decay_unwrap` and `unwrap_reference`"

[P0323R0]: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0323r0.pdf "A proposal to add a utility class to represent expected monad (Revision 2)"

[Range-V3]: https://github.com/ericniebler/range-v3

[Meta]: https://github.com/ericniebler/meta

[Boost.Hana]: https://github.com/ldionne/hana

[Pure]: https://github.com/splinterofchaos/Pure

[Fit]: https://github.com/pfultz2/Fit


* [N4381] - Suggested Design for Customization Points

    http://open-std.org/JTC1/SC22/WG21/docs/papers/2015/n4381.html

* [N4480] - Programming Languages — C++ Extensions for Library Fundamentals

    http://open-std.org/JTC1/SC22/WG21/docs/papers/2015/n4480.html

* [P0091R0] - Template parameter deduction for constructors (Rev. 3)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0091r0.html

* [P0318R0] `decay_unwrap` and `unwrap_reference`
 
    http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0318r0.pdf

* [P0323R0] - A proposal to add a utility class to represent expected monad (Revision 2)

    http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0323r0.pdf

* [Range-V3]

    https://github.com/ericniebler/range-v3

* [Meta]

    https://github.com/ericniebler/meta

* [Boost.Hana]

    https://github.com/ldionne/hana

* [Pure]

    https://github.com/splinterofchaos/Pure

* [Fit]

    https://github.com/pfultz2/Fit



# Appendix - Helper Classes

In the original proposal there were some helper classes as 
`quote`, `bind_front`, `bind_back``_t` and `id`  that are not mandatory for this proposal. 
If the committee has interest, a specific proposal can be written. 

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  // type placeholder
  struct _t {};
  
namespace meta
{

}}}}
```

