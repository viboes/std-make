// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_SUM_HPP
#define JASEL_FUNDAMENTAL_V2_SUM_HPP

#include <experimental/fundamental/v2/newtype.hpp>
#include <experimental/fundamental/v2/monoid.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class T>
  using sum = newtype<T, struct sum_tag
      // deriving Num
      >;

#if 0
  template <class T>
  // requires Num<T>
  struct monoid::instance<product<T>> {
    static sum<T> monoid_id_impl(meta::type<product<T>>)
    {
      return sum<T>(0);
    }

    static sum<T> monoid_op_impl(sum<T> const& x, sum<T> const& y)
    {
      //return x + y; if product derivesNumoperations
      return sum<T>(T(x) + T(y));
    }

  };
#else
  template <class T>
  // requires Num<T>
  sum<T> monoid_id_custom(meta::type<sum<T>>, meta::type<sum<T>>)
  {
    return sum<T>(0);
  }

  template <class T>
  sum<T> monoid_op_custom(meta::type<sum<T>>, sum<T> const& x, sum<T> const& y)
  {
    //return x + y; if product<T> derives Num<T> operations
    return sum<T>(T(x) + T(y));
  }
#endif
}
}
}

#endif // header
