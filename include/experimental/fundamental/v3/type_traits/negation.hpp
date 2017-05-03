//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_NEGATION_HPP
#define JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_NEGATION_HPP

#include <type_traits>

#if __cplusplus <= 201402L
namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  // negation
  template<class B>
  struct negation : std::integral_constant<bool, !B::value> { };

#if __cplusplus >= 201402L
  template<class B>
  //inline C++17
  constexpr bool negation_v = negation<B>::value;
#endif

}
}
using experimental::negation;

}
#endif
#endif // header
