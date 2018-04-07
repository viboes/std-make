// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_NULLABLE_NONE_HPP
#define JASEL_FUNDAMENTAL_V3_NULLABLE_NONE_HPP

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

  namespace nullable {
  namespace detail {
      struct not_a_nullable_tag{};
  }

    template <class T, class Enabler=void>
      struct traits
#if ! defined JASEL_DOXYGEN_INVOKED
    : traits<T, meta::when<true>> {}
#endif
    ;

    // Default specialization
    template <typename T, bool condition>
    struct traits<T, meta::when<condition>> : detail::not_a_nullable_tag
    {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
      static
      auto none()  = delete;

      template <class U>
      static
      bool has_value(U && ptr)  = delete;

      template <class N, class VT>
      static
      auto holds_alternative_value(N&& n, VT && x) = delete;
#endif
    };

    struct traits_pointer_like
    {
      static constexpr
      nullptr_t none() noexcept { return nullptr; }

      template <class Ptr>
      static constexpr
      bool has_value(Ptr && ptr) noexcept { return bool(forward<Ptr>(ptr)); }

      template <class Ptr, class T>
      static constexpr
      auto holds_alternative_value(Ptr&& ptr, T && x)
      {
          return has_value(ptr) && *ptr == x;
      }
    };

    template <>
    struct traits<add_pointer<_t>> : traits_pointer_like {};
    template <class T>
    struct traits<T*> : traits_pointer_like {};

  struct none_t {
    explicit none_t() = default;
    template <class T>
    operator T*() const noexcept // NOLINT google-explicit-constructor
    { return nullptr; }
  };
  constexpr bool operator==(none_t, none_t) noexcept { return true; }
  constexpr bool operator!=(none_t, none_t) noexcept { return false; }
  constexpr bool operator<(none_t, none_t) noexcept { return false; }
  constexpr bool operator<=(none_t, none_t) noexcept { return true; }
  constexpr bool operator>(none_t, none_t) noexcept { return false; }
  constexpr bool operator>=(none_t, none_t) noexcept { return true; }

  constexpr
  none_t none() noexcept { return none_t{}; }

  template <class TC>
  constexpr
  auto none()
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      traits<TC>::none()
    )

  template <template <class ...> class TC>
  constexpr
  auto none()
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
        none<type_constructor_t<meta::quote<TC>>>()
        //none< TC<_t> >()
    )

  template <class TC>
  struct none_type;

  template <class TC>
  struct none_type {
    using type = decltype(nullable::none<TC>());
  };
  template <class TC>
  using none_type_t = typename none_type<TC>::type;

  template <class N>
  constexpr
  auto has_value(N && n)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      traits<meta::uncvref_t<N>>::has_value(std::forward<N>(n))
    )

  template <class N, class T,
      enable_if_t<! is_same<T, none_t>::value, int> = 0
  >
  constexpr
  auto holds_alternative_value(N&& n, T && x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT (
        traits<meta::uncvref_t<N>>::holds_alternative_value(std::forward<N>(n), std::forward<T>(x))
      )

   template <class N>
   constexpr
   auto holds_alternative_value(N&& n, none_t)
          JASEL_DECLTYPE_RETURN_NOEXCEPT (
            ! has_value(std::forward<N>(n))
          )

//  template <class T>
//  constexpr
//  bool has_value(T const* ptr) noexcept {
//    return ptr != nullptr;
//  }

  template <class M>
  auto have_value(M && v)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      nullable::has_value(std::forward<M>(v))
    )

  template <class M1, class M2, class ...Ms>
  auto have_value(M1 && v1, M2 && v2, Ms && ...vs)
    //-> decltype(has_value(std::forward<M1>(v1)) && have_value(std::forward<M1>(v2), std::forward<M1>(vs)...))
    noexcept(noexcept(nullable::has_value(std::forward<M1>(v1))))
    -> decltype(nullable::has_value(std::forward<M1>(v1)))

  {
    return nullable::has_value(std::forward<M1>(v1)) && have_value(std::forward<M1>(v2), std::forward<M1>(vs)...);
  }

}

  using nullable::none_t;
  using nullable::none_type_t;
  using nullable::none;
  using nullable::has_value;
  using nullable::holds_alternative_value;
  using nullable::have_value;

  // todo: define in function of whether
  // EqualityComparable && DefaultConstructible && Destructible & PossibleValued
  // nullable::none<T>()
  // nullable::has_value(t) -> {bool}
  // T{}
  // T{nullable::none_t}
  // T{nullable::none<T>()}

  template <class T>
  struct is_nullable
