<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXX</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2015-09-05</td>
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

Both `optional` and `any` are classes that can store possibly some underlying type. In the case of optional the underlying type is know at compile time, for `any` the underlying type is any and know at run-time.

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

* Add `in_place_f` a template function (see [eggs::variant](https://github.com/eggs-cpp/variant)).

* In class `optional<T>`
  * Add a `reset` member function.

* Add an `emplace_optional` factory.

* In class `any`
  * make the default constructor constexpr,
  * add in place forward constructors,
  * add emplace forward member functions,
  * rename the `empty` function with an `explicit bool` conversion,
  * rename the `clear` member function to `reset`,

* Add a `none` constexpr variable of type `any`.

* Add an `emplace_any` factory.
  
# Design rationale

## `any` in_place constructor

`optonal<T>` in place constructor constructs implicitly a `T`. 

```c++
template <class... Args> 
constexpr explicit optional<T>::optional(in_place_t, Args&&... args);
```

In place construct for `any` can not have an implicit type `T`. We need a way to state explicitly which `T` must be constructed in place.
The function `in_place_t(*)(unspecified<T>)` is used to convey the type `T` participating in overload resolution.

```c++
template <class T, class ...Args>
any(in_place_t(*)(unspecified<T>), , Args&& ...);
```

This can be used as 

```c++
any(in_place<X>, v1, ..., vn);
```

Adopting this template class to optional would needs to change the definition of `in_place` to 

```c++
constexpr in_place_t in_place() { return {} };
```

and 

```c++
template <class... Args> 
constexpr explicit optional<T>::optional(in_place_t (*)(), Args&&... args);
```

But this doesn't works as

```c++
optional<int>(0) 
```

is ambiguous, as convertible to `in_place_t (*)()` and to `int`.

So we need can not use `in_place` for both cases. We propose `in_place_f` 

```c++
any(in_place_f<X>, v1, ..., vn);
```

## `any` emplace forward member function

`optional<T>` emplace member function emplaces implicitly a `T`. 

```c++
template <class ...Args>
optional<T>::emplace(Args&& ...);
```

`emplace` for `any` can not have an implicit type `T`. 
We need a way to state explicitly which `T` must be emplaced. 

```c++
template <class T, class ...Args>
any::emplace(Args&& ...);
```

and used as follows

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

## About the constant `none`

Instead of an additional `none`, `any{}` plays well the role. 
However, the authors think that using `none` is much more explicit.

```c++
any a = 1;
a = none;
```

## Do we need a specific type for `none` 

Instead of an additional type `none_t` to declare the constant `none` as `constexpr none_t none{}`,
we can just declare it as `constexpr any none{}` which plays well its role. 

The advantages of the `any` constant is that we don't need conversions.
However, assignment from `none` could be less efficient. 
If performance is required the user should use the `reset` function.

Alternatively we can add `none_t` and define the corresponding conversions.

## About a `none_t` type implicitly convertible to `any` and `optional` 

An alternative to the reset member function would be to be able to assign a `none_t` to an `optional` and to an `any`. 
We could consider that a default instance of `any` contains an instance of a `none_t` type, as `optional` contains a `nullopt`.

The problem is that then `none` can be seen as an `optional` or an `any` and could result in posible ambiguity.

We think that this implicit conversions go against the raison d'être of `nullptr` and that we need explicit factories/constants.

## Do we need an explicit `make_any` factory? 

`any` is not a generic type but a type erased type. `any` play the same role than a posible `make_any`.

This is way this paper doesn't propose a `make_any` factory. 

## About an emplace factories

However, we could consider an emplace_xxx factory that in place constructs a `T`.

`optional<T>` and `any` could be in place constructed as follows:

```c++
optional<T> opt(in_place_t, v1, vn);
f(optional<T>(in_place, v1, vn));

any a(in_place_t(*)(unspecified<T>), v1, vn);
f(any(in_place_f<T>, v1, vn));
```

When we use auto things change a little bit

```c++
auto opt=optional<T>(in_place, v1, vn);
auto a=any(in_place_f<T>, v1, vn);
```

This is not uniform. Having an `emplace_xxx` factory function would make the code more uniform

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
    return any(in_place_f<T>, std::forward<Args>(args)...);
}
```

It is possible to replace `emplace_optional` by an overloaded extension of `make_optional`. 
We have an implementation of this kind of overload in [GF](https://github.com/viboes/std-make/blob/master/doc/proposal/any_optional/fundamental_ts_improvements.md#c-generic-factory---implementation) as part of the  [DXXXX](https://github.com/viboes/std-make/blob/master/doc/proposal/any_optional/fundamental_ts_improvements.md#dxxxx---c-generic-factory) proposal. 

The authors prefer the overloaded version, however this is not part of this proposal.

```c++
auto opt=emplace_optional<T>(v1, vn);
f(make_optional<T>(v1, vn));

auto a=emplace_any<T>(v1, vn);
f(make_any<T>(v1, vn));
```

## Which file for `in_place_t`, `in_place` and `in_place_f`?

As `in_place_t` is used by `optional` and `any` we need to move its definition to another file.
The preference of the authors will be to place `in_place_t` in `<experimental/utility>`.
For coherence purposes `in_place` and  `in_place_f` would be moved also.

Note that `in_place_f`can also be used by `experimental::variant` and that in this case it couldalso take an int as template parameter.

# Open points

* Do we want in place constructor for `any`?

* Do we want to adopt the new `in_place_f` definition? 

* Do we want the `clear` and `reset` changes?

* Do we want the `operator bool` changes?

* Do we want the `emplace_xxx` factories?


# Technical Specification

The wording is relative to [n4335] (https://github.com/viboes/std-make/blob/master/doc/proposal/any_optional/fundamental_ts_improvements.md#n4335---working-draft-c-extensions-for-library-fundamentals).

Move `in_place_t` and `in_place` to the <experimental/utility> synopsis and add `in_place_f`.

```c++
struct in_place_t {};
constexpr in_place_t in_place;
template <class ...T>;
constexpr in_place_t in_place_f(unspecified<T...>);
template <size N>;
constexpr in_place_t in_place_f(unspecified<N>);
```

Update 5.2 [optional.synopsis] adding after `make_optional`

```c++
  template <class T, class ...Args>
    optional<T> emplace_optional(Args&& ...args);
```

Add to 5.3 [optional.object]  

```c++
    void reset() noexcept;
```

Add in 5.10 [optional.specalg]

```c++
  template <class T, class ...Args>
    optional<T> emplace_optional(Args&& ...args);
```

  *Returns*:
    optional<T>(in_place, std::forward<Args>(args)...). 

Update 6.1 [any.synopsis] adding

```c++
  template <class T, class ...Args>
    any emplace_any(Args&& ...args);
```

Add inside class `any`

```c++
    template <class T, class ...Args>
      any(in_place_t (*)unspecified<T>, Args&& ...);
    template <class T, class U, class... Args>
      explicit any(in_place_t (*)(unspecified<T>), initializer_list<U>, Args&&...);
    
    template <class T, class ...Args>
      void emplace(Args&& ...);
    template <class T, class U, class... Args>
      void emplace(initializer_list<U>, Args&&...);
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

Add after class `any`

  constexpr any none{};

Add in 6.3.1 `any` construct/destruc after p14

```c++
    template <class T, class ...Args>
    any(in_place_t(*)(unspecified<T>), Args&& ...);   
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


```c++
    template <class T, class U, class ...Args>
    any(in_place_t (*)(unspecified<T>), initializer_list<U> il, Args&& ...args);   
  };
