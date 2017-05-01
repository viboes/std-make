// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/out_param.hpp
// See https://github.com/akrzemi1/explicit/blob/master/doc/out_param.md
// Copyright (C) 2016 Andrzej Krzemienski.
// (C) Copyright 2017 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_UTILITY_OUT_PARAM_HPP
#define JASEL_EXPERIMENTAL_UTILITY_OUT_PARAM_HPP

#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <typename T>
  class out_param
  {
    static_assert(is_lvalue_reference<T>::value, "only references to non-const");
    static_assert(!is_const<remove_reference_t<T>>::value, "only references to non-const");
    T& _ref;
    // no assignable by default as it contains a reference
  public:
    explicit out_param(T& ref) : _ref(ref) {}
    // assign and get access to the reference
    T& operator=(T value) // NOLINT cppcoreguidelines-c-copy-assignment-signature
    {
      _ref = move(value);
      return ref;
    }
    T& get()
    {
      return _ref;
    }

  };

  template <typename T>
  out_param<T&> out(T& ref)
  {
    static_assert(!is_const<T>::value, "only references to non-const");
    return out_param<T&>(ref);
}

}}}

#endif // header
