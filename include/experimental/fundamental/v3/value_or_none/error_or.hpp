//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

// fixme: It isn't too much to provide these functions for Nullable as they are more adapted to ValueOrError

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_ERROR_OR_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_ERROR_OR_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/value_or_none/value_or_none.hpp>
#include <experimental/meta.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace value_or_none
{
  /**
   * value_or_none::error_or
   * @par Returns
   *  The error value or the parameter
   */
  template <class N, class E
  // constraint on E
    , class = enable_if_t<
          is_value_or_none< remove_cvref_t<N> >::value
          && is_convertible< E, none_type_t< remove_cvref_t<N> > >::value
    >
  >
  BOOST_CXX14_CONSTEXPR
  none_type_t<remove_cvref_t<N>>
  error_or(N&& n, E&& e)
  {
    if (value_or_none::has_value(forward<N>(n)))
    {
      return value_or_none::none<remove_cvref_t<N>>();
    }
    return forward<E>(e);
  }

} // value_or_none
}}
}
#endif // header
