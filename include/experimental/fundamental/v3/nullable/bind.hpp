//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201002L || defined JASEL_DOXYGEN_INVOKED

#ifndef JASEL_FUNDAMENTAL_V3_NULLABLE_BIND_HPP
#define JASEL_FUNDAMENTAL_V3_NULLABLE_BIND_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/nullable/none.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/monad/monad.hpp>
#include <utility>
#include <functional>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace nullable
{

  /**
   * monad::bind for TypeConstructible Nullable types
   */

  template <class M, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_nullable_v<decay_t<N>>
  // && is_type_constructible_v<decay_t<N>>
  //>
  >
  JASEL_CXX14_CONSTEXPR auto
  bind(M&& m, F&& f) -> decltype(
      std::forward<F>(f)(nullable::deref(forward<M>(m)))
      )

  {
    if (nullable::has_value(forward<M>(m)))
    {
      return JASEL_INVOKE(std::forward<F>(f), nullable::deref(forward<M>(m)));
    }
    return nullable::none<meta::uncvref_t<M>>();

  }
  struct as_monad: monad::tag
  {
    template <class M, class F>
      static constexpr auto bind(M&& x, F&& f)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
            nullable::bind(forward<M>(x), forward<F>(f))
        )
  };
}
namespace monad {

template <class N>
struct traits<N, meta::when<
    is_nullable<N>::value && is_type_constructible<N>::value
>> : nullable::as_monad {};

}

}}
}
#endif // header
#endif
