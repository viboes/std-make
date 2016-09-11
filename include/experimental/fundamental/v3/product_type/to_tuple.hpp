//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_TO_TUPLE_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_TO_TUPLE_HPP

#include <experimental/fundamental/v3/product_type.hpp>
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

    template <class ProductType, std::size_t... I>
    constexpr decltype(auto) to_tuple_impl( ProductType&& pt, index_sequence<I...> )
    {
      return make_tuple(product_type::get<I>(forward<ProductType>(pt))...);
    }

  } // namespace detail

  /**
   * Constructs a tuple having the same elements as the product type arg.
   *
   * @par arg  a product_type
   *
   * @pre The parameter must be a model of ProductType
   */

  template <class ProductType
  , typename = enable_if_t< is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>>  >
  >
  constexpr decltype(auto) to_tuple(ProductType&& pt)
  {
      return detail::to_tuple_impl(forward<ProductType>(pt),
          make_index_sequence<product_type::size_v<remove_cv_t<remove_reference_t<ProductType>>>>{});
  }

}
}}
}

#endif // header
