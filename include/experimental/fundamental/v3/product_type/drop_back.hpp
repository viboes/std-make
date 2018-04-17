//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_DROP_BACK_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_DROP_BACK_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
#include <experimental/type_traits.hpp>
#include <tuple>
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

    template <size_t N, class ProductType, std::size_t... I>
    constexpr decltype(auto) select_impl( ProductType&& pt, index_sequence<I...> )
    {
      return make<type_constructor_t<remove_cvref_t<ProductType>>>(
          product_type::get<I>(forward<ProductType>(pt))...
          );
    }
  } // namespace product_type_detail
#endif

  template <size_t N, class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  , class = enable_if_t< is_product_type_v<remove_cvref_t<ProductType>>  >
#endif
  >
  constexpr auto drop_back(ProductType&& pt)
  JASEL_DECLTYPE_RETURN_NOEXCEPT(
      product_type_detail::select_impl<N>(forward<ProductType>(pt),
          make_index_sequence<product_type::size<remove_cvref_t<ProductType>>::value-N>{}
          )
  )

  template <size_t N, class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  , class = enable_if_t< is_product_type_v<remove_cvref_t<ProductType>>  >
#endif
  >
  constexpr auto drop_front(ProductType&& pt)
  JASEL_DECLTYPE_RETURN_NOEXCEPT(
      product_type_detail::select_impl<N>(forward<ProductType>(pt),
          make_index_sequence_for_range<N, product_type::size<remove_cvref_t<ProductType>>::value-1>{}
          )
  )

}
}}
}

#endif // header
