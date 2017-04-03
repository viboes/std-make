//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_SWAP_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_SWAP_HPP
#if __cplusplus >= 201402L and defined JASEL_SUPPORT_SWAPPABLE

#include <experimental/fundamental/v3/swappable/swappable.hpp>
#include <experimental/fundamental/v3/product_type/product_type.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace product_type
{
#if ! defined JASEL_DOXYGEN_INVOKED
  namespace product_type_detail {
#if defined JASEL_HAS_FOLD_EXPRESSIONS

    template <class T, size_t ... ids>
      constexpr void swap_impl(T& x, T& y, index_sequence<ids...>)
      {
        (..., swappable::swap(product_type::get<ids>(x), product_type::get<ids>(y)));
      }
#else
  template <size_t N>
  struct swap_impl {
    template <class T>
    static constexpr void apply(T& x, T& y)
    {
      swap_impl<N-1>::template  apply(x,y);
      swappable::swap(product_type::get<N-1>(x), product_type::get<N-1>(y));
      return;
    }
  };
  template <>
  struct swap_impl<0> {
    template <class T>
    static constexpr void apply(T& x, T& y)
    {
      return;
    }
  };
#endif
  }
#endif

  template <class T>
    constexpr void swap(T& x, T& y)
    {
#if defined JASEL_HAS_FOLD_EXPRESSIONS
      product_type_detail::swap_impl(x, y, product_type::element_sequence_for<T>{});
#else
      product_type_detail::swap_impl<product_type::size_v<T>>::template apply(x, y);
#endif
    }

  template <class T, std::size_t N>
#if ! defined JASEL_DOXYGEN_INVOKED
  enable_if_t<
    ::std::experimental::is_swappable_v<T>
  >
#else
  void
#endif
    swap(T (&t)[N], T (&u)[N])
      noexcept(::std::experimental::is_nothrow_swappable_v<T>)
    {
      for (std::size_t i = 0; i < N; ++i)
      {
          ::std::experimental::swappable::swap(t[i], u[i]);
      }
    }
}

namespace swappable {
  /**
   */

  template <class PT>
  struct traits<PT, PT, meta::when<
    not is_adl_swappable_v<PT,PT> // this is needed if ADL is used as a specialization of traits
    and is_product_type_v<PT>
  >>
  {
    template <class T>
      static constexpr void swap(T& x, T& y)
      {
        product_type::swap(x, y);
      }
  };
}
}}
}
#endif
#endif // header