#if ! defined JASEL_DOXYGEN_INVOKED
          : integral_constant<bool,
                ! is_base_of<nullable::detail::not_a_nullable_tag, nullable::traits<T>>::value
            > {}
#endif
      ;
  template <class T>
  struct is_nullable<const T> : is_nullable<T> {};
  template <class T>
  struct is_nullable<volatile T> : is_nullable<T> {};
  template <class T>
  struct is_nullable<const volatile T> : is_nullable<T> {};

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
  constexpr bool is_nullable_v = is_nullable<T>::value ;
#endif

//  template <class T>
//  struct is_nullable<T*>
//#if ! defined JASEL_DOXYGEN_INVOKED
//  : true_type {}
//#endif
//  ;

  // todo: implement this traits
  template <class T>
  struct is_strict_weakly_ordered
#if ! defined JASEL_DOXYGEN_INVOKED
      : false_type {}
#endif
      ;

  template <class T>
  struct is_strict_weakly_ordered_nullable
#if ! defined JASEL_DOXYGEN_INVOKED
      : conjunction<
    is_strict_weakly_ordered<T>
  , is_nullable<T>
  > {}
#endif
      ;

  namespace nullable {

  template < class C, enable_if_t<is_nullable<C>::value, int> = 0 >
  constexpr bool operator==(none_t, C const& x) { return ! nullable::has_value(x); }
  template < class C, enable_if_t<is_nullable<C>::value, int> = 0  >
  constexpr bool operator==(C const& x, none_t) { return ! nullable::has_value(x); }

  template < class C, enable_if_t<is_nullable<C>::value, int> = 0  >
  constexpr bool operator!=(none_t, C const& x) { return nullable::has_value(x); }
  template < class C, enable_if_t<is_nullable<C>::value, int> = 0  >
  bool operator!=(C const& x, none_t) { return nullable::has_value(x); }

  template < class C>
  constexpr bool operator==(none_t, C* x) { return ! nullable::has_value(x); }
  template < class C>
  constexpr bool operator==(C * x, none_t) { return ! nullable::has_value(x); }

  template < class C>
  constexpr bool operator!=(none_t, C * x) { return nullable::has_value(x); }
  template < class C>
  bool operator!=(C * x, none_t) { return nullable::has_value(x); }


  template < class C, enable_if_t<is_strict_weakly_ordered_nullable<C>::value, int> = 0  >
  constexpr bool operator<(none_t, C const& x) { return nullable::has_value(x); }
  template < class C, enable_if_t<is_strict_weakly_ordered_nullable<C>::value, int> = 0 >
  constexpr bool operator<(C const&, none_t) { return false; }

  template < class C, enable_if_t<is_strict_weakly_ordered_nullable<C>::value, int> = 0 >
  constexpr bool operator<=(none_t, C const&) { return true; }
  template < class C, enable_if_t<is_strict_weakly_ordered_nullable<C>::value, int> = 0 >
  constexpr bool operator<=(C const& x, none_t) { return ! nullable::has_value(x); }

  template < class C, enable_if_t<is_strict_weakly_ordered_nullable<C>::value, int> = 0 >
  constexpr bool operator>(none_t, C const&) { return false; }
  template < class C, enable_if_t<is_strict_weakly_ordered_nullable<C>::value, int> = 0 >
  constexpr bool operator>(C const& x, none_t) { return nullable::has_value(x); }

  template < class C, enable_if_t<is_strict_weakly_ordered_nullable<C>::value, int> = 0  >
  constexpr bool operator>=(none_t, C const& x) { return ! nullable::has_value(x); }
  template < class C, enable_if_t<is_strict_weakly_ordered_nullable<C>::value, int> = 0  >
  constexpr bool operator>=(C const&, none_t) { return true; }

  }
}
}
}

#endif // header
