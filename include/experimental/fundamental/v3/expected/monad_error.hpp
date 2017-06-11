//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_EXPECTED_MONAD_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_EXPECTED_MONAD_ERROR_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/expected/expected.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/monad_error/monad_error.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace monad_error
{
template <class T, class E>
struct traits<expected<T,E>> : monad_error::tag
{
  template <class M>
  using error_type = typename M::error_type;

  template <class M, class ...Xs>
    static constexpr auto make_error(Xs&& ...xs)
    JASEL_DECLTYPE_RETURN_NOEXCEPT(
      make_unexpected(typename M::error_type(std::forward<Xs>(xs)...))
    )

  template <class M, class F>
    static constexpr auto catch_error(M&& x, F&& f)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
        x.catch_error(forward<F>(f))
      )

  template <class M, class F>
    static constexpr auto adapt_error(M&& x, F&& f)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
        x.adapt_error(forward<F>(f))
      )

};
}
}}
}
#endif // header
