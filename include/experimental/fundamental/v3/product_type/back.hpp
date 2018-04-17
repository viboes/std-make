//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_BACK_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_BACK_HPP

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
  template <class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  , class = enable_if_t< is_product_type_v<remove_cvref_t<ProductType>>  >
#endif
  >
  constexpr auto back(ProductType&& pt)
    JASEL_DECLTYPE_RETURN_NOEXCEPT(
      product_type::get<product_type::size<remove_cvref_t<ProductType>>::value-1>(forward<ProductType>(pt))
    )

}
}}
}

#endif // header
