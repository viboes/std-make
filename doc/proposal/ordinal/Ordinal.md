<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXXR0</span></td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2017-05-07</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">ISO/IEC JTC1 SC22 WG21 Programming Language C++</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Audience:</td>
        <td width="435">Library Evolution Working Group/Reflection Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escrib&aacute; &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@wanadoo.fr</a>&gt;</td>
    </tr>
</table>

# C++ Ordinal types and associated containers
=============================================

**Abstract**



# Table of Contents

* [Introduction](#introduction)
* [Motivation and Scope](#motivation-and-scope)
* [Proposal](#proposal)
* [Design Rationale](#design-rationale)
* [Impact on the standard](#impact-on-the-standard)
* [Proposed Wording](#proposed-wording)
* [Implementability](#implementability)
* [Open points](#open-points)
* [Acknowledgements](#acknowledgements)
* [References](#references)


# Introduction



# Motivation and Scope

With the advent of more strong typing, like bounded integers `month`, `weekday`, we have now that these types cannot be used directly as index of `std::bit_set`. Kevlin Henney presented in Flag Waiving [Flag_Waiving] a safe design for enum sets which is based on the association of an enumeration element and its position on the enumeration itself or to some kind of transformation (bit flags) to the range `0`..`N`. 

```c++
using B = bounded_int<1,10> ;
using S = std::bitset<B::size> ;
S set;
B b;
do_somthing(set[int(b)-1]);
```

With

```c++
using B = bounded_int<1,10> ;
using S = ordinal_set<B::size> ;
S set;
B b;
do_somthing(set[b]);
```

We have that types as `bounded_int` cannot be used as index of arrays. The same mapping can be used to implement arrays having a `bounded_int` type as index type. 

```c++
using B = bounded_int<1,10> ;
using A = array<X,B::size> ;
A arr;
B b;
do_somthing(arr[int(b)-1]);
```

With

```c++
using B = bounded_int<1,10> ;
using A = ordinal_array<X,B> ;
A arr;
B b;
do_something(arr[b]);
```

The range-v3 library [RangeV3] proposes some integral range view.
Given a type that is isomorphic to 0..N, it is natural to consider the type itself as the range of its values.

Without

```c++
using B = bounded_int<1,10> ;
for (int i = B::first; i<=B::last, ++i )
    do_someting(B(i));    
```

With

```c++
using B = bounded_int<1,10> ;
for (B b : ordinal_range<B>())
    do_someting(b);    
```

This is found also in language as Ada, which defines attributes on enumerations like `first`, `last`, `succ`, `value`, `pos`, ... This association has constant time complexity most of the times, as usual enumerations follows linear or exponential (base 2) progressions. 

Examples of libraries that have tried to cope with some of these aspects are:

*    Boost.Enum by Frank Laub, takes care of scoping, underlying type, iteration and string conversions, but doesn't take care of avoiding implicit conversion and portability to C++11 scoped enums.
    This thread Smart enums and this one safe assign of int to enum are quite interesting. Unfortunately there were no concrete proposal.
*    Boost.SmartEnums has complex design to take care of iteration.
*    BEnum.

# Proposal

This paper proposes to add the following 

* *Ordinal* types isomorphic to `0`..`N`.
* Containers using *Ordinal* types as elements or indexes
    * `ordinal_set` using a bit for each element of the *Ordinal* type.
    * `ordinal_array` using an *Ordinal* type as index.
    * `ordinal_range` a run-time range view of an *Ordinal* type.

# Design Rationale

## Traits based customization concept

## Range view

The range library proposes some integral range view.


# Impact on the standard

These changes are entirely based on library extensions and do not require any language features beyond what is available in C++17. There are however some classes in the standard that can be customized.


# Proposed Wording

The proposed changes are expressed as edits to [N4564] the Working Draft - C++ Extensions for Library Fundamentals V2.

**Add a "Ordinal Type Constructors" section**

## Ordinal Type Constructors

### *Ordinal* requirements

A *Ordinal* is a type that supports the `transform` function. A type `T` meets the requirements of *Ordinal* if:

* `O` satisfies the requirements of *EqualityComparable* *DefaultConstructible*, and *Destructible*,
* the expressions shown in the table below are valid and have the indicated semantics, and
* `T` satisfies all the other requirements of this sub-clause.

In Table X below, `o` denotes an rvalue of type `O`, `p` denotes a rvalue of type `size_t`.

<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="850">
    <tr>
        <td align="left" valign="top"> <b>Expression</b> </td>
        <td align="left" valign="top"> <b>Return Type</b> </td>
        <td align="left" valign="top"> <b>Operational Semantics</b> </td>
    </tr>
    <tr>
        <td align="left" valign="top"> ordinal::size<O>() </td>
        <td align="left" valign="top"> size_t </td>
        <td align="left" valign="top"> number of elements in O</td>
    </tr>
    <tr>
        <td align="left" valign="top"> ordinal::val(p) </td>
        <td align="left" valign="top"> O </td>
        <td align="left" valign="top"> value at position p, for p in 0..(size<O>-1)</td>
    </tr>
    <tr>
        <td align="left" valign="top"> ordinal::pos(o) </td>
        <td align="left" valign="top"> size_t </td>
        <td align="left" valign="top"> position of o</td>
    </tr>

</table>

### Header <experimental/ordinal> synopsis [ordinal.synop]

```c++
namespace std {
namespace experimental {
inline namespace fundamentals_v3 {
namespace ordinal {

  struct tag{};
  template <class TC, class Enabler=void>
    struct traits {};
    
  template <class Ordinal>
    constexpr size_t size() noexcept;
  template <class Ordinal>
    constexpr Ordinal val(size_t pos);
  template <class Ordinal>
    constexpr size_t pos(Ordinal&& val);   
  template <class Ordinal>
    constexpr Ordinal first() noexcept;
  template <class Ordinal>
    constexpr Ordinal last() noexcept;
  template <class Ordinal>
    constexpr Ordinal succ(Ord&& val);
  template <class Ordinal>
    constexpr Ordinal pred(Ord&& val);

  template <class T, T Low, T High, T Step = T{1}, class SizeType=size_t>
    struct arithmetic_traits;  
  template <typename T>
    struct integral_traits: 
  template <> struct traits<int>;
  template <> struct traits<short>;
  template <> struct traits<signed char>;
  template <> struct traits<unsigned int>;
  template <> struct traits<unsigned short>;
  template <> struct traits<unsigned char>;
  template <> struct traits<bool>;
        
}

  template <class T>
    struct is_ordinal;
  template <class T>
    struct is_ordinal<const T>;
  template <class T>
    struct is_ordinal<volatile T>;
  template <class T>
    struct is_ordinal<const volatile T>;    
  template <class T>
    inline constexpr bool is_ordinal_v;
}
}
}
```

#### Class `tag` [ordinal.tag]

Tag used to identify an ordinal.

####  Class Template `traits` [ordinal.traits]

```c++
namespace ordinal {
    template <class T, class Enabler=void>
        struct traits {};
} 
```

This traits must be specialized for a type to become an *Ordinal* type. 
The specialization must define the 3 customization functions `size`, `val` and `pos`.

In addition the specialization must inherit from `ordinal::tag`.

Next follow the archetype of this customization

```c++
namespace ordinal {
    template <>
    struct traits<O> : tag
    {
      using size_type = ...;
      static size_type size() noexcept;
      using size = integral_constant<size_type, S>;
      static O val(size_t p);
      static size_t pos(O u);
    };
}
```

where the specialization shall have the following semantics:

* `ordinal::traits<O>::size::value` or `ordinal::traits<O>::size()` should give the number of elements of the *Ordinal* type `O`.

* `ordinal::traits<O>::val(p)` should give the value at the position `p` in the *Ordinal* type `O`.

* `ordinal::traits<O>::pos(o)` should give the position of the value `o` in the *Ordinal* type `O`.


####  Function Template `size` [ordinal.size]

```c++
namespace ordinal {
  template <class Ordinal>
    constexpr size_t size() noexcept;
}
```

*Equivalent to*: `traits<Ordinal>::size` 

####  Function Template `val` [ordinal. val]

```c++
namespace ordinal {
  template <class Ordinal>
    constexpr Ordinal val(size_t pos) noexcept;
}
```

*Equivalent to*: `traits<Ordinal>::val(pos)` 

####  Function Template `pos ` [ordinal. pos]

```c++
namespace ordinal {
  template <class Ordinal>
    constexpr size_t pos(Ordinal&& val) noexcept;
}
```

*Equivalent to*: `traits<decay_t<Ordinal>>::pos(forward<Ordinal>(val))` 

####  Function Template `pos ` [ordinal. pos]

```c++
namespace ordinal {
  template <class Ordinal>
    constexpr size_t pos(Ordinal&& val) noexcept;
}
```

*Equivalent to*: `traits<decay_t<Ordinal>>::pos(forward<Ordinal>(val))` 


####  Function Template `first` [ordinal.first]

```c++
namespace ordinal {
  template <class Ordinal>
    constexpr Ordinal first() noexcept;
}
```

*Equivalent to*: `ordinal::val<Ordinal>(0)` 

####  Function Template `last` [ordinal.last]

```c++
namespace ordinal {
  template <class Ordinal>
    constexpr Ordinal last() noexcept;
}
```

*Equivalent to*: `ordinal::val<Ordinal>(ordinal::size<Ordinal>()-1)` 

####  Function Template `succ ` [ordinal.succ]

```c++
namespace ordinal {
  template <class Ordinal>
    constexpr decay_t<Ordinal> succ(Ordinal&& val) noexcept;
}
```

*Pre-condition*: `ordinal::pos(val)+1 < ordinal::size<decay_t<Ordinal>>()`.

*Equivalent to*: `ordinal::val<decay_t<Ordinal>>(ordinal::pos(val)+1)` 

####  Function Template `pred ` [ordinal.pred]

```c++
namespace ordinal {
  template <class Ord>
    constexpr decay_t<Ordinal> pred(Ordinal&& val) noexcept;
        
}
```

*Pre-condition*: `ordinal::pos(val) > 0`.

*Equivalent to*: `ordinal::val<Ordinal>(ordinal::pos(val)-1)` 


####  Template class `is_ordinal` [ordinal.is_ordinal]

```c++ 
template <class T>
    struct is_ordinal : is_base_of<ordinal::tag, ordinal::traits<T>> {};
template <class T>
    struct is_ordinal<const T> : is_ordinal<T> {};
template <class T>
    struct is_ordinal<volatile T> : is_ordinal<T> {};
template <class T>
    struct is_ordinal<const volatile T> : is_ordinal<T> {};
```

####  Template class `arithmetic_traits` [ordinal.arith]

```c++
namespace ordinal {
  template <class T, T Low, T High, T Step = T{1}, class SizeType=size_t>
    struct arithmetic_traits
    {
      static_assert(is_arithmetic<T>::value, "T must be arithmetic");

      using size_type = SizeType;
      using value_type = T;

      static constexpr size_type size_v = (size_type(High)-size_type(Low)+1u);
      using size = integral_constant<size_type, size_v>;
      static constexpr value_type val(size_type p) { return value_type{p*Step+Low}; }

      static constexpr size_type pos(value_type v)  { return static_cast<size_type>(v*Step-Low); };
    };
}
```

`arithmetic_traits` defines a mapping from a linear progression to `0`..`N`  where `val(n) = n*Step+Low`.

####  Template class `logarithmic_traits` [ordinal.arith]

```c++
namespace ordinal {
  template <class T, class SizeType=size_t>
    struct logarithmic_traits
```

`logarithmic_traits ` defines a mapping from a logarithmic progression to `0`..`N` where `val(n) = 2^n`.

####  Template class `numeric_traits` [ordinal.num]

```c++
namespace ordinal {
    template <typename T>
        struct integral_traits 
            : arithmetic_traits<T, numeric_limits<T>::min(),
                                   numeric_limits<T>::max()>
}
```
    
*Requires* `T` is an integral type that can be used in a constant expression.

[Note This class could be considered an implementation detail for the definition of the following traits specialization.]

####  Ordinal Traits specialization `traits<>` [ordinal.traits_spec]

```c++
  template <> struct traits<int>;
  template <> struct traits<short>;
  template <> struct traits<signed char>;
  template <> struct traits<unsigned int>;
  template <> struct traits<unsigned short>;
  template <> struct traits<unsigned char>;
  template <> struct traits<bool>;
```c++

Each one of the preceding traits is specialized using the `integral_traits` with the 

### Header <experimental/ordinal_array> synopsis [ordinal_array.synop]


### Header <experimental/ordinal_set> synopsis [ordinal_set.synop]


### Header <experimental/ordinal_range> synopsis [ordinal_range.synop]



# Implementability

This proposal can be implemented as pure library extension, without any language support, in C++17. See [Ordinal] for a poc.

# Open points

The authors would like to have an answer to the following points if there is any interest at all in this proposal:

* Is the customization approach acceptable?
* Are the names `size`, `pos`, `val`, `succ`, `pred` the correct ones?  
* Should small integral types be considered as *Ordinal*s?
 

# Future work

## Standard classes customization

Some standard classes can be seen as *Ordinal*, in particular enumerations that have all its enumerators different.

## Other classes

If classes as bounded integers are added to the standard, they could be seen as *Ordinal* types using the `arithmetic_traits`.

## Compile-time ordinal types

In addition to the run-type mapping, we could have a compile-time mapping.

## Reflection

Any enumeration having all its enumerators different can be seen as *Ordinal* types. A `reflection_traits` could be defined. The mapping could be implicit or explicit.


# Acknowledgements

Special thanks and recognition goes to Technical Center of Nokia - Lannion for supporting in part the production of this proposal.

# References

[N4564]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf "N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS"

[P0088R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf "Variant: a type-safe union that is rarely invalid (v5)"  

[p0338r1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0338r1.pdf "C++ generic factories"

[P0343R0]: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0343r0.pdf "Meta-programming High-Order functions"

 

* [N4564] N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf

* [P0088R0] Variant: a type-safe union that is rarely invalid (v5)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf

* [P0091R0] Template parameter deduction for constructors (Rev. 3)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0091r0.html
    
* [p0338r1] C++ generic factories

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0338r1.pdf

* [P0343R0] - Meta-programming High-Order functions

    http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0343r0.pdf

* [MONADS] ordinals, Applicatives and Monads

* [SUM_TYPE] Generic Sum Types

