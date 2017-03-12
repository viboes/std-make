// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ID_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ID_HPP

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
  `strong_id` is a strongly type that wraps a regular type and behaves like a `Regular` type
  The main goal is to be able to define strong identifiers that don't mix between them.
  No conversion to the underlying integer type is provided.

  Example
  <code>
  using X = strong_id<XTag>;
  using Y = strong_id<YTag>;
  void f(X, Y);

  f(X(true), Y(false));

  </code>
  */
  template <class Tag, class UT = int>
  struct strong_id final : private_tagged<Tag, UT>
  {
      using base_type = private_tagged<Tag, UT>;
      using base_type::base_type;

      // copy constructor/assignment default
      constexpr strong_id() noexcept = default;
      //constexpr explicit strong_id (void*) = delete;

      //!@{
      //! relational operators
      //!
      //! Forwards to the underlying value
      friend constexpr bool operator==(strong_id x, strong_id y)  noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_id x, strong_id y)  noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_id x, strong_id y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strong_id x, strong_id y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strong_id x, strong_id y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strong_id x, strong_id y)  noexcept { return x.value >= y.value; }
      //!@}
  };

  static_assert(std::is_pod<strong_id<int>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_id<int>>::value, "");
  static_assert(std::is_trivially_copyable<strong_id<int>>::value, "");
  static_assert(std::is_standard_layout<strong_id<int>>::value, "");
  static_assert(std::is_trivial<strong_id<int>>::value, "");

  //! underlying_type specialization for strong_id
  template <class Tag, class UT>
  struct underlying_type<strong_id<Tag, UT>> { using type = UT; };


}
}

  template <class Tag, class UT>
  struct hash<experimental::strong_id<Tag, UT>>
    : experimental::wrapped_hash<experimental::strong_id<Tag, UT>> {};

}

#endif // header
