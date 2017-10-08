// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_VALEU_OR_NONE_HPP
#define JASEL_FUNDAMENTAL_V3_VALUE_OR_NONE_VALEU_OR_NONE_HPP

#include <experimental/nullable.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/type_traits.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/meta/v1/when.hpp>
#include <experimental/value_or_error.hpp>

#include <functional>

#include <utility>
#include <experimental/type_traits.hpp>


// the operation deref_none make Nullable something more like Error-able.
// It allows to forward errors from one Nullable to another.

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

namespace value_or_none
{
    using namespace nullable;

    namespace detail {
        struct not_a_value_or_none_tag{};
    }

    template <class T, class Enabler=void>
    struct traits
#if ! defined JASEL_DOXYGEN_INVOKED
    : traits<T, meta::when<true>>
    {}
#endif
    ;

    // Default specialization
    template <typename T, bool condition>
    struct traits<T, meta::when<condition>> : detail::not_a_value_or_none_tag
    {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
        template <class U>
        static
        auto deref(U && ptr) = delete;

#endif
    };

    struct traits_pointer_like
    {
        template <class U>
        static constexpr
        auto deref(U && ptr)
        JASEL_DECLTYPE_RETURN (
               *(forward<U>(ptr))
        )
    };

    template <>
    struct traits<add_pointer<_t>> : traits_pointer_like
    {};
    template <class T>
    struct traits<T*> : traits_pointer_like
    {};

    template <class T>
    constexpr
    auto deref(T && x)
    JASEL_DECLTYPE_RETURN (
            traits<decay_t<T>>::deref(x)
    )

    template <class T>
    constexpr
    T& deref(T* ptr) noexcept
    {
        return *ptr;
    }

    template <class T>
    struct value_type;
    template <class T>
    using value_type_t = typename value_type<T>::type;

    template <class T>
    struct value_type
    {
        using type = remove_reference_t<decltype(value_or_none::deref(declval<T>()))>;
    };

    template <class T>
    constexpr
    auto deref_none(T&& )
    JASEL_DECLTYPE_RETURN (
            none<decay_t<T>>()
    )

    template <class T>
    constexpr
    std::nullptr_t deref_none(T*) noexcept
    {
        return nullptr;
    }

}

//using value_or_none::value_type;
//using value_or_none::deref;
//using value_or_none::deref_none;

  // todo: define in function of whether
  // EqualityComparable && DefaultConstructible && Destructible & PossibleValued & Nullable
  // value_or_none::deref(t)
  // T{value_or_none::deref(t)}

  template <class T>
  struct is_value_or_none
#if ! defined JASEL_DOXYGEN_INVOKED
          : integral_constant<bool,
                ! is_base_of<value_or_none::detail::not_a_value_or_none_tag, value_or_none::traits<T>>::value
            > {}
#endif
      ;
  template <class T>
  struct is_value_or_none<const T> : is_value_or_none<T> {};
  template <class T>
  struct is_value_or_none<volatile T> : is_value_or_none<T> {};
  template <class T>
  struct is_value_or_none<const volatile T> : is_value_or_none<T> {};

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
  constexpr bool is_value_or_none_v = is_value_or_none<T>::value ;
#endif

  template <class T>
  struct is_value_or_none<T*>
#if ! defined JASEL_DOXYGEN_INVOKED
  : true_type {}
#endif
  ;

namespace value_or_error
{
  template <class T>
  struct traits<T, meta::when<
    is_value_or_none<T>::value
  >> : mcd_success_or_failure
  {
      template <class U>
      static constexpr
      bool succeeded(U && u)
      { return value_or_none::has_value(forward<U>(u)); }

      template <class U>
      static constexpr
      bool failed(U && u)
      { return ! value_or_none::has_value(forward<U>(u)); }

      template <class U>
      static constexpr
      auto success_value(U && u)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
              value_or_none::deref(forward<U>(u))
      )

      template <class U>
      static constexpr
      auto failure_value(U && u)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
              value_or_none::deref_none(forward<U>(u))
      )

  };
}

}
}
}

#endif // header
