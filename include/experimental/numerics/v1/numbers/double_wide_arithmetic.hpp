// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba
// Based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0103r1.html

#ifndef JASEL_NUMERIC_V1_NUMBERS_DOUBLE_WIDE_ARITHMETIC_HPP
#define JASEL_NUMERIC_V1_NUMBERS_DOUBLE_WIDE_ARITHMETIC_HPP

#include <experimental/numerics/v1/numbers/double_wide_types.hpp>

namespace std
{
namespace experimental
{
namespace numerics
{
inline  namespace v1
{
  template <typename T>
  double_width<T> wide_neg( T a )
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return -to_double_width(a);
  }
  template <typename T>
  double_width<T> wide_add( T a, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_double_width(a) + to_double_width(b);
  }
  template <typename T>
  double_width<T> wide_sub( T a, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    return to_double_width(a) - to_double_width(b);
  }
  template <typename T>
  double_width<T> wide_add2( T a, T b, T c )
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_double_width(a) + to_double_width(b) + to_double_width(c);
  }
  template <typename T>
  double_width<T> wide_sub2( T a, T b, T c )
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_double_width(a) - to_double_width(b) - to_double_width(c);
  }

  template <typename T>
  double_width<T> wide_lsh( T a, int b )
          //[[expects: (b >= 0) && (b < width_v<T>) ]]
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return static_cast<double_width<T>>(to_double_width(a) << b);
  }

  template <typename T>
  double_width<T> wide_lshadd( T a, int b, T c )
          //[[expects: (b >= 0) && (b < width_v<T>) ]]
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return (to_double_width(a) << b) + to_double_width(c);
  }
  template <typename T>
  double_width<T> wide_lshsub( T a, int b, T c )
          //[[expects: (b >= 0) && (b < width_v<T>) ]]
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return (to_double_width(a) << b) - to_double_width(c);
  }
  template <typename T>
  double_width<T> wide_mul( T a, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_double_width(a) * to_double_width(b);
  }
  template <typename T>
  double_width<T> wide_muladd( T a, T b, T c )
           //[[expects: (a != minimum<T>()) || (b ! minimum<T>()) ]]
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_double_width(a) * to_double_width(b) + to_double_width(c);
  }
  template <typename T>
  double_width<T> wide_mulsub( T a, T b, T c )
           //[[expects: (a != minimum<T>()) || (b ! minimum<T>()) ]]
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_double_width(a) * to_double_width(b) - to_double_width(c);
  }
  template <typename T>
  double_width<T> wide_muladd2( T a, T b, T c, T d )
           //[[expects: (a != minimum<T>()) && (b ! minimum<T>()) ]]
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_double_width(a) * to_double_width(b) + to_double_width(c) + to_double_width(d);
  }
  template <typename T>
  double_width<T> wide_mulsub2( T a, T b, T c, T d )
           //[[expects: (a != minimum<T>()) && (b ! minimum<T>()) ]]
  {
    static_assert(is_integral<T>::value, "T must be integral");

    return to_double_width(a) * to_double_width(b) - to_double_width(c) - to_double_width(d);
  }
  template <typename T, typename D>
  T wide_divn( D a, T b )
          //[[expects: (b != 0)  ]]
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<D, double_width<T>>::value, "D must be double_width<T>");

    return static_cast<T>(to_half_width(a / to_double_width(b)));
  }
  template <typename T, typename D>
  D wide_divw( D a, T b )
          //[[expects: (b != 0)  ]]
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<D, double_width<T>>::value, "D must be double_width<T>");

    return a / to_double_width(b);
  }

  // use double when posible
  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_neg( UT* low, T a )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");
    double_width<T> r = -to_double_width(a);
    *low = split_lower(r);
    return split_upper(r);
  }

  // use split when there is no builtin double_width
  template <typename T, typename UT>
  enable_if_t<(sizeof(T) > 4) && is_signed<T>::value, UT> split_neg( UT* low, T a )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");
    *low = make_unsigned_t<T>(-a);
    return make_unsigned_t<T>(-1)*(a>=0);
  }
  template <typename T, typename UT >
  enable_if_t<(sizeof(T) > 4) && ! is_signed<T>::value, UT> split_neg( UT* low, T a )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");
    *low = make_unsigned_t<T>(-1)-a+1u;
    return make_unsigned_t<T>(-1);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_add( UT* low, T a, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) + to_double_width(b);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_sub( UT* low, T a, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) - to_double_width(b);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_add2( UT* low, T a, T b, T c )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) + to_double_width(b) + to_double_width(c);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_sub2( UT* low, T a, T b, T c )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) - to_double_width(b) - to_double_width(c);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) > 4), UT> split_add( UT* low, T a, T b );

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) > 4), UT> split_sub( UT* low, T a, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    if (a>=0 and b>=0)
    {
      const auto ah = split_upper(a);
      const auto al = split_lower(a);
      const auto bh = split_upper(b);
      const auto bl = split_lower(b);

      half_width<UT> low_l;
      const auto c1 = split_sub(&low_l, al, bl);
      half_width<UT> low_h;
      const auto c2 = split_sub2(&low_h, ah, bh, c1);
      *low = wide_unsigned(low_h, low_l);
      return wide_unsigned(half_width<UT>(0), c2);
    }
    //assert(false);
    if (a<0 and b<0)
      return split_sub(low, b, -a);
    if (a>=0 and b<0)
      return split_add(low, a, -b);
    //if (a<0 and b>=0)
    return UT(-split_add(low, -a, b));
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) > 4), UT> split_add( UT* low, T a, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    if (a<0 and b<0)
      return UT(-split_add(low, -a, -b));
    if (a>=0 and b<0)
      return split_sub(low, a, b);
    if (a<0 and b>=0)
      return split_sub(low, b, -a);

    //if (a>=0 and b>=0)
    const auto ah = split_upper(a);
    const auto al = split_lower(a);
    const auto bh = split_upper(b);
    const auto bl = split_lower(b);

    half_width<UT> low_l;
    const auto c1 = split_add(&low_l, al, bl);
    half_width<UT> low_h;
    const auto c2 = split_add2(&low_h, ah, bh, c1);
    *low = wide_unsigned(low_h, low_l);
    return wide_unsigned(half_width<UT>(0), c2);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_lsh( UT* low, T a, int b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) << b;
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_lshadd( UT* low, T a, int b, T c )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = (to_double_width(a) << b) + to_double_width(c);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_lshsub( UT* low, T a, int b, T c )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = (to_double_width(a) << b) - to_double_width(c);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_mul( UT* low, T a, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) * to_double_width(b);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_muladd( UT* low, T a, T b, T c )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) * to_double_width(b) + to_double_width(c);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_mulsub( UT* low, T a, T b, T c )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) * to_double_width(b) - to_double_width(c);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_muladd2( UT* low, T a, T b, T c, T d )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) * to_double_width(b) + to_double_width(c) + to_double_width(d);
    *low = split_lower(r);
    return split_upper(r);
  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_mulsub2( UT* low, T a, T b, T c, T d )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");

    double_width<T> r = to_double_width(a) * to_double_width(b) - to_double_width(c) - to_double_width(d);
    *low = split_lower(r);
    return split_upper(r);
  }
#if 0
  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), T> split_divn( UT* low, UT ah, UT al, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");


  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_divw( UT* low, UT ah, UT al, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");


  }

  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), T> split_divnrem( T* remainder, UT ah, UT al, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");


  }
  template <typename T, typename UT>
  enable_if_t< (sizeof(T) <= 4), UT> split_divwrem( UT* low, T* remainder, UT ah, UT al, T b )
  {
    static_assert(is_integral<T>::value, "T must be integral");
    static_assert(is_same<UT, make_unsigned_t<T>>::value, "UT must be make_unsigned_t<T>");


  }
#endif
}
}
}
}

#endif // header
