//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_POSSIBLY_VALUED_VALUE_OR_HPP
#define JASEL_FUNDAMENTAL_V3_POSSIBLY_VALUED_VALUE_OR_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/possibly_valued/possibly_valued.hpp>
#include <experimental/meta.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace possibly_valued
{
  /**
   * possibly_valued::value_or
   * @par Returns
   *  The contained value or the parameter
   *
   * @par Note
   *  value_or(n, v) == resolve(n, always(v))
   */
  template <class N, class T
  // todo add constraint on T
  //, class = enable_if_t<
  //    is_possibly_valued_v<remove_cvref_t<N>>
  // && is_convertible_v< T, value_type_t<remove_cvref_t<N>> >
  //>
  >
  BOOST_CXX14_CONSTEXPR
  value_type_t<remove_cvref_t<N>>
  value_or(N&& n, T&& v)
  {
    if (possibly_valued::has_value(forward<N>(n)))
    {
      return possibly_valued::deref(forward<N>(n));
    }
    return forward<T>(v);
  }

} // possibly_valued
}}
}
#endif // header
