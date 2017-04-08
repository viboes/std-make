// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED

#include <experimental/fundamental/v3/strong/tagged.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>

#include <stdexcept>
#include <limits>
#include <functional>
#include <type_traits>
#include <iosfwd>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  struct bad_bounded_int_cast : std::range_error {
    bad_bounded_int_cast() : std::range_error("Bad bounded int cast") {}
  };
  /**
  `strong_bounded_int` is a strongly type that wraps an integral type  @c UT and behaves like an  @c UT
  The main goal is to be able to define strong bounded integers that don't mix between them and that have a reduced set of values.
  No conversion to the underlying integer type is provided.

  Note that this is not a safe int, it just forbids the conversions between
  different strong bounded integers types.

  Example
  <code>
  using X = strong_bounded_int<XTag, int, 0, 9>;
  using Y = strong_bounded_int<YTag, int, 1, 5>;
  void f(X, Y);

  f(X(1), Y(1));

  </code>
  */
  // todo rename to strong_bounded_integer
  // todo make use of domain instead of tags
  //    Should this type be just a representation for an quantity?
  //    and use it in quantity<Domain, bounded_integer<>>

  template <class Tag, class UT, UT Low, UT High>
  struct strong_bounded_int final : private_tagged<Tag, UT>
  {
      static_assert(is_integral<UT>::value, "UT must be integral");
      static_assert(Low <= High, "Low must be less equal than High");

      using base_type = private_tagged<Tag, UT>;
      using base_type::base_type;
      using base_type::underlying;

      enum bounds : UT { low=Low, high=High };

      static constexpr bool valid(UT x) noexcept
      {
        return (Low <= x && x <= High) ;
      }
      static constexpr UT cast(UT x)
      {
        if ( valid(x) ) return x;
        throw bad_bounded_int_cast();
      }
      // copy constructor/assignment default
      constexpr strong_bounded_int() noexcept = default;
      constexpr explicit strong_bounded_int(UT v) : base_type(cast(v)) {}

      // additive operators
      friend constexpr strong_bounded_int operator+(strong_bounded_int x)  noexcept
      { return x; }
      friend constexpr strong_bounded_int operator+(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value + y.value); }
      constexpr strong_bounded_int& operator+=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value + y.value);
        return *this;
      }
      constexpr strong_bounded_int operator++()  noexcept
      { return strong_bounded_int(++this->value); }
      constexpr strong_bounded_int operator++(int)  noexcept
      { return strong_bounded_int(this->value++); }

      friend constexpr strong_bounded_int operator-(strong_bounded_int x)  noexcept
      { return strong_bounded_int(-x.value); }
      friend constexpr strong_bounded_int operator-(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value - y.value); }
      constexpr strong_bounded_int& operator-=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value - y.value);
        return *this;
      }
      constexpr strong_bounded_int operator--()  noexcept
      { return strong_bounded_int(--this->value); }
      constexpr strong_bounded_int operator--(int)  noexcept
      { return strong_bounded_int(this->value--); }

      //  Multiplicative operators
      friend constexpr strong_bounded_int operator*(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value * y.value); }
      constexpr strong_bounded_int& operator*=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value * y.value);
        return *this;
      }

      friend constexpr strong_bounded_int operator/(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value / y.value); }
      constexpr strong_bounded_int& operator/=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value / y.value);
        return *this;
      }

      friend constexpr strong_bounded_int operator%(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value % y.value); }
      constexpr strong_bounded_int& operator%=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value % y.value);
        return *this;
      }

      // Bitwise logic operators
      friend constexpr strong_bounded_int operator~(strong_bounded_int x)  noexcept
      { return strong_bounded_int(~x.value); }

      friend constexpr strong_bounded_int operator&(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value & y.value); }
      constexpr strong_bounded_int& operator&=(strong_bounded_int y)  noexcept
      { this->value = cast(this->value & y.value); return *this; }
      friend constexpr strong_bounded_int operator|(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value | y.value); }
      constexpr strong_bounded_int& operator|=(strong_bounded_int y)  noexcept
      { this->value = cast(this->value | y.value); return *this; }
      friend constexpr strong_bounded_int operator^(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value ^ y.value); }
      constexpr strong_bounded_int& operator^=(strong_bounded_int y)  noexcept
      { this->value = cast(this->value ^ y.value); return *this; }

      // Bitwise logic operators
      friend constexpr strong_bounded_int operator<<(strong_bounded_int x, int y)  noexcept
      { return strong_bounded_int(x.value << y); }
      constexpr strong_bounded_int& operator<<=(int y)  noexcept
      { this->value = cast(this->value << y); return *this; }
      friend constexpr strong_bounded_int operator>>(strong_bounded_int x, int y)  noexcept
      { return strong_bounded_int(x.value >> y); }
      constexpr strong_bounded_int& operator>>=(int y)  noexcept
      { this->value = cast(this->value >> y); return *this; }

      // relational operators
      friend constexpr bool operator==(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value >= y.value; }
  };

  template <class Tag, class UT, UT Low, UT High>
  struct underlying_type<strong_bounded_int<Tag,UT,Low,High>> { using type = UT; };

  static_assert(std::is_pod<strong_bounded_int<bool,int,0,3>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_bounded_int<bool,int,0,3>>::value, "");
  static_assert(std::is_trivially_copyable<strong_bounded_int<bool,int,0,3>>::value, "");
  static_assert(std::is_standard_layout<strong_bounded_int<bool,int,0,3>>::value, "");
  static_assert(std::is_trivial<strong_bounded_int<bool,int,0,3>>::value, "");

  // stream operators

  //! input function.
  //! @par Effects:<br> Extracts a T from is and stores it in the passes x.
  //! @param is the input stream
  //! @param x the \c strong_bool
  //! @par Returns:<br> \c is.

  template <class CharT, class Traits, class Tag, class T, T Low, T High >
  std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, strong_bounded_int<Tag, T, Low, High>& x)
  {
    T v;
    is >> v;
    x = strong_bounded_int<Tag, T, Low, High>(v);
    return is;
  }

  //! output function.
  //! @param os the output stream
  //! @param x the \c strong_bool
  //!
  //! @par Returns:<br> the result of the following expression
  //! @code
  //! os << x.undelying()
  //! @endcode

  template <class CharT, class Traits, class Tag, class T, T Low, T High >
  std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const strong_bounded_int<Tag, T, Low, High>& x)
  {
    return os << x.underlying();
  }

}
}

  template <class Tag, class UT, UT Low, UT High>
  struct hash<experimental::strong_bounded_int<Tag,UT,Low,High>>
    : experimental::wrapped_hash<experimental::strong_bounded_int<Tag,UT,Low,High>> {};

  template <class Tag, class UT, UT Low, UT High>
  struct numeric_limits<experimental::strong_bounded_int<Tag,UT,Low,High>> : numeric_limits<UT> {
      using T = experimental::strong_bounded_int<Tag,UT,Low,High>;
      static constexpr T min() noexcept { return T(Low); }
      static constexpr T max() noexcept { return T(High); }
      static constexpr T lowest() noexcept { return T(Low); }
  };

}
#endif
#endif // header
