//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

// fixme: move this to PossiblyValued.

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_VALUE_OR_THROW_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_VALUE_OR_THROW_HPP

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
   * value_or_error::value_or
   * @par Returns
   *  The contained value or the parameter
   *
   * @par Note
   *  value_or(n, v) == resolve(n, always(v))
   */
  template <class E, class N
  // todo add constraint on T
  //, class = enable_if_t<
  //    is_value_or_error_v<remove_cvref_t<N>>
  // && is_convertible_v< T, value_type_t<remove_cvref_t<N>> >
  //>
  >
  BOOST_CXX14_CONSTEXPR
  value_type_t<remove_cvref_t<N>>
  value_or_throw(N&& n)
  {
    if (value_or_error::has_value(forward<N>(n)))
    {
      return value_or_error::deref(forward<N>(n));
    }
    return throw E{value_or_error::error(forward<N>(n))};
  }

} // value_or_error
}}
}
#endif // header
