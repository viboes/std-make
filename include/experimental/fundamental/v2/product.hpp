// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_PRODUCT_HPP
#define JASEL_FUNDAMENTAL_V2_PRODUCT_HPP

#include <experimental/fundamental/v2/newtype.hpp>
#include <experimental/fundamental/v2/monoid.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class T>
  using product = newtype<T, struct product_tag
      // deriving Num
      >;
#if 0
  template <class T>
  // requires Num<T>
  struct monoid::instance<product<T>> {
    static product<T> monoid_id_impl(meta::type<product<T>>)
    {
      return product<T>(1);
    }

    static product<T> monoid_op_impl(product<T> const& x, product<T> const& y)
    {
      //return x * y; if product<T> derives Num<T> operations
      return product<T>(T(x) * T(y));
    }
  };
#else
  template <class T>
  // requires Num<T>
  product<T> monoid_id_custom(meta::type<product<T>>, meta::type<product<T>>)
  {
    return product<T>(1);
  }

  template <class T>
  product<T> monoid_op_custom(meta::type<product<T>>, product<T> const& x, product<T> const& y)
  {
    //return x * y; if product<T> derives Num<T> operations
    return product<T>(T(x) * T(y));
  }
#endif
}
}
}

#endif // header
