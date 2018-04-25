//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_CHECK_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_CHECK_ERROR_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/value_or_error/value_or_error.hpp>
#include <experimental/type_traits.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace value_or_error
{
  /**
   * value_or_error::check_error
   * @par Returns
   *  Whether the contained error (if any) is equal to the parameter `e
   */
  template <class N, class E
    , class = enable_if_t<
        is_value_or_error< remove_cvref_t<N> >::value
        // todo add constraint on E
        //&& is_comparable_v< remove_cvref_t<E>, none_type_t<remove_cvref_t<N>> >
    >
  >
  BOOST_CXX14_CONSTEXPR
  bool
  check_error(N&& n, E&& e)
  {
    if (value_or_error::has_value(forward<N>(n)))
    {
      return false;
    }
    return value_or_error::error(forward<N>(n)) == forward<E>(e);
  }

} // value_or_error
}}
}
#endif // header
