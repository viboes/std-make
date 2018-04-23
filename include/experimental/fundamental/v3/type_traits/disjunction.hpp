//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_DISJUNCTION_HPP
#define JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_DISJUNCTION_HPP

#include <experimental/fundamental/v3/type_traits/type_traits_t.hpp>
#include <experimental/fundamental/v2/config.hpp>

#if __cplusplus <= 201402L
namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  // disjunction
  template<class...> struct disjunction : false_type { };
  template<class B1> struct disjunction<B1> : B1 { };
  template<class B1, class... Bn>
  struct disjunction<B1, Bn...>
      : conditional_t<bool(B1::value), B1, disjunction<Bn...>>  { };

#if __cplusplus >= 201402L
  template <class ...Bs>
  JASEL_INLINE_VAR constexpr bool disjunction_v = disjunction<Bs...>::value;
#endif

}
}
using experimental::disjunction;

}
#endif
#endif // header
