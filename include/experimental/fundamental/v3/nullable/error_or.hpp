//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

// fixme: It isn't too much to provide these functions for Nullable as they are more adapted to ValueOrError

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
   * nullable::error_or
   * @par Returns
   *  The error value or the parameter
   */
  template <class N, class E
  // constraint on E
    , class = enable_if_t<
          is_nullable< meta::uncvref_t<N> >::value
          && is_convertible< E, none_type_t< meta::uncvref_t<N> > >::value
    >
  >
  BOOST_CXX14_CONSTEXPR
  none_type_t<meta::uncvref_t<N>>
  error_or(N&& n, E&& e)
  {
    if (nullable::has_value(forward<N>(n)))
    {
      return nullable::none<meta::uncvref_t<N>>();
    }
    return forward<E>(e);
  }

} // nullable
}}
}
#endif // header
