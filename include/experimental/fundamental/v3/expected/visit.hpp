//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201402L

#ifndef JASEL_FUNDAMENTAL_V3_EXPECTED_VISIT_HPP
#define JASEL_FUNDAMENTAL_V3_EXPECTED_VISIT_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/expected/expected.hpp>
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
namespace expected_helpers
{
  template <size_t I>
  struct expected_get;
  template <>
  struct expected_get<0> {
    template <class ST>
    static constexpr decltype(auto) get(ST&& st) noexcept
    {
      return *st;
    }
  };
  template <>
  struct expected_get<1> {
    template <class ST>
    static constexpr decltype(auto) get(ST&& st) noexcept
    {
      return st.get_unexpected();
    }
  };
}


namespace sum_type
{
  template <class T, class E>
  struct traits<expected<T,E>> : sum_type::tag
  {
    using size = integral_constant<size_t, 2>;

    template <size_t I>
    using alternative = tuple_element<I, meta::types<T, unexpected<E> >>;

    template <size_t I, class ST, class= std::enable_if_t< I < size::value > >
      static constexpr decltype(auto) get(ST&& st) noexcept
      {
        return expected_helpers::expected_get<I>::get(forward<ST>(st));
      }
    template <class V, class ST>
      static constexpr decltype(auto) visit(V&& v, ST&& st) noexcept
      {
        if (st)
          return JASEL_INVOKE(forward<V>(v),*st);
        return JASEL_INVOKE(forward<V>(v), st.get_unexpected());
      }
  };
}
}}
}
#endif // header
#endif
