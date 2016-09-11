// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MONAD_MBIND_HPP
#define JASEL_FUNDAMENTAL_V2_MONAD_MBIND_HPP

#include <experimental/fundamental/v2/monad/monad_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v2/value_type.hpp>
#include <experimental/fundamental/v2/result_type.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class M, class F>
  //  requires Function<F, ValueType<PV>>
  //  && Same<TypeConstructor<meta::ResultType<F, ValueType<PV>>>, TypeConstructor<PV>>
  auto mbind(const M& m, F&& f)
  //-> meta::ResultType<F, ValueType<M>>
  -> decltype(mbind_custom(concept_tag_t<monad, M>{}, m, std::forward<F>(f)))
  {
    return mbind_custom(concept_tag_t<monad, M>{}, m, std::forward<F>(f));
  }

  template <class M, class F>
  //  requires Function<F, ValueType<PV>>
  //  && Same<TypeConstructor<meta::ResultType<F, ValueType<PV>>>, TypeConstructor<PV>>
  auto mbind(const M& m, F&& f)
  //-> meta::ResultType<F, ValueType<M>>
  -> decltype(concept_instance_t<monad, M>::mbind_impl(m, forward<F>(f)))
  {
    return concept_instance_t<monad, M>::mbind_impl(m, forward<F>(f));
  }

}
}
}

#endif // header
