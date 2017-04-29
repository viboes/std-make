//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_TRANSFORM_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_TRANSFORM_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
#include <experimental/make.hpp>
#include <experimental/functor.hpp>
#include <experimental/n_functor.hpp>
#include <experimental/p_functor.hpp>
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
#if ! defined JASEL_DOXYGEN_INVOKED
  namespace product_type_detail {

    template <class TC, class F, class ProductType, std::size_t... I>
    constexpr decltype(auto) n_transform_impl( ProductType&& pt, F&& f, index_sequence<I...> )
    {
      return make<TC>(
          JASEL_INVOKE(product_type::get<I>(forward<F>(f)), product_type::get<I>(forward<ProductType>(pt))) ...
      );
    }
    template <class TC, class F, class ProductType, std::size_t... I>
    constexpr decltype(auto) p_transform_impl( ProductType&& pt, F&& f, index_sequence<I...> )
    {
      return make<TC>(
          JASEL_INVOKE(forward<F>(f), product_type::get<I>(forward<ProductType>(pt))) ...
      );
    }

  } // namespace product_type_detail
#endif

  /**
   * Invoke the Callable object f for each one of the product type elements and return the result wrapped using a tuple.
   *
   * @param pt product type whose elements to be used as arguments to f
   * @param f Callable object to be invoked on each product type element
   *
   * @par Requires:
   * - DProductType is a model of ProductType with all its elements the same and
   * - DF is a model of Callable<product_type::element_t<I, DProductType>>...
   *
   * where DF and DProductType are respectively the decay of F and ProductType removing cv and reference.
   */
  template <class F, class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>> >
#endif
  >
  constexpr decltype(auto) transform(ProductType&& pt, F&& f)
  {
      return product_type_detail::p_transform_impl<type_constructor_t<meta::uncvref_t<ProductType>>>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }
  template <class TC, class F, class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>> >
#endif
  >
  constexpr decltype(auto) transform(ProductType&& pt, F&& f)
  {
      return product_type_detail::p_transform_impl<TC>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }

  /**
   * Invoke the Callable object fi for each one of the product type elements and return the result wrapped using a type constructor.
   *
   * @param pt product type whose elements to be used as arguments to f
   * @param f a product type of Callable object to be invoked on each product type element
   *
   * @par Requires:
   * - DProductType is a model of ProductType with all its elements the same and
   * - DF is a model of ProductType where each element is a model of Callable<product_type::element_t<I, DProductType>>.
   * - Both product types have the same size
   *
   * where DF and DProductType are respectively the decay of F and ProductType removing cv and reference.
   */
  template <class F, class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>> >
#endif
  >
  constexpr decltype(auto) n_transform(ProductType&& pt, F&& f)
  {
      return product_type_detail::n_transform_impl<type_constructor_t<meta::uncvref_t<ProductType>>>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }
  template <class TC, class F, class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>> >
#endif
  >
  constexpr decltype(auto) n_transform(ProductType&& pt, F&& f)
  {
      return product_type_detail::n_transform_impl<TC>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }

  /**
   * Invoke the Callable object f for each one of the product type elements and return the result wrapped using a product type  type constructor.
   *
   * @param pt product type whose elements to be used as arguments to f
   * @param f Callable object to be invoked on each product type element. It is an overloaded set.
   *
   * @par Requires:
   * - DProductType is a model of ProductType with all its elements the same and
   * - DF is a model of Callable<product_type::element_t<I, DProductType>>...
   *
   * where DF and DProductType are respectively the decay of F and ProductType removing cv and reference.
   */

  template <class F, class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>> >
#endif
  >
  constexpr decltype(auto) p_transform(ProductType&& pt, F&& f)
  {
      return product_type_detail::p_transform_impl<type_constructor_t<meta::uncvref_t<ProductType>>>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }
  template <class TC, class F, class ProductType
#if ! defined JASEL_DOXYGEN_INVOKED
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>> >
#endif
  >
  constexpr decltype(auto) p_transform(ProductType&& pt, F&& f)
  {
      return product_type_detail::p_transform_impl<TC>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }

  //! struct mapping a product_type to a Functor
  struct as_functor : functor::tag
  {
    template <class T, class F>
      static constexpr auto transform(T&& x, F&& f)
      {
        return product_type::transform(forward<T>(x), forward<F>(f));
      }
  };

  //! struct mapping a product_type to a N-Functor
  struct as_n_functor : n_functor::tag
  {
    template <class T, class F>
      static constexpr auto transform(T&& x, F&& f)
      {
        return product_type::n_transform(forward<T>(x), forward<F>(f));
      }
  };

  //! struct mapping a product_type to a P-Functor
  struct as_p_functor : p_functor::tag
  {
    template <class T, class F>
      static constexpr auto transform(T&& x, F&& f)
      {
        return product_type::p_transform(forward<T>(x), forward<F>(f));
      }
  };

}
#if 0
namespace functor {

template <class N>
struct traits<N, meta::when<
  is_product_type_v<N> && is_type_constructible_v<N>
>> : tag
{
  template <class T, class F>
    static constexpr auto transform(T&& x, F&& f)
    {
      return product_type::transform(forward<T>(x), forward<F>(f));
    }
};
}
#endif
}}
}

#endif // header
