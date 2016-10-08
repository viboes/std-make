// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_FACTORY_MAKE_HPP
#define JASEL_FUNDAMENTAL_V2_FACTORY_MAKE_HPP

#include <utility>
#include <type_traits>
#include <experimental/meta/v1/is_callable.hpp>
#include <experimental/meta/v1/decay_unwrap.hpp>
#include <experimental/meta/v1/when.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  template <class T>
    struct is_type_constructible;

#if __cplusplus >= 201402L
  template <class T>
    constexpr bool is_type_constructible_v = is_type_constructible<T>::value;
#endif

namespace type_constructible
{
  struct tag {};

  template <class T>
  struct traits_constructor : tag {
    template <class ...Xs>
    static constexpr
    auto make(Xs&& ...xs)
    -> decltype(T(std::forward<Xs>(xs)...))
    {
      return T(std::forward<Xs>(xs)...);
    }
  };

  template <class T, class Enabler=void>
    struct traits : traits<T, meta::when<true>> {};

  // Default specialization
  template <typename T, bool condition>
  struct traits<T, meta::when<condition>> : traits_constructor<T>  {};

  template <class T>
  struct traits<T*> : tag
  {
    template <class ...Xs>
    static
    auto make(Xs&& ...xs)
    -> decltype(new T(std::forward<Xs>(xs)...))
    {
      return new T(std::forward<Xs>(xs)...);
    }
  };

  // make() overload
  template <class TC, int = 0, int...>
  constexpr
  typename enable_if<
      meta::is_callable<TC(void)>::value,
      meta::invoke<TC, void>
  >::type make()
  {
    return traits<meta::invoke<TC, void>>::make();
  }

  template <template <class> class M>
  constexpr M<void> make()
  {
    return traits<M<void>>::make();
  }

  // make overload: requires a type constructor, deduce the underlying type
  template <class TC, int = 0, int..., class ...Xs>
  constexpr typename enable_if<
    meta::is_callable<TC(meta::decay_unwrap_t<Xs>...)>::value,
    meta::invoke<TC, meta::decay_unwrap_t<Xs>...>
  >::type
  make(Xs&& ...xs)
  {
    return traits<meta::invoke<TC, meta::decay_unwrap_t<Xs>...>>::make(std::forward<Xs>(xs)...);

  }

  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from Xs
  template <class M, int = 0, int..., class ...Xs>
  constexpr typename enable_if<
    ! meta::is_callable<M(meta::decay_unwrap_t<Xs>...)>::value
    , M
  >::type
  make(Xs&& ...xs)
  {
    return traits<M>::make(std::forward<Xs>(xs)...);
  }

  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, int = 0, int..., class ...Xs>
  constexpr M<meta::decay_unwrap_t<Xs>...>
  make(Xs&& ...xs)
  {
    return traits<M<meta::decay_unwrap_t<Xs>...>>::make(std::forward<Xs>(xs)...);
  }
}

using type_constructible::make;

template <class T>
struct is_type_constructible : is_base_of<type_constructible::tag, type_constructible::traits<T>> {};
template <class T>
struct is_type_constructible<const T> : is_type_constructible<T> {};
template <class T>
struct is_type_constructible<volatile T> : is_type_constructible<T> {};
template <class T>
struct is_type_constructible<const volatile T> : is_type_constructible<T> {};

}
}
}


#endif // header
