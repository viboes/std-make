// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_BOOL_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_BOOL_HPP

#include <experimental/fundamental/v3/strong/tagged.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
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
  The single conversion is the explicit conversion to the underlying bool type.

  Note that this is not a safe bool, it just forbids the conversions between
  different strong bools types.

  Example
  <code>
  using X = strong_bool<XTag>;
  using Y = strong_bool<YTag>;
  void f(X, Y);

  f(X(true), Y(false));

  </code>
  */
  template <class Tag, class Bool = bool>
  struct strong_bool final : protected_tagged<Tag, Bool>
  {
      using base_type = protected_tagged<Tag, Bool>;
      using base_type::base_type;

      // copy constructor/assignment default
      constexpr strong_bool() noexcept = default;

      constexpr explicit strong_bool (int) = delete;
      constexpr explicit strong_bool (double) = delete;
      constexpr explicit strong_bool (void*) = delete;

      //!@{
      //! relational operators
      //!
      //! Forwards to the underlying value
      friend constexpr bool operator==(strong_bool x, strong_bool y)  noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_bool x, strong_bool y)  noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_bool x, strong_bool y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strong_bool x, strong_bool y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strong_bool x, strong_bool y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strong_bool x, strong_bool y)  noexcept { return x.value >= y.value; }
      //!@}
  };

  static_assert(std::is_pod<strong_bool<bool>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_bool<bool>>::value, "");
  static_assert(std::is_trivially_copyable<strong_bool<bool>>::value, "");
  static_assert(std::is_standard_layout<strong_bool<bool>>::value, "");
  static_assert(std::is_trivial<strong_bool<bool>>::value, "");

  //! underlying_type specialization for strong_bool
  template <class Tag, class Bool>
  struct underlying_type<strong_bool<Tag, Bool>> { using type = Bool; };


}
}

  template <class Tag, class UT>
  struct hash<experimental::strong_bool<Tag,UT>> :
    experimental::wrapped_hash<experimental::strong_bool<Tag,UT>> {};

}

#endif // header
