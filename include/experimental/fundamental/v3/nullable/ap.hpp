//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201002L || defined JASEL_DOXYGEN_INVOKED

#ifndef JASEL_FUNDAMENTAL_V3_NULLABLE_AP_HPP
#define JASEL_FUNDAMENTAL_V3_NULLABLE_AP_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/nullable/none.hpp>
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

namespace nullable
{

  /**
   * applicative::ap for TypeConstructible Nullable types
   */

  template <class N, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_nullable_v<decay_t<N>>
  // && is_type_constructible_v<decay_t<N>>
  //>
  >
  JASEL_CXX14_CONSTEXPR auto
  ap(F&& f, N&& n) -> decltype(
      make<TypeConstructor<decay_t<N>>>(nullable::deref(std::forward<F>(f))(nullable::deref(forward<N>(n))))
      )
  {
    if (! nullable::has_value(forward<F>(f)) || ! nullable::has_value(forward<N>(n)))
      return nullable::deref_none(forward<N>(n));

    return make<TypeConstructor<decay_t<N>>>(
        JASEL_INVOKE(nullable::deref(std::forward<F>(f)), nullable::deref(forward<N>(n)))
      );
  }
  struct as_applicative: applicative::tag
  {
    template <class T, class F>
      static constexpr auto ap(F&& f, T&& x)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
          nullable::ap(forward<F>(f), forward<T>(x))
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
    is_nullable<N>::value && is_type_constructible<N>::value
>> : nullable::as_applicative {};

}

}}
}
#endif // header
#endif
