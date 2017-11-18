// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_FACTORY_MAKE_HPP
#define JASEL_FUNDAMENTAL_V2_FACTORY_MAKE_HPP

#include <utility>
#include <experimental/type_traits.hpp>
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

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
    constexpr bool is_type_constructible_v = is_type_constructible<T>::value;
#endif

namespace type_constructible
{
  namespace detail {
      struct not_a_type_constructible_tag{};
  }
  template <typename T>
  struct traits_constructor {
      template <class M, class ...Xs>
      static constexpr
      auto make(Xs&& ...xs)
      -> decltype(M(std::forward<Xs>(xs)...))
      {
        return M(std::forward<Xs>(xs)...);
      }
  };
  template <class T, class Enabler=void>
    struct traits
#if ! defined JASEL_DOXYGEN_INVOKED
        : traits<T, meta::when<true>> {}
#endif
        ;

  // Default specialization
  // todo enable if constructible
  template <typename T, bool condition>
  struct traits<T, meta::when<condition>> : traits_constructor<T> {};

//  template <class T>
//  struct traits<T*>
//  {
//    template <class M, class ...Xs>
//    static
//    auto make(Xs&& ...xs)
//    -> decltype(new T(std::forward<Xs>(xs)...))
//    {
//      return new T(std::forward<Xs>(xs)...);
//    }
//  };

  // make() overload
  template <class TC, int = 0, int...>
  constexpr
  enable_if_t<
      meta::is_callable<TC(void)>::value,
      meta::invoke<TC, void>
  > make()
  {
    using M = meta::invoke<TC, void>;
    return traits<M>::template make<M>();
  }

  template <template <class> class TC>
  constexpr TC<void> make()
  {
    using M = TC<void>;
    return traits<M>::template make<M>();
  }

  // make overload: requires a type constructor, deduces the underlying type
  template <class TC, class ...Xs>
  constexpr enable_if_t<
    meta::is_callable<TC(meta::decay_unwrap_t<Xs>...)>::value,
    meta::invoke<TC, meta::decay_unwrap_t<Xs>...>
  >
  make(Xs&& ...xs)
  {
    using M = meta::invoke<TC, meta::decay_unwrap_t<Xs>...>;
    return traits<M>::template make<M>(std::forward<Xs>(xs)...);
  }

  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from Xs
  template <class M, class ...Xs>
  constexpr enable_if_t<
    ! meta::is_callable<M(meta::decay_unwrap_t<Xs>...)>::value
    , M
  >
  make(Xs&& ...xs)
  {
    return traits<M>::template make<M>(std::forward<Xs>(xs)...);
  }

  // make overload: requires a template class, deduces the underlying type
  template <template <class ...> class M, class ...Xs>
  constexpr M<meta::decay_unwrap_t<Xs>...>
  make(Xs&& ...xs)
  {
    using N = M<meta::decay_unwrap_t<Xs>...>;
    return traits<N>::template make<N>(std::forward<Xs>(xs)...);
  }

  // emplace() overload
  template <class TC>
  constexpr
  enable_if_t<
      meta::is_callable<TC(void)>::value,
      meta::invoke<TC, void>
  > emplace()
  {
    using M = meta::invoke<TC, void>;
    return traits<M>::template emplace<M>();
  }

  template <template <class> class TC>
  constexpr TC<void> emplace()
  {
    using M = TC<void>;
    return traits<M>::template emplace<M>();
  }

  // make overload: requires a type constructor, deduces the underlying type
  template <class TC, class ...Xs>
  constexpr enable_if_t<
    meta::is_callable<TC(meta::decay_unwrap_t<Xs>...)>::value,
    meta::invoke<TC, meta::decay_unwrap_t<Xs>...>
  >
  emplace(Xs&& ...xs)
  {
    using M = meta::invoke<TC, meta::decay_unwrap_t<Xs>...>;
    return traits<M>::template emplace<M>(std::forward<Xs>(xs)...);
  }
  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from Xs
  template <class M, class ...Xs>
  constexpr enable_if_t<
    ! meta::is_callable<M(meta::decay_unwrap_t<Xs>...)>::value
    , M
  >
  emplace(Xs&& ...xs)
  {
    return traits<M>::template emplace<M>(std::forward<Xs>(xs)...);
  }

  // make overload: requires a template class, deduces the underlying type
  template <template <class ...> class M, class ...Xs>
  constexpr M<meta::decay_unwrap_t<Xs>...>
  emplace(Xs&& ...xs)
  {
    using N = M<meta::decay_unwrap_t<Xs>...>;
    return traits<N>::template emplace<N>(std::forward<Xs>(xs)...);
  }
}

using type_constructible::make;
using type_constructible::emplace;

template <class T>
struct is_type_constructible
#if ! defined JASEL_DOXYGEN_INVOKED
    : integral_constant<bool,
          ! is_base_of<type_constructible::detail::not_a_type_constructible_tag, type_constructible::traits<T>>::value
      > {}

#endif
    ;
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
