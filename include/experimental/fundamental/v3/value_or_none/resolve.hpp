//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_RESOLVE_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_RESOLVE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/value_or_none/value_or_none.hpp>
#include <experimental/type_traits.hpp>
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
   * value_or_none::resolve
   * @par Returns
   *  The contained value or the resolution of none via the function `f`
   */
  template <class N, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_value_or_none_v<remove_cvref_t<N>>
  // && is_convertible_v< F(error_type_t<remove_cvref_t<N>>), value_type_t<remove_cvref_t<N>> >
  //>
  >
  BOOST_CXX14_CONSTEXPR
  value_type_t<remove_cvref_t<N>>
  resolve(N&& n, F&& f)
  {
    if (value_or_none::has_value(forward<N>(n)))
    {
      return value_or_none::deref(forward<N>(n));
    }
    return JASEL_INVOKE(std::forward<F>(f),  value_or_none::none<remove_cvref_t<N>>() );
  }

} // value_or_none
}}
}
#endif // header
