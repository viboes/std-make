<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">D0XXXR0</span></td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2016-10-16</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">ISO/IEC JTC1 SC22 WG21 Programming Language C++</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Audience:</td>
        <td width="435"> Reflection Working Group / Library Evolution Working Group </td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escrib&aacute; &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@nokia.com</a>&gt;</td>
    </tr>
</table>


Product-Type algorithms
=======================

# Abstract

This paper proposes some *ProductType* algorithms based on [P0327R2] proposal. 

# Table of Contents

[History](#history)
[Introduction](#introduction)
[Motivation](#motivation)
[Proposal](#proposal)
[Design Rationale](#design-rationale)
[Proposed Wording](#proposed-wording)
[Implementability](#implementability)
[Open points](#open-points)
[Future work](#future-work)
[Acknowledgements](#acknowledgements)
[References](#references)

# History
## R0

Take in account the feedback from Kona meeting concerning [P0327R1]. Next follows the direction of the committee: 

* Split the proposal into 3 documents
    * Product Type access
    * Adaptation of current tuple-like algorithms to *ProductType*
    * Basic *ProductType* algorithms
    * Other *ProductType* algorithms
     
* In this document, we describe more algorithms applicable to *ProductTypes.  


# Introduction


# Motivation

## Status-quo

There are a lot of algorithms working on *ProductTYpe*; a lot of the homogeneous container algorithm are applicable to heterogeneous containers and functions, see [Boost.Fusion] and [Boost.Hana]. Some examples of such algorithms are `for_each`, `filter`, `find`, `fold`, `any_of`, `all_of`, `none_of`, `accumulate`, `count`, ...  

Other algorithms that need in addition that the *ProductType* to be also *TypeConstructible* are e.g. `transform`, `replace`, `join`, `zip`, `flatten`, ... 

# Proposal

This paper proposes some algorithms that can be built on top of the *ProductType* and the *TypeConstructible* requirements.

# Design Rationale

## Locating the interface on a specific namespace

The name of  *product type* interface, `size`, `get`, `element`, are quite common. Nesting them on a specific namespace makes the intent explicit. 

We can also preface them with `product_type_`, but the role of namespaces was to be able to avoid this kind of prefixes.

## Other functions for *ProductType*

Aside [BPT] there are a lot of useful function associated to product types that make use only of the product type access traits and functions.

### `for_each`

```c++
  template <class F, class ProductType>
    constexpr void for_each(F&& f, ProductType&& pt);
```

This is the equivalent of `std::for_each` applicable to product types instead of homogeneous containers or range types.

## `fold_left`/`fold_right`/`accumulate`

This is the equivalent of `std::accumulate` applicable to product types instead of homogeneous containers types. 

## `all_of`

Checks if n-unary n-predicate `p` returns `true` for all elements in the product type. 

## `any_of`

Checks if n-unary n-predicate `p` returns `true` for at least one elements in the product type. 

## `none_of`

Checks if n-n-unary predicate `p` returns `true` for no elements in the product type. 

## Other functions for *TypeConstructible* *ProductTypes*

Some algorithms need a *TypeConstructible* *ProductTypes* as they need to construct a new instance of a *ProductTypes*.

An alternative is to use `std::tuple` as the parameter determining the *Product Type* to construct. 

We could also add a *TypeConstructible* parameter, as e.g.  

```c++
    template <template <class...> TC, class ...ProductTypes>
        constexpr `see below` cat(ProductTypes&& ...pts);
    template <class TC, class ...ProductTypes>
        constexpr `see below` cat(ProductTypes&& ...pts);
```

Where `TC` is a variadic template for a *ProductType* as e.g. `std::tuple` or a TypeConstructor [P0343R0]. 

### `transform`

```c++
  template <class F, class ProductType>
    constexpr `see below` transform(F&& f, ProductType&& pt);
```

This is the equivalent of `std::transform` applicable to product types instead of homogeneous containers types.

This needs in addition that `ProductType` is *TypeConstructible* (See [P0338R0]).
Note that `std::pair`, `std::tuple` and `std::array` are *TypeConstructible*, but `std::pair` and `std::array` limit either in the number or in the kind of types (all the ame). 
A c-array is not type *TypeConstructible* as it cannot be returned by value.

# Proposed Wording

The proposed changes are expressed as edits to 
[N4564] .

**Add the following section in [N4564]**

## Product type algorithms

Some algorithms need a `make<TC>(args...)` factory [P0338R1].

If the first product type argument is *TypeConstructible* from the `CTypes` then return an instance of it; otherwise construct a `std::tuple`. 

### Product type algorithms synopsis

```c++
namespace std {

namespace product_type {
           
    template <class F, class State, class ProductType
        constexpr State fold_left(ProductType&& pt, State&& state, F&& f);
    template <class F, class ProductType
        constexpr State fold_left(ProductType&& pt, F&& f);
    
    template <class F, class ProductType
        constexpr void for_each(ProductType&& pt, F&& f);                                 
}}
```

### Function Template `product_type::fold_left`

```c++
template <class F, class State, class ProductType>
  constexpr State fold_left(ProductType&& pt, State&& state, F&& f);

template <class F, class ProductType
  constexpr State fold_left(ProductType&& pt, F&& f);       
```
   
### `front`

`front: PT(T) -> T`

### `back`

`back: PT(T) -> T`

### `is_empty`

`is_empty : PT(T) -> bool`

### `transform`

`transform: TCPT(T)... -> TCPT(T)`

### `drop_front`

`drop_front: TCPT(T) -> TCPT(T)`

### `drop_back`

`drop_back: TCPT(T) -> TCPT(T)`

### `group`

`TCPT(T) -> TCPT(TCPT(T))`

### `insert`

`insert: TCPT(T) x unsigned x T -> TCPT(T)`
             
# Implementability


# Open Questions

The authors would like to have an answer to the following points if there is any interest at all in this proposal:

* Do we want this for Fundamental TS V3?

# Future work

## Add other algorithms on *Product Types* 


## *Product Types* views and lazy algorithms

Based on Range views for homogeneous Ranges [Range-v3], views for heterogeneous sequences [Boost.Fusion][Boost.Hana] define *Product Types* views, adaptors, ...

## Tagged *Product Types*

Based on the work [N4569] for tagged tuples, associative sequences in [Boost.Fusion], Struct in [Boost.Hana] define Tagged *ProductTypes* and specific algorithms for them.

# Acknowledgments

Thanks to all those that help on [P0327R1]. 

Special thanks and recognition goes to Technical Center of Nokia - Lannion for supporting in part the production of this proposal.

# References


[N4564]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf "N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS"

[P0327R1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0327r1.pdf  "Product Type Access (Revision 1)"   

[P0327R2]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0327r2.pdf  "Product Type Access (Revision 2)"   

[P0338R1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0338r1.pdf  "C++ generic factories (Revision 1)"   

[P0343R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0343r0.html "Meta-programming High-Order Functions"  

[PT_impl]: https://github.com/viboes/std-make/tree/master/include/experimental/fundamental/v3/product_type "Product types access emulation and algorithms."

[Range-v3]: https://github.com/ericniebler/range-v3 "range-v3"


* [N4564] N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf

* [P0327R1] Product Type Access (Revision 1)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0327r1.pdf   

* [P0327R2] Product Type Access (Revision 2)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0327r2.pdf     

* [P0338R1] C++ generic factories (Revision 1)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0338r1.pdf 

* [P0343R0] Meta-programming High-Order Functions 

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0343r0.html
     
* [PT_impl] Product types access emulation and algorithms

    https://github.com/viboes/std-make/tree/master/include/experimental/fundamental/v3/product_type

* [Range-v3] range-v3

    https://github.com/ericniebler/range-v3
