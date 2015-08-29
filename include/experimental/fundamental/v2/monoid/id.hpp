// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MONOID_ID_HPP
#define JASEL_FUNDAMENTAL_V2_MONOID_ID_HPP

#include <experimental/fundamental/v2/monoid/monoid_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>
#include <experimental/meta/v1/type.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class M>
  auto monoid_id()
    -> decltype(monoid_id_custom(concept_tag_t<monoid, M>{}, meta::type<M>{}))
  {
    return monoid_id_custom(concept_tag_t<monoid, M>{}, meta::type<M>{});
  }

  template <class M>
  auto monoid_id()
    -> decltype(concept_instance_t<monoid, M>::monoid_id_impl(meta::type<M>{}))
  {
    return concept_instance_t<monoid, M>::monoid_id_impl(meta::type<M>{});
  }


}
}
}

#endif // header
