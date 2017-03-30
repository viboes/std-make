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
#include <limits>
#include <type_traits>

// Open points:
// fixme: should this be bounded_ordinal as it is bounded
// fixme: should pos/val/succ/pred be renamed to whole words position/value/successor/predecessor
// fixme: should the position depend on ordinal type?
// fixme: should the position be a size_t or an int?
// fixme: should the position depend on the size?
// fixme: Shouldn't the size trait be part of ordinal::traits?

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
      using size = void;
      using size_type = size_t;
#if __cplusplus >= 201402L
      template <class U>
      static
      U val(size_type)  = delete;

      template <class U>
      static
      size_type pos(U && )  = delete;

#endif
    };

    // Trivial trait that makes use of explicit conversion
    // valid for all the signed integral builtins
    template <typename T>
    struct integral_traits
    {
      static_assert(is_integral<T>::value, "");
      using size_type = size_t;
      // todo uncomment when numeric_limits<T>::max() and numeric_limits<T>::min() will be constexpr
      //using size = integral_constant<size_type, numeric_limits<T>::max()-numeric_limits<T>::min()+1>;
      static T val(size_type p) { return T{p}; }

      static size_type pos(T u)  { return size_type{u}; };
    };
    template <>
    struct traits<long long> : integral_traits<long long> {};
    template <>
    struct traits<long> : integral_traits<long> {};
    template <>
    struct traits<int> : integral_traits<int> {};
    template <>
    struct traits<short> : integral_traits<short> {};
    template <>
    struct traits<char> : integral_traits<char> {};
    template <>
    struct traits<bool> : integral_traits<bool> {};

namespace meta {

  // forwards to traits<O>::size
  template <class O>
  struct size : traits<O>::size {};

  // this traits has a sense only for compile time ordinal types
  template <class O, O V>
  struct pos;

  // this traits has a sense only for compile time ordinal types
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
      //JASEL_EXPECTS(valid_position<Ord>(pos));
      return traits<Ord>::val(pos);
  }

  template <class Ord>
  constexpr decltype(auto) pos(Ord&& val) noexcept
  {
      //JASEL_EXPECTS(unique<Ord>());
      return traits<decay_t<Ord>>::pos(std::forward<Ord>(val));
  }

  template <class Ord>
  constexpr decltype(auto) first() noexcept {
    return ordinal::val<Ord>(0);
  }

  template <class Ord>
  constexpr decltype(auto) last() noexcept {
    return ordinal::val<Ord>(ordinal::meta::size<Ord>::value-1);
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

  // fixme check for the real operations size/val/pos
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

  // todo add is_ct_ordinal<CTO>

  // todo add specialization for integral types
}
}
}

#endif // header
