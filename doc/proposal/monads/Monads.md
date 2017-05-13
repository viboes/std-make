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
        <td width="435">Library Evolution Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escrib&aacute; &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@nokia.com</a>&gt;</td>
    </tr>
</table>

# C++ Monadic interface
=======================

**Abstract**



# Table of Contents

* [History](#history)
* [Introduction](#introduction)
* [Motivation and Scope](#motivation-and-scope)
* [Proposal](#proposal)
* [Design Rationale](#design-rationale)
* [Proposed Wording](#proposed-wording)
* [Implementability](#implementability)
* [Open points](#open-points)
* [Future work](#future-work)
* [Acknowledgements](#acknowledgements)
* [References](#references)

# History

## Revision 0

Creation in response to request of the committee to split the expected proposal [P0323R0] into a expected class [P0323R0] and a monadic interface (this document).


# Introduction

Most of you know *Functor*, *Applicative*, *Monad* and *MonadError* from functional programming. The underlying types of the types modeling *Functor*, *Applicatives*, *Monad* and *MonadError* are homogeneous, that is, the functions have a single type. 

In the following notation `[T]` stands for a type wrapping instances of a type `T`, possibly zero or `N` instances. `(T -> U)` stands for a function taking a `T` as parameter and returning a `U`.

```
functor::transform : [T] x (T->U) -> [U]

applicative::ap : [T] x [(T->U)] -> [U]
applicative::make<A> : T -> [T]

monad::chain : [T] x (T->[U]) -> [U] //mbind
monad::make<M> : T -> [T] // mreturn

monad_error::make_error<M>: E -> error_type_t<M,E>
monad_error::catch_error: [T] x (E->T) -> [T] where E = error_type_t<[T]> 
monad_error::catch_error: [T] x (E->[T]) -> [T]
```

# Motivation and Scope

There are a lot of types that have nothing in common except that they can implement these very useful functional type of classes on top of which we can define very useful algorithms as Haskell and other functional language have proved.

# Proposal

This paper proposes to add the following type of classes with the associated customization points and the algorithms that work well with them.

* *Functor*, 
* *Applicative*
* *Monad*
* *Monad-Error*

These are the basic operations. More will come later if the committee adopts the design (See Future Work section).

# Design Rationale

## Functor

### `transform` versus `fmap`

The signature of the more C++ `transform` is different from the usual Functor `fmap` function.

`transform` has the advantage to be closer to the STL signature.
The advantage of the `fmap` is that it can be extended to a variadic number of *Functors*.

Both seem to be useful.

## Applicative

## Monad

### `chain`

C++ has the advantage to be able to overload on the parameters of the signature.

`chain` can be overloaded with functions that return a Monad or functions that return the *ValueType* as it proposed for `std::experimental::future::then`.

The authors don't see any inconvenient in this overload, but would prefer to have an additional function that supports this ability, so that we know that chain will only work with functions that return a *Monad*.

Note that the user could use `transform` and `chain`  to get this overload.

### `chain` function parameter parameter

The chain function accepts functions that take the `ValueType` as parameter.
`std::experimental::future::then` function parameter takes a `future`.

We could define `chain` in function of a possibly `then` function (or whatever is the appropriated name) when the type provides access to the `ValueType` as it is the case for `std::future` and all the *Nullable* types. However the authors don't know how to do it in the general case.


## Customization: ADL versus traits

These concepts have some functions that can not be customized using overload, as the the dispatching type is not a function parameters,e.g. `make<TC>(C)` or `make_error<TC>(E)`.

We have also some customization points that are types, as `error_type<T>::type`

The authors prefer to have a single customization mechanism, and trait the one that allows to do everything.

See [CUSTOM] where we describe the advantages and liabilities of each approach.

## Customization: All at once or one by one

Boost.Hana has chosen to customize each operation individually. The approach  of this proposal is closer to how other languages have addressed the problem, that is, customize all operations at once.

There are advantages and liabilities to both approaches. See [CUSTOM] where we describe the advantages and liabilities of each approach.


## About names

### transform versus fmap

### bind versus chain

# Impact on the standard

## Customization

This paper is based on an alternative customization approach [CUSTOM]. While this is not an imperative this approach helps to define such concepts. 

## Factory functions

Both *Applicative* and *Monad* have factory function `applicative::make` and `monad::make`. We have already such a factory function isolated in the *Factory* concept via `factory::make`.

We could define those specific factory functions but functions that forward to the `factory::make` function, but there is not too much interest in duplicating such factories. We can just nest the factory namespace in `applicative` meaning that any `Applicative` is a `Factory`.

# Impact on the standard

These changes are entirely based on library extensions and do not require any language features beyond what is available in C++17. There are however some classes in the standard that need to be customized.

This paper depends in some way on the helper classes proposed in [P0343R0], as e.g. the place holder `_t` and the associated specialization for the type constructors `optional<_t>`, `unique_ptr<_t>`, `shared_ptr<_t>`.

# Proposed Wording

The proposed changes are expressed as edits to [N4564] the Working Draft - C++ Extensions for Library Fundamentals V2.

**Add a "Functor Types" section**

## Functor Types

### *Functor* requirements

A *Functor* is a type constructor that supports the `transform` function. A type constructor `TC` meets the requirements of *Functor* if:

* `TC` is a *TypeConstructor* 
* for any T *EqualityComparable* *DefaultConstructible*, and *Destructible*, `invoke_t<TC,T>` satisfies the requirements of *EqualityComparable* *DefaultConstructible*, and *Destructible*,
* the expressions shown in the table below are valid and have the indicated semantics, and
* `TC` satisfies all the other requirements of this sub-clause.

In Table X below, `t` denotes an rvalue of type `invoke<TC,T>`, `f` denotes a  rvalue of type `F` where `F` satisfies *Callable<F,U(T)>*.

<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="850">
    <tr>
        <td align="left" valign="top"> <b>Expression</b> </td>
        <td align="left" valign="top"> <b>Return Type</b> </td>
        <td align="left" valign="top"> <b>Operational Semantics</b> </td>
    </tr>
    <tr>
        <td align="left" valign="top"> invoke_t&lt;TC, VT...> </td>
        <td align="left" valign="top"> T </td>
        <td align="left" valign="top">  </td>
    </tr>
    <tr>
        <td align="left" valign="top"> type_constructor_t&lt;T> </td>
        <td align="left" valign="top"> TC </td>
        <td align="left" valign="top">  </td>
    </tr>
    <tr>
        <td align="left" valign="top"> functor::transform(t, f) </td>
        <td align="left" valign="top"> invoke_t&lt;TC,U> </td>
        <td align="left" valign="top"> Applies `f` to the contents of `t` if any.</td>
    </tr>

</table>

### Header <experimental/functor> synopsis [functor.synop]

```c++
namespace std {
namespace experimental {
inline namespace fundamentals_v3 {
namespace functor {

  struct tag{};

  // class traits
  template <class TC, class Enabler=void>
    struct traits {};

  template <class T, class F>
    `see below` transform(T&& x, F&& f);
}

  template <class T>
    struct is_functor;
  template <class T>
    inline constexpr bool is_functor_v = is_functor<T>::value;
  template <class T>
    struct is_functor<const T> : is_functor<T> {};
  template <class T>
    struct is_functor<volatile T> : is_functor<T> {};
  template <class T>
    struct is_functor<const volatile T> : is_functor<T> {};
}
}
}
```

### Class `tag` [functor.tag]

A tag used to identify the *functor* types. The customization of the `traits<T>` must inherit from this tag.

###  Class Template `traits` [functor.traits]

```c++
namespace functor {
    template <class T, class Enabler=void>
        struct traits {};
} 
```

*Remark* The `Enabler` parameter is a way to allow conditional specializations.

###  Function Template `transform` [functor.transform]

```c++
namespace functor {
  template <class T, class F>
    auto transform(T&& x, F&& f)
}
```

Let `TC` be `type_constructor<decay_t<T>>`  

*Effects*: forward the call to the `traits<TC>::transform` 
*Remark*: The previous function shall not participate in overload resolution unless:
 
* `T` has a type constructor `TC` that satisfies *Functor*, 
* `F` is a *Callable* taking as parameter the `ValueType` of `T` and result `U`,
* The result of `transform` is the rebinding of `T` with the result of the invocation of `f` with the value of `x`.

```
transform : [T] x T->U -> [U]
```


###  Template class `is_functor` [functor.is_functor]

```c++
  
  template <class T>
    struct is_functor : is_base_of<functor::tag, functor::traits<T>> {};
```

**Add a "Applicative Types" section**

## Applicative Type

### *Applicative* requirements

A *Applicative* is a type constructor that supports the *Functor* requirements and supports the `ap` function. A type constructor `TC` meets the requirements of *Applicative* if:

* `TC` is a *Functor* 
* the expressions shown in the table below are valid and have the indicated semantics, and
* `TC` satisfies all the other requirements of this sub-clause.

In Table X below, `a` denotes an rvalue of type `invoke<TC,T>`, `f` denotes a  rvalue of type `invoke<TC,T>` where `F` satisfies *Callable<F,U(T)>*.

<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="850">
    <tr>
        <td align="left" valign="top"> <b>Expression</b> </td>
        <td align="left" valign="top"> <b>Return Type</b> </td>
        <td align="left" valign="top"> <b>Operational Semantics</b> </td>
    </tr>
    <tr>
        <td align="left" valign="top"> invoke_t&lt;TC, VT...> </td>
        <td align="left" valign="top"> T </td>
        <td align="left" valign="top">  </td>
    </tr>
    <tr>
        <td align="left" valign="top"> type_constructor_t&lt;T> </td>
        <td align="left" valign="top"> TC </td>
        <td align="left" valign="top">  </td>
    </tr>
    <tr>
        <td align="left" valign="top"> applicative::ap(a, f) </td>
        <td align="left" valign="top"> rebind_t&lt;TC,U> </td>
        <td align="left" valign="top"> Applies the contents of `f` to the contents of `a` if both are present.</td>
    </tr>

</table>

### Header <experimental/functor> synopsis [functor.synop]

```c++
namespace std {
namespace experimental {
inline namespace fundamentals_v3 {
namespace applicative {
    using namespace functor;

  struct tag{};

  // class traits
  template <class TC, class Enabler=void>
    struct traits {};

  template <class A, class F>
    `see below` ap(A&& x, F&& f);
}

  template <class T>
    struct is_applicative;
  template <class T>
    inline constexpr bool is_applicative_v = is_applicative<T>::value;
  template <class T>
    struct is_applicative<const T> : is_applicative<T> {};
  template <class T>
    struct is_applicative<volatile T> : is_applicative<T> {};
  template <class T>
    struct is_applicative<const volatile T> : is_applicative<T> {};
}
}
}
```

### Class `tag` [applicative.tag]

A tag used to identify the *applicative* types. The customization of the `traits<T>` must inherit from this tag.

###  Class Template `traits` [functor.traits]

```c++
namespace functor {
    template <class T, class Enabler=void>
        struct traits {};
} 
```

*Remark* The `Enabler` parameter is a way to allow conditional specializations.

###  Function Template `transform` [functor.transform]

```c++
namespace applicative {
  template <class A, class F>
    auto ap(A&& x, F&& f)
}
```

Let `TC` be `type_constructor<decay_t<A>>`  

*Effects*: forward the call to the `traits<TC>::ap`. 
*Remark*: The previous function shall not participate in overload resolution unless:
 
* `A` has a type constructor `TC` that satisfies *Applicative*, 
* `F` has a type constructor `TC` that satisfies *Applicative*, 
* `value_type_t<F>` is a *Callable* taking as parameter the `ValueType` of `T` and result `U`,
* The result of `ap` is the rebinding of `T` with the result of the invocation of the contents of `f` with the value of `x`.

```
ap : [T] x [T->U] -> [U]
```


###  Template class `is_applicative` [applicative.is_applicative]

```c++
  
  template <class T>
    struct is_applicative : is_base_of<applicative::tag, applicative::traits<T>> {};
```


**Add a "Monad Types" section**

## Monad Types

### *Monad* requirements

A *Monad* is a type constructor that in addition to supporting *Applicative*  supports the `chain` function. A type constructor `TC` meets the requirements of *Monad* if:

* `TC` is an *TypeConstructor* 
* for any T *EqualityComparable* *DefaultConstructible*, and *Destructible*, `invoke_t<TC,T>` satisfies the requirements of *EqualityComparable* *DefaultConstructible*, and *Destructible*,
* the expressions shown in the table below are valid and have the indicated semantics, and
* `TC` satisfies all the other requirements of this sub-clause.

In Table X below, `m` denotes an rvalue of type `invoke<TC,T>`, `f` denotes a *Callable* rvalue of type `F`.
In Table X below, `m` denotes an rvalue of type `invoke<TC,T>`, `f` denotes a  rvalue of type `F` where `F` satisfies *Callable<F,invoke<TC,U>(T)>*.


<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="850">
    <tr>
        <td align="left" valign="top"> <b>Expression</b> </td>
        <td align="left" valign="top"> <b>Return Type</b> </td>
        <td align="left" valign="top"> <b>Operational Semantics</b> </td>
    </tr>
    <tr>
        <td align="left" valign="top"> invoke_t&lt;TC, VT...> </td>
        <td align="left" valign="top"> T </td>
        <td align="left" valign="top">  </td>
    </tr>
    <tr>
        <td align="left" valign="top"> type_constructor_t&lt;T> </td>
        <td align="left" valign="top"> TC </td>
        <td align="left" valign="top">  </td>
    </tr>
    <tr>
        <td align="left" valign="top"> monad::chain(m, f) </td>
        <td align="left" valign="top"> invoke_t&lt;TC,U> </td>
        <td align="left" valign="top"> Applies `f` to the contents of `m` if any. Otherewise it return a monad without a value.</td>
    </tr>

</table>

### Header <experimental/monad> synopsis [monad.synop]

```c++
namespace std {
namespace experimental {
inline namespace fundamentals_v3 {
namespace monad {
  using namespace applicative;

  struct tag{};

  // class traits
  template <class TC, class Enabler=void>
    struct traits {};

  template <class T, class F>
    `see below` chain(T&& x, F&& f);
}

  template <class T>
    struct is_monad;
  template <class T>
    inline constexpr bool is_monad_v = is_monad <T>::value;
  template <class T>
    struct is_monad<const T> : is_monad<T> {};
  template <class T>
    struct is_monad<volatile T> : is_monad<T> {};
  template <class T>
    struct is_monad<const volatile T> : is_monad<T> {};
}
}
}
```

### Class `tag` [monad.tag]

A tag used to identify the *monad* types. The customization of the `traits<T>` must inherit from this tag.

###  Class Template `traits` [monad.traits]

```c++
namespace monad {
    template <class T, class Enabler=void>
        struct traits {};
} 
```

*Remark* The `Enabler` parameter is a way to allow conditional specializations.

###  Function Template `transform` [monad.chain]

```c++
namespace monad {
  template <class M, class F>
    auto chain(M&& x, F&& f)
}
```

Let `TC` be `type_constructor<decay_t<M>>`  
Let `T` be `value_type<decay_t<M>>`  

*Effects*: forward the call to the `traits<TC>::chain`. This function must return the result of calling to the the `f` parameter with the contained value type, if any; Otherwise it must return a monad of the same type that `F` returns without a value type.  
*Remark*: The previous function shall not participate in overload resolution unless:
 
* `M` has a type constructor `TC` that satisfies *monad*, 
* `F` satisfies `Callable<F, invoke_t<TC,U>(T)>` where `T` is the `ValueType` of `M` for some type `U`,
* The result of `chain` is the result of the invocation of `f` with the value of `x` if any, otherwise an `invoke_t<TC,U>(T)` instance without a value.

```
chain : [T] x T->[U] -> [U]
```


###  Template class `is_monad` [monad.is_monad]

```c++
  
  template <class T>
    struct is_monad : is_base_of<monad::tag, monad::traits<T>> {};
```

**Add a "Monad Error Types" section**

## Monad Error Types

### *MonadError* requirements

A *MonadError* is a type constructor that in addition to supporting *Monad*  supports the `make_error` and the `catch_error` functions. A type constructor `TC` meets the requirements of *MonadError* if:

* `TC` is an *Monad* 
* the expressions shown in the table below are valid and have the indicated semantics, and
* `TC` satisfies all the other requirements of this sub-clause.

In Table X below, `m` denotes an rvalue of type `invoke<TC,T>`, `f` denotes a *Callable* rvalue of type `F`.
In Table X below, `m` denotes an rvalue of type `invoke<TC,T>`, `f` denotes a  rvalue of type `F` where `F` satisfies *Callable<F,invoke<TC,U>(T)>*.

<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="850">
    <tr>
        <td align="left" valign="top"> <b>Expression</b> </td>
        <td align="left" valign="top"> <b>Return Type</b> </td>
        <td align="left" valign="top"> <b>Operational Semantics</b> </td>
    </tr>
    <tr>
        <td align="left" valign="top"> invoke_t&lt;TC, VT...> </td>
        <td align="left" valign="top"> T </td>
        <td align="left" valign="top">  </td>
    </tr>
    <tr>
        <td align="left" valign="top"> type_constructor_t&lt;T> </td>
        <td align="left" valign="top"> TC </td>
        <td align="left" valign="top">  </td>
    </tr>
    <tr>
        <td align="left" valign="top"> error_type_t&lt;TC> </td>
        <td align="left" valign="top"> E </td>
        <td align="left" valign="top">  </td>
    </tr>
    <tr>
        <td align="left" valign="top"> monad_error<TC>::make_error(e) </td>
        <td align="left" valign="top"> Err </td>
        <td align="left" valign="top"> a instance of a type depending on error_type_t&lt;TC> that is convertible to any invoke_t<TC, T>.</td>
    </tr>
    <tr>
        <td align="left" valign="top"> monad_error::catch_error(m, f) </td>
        <td align="left" valign="top"> M </td>
        <td align="left" valign="top"> Applies f to the error of m if any. Otherwise it return m.</td>
    </tr>

</table>

### Header <experimental/monad_error> synopsis [monad_error.synop]

```c++
namespace std {
namespace experimental {
inline namespace fundamentals_v3 {
namespace monad {
  using namespace applicative;

  struct tag{};

  // class traits
  template <class TC, class Enabler=void>
    struct traits {};

  template <class T, class F>
    `see below` chain(T&& x, F&& f);
}

  template <class T>
    struct is_monad;
  template <class T>
    inline constexpr bool is_monad_v = is_monad <T>::value;
  template <class T>
    struct is_monad<const T> : is_monad<T> {};
  template <class T>
    struct is_monad<volatile T> : is_monad<T> {};
  template <class T>
    struct is_monad<const volatile T> : is_monad<T> {};
}
}
}
```

### Class `tag` [monad.tag]

A tag used to identify the *monad* types. The customization of the `traits<T>` must inherit from this tag.

###  Class Template `traits` [monad.traits]

```c++
namespace monad {
    template <class T, class Enabler=void>
        struct traits {};
} 
```

*Remark* The `Enabler` parameter is a way to allow conditional specializations.

###  Function Template `transform` [monad.chain]

```c++
namespace monad {
  template <class M, class F>
    auto chain(M&& x, F&& f)
}
```

Let `TC` be `type_constructor<decay_t<M>>`  
Let `T` be `value_type<decay_t<M>>`  

*Effects*: forward the call to the `traits<TC>::chain`. This function must return the result of calling to the the `f` parameter with the contained value type, if any; Otherwise it must return a monad of the same type that `F` returns without a value type.  
*Remark*: The previous function shall not participate in overload resolution unless:
 
* `M` has a type constructor `TC` that satisfies *monad*, 
* `F` satisfies `Callable<F, invoke_t<TC,U>(T)>` where `T` is the `ValueType` of `M` for some type `U`,
* The result of `chain` is the result of the invocation of `f` with the value of `x` if any, otherwise an `invoke_t<TC,U>(T)` instance without a value.

```
chain : [T] x T->[U] -> [U]
```


###  Template class `is_monad` [monad.is_monad]

```c++
  
  template <class T>
    struct is_monad : is_base_of<monad::tag, monad::traits<T>> {};
```



## Nullable Objects

**Add Specializations of *Functor*, *Applicative*, *Monad* and *MonadError*.**

Nullable objects can be seen as *Functor*, *Applicative*, *Monad* and *MonadError*.

```c++
namespace nullable {
  template <class N, class F>
  constexpr `see below` transform(N&& n, F&& f);

  template <class N, class F
  constexpr `see below` ap(F&& f, N&& n);

  template <class M, class F>
  constexpr `see below`
  chain(M&& m, F&& f);

  template <class M>
  constexpr `see below`
  make_error();
 
  template <class M, class F>
  constexpr `see below`
  catch_error(M&& m, F&& f);
            
  struct as_functor: functor::tag  {
    template <class T, class F>
      static constexpr auto transform(T&& x, F&& f) {
        return nullable::transform(forward<T>(x), forward<F>(f));
      }
  };
  struct as_applicative: applicative::tag {
    template <class T, class F>
      static constexpr auto ap(F&& f, T&& x) {
        return nullable::ap(forward<F>(f), forward<T>(x));
      }
  };  
  
  struct as_monad: monad::tag {
    template <class M, class F>
      static constexpr auto bind(M&& x, F&& f) {
        return nullable::bind(forward<M>(x), forward<F>(f));
      }
  };
  struct as_monad_error: monad_error::tag {
    template <class M, class F>
      static constexpr auto catch_error(M&& x, F&& f) {
        return nullable::catch_error(forward<M>(x), forward<F>(f));
      }
  };
} 
namespace functor {
    template <class N>
    struct traits<N, meta::when<
        is_nullable<N>::value && is_type_constructible<N>::value
    >> : nullable::as_functor {};
} 
namespace applicative {
    template <class N>
    struct traits<N, meta::when<
        is_nullable<N>::value && is_type_constructible<N>::value
    >> : nullable::as_applicative {};
}

namespace monad {
    template <class N>
    struct traits<N, meta::when<
        is_nullable<N>::value && is_type_constructible<N>::value
    >> : nullable::as_monad {};
}

namespace monad_error {
    template <class N>
    struct traits<N, meta::when<
        is_nullable<N>::value && is_type_constructible<N>::value
    >> : nullable::as_monad_error {};
}

```   
  

## Expected Objects

**Add Specialization of *expected* [expeced.object.monadic_spec]**.

```c++
namespace functor {
  template <class T, class E>
  struct traits<expected<T,E>> : functor::tag
  {
    template <class Expected, class F>
      static constexpr auto transform(Expected&& x, F&& f);
  };
} 
namespace applicative {
    template <class T, class E>
    struct traits<expected<T,E>> : applicative::tag
    {
        template <class Expected, class F>
        static auto ap(F&& f, Expected&& x);
    };
}
namespace monad {
    template <class T, class E>
    struct traits<expected<T,E>> : monad::tag
    {
        template <class M, class F>
        static constexpr auto bind(M&& x, F&& f);
    };
}
namespace monad_error {
    template <class T, class E>
    struct traits<expected<T,E>> : monad_error::tag
    {
        template <class M>
        using error_type = typename M::error_type;

        template <class M, class ...Xs>
        static constexpr auto make_error(Xs&& ...xs);

        template <class M, class F>
        static constexpr auto catch_error(M&& x, F&& f);
    };
}
```   


# Implementability

This proposal can be implemented as pure library extension, without any language support, in C++14.

# Open points

The authors would like to have an answer to the following points if there is any interest at all in this proposal:

* Do we want the proposed customization approach?
* Do we want separated proposals for each type class?
* Should a smart pointer (a pointer) be considered a *Functor*?
* Should a *Nullable* be considered a *MonadError*?
* Should `std::array`, `std::vector` be considered a *Functor*?
* Should a *Product Type* be considered a *Functor*, *Applicative*, *Monad* when all the elements have the same type? 

# Future work

## Add more algorithms

Based on what [Boost.Hana] and [Boost.Fusion] provides already, extend the basic functionality with useful algorithms.

### *Functor* algorithms

```
functor::adjust_if : [T] x (T->bool) x (T->U) -> [U]
functor::adjust : [T] x CT x (T->U) -> [U]
functor::fill : [T] x U -> [U]
functor::replace_if : [T] x (T->bool) x T -> [T]
functor::replace : [T] x CT x T -> [T]
```

### *Applicative* algorithms

```
applicative::lift : [T] x (T->bool) x (T->U) -> [U]
```

### *Monad* algorithms

```
monad::flatten : [[T]] -> [T] // unwrap
monad::compose : (B->[C]) x (A->[B])-> (A->[C])
monad::then : [[T]] -> [T] // do
monad::next : [T] x ([T]->U) -> [U] // then
monad::next : [T] x ([T]->[U]) -> [U]
```

## Add *N-Functor*, *N-Applicative*, *N-Monad*


Do we need *N-Functor*, *N-Applicative*, *N-Monad* that support *Product Type*?


## Allow alternative way to customize a type of classes

Some type of classes can be customized using different customization points. E.g. *Monad* can be customized by either defining `chain` or `flatten`. The other customization point been defined wit a default behavior.

## Add Transformers

Monadic type don't compose very well. We need some kind of transformer that facilitates the composition. See `Haskell Transformers`.

## See how to add `Haskell::Alternative` type class

# Acknowledgements

Thanks to Louis for his work on the monadic interface of [Boost.Hana].
 
Special thanks and recognition goes to Technical Center of Nokia - Lannion for supporting in part the production of this proposal.

# References

[N4564]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf "N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS"

[P0088R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf "Variant: a type-safe union that is rarely invalid (v5)"  

[P0323R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0323r0.pdf
"A proposal to add a utility class to represent expected monad (Revision 2)"

[P0323R1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0323r1.pdf
"A proposal to add a utility class to represent expected monad (Revision 2)"

[P0338R1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0338r1.pdf "C++ generic factories"

[P0343R0]: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0343r0.pdf "Meta-programming High-Order functions"

 

* [N4564] N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf

* [P0088R0] Variant: a type-safe union that is rarely invalid (v5)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf

* [P0323R0] A proposal to add a utility class to represent expected monad (Revision 2)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0323r0.pdf

* [P0323R1] A proposal to add a utility class to represent expected monad (Revision 3)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0323r1.pdf
   
* [P0338R1] C++ generic factories

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0338r1.pdf

* [P0343R0] - Meta-programming High-Order functions

    http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0343r0.pdf

* [MONADS] Functors, Applicatives and Monads

* [SUM_TYPE] Generic Sum Types

