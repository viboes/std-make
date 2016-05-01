**____________________________________ DRAFT ___________________________________**

<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">D0XXXR0</span></td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2016-05-01</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">ISO/IEC JTC1 SC22 WG21 Programming Language C++</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Audience:</td>
        <td width="435">Evolution Working Group / Library Evolution Working Group </td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escrib&aacute; &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@nokia.com</a>&gt;</td>
    </tr>
</table>


Structured binding: customization points issues
===============================================

# Abstract

This paper proposes two alternative approaches to cover with two possible issue on Structured binding [P0217R1] wording: [P0217R1] adds an additional dependency of the core language on the library file `<utility>` and that, to the authors knowledge, the user is unable to customize completely bitfields members.  

The first approach is to add additional wording to Structured binding [P0217R1] to cover with the core language dependency on the library file `<utility>`. 

The second one consists in changing the customization points of Structured binding to something more specific and related to product types: `product_type_size` and  `product_type_get`. These functions would be either members or non-members found by ADL so that we remove the dependency from the library file.

It is weird that Structured binding provides access to types that can not be customized by the user.


1. [Introduction](#introduction)
2. [Motivation](#motivation)
3. [Proposal](#proposal)
4. [Design Rationale](#design-rationale)
5. [Wording](#wording)
6. [Implementability](#implementability)
7. [Open points](#open-points)
8. [Future work](#future-work)
9. [Acknowledgements](#acknowledgements)
10. [References](#references)


# Introduction

This paper proposes two alternative approaches to cover with two possible issue on Structured binding [P0217R1] wording: [P0217R1] adds an additional dependency of the core language on the library file `<utility>` and that, to the authors knowledge, the user is unable to customize completely bitfields members.  

The first approach is to add additional wording to Structured binding [P0217R1] to cover with the core language dependency on the library file `<utility>`. 

The second one consists in changing the customization points of Structured binding to something more specific and related to product types: `product_type_size` and  `product_type_get`. These functions would be either members or non-members found by ADL so that we remove the dependency from the library file.

The wording has been modified so that both structured binding and the possible product type access [PT]  wording doesn't repeat themselves. An alternative could be to preserve the structure of the current wording [P0217R1] and let the refactoring of the wording to a future product type access proposal [PT].

# Motivation

There are two issues.

## Dependent on library

[P0217R1] makes the language dependent on the customization point `std::tuple_size<T>`, which is defined in the library file `<utility>`. This file is not part of the freestanding implementation. We should avoid this kind of dependency as much as possible.

Suppose we want to do a customized the following class

```c++
class S { 
    int x, y; 
public:
    // ...    
};
```

[P0217R1] says you must specialize `std::tuple_size<S>` and `std::tuple_element<i,E>` for your own `get()` to work.

Fine so far, except that you can only specialize a template if you've seen the declaration of the primary template.  Users can't declare the primary template themselves (it's in namespace std), so they
need to include the correct header, which happens to be `<utility>`.

However, `<utility>` is not required to be available in a freestanding implementation (see 17.6.1.3 table 16).

Net result: You can't (portably) use structured bindings with customized `get()` in a freestanding implementation. 

Adopting [P0144R2] would at least mean that we need to change the freestanding implementation requirements. While the authors feel that this may be a tenable direction (and provide wording to fix this), we would strongly prefer that the committee considers our proposed alternate design which would address these issues more cleanly.

### Alternative customization approach

Changing the customization point would mean a change on the definition of *tuple-like* access or more precisely that Structured binding is not based on *tuple-like* access.

#### Independence from library

In order to overcome the library dependency we need to find a way to avoid the use of `std::tuple_size<T>`. We have 3 possibilities:

* A non-member function `product_type_size` that must be found by ADL.
* A member function `product_type_size`.
* Use `product_type_get<N>(tpl)` as customization point and deduce the tuple size as `N` for which `product_type_get<I>(tpl)` is well defined for any `I` in `0..N(-1)` and `product_type_get<N>(tpl)` is not defined.

##### non-member function `product_type_size`

We could think of a non-member function `product_type_size` that must be found by ADL. However `product_type_size<T>()` couldn't be found by ADL. We need to add the type on as a parameter. 

We could look for `product_type_size(T)` but this would be restricted to copyable types.
We could look for `product_type_size(T const&)` but this would accept types inheriting from `T`.
We could use a nullary function that returns a pointer to `T` and look for `product_type_size(T*(*)())`. This has the advantage that it doesn't accept derived types and works for any type.

##### member function `product_type_size`

This seems much simpler. However it prevents the customization of 3PP classes.

##### non-member function `product_type_get<I>(tpl)`

This also seems much simpler, but determining the size could be expensive at compile time.

##### A combination of all the previous

Let the user define member or non-member functions of `product_type_size` and `product_type_get`.

We consider that the user has customized his class when 

* either `PT::product_type_size()` or `product_type_size([]()->PT* {return nullptr})` and the result is `N` and `pt.product_type_get<I>()` or `product_type_get<I>(pt)` is well defined for all `I` in `0..(N-1)`,
* either the member or non-member functions for `product_type_size` and the `pt.product_type_get<I>()` or `product_type_get<I>(pt)` is well defined for all `I` in `0..(N-1)` and `pt.product_type_get<N>()` or `product_type_get<N>(pt)` is not well defined,

## Ability to work with bitfields only partially

[P0217R1] wording make use of `std::tuple_element` to define the type of the elements of the lvalues. 

> Given the type `Ti` designated by `std::tuple_element<i-1,E>::type`, each `vi` is a variable of type "reference to `Ti`" initialized with the initializer, where the reference is an lvalue reference if the initializer is an lvalue and an rvalue reference otherwise; the referenced type is `Ti`. 

This works well as far as the type returned by `get<I,e>` is a reference to `std::tuple_element_t<I,E>`.

However, when a user that wants to customize a class with bitfields members would need to define `std::tuple_element` for this bitfield member is some `bitfield_ref` class. However, we this is not the   real type of the member. This would mean that the the function `get`should return a reference to this `bitfield_ref`, but the `bitfield_ref` returned by get can not be reference as it should be an rvalue.. 

Wondering if instead of using a reference to `std::tuple_element<I,e>` the type should be the type returned by `get<I>(pt)`.

```c++
template <size_t I, class X>
class bitfield_ref;

struct X3 {
  unsigned i:2;
  int j:5;
  int k;
public:

};

template <>
class bitfield_ref<0,X3> {
  X3& x;
public:
  bitfield_ref(X3& x) : x(x) {}
  operator unsigned() const { return x.i; }
  bitfield_ref& operator=(int v)
  {
    x.i = v;
    return *this;
  }
};
template <>
class bitfield_ref<1,X3> {
  X3& x;
public:
  bitfield_ref(X3& x) : x(x) {}
  operator int() const { return x.j; }
  bitfield_ref& operator=(int v)
  {
    x.j = v;
    return *this;
  }
};
// Something similar for const& and &&, but without assignment
// ...
namespace std {
  template <>
  class tuple_size<X3> : integral_constant<size_t, 3> {};
  template <>
  class tuple_element<0,X3> { public: using type = bitfield_ref<0, const X3>; };
  template <>
  class tuple_element<1,X3> { public: using type = bitfield_ref<1, const X3>; };
  template <>
  class tuple_element<2,X3> { public: using type = int; };
}
bitfield_ref<0, X3> get_element(std::integral_constant<size_t, 0>, X3 & x) {
  return bitfield_ref<0, X3>(x);
}
bitfield_ref<1, X3> get_element(std::integral_constant<size_t, 1>, X3 & x) {
  return bitfield_ref<1, X3>(x);
}
int& get_element(std::integral_constant<size_t, 2>, X3 & x) {
  return x.k;
}
// Something similar for const& and &&
// ...

```

Given

```c++
    X3 x {0,1,2};
```

the following couldn't compile for a bitfield member

```c++
      auto &xi = get<0>(x);
```

while the following compiles for a non bitfield member

```c++
      auto &xk = get<2>(x);
```



# Proposal

Taking into consideration these points, this paper proposes two alternative proposals for each one of the points. 

We name *product type* the types covered by Structured binding.

[PT] is an extension paper to this proposal that includes *product type* access library interface.

## Alternative proposal 1.1

Let the core language depend on an additional library file and add this file to the freestanding implementation.

Currently the traits `std::tuple_size` and `std::tuple_element` are defined in the `<utility>` file. In order to reduce the freestanding implementation constraints, we proposes to move these traits to a `<tuple_like>` file.

## Alternative proposal 1.2

Change the customization points of Structured binding to something more specific and related to product types: `product_type_size` and  `product_type_get` either as members or non-members functions found by ADL so that we remove the dependency from the library file.

## Alternative proposal 2.1

Status quo: Let Structured binding be able to manage with bitfield members in case 3 even if the user is unable to customize them and that we will be unable to define a function interface to access [PT] all types supported by Structured binding. 

## Alternative proposal 2.2

Don't support bitfield members in any case.

# Design Rationale

## Why the language core shouldn't depend on the library files?

The current C++ standard depends already on the library files at least for `<initializer_list>`. Adding more dependencies will open the door to more dependencies. This makes the freestanding implementations more library dependent.

## What do we loss by changing the current customization point?

There are not too much classes providing a *tuple-like* access on the standard and they can be adapted easily. However we don't know on the user side. 

## What do we gain by changing the current customization point?

We don't increase the dependency of the core language on the library.

The current *tuple-like* access `tuple_size`/`tuple_element`/`get` has a customization point `get` that is used also for other types that don't provide a *tuple-like* access. There is no real problem as the other customization points are more specific.

Adopting the *product type* customization  points `product_type_size`/`product_type_get`  are more explicit and in line with *product type* access [PT]. 

## What do we loss if Structured binding is not able to bind to bitfield members?

There is not too much we loss. The user can always nest the bitfields on a nested structure and use the bitfield access.

Instead of

```c++
class X3 {
  unsigned i:2;
  int j:5;
  int k;
  // friend declarations as needed
public:
  ...
};
```

the user can define 

```c++
class X3 {
public:
  struct IJ {
    unsigned i:2;
    int j:5;
  };
private: 
  IJ ij;
  int k;
  // friend declarations as needed
public:
  ...
};
```

## What do we gain if Structured binding don't support bitfield members?

We can not take the address of a bitfield, have a pointer to bitfield member function, we don't know how to return references to bitfield members, ...

Don't supporting them make it possible to have a uniform interface. We would be able to name the exact type for any element and have access to the element via a reference to the member data. 

If a uniform solution is found later on that support bitfields references we could always update the proposal in a backward compatible way.

# Wording 

## Alternative 1.1

**Add a new `<utility>` file in 17.6.2.2 Headers [using.headers] Table 16**

**Add the following to `[utility]` Header <utility> synopsis** 

```c++
namespace std {
    template <class T> class tuple_size<const T>; 
    template <class T> class tuple_size<volatile T>; 
    template <class T> class tuple_size<const volatile T>;
        template <size_t I, class T> class tuple_element<I, const T>; 
    template <size_t I, class T> class tuple_element<I, volatile T>; 
    template <size_t I, class T> class tuple_element<I, const volatile T>;    
}
```


## Alternative 1.2

**Add a new `<tuple_like>` file in 17.6.1.2 Headers [headers] Table 14**

**Add a section Tuple like Objects in 20**

** 20.X Tuple like Objects**

Header <tuple_like> synopsis

The header <tuple_like> defines the tuple-like traits.

```c++
namespace std {
    template <class T> class tuple_size; // undefined
    template <class T> class tuple_size<const T>; 
    template <class T> class tuple_size<volatile T>; 
    template <class T> class tuple_size<const volatile T>;
        template <size_t I, class T> class tuple_element; // undefined
    template <size_t I, class T> class tuple_element<I, const T>; 
    template <size_t I, class T> class tuple_element<I, volatile T>; 
    template <size_t I, class T> class tuple_element<I, const volatile T>;    
}
```

```c++
template <class T> struct tuple_size;
```
*Remarks*: All specializations of `tuple_size<T>` shall meet the *UnaryTypeTrait* requirements (20.10.1) with a *BaseCharacteristic* of `integral_constant<size_t, N>` for some `N`.


```c++
template <class T> class tuple_size<const T>;template <class T> class tuple_size<volatile T>;template <class T> class tuple_size<const volatile T>;
```
Let `TS` denote `tuple_size<T>` of the cv-unqualified type `T`. Then each of the three templates shall meet the `UnaryTypeTrait` requirements (20.10.1) with a `BaseCharacteristic` of  `integral_constant<size_t, TS::value>`In addition to being available via inclusion of the `<tuple_like>` header, the three templates are available when either of the headers `<array>` or `<utility>` or `tuple` are included.

```c++
    template <size_t I, class T> class tuple_element; // undefined
```

*Remarks*: `std::tuple_element<I,T>::type` shall be defined for all the `I` in `0..(std::tuple_size<T>::value-1)`.

```c++
template <size_t I, class T> class tuple_element<I, const T>;template <size_t I, class T> class tuple_element<I, volatile T>;template <size_t I, class T> class tuple_element<I, const volatile T>;
```
Let `TE` denote `tuple_element<I, T>` of the cv-unqualified type `T`. Then each of the three templates shall meet the `TransformationTrait` requirements (20.10.1) with a member typedef type that names the following type:
* for the first specialization, `add_const_t<TE::type>`,* for the second specialization, `add_volatile_t<TE::type>`, and* for the third specialization, `add_cv_t<TE::type>`.
In addition to being available via inclusion of the <tuple_like> header, the three templates are available when either of the headers <array> or <utility>  or <tuple> are included.


**Extract the following from `[utility]` Header <utility> synopsis**


```c++
    template <class T> class tuple_size;    template <size_t I, class T> class tuple_element;
```

**Add the following to `[utility]` Header <utility> synopsis**

```c++
#include <tuple_like>
```

**Extract the following from `[tuple.general]` Header <utility> synopsis**


```c++
    template <class T> class tuple_size; // undefined
    template <class T> class tuple_size<const T>; 
    template <class T> class tuple_size<volatile T>; 
    template <class T> class tuple_size<const volatile T>;
        template <size_t I, class T> class tuple_element; // undefined
    template <size_t I, class T> class tuple_element<I, const T>; 
    template <size_t I, class T> class tuple_element<I, volatile T>; 
    template <size_t I, class T> class tuple_element<I, const volatile T>;    
```


**Add the following to `[tuple.general]` Header <tuple> synopsis**

```c++
#include <tuple_like>
```

**Rename 20.4.2.5 Tuple helper classes as Tuple Tuple-like configuration.** 

**Remove from 20.4.2.5 the definition for tuple_size and tuple_element 0 3,4, 5 and 6** 


**Add a new `<tuple_like>` file in 17.6.2.2 Headers [using.headers] Table 16**

## Wording Alternative 2

**Add the following Product type section**

**Product types terms**

A type `E` is a *product type* if the following terms are well defined.

*product type size*

* If E is an array type with element type T, equal to the number of elements of E.
* Else, if the expression `e.product_type_size()` is a well-formed integral constant expression, equal to `e.product_type_size()`.
* Else, if all of E's non-static data members and bit-fields shall be public direct members of E or of the same unambiguous public base class of E, E shall not have an anonymous union member, equal to the number of non-static data members of E. 
* Else it is undefined.

*product type i <sup>th</sup>-element* 

* If the *product type size* of `E` is defined and `i` is less than the *product type size* of `E`.
    * If `E` is an array type with element type `T`, equal to `e[i]`.
    * Else, if the expression `e.product_type_size()` is a well-formed integral constant expression, equal to the following: The unqualified-id `product_type_get` is looked up in the scope of `E` by class member access lookup (3.4.5 [basic.lookup.classref]), and if that finds at least one declaration, the value is `e.product_type_get<i-1>()`. Otherwise, the value is `product_type_get<i-1>(e)`, where `product_type_get` is looked up in the associated namespaces (3.4.2 [basic.lookup.argdep]). [ Note: Ordinary unqualified lookup (3.4.1 [basic.lookup.unqual]) is not performed. -- end note ].
    * else, if all of `E`'s non-static data members and bit-fields shall be public direct members of `E` or of the same unambiguous public base class of `E`, `E` shall not have an anonymous union member, equal to  `e.mi` where `i`-th non-static data member of `E` in declaration order is designated by `mi`. 
    * Else it is undefined.
* Else it is undefined.


**In 7.1.6.4 [dcl.spec.auto] paragraph 8 of the Structured Binding proposal, replace**

If `E` is an array type with element type `T`, the number of elements in the identifier-list shall be equal to the number of elements of `E`. Each `v`<sub>`i`</sub> is the name of an lvalue that refers to the element `i-1` of the array and whose type is `T`; the referenced type is `T`. [ Note: The top-level cv-qualifiers of `T` are cv. -- end note ]

Otherwise, if the expression `std::tuple_size<E>::value` is a well-formed integral constant expression, the number of elements in the identifier-list shall be equal to the value of that expression. The unqualified-id `get` is looked up in the scope of `E` by class member access lookup (3.4.5 [basic.lookup.classref]), and if that finds at least one declaration, the initializer is `e.get<i-1>()`. Otherwise, the initializer is `get<i-1>(e)`, where `get` is looked up in the associated namespaces (3.4.2 [basic.lookup.argdep]). [ Note: Ordinary unqualified lookup (3.4.1 [basic.lookup.unqual]) is not performed. -- end note ] In either case, `e` is an lvalue if the type of the entity `e` is an lvalue reference and an xvalue otherwise. Given the type `T`<sub>`i`</sub> designated by `std::tuple_element<i-1,E>::type`, each `v`<sub>`i`</sub> is a variable of type "reference to `T`<sub>`i`</sub>" initialized with the initializer, where the reference is an lvalue reference if the initializer is an lvalue and an rvalue reference otherwise; the referenced type is `T`<sub>`i`</sub>.

Otherwise, all of `E`'s non-static data members and bit-fields shall be public direct members of `E` or of the same unambiguous public base class of `E`, `E` shall not have an anonymous union member, and the number of elements in the identifier-list shall be equal to the number of non-static data members of `E`. The `i`-th non-static data member of `E` in declaration order is designated by `m`<sub>`i`</sub>. Each `v`<sub>`i`</sub> is the name of an lvalue that refers to the member `m`<sub>`i`</sub> of `e` and whose type is cv `T`<sub>`i`</sub>, where `T`<sub>`i`</sub> is the declared type of that member; the referenced type is cv `T`<sub>`i`</sub>. The lvalue is a bit-field if that member is a bit-field.

**with**

The number of elements in the identifier-list shall be equal to *product type size of `E`*.

Each `v`<sub>`i`</sub> is the name of an lvalue that refers to *product type `i`<sup>th</sup>-element of `e`*.

**Add the associated customization in `[tuple.tuple]`**

**Class template tuple**


```c++
    ...
    constexpr size_t product_type_size() { return sizeof...(Ts); };
    template <size_t I>
    constexpr auto product_type_get();
    template <size_t I>
    constexpr auto product_type_get() const;
    template <size_t I>
    constexpr auto product_type_get() &&;
    template <size_t I>
    constexpr auto product_type_get() const &&;
```

**std::array**

```c++
template <class T, size_t N>
class array {
    ...
    constexpr size_t product_type_size() { return N; };
    template <size_t I>
    constexpr auto product_type_get();
    template <size_t I>
    constexpr auto product_type_get() const;
    template <size_t I>
    constexpr auto product_type_get() &&;
    template <size_t I>
    constexpr auto product_type_get() const &&;
};
```


# Implementability

There is no implementation as of the date of this paper.

# Open Questions

The authors would like to have an answer to the following points if there is any interest at all in this proposal:

* Do we want the core language depend on the file library?
* If yes, do we prefer to move to a `<tuple_like>` file?
* If not, 
* Do we want the proposed customization points?
* Do we want customization points for *product type* size to be optional?

# Future work

## Extend the default definition to aggregates

With [P0017R1] we have now that we can consider classes with non-virtual public base classes as aggregates. 
[P0197R0] considers the elements of the base class as elements of the *tuple-like* type.
I would expect that all the aggregates can be seen as *tuple-like* types, so we need surely to consider this case in [P0217R1] and [P0197R0].

We should see aggregate initialization and structured binding almost as inverse operations.

This could already be the case for predefined *tuple-like* types which will have aggregate initialization. However user defined *tuple-like* types would need to define the corresponding constructor.

# Acknowledgments

Thanks to Jens Maurer, Matthew Woehlke and Tony Van Eerd for their comments in private discussion about structured binding and product types.

Thanks to all those that have commented the idea of a tuple-like generation on the std-proposals ML better helping to identify the constraints, in particular to J. "Nicol Bolas" McKesson, Matthew Woehlke and Tim "T.C." Song.

# References

[Boost.Fusion]: http://www.boost.org/doc/libs/1_60_0/libs/fusion/doc/html/index.html "Boost.Fusion 2.2 library"

[Boost.Hana]: http://boostorg.github.io/hana/index.html "Boost.Hana library"

[N4381]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4381.html "Suggested Design for Customization Points"
   

[N4387]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4387.html "Improving pair and tuple, revision 3"

[N4428]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4428.pdf "Type Property Queries (rev 4)"

[N4451]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4451.pdf "Static reflection"

[N4475]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4475.pdf "Default comparisons (R2)"

[N4527]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4527.pdf "Working Draft, Standard for Programming Language C++"

[N4532]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4532.html "Proposed wording for default comparisons"

[P0017R1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0017r1.html "Extension to aggregate initialization"

[P0091R1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0091r1.html "Template argument deduction for class templates (Rev. 4)"

[P0144R2]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0144r2.pdf "Structured Bindings"

[P0151R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0151r0.pdf "Proposal of Multi-Declarators"

[P0197R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0197r0.pdf "Default Tuple-like Access"

[P0217R1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0217r1.pdf "Proposed wording for structured bindings"   

[P0311R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0311r0.html

[DSPM]: http://davidsankel.com/uncategorized/c-language-support-for-pattern-matching-and-variants "C++ Language Support for Pattern Matching and Variants"

[PT]: https://github.com/viboes/std-make/blob/master/doc/proposal/reflection/ProductTypes.md "Product types access"

     

    

* [Boost.Fusion] Boost.Fusion 2.2 library

    http://www.boost.org/doc/libs/1_60_0/libs/fusion/doc/html/index.html

* [Boost.Hana] Boost.Hana library

    http://boostorg.github.io/hana/index.html

* [N4381] Suggested Design for Customization Points

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4381.html

* [N4387] Improving pair and tuple, revision 3

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4387.html

* [N4428] Type Property Queries (rev 4)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4428.pdf

* [N4451] Static reflection

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4451.pdf

* [N4475] Default comparisons (R2)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4475.pdf

* [N4527] Working Draft, Standard for Programming Language C++

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4527.pdf

* [N4532] Proposed wording for default comparisons

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4532.html

 
* [P0017R1] Extension to aggregate initialization

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0017r1.html

* [P0091R1] - Template argument deduction for class templates (Rev. 4)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0091r1.html

* [P0144R2] Structured Bindings

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0144r2.pdf

* [P0151R0] Proposal of Multi-Declarators

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0151r0.pdf

* [P0197R0] Default Tuple-like Access

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0197r0.pdf

* [P0217R1] Proposed wording for structured bindings

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0217r1.pdf

* [P0311R0] A Unified Vision for Manipulating Tuple-like Objects

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0311r0.html

* [DSPM] C++ Language Support for Pattern Matching and Variants

    http://davidsankel.com/uncategorized/c-language-support-for-pattern-matching-and-variants 

* [PT] Product types access

    https://github.com/viboes/std-make/blob/master/doc/proposal/reflection/ProductTypes.md 
