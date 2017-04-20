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
  template < class T >
  struct make_signed : std::make_signed<T> {};
  template <class T>
  using signed_ = typename make_signed<T>::type;

  template <class T>
  signed_<T> to_signed(T x) { return static_cast<signed_<T>>(x); }

  template < class T >
  struct make_unsigned : std::make_unsigned<T> {};
  template <class T>
  using unsigned_ = typename make_unsigned<T>::type;
  template <class T>
  unsigned_<T> to_unsigned(T x) { return static_cast<unsigned_<T>>(x); }

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

  template <class T>
  using double_width = typename make_double_width<T>::type;
  template <class T>
  double_width<T> to_double(T x) { return double_width<T>(x); }

  template < class T >
  struct make_half_width;
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

  template <class T>
  using half_width = typename make_half_width<T>::type;
  template <class T>
  half_width<T> to_half(T x) { return static_cast<half_width<T>>(x); }


  typedef int single_sword; // this is implementation defined
  typedef unsigned_<single_sword> single_uword;
  typedef double_width<single_sword> double_sword;
  typedef double_width<single_uword> double_uword;

  template <typename T>
  half_width<unsigned_<T>> split_upper( T a )
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_half(to_unsigned(a) >> (CHAR_BIT * sizeof(T) / 2));
  }
  template <typename T>
  half_width<unsigned_<T>> split_lower( T a )
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_half(to_unsigned(a));
  }

  template <typename T>
  unsigned_<double_width<T>> wide_unsigned( T high, T low )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_unsigned<T>::value, "T must be integral");

    return (to_double(high) << (CHAR_BIT * sizeof(T)))  | to_double(low);
  }
  template <typename T>
  signed_<double_width<T>> wide_signed( T high, T low )
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
