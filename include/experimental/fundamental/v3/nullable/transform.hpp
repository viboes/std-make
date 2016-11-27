//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201402L

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
  //    is_nullable_v<remove_cv_t<remove_reference_t<N>>>
  // && is_type_constructible_v<remove_cv_t<remove_reference_t<N>>>
  //>
  >
  constexpr meta::invoke<TypeConstructor<decay_t<N>>, meta::ResultType<decay_t<F>, value_type_t<decay_t<N>>>>
  transform(N&& n, F&& f)
  {
    if (nullable::has_value(forward<N>(n)))
      return make<TypeConstructor<decay_t<N>>>(
#if defined JASEL_HAS_INVOKE
          invoke(std::forward<F>(f), nullable::deref(forward<N>(n)))
#else
          std::forward<F>(f)(nullable::deref(forward<N>(n)))
#endif
      );
    else
      return nullable::deref_none(forward<N>(n));
  }

  struct as_functor: functor::tag
  {
    template <class T, class F>
      static constexpr auto transform(T&& x, F&& f)
      {
        return nullable::transform(forward<T>(x), forward<F>(f));
      }
  };
}

/**
 */
#if 0
namespace functor {
template <class N>
struct traits<N, meta::when<
  is_nullable_v<N> && is_type_constructible_v<N>
>> : tag
{
  template <class T, class F>
    static constexpr auto transform(T&& x, F&& f)
    {
      return nullable::transform(forward<T>(x), forward<F>(f));
    }
};
}
#endif
}}
}
#endif // header
#endif
