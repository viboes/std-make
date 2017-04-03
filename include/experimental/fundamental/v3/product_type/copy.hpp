//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_COPY_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_COPY_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
#include <tuple>
#include <experimental/fundamental/v3/utility/swallow.hpp>
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

    template <class PT1, class PT2, size_t... I>
    decltype(auto) copy_impl( PT1&& pt1, PT2& pt2, index_sequence<I...>)
    {
       swallow{ (product_type::get<I>(pt2) = product_type::get<I>(forward<PT1>(pt1)), unit{}) ... };
       return pt2;
    }

    template <class PT1, class PT2, bool B>
    struct have_same_size : false_type {};
    template <class PT1, class PT2>
    struct have_same_size<PT1,  PT2, true> : integral_constant<bool, product_type::size_v<PT1> == product_type::size_v<PT2>> {};

    template <class PT1, class PT2, bool B, class Ids>
    struct have_assignable_elements : false_type {};
    template <class PT1, class PT2, size_t... I>
    struct have_assignable_elements<PT1&,  PT2&&, true, index_sequence<I...>> :
#if defined JASEL_HAS_FOLD_EXPRESSIONS
      integral_constant<bool,
        (... && is_assignable<element_t<I, PT1>&, product_type::element_t<I, PT2>&&>::value)
      >
#else
    conjunction<
      is_assignable<product_type::element_t<I, PT1>&, element_t<I, PT2>&&>...
    >
#endif
    {};

  } // namespace detail
#endif

  /**
   * Copy a product type pt1 into a product type pt2 having the same elements as the product type arg.
   *
   * @param pt1 a rvalue to a product type
   * @param pt2 a lvalue to a product type
   *
   * @par Requires
   *  The parameters must be a model of ProductType, the size be the same and is_assignable_v<Ti&, Ui&&> == true for all i.
   */

#if 1
  template <class PT1, class PT2
#if ! defined JASEL_DOXYGEN_INVOKED
  , class = enable_if_t<
        product_type_detail::have_same_size<
          PT2, meta::uncvref_t<PT1>,
          (is_product_type_v<PT2> && is_product_type_v<meta::uncvref_t<PT1>>)
        >::value
      && product_type_detail::have_assignable_elements<
          PT2&, meta::uncvref_t<PT1>&&,
          (is_product_type_v<PT2> && is_product_type_v<meta::uncvref_t<PT1>>),
          product_type::element_sequence_for<PT2>
        >::value
    >
#endif
  >
  PT2& copy(PT1&& pt1, PT2& pt2)
  {
    return product_type_detail::copy_impl(forward<PT1>(pt1), pt2,
        product_type::element_sequence_for<PT2>{}
        );

  }
#else
  template <class PT1, class PT2>
  auto copy(PT1&& pt1, PT2& pt2)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
          detail::copy_impl(forward<PT1>(pt1), pt2
              product_type::element_sequence_for<PT2>{})
      )
#endif
}
}}
}

#endif // header
