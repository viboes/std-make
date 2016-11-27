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

#include <utility>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  namespace nullable {
    struct tag {};

    template <class T, class Enabler=void>
      struct traits : traits<T, meta::when<true>> {};

    // Default specialization
    template <typename T, bool condition>
    struct traits<T, meta::when<condition>>
    {
#if __cplusplus >= 201402L
      static
      auto none()  = delete;

      template <class U>
      static
      bool has_value(U const& ptr)  = delete;

      template <class U>
      static
      auto deref(U && ptr) = delete;

      template <class U>
      static
      std::nullptr_t deref_none(U &&) = delete;
#endif
    };

    struct traits_pointer_like : tag
    {
      static constexpr
      nullptr_t none() noexcept { return nullptr; }

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
      std::nullptr_t deref_none(U &&)
        { return nullptr; }

    };

    template <>
    struct traits<add_pointer<_t>> : traits_pointer_like {};
    template <class T>
    struct traits<T*> : traits_pointer_like {};


  struct none_t {
    explicit none_t() = default;
    template <class T>
    operator T*() const noexcept { return nullptr; }
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
  using none_type_t = decltype(nullable::none<TC>());

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
  auto deref(T&& x)
    JASEL_DECLTYPE_RETURN (
      traits<typename decay<T>::type>::deref(x)
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
    struct value_type { using type = typename remove_reference<decltype(nullable::deref(declval<T>()))>::type; };

  template <class T>
  constexpr
  auto deref_none(T&& x)
    JASEL_DECLTYPE_RETURN (
      traits<typename decay<T>::type>::deref_none(x)
    )

  template <class T>
  constexpr
  std::nullptr_t deref_none(T* ptr) noexcept {
    return nullptr ;
  }

  template <class M>
  auto have_value(M const& v)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      nullable::has_value(v)
    )

  template <class M1, class M2, class ...Ms>
  auto have_value(M1 const& v1, M2 const& v2, Ms const& ...vs)
    //-> decltype(has_value(v1) && have_value(v2, vs...))
    noexcept(noexcept(nullable::has_value(v1)))
    -> decltype(nullable::has_value(v1))

  {
    return nullable::has_value(v1) && have_value(v2, vs...);
    //return nullable::has_value(v1) &&
    //    nullable::has_value(v2) && (nullable::has_value(vs) && ...);
  }

}

  using nullable::none_t;
  using nullable::none_type_t;
  using nullable::none;
  using nullable::has_value;
  using nullable::have_value;
  using nullable::deref;
  using nullable::deref_none;

  // todo: define in function of whether
  // EqualityComparable && DefaultConstructible && Destructible
  // nullable::none<T>()
  // nullable::has_value(t)
  // T{none_t}
  // T{none<T>()}

  template <class T>
  struct is_nullable : is_base_of<nullable::tag, nullable::traits<T>> {};
  template <class T>
  struct is_nullable<const T> : is_nullable<T> {};
  template <class T>
  struct is_nullable<volatile T> : is_nullable<T> {};
  template <class T>
  struct is_nullable<const volatile T> : is_nullable<T> {};

#if __cplusplus >= 201402L
  template <class T>
  constexpr bool is_nullable_v = is_nullable<T>::value ;
#endif

  template <class T>
  struct is_nullable<T*> : true_type {};

  // todo: implement this traits
  template <class T>
  struct is_strict_weakly_ordered : false_type {};

  template <class T>
  struct is_strict_weakly_ordered_nullable : conjunction<
    is_strict_weakly_ordered<T>
  , is_nullable<T>
  > {};

  namespace nullable {

  template < class C, typename enable_if<is_nullable<C>::value, int>::type=0 >
  constexpr bool operator==(none_t, C const& x) { return ! nullable::has_value(x); }
  template < class C, typename enable_if<is_nullable<C>::value, int>::type=0  >
  constexpr bool operator==(C const& x, none_t) { return ! nullable::has_value(x); }

  template < class C, typename enable_if<is_nullable<C>::value, int>::type=0  >
  constexpr bool operator!=(none_t, C const& x) { return nullable::has_value(x); }
  template < class C, typename enable_if<is_nullable<C>::value, int>::type=0  >
  bool operator!=(C const& x, none_t) { return nullable::has_value(x); }

  template < class C>
  constexpr bool operator==(none_t, C* x) { return ! nullable::has_value(x); }
  template < class C>
  constexpr bool operator==(C * x, none_t) { return ! nullable::has_value(x); }

  template < class C>
  constexpr bool operator!=(none_t, C * x) { return nullable::has_value(x); }
  template < class C>
  bool operator!=(C * x, none_t) { return nullable::has_value(x); }


  template < class C, typename enable_if<is_strict_weakly_ordered_nullable<C>::value, int>::type=0  >
  constexpr bool operator<(none_t, C const& x) { return nullable::has_value(x); }
  template < class C, typename enable_if<is_strict_weakly_ordered_nullable<C>::value, int>::type=0 >
  constexpr bool operator<(C const& x, none_t) { return false; }

  template < class C, typename enable_if<is_strict_weakly_ordered_nullable<C>::value, int>::type=0 >
  constexpr bool operator<=(none_t, C const& x) { return true; }
  template < class C, typename enable_if<is_strict_weakly_ordered_nullable<C>::value, int>::type=0 >
  constexpr bool operator<=(C const& x, none_t) { return ! nullable::has_value(x); }

  template < class C, typename enable_if<is_strict_weakly_ordered_nullable<C>::value, int>::type=0 >
  constexpr bool operator>(none_t, C const& x) { return false; }
  template < class C, typename enable_if<is_strict_weakly_ordered_nullable<C>::value, int>::type=0 >
  constexpr bool operator>(C const& x, none_t) { return nullable::has_value(x); }

  template < class C, typename enable_if<is_strict_weakly_ordered_nullable<C>::value, int>::type=0  >
  constexpr bool operator>=(none_t, C const& x) { return ! nullable::has_value(x); }
  template < class C, typename enable_if<is_strict_weakly_ordered_nullable<C>::value, int>::type=0  >
  constexpr bool operator>=(C const& x, none_t) { return true; }
  }

}
}
}

#endif // header
