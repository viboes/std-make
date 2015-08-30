// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_NOVALUE_HPP
#define JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_NOVALUE_HPP

#include <experimental/fundamental/v2/possible_valued/possible_value_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>
#include <experimental/fundamental/v2/none.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class M>
  auto novalue(M const& m) ->
    decltype(novalue_custom(concept_tag_t<possible_value, M>{}, m))
  {
    return novalue_custom(concept_tag_t<possible_value, M>{}, m);
  }

  template <class M>
  auto novalue(M const& m) ->
    decltype(concept_instance_t<possible_value, M>::novalue_impl(m))
  {
    return concept_instance_t<possible_value, M>::novalue_impl(m);
  }

  // this overload a little bit intrusive. It must be mappedfrom the customization.
  // it is used by the pointer-like mappings.

  template <class M>
  auto novalue(M const& ptr)
  -> decltype(none<meta::type_constructor_t<M>>())
  {
    return none<meta::type_constructor_t<M>>();
  }

  template <class T>
  nullptr_t novalue(const T*) {
    return nullptr;
  }
}
}
}

#endif // header
