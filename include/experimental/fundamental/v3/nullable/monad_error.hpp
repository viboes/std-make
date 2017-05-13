//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_NULLABLE_MONAD_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_NULLABLE_MONAD_ERROR_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/nullable/none.hpp>
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
  namespace nullable
  {

    // fixme: Should we provide another overload taking a Callable<F(none_type_t<N>) as parameter?
    template <class N, class F>
    // requires Nullable<N> and Callable<F,none_type_t<N>(void)>
    static constexpr decay_t<N> catch_error(N&& x, F&& f)
    {
      return (has_value(forward<N>(x))) ? move(x) : f();
    }

    struct as_monad_error: monad_error::tag
    {
      template <class M>
      using error_type = none_type_t<M>;

      template <class M, class ...Xs>
        static constexpr auto make_error(Xs&& ...xs)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
          none<M>()
        )

      template <class M, class F>
        static constexpr auto catch_error(M&& x, F&& f)
          JASEL_DECLTYPE_RETURN_NOEXCEPT(
              nullable::catch_error(forward<M>(x), forward<F>(f))
          )
    };
  }

namespace monad_error
{

  template <class N>
  struct traits<N, meta::when<
    is_nullable<N>::value
  >> : nullable::as_monad_error {};

}
}}
}
#endif // header
