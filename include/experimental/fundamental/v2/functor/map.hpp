// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_FUNCTOR_MAP_HPP
#define JASEL_FUNDAMENTAL_V2_FUNCTOR_MAP_HPP

#include <experimental/fundamental/v2/functor/functor_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>
#include <experimental/meta/v1/apply.hpp>
#include <experimental/meta/v1/type_constructor.hpp>
#include <experimental/meta/v1/value_type.hpp>
#include <experimental/meta/v1/result_type.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class M, class F>
  //  requires Function<F, ValueType<PV>>
  auto map(const M& m, F&& f)
    //-> meta::apply<meta::TypeConstructor<M>, meta::ResultType<F, meta::ValueType<PV>>>
    -> decltype(map_custom(concept_tag_t<functor, M>{}, m, forward<F>(f)))
  {
    return map_custom(concept_tag_t<functor, M>{}, m, forward<F>(f));
  }

  template <class M, class F>
  //  requires Function<F, ValueType<PV>>
  auto map(const M& m, F&& f)
    // -> meta::apply<meta::TypeConstructor<M>, meta::ResultType<F, meta::ValueType<PV>>>
    -> decltype(concept_instance_t<functor, M>::map_impl(m, forward<F>(f)))
  {
    return concept_instance_t<functor, M>::map_impl(m, forward<F>(f));
  }

}
}
}

#endif // header
