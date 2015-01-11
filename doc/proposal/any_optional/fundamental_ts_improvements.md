<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXX</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2015-01-11</td>
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

Adding Coherency Between `any` and `optional<T>`
===============================================

# Introduction

This paper identifies some minor inconveniences in the design of `any` and `optional`, 
diagnoses them as owing to unnecessary asymmetry between those two classes, 
and proposes wording to eliminate the asymmetry (and thus the inconveniences). 

The identified issues are related to the last Fundamental TS proposal concerning mainly 
* coherency of functions that behave the same but that are named differently,
* emplace functions that are missing for `any` class,
* new emplace factories for `any` and `optional` classes.


# Motivation and Scope

The following incoherencies have been identified:
* `optional` provides in place construction while `any` requires a specific instance.

* `optional` provides emplace assignment while `any` requires a specific instance to be assigned. 

* `any` provides a `any::clear()` to unset the value while `optional` uses assignment from a `nullopt_t`.

* `optional` provides a `explicit bool` conversion while `any` provides an `any::empty` member function.

The C++ standard should be coherent for features that behave the same way on different types. 
Instead of creating specific issues, we have preferred to write a specific paper so that 
we can discuss of the whole view.

# Proposal

We propose to: 
* Add to class `none_t` and a `none` constexpr variable.

* Add to class `any`
  * a implicit conversion from none_t,
  * an in place forward constructor,
  * an emplace forward member function,
  * rename the `empty` function with an `explicit bool` conversion,
  * rename the `clear` member function to `reset`,
  * an assignment from `none_t`.

* Add to class `optional`
  * a `reset` member function.

# Design rationale

## `any` in_place constructor

`optonal<T>` in place constructor constructs implicitly a `T`. 

```c++
template <class... Args> 
constexpr explicit optional<T>::optional(in_place_t, Args&&... args);
```

In place construct for `any`can not have an implicit type `T`. We need a way to state explicitly which `T`must be constructed in place 

```c++
template <class T> struct type {};

template <class T, class ...Args>
any(type<T>, in_place_t, Args&& ...);
```

```c++
any(type<X>{}, in_place, v1, ..., vn);
```

## `any` emplace assignment constructor

`optonal<T>` emplace assignment emplace implicitly a `T`. 

```c++
template <class ...Args>
optional<T>::emplace(Args&& ...);
```

Emplace assignment for `any` can not have an implicit type `T`. We need a way to state explicitly which `T` must be emplaced. 

```c++
template <class T, class ...Args>
any::emplace(Args&& ...);
```

```c++
any a;
a.emplace<T>(v1, ..., vn);
```

## About `empty()`/`explicit operator bool()` member functions

`empty` is more associated to containers. We don't see neither `any` nor `optional` as container classes. 
For probably valued types (as are the smart pointers and optional) the standard uses explicit bool conversion instead.  
We consider `any` as a probably valued type.

## About `clear()/reset()` member functions

`clear` is more associated to containers. We don't see neither `any` nor `optional` as container classes. 
For probably valued types (as are the smart pointers) the standard uses `reset` instead.  

## About a `none_t` type implicitly convertible to `any` and `optional` 

An alternative to the clear member function would be to be able to assign a `none_t` to an `optional` and to an `any`. 
We could consider that a default instance of `any` contains an instance of a `none_t` type, as `optional` contains a `nullopt`.

The problem is that then `none` can be seen as an `optional` or an `any` and could result in posible ambiguity.

We think that this implicit conversion goes against the raison d'être of `nullptr` and that we need explicit factories/constants.

## About a `none_t` type implicitly convertible to only `any` 

Instead of an additional `none`, `any()` plays well the role. 
However, the authors think that using `none` is more explicit.

## Do we need an explicit `make_any` factory? 

`any` is not a generic type but a type erased type. `any` play the same role than a posible `make_any`.

This is way this paper doesn't propose a `make_any` factory. 

## About an `emplace_xxx` factory

However, we could consider an emplace_xxx factory that in place construct a `T`.

`optional<T>` and `any`can be in place constructed as follows:

