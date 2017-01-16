// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_HPP
#define JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_HPP

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

namespace ordinal {
    struct tag {};

    template <class T, class Enabler=void>
      struct traits : traits<T, meta::when<true>> {};

    // Default specialization
    template <typename T, bool condition>
    struct traits<T, meta::when<condition>>
    {
      using size_type = size_t;
#if __cplusplus >= 201402L
      template <class U>
      static
      U val(size_type)  = delete;

      template <class U>
      static
      bool pos(U && )  = delete;

#endif
    };

namespace meta {

  template <class O>
  struct size;

  template <class O, O V>
  struct pos;

  template <class O, size_t I>
  struct val;

  template <class O>
  struct first : meta::val<O, 0> {};

  template <class O>
  struct last : meta::val<O, meta::size<O>::val-1> {};

  template <class O, O V>
  struct succ : meta::val<O, meta::pos<O,V>::value+1> {};

  template <class O, O V>
  struct pred : meta::val<O, meta::pos<O,V>::value-1> {};

}

  template <class O>
  constexpr
  decltype(auto) size() noexcept { return meta::size<O>::value; }

  template <class Ord>
  constexpr decltype(auto) val(size_t pos) noexcept
  {
      return traits<Ord>::template val<Ord>(pos);
  }

  template <class Ord>
  constexpr decltype(auto) pos(Ord&& val) noexcept
  {
      return traits<decay_t<Ord>>::pos(std::forward<Ord>(val));
  }

  template <class Ord>
  constexpr decltype(auto) first() noexcept {
    return ordinal::val<Ord>(0);
    //return ordinal::meta::first<Ord>::value ;
  }

  template <class Ord>
  constexpr decltype(auto) last() noexcept {
    return ordinal::val<Ord>(ordinal::meta::size<Ord>::value-1);
    //return ordinal::meta::last<Ord>::value ;
  }

  template <class Ord>
  constexpr decltype(auto) succ(Ord&& val) noexcept {
    return ordinal::val<Ord>(ordinal::pos(val)+1) ;
  }
  template <class Ord>
  constexpr decltype(auto) pred(Ord&& val) noexcept {
    return ordinal::val<Ord>(ordinal::pos(val)-1) ;
  }

}

  template <class T>
  struct is_ordinal : is_base_of<ordinal::tag, ordinal::traits<T>> {};
  template <class T>
  struct is_ordinal<const T> : is_ordinal<T> {};
  template <class T>
  struct is_ordinal<volatile T> : is_ordinal<T> {};
  template <class T>
  struct is_ordinal<const volatile T> : is_ordinal<T> {};

#if __cplusplus >= 201402L
  template <class T>
  constexpr bool is_ordinal_v = is_ordinal<T>::value ;
#endif

}
}
}

#endif // header
