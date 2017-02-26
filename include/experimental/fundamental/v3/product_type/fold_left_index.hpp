//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_FOLD_LEFT_INDEX_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_FOLD_LEFT_INDEX_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
#include <experimental/fundamental/v3/product_type/fold_left.hpp>
#include <utility>

// fixme: fold_let is implementable, as Hana does it, using foldable::apply (hana::unpack).
// Which implementation is more efficient at run-time? at compile time?

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace product_type
{
  namespace detail {

    template <class F, class State, class ProductType>
    constexpr decltype(auto) fold_left_index_impl( ProductType&& pt, State&& state, F&& f, index_sequence<> )
    {
      return forward<State>(state);
    }

    template <class F, class State, class ProductType, std::size_t I , std::size_t... Is>
    constexpr decltype(auto) fold_left_index_impl( ProductType&& pt, State&& state, F&& f, index_sequence<I, Is...> )
    {
      return fold_left_index_impl(
          forward<ProductType>(pt),
          f(integral_constant<size_t, I>{}, forward<State>(state), product_type::get<I>(forward<ProductType>(pt))),
          forward<F>(f),
          index_sequence<Is...>{}
          );
    }

  } // namespace detail

  /**
   * Left-fold of a structure using a binary operation and an optional initial reduction state.
   * fold_left_index is a left-associative fold using a binary operation.
   * Given a structure containing x1, ..., xn, a function f and an optional initial state, fold_left_index applies f as follows.
   *  * f(... f(f(f(x1, x2), x3), x4) ..., xn) // without state
   *  * f(... f(f(f(f(state, x1), x2), x3), x4) ..., xn) // with state
   *
   *  When the structure is empty, two things may arise. If an initial state was provided, it is returned as-is.
   *  Otherwise, if the no-state version of the function was used, an error is triggered.
   *  When the stucture contains a single element and the no-state version of the function was used,
   *  that single element is returned as is.
   */

  template <class F, class State, class ProductType
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<meta::uncvref_t<ProductType>> >
  >
  constexpr decltype(auto) fold_left_index(ProductType&& pt, State&& state, F&& f)
  {
    return detail::fold_left_index_impl(forward<ProductType>(pt), forward<State>(state), forward<F>(f),
        product_type::element_sequence_for<ProductType>{});
  }

  template <class F, class ProductType
  // todo add constraint on F
  , class = enable_if_t<
              product_type::friendly_type_trait<
                meta::uncvref_t<ProductType>,
                product_type::not_empty
              >::value
      >
  >
  constexpr decltype(auto) fold_left_index(ProductType&& pt, F&& f)
  {
    return detail::fold_left_index_impl(forward<ProductType>(pt), product_type::get<0>(forward<ProductType>(pt)), forward<F>(f),
          detail::drop_front_t<product_type::element_sequence_for<ProductType>>{});
  }

}
}}
}

#endif // header
