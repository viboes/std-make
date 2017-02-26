//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_TO_TUPLE_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_TO_TUPLE_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
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
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>>  >
  >
  constexpr decltype(auto) to_tuple(ProductType&& pt)
  {
      return detail::to_tuple_impl(forward<ProductType>(pt),
          product_type::element_sequence_for<ProductType>{});
  }



#if 0
  /**
   * Constructs a tuple having the same elements as the product type arg selecting the index in the IndexSequence.
   *
   * @par arg  a product_type
   *
   * @pre The parameter must be a model of ProductType
   */

  template <class IndexSequence, class ProductType
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>>  >
  >
  constexpr decltype(auto) select_to_tuple(ProductType&& pt)
  {
      return detail::to_tuple_impl(forward<ProductType>(pt),
          IndexSequence{});
  }

  template <class T, T J, class IntSeq>
  struct add_integer_sequence;
  template <class T, T J, T ... Is>
  struct add_integer_sequence<T, J, integer_sequence<T, Is...>>
    : integer_sequence<T, (J+Is)...>  { };

  template <class IntSeq, class T, T I>
  struct push_back_integer_sequence;
  template <class T, T ... Is, T I>
  struct push_back_integer_sequence<integer_sequence<T, Is...>, T, I>
    : integer_sequence<T, Is..., I>  { };

  template<typename T, typename=integer_sequence<typename T::value_type>>
  struct reverse_integer_sequence;
  template<typename T, class IntegerSequence>
  struct reverse_integer_sequence<integer_sequence<T>, IntegerSequence> :  IntegerSequence  { };
  template<typename T, T first, T... rest, T... done>
  struct reverse_integer_sequence<integer_sequence<T, first, rest...>, integer_sequence<T, done...>> :
    reverse_integer_sequence<integer_sequence<T, rest...>, integer_sequence<T, first, done...>>
  { };


  template <class T, T I, T J>
  using make_slice_integer_sequence =
      conditional_t<I<=J,
        add_integer_sequence<I, make_integer_sequence<T, J - I + 1>>,
        reverse_integer_sequence<add_integer_sequence<J, make_integer_sequence<T, I - J + 1>>
      >
      ;


  /**
   * Constructs a tuple having the same elements as the product type arg sliced by the indexes I, J.
   *
   * @par arg  a product_type
   *
   * @pre The parameter must be a model of ProductType
   */

  template <std::size_t I, std::size_t J, class ProductType
  , class = enable_if_t<
      is_product_type_v<meta::uncvref_t<ProductType>>
      && I < product_type::size_v<ProductType>
      && J < product_type::size_v<ProductType>
      >
  >
  constexpr decltype(auto) slice_to_tuple(ProductType&& pt)
  {
      return detail::to_tuple_impl(forward<ProductType>(pt),
          make_slice_integer_sequence<size_t, I, J, ProductType>{});
  }

  template<typename IndexPred, typename IntegerSequence,
    typename ResIntegerSequence = integer_sequence<typename T::value_type>
  >
      struct make_projected_integer_sequence;
  template<typename IndexPred, typename T, class ResIntegerSequence>
      struct make_projected_integer_sequence<IndexPred, integer_sequence<T>, ResIntegerSequence> : ResIntegerSequence  { };
  template<typename IndexPred, typename T, T First, T... Rest, class ResIntegerSequence>
      struct make_projected_integer_sequence<IndexPred, integer_sequence<T, First, Rest...>, ResIntegerSequence> :
          make_projected_integer_sequence<IndexPred, integer_sequence<T, Rest...>,
              conditional_t<IndexPred<T, I>::value,
                  push_back_integer_sequence<ResIntegerSequence, First>,
                  ResIntegerSequence
              >
          >
  { };


  /**
   * Constructs a tuple having the same elements as the product type arg projecting the index satisfying the IndexPred.
   *
   * @par arg  a product_type
   *
   * @pre The parameter must be a model of ProductType
   */

  template <class IndexPred, class ProductType
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>>  >
  >
  constexpr decltype(auto) project_to_tuple(ProductType&& pt)
  {
      using PT = meta::uncvref_t<PT2>;
      return detail::to_tuple_impl(forward<ProductType>(pt),
          make_projected_integer_sequence<IndexPred, product_type::element_sequence_for<PT>>{}{});
  }




  /**
   * Constructs a tuple having the same elements as the product type pt projecting the index satisfying the TypePred.
   *
   * @par pt  a product_type
   *
   * @pre The parameter must be a model of ProductType
   */
namespace detail {
  template <class TypePred, typename PT>
  struct TypeToIndexPred {
    template <class T, T Index>
    constexpr bool const value =  TypePred<product_type::element_t<Index,PT>>::value;
  };
}
  template <class TypePred, class PT2
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<PT2>>  >
  >
  constexpr decltype(auto) filter_to_tuple(PT2&& pt)
  {
      using PT = meta::uncvref_t<PT2>;
      return detail::to_tuple_impl(forward<ProductType>(pt),
          make_project_index_sequence<detail::TypeToIndexPred<TypePred, PT>, product_type::element_sequence_for<PT>>{});
  }
#endif

}
}}
}

#endif // header
