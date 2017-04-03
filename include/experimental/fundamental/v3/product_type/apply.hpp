//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_APPLY_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_APPLY_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
#include <utility>
#include <functional>

// fixme: apply is implementable, as Hana does it, using foldable::fold_left.
// Which implementation is more efficient at run-time? at compile time?

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

    template <class F, class ProductType, std::size_t... I>
    constexpr decltype(auto) apply_impl( F&& f, ProductType&& pt, index_sequence<I...> )
    {
      return JASEL_INVOKE(std::forward<F>(f), product_type::get<I>(forward<ProductType>(pt))...);
    }

  } // namespace product_type_detail
#endif

  /**
   * Invoke the Callable object f with a product_type of the arguments.
   *
   * @param f Callable object to be invoked
   * @param pt product type whose elements to be used as arguments to f
   *
   * @par Requires:
   * - DF is a model of Callable<product_type::element_t<I, DProductType>... > and
   * - DProductType is a model of ProductType.
   *
   * where DF and DProductType are respectively the decay of F and ProductType removing cv and reference.
   */

  template <class F, class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>> >
#endif
  >
  constexpr decltype(auto) apply(F&& f, ProductType&& pt)
  {
      return product_type_detail::apply_impl(forward<F>(f), forward<ProductType>(pt),
          product_type::element_sequence_for<ProductType>{});
  }

}
}}
}

#endif // header
