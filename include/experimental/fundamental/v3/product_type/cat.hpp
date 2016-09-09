//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_CAT_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_CAT_HPP

#include <experimental/fundamental/v3/product_type/to_tuple.hpp>
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
  /**
   * Constructs a tuple that is a concatenation of all product type in args.
   *
   * @par args  zero or more product_type to concatenate
   *
   * @pre All the parameter must be a model of ProductType
   */

  template <class ...ProductTypes>
  constexpr decltype(auto) cat(ProductTypes&& ...pts)
  {
    // todo: improve the implementation without using to_tuple
      return std::tuple_cat(to_tuple(forward<ProductTypes>(pts))...);
  }

}
}}
}

#endif // header
