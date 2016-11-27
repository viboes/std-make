// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_OPTIONAL_HPP
#define JASEL_FUNDAMENTAL_V3_OPTIONAL_HPP

#include <optional.hpp>

#include <experimental/make.hpp>
#include <experimental/nullable.hpp>
#include <experimental/sum_type.hpp>
#include <experimental/meta.hpp>
#include <experimental/functor.hpp>
#include <experimental/applicative.hpp>
#include <experimental/monad.hpp>

#if defined JASEL_FUNDAMENTAL_EXTENDED
#include <experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp>
#include <experimental/fundamental/v2/functor/mcd/possible_valued_mcd.hpp>
#include <experimental/fundamental/v2/monad/mcd/possible_valued_mcd.hpp>
#endif
namespace std
{
namespace experimental
{
  // todo: remove this specialization once is_strict_weakly_ordered is defined correctly
  template <class T>
  struct is_strict_weakly_ordered<optional<T>> : true_type {};

  // Callable specialization
  template <>
  struct optional<_t>  : meta::quote<optional> {};

#if defined JASEL_FUNDAMENTAL_EXTENDED
  template <class T>
  struct possible_value::tag<optional<T>> : meta::id<pointer_like> {};

  template <class T>
  struct functorv2::tag<optional<T>> : meta::id<possible_value> {};

  template <class T>
  struct monadv2::tag<optional<T>> : meta::id<possible_value> {};
#endif
  // type_constructor customization
  template <class T>
  struct type_constructor<optional<T>> : meta::id<optional<_t>> {};

#if defined JASEL_FUNDAMENTAL_EXTENDED
  template <class T>
  struct value_type<optional<T>> : meta::id<T> { };

#endif

inline namespace fundamental_v3
{
namespace nullable
{
  template <class T>
  struct traits<optional<T>> : traits_pointer_like {
    static constexpr
    nullopt_t none() { return nullopt; }

    template <class U>
    static constexpr
    nullopt_t deref_none(U &&)
      { return nullopt; }

  };
}

#if __cplusplus >= 201402L

namespace functor {
template <>
struct traits<optional<_t>> : nullable::as_functor {};
}

namespace applicative {
template <>
struct traits<optional<_t>> : nullable::as_applicative {};
}

namespace monad {
template <>
struct traits<optional<_t>> : nullable::as_monad {};
}

namespace sum_type
{
namespace detail {
  template <size_t I>
  struct optional_get;
  template <>
  struct optional_get<0> {
    template <class ST>
    static constexpr decltype(auto) get(ST&& st) noexcept
    {
      return nullopt;
    }
  };
  template <>
  struct optional_get<1> {
    template <class ST>
    static constexpr decltype(auto) get(ST&& st) noexcept
    {
      return *(forward<ST>(st));
    }
  };
}
  template <class T>
  struct traits<optional<T>> : tag {
    using size = integral_constant<size_t, 2>;

    template <size_t I>
    using alternative = tuple_element<I, meta::types<nullopt_t, T>>;

    template <size_t I, class ST2, class= std::enable_if_t< I < size::value > >
      static constexpr decltype(auto) get(ST2&& st) noexcept
      {
        return detail::optional_get<I>::get(forward<ST2>(st));
      }
    template <class V, class ST2>
      static constexpr decltype(auto) visit(V&& v, ST2&& st) noexcept
      {
        if (st) return forward<V>(v)(*forward<ST2>(st));
        else  return forward<V>(v)(nullopt);
      }
  };
}
#endif
}
}
}

#endif // header
