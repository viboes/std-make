// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

// fixme: move has_value and deref to PossiblyValued: ValuedOrError and Nullable are models of PossiblyValued

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_ERROR_HPP

//#include <experimental/type_constructible.hpp>
#include <experimental/possibly_valued.hpp>
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

  namespace value_or_error {
    using namespace possibly_valued;

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
      auto error(U && ptr)  = delete;

#endif
    };

    struct traits_pointer_like : tag
    {
      template <class U>
      static constexpr
      nullptr_t error(U && ptr) noexcept { return nullptr; }
    };

    template <>
    struct traits<add_pointer<_t>> : traits_pointer_like {};
    template <class T>
    struct traits<T*> : traits_pointer_like {};

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
//if ( ! value_or_error::has_value(UNIQUE) ) return value_or_error::error(UNIQUE);
//auto V = value_or_error::deref(UNIQUE)

#endif // header
