//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201002L || defined JASEL_DOXYGEN_INVOKED

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_AP_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_AP_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/value_or_error/value_or_error.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/applicative/applicative.hpp>
#include <utility>
#include <functional>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace value_or_error
{

  /**
   * applicative::ap for TypeConstructible ValueOrError types
   */

  template <class N, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_value_or_error_v<decay_t<N>>
  // && is_type_constructible_v<decay_t<N>>
  //>
  >
  JASEL_CXX14_CONSTEXPR auto
  ap(F&& f, N&& n) -> decltype(
      make<TypeConstructor<decay_t<N>>>(value_or_error::deref(std::forward<F>(f))(value_or_error::deref(forward<N>(n))))
      )
  {
    if (! value_or_error::has_value(forward<F>(f)) )
        return success_or_failure::failure_value(forward<F>(f));

    if (! value_or_error::has_value(forward<N>(n)))
        return success_or_failure::failure_value(forward<N>(n));

    return make<TypeConstructor<decay_t<N>>>(
        JASEL_INVOKE(value_or_error::deref(std::forward<F>(f)), value_or_error::deref(forward<N>(n)))
      );
  }
  struct as_applicative
  {
    template <class T, class F>
      static constexpr auto ap(F&& f, T&& x)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
          value_or_error::ap(forward<F>(f), forward<T>(x))
        )
    template <class N, class T>
      static auto pure(T&& x)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
          make<N>(forward<T>(x))
        )
  };
}

/**
 */

namespace applicative {

template <class N>
struct traits<N, meta::when<
    is_value_or_error<N>::value && is_type_constructible<N>::value
>> : value_or_error::as_applicative {};

}

}}
}
#endif // header
#endif
