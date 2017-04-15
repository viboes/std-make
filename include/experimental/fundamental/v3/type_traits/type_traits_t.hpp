//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_TYPE_TRAITS_T__HPP
#define JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_TYPE_TRAITS_T__HPP

#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

#if __cplusplus == 201103L

  template< bool B, class T, class F >
  using conditional_t = typename conditional<B,T,F>::type;

  template< class T >
  using decay_t = typename decay<T>::type;

  template< bool B, class T = void >
  using enable_if_t = typename enable_if<B,T>::type;

#endif

}
}
}
#endif // header
