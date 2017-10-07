//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_VISIT_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_VISIT_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/value_or_error/value_or_error.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v3/sum_type/sum_type.hpp>
#include <experimental/type_traits.hpp>
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
#if ! defined JASEL_DOXYGEN_INVOKED
namespace detail
{
  template <size_t I>
  struct value_or_error_get;
  template <>
  struct value_or_error_get<0> {
    template <class ST>
    static constexpr decltype(auto) get(ST&& st) noexcept
    {
      return value_or_error::error(forward<ST>(st));
    }
  };
  template <>
  struct value_or_error_get<1> {
    template <class ST>
    static constexpr decltype(auto) get(ST&& st) noexcept
    {
      return value_or_error::deref(forward<ST>(st));
    }
  };
}
#endif

//! struct mapping a ValueOrError to a SumType
  template <class N>
  struct as_sum_type {
    using size = integral_constant<size_t, 2>;
    using single = false_type;

    template <size_t I>
    using alternative = tuple_element<I, meta::types<error_type_t<N>, value_type_t<N>>>;

    template <size_t I, class ST
#if ! defined JASEL_DOXYGEN_INVOKED
    , class= enable_if_t< I < size::value >
#endif
    >
      static constexpr decltype(auto) get(ST&& st) noexcept
      {
        return detail::value_or_error_get<I>::get(forward<ST>(st));
      }
    template <class V, class ST>
      static constexpr decltype(auto) visit(V&& v, ST&& st) noexcept
      {
        if (value_or_error::has_value(forward<ST>(st)))
          return JASEL_INVOKE(forward<V>(v),value_or_error::deref(forward<ST>(st)));
        return JASEL_INVOKE(forward<V>(v), value_or_error::error(forward<ST>(st)) );


      }
  };
}
}}
}
#endif // header
#endif
