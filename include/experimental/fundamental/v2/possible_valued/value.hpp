// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_VALUE_HPP
#define JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_VALUE_HPP

#include <experimental/fundamental/v2/possible_valued/possible_value_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  /**
   * @Requires has_value(m)
   */

  // define value for pointers
  template <class T>
  T& value(T* ptr)
  {
    return *ptr;
  }
  template <class T>
  T const& value(T const * ptr)
  {
    return *ptr;
  }

  // define value for explicit models of possible_value customized by overload
  template <class M>
  auto value(M const& v) ->
    decltype(value_custom(concept_tag_t<possible_value, M>{}, v))
  {
    return value_custom(concept_tag_t<possible_value, M>{}, v);
  }

  // define value for explicit models of possible_value customized by instance
  template <class M>
  auto value(M const& v) ->
    decltype(concept_instance_t<possible_value, M>::value(v))
  {
    return concept_instance_t<possible_value, M>::value(v);
  }

}
}
}

#endif // header
