// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_UNIQUE_PTR_HPP
#define JASEL_FUNDAMENTAL_V3_UNIQUE_PTR_HPP

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/nullable.hpp>
#include <experimental/value_or_none.hpp>
#include <experimental/functor.hpp>
#include <experimental/type_traits.hpp>
#include <memory>

namespace std
{
#if __cplusplus <= 201103L || defined JASEL_DOXYGEN_INVOKED
  template <int = 0, int..., class T>
    unique_ptr<typename decay<T>::type> make_unique(T&& x)
  {
    return unique_ptr<typename decay<T>::type>(new typename decay<T>::type(x));
  }

  // explicit overloads
  template <class T>
    unique_ptr<T> make_unique(typename remove_reference<T>::type const& x)
  {
    return unique_ptr<T>(new T(x));
  }

  template <class T>
    unique_ptr<T> make_unique(typename remove_reference<T>::type&& x)
  {
    return unique_ptr<T>(new T(forward<typename remove_reference<T>::type>(x)));
  }

  // variadic overload
  template <class T, class ...Args>
    unique_ptr<T> make_unique(Args&&... args)
  {
    return unique_ptr<T>(new T(forward<Args>(args)...));
  }
#endif

  // Holder customization
  template <class D>
  struct unique_ptr<experimental::_t, D>
  {
      template <class ...T>
      using invoke = unique_ptr<T..., experimental::meta::rebind_t<D, T...>>;
  };


//  template <>
//  struct default_delete<experimental::_t> : experimental::meta::quote<default_delete> {};

namespace experimental
{
  namespace meta
  {
    template <class T, class U>
    struct rebind<default_delete<T>, U> : id<default_delete<U>> {};
  }
inline namespace fundamental_v3
{
  // type_constructor customization
  template <class T, class D>
  struct type_constructor<unique_ptr<T,D>> : meta::id<unique_ptr<_t, D>> {};

namespace type_constructible {
  template <class T, class D>
  struct traits<unique_ptr<T, D>> : tag
  {
    template <class M, class ...Xs>
    static //constexpr
    M make(Xs&& ...xs)
    {
      // fixme M doesn't works here
      return make_unique<T>(forward<Xs>(xs)...);
    }
  };
}

namespace nullable {
  template <class T, class D>
  struct traits<unique_ptr<T, D>> : traits_pointer_like {};
}

namespace value_or_none {
  template <class T, class D>
  struct traits<unique_ptr<T, D>> : traits_pointer_like {};
}
}
}
}

#endif // header
