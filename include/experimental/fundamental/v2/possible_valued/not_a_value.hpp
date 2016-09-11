// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_NOT_A_VALUE_HPP
#define JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_NOT_A_VALUE_HPP

#include <experimental/fundamental/v2/possible_valued/possible_value_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>
#include <experimental/nullable.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  /**
   * @Requires ! has_value(m)
   */
  template <class M>
  auto not_a_value(M const& m) ->
    decltype(not_a_value_custom(concept_tag_t<possible_value, M>{}, m))
  {
    return not_a_value_custom(concept_tag_t<possible_value, M>{}, m);
  }

  template <class M>
  auto not_a_value(M const& m) ->
    decltype(concept_instance_t<possible_value, M>::not_a_value_impl(m))
  {
    return concept_instance_t<possible_value, M>::not_a_value_impl(m);
  }

  template <class T>
  nullptr_t not_a_value(const T*) {
    return nullptr;
  }
}
}
}

#endif // header
