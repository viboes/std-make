// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/lvalue_ref.hpp
// See https://github.com/akrzemi1/explicit/blob/master/doc/lvalue_ref.md
// Copyright (C) 2016 Andrzej Krzemienski.
// (C) Copyright 2017 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_UTILITY_LVALUE_REF_HPP
#define JASEL_EXPERIMENTAL_UTILITY_LVALUE_REF_HPP

#include <type_traits>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  namespace detail
  {
  template <typename T, typename U>
  struct is_lvalue_ref_or_wrapper : conditional<
    is_convertible<U&&, T&>::value && !is_convertible<U&&, T&&>::value,
    true_type,
    false_type
  >::type {};
  }

  template <typename T>
  class lvalue_ref
  {
    static_assert(!is_reference<T>::value, "lvalue_ref<T> can only be instantiated with a non-reference type");
    T& _ref;

  public:

    template <typename U, typename enable_if<detail::is_lvalue_ref_or_wrapper<T, U>::value, bool>::type = true>
      lvalue_ref(U&& ref) : _ref(forward<U>(ref)) {}

    template <typename U, typename enable_if<!detail::is_lvalue_ref_or_wrapper<T, U>::value, bool>::type = true>
      lvalue_ref(U&& ref) = delete;

    T& get() const { return _ref; }
    operator T& () const { return _ref; }
  };


}}}

#endif // header
