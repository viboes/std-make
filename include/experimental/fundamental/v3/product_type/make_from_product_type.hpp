//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_MAKE_FROM_PRODUCT_TYPE_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_MAKE_FROM_PRODUCT_TYPE_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
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
  namespace detail {

    template <class T, class ProductType, std::size_t... I>
    constexpr decltype(auto) make_from_product_type_impl( ProductType&& pt, index_sequence<I...> )
    {
      return T(product_type::get<I>(forward<ProductType>(pt))...);
    }

  } // namespace detail

  /**
   * Constructs a type T using the product type elements as forward parameters to the constructor
   * .
   *
   * @par arg a product_type
   *
   * @pre The parameter `ProductType` must be a model of /ProductType/. The parameter
   * T must be CopyConstructible from the ProductType element types.
   */

  template <class T, class ProductType
  , class = enable_if_t< is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>>  >
  >
  constexpr decltype(auto) make_from(ProductType&& pt)
  {
      return detail::make_from_product_type_impl<T>(forward<ProductType>(pt),
          product_type::element_sequence_for<ProductType>{});
  }

}
}}
}

#endif // header
