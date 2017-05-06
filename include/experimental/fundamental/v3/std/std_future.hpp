// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_FUTURE_HPP
#define JASEL_FUNDAMENTAL_V3_FUTURE_HPP

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
    using invoke = future<remove_reference_t<T>& ...>;
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

  // deduced type
  template <int = 0, int..., class T>
  future<experimental::meta::decay_unwrap_t<T>> make_ready_future(T&& x)
  {
    promise<experimental::meta::decay_unwrap_t<T>> p;
    p.set_value(forward<T>(x));
    return p.get_future();
  }

  // forced type
  template <class T, int = 0, int..., class U, typename = enable_if_t<is_same<T,U>::value> >
  future<T> make_ready_future(U x)
  {
    promise<T> p;
    p.set_value(forward<T>(x));
    return p.get_future();
  }

  // emplace variadic overload - non-deduced
  template <class T, int = 0, int..., class ...Args>
    future<T> emplace_ready_future(Args&&... args)
  {
    promise<T> p;
    p.set_value(T(forward<Args>(args)...));
    return p.get_future();

  }
//  template <class T, class ...Args>
//    future<T> make_ready_future(Args&&... args)
//  {
//    promise<T> p;
//    p.set_value(T(forward<Args>(args)...));
//    return p.get_future();
//
//  }
}

  // type_constructor customization
  template <class T>
  struct type_constructor<future<T>> : meta::id<future<_t>> {};
  template <class T>
  struct type_constructor<future<T&>> : meta::id<future<_t&>> {};

  template <class T>
  struct type_constructor<shared_future<T>> : meta::id<shared_future<_t>> {};
  template <class T>
  struct type_constructor<shared_future<T&>> : meta::id<shared_future<_t&>> {};

inline namespace fundamental_v3 {
namespace type_constructible {

  template <class T>
  struct traits<future<T>> {
    template <class M, class ...Xs>
    static
    auto make(Xs&& ...xs)
    JASEL_DECLTYPE_RETURN_NOEXCEPT(
      make_ready_future(forward<Xs>(xs)...)
    )
    template <class M, class ...Xs>
    static
    M emplace(Xs&& ...xs)
    {
      return emplace_ready_future<T>(forward<Xs>(xs)...);
    }
  };
  template <>
  struct traits<future<void>> {
    template <class M>
    static
    M make()
    {
      return make_ready_future();
    }
    template <class M>
    static
    M emplace()
    {
      return make_ready_future();
    }
  };


  template <class T>
  struct traits<shared_future<T>> {
    template <class M, class ...Xs>
    static
    M make(Xs&& ...xs)
    {
      return make_ready_future(forward<Xs>(xs)...).share();
    }
    template <class M, class ...Xs>
    static
    M emplace(Xs&& ...xs)
    {
      return emplace_ready_future<T>(forward<Xs>(xs)...).share();
    }
  };

  template <>
  struct traits<shared_future<void>> {
    template <class M>
    static
    M make()
    {
      return make_ready_future().share();
    }
    template <class M>
    static
    M emplace()
    {
      return make_ready_future().share();
    }
  };
}
}
}
}

#endif // header
