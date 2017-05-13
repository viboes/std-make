<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">D0XXXR0</span></td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2016-06-11</td>
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
        <td width="435">Vicente J. Botet Escrib&aacute; &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@nokia.com</a>&gt;</td>
    </tr>
</table>


Traits: An Alternative Design for Customization Points 
==============================================

**Abstract**


# Table of Contents

1. [Introduction](#introduction)
2. [Motivation](#motivation)
3. [Proposal](#proposal)
4. [Design rationale](#design-rationale)
5. [Proposed wording](#proposed-wording)
6. [Implementability](#implementability)
7. [Open points](#open-points)
8. [Acknowledgements](#acknowledgements)
9. [References](#references)


# Introduction

This paper revisits the design of customization points in [N4381] identifying some liabilities and propose an alternative design to solve them.

The proposed design:

* Helps to avoid unpredictable and unintended overloading due to argument-dependent lookup (ADL).
* Provides a clean way for generic libraries to supply points-of-customization  for user-defined types.
* Avoid the problem of unintended "name reservation".
* Force clients to be explicit about which customization point they want to customize.  
* And describes how existing code (and even the standard library) can be migrated to use the new facility. 
* Customize user defined customization points for standard types are possible following the same design.

The design is based on:

* Associate the customization points to a concept.
* Split the customized and the customization point following in some way the Method Pattern.
* Make use of traits as customization points.
* Include the customized points inside an explicit scope (a prefix) associated to the concept.


# Motivation

[N1691] describes some of the problems raised by the current way (C++17) the standard defines customization points using ADL. 

> 1 Invite clients to write a function that implements the procedure in the same namespace as their class:
>
> ...
> 
> Option 1 is the simplest and most natural for both library authors and for clients, but there's a problem: two libraries could advertise identically named points of customization with different semantics. Under option 1, libraries must declare explicitly which names are being used without qualification by which functions; users must take care to avoid defining these names in their own namespaces except where the intention is to customize a given library. If two library implementors happen to choose the same function name as a point-of-customization with different semantics, the result is at best confusing: the user may need to create two overloads of the same name in his own namespace with different semantics. Since the name is the same, it's quite likely that the semantics will be similar, but not identical. In the worst case, the functions have identical signatures, and the libraries simply refuse to interoperate in the user's application. A corollary problem is that when providing a customization with a given name, there's no way for a client to indicate in code which library's meaning of that name she is implementing.
 
The paper continues with the following summary

> The irony here is hard to escape: the sort of name clashes we've seen are exactly what namespaces are supposed to help us avoid. In fact, compared with the situation before namespaces, we seem to have added new opportunities for accidents. In a world of prefixed names, a library writer's own test code would be unlikely to compile correctly if a prefix were omitted, so once the choice to use prefixes is made, the compiler enforces qualification. Today, both library authors and users must exercise extreme vigilance to be sure that, except where ADL is explicitly intended, all calls whose arguments could come from any other namespace are qualified. Since unqualified calls are perfectly legal, programmers get no support from the compiler in this effort. Since they are perfectly easy, and unintended ADL will pass unnoticed through all but the most sadistic tests, there is little incentive other than good conscience to add qualification. These errors are the sort that show up only after libraries are deployed, and when user code is being maintained by someone other than its original author. 

Let me rephrase the previous issue as:

I. Having 3pp libraries that define the same customization point based on ADL is a source of ambiguity if not a source of inter-operation, having unpredictable and unintended overloading due to argument-dependent lookup (ADL) and unintended "name reservation".

There are two additional problems: 

II. Not all the C++ types are found by ADL, in particular the builtin types.

```c++    
    int a, b;
    swap(a,b); // not found
```    

The standard solution consists in overloading the customization point in the `std` namespace and let the user the responsibility to introduce the `std::swap` function via a using namespace declaration 
    

```c++    
    int a, b;
    using std::swap;
    swap(a,b); // not found
```

This design has some flaws that [N4569] has very well described.

III. The user is unable to overload UDT satisfying a specific concept. E.g. we want to be able to define `swap` for tuple-like types. However, overloading doesn't allow us to do that. We need traits for this purpose. 

Note that `std::hash` is a trait customization point, but new C++ standard proposals go in the opposite direction and request alternatively an overloaded `hash_value` function found by ADL. 

[N4569] address II by defining the customized point `std::experimental::ranges::swap` that is a function object taking precedence over functions found by ADL. It manages partially with point I. However it doesn't address III.

To address III, we could always include the tuple-like customization for `std::swap` in the standard, but the idea is to allow users to define their own specializations for non-standard concepts. Maybe the committee doesn't see point III as a problem and prefer to disable the user to do such customizations. However, if the committee believe that users should be able to do this kind of customizations, we need traits and partial specialize them based on concepts.

To address point I, I believe that the customization and the use of the customization points should be explicit, that is, make use of some kind of prefix that states clearly what the user is customizing and which customization point it is using.

[N4381] design has in the author opinion two major liabilities already identified in [N4381]. Adding a function objet in the namespace `std2` will disable the addition of ADL overloads to customize classes in `std2`.  The proposed solution is to use friend functions. While this is a solution for classes it doesn't work for enums and [N4381] proposes a workaround, move the enum to a nested namespace add the overload there. Then import the defined enum in `std2`.

There is an additional issue that concerns the standard indirectly. 

IV. The user cannot define customization points following [N4381], using only overload and ADL, as the user cannot add any function on the standard namespace. This means that the way the standard do customization by ADL cannot be taken as an example of how customization can be done by the user. This means that the C++ standard approach doesn't scales. This make teachability more difficult. 

# Proposal

The proposed alternative design will take into account the following goals:

* Avoid unpredictable and unintended overloading due to argument-dependent lookup (ADL).
* Avoid the problem of unintended "name reservation".
* Provides a clean way for generic libraries to supply points-of-customization  for user-defined types.
* Force clients to be explicit about which customization point they want to customize.  
* Force clients to be explicit about which customization point they want to use.  
* Allows to customize user defined customization points for standard types.

In summary the proposal consists in:

* Adding an indirection level, using traits as customization points. 
* Making explicit the customization points call

To be able to customize also types satisfying some requirements (e.g. a concept) the trait has an `Enabler` parameter (`std::void_t`).

The technique is well know. Boost.Hana has a similar approach for customization points that has yet an additional level of indirection (Hana uses a tag associated to a class that is used as tag dispatcher).

However Boost.Hana requires that all the customization points have a different name as all the customization points are at the `boost::hana` namespace level.

Enabling customizations for groups of types has also some liabilities, possible ambiguity for types on several groups. These are the same kind of problems as we can find with multiple inheritance and the technique to solve the possible ambiguity is almost the same: be more specific.

## Traits as customization points
 
The following code uses SFINAE (based on Boost.Hana design, `when<Cond>` is used to SFINAE on conditions), but any other way to do partial specialization based on SFINAE will be good.

```c++
namespace std2 {
explicit namespace swappable {

template <class R, class S, class Enabler=void>
struct traits: traits<R, S, when<true>> {};

// Default failing specialization
template <typename R, typename S, bool condition>
struct traits<R, S, when<condition>>
{
    template <class T, class U>
    static constexpr auto swap(const T& x, const U& y) =delete;
};
}
```

Note the use of template parameters on the traits class and for each operation. The first ones are used to specialize the traits. The second ones are used to define the functions.

## Locate the customized functions inside a specific scope

Add a class representing the associated concept (the prefix), with static functions that will forward the call to the traits.

```c++
namespace std2 {
explicit namespace swappable {

template <class T, class U>
static constexpr auto swap(const T& x, const U& y) 
    noexcept(noexcept(traits<T, U>::swap(x,y))) ->
    decltype(traits<T, U>::swap(x,y)) 
{
    return traits<T, U>::swap(x,y);
}

};
}
```

We've chosen an explicit namespace because it is open for additions and the scope is explicit. 

## Syntactic sugar

[SimpleFunctions] proposed a syntactic sugar extension that allows to replace

```c++
template <class T, class U>
static constexpr auto swap(const T& x, const U& y) 
    noexcept(noexcept(traits<T, U>::swap(x,y))) ->
    decltype(traits<T, U>::swap(x,y)) 
{
    return traits<T, U>::swap(x,y);
}
```

by

```c++
template <class T, class U>
static constexpr auto swap(const T& x, const U& y)  =>
    traits<T, U>::swap(x,y));
```

We will use this extension in this paper to make the paper more succinct, avoid errors and remove the noise. 

## Add the concept traits

* Define `is_swappable` using SFINAE on `std2::swappable::swap`

* Define `is_adl_swappable` using SFINAE on `swap`

## Usage

```c++
std2::swappable::swap(a,b)
``` 

or

```c++
using namespace std2;

swappable::swap(a,b)
``` 

or

```c++
auto swap = std2::swappable::swap; // overload set function object

swap(a,b) 
``` 

## Backward compatibility

In order to be backward compatible, specialize the `swappable` traits by default to use overloading, use the default swap algorithm when possible and define the builtin specialization.

```c++
namespace std {

//To avoid the legacy std definitions [P0370r0]
template <class T>
void swap(T&, T&) = delete;
template <class T, size_t N>
void swap(T(&)[N], T(&)[N]) = delete;

}

namespace std2::swappable {
    
//Specialization types found by ADL
template <typename T>
struct traits<T, T, 
    when<   is_adl_swappable<T&, T&>{} >> 
{
    constexpr auto swap(const T& x, const T& y) 
        =
            // Found by ADL as customization is not an explicit namespace
            (void)swap(forward<T>(t), forward<U>(u))        
        ;
};
    
//specialization for types not found by ADL
template <typename T, typename U>
struct traits<T, U,
    when< ! is_adl_swappable<T&, U&>{}() &&
            models::MoveConstructible<T>{}() &&
            models::Assignable<T&, U&&>{}() >> 
{
    static constexpr auto swap(const T& x, const T& y) 
    =
        (void)(b = std::exchange(a, std::move(b)))
    ;
};
    
template <class T, class U, std::size_t N>
struct traits<T(&)[N], U(&)[N], 
    when< ! is_adl_swappable<T(&)[N], U(&)[N]>{} &&
            is_swappable<T, U>{} >> 
{
    constexpr void swap(T (&x)[N] t, U (&y)[N] u) 
        noexcept(noexcept(std::swappable::swap(t[0], u[0])))
    {
        for (std::size_t i = 0; i < N; ++i) {
            std::swappable::swap(t[i], u[i]);
        }        
    }
};
} // std2::swappable

struct swappable {

template <class T, class U>
using traits = swappable::traits<T, U>;

template <class T, class U>
static constexpr auto 
swap(const T& x, const U& y) 
    => traits<T, U>::swap(x,y) ; // Syntactic sugar
}

template <class T, class U, std::size_t N>
static constexpr enable_if< is_swappable<T, U>{} > 
swap(T (&x)[N] t, U (&y)[N] u) 
    noexcept(noexcept(::std::swappable::swap(t[0], u[0])))
{
    for (std::size_t i = 0; i < N; ++i) {
        ::std::swappable::swap(t[i], u[i]);
    }        
}

};

```


Allowing specialization of classes from another namespace ([N1691] - IV) would help also here

```c++
    struct ::std2::swappable::traits<MyClass> {...};
```    

* Last define a `swap` overload set if we consider that `std2` should behave as `std`. However, adding customization points as function objects has some liabilities as described above.

```c++
namespace std2 {
    auto swap = swappable::swap;
} // std2
```

## Do we need a new language struct?

Virtual functions are used as customization points for run-time polymorphism. However, we don't have a language construct for static polymorphism.

Type classes in Haskell are a tool that is close to the problematic we are handing on here.

C++0X Concepts had a concept mapping feature that is close to this customization problem.

# Proposal Design for Customization Points


## Design Goals 

The goals are based on the goals defined in [N4381]. The main differences are:

* The mapping for the future customization points shall be explicit. However in order to be backward compatible with the current customization points, it is done implicitly for them.
* Instead of requesting a customization point to be a function object we require it to be a static function in a struct, which ensure that the customization point lookup will remove non explicit namespaces from the associated namespace set.

The goals of customization point design are as follows (for some hypothetical future customization point `cust`):

* A customization point must be located in an struct `std2::xxx` which is associated to (either because the namespace names the concept).
* Code that calls `cust` either qualified as `std::xxx::cust(a)`; or unqualified as using `auto cust = std::xxx::cust; cust(a);` should behave identically. 
* Code that calls `cust` as using `auto cust = std::xxx::cust; cust(a);` should not bypass any constraints defined on `std::xxx::cust`.
* Calls to the customization point should be optimally efficient by any reasonably modern compiler.
* The solution should not introduce any potential violations of the one-definition rule or excessive executable size bloat.
* Customization is done explicitly. 
* Related customization points are all customized at once.
* Customization for a set of types satisfying a specific constraint is possible.
* customization for associated types and constants is also possible

## Design Details


### Single Customization point

#### Customization interface


```c++
/////////////////////////////
explicit namespace functor {

    template <class R, class Enabler=void>
    struct traits : traits<R, when<true>> {};

    // Default failing specialization
    template <typename R, bool condition>
    struct traits<R, when<condition>> : default_ 
    {
        template <class Callable, class F>
            static constexpr auto transform(F&&, Callable&& ) = delete;
    };
    
}} // functor/customization
```
#### User interface

The customized user interface is located in a specific explicit scope.

```c++
explicit namespace functor {   
   
    template <class Callable, class F>
    constexpr auto transform(F&& f, Callable&& c) =>
        traits<decay_t<F>>::transform(forward<R>(f), forward<Callable>(c));
    }
};
```

There are some notable things about this solution. As promised, the customization point `transform` is located in an explicit scope `struct functor`. 

#### operators

```c++
namespace functor_operators {

}
```

#### Customization of a type

The user must state explicitly that his type customizes the `functor` by specializing the `functor::traits`.

```c++
    template <class R>
    struct std2::functor::traits<std::optional<R>> 
    {
        template <class Callable, class Optional>
            static constexpr auto transform(Optional&& opt, Callable&& c) 
                noexcept(noexcept(c(*opt))) ->
                decltype(c(*opt)) 
        {
            if (opt) return c(*opt);
            return std::nullopt;
        }    
    };
```

Compare this with a customization based on ADL function lookup. Note that  `optional<T>&&` is not a universal reference and so we need the overload for lvalues `optional<T> const&`.

```c++
namespace std {   
    template <class Callable, class T>
    constexpr auto transform(optional<T>&& opt, Callable&& c) 
                noexcept(noexcept(c(*opt))) ->
                decltype(c(*opt)) 
        {
            if (opt) return c(*opt);
            return std::null_opt;
        }    
    template <class Callable, class T>
    constexpr auto transform(optional<T> const& opt, Callable&& c) 
                noexcept(noexcept(c(*opt))) ->
                decltype(c(*opt)) 
        {
            if (opt) return c(*opt);
            return std::null_opt;
        }    
}
```

#### Customization of a concept

We shall be able to also specialize for a specific kind of types, e.g. *Nullable*.

The `Enabler` parameter is used to this end. 

```c++
namespace nullable {
    struct as_functor_traits 
    {
        template <class Callable, class Nullable>
            static constexpr auto transform(Nullable&& n, Callable&& c) 
                noexcept(noexcept( c(n.value()) )) ->
                decltype( c(n.value()) ) 
        {
            if (nullable::has_value(n)) return c(nullable::value(n));
            return nullable::none<Nullable>();
        }    
    };
}

  template <class N>
  struct functor::traits<N, when< is_nullable_v<N> >>  : 
    nullable_as_functor_traits {};

```

With **Concepts-Lite** the syntax will be different, but we should be able to do the same kind of conditional specialization.

```c++
namespace functor {
    template <Nullable N>
    struct traits<N>  : nullable::as_functor_traits<N>  {};
} // functor/customization
```

Now any *Nullable* type is seen as a *Functor*.

This mapping shall be defined where *Nullable* is defined.

#### Customization of a type using a concept customization

A type `T` can satisfy the constraints of different concepts `C1`and `C2` that have a mapping for *Functor*. This will result in an ambiguity when seen `T` as a *Functor*. The author of the type `T` shall state explicitly the specialization for the class, and choose the best mapping or a specific specialization.

```c++
namespace functor {
    template <class T>
    struct traits<std::optional<T>> : nullable::as_functor_traits<N>  {};
}} // functor/customization
```

### Backward compatible `swap` customization point

This design proposes to locate customization point `swap` in an explicit namespace `std2::swappable`. Below is what `std2::swappable::swap` would look like if it were redesigned.

#### Customization interface

```c++
namespace std2 { 
namespace swappable {

template <class R, class Enabler=void>
struct traits: traits<R, when<true>> {};

// Default failing specialization
template <typename R, bool condition>
struct traits<R, R, when<condition>> : default_ 
{
    template <class T>
    static constexpr auto swap(const T& x, const T& y) =delete;
};
}} // swappable   
}
```


#### User interface

```c++
namespace std2 {
struct swappable {

template <class T, class U>
using traits = swappable::traits<T, U>;

template <class T, class U>
static constexpr auto swap(const T& x, const U& y) =>
    traits<T, U>::swap(x,y);
    
} // swapppable
}
```

#### Specializations

```c++
namespace std2::swappable {

// To avoid the legacy std definitions [P0370r0]
template <class T>
void swap(T&, T&) = delete;
template <class T, size_t N>
void swap(T(&)[N], T(&)[N]) = delete;
    
// specialization types found by ADL
template <typename T>
struct traits<T, T, 
    when<   is_adl_swappable<T&, T&>{} >> 
{
    static constexpr auto swap(const T& x, const T& y) =>
            // Found by ADL as customization is not an explicit namespace
            swap(forward<T>(t), forward<U>(u))        
};
    
// specialization for types not found by ADL
template <typename T>
struct traits<T, T,
    when< ! is_adl_swappable<T&, T&>{} &&
            models::MoveConstructible<T>{} &&
            models::Assignable<T&, T&&>{} >> 
{
    constexpr void swap(const T& x, const T& y) =>
        (void)(b = std::exchange(a, std::move(b)))
    ;
};
    
template <class T, class U, std::size_t N>
struct traits<T(&)[N], U(&)[N], 
    when< ! is_adl_swappable<T(&)[N], U(&)[N]>{} &&
            is_swapable<T, U>{} >> 
{
    constexpr void swap(T (&x)[N], U (&y)[N]) 
        noexcept(noexcept(std::swappable::swap(t[i], u[i])))
    {
        for (std::size_t i = 0; i < N; ++i) {
            std::swappable::swap(t[i], u[i]);
        }        
    }
};
} // std2::swappable   
```

As promised, `swap` is located in an explicit scope `std2::swappable`. `std2::swappable::swap(T,U)` forwards to the `std2::customization::swappable::traits<T,U>::swap(T,U)`.


Also `std2::swappable` contains the `traits` specialization for the familiar `std::swap` free functions cases and in addition for the backward compatible ADL customized case. 

With [P0119R2]/[P0382R0] `std2::swappable::swap` can be used as a function object.

This is not yet backward compatible as users would need to change the call to the customization point, but Range TS suffers from the same issue.

### Backward compatible `begin`/`end` customization points

#### Customization interface

```c++
namespace std2 {
namespace range {   

template <class T, class Enabler=void>
struct traits: traits<T, when<true>> {};

// Default failing specialization
template <typename T, bool condition>
struct traits<T, when<condition>>  
{
    constexpr auto begin(const T& x) =delete;
    constexpr auto end(const T& x) =delete;
};
}} // range
}
```

#### User interface

```c++
namespace std2 {
namespace range {

template <class T>
constexpr auto begin(const T& x)     =>        traits<T>::begin(x)    ;

template <class T>
constexpr auto end(const T& x)     =>        traits<T>::end(x)    ;

} // range
}
```

#### Specializations

```c++
namespace std2 {
namespace range {  

// specialization types found by ADL
template <typename T>
struct traits<T, T, 
    when<   is_adl_range<T>{} >> 
{
        template <class T>
        static constexpr auto begin(T& rng) => begin(forward<T>(rng)) ;
            
        template <class T>
        static constexpr auto end(T& rng) => end(forward<T>(rng)) ;
};
    

// specialization for types not found by ADL
// define begin for arrays
template <class T, size_t N>
struct traits<T (&)[N], 
    when< ! is_adl_range<T>{} >> 
{
    static constexpr T* begin(T (&a)[N]) noexcept {
        return a;
    };
    static constexpr T* end(T (&a)[N]) noexcept {
        return a+N;
    };
};

// Define begin for containers
// (trailing return type needed for SFINAE   
template <class R, 
    when< ! is_adl_range<R>{} &&
            has_begin_end_member<R> >>
struct traits<R, when<has_begin_end_member<R>> 
{
    template <class T>
    static constexpr auto begin(T && rng) => forward<T>(rng).begin() ;
            
    template <class T>
    static constexpr auto end(T && rng) => forward<T>(rng).end() ;
};
} // range
}
```

### backward compatible `hash` customization points

#### Customization interface

```c++
namespace std2 {
explicit namespace hashable {   

template <class T, class Enabler=void>
struct traits: traits<T, when<true>> {};

// Default failing specialization
template <typename T, bool condition>
struct traits<T, when<condition>> : default_ 
{
    constexpr auto hash_value(const T& x) =delete;
};
} // hashable   
}
```

#### User interface

```c++
namespace std2 {
namespace hashable {

template <class T>
constexpr auto hash_value(T&& x) => traits<T>:: hash_value(forward<T>(x)) ;

}; // hashable
}
```

#### Specializations

```c++
namespace std2 {
explicit namespace hashable {  

// specialization types found by ADL
template <typename T>
struct traits<T, T, 
    when<   is_adl_hashable<T>{} >> 
{
    template <class T>
    static constexpr auto hash_value(T&& v) => hash_value(forward<T>(v)) ;
};

// Define hash_value for ranges
template <class R, 
struct traits<R,  when< ! is_adl_hashable<R>{} &&
            is_trivially_copyable<R>{} &&
            is_standard_layout<R>{} &&
            is_contiguous_layout<R>{} >>
{
    template <class T>
    static constexpr auto hash_value(T&& v) noexcept {
        return hash_as_bytes(forward<T>(v));
    };           
};
    
// Define hash_value for ranges
template <class R, 
struct traits<R,  when< ! is_adl_hashable<R>{} &&
            ! (is_trivially_copyable<R>{} &&
            is_standard_layout<R>{} &&
            is_contiguous_layout<R>{}) &&
            is_range<R> >>
{
    template <class Range>
    static constexpr auto hash_value(Range&& rng) noexcept {
        return hash_combine_range(forward<Range>(rng));
    };
            
};
} // hashable
}
```


# Design Rationale

## On the need of a specific scope to locate customization point 

Having a function object for the customization point `cust` in `std2` namespace forbids to add `cust` overloads for the types defined in `std2` needs of some workarounds to overload the customization point.

Moving it to a nested scope avoids all kind of troubles.

Of course the call is more explicit, but in the author opinion it is better to be explicit to avoid possible ambiguities.

## Multiple customization versus simple customization 

Customization of several function at once have several advantages: 

* It allows to reuse all the customization at once.
* Often, the user doesn't need to customize all the functions, as some can in the general case be defined in function of the others. This allows to have some *minimal definition sets* that can be reused.

It has also some liabilities:

* When a customization point must be specialized all the customization points must be specialized.
* The customization point is dependent of the concept, so moving it to another concept has high repercussions on the customizations.


## On the need of function object to represent a customization point

We need some way to ensure that a customization point inhibits ADL. Function object provide that, but explicit scope avoid it as well. 

# Proposed Wording


# Open Points

# Comparison


<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td align="left" valign="top"> WITHOUT proposal </td>
        <td align="left" valign="top"> WITH proposal </td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Usage
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>
std::experimental::ranges::swap(a,b);
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> std2::swappable::swap(a,b);
        </pre></td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Usage
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'> 
using std2::swap;
swap(a,b);
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
auto swap = std2::swappable::swap;
swap(a,b);
        </pre></td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Interface
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

namespace std::experimental{
namespace range {
namespace __swap {
    struct swap_fn {
    
    
        template &lt;class T, class U>
        void operator()(T&, U&) {
            static_assert(false, "");
        }
        //...
    };
}




    auto swap = __swap::__swap_fn;
}}
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
namespace std::experimental {
namespace swappable {
    template &lt;class T, class U, class Enabler = void>
    struct traits: traits&lt;T,U, when&lt;true>> {};
    
    template &lt;class T, class U, bool Cond>
    struct traits&lt;T,U, when&lt;Cond>> {
        static void swap(T&, U&) = delete;
    };
        
    template &lt;class T, class U>
    auto swap(T& t, U& u) 
            BABEL_NOEXCEPT_RETURNS(traits&lt;T,U>::swap(t,u)))            
    };
}
}
        </pre></td>
    </tr>
    
    <tr>
        <td  align="left" valign="top"><b>
Customization by overload in user namespace
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

namespace user {
    class A;
    void swap(A&, A&);
}
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
namespace user {
    class A;
    void swap(A&, A&);
}
        </pre></td>
    </tr>
    
        <tr>
        <td  align="left" valign="top"><b>
Customization by overload in std::experimental::ranges for classes
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

namespace std::experimental::ranges {
    class C {
        friend void swap(C&, C&);
    };
}
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
namespace std::experimental::ranges {
    class C;
    void swap(C&, C&);
}
        </pre></td>
    </tr>

    </tr>
    
    <tr>
        <td  align="left" valign="top"><b>
Customization by overload in std::experimental::ranges for enums
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

namespace std::experimental::ranges {
namespace __hidden {
    enum E { ... };
    void swap(E&, E&);
}
    using __hidden::E;
}
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
namespace std::experimental::ranges {
    enum E { ... };
    void swap(E&, E&);
}
        </pre></td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Customization of a std condition StdCond
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

namespace std::experimental::ranges {
namespace __swap {
    struct swap_fn {
        template &lt;class T, class U>
        enable_if&lt;StdCond&lt;T,U>{}> operator()(T&, U&);
        //...
    };
}
}
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
namespace std::experimental::swappable  {
    template &lt;class T, class U>
    struct traits&lt;T,U, when&lt;StdCond&lt;T,U>{}>> {
        static void swap(T&, U&);
        //...
    };
}
        </pre></td>
    </tr>
    
    <tr>
        <td  align="left" valign="top"><b>
Customization of a user condition UserCond
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

Not possible with overload.
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
namespace std::experimental::swappable {
    template &lt;class T, class U>
    struct traits&lt;T,U, when&lt;UserCond&lt;T,U>{}>> {
        static void swap(T&, U&);
        //...
    };
}
        </pre></td>
    </tr>
    
    <tr>
        <td  align="left" valign="top"><b>
Customization of a user class in case of inter-operation
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

Not possible with overload as there is conflict.
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
namespace std::experimental::hashable {
    struct traits&lt;user::A> {
        static void hash_value(A const&) {...}
        //...
    };
}
        </pre></td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Customization of a user class in case of inter-operation with two concepts, e.g. <em>ProductType</em> and <em>Range</em>. 
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

Not possible with overload.
        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
namespace std::experimental::hashable {
    struct traits&lt;user::A,user::A> 
        : range_as_hashable
    {};
}
        </pre></td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Customization of functions that have non-deduced template parameters, e.g. <code>make&lt;optional></code>. 
        </b></td>
    </tr>   
    <tr>
        <td align="left" valign="top">
Not possible with overload, except if the customization point includes an artificial parameter <code>type&lt;T></code>
        <pre class='brush: cpp'>


        </pre></td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
namespace std::experimental::factory {
    struct traits&lt;future> {
        template <class T>
        static future&lt;decay_unwrap&lt;T>> make(T&& v) {
            return make_future_ready(forward&lt;T>(v));
        }
        //...
    };
}
        </pre></td>
    </tr>
    
    
</table>


# Acknowledgements 

Thanks to Louis Dionne for his library [Boost.Hana] which has inspired the proposed design.

Thanks to Eric Nibbler for his proposal [N4381] for customization points.


# References

[Boost.Hana]: http://boostorg.github.io/hana/index.html "Boost.Hana library"

[N1691]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1691.html "Explicit Namespaces"[N4381]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4381.html "Suggested Design for Customization Points"
[N4569]: www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/n4569.pdf "Working Draft, C++ Extensions for Ranges"

[P0119R2]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0119r2.pdf "Overload sets as function arguments"

[P0370r0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0370r0.html "Ranges TS Design Updates Omnibus"

[P0382R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0382r0.html "Comments on P0119: Overload sets as function arguments"

* [Boost.Hana] Boost.Hana library
http://boostorg.github.io/hana/index.html

* [N1691] Explicit Namespaces http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1691.html

* [N4381] Suggested Design for Customization Points
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4381.html

* [N4569] Working Draft, C++ Extensions for Ranges
www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/n4569.pdf

* [P0119R2] Overload sets as function arguments
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0119r2.pdf
	
* [P0370r0] Ranges TS Design Updates Omnibus
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0370r0.html

* [P0382R0] Comments on P0119: Overload sets as function arguments
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0382r0.html
