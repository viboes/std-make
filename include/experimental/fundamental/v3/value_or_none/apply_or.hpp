//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_APPLY_OR_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_APPLY_OR_HPP

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
   * value_or_none::apply_or
   * @par Returns
   *  The contained value or the parameter
   */
  template <class N, class F, class T
  // todo add constraint on T and F
  //, class = enable_if_t<
  //    is_value_or_none_v<meta::uncvref_t<N>>
  // && is_convertible_v< T, value_type_t<meta::uncvref_t<N>> >
  //>
  >
  BOOST_CXX14_CONSTEXPR
  value_type_t<meta::uncvref_t<N>>
  apply_or(N&& n, F&& f, T&& v)
  {
    if (value_or_none::has_value(forward<N>(n)))
    {
      return JASEL_INVOKE(std::forward<F>(f), value_or_none::deref(forward<N>(n)));
    }
    return forward<T>(v);
  }

} // value_or_none
}}
}
#endif // header
