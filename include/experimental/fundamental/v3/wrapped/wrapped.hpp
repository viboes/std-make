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

namespace std
{

#if __cplusplus < 201402L
template <class E>
using underlying_type_t = typename underlying_type<E>::type;

#endif

namespace experimental
{
inline namespace fundamental_v3
{

  // This is in relation with enums/underlying_type. However we don't want an enum to be unwrapped implicitly.
  // This is why we define the underlying operation. underlying is the identity by default.
  // For real wrapping types we need an explicit specialization.
  // Examples of Wrapped types are the proposed unexpected type constructor for expected errors and
  // the success/failure type constructors for result<T>.
  // These corresponds to the Haskell type-constructors. Maybe a better name is needed.
  namespace wrapping {
  namespace detail {
      struct not_a_wrapping_tag{};
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
        auto underlying(U && u)
        JASEL_DECLTYPE_RETURN_NOEXCEPT (
                forward<U>(u)
        )
    };

    template <typename T>
    struct traits<T, meta::when<is_enum<T>::value>>
    {
        template <class U>
        static
        auto underlying(U && u)
        JASEL_DECLTYPE_RETURN_NOEXCEPT (
                        static_cast<std::underlying_type_t<decay_t<U>>>(forward<U>(u))
        )
    };

    // fixme: should the result be a reference
    // If we want to be able to correspond to the behavior of the function expected::error() yes?
    // However underlying should be the identity by default.

    template <class T>
    constexpr
    auto underlying(T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
        traits<decay_t<T>>::underlying(forward<T>(x))
      )

    template <class T>
      struct unwrapped_type;
    template <class T>
        using unwrapped_type_t = typename unwrapped_type<T>::type;

    template <class T>
      struct unwrapped_type { using type = remove_reference_t<decltype(wrapping::underlying(declval<T>()))>; };

  }

  // todo: define in function of whether
  // wrapping::underlying(t)
  // Should the type T be explicitly convertible from wrapping::unwrap_type_t<T>?

  template <class T>
  struct is_wrapping
#if ! defined JASEL_DOXYGEN_INVOKED
      : integral_constant<bool,
            ! is_base_of<wrapping::detail::not_a_wrapping_tag, wrapping::traits<T>>::value
        > {}
#endif
      ;
  template <class T>
  struct is_wrapping<const T> : is_wrapping<T> {};
  template <class T>
  struct is_wrapping<volatile T> : is_wrapping<T> {};
  template <class T>
  struct is_wrapping<const volatile T> : is_wrapping<T> {};

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
  constexpr bool is_wrapping_v = is_wrapping<T>::value ;
#endif

  namespace detail
  {
      template <class T, class Enabler=void>
        struct traits
  #if ! defined JASEL_DOXYGEN_INVOKED
      : traits<T, meta::when<true>> {}
  #endif
      ;
      template <typename T, bool condition>
      struct traits<T, meta::when<condition>>
      {
          template <class U>
          static
          auto underlying_or_identity(U && u)
          JASEL_DECLTYPE_RETURN_NOEXCEPT (
                  forward<U>(u)
          )
      };
      template <typename T>
      struct traits<T, meta::when<is_wrapping<T>::value>>
      {
          template <class U>
          static
          auto underlying_or_identity(U && u)
          JASEL_DECLTYPE_RETURN_NOEXCEPT (
                   wrapping::underlying(forward<U>(u))
          )
      };

      template <class T>
      constexpr
      auto underlying_or_identity(T && x)
        JASEL_DECLTYPE_RETURN_NOEXCEPT (
          traits<decay_t<T>>::underlying_or_identity(forward<T>(x))
        )
  }

  using wrapping::unwrapped_type;
  using wrapping::unwrapped_type_t;
  using wrapping::underlying;
  using detail::underlying_or_identity;

  //! used instead of static_cast
  template <class U, class T>
  U underlying_cast(T const& v)
  {
    return static_cast<U>(underlying(v));
  }

}
}
}

//#define JASEL_TRY_HELPER(UNIQUE, V, EXPR)
//auto UNIQUE = EXPR;
//if ( ! wrapping::succeeded(UNIQUE) ) return wrapping::failure_value(UNIQUE);
//auto V = wrapping::success_value(UNIQUE)

#endif // header
