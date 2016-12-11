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
  namespace detail {

    template <class TC, class F, class ProductType, std::size_t... I>
    constexpr decltype(auto) transform_impl( ProductType&& pt, F&& f, index_sequence<I...> )
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

  } // namespace detail

  /**
   * Invoke the Callable object f for each one of the product type elements and return the result wrapped using a tuple.
   *
   * @par f Callable object to be invoked on each product type element
   * @par pt product type whose elements to be used as arguments to f
   *
   * @pre
   * - DProductType is a model of ProductType.
   * - DF is a model of Callable<product_type::element_t<I, DProductType>>...  and
   *
   * where DF and DProductType are respectively the decay of F and ProductType removing cv and reference.
   */

  template <class F, class ProductType
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>> >
  >
  constexpr decltype(auto) transform(ProductType&& pt, F&& f)
  {
      return detail::transform_impl<type_constructor_t<remove_cv_t<remove_reference_t<ProductType>>>>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }

  template <class TC, class F, class ProductType
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>> >
  >
  constexpr decltype(auto) transform(ProductType&& pt, F&& f)
  {
      return detail::transform_impl<TC>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }

  template <class F, class ProductType
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>> >
  >
  constexpr decltype(auto) p_transform(ProductType&& pt, F&& f)
  {
      return detail::p_transform_impl<type_constructor_t<remove_cv_t<remove_reference_t<ProductType>>>>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }

  template <class TC, class F, class ProductType
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>> >
  >
  constexpr decltype(auto) p_transform(ProductType&& pt, F&& f)
  {
      return detail::p_transform_impl<TC>(
          forward<ProductType>(pt), forward<F>(f),
          product_type::element_sequence_for<ProductType>{});
  }

  struct as_n_functor : n_functor::tag
  {
    template <class T, class F>
      static constexpr auto transform(T&& x, F&& f)
      {
        return product_type::transform(forward<T>(x), forward<F>(f));
      }
  };

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
