// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_UNIQUE_PTR_HPP
#define JASEL_FUNDAMENTAL_V2_UNIQUE_PTR_HPP

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/nullable.hpp>
#include <memory>


namespace std
{
#if __cplusplus <= 201103L
  template <int = 0, int..., class T>
    unique_ptr<decay_t<T>> make_unique(T&& x)
  {
    return unique_ptr<decay_t<T>>(new decay_t<T>(x));
  }

  // explicit overloads
  template <class T>
    unique_ptr<T> make_unique(remove_reference_t<T> const& x)
  {
    return unique_ptr<T>(new T(x));
  }

  template <class T>
    unique_ptr<T> make_unique(remove_reference_t<T>&& x)
  {
    return unique_ptr<T>(new T(std::forward<remove_reference_t<T>>(x)));
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
inline namespace fundamental_v2
{

  // type_constructor customization
  template <class T, class D>
  struct type_constructor<unique_ptr<T,D>> : meta::id<unique_ptr<_t, D>> {};

  template <class T, class D>
  struct none_type<unique_ptr<T,D>> : meta::id<nullptr_t> { };

  template <class T, class D>
  struct factory_traits<unique_ptr<T, D>>
  {
    template <class ...Xs>
    static //constexpr
    unique_ptr<T, D> make(Xs&& ...xs)
    {
      return make_unique<T>(forward<Xs>(xs)...);
    }
  };

  template <class T, class D>
  struct nullable_traits<unique_ptr<T, D>> : nullable_traits_pointer_like {};

}
}
}

#endif // header
