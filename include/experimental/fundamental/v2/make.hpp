// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MAKE_HPP
#define JASEL_FUNDAMENTAL_V2_MAKE_HPP

#include <utility>
#include <type_traits>
#include <experimental/meta/v1/is_callable.hpp>
#include <experimental/meta/v1/decay_unwrap.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
  template <class T>
  struct factory_traits_default {
    template <class ...Xs>
    static constexpr
    auto make(Xs&& ...xs)
    -> decltype(T(std::forward<Xs>(xs)...))
    {
      return T(std::forward<Xs>(xs)...);
    }
  };

  template <class T>
  struct factory_traits : factory_traits_default<T> {};

  template <class T>
  struct factory_traits<T*>
  {
    template <class ...Xs>
    static
    auto make(Xs&& ...xs)
    -> decltype(new T(std::forward<Xs>(xs)...))
    {
      return new T(std::forward<Xs>(xs)...);
    }
  };


inline namespace factories
{
  // make() overload
  template <class TC, int = 0, int...>
  constexpr
  typename enable_if<
      meta::is_callable<TC(void)>::value,
      meta::invoke<TC, void>
  >::type make()
  {
    return factory_traits<meta::invoke<TC, void>>::make();
  }

  template <template <class> class M>
  constexpr M<void> make()
  {
    return factory_traits<M<void>>::make();
  }

  // make overload: requires a type constructor, deduce the underlying type
  template <class TC, int = 0, int..., class ...Xs>
  constexpr typename enable_if<
    meta::is_callable<TC(meta::decay_unwrap_t<Xs>...)>::value,
    meta::invoke<TC, meta::decay_unwrap_t<Xs>...>
  >::type
  make(Xs&& ...xs)
  {
    return factory_traits<meta::invoke<TC, meta::decay_unwrap_t<Xs>...>>::make(std::forward<Xs>(xs)...);

  }

  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from Xs
  template <class M, int = 0, int..., class ...Xs>
  constexpr typename enable_if<
    ! meta::is_callable<M(meta::decay_unwrap_t<Xs>...)>::value
    , M
  >::type
  make(Xs&& ...xs)
  {
    return factory_traits<M>::make(std::forward<Xs>(xs)...);
  }

  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, int = 0, int..., class ...Xs>
  constexpr M<meta::decay_unwrap_t<Xs>...>
  make(Xs&& ...xs)
  {
    return factory_traits<M<meta::decay_unwrap_t<Xs>...>>::make(std::forward<Xs>(xs)...);
  }
}
}
}
}


#endif // header
