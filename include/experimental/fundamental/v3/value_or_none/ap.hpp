//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201002L || defined JASEL_DOXYGEN_INVOKED

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_AP_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_AP_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/value_or_none/value_or_none.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_traits.hpp>
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

namespace value_or_none
{

  /**
   * applicative::ap for TypeConstructible Nullable types
   */

  template <class N, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_value_or_none_v<remove_cvref_t<N>>
  // && is_type_constructible_v<remove_cvref_t<N>>
  //>
  >
  JASEL_CXX14_CONSTEXPR auto
  ap(F&& f, N&& n) -> decltype(
      make<TypeConstructor<remove_cvref_t<N>>>(value_or_none::deref(std::forward<F>(f))(value_or_none::deref(forward<N>(n))))
      )
  {
    if (! value_or_none::has_value(forward<F>(f)) || ! value_or_none::has_value(forward<N>(n)))
      return value_or_none::none<remove_cvref_t<N>>();


    return make<TypeConstructor<remove_cvref_t<N>>>(
        JASEL_INVOKE(value_or_none::deref(std::forward<F>(f)), value_or_none::deref(forward<N>(n)))
      );
  }
  struct as_applicative
  {
    template <class T, class F>
      static constexpr auto ap(F&& f, T&& x)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
          value_or_none::ap(forward<F>(f), forward<T>(x))
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
    is_value_or_none<N>::value && is_type_constructible<N>::value
>> : value_or_none::as_applicative {};

}

}}
}
#endif // header
#endif
