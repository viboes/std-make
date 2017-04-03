// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_FACTORY_MAKER_HPP
#define JASEL_FUNDAMENTAL_V3_FACTORY_MAKER_HPP

#include <utility>
#include <experimental/fundamental/v3/factory/make.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace type_constructible {
#if ! defined JASEL_DOXYGEN_INVOKED
namespace detail_maker
{

  template <class TC>
  struct maker_tc
  {
    template <class ...Xs>
    constexpr auto
    operator()(Xs&& ...xs) const
    -> decltype(make<TC>(std::forward<Xs>(xs)...))
    {
      return make<TC>(std::forward<Xs>(xs)...);
    }
  };

  template <template <class ...> class TC>
  struct maker_tmpl
  {
    template <class ...Xs>
    constexpr auto
    operator()(Xs&& ...xs) const
    -> decltype(make<TC>(std::forward<Xs>(xs)...))
    {
      return make<TC>(std::forward<Xs>(xs)...);
    }
  };

  template <class M>
  struct maker_t
  {
    template <class ...Args>
    constexpr auto
    operator()(Args&& ...args) const
    -> decltype(make<M>(std::forward<Args>(args)...))
    {
      return make<M>(std::forward<Args>(args)...);
    }
  };
}
#endif
  template <class T>
#if ! defined JASEL_DOXYGEN_INVOKED
  typename enable_if<
      ! is_type_constructor<T>::value
      , detail_maker::maker_t<T>
    >::type
#else
    decltype(auto)
#endif
    maker() { return detail_maker::maker_t<T>{}; }

  template <class TC>
#if ! defined JASEL_DOXYGEN_INVOKED
  typename enable_if<
      is_type_constructor<TC>::value
      , detail_maker::maker_tc<TC>
    >::type
#else
    auto
#endif
    maker() { return detail_maker::maker_tc<TC>{}; }

  template <template <class ...> class TC>
#if ! defined JASEL_DOXYGEN_INVOKED
  detail_maker::maker_tmpl<TC>
#else
    auto
#endif
  maker() { return detail_maker::maker_tmpl<TC>{}; }
}
using type_constructible::maker;

}
}
}

#endif // header
