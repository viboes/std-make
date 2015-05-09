// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MAKER_HPP
#define JASEL_FUNDAMENTAL_V2_MAKER_HPP

#include <utility>
#include <type_traits>
#include <experimental/fundamental/v1/in_place.hpp>
#include <experimental/fundamental/v2/holder.hpp>
#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/types.hpp>
#include <experimental/meta/v1/type.hpp>
#include <experimental/meta/v1/template_class.hpp>
#include <experimental/meta/v1/deduced_type.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/meta/v1/always.hpp>
#include <experimental/meta/v1/void_.hpp>
#include <experimental/meta/v1/is_applicable_with.hpp>
#include <experimental/meta/v1/rebind.hpp>
#include <experimental/meta/v1/quote.hpp>
#include <experimental/meta/v1/lift.hpp>
#include <experimental/meta/v1/reverse_lift.hpp>
#include <experimental/meta/v1/type_constructor.hpp>
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

#endif // VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_HPP
