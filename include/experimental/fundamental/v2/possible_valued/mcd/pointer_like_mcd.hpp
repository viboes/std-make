//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_POSSIBLE_VALUED_MCD_POINTER_LIKE_HPP
#define JASEL_FUNDAMENTAL_V2_POSSIBLE_VALUED_MCD_POINTER_LIKE_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// This type that models the PointerLike concept can be seen as a model of a PossibleValued
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v2/possible_valued.hpp>
#include <experimental/fundamental/v2/pointer_like.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class M>
  bool has_value_custom(pointer_like, M const& v)
  {
    return bool(v);
  }

  template <class M>
  meta::value_type_t<M>&
  value_custom(pointer_like, M& ptr)
  {
    return *ptr;
  }
  template <class M>
  meta::value_type_t<M> const&
  value_custom(pointer_like, M const& ptr)
  {
    return *ptr;
  }

  template <class M>
  auto novalue(M const& ptr)
  {
    return none<meta::type_constructor_t<M>>();
  }

}
}
}


#endif // header

