//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201002L || defined JASEL_DOXYGEN_INVOKED

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_BIND_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_BIND_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/value_or_none/value_or_none.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_traits.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/monad/monad.hpp>
#include <utility>
#include <functional>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace value_or_none
{

  /**
   * monad::bind for TypeConstructible Nullable types
   */

  template <class M, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_value_or_none_v<remove_cvref_t<N>>
  // && is_type_constructible_v<remove_cvref_t<N>>
  //>
  >
  JASEL_CXX14_CONSTEXPR auto
  bind(M&& m, F&& f) -> decltype(
      std::forward<F>(f)(value_or_none::deref(forward<M>(m)))
      )

  {
    if (value_or_none::has_value(forward<M>(m)))
    {
      return JASEL_INVOKE(std::forward<F>(f), value_or_none::deref(forward<M>(m)));
    }
    return value_or_none::none<remove_cvref_t<M>>();

  }
  struct as_monad: monad::mcd_bind
  {
    template <class M, class F>
      static constexpr auto bind(M&& x, F&& f)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
            value_or_none::bind(forward<M>(x), forward<F>(f))
        )
  };
}
namespace monad {

template <class N>
struct traits<N, meta::when<
    is_value_or_none<N>::value && is_type_constructible<N>::value
>> : value_or_none::as_monad {};

}

}}
}
#endif // header
#endif
