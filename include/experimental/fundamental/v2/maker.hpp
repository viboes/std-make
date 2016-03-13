// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MAKER_HPP
#define JASEL_FUNDAMENTAL_V2_MAKER_HPP

#include <utility>
#include <experimental/fundamental/v2/make.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

//  template <class MFC>
//  struct maker_mfc
//  {
//    template <class ...Xs>
//    constexpr auto
//    operator()(Xs&& ...xs) const
//    -> decltype(make<MFC>(std::forward<Xs>(xs)...))
//    {
//      return make<MFC>(std::forward<Xs>(xs)...);
//    }
//  };

  template <template <class ...> class TC>
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

  template <class T>
  maker_t<T> maker() { return maker_t<T>{}; }

  template <template <class ...> class TC>
  maker_tc<TC> maker() { return maker_tc<TC>{}; }

//  template <template <class ...> class TC>
//  using maker = maker_tc<TC>;

}
}
}

#endif // header
