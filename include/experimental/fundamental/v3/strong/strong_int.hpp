// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP

#if __cplusplus >= 201402L

#include <experimental/fundamental/v3/strong/tagged.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>

#include <limits>
#include <functional>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  /**
  `strong_int` is a strongly type tagged by the tag @c Tag that wraps an integral type @c UT and behaves like an @c UT
  The main goal is to be able to define strong integers that don't mix between them.
  The single allowed conversion is to the underlying integer type.

  Note that this is not a safe int, it just forbids the conversions between
  different strong integers types.

  Example
  <code>
  using X = strong_int<XTag, int>;
  using Y = strong_int<YTag, int>;
  void f(X, Y);

  f(X(1), Y(1));

  </code>
  */

  template <class Tag, class UT>
  struct strong_int final : private_tagged<Tag, UT>
  {
      using base_type = private_tagged<Tag, UT>;
      using base_type::base_type;

      // additive operators
      friend constexpr strong_int operator+(strong_int x)  noexcept
      { return x; }
      friend constexpr strong_int operator+(strong_int x, strong_int y)  noexcept
      { return strong_int(x.value + y.value); }
      constexpr strong_int& operator+=(strong_int y)  noexcept
      { this->value += y.value; return *this; }
      constexpr strong_int operator++()  noexcept
      { return strong_int(++this->value); }
      constexpr strong_int operator++(int)  noexcept
      { return strong_int(this->value++); }

      friend constexpr strong_int operator-(strong_int x)  noexcept
      { return strong_int(-x.value); }
      friend constexpr strong_int operator-(strong_int x, strong_int y)  noexcept
      { return strong_int(x.value - y.value); }
      constexpr strong_int& operator-=(strong_int y)  noexcept
      { this->value -= y.value; return *this; }
      constexpr strong_int operator--()  noexcept
      { return strong_int(--this->value); }
      constexpr strong_int operator--(int)  noexcept
      { return strong_int(this->value--); }

      //  Multiplicative operators
      friend constexpr strong_int operator*(strong_int x, strong_int y)  noexcept
      { return strong_int(x.value * y.value); }
      constexpr strong_int& operator*=(strong_int y)  noexcept
      { this->value *= y.value; return *this; }

      friend constexpr strong_int operator/(strong_int x, strong_int y)  noexcept
      { return strong_int(x.value / y.value); }
      constexpr strong_int& operator/=(strong_int y)  noexcept
      { this->value /= y.value; return *this; }

      friend constexpr strong_int operator%(strong_int x, strong_int y)  noexcept
      { return strong_int(x.value % y.value); }
      constexpr strong_int& operator%=(strong_int y)  noexcept
      { this->value %= y.value; return *this; }

      //fixme: do we want the Bitwise logic operators for an integer
      // Bitwise logic operators
      friend constexpr strong_int operator~(strong_int x)  noexcept
      { return strong_int(~x.value); }

      friend constexpr strong_int operator&(strong_int x, strong_int y)  noexcept
      { return strong_int(x.value & y.value); }
      constexpr strong_int& operator&=(strong_int y)  noexcept
      { this->value &= y.value; return *this; }
      friend constexpr strong_int operator|(strong_int x, strong_int y)  noexcept
      { return strong_int(x.value | y.value); }
      constexpr strong_int& operator|=(strong_int y)  noexcept
      { this->value |= y.value; return *this; }
      friend constexpr strong_int operator^(strong_int x, strong_int y)  noexcept
      { return strong_int(x.value ^ y.value); }
      constexpr strong_int& operator^=(strong_int y)  noexcept
      { this->value ^= y.value; return *this; }

      // Bitwise logic operators
      friend constexpr strong_int operator<<(strong_int x, int y)  noexcept
      { return strong_int(x.value << y); }
      constexpr strong_int& operator<<=(int y)  noexcept
      { this->value <<= y; return *this; }
      friend constexpr strong_int operator>>(strong_int x, int y)  noexcept
      { return strong_int(x.value >> y); }
      constexpr strong_int& operator>>=(int y)  noexcept
      { this->value >>= y; return *this; }

      // relational operators
      friend constexpr bool operator==(strong_int x, strong_int y)  noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_int x, strong_int y)  noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_int x, strong_int y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strong_int x, strong_int y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strong_int x, strong_int y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strong_int x, strong_int y)  noexcept { return x.value >= y.value; }
  };

  // fixme do we need swap?

  template <class Tag, class UT>
  constexpr strong_int<Tag, UT> make_strong_int(UT x)
  {
    return strong_int<Tag, UT>(x);
  }

  //! underlying_type specialization for strong_int
  template <class Tag, class UT>
  struct underlying_type<strong_int<Tag,UT>> { using type = UT; };

  static_assert(std::is_pod<strong_int<bool,int>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_int<bool,int>>::value, "");
  static_assert(std::is_trivially_copyable<strong_int<bool,int>>::value, "");
  static_assert(std::is_standard_layout<strong_int<bool,int>>::value, "");
  static_assert(std::is_trivial<strong_int<bool,int>>::value, "");

}
}

  template <class Tag, class UT>
  struct hash<experimental::strong_int<Tag,UT>>
    : experimental::wrapped_hash<experimental::strong_int<Tag, UT>> {};

  template <class Tag, class UT>
  struct numeric_limits<experimental::strong_int<Tag,UT>> : numeric_limits<UT> {  };


}
#endif
#endif // header
