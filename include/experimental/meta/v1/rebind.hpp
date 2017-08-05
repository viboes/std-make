//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_REBIND_HPP
#define JASEL_META_V1_REBIND_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  template <class M, class ...U>
  struct rebind : id<typename M::template rebind<U...>> {};

  // This specialization inhibits specializations for for example rebind<expected<T,E>, U>
  //template <template<class ...> class TC, class ...Ts, class ...Us>
  //struct rebind<TC<Ts...>, Us...> : id<TC<Us...>> {};

  template <class M, class ...Us>
  using rebind_t = eval<rebind<M, Us...>>;

}
}
}
}

#endif // header
