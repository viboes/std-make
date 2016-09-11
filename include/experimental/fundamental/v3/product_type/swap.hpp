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
    template <class T, size_t ... ids>
      constexpr void swap_impl(T& x, T& y, index_sequence<ids...>)
      {
        (..., swappable::swap(product_type::get<ids>(x), product_type::get<ids>(y)));

      }
  }

  template <class T>
    constexpr void swap(T& x, T& y)
    {
      detail::swap_impl(x, y, make_index_sequence<product_type::size_v<T>>{});
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