```

  *Requires*:
    `is_constructible_v<T, initializer_list<U>&, Args&&...>` is true.
  *Effects*:
    Initializes the contained value as if direct-non-list-initializing an object of type `T` with the arguments `il, std::forward<Args>(args)...`.
  *Postconditions*:
    `*this` contains a value.
  *Throws*:
    Any exception thrown by the selected constructor of `T`.
  *Remarks*:
    The function shall not participate in overload resolution unless `is_constructible_v<T, initializer_list<U>&, Args&&...>` is true. 
    

Add in 6.3.3 `any` modifiers

```c++
    template <class T, class ...Args>
    void emplace(Args&& ...);
  };
```

  *Requires*:
    is_constructible_v<T, Args&&...> is true.
  *Effects*:
    Calls `this.reset()`. Then initializes the contained value as if direct-non-list-initializing 
    an object of type `T` with the arguments `std::forward<Args>(args)...`.
  *Postconditions*:
    *this contains a value.
  *Throws*:
    Any exception thrown by the selected constructor of `T`.
  *Remarks*:
    If an exception is thrown during the call to `T`'s constructor, 
    `*this` does not contain a value, and the previous (if any) has been destroyed. 

```c++
    template <class T, class U, class ...Args>
    void emplace(initializer_list<U> il, Args&& ...);
  };
```

  *Effects*:
    Calls this->reset()`. 
    Then initializes the contained value as if direct-non-list-initializing 
    an object of type `T` with the arguments `il, std::forward<Args>(args)...`.
  *Postconditions*:
    `*this` contains a value.
  *Throws*:
    Any exception thrown by the selected constructor of `T`.
  *Remarks*:
    If an exception is thrown during the call to `T`'s constructor, 
    `*this` does not contain a value, and the previous (if any) has been destroyed.

    The function shall not participate in overload resolution unless `is_constructible_v<T, initializer_list<U>&, Args&&...>` is true.

Replace in 6.3.3 `any` modifiers, `clear` by `reset`.

Replace in 6.3.4 `any` observers, `empty` by `explicit operator bool.

Add in 6.4 Non-member functions

```c++
  template <class T, class ...Args>
    any emplace_any(Args&& ...args);
```

  *Returns*:
    any(in_place_t<T>{}, std::forward<Args>(args)...). 
   
# Acknowledgements 

Thanks to Jeffrey Yasskin to encourage me to report these as possible issues of the TS, 
Agustin Bergé for his suggestions about `none` and `in_place`.  

# References

## n4335 - Working Draft, C++ Extensions for Library Fundamentals 
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4335.html

## DXXXX - C++ generic factory 
https://github.com/viboes/std-make/blob/master/doc/proposal/factories/DXXXX_factories.md

## C++ generic factory - Implementation 
https://github.com/viboes/std-make/blob/master/include/experimental/std_make_v1/make.hpp

## eggs::variant
https://github.com/eggs-cpp/variant



