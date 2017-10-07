//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
// fixme: It isn't too much to provide these functions for Nullable as they are more adapted to ValueOrError

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_CHECK_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_CHECK_ERROR_HPP

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
   * value_or_none::check_error
   * @par Returns
   *  Whether the contained error (if any) is equal to the parameter `e
   */
  template <class N, class E
    , class = enable_if_t<
        is_value_or_none< meta::uncvref_t<N> >::value
        // add constraint on E
        //&& is_comparable_v< decay_t<E>, none_type_t<meta::uncvref_t<N>> >
    >
  >
  BOOST_CXX14_CONSTEXPR
  bool
  check_error(N&& n, E&& e)
  {
    if (value_or_none::has_value(forward<N>(n)))
    {
      return false;
    }
    return value_or_none::none<meta::uncvref_t<N>>() == forward<E>(e);
  }

} // value_or_none
}}
}
#endif // header
