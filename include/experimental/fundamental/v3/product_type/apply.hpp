//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_APPLY_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_APPLY_HPP

#include <utility>
#include <functional>
#include <experimental/product_type.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace product_type
{
  namespace detail {

    template <class F, class ProductType, std::size_t... I>
    constexpr decltype(auto) apply_impl( F&& f, ProductType&& t, index_sequence<I...> )
    {
      return invoke(std::forward<F>(f), product_type::get<I>(forward<ProductType>(t))...);
    }

  } // namespace detail

  /**
   * Invoke the Callable object f with a product_type of the arguments.
   *
   * @par f Callable object to be invoked
   * @par pt tuple whose elements to be used as arguments to f
   *
   * @Note This function should't participate in overload resolution until
   * - DF is a model of Callable<product_type::element_t<I, DProductType>... > and
   * - DProductType is a model of ProductType.
   *
   * where DF and DProductType are respectively the decay of F and ProductType
   */

  template <class F, class ProductType>
  constexpr decltype(auto) apply(F&& f, ProductType&& pt)
  {
      return detail::apply_impl(forward<F>(f), forward<ProductType>(pt),
          make_index_sequence<product_type::size_v<remove_cvr_t<ProductType>>>{});
  }

}
}}
}

#endif // header
