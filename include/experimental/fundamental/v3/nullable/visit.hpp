//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201402L

#ifndef JASEL_FUNDAMENTAL_V3_NULLABLE_VISIT_HPP
#define JASEL_FUNDAMENTAL_V3_NULLABLE_VISIT_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/nullable/none.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v3/sum_type/sum_type.hpp>
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
namespace detail
{
  template <size_t I>
  struct nullable_get;
  template <>
  struct nullable_get<0> {
    template <class ST>
    static constexpr decltype(auto) get(ST&& st) noexcept
    {
      return nullable::deref_none(forward<ST>(st));
    }
  };
  template <>
  struct nullable_get<1> {
    template <class ST>
    static constexpr decltype(auto) get(ST&& st) noexcept
    {
      return nullable::deref(forward<ST>(st));
    }
  };
}
  template <class N>
  struct as_sum_type : sum_type::tag {
    using size = integral_constant<size_t, 2>;

    template <size_t I>
    using alternative = tuple_element<I, meta::types<none_type_t<N>, value_type_t<N>>>;

    template <size_t I, class ST, class= std::enable_if_t< I < size::value > >
      static constexpr decltype(auto) get(ST&& st) noexcept
      {
        return detail::nullable_get<I>::get(forward<ST>(st));
      }
    template <class V, class ST>
      static constexpr decltype(auto) visit(V&& v, ST&& st) noexcept
      {
        if (nullable::has_value(std::forward<ST>(st)))
          return JASEL_INVOKE(forward<V>(v),nullable::deref(forward<ST>(st)));
        return forward<V>(v)(nullable::deref_none(forward<ST>(st)));
      }
  };
}
}}
}
#endif // header
#endif
