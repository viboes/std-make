//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_SWAP_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_SWAP_HPP

#include <experimental/fundamental/v3/swappable.hpp>
#include <experimental/fundamental/v3/product_type.hpp>
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
  namespace detail {
#if defined JASE_HAS_FOLD_EXPRESSIONS

    template <class T, size_t ... ids>
      constexpr void swap_impl(T& x, T& y, index_sequence<ids...>)
      {
        (..., swappable::swap(product_type::get<ids>(x), product_type::get<ids>(y)));
      }
#else
  template <size_t N, class T>
  struct swap_impl {
    static constexpr void apply(T& x, T& y)
    {
      swap_impl<N-1, T>:: template apply(x,y);
      swappable::swap(product_type::get<N>(x), product_type::get<N>(y));
      return;
    }
  };
  template <class T>
  struct swap_impl<0, T> {
    static constexpr void apply(T& x, T& y)
    {
      return;
    }
  };
#endif
  }

  template <class T>
    constexpr void swap(T& x, T& y)
    {
#if defined JASE_HAS_FOLD_EXPRESSIONS
      detail::swap_impl(x, y, make_index_sequence<product_type::size_v<T>>{});
#else
      detail::swap_impl<product_type::size_v<T>, T>::template eapply(x, y);
#endif
    }
}

  /**
   */

  template <class PT>
  struct swappable_traits<PT, PT, meta::when<
    is_product_type_v<PT>
  >>
  {
    template <class T>
      static constexpr void swap(T& x, T& y)
      {
        product_type::swap(x, y);
      }
  };

}}
}

#endif // header