//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_NULLABLE_TRANSFORM_HPP
#define JASEL_FUNDAMENTAL_V3_NULLABLE_TRANSFORM_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/nullable/none.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/functor/functor.hpp>
#include <utility>
#include <functional>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace nullable
{

  /**
   * functor::transform for TypeConstructible Nullable types
   */

  template <class N, class F
  // todo add constraint on F
  //, class = enable_if_t<
  //    is_nullable_v<meta::uncvref_t<N>>
  // && is_type_constructible_v<meta::uncvref_t<N>>
  //>
  >
  BOOST_CXX14_CONSTEXPR meta::invoke<TypeConstructor<decay_t<N>>, meta::ResultType<decay_t<F>, value_type_t<decay_t<N>>>>
  transform(N&& n, F&& f)
  {
    if (nullable::has_value(forward<N>(n)))
    {
      return make<TypeConstructor<decay_t<N>>>(
          JASEL_INVOKE(std::forward<F>(f), nullable::deref(forward<N>(n)))
      );
    }
    return nullable::none<meta::uncvref_t<N>>();
  }

  struct as_functor: functor::mcd_transform
  {
    template <class T, class F>
      static constexpr auto transform(T&& x, F&& f)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
        nullable::transform(forward<T>(x), forward<F>(f))
      )
  };
}

/**
 */
namespace functor {

template <class N>
struct traits<N, meta::when<
  is_nullable<N>::value && is_type_constructible<N>::value
>> : nullable::as_functor {};

}

}}
}
#endif // header
