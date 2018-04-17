//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_POSSIBLY_VALUED_APPLY_OR_HPP
#define JASEL_FUNDAMENTAL_V3_POSSIBLY_VALUED_APPLY_OR_HPP

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
   * possibly_valued::apply_or
   * @par Returns
   *  The contained value or the parameter
   */
  template <class N, class F, class T
  // todo add constraint on T and F
  //, class = enable_if_t<
  //    is_possibly_valued_v<remove_cvref_t<N>>
  // && is_convertible_v< T, value_type_t<remove_cvref_t<N>> >
  //>
  >
  BOOST_CXX14_CONSTEXPR
  value_type_t<remove_cvref_t<N>>
  apply_or(N&& n, F&& f, T&& v)
  {
    if (possibly_valued::has_value(forward<N>(n)))
    {
      return JASEL_INVOKE(std::forward<F>(f), possibly_valued::deref(forward<N>(n)));
    }
    return forward<T>(v);
  }

} // possibly_valued
}}
}
#endif // header
