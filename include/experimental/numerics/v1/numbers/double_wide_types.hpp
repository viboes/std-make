// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017-2018 Vicente J. Botet Escriba
// Based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0103r1.html

#ifndef JASEL_NUMERIC_V1_NUMBERS_DOUBLE_WIDE_TYPES_HPP
#define JASEL_NUMERIC_V1_NUMBERS_DOUBLE_WIDE_TYPES_HPP

#include <cstdint>
#include <climits>
#include <limits>
#include <cassert>
#include <experimental/type_traits.hpp>
#include <experimental/contract.hpp>
#include <experimental/utility.hpp>

namespace std
{
namespace experimental
{
namespace numerics
{
inline  namespace v1
{
  // Why to redefine it? It can be specialized for UDT
  template < class T >
  struct make_signed : ::std::make_signed<T> {};
  template <class T>
  using make_signed_t = typename make_signed<T>::type;

  // this is equivalent to bit_cast to the signed type of the unsigned integral type where we don't need to use memcpy and pass the address of the integral but the value itself
  // Note that the value passed as argument can be greater than the max signed value and that it will be seen as a negative number.

  template <class T>
  constexpr make_signed_t<T> to_signed_bit_cast(T x) noexcept
  {
      return static_cast<make_signed_t<T>>(x);
  }

  // this is equivalent to a narrow cast to the signed type
  // precondition: can_narrow_to<make_signed_t<T>>(x)
  template <class T>
  constexpr make_signed_t<T> to_signed_cast(T x) noexcept
  {
      return narrow_cast<make_signed_t<T>>(x);
  }
  // this is equivalent to narrow to the signed type
  template <class T>
  constexpr make_signed_t<T> to_signed(T x)
  {
      return narrow<make_signed_t<T>>(x);
  }

  // Why to redefine it? It can be specialized for UDT
  template < class T >
  struct make_unsigned : ::std::make_unsigned<T> {};
  template <class T>
  using make_unsigned_t = typename make_unsigned<T>::type;


  // this is equivalent to bit_cast to the unsigned type of the signed integral type where we don't need to use memcpy and pass the address of the integral but the value itself
  // Note that the value passed as argument can be negative and the function is wide
  template <class T>
  constexpr make_unsigned_t<T> to_unsigned_bit_cast(T x) noexcept
  {
      return static_cast<make_unsigned_t<T>>(x);
  }

  // this is equivalent to a narrow cast to the unsigned type
  // precondition: can_narrow_to<make_unsigned_t<T>>(x)
  template <class T>
  constexpr make_unsigned_t<T> to_unsigned_cast(T x) noexcept
  {
      return narrow_cast<make_unsigned_t<T>>(x);
  }

  // this is equivalent to narrow to the unsigned type
  template <class T>
  constexpr make_unsigned_t<T> to_unsigned(T x)
  {
      return narrow<make_unsigned_t<T>>(x);
  }

  template < class T >
  struct make_double_width;
  template <  >
  struct make_double_width <int8_t> { using type = int16_t; };
  template <  >
  struct make_double_width <uint8_t> { using type = uint16_t; };
  template <  >
  struct make_double_width <int16_t> { using type = int32_t; };
  template <  >
  struct make_double_width <uint16_t> { using type = uint32_t; };
  template <  >
  struct make_double_width <int32_t> { using type = int64_t; };
  template <  >
  struct make_double_width <uint32_t> { using type = uint64_t; };
#if 0
  template <  >
  struct make_double_width <int64_t> { using type = __int128_t; };
  template <  >
  struct make_double_width <uint64_t> { using type = __uint128_t; };
#endif

  // fixme: Why not make_double_width_t?
  template <class T>
  using double_width = typename make_double_width<T>::type;
  template <class T>
  using make_double_width_t = typename make_double_width<T>::type;

  template <class T>
  using make_double_signed_t = make_double_width_t<make_signed_t<T>>;


  template <class T>
  constexpr double_width<T> to_double_width(T x) noexcept
  {
      return double_width<T>(x);
  }

  template < class T >
  struct make_half_width;
  template <  >
  struct make_half_width <int16_t> { using type = int8_t; };
  template <  >
  struct make_half_width <uint16_t> { using type = uint8_t; };
  template <  >
  struct make_half_width <int32_t> { using type = int16_t; };
  template <  >
  struct make_half_width <uint32_t> { using type = uint16_t; };
  template <  >
  struct make_half_width <int64_t> { using type = int32_t; };
  template <  >
  struct make_half_width <uint64_t> { using type = uint32_t; };
#if 0
  template <  >
  struct make_half_width <int128_t> { using type = int64_t; };
  template <  >
  struct make_half_width <uint128_t> { using type = uint64_t; };
#endif

  // fixme: Why not make_half_width_t?
  template <class T>
  using half_width = typename make_half_width<T>::type;
  template <class T>
  using make_half_width_t = typename make_half_width<T>::type;
  template <class T>
  constexpr half_width<T> to_half_width(T x) noexcept
  {
      return static_cast<half_width<T>>(x);
  }


  typedef int single_sword; // this is implementation defined
  typedef make_unsigned_t<single_sword> single_uword;
  typedef double_width<single_sword> double_sword;
  typedef double_width<single_uword> double_uword;

  template <typename T, typename H = half_width<make_unsigned_t<T>>>
  constexpr H split_upper( T a ) noexcept
  {
      static_assert(is_integral<T>::value, "T must be integral");

    return to_half_width( to_unsigned_bit_cast(a) >> std::numeric_limits<H>::digits );
  }
  template <typename T, typename H = half_width<make_unsigned_t<T>>>
  constexpr H split_lower( T a ) noexcept
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_half_width(to_unsigned_bit_cast(a));
  }

  template <typename T>
  constexpr make_unsigned_t<double_width<T>> wide_unsigned( T high, T low ) noexcept
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_unsigned<T>::value, "T must be unsigned");

    return (to_double_width(high) << (CHAR_BIT * sizeof(T)))  | to_double_width(low);
  }
  template <typename T>
  constexpr make_signed_t<double_width<T>> wide_signed( T high, T low ) noexcept
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_unsigned<T>::value, "T must be unsigned");

    return to_signed_bit_cast(wide_unsigned(high, low));
  }

}
}
}
}

#endif // header
