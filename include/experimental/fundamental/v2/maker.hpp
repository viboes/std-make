// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MAKER_HPP
#define JASEL_FUNDAMENTAL_V2_MAKER_HPP

#include <utility>
//#include <type_traits>
#include <experimental/meta/v1/deduced_type.hpp>
#include <experimental/meta/v1/always.hpp>
#include <experimental/fundamental/v2/make.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class MFC>
  struct maker_mfc
  {
    template <class ...Xs>
    constexpr meta::apply<MFC, meta::deduced_type_t<Xs>...>
    operator()(Xs&& ...xs) const
    {
      return make<MFC>(std::forward<Xs>(xs)...);
    }
  };

  template <template <class ...> class TC>
  struct maker_tc
  {
    template <class ...Xs>
    constexpr TC<meta::deduced_type_t<Xs>...>
    operator()(Xs&& ...xs) const
    {
      return make<TC>(std::forward<Xs>(xs)...);
    }
  };

  template <class M>
  struct maker_t
  {
    template <class ...Args>
    constexpr M
    operator()(Args&& ...args) const
    {
      return make<M>(std::forward<Args>(args)...);
    }
  };

  template <template <class ...> class TC>
  struct maker : maker_tc<TC> {};

}
}
}

#endif // header
