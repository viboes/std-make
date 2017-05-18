//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_NULLABLE_RESOLVE_HPP
#define JASEL_FUNDAMENTAL_V3_NULLABLE_RESOLVE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/nullable/none.hpp>
#include <experimental/meta.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace nullable
{
  /**
   * nullable::resolve
   * @par Returns
   *  The contained value or the resolution of none via the function `f`
   */
  template <class N, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_nullable_v<meta::uncvref_t<N>>
  // && is_convertible_v< F(error_type_t<meta::uncvref_t<N>>), value_type_t<meta::uncvref_t<N>> >
  //>
  >
  BOOST_CXX14_CONSTEXPR
  value_type_t<meta::uncvref_t<N>>
  resolve(N&& n, F&& f)
  {
    if (nullable::has_value(forward<N>(n)))
    {
      return nullable::deref(forward<N>(n));
    }
    return JASEL_INVOKE(std::forward<F>(f),  nullable::deref_none(forward<N>(n)) );
  }

} // nullable
}}
}
#endif // header
