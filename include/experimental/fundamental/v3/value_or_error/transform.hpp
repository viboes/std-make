//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_TRANSFORM_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_TRANSFORM_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/value_or_error/value_or_error.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/functor/functor.hpp>
#include <utility>
#include <functional>
#include <iostream>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace value_or_error
{

  /**
   * functor::transform for TypeConstructible ValueOrError types
   */

  template <class N, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_value_or_error_v<meta::uncvref_t<N>>
  // && is_type_constructible_v<meta::uncvref_t<N>>
  //>
  >
  BOOST_CXX14_CONSTEXPR meta::invoke<TypeConstructor<meta::uncvref_t<N>>, meta::ResultType<decay_t<F>, value_type_t<meta::uncvref_t<N>>>>
  transform(N&& n, F&& f)
  {
    if (value_or_error::has_value(forward<N>(n)))
    {
      return make<TypeConstructor<meta::uncvref_t<N>>>(
          JASEL_INVOKE(std::forward<F>(f), value_or_error::deref(forward<N>(n)))
      );
    }
    return value_or_error::failure_value(forward<N>(n));
  }

  struct as_functor: functor::mcd_transform
  {
    template <class T, class F>
      static constexpr auto transform(T&& x, F&& f)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
        value_or_error::transform(forward<T>(x), forward<F>(f))
      )
  };
}

/**
 */
namespace functor {

template <class N>
struct traits<N, meta::when<
  is_value_or_error<N>::value && is_type_constructible<N>::value
>> : value_or_error::as_functor {};

}

}}
}
#endif // header
