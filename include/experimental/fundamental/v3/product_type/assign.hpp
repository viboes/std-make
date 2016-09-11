//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_ASSIGN_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_ASSIGN_HPP

#include <experimental/fundamental/v3/product_type.hpp>
#include <tuple>
#include <utility>
#include <type_traits>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace product_type
{
  namespace product_type_detail {

    template <class PT1, class PT2, std::size_t... I>
    decltype(auto) assign_impl( PT1& pt1, PT2&& pt2, index_sequence<I...>)
    {
       swallow(product_type::get<I>(pt1) = product_type::get<I>(forward<PT2>(pt2)) ...);
       return pt1;
    }

    template <class PT1, class PT2, bool B>
    struct have_same_size : false_type {};
    template <class PT1, class PT2>
    struct have_same_size<PT1,  PT2, true> : bool_constant<size_v<PT1> == size_v<PT2>> {};

    template <class PT1, class PT2, bool B, class Ids>
    struct have_assignable_elements : false_type {};
    template <class PT1, class PT2, std::size_t... I>
    struct have_assignable_elements<PT1&,  PT2&&, true, index_sequence<I...>> : bool_constant<
      (... && std::is_assignable<element_t<I, PT1>&, element_t<I, PT2>&&>::value)
    > {};

  } // namespace detail

  /**
   * Assigns a product type pt2 to a  product type pt2 having the same elements as the product type arg.
   *
   * @par pt1 a lvalue to a product type
   * @par pt2 a rvalue to a product type
   *
   * @pre The parameters must be a model of ProductType, the size be the same and is_assignable_v<Ti&, Ui&&> == true for all i.
   */

#if 1
  template <class PT1, class PT2
  , class = enable_if_t<
        product_type_detail::have_same_size<
          PT1, remove_cv_t<remove_reference_t<PT2>>,
          (is_product_type_v<PT1> && is_product_type_v<remove_cv_t<remove_reference_t<PT2>>>)
        >::value
      && product_type_detail::have_assignable_elements<
          PT1&, remove_cv_t<remove_reference_t<PT2>&&>,
          (is_product_type_v<PT1> && is_product_type_v<remove_cv_t<remove_reference_t<PT2>>>),
          make_index_sequence<product_type::size_v<PT1>>
        >::value
    >
  >
  PT1& assign(PT1& pt1, PT2&& pt2)
  {
    return product_type_detail::assign_impl(pt1, forward<PT2>(pt2),
        make_index_sequence<product_type::size_v<PT1>>{}
        );

  }
#else
  template <class PT1, class PT2>
  auto assign(PT1& pt1, PT2&& pt2)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
          detail::assign_impl(pt1, forward<PT2>(pt2),
                    make_index_sequence<product_type::size_v<PT1>>{})
      )
#endif
}
}}
}

#endif // header
