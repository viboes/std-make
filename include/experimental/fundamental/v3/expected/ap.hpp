//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_EXPECTED_AP_HPP
#define JASEL_FUNDAMENTAL_V3_EXPECTED_AP_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/expected/expected.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/applicative/applicative.hpp>
#include <utility>
#include <functional>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace expected_helpers
{

  /**
   * functor::ap for TypeConstructible Nullable types
   */

  template <class N, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_nullable_v<decay_t<N>>
  // && is_type_constructible_v<decay_t<N>>
  //>
  >
  //expected<int>
  auto
  ap(F&& f, N&& n)
  -> decltype(meta::rebind_t<decay_t<N>, decltype(
          JASEL_INVOKE((*forward<F>(f)), *forward<N>(n))
        )>(JASEL_INVOKE((*forward<F>(f)), *forward<N>(n))))
  {
    if ( ! f )
      return f.get_unexpected();
    if ( ! n )
      return n.get_unexpected();

    return
        meta::rebind_t<decay_t<N>, decltype(
                      JASEL_INVOKE((*forward<F>(f)), *forward<N>(n))
                    )>(JASEL_INVOKE((*forward<F>(f)), *forward<N>(n)));
  }
}

/**
 */

namespace applicative
{
template <class T, class E>
struct traits<expected<T,E>> : applicative::tag
{
  template <class Expected, class F>
    static auto ap(F&& f, Expected&& x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
        expected_helpers::ap(forward<F>(f), forward<Expected>(x))
      )
  template <class Expected, class U>
    static auto pure(U&& x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
        make<Expected>(forward<U>(x))
      )
};
}
}}
}
#endif // header
