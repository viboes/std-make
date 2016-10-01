// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_NULLABLE_NONE_HPP
#define JASEL_FUNDAMENTAL_V2_NULLABLE_NONE_HPP

#include <experimental/type_constructible.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/config.hpp>

#include <utility>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  struct nullable_tag {};

  template <class T>
  struct nullable_traits  {};

  struct nullable_traits_pointer_like : nullable_tag
  {
    template <class U>
    struct none_type : meta::id<nullptr_t> { };

    static constexpr
    nullptr_t none() noexcept { return nullptr; }

    template <class U>
    static constexpr
    bool has_value(U const& ptr) noexcept { return ptr!=nullptr; }

    template <class U>
    static constexpr
    auto deref(U const& ptr)
      JASEL_DECLTYPE_RETURN (
          *ptr
      )
    template <class U>
    static constexpr
    auto deref(U & ptr)
      JASEL_DECLTYPE_RETURN (
          *ptr
      )
    template <class U>
    static constexpr
    auto deref(U const&& ptr)
      JASEL_DECLTYPE_RETURN (
          *move(ptr)
      )
    template <class U>
    static constexpr
    auto deref(U && ptr)
      JASEL_DECLTYPE_RETURN (
          *move(ptr)
      )
  };

  template <>
  struct nullable_traits<add_pointer<_t>> : nullable_traits_pointer_like {};

  namespace nullable {

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

  template <class T>
  struct none_type : nullable_traits<T>::template none_type<T> { };

  constexpr
  none_t none() noexcept { return none_t{}; }

  template <class TC>
  constexpr
  auto none()
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      nullable_traits<TC>::none()
    )

  template <template <class ...> class TC>
  constexpr
  auto none()
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
        //none<type_constructor_t<meta::quote<TC>>>()
        none< TC<_t> >()
    )

  template <class T>
  constexpr
  auto has_value(T const& x)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
      nullable_traits<T>::has_value(x)
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
      nullable_traits<typename decay<T>::type>::deref(x)
    )

  template <class T>
  constexpr
  T& deref(T* ptr) noexcept {
    return *ptr ;
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
  using nullable::none_type;
  using nullable::none;
  using nullable::has_value;
  using nullable::have_value;
  using nullable::deref;

  template <class T>
  struct is_nullable : is_base_of<nullable_tag, nullable_traits<T>> {};

  template <class T>
  struct is_nullable<T*> : true_type {};

  template <class T>
  struct is_strict_weakly_ordered_nullable : false_type {};

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
