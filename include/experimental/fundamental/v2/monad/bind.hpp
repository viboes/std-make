// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MONAD_BIND_HPP
#define JASEL_FUNDAMENTAL_V2_MONAD_BIND_HPP

#include <experimental/fundamental/v2/monad/monad_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>
#include <experimental/meta/v1/type_constructor.hpp>
#include <experimental/meta/v1/value_type.hpp>
#include <experimental/meta/v1/result_type.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class M, class F>
  //  requires Function<F, meta::ValueType<PV>>
  //  && Same<meta::TypeConstructor<meta::ResultType<F, meta::ValueType<PV>>>, meta::TypeConstructor<PV>>
  auto bind(const M& m, F&& f)
  //-> meta::ResultType<F, meta::ValueType<M>>
  -> decltype(bind_custom(concept_tag_t<monad, M>{}, m, std::forward<F>(f)))
  {
    return bind_custom(concept_tag_t<monad, M>{}, m, std::forward<F>(f));
  }

  template <class M, class F>
  //  requires Function<F, ValueType<PV>>
  //  && Same<meta::TypeConstructor<meta::ResultType<F, meta::ValueType<PV>>>, meta::TypeConstructor<PV>>
  auto bind(const M& m, F&& f)
  //-> meta::ResultType<F, meta::ValueType<M>>
  -> decltype(concept_instance_t<monad, M>::bind_impl(m, forward<F>(f)))
  {
    return concept_instance_t<monad, M>::bind_impl(m, forward<F>(f));
  }

}
}
}

#endif // header
