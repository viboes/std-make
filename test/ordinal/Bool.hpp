// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef JASEL_EXPERIMENTAL_TEST_ORDINAL_BOOL_HPP
#define JASEL_EXPERIMENTAL_TEST_ORDINAL_BOOL_HPP

#include <experimental/ordinal.hpp>

enum class Bool : bool { False, True };

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  namespace ordinal {
    namespace meta {
      template <>
      struct size<Bool> : integral_constant<size_t, 2> {};
      template <>
      struct pos<Bool, Bool::False> : integral_constant<size_t, 0> {};
      template <>
      struct pos<Bool, Bool::True> : integral_constant<size_t, 1> {};
      template <>
      struct val<Bool, 0> : integral_constant<Bool, Bool::False> {};
      template <>
      struct val<Bool, 1> : integral_constant<Bool, Bool::True> {};
    }
    template <>
    struct traits<Bool>
    {
      using size_type = size_t;
      template <class U>
      static constexpr
      Bool val(size_type pos)  {
        switch (pos) {
        case 0: return Bool::False;
        default:  return Bool::True;
        }
      }

      template <class U>
      static constexpr
      size_t pos(U && val)    {
        switch (val) {
        case Bool::False: return 0;
        default:  return 1;
        }
      }
    };
  }
}}
}

#endif
