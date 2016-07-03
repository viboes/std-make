// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_TUPLE_HPP
#define JASEL_FUNDAMENTAL_V2_TUPLE_HPP

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <tuple>

namespace std
{
  // There is no way to have a variadic type constructor so we opt for a type constructor with a single type holder

  // Holder customization
  template <>
  struct tuple<experimental::_t> : experimental::meta::quote<tuple> {};

namespace experimental
{

inline namespace fundamental_v2
{
  // type_constructor customization
  template <class ...Ts>
  struct type_constructor<tuple<Ts...>> : meta::id<tuple<_t>> {};
}
}
}

#endif // header
