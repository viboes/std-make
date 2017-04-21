//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_CONJUNCTION_HPP
#define JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_CONJUNCTION_HPP

#include <experimental/fundamental/v3/type_traits/type_traits_t.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  // conjunction
  template<class...> struct conjunction;
  template<> struct conjunction<> : true_type {};

  template<class B0> struct conjunction<B0> : B0 {};

  template<class B0, class B1>
  struct conjunction<B0, B1> : conditional_t<B0::value, B1, B0> {};

  template<class B0, class B1, class B2, class... Bs>
  struct conjunction<B0, B1, B2, Bs...>
          : conditional_t<B0::value, conjunction<B1, B2, Bs...>, B0> {};

#if __cplusplus >= 201402L
  template <class ...Bs>
  //inline C++17
  constexpr bool conjunction_v = conjunction<Bs...>::value;
#endif
}
}
}
#endif // header
