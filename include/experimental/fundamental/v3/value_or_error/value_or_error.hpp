// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

// fixme: move has_value and deref to PossiblyValued: ValuedOrError and Nullable are models of PossiblyValued

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_HPP

#include <experimental/type_traits.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/meta/v1/when.hpp>
#include <experimental/wrapped.hpp>
#include <functional>

#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  namespace value_or_error {

    namespace detail {
        struct not_a_value_or_error_tag{};
    }

    // A ValueOrError must specialize the following traits and be implicitly convertible from the value_type_t<T> and the error_type_t<T>.
    // value_type_t<T> and the error_type_t<T>
    template <class T, class Enabler=void>
      struct traits
#if ! defined JASEL_DOXYGEN_INVOKED
    : traits<T, meta::when<true>> {}
#endif
    ;

    // Default specialization
    template <typename T, bool condition>
    struct traits<T, meta::when<condition>> : detail::not_a_value_or_error_tag
    {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
        template <class U>
        static constexpr
        bool succeeded(U && ptr)  = delete;

        template <class U>
        static constexpr
        bool failed(U && ptr)  = delete;

        template <class U>
        static constexpr
        auto success_value(U && ptr) = delete;

        template <class U>
        static constexpr
        auto failure_value(U && ptr)  = delete;

        template <class U>
        static
        bool has_value(U && ptr)  = delete;

        template <class U>
        static
        auto deref(U && ptr) = delete;

        template <class U>
        static
        auto error(U && ptr)  = delete;

#endif
    };


    template <class T>
    constexpr
    auto succeeded(T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
        traits<meta::uncvref_t<T>>::succeeded(forward<T>(x))
      )
    template <class T>
    constexpr
    auto failed(T && x)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      traits<meta::uncvref_t<T>>::failed(forward<T>(x))
    )

    template <class T>
    constexpr
    auto success_value(T && x)
      JASEL_DECLTYPE_RETURN (
        traits<meta::uncvref_t<T>>::success_value(x)
      )

    template <class T>
      struct success_type;
    template <class T>
        using success_type_t = typename success_type<T>::type;

    template <class T>
      struct success_type { using type = remove_reference_t<decltype(value_or_error::success_value(declval<T>()))>; };

    template <class T>
    constexpr
    auto failure_value(T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
          traits<meta::uncvref_t<T>>::failure_value(forward<T>(x))
      )

    template <class T>
      struct failure_type { using type = remove_reference_t<decltype(value_or_error::failure_value(declval<T>()))>; };

    template <class TC>
    using failure_type_t = typename failure_type<TC>::type;

    template <class T>
    constexpr
    auto has_value(T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
        traits<meta::uncvref_t<T>>::has_value(forward<T>(x))
      )

    template <class T>
    constexpr
    auto has_error(T && x)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      ! traits<meta::uncvref_t<T>>::has_value(forward<T>(x))
    )

    template <class T>
    constexpr
    auto deref(T && x)
      JASEL_DECLTYPE_RETURN (
        traits<meta::uncvref_t<T>>::deref(x)
      )

    template <class T>
      struct value_type;
    template <class T>
        using value_type_t = typename value_type<T>::type;

    template <class T>
      struct value_type { using type = remove_reference_t<decltype(value_or_error::deref(declval<T>()))>; };

    template <class M>
    auto have_value(M && v)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
              value_or_error::has_value(std::forward<M>(v))
      )

    template <class M1, class M2, class ...Ms>
    auto have_value(M1 && v1, M2 && v2, Ms && ...vs)
      //-> decltype(has_value(std::forward<M1>(v1)) && have_value(std::forward<M1>(v2), std::forward<M1>(vs)...))
      noexcept(noexcept(value_or_error::has_value(std::forward<M1>(v1))))
      -> decltype(value_or_error::has_value(std::forward<M1>(v1)))

    {
      return value_or_error::has_value(std::forward<M1>(v1)) && have_value(std::forward<M1>(v2), std::forward<M1>(vs)...);
    }

    template <class T>
    constexpr
    auto error(T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
          traits<meta::uncvref_t<T>>::error(forward<T>(x))
      )

    template <class T>
      struct error_type { using type = remove_reference_t<decltype(value_or_error::error(declval<T>()))>; };

    template <class TC>
    using error_type_t = typename error_type<TC>::type;

    // mcd for typed defining the SuccessOrFailure types.
    struct mcd_success_or_failure
    {

        template <class U>
        static constexpr
        bool failed(U && u) noexcept { return ! value_or_error::succeeded(forward<U>(u)); }

        template <class U>
        static constexpr
        bool has_value(U && u)  { return value_or_error::succeeded(forward<U>(u)); }

        template <class U>
        static constexpr
        auto deref(U && u)
        JASEL_DECLTYPE_RETURN_NOEXCEPT (
                underlying_or_identity(value_or_error::success_value(forward<U>(u)))
        )

        template <class U>
        static constexpr
        auto error(U && u)
        JASEL_DECLTYPE_RETURN_NOEXCEPT (
                underlying_or_identity(value_or_error::failure_value(forward<U>(u)))
        )

    };


  }

//  using value_or_error::error_type_t;
//  using value_or_error::error;

  // todo: define in function of whether
  // EqualityComparable && DefaultConstructible && Destructible & PossibleValued
  // value_or_error::error(t)
  // T{value_or_error::deref(t)}

  template <class T>
  struct is_value_or_error
#if ! defined JASEL_DOXYGEN_INVOKED
          : integral_constant<bool,
                ! is_base_of<value_or_error::detail::not_a_value_or_error_tag, value_or_error::traits<T>>::value
            > {}
#endif
      ;
  template <class T>
  struct is_value_or_error<const T> : is_value_or_error<T> {};
  template <class T>
  struct is_value_or_error<volatile T> : is_value_or_error<T> {};
  template <class T>
  struct is_value_or_error<const volatile T> : is_value_or_error<T> {};

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
  constexpr bool is_value_or_error_v = is_value_or_error<T>::value ;
#endif

}
}
}

//#define JASEL_TRY_HELPER(UNIQUE, V, EXPR)
//auto UNIQUE = EXPR;
//if ( ! value_or_error::has_value(UNIQUE) ) return value_or_error::failure_value(UNIQUE);
//auto V = value_or_error::deref(UNIQUE)

#endif // header
