// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_ARRAY_HPP
#define JASEL_FUNDAMENTAL_V3_ARRAY_HPP

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <array>
#include <type_traits>

namespace std
{

namespace experimental
{

  struct array_tc
  {
      template <class ...T>
      using invoke = array<typename common_type<typename decay<T>::type...>::type, sizeof...(T)>;
  };
  // type_constructor customization
  template <class T, size_t N>
  struct type_constructor<array<T, N>> : meta::id<array_tc> {};

  template <class T, size_t N>
  struct factory_traits<array<T, N>>
  {
    template <class ...Xs>
    static constexpr
    array<T, sizeof...(Xs)> make(Xs&& ...xs)
    {
      return {{forward<Xs>(xs)...}};
    }
  };
}
}

#endif // header
