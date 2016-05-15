// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MAKE_HPP
#define JASEL_FUNDAMENTAL_V2_MAKE_HPP

#include <utility>
#include <type_traits>
#include <experimental/meta/v1/types.hpp>
#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/is_invokable.hpp>
#include <experimental/meta/v1/type_constructor.hpp>
#include <experimental/meta/v1/deduced_type.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  // make() overload

  template <class TC, int = 0, int...>
  constexpr
  typename enable_if<
      meta::is_invokable<TC, void>::value,
      meta::invoke<TC, void>
  >::type
  make()
  {
    return make_custom(meta::id<meta::invoke<TC, void>>{});
  }

  template <template <class> class M>
  constexpr M<void> make()
  {
    //return make_custom(meta::id<M<void>>{});
    return make<meta::type_constructor_t<meta::quote<M>>>();

  }

  // make overload: requires a type constructor, deduce the underlying type
  template <class TC, int = 0, int..., class ...Xs>
  constexpr typename enable_if<
    meta::is_invokable<TC, meta::deduced_type_t<Xs>...>::value,
    meta::invoke<TC, meta::deduced_type_t<Xs>...>
  >::type
  make(Xs&& ...xs)
  {
    return make_custom(meta::id<meta::invoke<TC, meta::deduced_type_t<Xs>...>>{}, std::forward<Xs>(xs)...);
  }

  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from Xs
  template <class M, int = 0, int..., class ...Xs>
  constexpr typename enable_if<
    ! meta::is_invokable<M, meta::deduced_type_t<Xs>...>::value
    , M
  >::type
  make(Xs&& ...xs)
  {
    return make_custom(meta::id<M>{}, std::forward<Xs>(xs)...);
  }

  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, int = 0, int..., class ...Xs>
  constexpr M<meta::deduced_type_t<Xs>...>
  make(Xs&& ...xs)
  {
    return make<meta::type_constructor_t<meta::quote<M>>>(std::forward<Xs>(xs)...);
  }
}
}
}
namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  // default customization point for constructor from Xs...
  template <class M, class ...Xs>
  constexpr auto
  make_custom(meta::id<M>, Xs&& ...xs)
  -> decltype(M(std::forward<Xs>(xs)...))
//  -> decltype(auto)
  {
    return M(std::forward<Xs>(xs)...);
  }

  // default customization point for constructor from Xs...
  template <class T, class ...Xs>
  constexpr auto
  make_custom(meta::id<T*>, Xs&& ...xs)
  -> decltype(new T(std::forward<Xs>(xs)...))
//  -> decltype(auto)
  {
    return new T(std::forward<Xs>(xs)...);
  }
}
}
}
}

#endif // header
