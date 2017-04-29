//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_TYPE_TRAITS_V_HPP
#define JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_TYPE_TRAITS_V_HPP

#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

#if __cplusplus >= 201402L
  template <class R1, class R2>
  //inline C++17
  constexpr bool is_constructible_v = is_constructible<R1,R2>::value;

  template <class R1, class R2>
  //inline C++17
  constexpr bool is_convertible_v = is_convertible<R1,R2>::value;

#endif

}
}
}
#endif // header
