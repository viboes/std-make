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
#include <experimental/success_or_failure.hpp>
#include <experimental/wrapped.hpp>
#include <functional>

#include <utility>
#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  namespace value_or_error {

    struct tag {};

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
    struct traits<T, meta::when<condition>>
    {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED

        template <class U>
        static
        bool has_value(U const& ptr)  = delete;

        template <class U>
        static
        auto deref(U && ptr) = delete;

        template <class U>
        static
        auto error(U && ptr)  = delete;

#endif
    };

    struct traits_pointer_like : tag
    {
        template <class U>
        static constexpr
        bool has_value(U const& ptr) noexcept { return bool(ptr); }

        template <class U>
        static constexpr
        auto deref(U && ptr)
          JASEL_DECLTYPE_RETURN (
              *(forward<U>(ptr))
          )

        template <class U>
        static constexpr
        nullptr_t error(U && ptr) noexcept { return nullptr; }
    };

    // fixme
    //template <>
    //struct traits<add_pointer<_t>> : traits_pointer_like {};
    template <class T>
    struct traits<T*> : traits_pointer_like {};

    // specialization for SuccessOrFailure types.
    template <typename T>
    struct traits<T, meta::when<is_success_or_failure<T>::value>> : tag
    {

        template <class U>
        static
        bool has_value(U && u)  { return success_or_failure::succeeded(forward<U>(u)); }

        template <class U>
        static
        auto deref(U && u)
        JASEL_DECLTYPE_RETURN_NOEXCEPT (
                wrapped::unwrap(success_or_failure::success_value(forward<U>(u)))
        )

        template <class U>
        static
        auto error(U && u)
        JASEL_DECLTYPE_RETURN_NOEXCEPT (
                wrapped::unwrap(success_or_failure::failure_value(forward<U>(u)))
        )

    };

    template <class T>
    constexpr
    auto has_value(T const& x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
        traits<T>::has_value(x)
      )

    template <class T>
    constexpr
    bool has_value(T const* ptr) noexcept {
      return ptr != nullptr;
    }

    template <class T>
    constexpr
    auto deref(T && x)
      JASEL_DECLTYPE_RETURN (
        traits<decay_t<T>>::deref(x)
      )

    template <class T>
    constexpr
    T& deref(T* ptr) noexcept {
      return *ptr ;
    }

    template <class T>
      struct value_type;
    template <class T>
        using value_type_t = typename value_type<T>::type;

    template <class T>
      struct value_type { using type = remove_reference_t<decltype(value_or_error::deref(declval<T>()))>; };

    template <class M>
    auto have_value(M const& v)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
        value_or_error::has_value(v)
      )

    template <class M1, class M2, class ...Ms>
    auto have_value(M1 const& v1, M2 const& v2, Ms const& ...vs)
      //-> decltype(has_value(v1) && have_value(v2, vs...))
      noexcept(noexcept(value_or_error::has_value(v1)))
      -> decltype(value_or_error::has_value(v1))

    {
      return value_or_error::has_value(v1) && have_value(v2, vs...);
    }

    template <class T>
    constexpr
    auto error(T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
          traits<decay_t<T>>::error(forward<T>(x))
      )

    template <class T>
    constexpr
    std::nullptr_t error(T* ptr) noexcept {
      return nullptr ;
    }

    template <class T>
      struct error_type { using type = remove_reference_t<decltype(value_or_error::error(declval<T>()))>; };

    template <class TC>
    using error_type_t = typename error_type<TC>::type;

  }

//  using value_or_error::error_type_t;
//  using value_or_error::error;

  // todo: define in function of whether
  // EqualityComparable && DefaultConstructible && Destructible & PossibleValued
  // value_or_error::error(t)
  // T{nullable::deref(t)}
  // T{nullable::error(t)}

  template <class T>
  struct is_value_or_error
#if ! defined JASEL_DOXYGEN_INVOKED
      : is_base_of<value_or_error::tag, value_or_error::traits<T>> {}
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

  template <class T>
  struct is_value_or_error<T*>
#if ! defined JASEL_DOXYGEN_INVOKED
  : true_type {}
#endif
  ;

}
}
}

//#define JASEL_TRY_HELPER(UNIQUE, V, EXPR)
//auto UNIQUE = EXPR;
//if ( ! value_or_error::has_value(UNIQUE) ) return value_or_error::failure_value(UNIQUE);
//auto V = value_or_error::deref(UNIQUE)

#endif // header
