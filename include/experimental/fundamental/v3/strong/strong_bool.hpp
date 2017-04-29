// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_BOOL_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_BOOL_HPP

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/convertible.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/logical.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/ordinal.hpp>

#include <type_traits>
#include <functional>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  /**
  `strong_bool` is a strongly type that wraps a bool and behaves like an `bool`
  The main goal is to be able to define strong bools that don't mix between them.
  The single conversion is the explicit conversion to bool type.
  This is a safe bool, in the sense that it doesn't allows conversions from integral, floating point and pointer types.
  In addition it forbids conversions between different strong bools types.

  As any tagged type, it makes the parameter more explicit.
  Example
  <code>
  using X = strong_bool<XTag>;
  using Y = strong_bool<YTag>;
  void f(X, Y);

  f(X(true), Y(false));

  </code>
  */

  // Q/A
  // Do we need a Bool parameter? Which type will be a good candidate?
  // bool takes 4 bytes in some machines. Sometimes we want to use just 1 byte
  //
  // fixme: Is this a safe bool or a strong type bool?
  // We are mixing strong features with safe features. Maybe we need a strong_bool and a strong_safe_bool.
  // Note that strong_bool<Tag, safe_bool<>> is not a strong_safe_bool :(
  //
  // Should strong_bool be implicitly convertible from bool?
  // It is better if strong types are only explicitly convertible from its underlying type, so that we avoid overload ambiguity.
  // However, requiring an explicit conversion make the code cumbersome.
  // See safe_bool.
  //
  // fixme Should this class convert implicitly or explicitly to bool?
  // A strong bool is a bool, so we could convert it implicitly.
  // Note that having implicit conversions introduce overload ambiguity and undesirable operations e.g. +
  // For the time being, we support only explicit conversion on both sides.
  //
  // fixme do we need the logical operators ?
  // strong_bool is explicit convertible to bool and so && and || operator and ?: operator will force the conversion to bool
  // If we don't define them we need to explicitly convert to strong_bool
  // If we define them, we need to explicitly to convert to bool.
  // This can be the reason d'être of another type.
  //
  // Do we need mixed boolean operators?
  // If yes, which would be the result. IMO, it should be bool.
  // If not, there will be a conversion to bool and the result is bool
  // So we don't need them
  //
  // fixme What should be the result of comparing two strong_bool? bool or strong_bool?
  // For the time being, these functions return bool.
  //
  // fixme Should strong_bool be comparable?
  //  is bool comparable because it converts to int?
  //  Maybe the best is to define them if the underlying type provides them.
  //
  // fixme Should strong_bool remove the default constructor?
  // We want strong types to be default constructible if they are by default.
  // Forcing the default construction, add more functionality than needed.
  // This is maybe the role of safe_bool.
  //
  // fixme Should strong_bool default construct to false?
  // This is maybe the role of safe_bool.

  template <class Tag, class Bool = bool>
  struct strong_bool final
     : strong_type<strong_bool<Tag, Bool>, Bool>
     , mixin::comparable<strong_bool<Tag, Bool>>
     , mixin::explicit_convertible_to<strong_bool<Tag, Bool>, bool>
     , mixin::logical<strong_bool<Tag, Bool>>
     , mixin::streamable<strong_bool<Tag, Bool>>
  {
      using base_type = strong_type<strong_bool<Tag, Bool>, Bool>;
      using base_type::base_type;

      // Now that we have a safe_bool, we don't want to restrict the representation to an integral type
      //static_assert(is_integral<Bool>::value, "The representation must be an integral type");

      // copy constructor/assignment default
      constexpr strong_bool() noexcept = default;
      // If Bool is not bool, we want an explicit conversion from bool but not Bool.
      constexpr explicit strong_bool (Bool) = delete;
      constexpr explicit strong_bool (bool b) : base_type(b) {};

      // unwanted conversions
      // Now that we have a safe_bool, we don't want to restrict the representation to an integral type
      constexpr explicit strong_bool (int) = delete;
      constexpr explicit strong_bool (double) = delete;
      constexpr explicit strong_bool (void*) = delete;
      template <class R, class ... Args>
      constexpr explicit strong_bool (R(*)(Args...)) = delete;
      template <class C, class R, class ... Args>
      constexpr explicit strong_bool (R(C::*)(Args...)) = delete;
  };

  template <class Tag>
  struct strong_bool<Tag, bool> final
  : strong_type<strong_bool<Tag, bool>, bool>
  , mixin::comparable<strong_bool<Tag, bool>>
  , mixin::explicit_convertible_to<strong_bool<Tag, bool>, bool>
  , mixin::logical<strong_bool<Tag, bool>>
  , mixin::streamable<strong_bool<Tag, bool>>
  {
      using base_type = strong_type<strong_bool<Tag, bool>, bool>;
      using base_type::base_type;

      // copy constructor/assignment default
      constexpr strong_bool() noexcept = default;

      // unwanted conversions
      constexpr explicit strong_bool (int) = delete;
      constexpr explicit strong_bool (double) = delete;
      constexpr explicit strong_bool (void*) = delete;
      template <class R, class ... Args>
      constexpr explicit strong_bool (R(*)(Args...)) = delete;
      template <class C, class R, class ... Args>
      constexpr explicit strong_bool (R(C::*)(Args...)) = delete;
  };

  static_assert(std::is_pod<strong_bool<bool>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_bool<bool>>::value, "");
  static_assert(std::is_trivially_copyable<strong_bool<bool>>::value, "");
  static_assert(std::is_standard_layout<strong_bool<bool>>::value, "");
  static_assert(std::is_trivial<strong_bool<bool>>::value, "");

  //! underlying_type specialization for strong_bool
  template <class Tag, class Bool>
  struct underlying_type<strong_bool<Tag, Bool>> { using type = Bool; };

  namespace ordinal {
    template <class Tag, class T>
    struct traits<strong_bool<Tag, T>>
    {
      using size_type = size_t;
      using size = integral_constant<size_type, 2>;
      static strong_bool<Tag, T> val(size_type p) { return strong_bool<Tag, T>{static_cast<T>(p)}; }

      static size_type pos(strong_bool<Tag, T> u)  { return size_type{u.underlying()}; }
    };
  }
}
}

  template <class Tag, class UT>
  struct hash<experimental::strong_bool<Tag,UT>> :
    experimental::wrapped_hash<experimental::strong_bool<Tag,UT>> {};
}

#endif // header
