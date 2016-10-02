// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_FUNCTOR_AP_HPP
#define JASEL_FUNDAMENTAL_V2_FUNCTOR_AP_HPP

#include <experimental/fundamental/v2/applicative/applicative_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/meta/v1/invoke.hpp>
#include <experimental/fundamental/v3/value_type.hpp>
#include <experimental/meta/v1/result_type.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class F, class M>
  //  requires Function<ValueType<F>, ValueType<M>>
  //  && Same<TypeConstructor<F>, TypeConstructor<M>>
  auto ap(F const& f, const M& m)
    //-> meta::invoke<TypeConstructor<M>, meta::ResultType<ValueType<F>, ValueType<M>>>
    -> decltype(ap_custom(concept_tag_t<applicative, M>{}, f, m))
  {
    return ap_custom(concept_tag_t<applicative, M>{}, f, m);
  }

  template <class F, class M>
  //  requires Function<ValueType<F>, ValueType<M>>
  //  && Same<TypeConstructor<F>, TypeConstructor<M>>
  auto ap(F const& f, const M& m)
    //-> meta::invoke<TypeConstructor<M>, meta::ResultType<ValueType<F>, ValueType<M>>>
    -> decltype(concept_instance_t<applicative, M>::ap_impl(f, m))
  {
    return concept_instance_t<applicative, M>::ap_impl(f, m);
  }

}
}
}

#endif // header
