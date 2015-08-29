// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MONOID_OP_HPP
#define JASEL_FUNDAMENTAL_V2_MONOID_OP_HPP

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
  auto monoid_op(M const& x, M const& y)
    -> decltype(monoid_op_custom(concept_tag_t<monoid, M>{}, x, y))
  {
    return monoid_op_custom(concept_tag_t<monoid, M>{}, x, y);
  }

  template <class M>
  auto monoid_op(M const& x, M const& y)
    -> decltype(concept_instance_t<monoid, M>::monoid_op_impl(x, y))
  {
    return concept_instance_t<monoid, M>::monoid_op_impl(x, y);
  }

  template <class M, class ...Ms>
  // requires Same<M, MS>...
  auto monoid_op(M const& x1, M const& x2, M const& x3, Ms const& ...y)
    //-> decltype(monoid_op(monoid_op(x1, x2), x3, y...))
    -> decltype(monoid_op(x1, x2))
  {
    return monoid_op(monoid_op(x1, x2), x3, y...);
  }

}
}
}

#endif // header
