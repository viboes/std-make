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

#include <experimental/type_traits.hpp>
#include <utility>

namespace std
{
  namespace experimental
  {
  inline namespace fundamental_v3
  {
#if ! defined JASEL_DOXYGEN_INVOKED
    namespace detail
    {
      // todo: move to detail

      template <typename T, typename U>
      struct _is_lvalue_ref_or_wrapper : conditional_t<
          is_convertible<U&&, T&>::value && !is_convertible<U&&, T&&>::value,
          true_type,
          false_type
      >
      {};
    }
#endif

    template <typename T>
    class lvalue_ref
    {
      static_assert(!is_reference<T>::value, "lvalue_ref<T> can only be instantiated with a non-reference type");
      T& _ref;

    public:

      template <typename U, enable_if_t<detail::_is_lvalue_ref_or_wrapper<T, U>::value, bool> = true>
      lvalue_ref(U&& ref) // NOLINT google-explicit-constructor
      : _ref(forward<U>(ref))
      {}

      template <typename U, enable_if_t<!detail::_is_lvalue_ref_or_wrapper<T, U>::value, bool> = true>
      lvalue_ref(U&& ref) = delete;

      T& get() const
      { return _ref;}
      operator T& () const // NOLINT google-explicit-constructor
      { return _ref;}
    };

  }}
}

#endif // header
