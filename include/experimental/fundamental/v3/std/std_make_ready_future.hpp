// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016-2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_FUTURE_STD_MAKE_READY_FUTURE_HPP
#define JASEL_FUNDAMENTAL_V3_FUTURE_STD_MAKE_READY_FUTURE_HPP

// todo move this to concurrency/v1
#include <future>

namespace std
{
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

  // deduced type
  template <int = 0, int..., class T>
  future<experimental::meta::decay_unwrap_t<T>> make_ready_future(T&& x)
  {
    promise<experimental::meta::decay_unwrap_t<T>> p;
    p.set_value(forward<T>(x));
    return p.get_future();
  }

  future<void> emplace_ready_future()
  {
    promise<void> p;
    p.set_value();
    return p.get_future();
  }

  template <class T, typename = enable_if_t<is_void<T>::value>>
  future<void> emplace_ready_future()
  {
    promise<void> p;
    p.set_value();
    return p.get_future();
  }

  template <int = 0, int..., class T>
  future<experimental::meta::decay_unwrap_t<T>> emplace_ready_future(T&& x)
  {
    promise<experimental::meta::decay_unwrap_t<T>> p;
    p.set_value(forward<T>(x));
    return p.get_future();
  }

  // emplace variadic overload - non-deduced
  template <class T, class ...Args, typename = enable_if_t<is_constructible<T, Args...>::value> >
    future<T> emplace_ready_future(Args&&... args)
  {
    promise<T> p;
    p.set_value(T(forward<Args>(args)...));
    return p.get_future();
  }

  // emplace variadic overload - non-deduced
  template <class T, class U, class ...Args, typename = enable_if_t<is_constructible<T, Args...>::value> >
    future<T> emplace_ready_future(initializer_list<U> il, Args&&... args)
  {
    promise<T> p;
    p.set_value(T(il, forward<Args>(args)...));
    return p.get_future();
  }

}
}
}

#endif // header
