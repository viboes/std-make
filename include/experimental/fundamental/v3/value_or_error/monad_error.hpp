//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_MONAD_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_MONAD_ERROR_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/value_or_error/value_or_error.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/make.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/monad_error/monad_error.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
  namespace value_or_error
  {

    // fixme: Should we provide another overload taking a Callable<F(error_type_t<N>) as parameter?
    template <class N, class F>
    // requires Nullable<N> and Callable<F,error_type_t<N>(void)>
    constexpr decay_t<N> catch_error(N&& x, F&& f)
    {
      return (value_or_error::has_value(forward<N>(x))) ? move(x) : f();
    }

    template <class M, class ...Xs>
    constexpr
    auto make_error(Xs && ...xs)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
              //error_type_t<M>(forward<Xs>(xs)...)
              type_constructible::make<error_type_t<M>>(forward<Xs>(xs)...)
      )
    struct as_monad_error
    {
      template <class M>
      using error_type = error_type_t<M>;

      template <class M, class ...Xs>
        static constexpr auto make_error(Xs&& ... xs)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
            value_or_error::make_error<M>(forward<Xs>(xs)...)
        )

      template <class M, class F>
        static constexpr auto catch_error(M&& x, F&& f)
          JASEL_DECLTYPE_RETURN_NOEXCEPT(
              value_or_error::catch_error(forward<M>(x), forward<F>(f))
          )
    };
  }

namespace monad_error
{

  template <class N>
  struct traits<N, meta::when<
    is_value_or_error<N>::value
  >> : value_or_error::as_monad_error {};

}
}}
}
#endif // header
