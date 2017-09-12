// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba
// Based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0103r1.html

#ifndef JASEL_NUMERIC_V1_NUMBERS_DOUBLE_WIDE_TYPES_HPP
#define JASEL_NUMERIC_V1_NUMBERS_DOUBLE_WIDE_TYPES_HPP

#include <cstdint>
#include <climits>
#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
namespace numerics
{
inline  namespace v1
{
  // fixme: Why to redefine it?
  template < class T >
  struct make_signed : std::make_signed<T> {};
  template <class T>
  using make_signed_t = typename make_signed<T>::type;

  template <class T>
  make_signed_t<T> to_signed(T x) { return static_cast<make_signed_t<T>>(x); }

  // fixme: Why to redefine it?
  template < class T >
  struct make_unsigned : std::make_unsigned<T> {};
  template <class T>
  using make_unsigned_t = typename make_unsigned<T>::type;
  template <class T>
  make_unsigned_t<T> to_unsigned(T x) { return static_cast<make_unsigned_t<T>>(x); }

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
  struct make_double_width <int64_t> { using type = int128_t; };
  template <  >
  struct make_double_width <uint64_t> { using type = uint128_t; };
#endif

  // fixme: Why not make_double_width_t?
  template <class T>
  using double_width = typename make_double_width<T>::type;
  template <class T>
  using make_double_width_t = typename make_double_width<T>::type;
  template <class T>
  double_width<T> to_double_width(T x) { return double_width<T>(x); }

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
  half_width<T> to_half_width(T x) { return static_cast<half_width<T>>(x); }


  typedef int single_sword; // this is implementation defined
  typedef make_unsigned_t<single_sword> single_uword;
  typedef double_width<single_sword> double_sword;
  typedef double_width<single_uword> double_uword;

  template <typename T>
  half_width<make_unsigned_t<T>> split_upper( T a )
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return static_cast<half_width<make_unsigned_t<T>>>(to_half_width(to_unsigned(a) >> (CHAR_BIT * sizeof(T) / 2)));
  }
  template <typename T>
  half_width<make_unsigned_t<T>> split_lower( T a )
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_half_width(to_unsigned(a));
  }

  template <typename T>
  make_unsigned_t<double_width<T>> wide_unsigned( T high, T low )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_unsigned<T>::value, "T must be integral");

    return (to_double_width(high) << (CHAR_BIT * sizeof(T)))  | to_double_width(low);
  }
  template <typename T>
  make_signed_t<double_width<T>> wide_signed( T high, T low )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_unsigned<T>::value, "T must be integral");

    return to_signed(wide_unsigned(high, low));
  }

}
}
}
}

#endif // header
