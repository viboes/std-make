//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_INSERT_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_INSERT_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
#include <tuple>
#include <utility>
#include <experimental/type_traits.hpp>

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

    template <size_t N, class ProductType, std::size_t... IF, class T, std::size_t... IL>
    constexpr decltype(auto) insert_impl( ProductType&& pt, index_sequence<IF...>, T&& x, index_sequence<IL...> )
    {
      return make<type_constructor_t<meta::uncvref_t<ProductType>>>(
          product_type::get<IF>(forward<ProductType>(pt))...,
          forward<T>(x),
          product_type::get<IL>(forward<ProductType>(pt))...
          );
    }
  } // namespace product_type_detail
#endif
  template <size_t N, class ProductType, class T
#if ! defined JASEL_DOXYGEN_INVOKED
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>>  >
#endif
  >
  constexpr auto insert(ProductType&& pt, T&& x)
  JASEL_DECLTYPE_RETURN_NOEXCEPT(
      product_type_detail::insert_impl<N>(forward<ProductType>(pt),
          make_index_sequence_for_range<0, N-1>{},
          forward<T>(x),
          make_index_sequence_for_range<N, product_type::size<meta::uncvref_t<ProductType>>::value-1>{}
          )
  )

}
}}
}

#endif // header
