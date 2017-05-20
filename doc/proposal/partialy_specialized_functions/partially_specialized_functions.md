<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXXR0</span></td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2016-03-13</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">ISO/IEC JTC1 SC22 WG21 Programming Language C++</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Audience:</td>
        <td width="435">Evolution Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escriba &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@wanadoo.fr</a>&gt;</td>
    </tr>
</table>

# Partially specialized template functions and aliases
======================================================

**Abstract**

This paper proposes to partially specialize template functions on the non-deduced types and template alias variables.

# Table of Contents

1. [Introduction](#introduction)
2. [Motivation and Scope](#motivation-and-scope)
3. [Proposal](#proposal)
4. [Design Rationale](#design-rationale)
5. [Proposed Wording](#proposed-wording)
6. [Implementability](#implementability)
7. [Open points](#open-points)
8. [Acknowledgements](#acknowledgements)
9. [References](#references)

# Introduction

This paper proposes to partially specialize template functions on the non-deduced template types or values.

# Motivation and Scope

Currently (C++14) we cannot partially specialize a function, we overload it. However a template function can have additional parameters that are not deduced from the function arguments and we cannot overload on these parameters as they cannot be deduced.

In the following example T is deduced, but TC not.

```c++
template <template <class> class TC, class T>
TC<T> make(T);
```

intended usage

```c++
auto x = make<optional>(1);
```

In order to obtain something similar without changes to C++14 there are two alternatives: 

* add a parameter that coveys the non-deduced types so that we can overload on them,  

```c++
template <template <class> class TC, class T>
TC<T> make(type<TC>, T);
template <template <class> class TC, class T>
TC<T> make(T v) { return make(type<TC>{}, v); }
```

customization

```c++
template <class T>
optional<T> make(type<optional>, T);
```

usage unchanged.


* use a helper trait or function object class that can be partially specialized.

```c++
template <template <class> class TC>
struct make_trait;
template <template <class> class TC, class T>
TC<T> make(T v) { return make_trait<TC>::apply(v); }
```

customization

```c++
template <>
struct make_trait<optional>
{
    template <class T>
    static optional<T> apply(T);
}
```

usage unchanged


The author is not a compiler guy, but wonder if partially specializing the non-deduced template parameters of a function is something that could fill in the current C++ language philosophy. 

Let me show how some generic factories could be defined using this new feature (design based on [factories]).

## Factories using this feature

Next follows an overload of the `make` function for template classes

```c++
template <template <class ...> class TC, class T>
auto make(T v) { return TC<T>(v); }
```

While the type `T` is deduced, the type `TC` is not, as it doesn't appears in the function parameters. We pass the parameter T by value for simplification purposes.

We can add another overload for type constructors

```c++
template <class TC, class T>
auto make(T v) { return apply<TC,T>(v); }
```

`TC` is not deduced here neither.

What we want is to partially specialize the non-deduced parameter `TC`.

If we adopted partial specialization for template functions for these non-deduced type, we can refine the overload of the template class for the class template future as follows

```c++
template <class T>
auto make<future>(T v) { return make_future_ready(v); }
```

We have used here a variation of the syntax used for class template partial specialization. 

Other possibilities would be to use auto to mean this parameter is deduced

```c++
template <class T>
auto make<future, auto>(T v) { return make_future_ready(v); }
```

An alternative syntax could be to make direct use of the argument on the template parameter list

```c++
template <future, class T>
auto make(T v) { return make_future_ready(v); }
```

The same applies for `unique_ptr` and `shared_ptr`

```c++
template <class T>
auto make<unique_ptr>(T v) { return make_unique(v); }
template <class T>
auto make<shared_ptr>(T v){ return make_shared<T>(v); }
```

and last for `expected<_, E>` type constructor

```c++
template <class E, class T>
auto make<expected<_t,E>>(T v){ return make_expected<E>(v); }
```c++

Possible usage

```c++
make<optional>(1);
make<future>(1)
make<expected>(1)
make<expected<_t,error_code>>(1)
make<unique_ptr>(1)
make<shared_ptr>(1)
```

Another useful factory is the null value for *Nullable* types. We can define it as a template function or as a template variable. As a template function, it follows the same design than the preceding `make` function.

```c++
// Not defined, used only for partial specialization purposes
template <class TC>
constexpr none_t none(); 
template <template <class...> class TC>
constexpr none_t none() { 
	return none<type_constructor_from_template_t<TC>>>(); 
}

// partial specialization definitions
template <class T, class D>
constexpr nullptr_t none<unique_ptr<T, D>>() { return nullptr; }
template <class T>
constexpr nullptr_t none<shared_ptr<T>>() { return nullptr; }
template <class T>
constexpr nullptr_t none<weak_ptr<T>>() { return nullptr; }
template <class T>
constexpr nullopt_t none<optional<T>>() { return nullopt; }
template <class ...Ts>
constexpr nullvar_t none<variant<nullvar_t, Ts...>>() { return nullvar; }
```

As a template alias variable, we need partial specialization.

```c++
// Not defined, used only for partial specialization purposes
template <class TC>
constexpr none_t const none; 
template <template <class> class TC>
constexpr none_t const none = none<type_constructor_from_template_t<TC>>>;
```
with the following partial specializations

```c++
template <class T>
constexpr nullptr_t none<unique_ptr<T, D>> = nullptr;
template <class T>
constexpr nullptr_t none<shared_ptr<T>> = nullptr;
template <class T>
constexpr nullptr_t none<weak_ptr<T>> = nullptr;
template <class T>
constexpr nullopt_t none<optional<T>> = nullopt;
template <class T>
constexpr nullvar_t none<nullable_variant<Ts...>> = nullvar;
template <>
constexpr nullvar_t none<variant<nullvar_t>> = nullvar;
```

## Forward to function with an additional parameter

A possible alternative using current C++14: Forward to a function that has an additional parameter that conveys the non deduced types that needs to be partially specialized.

We need first a trampoline that would introduce the non-deduced type as function argument

```c++
template <template <class ...> class TC, class T>
auto make(T v) { return make(template_<TC>{}, <T>(v); }
```

Now we need to define the default customization point

```c++
template <template <class ...> class TC, class T>
auto make(template_<TC>,T v) { return TC<T>(v); }
```

Another trampoline for type constructors (classes for which apply<TC,T> is a new type)

```c++
template <class TC, class T>
auto make(T v) { return make(id<TC>{}, v); }
```

and the corresponding default customization point


```c++
template <class TC, class T>
auto make(id<TC>, T v) { return apply<TC,T>(v); }
```

Once we have the default behavior we can overload for `future`

```c++
template <class T>
future<T> make(template_<future>, T v)
{
    return make_future_ready(v);
}
```

and for `expected`

```c++
template <class E, class T>
expected<T,E> make(id<expected<_,E>>, T v)
{
    return make_expected<E>(v);
}
```

Same usage.

## Factories using a helper function object

Another alternative using a trait or function object class that can be partially specialized having the default behavior for type constructors

```c++
template <class TC>
struct maker {
  template <class T>
  apply<TC,T> operator()(T v)
  {
    return apply<TC,T>(v);
  }
};
```

Partially specialization for templates using template_


```c++
template <template <class ...> class TC>
struct maker<template_<TC>> {
  template <class T>
  TC<T> operator()(T v)
  {
    return TC<T>(v);
  }
};
```

Specialization for future

```c++
template <>
struct maker<template_c<future>> {
  template <class T>
  future<T> operator()(T v)
  {
    return make_future_ready(v);
  }
};
```

However the usage changes, as now we need to instantiated the function object and give a type constructor.

```c++
maker<future<_t>>{}(1);
```

This is not very friendly. In order to preserve the same syntax we need yet to add the two overloads that forward the call to the function object

```c++
template <class TC, class T>
TC<T> make(T v)
{
    return maker<TC>{}(v);
}
template <template <class ...> class TC, class T>
TC<T> make(T v)
{
    return maker<template_<TC>>{}(v);
}
```

An alternative here is to define a template alias variable that provides a function object having a type constructor

```c++
template <class T>
constexpr T __static_const {}; //See Eric customization paper

template <class TC>
constexpr auto const &make = __static_const<maker<TC>>;
```

However the usage would be less friendly for template classes

```c++
make<template_<optional>>(1);
```

Permitting template aliases variable to be specialized would resolve the issue. The following would add a specialization for template classes

```c++
template <template <class ...> class TC>
constexpr auto &make = __static_const<maker<template_c<TC>>>;
```

The advantage of the last version is that we define at a high order function object.

```c++
make<optional>(1)
```


# Proposal

We propose then to allow partial specialization of function templates for the non-deduced types.

We could as well consider template aliases variables specialization. 


# Design rationale

 
# Proposed wordingTo be completed if there is at all an interest in this proposal.
 
# Implementability



# Open points
 
The authors would like to have an answer to the following points if there is at all an interest in this proposal:

 
# Acknowledgements


# References

