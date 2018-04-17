//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#error
#ifndef JASEL_FUNDAMENTAL_V3_NULLABLE_ERROR_OR_HPP
#define JASEL_FUNDAMENTAL_V3_NULLABLE_ERROR_OR_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/monad_error/monad_error.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_traits.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace monad_error
{
  /**
   * monad_error::error_or
   * @par Returns
   *  The monad error if it contains an error or the same monad error with the `e` parameter
   */

  // It will be great to be able to return the error itself, but monad_error, doesn't allows to extract the error
  // We need either to be Nullable or ValueOrError

  template <class M, class E
  // constraints on M and E
    , class = enable_if_t<
        is_monad_error_v<remove_cvref_t<N>>
      && is_convertible_v< E, error_type_t<remove_cvref_t<N>> >
    >
  >
  BOOST_CXX14_CONSTEXPR
  M
  error_or(M&& n, E&& e)
  {
    using ME = remove_cvref_t<M>;
    return (bind)(forward<M>(m), [e](auto) -> ME { return make_error<ME>(e)});
  }

} // monad_error
}}
}
#endif // header
