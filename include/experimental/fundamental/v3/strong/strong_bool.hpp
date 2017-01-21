// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_BOOL_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_BOOL_HPP

#include <experimental/fundamental/v3/strong/wrapper.hpp>
#include <experimental/fundamental/v3/strong/only_when.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  template <class T, class U>
  struct is_convertible_flip : std::is_convertible<U,T> {};

  /**
  `strong_bool` is a strongly type that wraps a bool and behaves like an `bool`
  The main goal is to be able to define strong bools that don't mix between them.
  The single conversion is to the underlying integer type.

  Note that this is not a safe bool, it just forbids the conversions between
  different strong bools types.

  Example
  <code>
  using X = strong_bool<XTag, int>;
  using Y = strong_bool<YTag, int>;
  void f(X, Y);

  f(X(1), Y(1));

  </code>
  */
  template <class Tag, class Default = uninitialized_t>
  struct strong_bool final : protected_wrapper<bool, Default>
  {
      using base_type = protected_wrapper<bool, Default>;
      using base_type::base_type;
      using base_type::underlying;

      using tag_type = Tag;
      using underlying_t = bool;

      // copy constructor/assignment default
      constexpr strong_bool() noexcept : base_type() {}
#ifdef JASEL_CONVERTIBLE_DELETED
      constexpr explicit strong_bool (only_when<bool, is_convertible_flip> v) = delete;
#else
      constexpr explicit strong_bool (int) = delete;
      constexpr explicit strong_bool (double) = delete;
      constexpr explicit strong_bool (void*) = delete;
#endif

      // relational operators
      friend constexpr bool operator==(strong_bool x, strong_bool y)  noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_bool x, strong_bool y)  noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_bool x, strong_bool y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strong_bool x, strong_bool y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strong_bool x, strong_bool y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strong_bool x, strong_bool y)  noexcept { return x.value >= y.value; }
  };

  template <class E, class D>
  struct underlying_type<strong_bool<E,D>> { typedef bool type; };

}
}
}

#endif // header
