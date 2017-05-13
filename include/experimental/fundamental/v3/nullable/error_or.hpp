//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_NULLABLE_ERROR_OR_HPP
#define JASEL_FUNDAMENTAL_V3_NULLABLE_ERROR_OR_HPP

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
   * nullable::value_or
   * @par Returns
   *  The contained value or the parameter
   */
  template <class N, class E
  // todo add constraint on E
  //, class = enable_if_t<
  //    is_nullable_v<meta::uncvref_t<N>>
  // && is_convertible_v< E, error_type_t<meta::uncvref_t<N>> >
  //>
  >
  BOOST_CXX14_CONSTEXPR
  none_type_t<meta::uncvref_t<N>>
  error_or(N&& n, E&& e)
  {
    if (nullable::has_value(forward<N>(n)))
    {
      return nullable::deref_none(forward<N>(n));
    }
    return forward<E>(e);
  }

} // nullable
}}
}
#endif // header
