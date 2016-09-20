// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_SHARED_PTR_HPP
#define JASEL_FUNDAMENTAL_V2_SHARED_PTR_HPP

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/nullable.hpp>
#include <memory>

namespace std
{
  // Holder customization
  template <>
  struct shared_ptr<experimental::_t> : experimental::meta::quote<shared_ptr> {};

namespace experimental
{
//inline namespace fundamental_v2
//{

  // type_constructor customization
  template <class T>
  struct type_constructor<shared_ptr<T>> : meta::id<shared_ptr<_t>> {};

  template <class T>
  struct none_type<shared_ptr<T>> : meta::id<nullptr_t> { };

  template <class T>
  struct factory_traits<shared_ptr<T>> {

    template <class ...Xs>
    static //constexpr
    shared_ptr<T> make(Xs&& ...xs)
    {
      return make_shared<T>(forward<Xs>(xs)...);
    }
  };
  template <class T>
  struct nullable_traits<shared_ptr<T>> : nullable_traits_pointer_like {};

//}
}
}

#endif // header
