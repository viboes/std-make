// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_SHARED_PTR_HPP
#define JASEL_FUNDAMENTAL_V3_SHARED_PTR_HPP

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/nullable.hpp>
#include <experimental/functor.hpp>
#include <memory>

namespace std
{
  // Holder customization
  template <>
  struct shared_ptr<experimental::_t> : experimental::meta::quote<shared_ptr> {};

namespace experimental
{
inline namespace fundamental_v3
{

  // type_constructor customization
  template <class T>
  struct type_constructor<shared_ptr<T>> : meta::id<shared_ptr<_t>> {};

namespace type_constructible {
  template <class T>
  struct traits<shared_ptr<T>> : tag
  {

    template <class ...Xs>
    static //constexpr
    shared_ptr<T> make(Xs&& ...xs)
    {
      return make_shared<T>(forward<Xs>(xs)...);
    }
  };
}

namespace nullable {
  template <class T>
  struct traits<shared_ptr<T>> : traits_pointer_like {};
}
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
namespace functor {
  template <>
  struct traits<shared_ptr<_t>> : nullable::as_functor {};
}
#endif
}
}
}

#endif // header
