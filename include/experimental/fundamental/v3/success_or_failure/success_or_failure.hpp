// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_SUCCESS_OR_FAILURE_HPP
#define JASEL_FUNDAMENTAL_V3_SUCCESS_OR_FAILURE_HPP

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

  namespace success_or_failure {

    namespace detail {
        struct not_a_success_or_failure_tag{};
    }
    // A SuccessOrFailure must specialize the following traits and be implicitly convertible from the success_type_t<T> and the failure_type_t<T>.
    // success_type_t<T> and the failure_type_t<T>
    template <class T, class Enabler=void>
      struct traits
#if ! defined JASEL_DOXYGEN_INVOKED
    : traits<T, meta::when<true>> {}
#endif
    ;

    // Default specialization
    template <typename T, bool condition>
    struct traits<T, meta::when<condition>> : detail::not_a_success_or_failure_tag
    {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED

        template <class U>
        static
        bool succeeded(U && ptr)  = delete;

        template <class U>
        static
        bool failed(U && ptr)  = delete;

        template <class U>
        static
        auto success_value(U && ptr) = delete;

        template <class U>
        static
        auto failure_value(U && ptr)  = delete;

#endif
    };


    struct traits_pointer_like
    {
        template <class U>
        static constexpr
        bool succeeded(U const& ptr) noexcept { return bool(ptr); }

        template <class U>
        static constexpr
        bool failed(U const& ptr) noexcept { return ! bool(ptr); }

        template <class U>
        static constexpr
        auto success_value(U && ptr)
          JASEL_DECLTYPE_RETURN (
              *(forward<U>(ptr))
          )

        template <class U>
        static constexpr
        nullptr_t failure_value(U && ptr) noexcept { return nullptr; }
    };

    // fixme
    //template <>
    //struct traits<add_pointer<_t>> : traits_pointer_like {};
    template <class T>
    struct traits<T*> : traits_pointer_like {};

    template <class T>
    constexpr
    auto succeeded(T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
        traits<decay_t<T>>::succeeded(forward<T>(x))
      )
    template <class T>
    constexpr
    auto failed(T && x)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      traits<decay_t<T>>::failed(forward<T>(x))
    )

    template <class T>
    constexpr
    bool succeeded(T const* ptr) noexcept {
      return ptr != nullptr;
    }

    template <class T>
    constexpr
    auto success_value(T && x)
      JASEL_DECLTYPE_RETURN (
        traits<decay_t<T>>::success_value(x)
      )

    template <class T>
    constexpr
    T& success_value(T* ptr) noexcept {
      return *ptr ;
    }

    template <class T>
      struct success_type;
    template <class T>
        using success_type_t = typename success_type<T>::type;

    template <class T>
      struct success_type { using type = remove_reference_t<decltype(success_or_failure::success_value(declval<T>()))>; };

    template <class T>
    constexpr
    auto failure_value(T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
          traits<decay_t<T>>::failure_value(forward<T>(x))
      )

    template <class T>
    constexpr
    std::nullptr_t failure_value(T* ptr) noexcept {
      return nullptr ;
    }

    template <class T>
      struct failure_type { using type = remove_reference_t<decltype(success_or_failure::failure_value(declval<T>()))>; };

    template <class TC>
    using failure_type_t = typename failure_type<TC>::type;

    struct mcd_succeeded
    {
        template <class U>
        static constexpr
        bool failed(U const& ptr) noexcept { return ! success_or_failure::succeeded(ptr); }
    };
    struct mcd_failed
    {
        template <class U>
        static constexpr
        bool succeeded(U const& ptr) noexcept { return ! success_or_failure::failed(ptr); }
    };
  }

//  using success_or_failure::failure_type_t;
//  using success_or_failure::failure_value;

  // todo: define in function of whether
  // EqualityComparable && DefaultConstructible && Destructible & PossibleValued
  // success_or_failure::failure_value(t)
  // T{nullable::success_value(t)}
  // T{nullable::failure_value(t)}

  template <class T>
  struct is_success_or_failure
#if ! defined JASEL_DOXYGEN_INVOKED
      : integral_constant<bool,
            ! is_base_of<success_or_failure::detail::not_a_success_or_failure_tag, success_or_failure::traits<T>>::value
        > {}
#endif
      ;
  template <class T>
  struct is_success_or_failure<const T> : is_success_or_failure<T> {};
  template <class T>
  struct is_success_or_failure<volatile T> : is_success_or_failure<T> {};
  template <class T>
  struct is_success_or_failure<const volatile T> : is_success_or_failure<T> {};

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
  constexpr bool is_success_or_failure_v = is_success_or_failure<T>::value ;
#endif

  template <class T>
  struct is_success_or_failure<T*>
#if ! defined JASEL_DOXYGEN_INVOKED
  : true_type {}
#endif
  ;

}
}
}

//#define JASEL_TRY_HELPER(UNIQUE, V, EXPR)
//auto UNIQUE = EXPR;
//if ( ! success_or_failure::succeeded(UNIQUE) ) return success_or_failure::failure_value(UNIQUE);
//auto V = success_or_failure::success_value(UNIQUE)

#endif // header
