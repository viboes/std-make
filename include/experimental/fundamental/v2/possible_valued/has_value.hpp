// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_HAS_VALUE_HPP
#define JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_HAS_VALUE_HPP

#include <experimental/fundamental/v2/possible_valued/possible_value_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class M>
  auto has_value(M const& v) ->
    decltype(has_value_custom(concept_tag_t<possible_value, M>{}, v))
  {
    return has_value_custom(concept_tag_t<possible_value, M>{}, v);
  }

  template <class M>
  auto has_value(M const& v) ->
    decltype(concept_instance_t<possible_value, M>::has_value_impl(v))
  {
    return concept_instance_t<possible_value, M>::has_value_impl(v);
  }

  template <class M>
  auto have_value(M const& v)
    -> decltype(has_value(v))
  {
    return has_value(v) ;
  }
  template <class M1, class M2, class ...Ms>
  auto have_value(M1 const& v1, M2 const& v2, Ms const& ...vs)
    //-> decltype(has_value(v1) && have_value(v2, vs...))
    -> decltype(has_value(v1))
  {
    return has_value(v1) && have_value(v2, vs...);
  }

  template <class T>
  bool has_value(T* ptr) {
    return ptr != nullptr;
  }
}
}
}

#endif // header
