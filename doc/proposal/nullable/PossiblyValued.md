<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXXR0</span></td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2017-09-27</td>
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

# *PossiblyValued* and *ValuedOrError* types
=================================================

**Abstract**

In the same way we have *Nullable* types that have a single not-a-value we have *PossiblyValued* types that can contain a single instance of value-type and a mean to retrieve it using  the `deref` function. These types have something in common with *Nullable* and is the ability to know if they have a value or not via the `has_value` function. We have named this superset of types *PossiblyNotValued*. Types that are *PossiblyValued* and have a single error are named in this paper *ValedOrError*. They provide the `error` function. `optional`, pointers and smart pointers are *PossiblyValued* types. `expected` is a *ValedOrError* type.


# Table of Contents

* [Introduction](#introduction)
* [Motivation and Scope](#motivation-and-scope)
* [Proposal](#proposal)
* [Design Rationale](#design-rationale)
* [Proposed Wording](#proposed-wording)
* [*Implementability](#implementability)
* [Open points](#open-points)
* [Acknowledgements](#acknowledgements)
* [History](#history)
* [References](#references)

# Introduction


Revision 3 adds a new `nullable::deref` customization point and a lot of algorithms that can be built on top of *Nullable* thanks to this addition, as *Functor* `transform`, *ApplicativeFunctor* `ap`, *Monad* `bind`,  *SumType* `visit`, and some minor algorithms `value_or`and `apply_or`. 

<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="807">
    <tr>
        <td align="left" valign="top"> BEFORE </td>
        <td align="left" valign="top"> AFTER </td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Construction
        </b></td>
        <td  align="left" valign="top"><b>
        </b></td>
    </tr>
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

        </pre>
        </td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
        </pre>
        </td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Conversion

        </b></td>
        <td  align="left" valign="top"><b>
        </b></td>
    </tr>
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

        </pre>
        </td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
        </pre>
        </td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Return
        </b></td>
        <td  align="left" valign="top"><b>
        </b></td>
    </tr>
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

        </pre>
        </td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
        </pre>
        </td>
    </tr>
    
       
</table>

# Motivation and Scope

## Why do we need a generic `deref()` access


## `possibly_valued::value_type_t`

## `value_or_error::error_type_t` 

A *ValueOrError* can be considered as a sum type. It is always useful reflect the related types. `value_or_error::error_type_t` and `nullable::value_type_t` give respectively the associated non-a-value and the value types.

# Proposal

This paper proposes to

* add `has_value(n)` and *PossiblyNotValued* types,
* add `deref(n)` and *PossiblyValued* types,
* add `error(n)` and *ValuedOrError* types,
* define *Nullable* types as *PossiblyNotValued* types and
* customize the standard types to these concepts.

# Design Rationale

## SucessOrFailure

Alternatively we could use SuccessOrFailure.

# Impact on the standard

These changes are entirely based on library extensions and do not require any language features beyond what is available in C++17. There are however some classes in the standard that needs to be customized.

This paper depends in some way on the helper classes proposed in [P0343R1], as e.g. the place holder `_t` and the associated specialization for the type constructors `optional<_t>`, `unique_ptr<_t>`, `shared_ptr<_t>`.

# Proposed Wording

The proposed changes are expressed as edits to [N4564] the Working Draft - C++ Extensions for Library Fundamentals V2.

**Add the "PossiblyNotValued Objects" section**

## PossiblyNotValued Objects

### Header <experimental/possibly_not_valued> synopsis [possibly_not_valued.synop]

```c++
namespace std::experimental {
inline namespace fundamentals_v3 {
namespace possibly_not_valued {       
    
    // class traits
    template <class T, class Enabler=void>
        struct traits {};

    // class traits_pointer_like
    struct traits_pointer_like;

    // class traits specialization for pointers
    template <class T>
        struct traits<T*>;
          
    template <class T>
        constexpr bool has_value(T const& v) noexcept;
    template <class T>
        constexpr bool has_value(T* v) noexcept;
        
}

    using nullable::has_value;
  
    template <class T>
        struct is_possibly_not_valued;
    template <class T>
        struct is_possibly_not_valued <const T> : is_possibly_not_valued <T> {};
    template <class T>
        struct is_possibly_not_valued <volatile T> : is_possibly_not_valued <T> {};
    template <class T>
        struct is_possibly_not_valued <const volatile T> : is_possibly_not_valued <T> {};
    template <class T>
        struct is_possibly_not_valued <T*> : true_type {};

    template <class T>
        constexpr bool is_possibly_not_valued_v = is_possibly_not_valued <T>::value ;

}}
```

###  class `traits` [possibly_not_valued.traits]

```c++
namespace possibly_not_valued {
    template <class T, class Enabler=void>
        struct traits {};

    // class traits_pointer_like
    struct traits_pointer_like
    {
        template <class Ptr>
            static constexpr
            bool has_value(Ptr ptr) { return bool(ptr) }
    };

    // class traits specialization for pointers
    template <class T>
        struct traits<T*> 
            : traits_pointer_like<T*>
        {};
} 
```

###  Template function `has_value` [possibly_not_valued.has_value]

```c++
namespace nullable {
    template <class T>
        bool has_value(T const& v) noexcept;
    template <class T>
        bool has_value(T* v) noexcept;
}
```


**Adapt the "Nullable Objects" section**

## Nullable Objects


### *Nullable* requirements

A *Nullable* type is a type that supports a distinctive null value. A type `N` meets the requirements of *Nullable* if:

* `N` satisfies the requirements of *EqualityComparable* *DefaultConstructible*, and *Destructible*,
* the expressions shown in the table below are valid and have the indicated semantics, and
* `N` satisfies all the other requirements of this sub-clause.

A value-initialized object of type `N` produces the null value of the type. The null value shall be equivalent only to itself. A default-initialized object of type `N` may have an indeterminate value. [ Note: Operations involving indeterminate values may cause undefined behavior. — end note ]

No operation which is part of the *Nullable* requirements shall exit via an exception.

In Table X below, `u` denotes an identifier, `t` denotes a non-const lvalue of type `N`, a and b denote values of type(possibly const) N, `x` denotes a (possibly const) expression of type `N`,
and `nN` denotes `std::experimental::none<N>()`
and `n` denotes `std::experimental::none()`.

<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td align="left" valign="top"> <b>Expression</b> </td>
        <td align="left" valign="top"> <b>Return Type</b> </td>
        <td align="left" valign="top"> <b>Operational Semantics</b> </td>
    </tr>
    <tr>
        <td align="left" valign="top"> nullable::none&lt;N>() </td>
        <td align="left" valign="top"> none_type_t&lt;N></td>
        <td align="left" valign="top"> </td>
    </tr>
    <tr>
        <td align="left" valign="top"> N{} </td>
        <td align="left" valign="top"> </td>
        <td align="left" valign="top"> post: N{} == nN</td>
    </tr>
    <tr>
        <td align="left" valign="top"> N u(n) </td>
        <td align="left" valign="top"> </td>
        <td align="left" valign="top"> post: u == nN</td>
    </tr>
    <tr>
        <td align="left" valign="top"> N u(nN) </td>
        <td align="left" valign="top"> </td>
        <td align="left" valign="top"> post: u == nN</td>
    </tr>
    <tr>
        <td align="left" valign="top"> N u = n </td>
        <td align="left" valign="top"> </td>
        <td align="left" valign="top"> post: u == nN</td>
    </tr>
    <tr>
        <td align="left" valign="top"> N u = nN </td>
        <td align="left" valign="top"> </td>
        <td align="left" valign="top"> post: u == nN</td>
    </tr>
    <tr>
        <td align="left" valign="top"> N(n) </td>
        <td align="left" valign="top"> </td>
        <td align="left" valign="top"> post: N(n) == nN</td>
    </tr>
    <tr>
        <td align="left" valign="top"> N(nN) </td>
        <td align="left" valign="top"> </td>
        <td align="left" valign="top"> post: N(nN) == nN</td>
    </tr>
    <tr>
        <td align="left" valign="top"> std::has_value(x) </td>
        <td align="left" valign="top"> contextually convertible to bool</td>
        <td align="left" valign="top"> true if x != nN</td>
    </tr>
    <tr>
        <td align="left" valign="top"> a != b </td>
        <td align="left" valign="top"> contextually convertible to bool</td>
        <td align="left" valign="top"> !(a == b) </td>
    </tr>
    <tr>
        <td align="left" valign="top"> a == np, np == a</td>
        <td align="left" valign="top"> contextually convertible to bool</td>
        <td align="left" valign="top"> a == N{} </td>
    </tr>
    <tr>
        <td align="left" valign="top"> a != np , np != a</td>
        <td align="left" valign="top"> contextually convertible to bool</td>
        <td align="left" valign="top"> !(a == N{}) </td>
    </tr>

</table>




**Adapt to possibly_not_valued**

### Header <experimental/nullable> synopsis [nullable.synop]

**Add using possibly_not_valued**

```c++
namespace std {
namespace experimental {
inline namespace fundamentals_v3 {
namespace nullable {
    using namespace possibly_not_valued;
   
}}}}
```


###  class `traits` [nullable.traits]

**Remove the has_value**

```c++
namespace nullable {

    // class traits_pointer_like
    struct traits_pointer_like
    {
        static constexpr
            nullptr_t none() noexcept { return nullptr; }
    };

} 
```


### Header <experimental/possibly_valued> synopsis [PossiblyValued.synop]

```c++
namespace std::experimental {
inline namespace fundamentals_v3 {
namespace possibly_valued {
    using namespace possibly_not_valued;

    // class traits
    template <class T, class Enabler=void>
        struct traits {};

    // class traits_pointer_like
    struct traits_pointer_like;

    // class traits specialization for pointers
    template <class T>
        struct traits<T*>;
        
    template <class T>
        constexpr auto deref(T&& x);
    template <class T>
        constexpr T& deref(T* ptr);
        
    template <class T>
        using value_type_t = decltype(nullable::deref(declval<T>));        

    template <class N, class T>
        constexpr auto value_or(N&& ptr, T&& val); 
                
    template <class N, class F, class T>
        constexpr auto apply_or(N&& n, F&& f, T&& v); 
          
}

 
    template <class T>
        struct is_possibly_valued;
    template <class T>
        struct is_possibly_valued<const T> : is_possibly_valued<T> {};
    template <class T>
        struct is_possibly_valued<volatile T> : is_possibly_valued<T> {};
    template <class T>
        struct is_possibly_valued<const volatile T> : is_possibly_valued<T> {};
    template <class T>
        struct is_possibly_valued<T*> : true_type {};

    template <class T>
        constexpr bool is_possibly_valued_v = is_possibly_valued<T>::value ;

}
}
```

### Header <experimental/value_or_error> synopsis [ValueOrError.synop]

```c++
namespace std::experimental {
inline namespace fundamentals_v3 {
namespace value_or_error {
    using namespace possibly_valued;

    // class traits
    template <class T, class Enabler=void>
        struct traits {};

    // class traits_pointer_like
    struct traits_pointer_like;

    // class traits specialization for pointers
    template <class T>
        struct traits<T*>;
        
    template <class T>
        constexpr auto error(T&& x);
    template <class T>
        constexpr T& error(T* ptr);

    template <class T>
        using error_type_t = decltype(nullable::error(declval<T>));        

    // when type constructible, is a functor
    template <class T, class F>
        constexpr auto transform(T&& n, F&& f); 

    // when type constructible, is an applicative
    template <class F, class T>
        constexpr auto ap(F&& f, T&& n); 

    // when type constructible, is a monad
    template <class T, class F>
        constexpr auto bind(T&& n, F&& f); 

    // when type constructible, is a monad_error
    template <class T, class F>
        constexpr auto catch_error(T&& n, F&& f); 
    template <class T, class ...Xs>
        constexpr auto make_error(Xs&&...xs); 
    template <class T, class F>
        constexpr auto adapt_error(T&& n, F&& f); 

    // sum_type::visit
    template <class N, class F>
        constexpr auto visit(N&& n, F&& f); 
        
    // helper functions    
    template <class N, class E>
        constexpr auto error_or(N&& ptr, E&& err); 
                
    template <class N, class E>
        constexpr bool has_error(N&& n, E&& err); 
          
    template <class N, class F>
        constexpr auto resolve(N&& n, F&& f); 
          
    template <class N, class F>
        constexpr auto adapt_error(N&& n, F&& f); 
}
 
    template <class T>
        struct is_value_or_error;
    template <class T>
        struct is_value_or_error <const T> : is_value_or_error <T> {};
    template <class T>
        struct is_value_or_error <volatile T> : is_value_or_error <T> {};
    template <class T>
        struct is_value_or_error <const volatile T> : is_value_or_error <T> {};
    template <class T>
        struct is_value_or_error <T*> : true_type {};

    template <class T>
        constexpr bool is_value_or_error_v = is_value_or_error <T>::value ;
}
}
```

### Header <experimental/value_or_none> synopsis [ValueOrNone.synop]

```c++
namespace std::experimental {
inline namespace fundamentals_v3 {
 
    template <class T>
        struct is_value_or_none;

    template <class T>
        constexpr bool is_value_or_none_v = is_value_or_none <T>::value ;

namespace value_or_none {
    

    // class value_or_none
    struct traits_value_or_none;

    // class traits specialization for Nullable and PossiblyValued
    template <class T>
        struct traits<T, when<is_value_or_none_v<T>>>;

}
}
}
```


## Optional Objects

**Add Specialization of *PossiblyNotValued* [optional.object.possibly_not_valued]**.

20.6.x PossiblyNotValued

`optional<T>` is a model of *PossiblyNotValued*.

```c++
namespace possibly_not_valued {
    template <class T>
    struct traits<optional<T>> {
      template <class U>
      static constexpr
      bool has_value(optional<U> const& v) noexcept { return v.has_value(); }
    };
}
```

**Add Specialization of *PossiblyValued* [optional.object.possibly_valued]**.

20.6.x PossiblyValued

`optional<T>` is a model of *PossiblyValued*.

```c++
namespace possibly_valued {
    template <class T>
    struct traits<optional<T>> {
      template <class U>
      static constexpr
      auto deref(optional<U> const& v) noexcept { return *v; }
    };
}
```

**Update Specialization of *Nullable* [optional.object.nullable]**.

20.6.x Nullable

`optional<T>` is a model of *Nullable*.

```c++
namespace nullable {
    template <class T>
    struct traits<optional<T>> {
      static constexpr
      nullopt_t none() noexcept { return nullopt; }
    };
}
```

    
## Class Any

**Add Specialization of *PossiblyNotValued* [any.object.nullable]**.

20.6.x PossiblyNotValued

`any` is a model of *PossiblyNotValued*.


```c++
namespace possible_not_valued {
    template <>
    struct traits<any> {
      static constexpr
      bool has_value(any const& v) noexcept { return v.has_value(); }
    };
}
```

**Update Specialization of *Nullable* [any.object.nullable]**.

20.6.x Nullable

`any` is a model of *Nullable*.


```c++
namespace nullable {
    template <>
    struct traits<any> {
      static constexpr
      none_t none() noexcept { return none_t{}; }
    };
}
```

## Variant Objects

**Update Specialization of *Nullable* [variant.object.nullable]**.

`variant<none_t, Ts...>` is a models of *Nullable*.

```c++
namespace nullable {
    template <class ...Ts>
    struct traits<variant<none_t, Ts...>> {
      static constexpr
      none_t none() noexcept { return none_t{}; }
      template <class ...Us>
      static constexpr
      bool has_value(variant<none_t, Us...> const& v) noexcept { return v.index()>0; }
    };
}
```

## Smart Pointers

`unique_ptr<T, D>` is a models of *Nullable*.

```c++
namespace nullable {
    template <class T, class D>
    struct traits<unique_ptr<T, D> : traits_pointer_like {};
}
```

`shared_ptr<T>` is a models of *Nullable*.

```c++
namespace nullable {
    template <class T>
    struct traits<shared_ptr<T>> : traits_pointer_like {};
}
```

# Implementability

This proposal can be implemented as pure library extension, without any language support, in C++14.

# Open points

The authors would like to have an answer to the following points if there is any interest at all in this proposal:

* Should we include ths in `<experimental/functional>` or in a specific file?
	* We believe that a specific file is a better choice as this is needed in `<experimental/optional>`, `<experimental/any>` and `<experimental/variant>`. We propose to locate each concept in one file `<experimental/valued_or_error>`.


## *PossiblyValued* types

We could define a *PossiblyValued* type of classes that will cover the operations `has_value` and `deref`. This is different  than 
*Nullable*, but we can have some kind of *Nullables* that are a special case of *PossiblyValued*.

*PossiblyValued* could have associated operations as 

* `value_or` 
* `apply_or` 

## *ValueOrError* types

We could define a *ValueOrError* type of classes that is more general than *Nullable* and see *Nullable* as a special case of *ValueOrError* when the not-a-value is a unit type. However the constraint on the default constructor could not be covered by *ValueOrError* as it has no sense and if we would want `expected<T,E>` to be a *ValueOrError* type. *ValueOrError* cannot have neither the `none<N>()`. Instead it could have associated the extractor

* `error(ValueOrError) -> Error` 

*ValueOrError* types will 

* not requires a default constructor.
* not be convertible from `none_t`, but 
* must be implicitly convertible from `value_type_t<N>` and `error_type_t<N>`. 

In addition to the *Nullable* operation, *ValueOrError* could have associated operations as 

* `error_or`, 
* `has_error`, 
* `adapt_error` and 
* `resolve`.

One important thing about *ValueOrError* types is that they are a subset of the *MonadError* and that we can define a TRY operator on these types that makes these good for transporting error. 

## About `PossiblyValued::value(n)`

We could define a wide `possibly_valued::value(n)` function on *PossiblyValued* that obtain the value or throws an exception. If we want to have a default implementation the function will need to throw a generic exception `bad_access`.

However to preserve the current behavior of `std::optional::value()` we will need to be able to consider this function as a customization point also.

## `PossiblyValued::deref`

Pointers as `std::optional` provide the de-reference operator.

Do we want to have an explicit `possibly_valued::deref(n)` or use the more friendly `*n`?

# Future work

## *PossiblyValued* as a Functor

While we don't have yet an adopted proposal for *Functor*, we can define a default `possibly_valued::transform` function for * PossiblyValued*. 

## *PossiblyValued* as an Applicative Functor

While we don't have yet an adopted proposal for *ApplicativeFunctor*, we can define a default `possibly_valued::ap` function for *PossiblyValued*. 

## *PossiblyValued* as a Monad

While we don't have yet an adopted proposal for *Monad*, we can define a default `possibly_valued::bind` function for *PossiblyValued*. 

## *ValueOrError* as a MonadError

While we don't have yet an adopted proposal for *MonadError*, we can define a default `value_or_error::catch_error` and `value_or_error::make_error` functions for *ValueOrError*. 


# Acknowledgements

Thanks to ...

Special thanks and recognition goes to Technical Center of Nokia - Lannion for supporting in part the production of this proposal.

# History

## Revision 0

* Rollback the modifications of revision 3, as they belong to *ValueOrError* kind of types. 
    * We should move `deref()` to `PossiblyValued`, as `std::any` cannot define it, as it has not a unique value. As consequence the functions added in the previous version `value_or` and `apply_or` should be moved as well.
    * The same applies to the added `visit` function. We cannot visit any *Nullable* as we don't know what are its possibly values. This function belongs to *ValueOrNone* and to *ValueOrError*. *ValueOrNone* is a *Nullable* that has only one possible value type. *ValueOrNone* is specialization of *ValueOrError* where the Error is a unit type.
    * Next follows the concept hierarchy:

        * *PossiblyNotValued* (has_value)
            * *PossiblyValued* (deref) => value_or, apply_or
                * *ValueOrError*  (error) (examples: expected, result => TRY, SumType, Functor, Applicative, Monad, MonadError
                    * *ValueOrNone*  
            * *Nullable* (none) (examples std::variant<none_t, Ts...>, std::any)
                * *ValueOrNone* (examples T*, std::optional, std::unique_ptr, ...)  
                 
    * Move *Functor*, *Applicative* and *Monad* specialization to *ValueOrError*.
    * We could add later the *ValueOrNone* that maps the none type as an error to behave like a *ValueOrError* when there is only a possibly value type and the Error is a unit type.
    * However, we should define *PossiblyNotValued*
     
# References

[N4564]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf "N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS"

[P0032R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0032r0.pdf "Homogeneous interface for variant, any and optional"

[P0050R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0050r0.pdf "C++ generic match function"

[P0088R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf "Variant: a type-safe union that is rarely invalid (v5)"  

[P0091R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0091r0.html "Template parameter deduction for constructors (Rev. 3)"

[P0338R2]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0338r2.pdf "C++ generic factories"

[P0343R1]: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0343r1.pdf "Meta-programming High-Order functions"

[P0650R0]: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0650r0.pdf "C++ Monadic interface"  

[LWG 2510]: http://cplusplus.github.io/LWG/lwg-active.html#2510 "Tag types should not be DefaultConstructible"
        
[CWG 1518]: http://open-std.org/JTC1/SC22/WG21/docs/cwg_active.html#1518 "Explicit default constructors and copy-list-initialization" 
    
[CWG 1630]: http://open-std.org/JTC1/SC22/WG21/docs/cwg_defects.html#1630 "Multiple default constructor templates" 

[SUM_TYPE]: https://github.com/viboes/std-make/tree/master/include/experimental/fundamental/v3/sum_type "Generic Sum Types"
    

* [N4564] N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf

* [P0032R0] Homogeneous interface for variant, any and optional

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0032r0.pdf

* [P0050R0] C++ generic match function

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0050r0.pdf

* [P0088R0] Variant: a type-safe union that is rarely invalid (v5)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf

* [P0091R0] Template parameter deduction for constructors (Rev. 3)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0091r0.html
    
* [P0338R2] C++ generic factories

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0338r2.pdf

* [P0343R1] - Meta-programming High-Order functions

    http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0343r1.pdf

* [P0650R0] C++ Monadic interface

    http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0650r0.pdf

* [LWG 2510] Tag types should not be DefaultConstructible

    http://cplusplus.github.io/LWG/lwg-active.html#2510
    
* [CWG 1518] Explicit default constructors and copy-list-initialization 

    http://open-std.org/JTC1/SC22/WG21/docs/cwg_active.html#1518

* [CWG 1630] Multiple default constructor templates  

    http://open-std.org/JTC1/SC22/WG21/docs/cwg_defects.html#1630

* [SUM_TYPE] Generic Sum Types

    https://github.com/viboes/std-make/tree/master/include/experimental/fundamental/v3/sum_type

