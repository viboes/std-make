//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_AP_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_AP_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
#include <experimental/make.hpp>
#include <experimental/applicative.hpp>
#include <utility>
#include <functional>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace product_type
{
  namespace detail {

    template <class TC, class F, class ProductType, std::size_t... I>
    constexpr decltype(auto) ap_impl( F&& f, ProductType&& pt, index_sequence<I...> )
    {
      return make<TC>(
          JASEL_INVOKE(product_type::get<0>(forward<F>(f)), product_type::get<I>(forward<ProductType>(pt))) ...
      );
    }

  } // namespace detail

  /**
   * Invoke the Callable object contained in f for each one of the product type elements and return the result wrapped using a ProductType.
   *
   * @par f is a product type having the Callable object to be invoked on each product type element as the single element
   * @par pt product type whose elements to be used as arguments to f
   *
   * @pre
   * - DProductType is a model of ProductType.
   * - DF is a model of is a model of ProductType having only one element satisfying
   *   Callable<product_type::element_t<I, DProductType>>...
   *
   * where DF and DProductType are respectively F and ProductType removing cv and reference.
   */

  template <class F, class ProductType
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>> >
  >
  constexpr decltype(auto) ap(F&& f, ProductType&& pt)
  {
      return detail::ap_impl<type_constructor_t<remove_cv_t<remove_reference_t<ProductType>>>>(
          forward<F>(f), forward<ProductType>(pt),
          product_type::element_sequence_for<ProductType>{});
  }

  template <class TC, class F, class ProductType
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>> >
  >
  constexpr decltype(auto) ap(F&& f, ProductType&& pt)
  {
      return detail::ap_impl<TC>(
          forward<F>(f), forward<ProductType>(pt),
          product_type::element_sequence_for<ProductType>{});
  }

  struct as_applicative : applicative::tag
  {
    template <class F, class T>
      static constexpr auto ap(F&& f, T&& x)
      {
        return product_type::ap(forward<F>(f), forward<T>(x));
      }
  };

}

}}
}

#endif // header
