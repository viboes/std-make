// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_WRAPPED_HPP
#define JASEL_FUNDAMENTAL_V3_WRAPPED_HPP

#include <experimental/type_traits.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/meta/v1/when.hpp>
#include <functional>

#include <utility>
#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  // This is in relation with enums/underlying_type. However we don't want an enum to be unwrapped implicitly.
  // This is why we define the unwrap operation. unwrap is the identity by default.
  // For real wrapped types we need an explicit specialization.
  // Examples of Wrapped types are the proposed unexpected type constructor for expected errors and
  // the success/failure type constructors for result<T>.
  // These corresponds to the Haskell type-constructors. Maybe a better name is needed.
  namespace wrapped {
  namespace detail {
      struct not_a_wrapped_tag{};
  }

    // A Wrapped must specialize the following traits
    template <class T, class Enabler=void>
      struct traits
#if ! defined JASEL_DOXYGEN_INVOKED
    : traits<T, meta::when<true>> {}
#endif
    ;

    // fixme:: Shouldn't we use SFINAE here?
    // Default specialization
    template <typename T, bool condition>
    struct traits<T, meta::when<condition>>
    {
        template <class U>
        static
        auto unwrap(U && u)
        JASEL_DECLTYPE_RETURN_NOEXCEPT (
                forward<U>(u)
        )
    };

    // fixme: should the result be a reference
    // If we want to be able to correspond to the behavior of the function expected::error() yes?
    // However unwrap should be the identity by default.

    template <class T>
    constexpr
    auto unwrap(T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
        traits<decay_t<T>>::unwrap(forward<T>(x))
      )

    template <class T>
      struct unwrapped_type;
    template <class T>
        using unwrapped_type_t = typename unwrapped_type<T>::type;

    template <class T>
      struct unwrapped_type { using type = remove_reference_t<decltype(wrapped::unwrap(declval<T>()))>; };

  }

  using wrapped::unwrapped_type;
  using wrapped::unwrapped_type_t;
  using wrapped::unwrap;

  // todo: define in function of whether
  // wrapped::unwrap(t)
  // Should the type T be explicitly convertible from wrapped::unwrap_type_t<T>?

  template <class T>
  struct is_wrapped
#if ! defined JASEL_DOXYGEN_INVOKED
      : integral_constant<bool,
            ! is_base_of<wrapped::detail::not_a_wrapped_tag, wrapped::traits<T>>::value
        > {}
#endif
      ;
  template <class T>
  struct is_wrapped<const T> : is_wrapped<T> {};
  template <class T>
  struct is_wrapped<volatile T> : is_wrapped<T> {};
  template <class T>
  struct is_wrapped<const volatile T> : is_wrapped<T> {};

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
  constexpr bool is_wrapped_v = is_wrapped<T>::value ;
#endif


}
}
}

//#define JASEL_TRY_HELPER(UNIQUE, V, EXPR)
//auto UNIQUE = EXPR;
//if ( ! wrapped::succeeded(UNIQUE) ) return wrapped::failure_value(UNIQUE);
//auto V = wrapped::success_value(UNIQUE)

#endif // header
