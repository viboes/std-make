// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_POSSIBLY_VALUED_HPP
#define JASEL_FUNDAMENTAL_V3_POSSIBLY_VALUED_HPP

#include <experimental/type_constructible.hpp>
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

  namespace possibly_valued {
    struct tag {};

    // A PossiblyValued type must specialize the following traits and be implicitly convertible from the value_type_t<T> and the error_type_t<T>.
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
    };

    template <>
    struct traits<add_pointer<_t>> : traits_pointer_like {};
    template <class T>
    struct traits<T*> : traits_pointer_like {};



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
    struct value_type { using type = remove_reference_t<decltype(possibly_valued::deref(declval<T>()))>; };

  template <class M>
  auto have_value(M const& v)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      possibly_valued::has_value(v)
    )

  template <class M1, class M2, class ...Ms>
  auto have_value(M1 const& v1, M2 const& v2, Ms const& ...vs)
    //-> decltype(has_value(v1) && have_value(v2, vs...))
    noexcept(noexcept(possibly_valued::has_value(v1)))
    -> decltype(possibly_valued::has_value(v1))

  {
    return possibly_valued::has_value(v1) && have_value(v2, vs...);
  }

}

//  using possibly_valued::has_value;
//  using possibly_valued::have_value;
//  using possibly_valued::deref;

  // todo: define in function of whether
  // possibly_valued::has_value(t) -> {bool}
  // possibly_valued::deref(t)


  template <class T>
  struct is_possibly_valued
#if ! defined JASEL_DOXYGEN_INVOKED
      : is_base_of<possibly_valued::tag, possibly_valued::traits<T>> {}
#endif
      ;
  template <class T>
  struct is_possibly_valued<const T> : is_possibly_valued<T> {};
  template <class T>
  struct is_possibly_valued<volatile T> : is_possibly_valued<T> {};
  template <class T>
  struct is_possibly_valued<const volatile T> : is_possibly_valued<T> {};

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
  constexpr bool is_possibly_valued_v = is_possibly_valued<T>::value ;
#endif

  template <class T>
  struct is_possibly_valued<T*>
#if ! defined JASEL_DOXYGEN_INVOKED
  : true_type {}
#endif
  ;

}
}
}

#endif // header
