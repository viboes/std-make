// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/additive.hpp>
#include <experimental/fundamental/v3/strong/mixins/bitwise.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/integer_multiplicative.hpp>
#include <experimental/fundamental/v3/strong/mixins/hashable.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/fundamental/v3/strong/mixins/ordinal.hpp>

#include <experimental/contract.hpp>

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
  struct strong_bounded_int;

  namespace mixin {
    template <class Tag, class UT1, UT1 Low1, UT1 High1, class UT2, UT2 Low2, UT2 High2>
    struct is_compatible_with<strong_bounded_int<Tag, UT1, Low1, High1>, strong_bounded_int<Tag, UT2, Low2, High2>> : true_type {};
  }

  template <class Tag, class UT, UT Low, UT High>
  struct strong_bounded_int final
    : strong_type<strong_bounded_int<Tag, UT, Low, High>, UT>
    , mixin::additive_with_if<strong_bounded_int<Tag, UT, Low, High>, mixin::check>
    , mixin::bitwise_with_if<strong_bounded_int<Tag, UT, Low, High>, mixin::check>
    , mixin::comparable_with_if<strong_bounded_int<Tag, UT, Low, High>>
    , mixin::integer_multiplicative_with_if<strong_bounded_int<Tag, UT, Low, High>, mixin::check>
    , mixin::hashable<strong_bounded_int<Tag, UT, Low, High>>
    , mixin::streamable<strong_bounded_int<Tag, UT, Low, High>>
  {
      static_assert(is_integral<UT>::value, "UT must be integral");
      static_assert(Low <= High, "Low must be less equal than High");

      using base_type = strong_type<strong_bounded_int<Tag, UT, Low, High>, UT>;
      using base_type::base_type;
      using base_type::underlying;

      enum bounds : UT { low=Low, high=High };

      static constexpr bool valid(UT x) noexcept
      {
        return (Low <= x && x <= High) ;
      }
      static JASEL_CXX14_CONSTEXPR UT cast(UT x)
      {
          JASEL_EXPECTS( valid(x) );
          return x;
      }
      // copy constructor/assignment default
      constexpr strong_bounded_int() noexcept = default;
      constexpr explicit strong_bounded_int(UT v) : base_type(cast(v)) {}
  };

  static_assert(std::is_pod<strong_bounded_int<bool,int,0,3>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_bounded_int<bool,int,0,3>>::value, "");
  static_assert(std::is_trivially_copyable<strong_bounded_int<bool,int,0,3>>::value, "");
  static_assert(std::is_standard_layout<strong_bounded_int<bool,int,0,3>>::value, "");
  static_assert(std::is_trivial<strong_bounded_int<bool,int,0,3>>::value, "");

  namespace ordinal {
    /// A strong_bounded_int is an ordinal type having the bounds Low..High
    template <class Tag, class T, T Low, T High >
    struct traits<strong_bounded_int<Tag, T, Low, High>>
    {
      using size_type = ordinal::index_t;
      using size = integral_constant<size_type, High-Low+1>;
      static strong_bounded_int<Tag, T, Low, High> val(size_type p) { return strong_bounded_int<Tag, T, Low, High>{static_cast<T>(p+Low)}; }

      static size_type pos(strong_bounded_int<Tag, T, Low, High> u)  { return static_cast<size_type>(u.underlying()-Low); }
    };
  }
}
}

  /// Hash specialization forwarding to the hash of underlying type
  template <class Tag, class UT, UT Low, UT High>
  struct hash<experimental::strong_bounded_int<Tag,UT,Low,High>>
    : experimental::wrapped_hash<experimental::strong_bounded_int<Tag,UT,Low,High>> {};

  /// numeric_limits specialization forwarding to the numeric_limits of underlying type
  template <class Tag, class UT, UT Low, UT High>
  struct numeric_limits<experimental::strong_bounded_int<Tag,UT,Low,High>> : numeric_limits<UT> {
      using T = experimental::strong_bounded_int<Tag,UT,Low,High>;
      static constexpr T min() noexcept { return T(Low); }
      static constexpr T max() noexcept { return T(High); }
      static constexpr T lowest() noexcept { return T(Low); }
  };

}
#endif // header
