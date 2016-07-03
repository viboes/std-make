// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_FUTURE_HPP
#define JASEL_FUNDAMENTAL_V2_FUTURE_HPP

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <future>

namespace std
{
  // Holder customization
  template <>
  struct future<experimental::_t> : experimental::meta::quote<future> {};

  template <>
  struct future<experimental::_t&>
  {
    template<class ...T>
    using invoke = future<T& ...>;
  };

  // Holder specialization
  template <>
  struct shared_future<experimental::_t> : experimental::meta::quote<shared_future> {};

  template <>
  struct shared_future<experimental::_t&>
  {
    template<class ...T>
    using invoke = shared_future<T& ...>;
  };

namespace experimental
{

inline namespace concurrency_v1
{

  future<void> make_ready_future()
  {
    promise<void> p;
    p.set_value();
    return p.get_future();
  }

  template <int = 0, int..., class T>
    future<experimental::meta::decay_unwrap_t<T>> make_ready_future(T&& x)
  {
    promise<experimental::meta::decay_unwrap_t<T>> p;
    p.set_value(forward<T>(x));
    return p.get_future();
  }

  // explicit overloads
  template <class T>
    future<T> make_ready_future(remove_reference_t<T> const& x)
  {
    promise<T> p;
    p.set_value(x);
    return p.get_future();
  }

  template <class T>
    future<T> make_ready_future(remove_reference_t<T>&& x)
  {
    promise<T> p;
    p.set_value(forward<remove_reference_t<T>>(x));
    return p.get_future();
  }

  // variadic overload
  template <class T, class ...Args>
    future<T> make_ready_future(Args&&... args)
  {
    promise<T> p;
    p.set_value(T(forward<Args>(args)...));
    return p.get_future();

  }
}

inline namespace fundamental_v2
{
  // type_constructor customization
  template <class T>
  struct type_constructor<future<T>> : meta::id<future<_t>> {};
  template <class T>
  struct type_constructor<future<T&>> : meta::id<future<_t&>> {};

  template <class T>
  struct type_constructor<shared_future<T>> : meta::id<shared_future<_t>> {};
  template <class T>
  struct type_constructor<shared_future<T&>> : meta::id<shared_future<_t&>> {};

  template <class T>
  struct factory_traits<future<T>> {

    template <class ...Xs>
    static
    future<T> make(Xs&& ...xs)
    {
      return make_ready_future<T>(forward<Xs>(xs)...);
    }
  };
  template <>
  struct factory_traits<future<void>> {

    static
    future<void> make()
    {
      return make_ready_future();
    }
  };


  template <class T>
  struct factory_traits<shared_future<T>> {

    template <class ...Xs>
    static
    shared_future<T> make(Xs&& ...xs)
    {
      promise<T> p;
      p.set_value(T(forward<Xs>(xs)...));
      return p.get_future().share();
    }
  };
  template <>
  struct factory_traits<shared_future<void>> {

    static
    shared_future<void> make()
    {
      promise<void> p;
      p.set_value();
      return p.get_future().share();
    }
  };
}
}
}

#endif // header