```c++
optional<T> opt(in_place, v1, vn);
f(optional<T>(in_place, v1, vn));

any a(type<T>{}, in_place, v1, vn);
f(any(type<T>{}, in_place, v1, vn));
```

When we use auto things change a little bit

```c++
auto opt=optional<T>(in_place, v1, vn);
auto a=any(type<T>{}, in_place, v1, vn);
```

This is not uniform. Having a `emplace_xxx` factory function would make the code more uniform

```c++
auto opt=emplace_optional<T>(v1, vn);
f(emplace_optional<T>(v1, vn));

auto a=emplace_any<T>(v1, vn);
f(emplace_any<T>(v1, vn));
```

The implementation of `emplace_any` could be:

```c++
template <class T, class ...Args>
emplace_any(Args&& ...args) {
    return emplace_any(type<T>{}, args...);
}

template <class T, class ...Args>
emplace_any(type<T>, Args&& ...args) {
    return any(type<T>, in_place, args...);
}

emplace_any(type<none_t>) {
    return any();
}
```

If possible it would even better if the `emplace_any`/`emplace_optional` could be added as overloads of `make_any`/`make_optional`.

We have an implementation of this kind of overload in [DXXXX]. The problem is that this implementation is not portable and make use of UB. 
If anyone know how to implement this overloads in a portable way, the authors would prefer the `make_any`/`make_optional` overloads.

# Technical Specification

The wording is relative to n4335.

Update 6.1 Header <experimental/any> synopsis adding

```c++
  struct none_t{};
  constexpr none_t none{};

  template <class T> 
    struct type {};
  
  //... as before adding 
  template <class T, class ...Args>
    void emplace_optional(Args&& ...args);

```
Add inside class `any`

```c++
    constexpr any(none_t) noexcept;
    
    template <class T, class ...Args>
    any(type<T>, in_place_t, Args&& ...);
    
    any& operator=(none_t) noexcept;
    
    template <class T, class ...Args>
    void emplace(Args&& ...);
  };
```

Replace inside class `any`

```c++
    void clear() noexcept;
    bool empty() const noexcept;
```

by

```c++
    void reset() noexcept;
    explicit operator bool() const noexcept;
```

Add in 6.3.1 `any` construct/destruc after p1

```c++
    any(none_t) noexcept;
  };
```

after p14

```c++
    template <class T, class ...Args>
    any(type<T>, in_place_t, Args&& ...);   
  };
```

  *Requires*:
    is_constructible_v<T, Args&&...> is true.
  *Effects*:
    Initializes the contained value as if direct-non-list-initializing an object of type 
    `T` with the arguments `std::forward<Args>(args)...`.
  *Postconditions*:
    *this contains a value of type `T`.
  *Throws*:
    Any exception thrown by the selected constructor of `T`.
  *Remarks*:
    If `T`'s constructor selected for the initialization is a constexpr constructor, 
    this constructor shall be a constexpr constructor. 

Add in 6.3.2 `any` assignments after p9

```c++
    any& operator=(none_t) noexcept;
  };
```
  *Effects*:
    If `*this` contains a value, calls `val->T::~T()`to destroy the contained value; otherwise no effect.
  *Returns*:
    `*this.
  *Postconditions*:
    `*this` does not contain a value. 
    
Add in 6.3.3 `any` modifiers

```c++
    template <class T, class ...Args>
    void emplace(Args&& ...);
  };
```

Replace in 6.3.3 `any` modifiers, `clear` by `reset`.

Replace in 6.3.4 `any` observers, `empty` by `explicit operator bool.

Add in 6.4 Non-member functions

```c++
  template <class T, class ...Args>
    void emplace_optional(Args&& ...args);
```

   
# Acknowledgements 

Thanks to Jeffrey Yasskin to encourage me to report these a possible issues of the TS. 

# References

* n4335 - Working Draft, C++ Extensions for Library Fundamentals 
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4335.html

* DXXXX - C++ generic factory 
https://github.com/viboes/std-make/blob/master/doc/proposal/factories/DXXXX_factories.md


