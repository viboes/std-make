// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_PAIR_HPP
#define JASEL_FUNDAMENTAL_V3_PAIR_HPP

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/n_functor.hpp>
#include <experimental/n_applicative.hpp>
#include <experimental/product_type.hpp>
#include <utility>

namespace std
{
  // Holder customization
  template <>
  struct pair<experimental::_t, experimental::_t>
  {
    template <class ...Ts>
    using invoke = pair<Ts...>;
  };

namespace experimental
{
inline namespace fundamental_v3
{
  using pair_tc = pair<experimental::_t, experimental::_t>;

  // type_constructor customization
  template <class T1, class T2>
  struct type_constructor<pair<T1,T2>> : meta::id<pair_tc> {};

  template <>
  struct type_constructor<meta::quote<pair>> : meta::id<pair_tc> {};

#if __cplusplus >= 201402L

  namespace n_functor {
    template <>
    struct traits<pair<_t,_t>> : product_type::as_n_functor {};
  }
  namespace n_applicative {
    template <>
    struct traits<pair<_t,_t>> : product_type::as_n_applicative {};
  }
#endif
}
}
}

#endif // header
