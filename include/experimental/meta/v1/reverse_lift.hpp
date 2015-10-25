//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_REVERSE_LIFT_HPP
#define JASEL_META_V1_REVERSE_LIFT_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  // transforms a template class into a type_constructor that adds the parameter at the begining

  template <template <class ...> class TC, class... Ts>
  struct reverse_lift
  {
    template <typename... Us>
    using apply = eval<defer<TC, Us..., Ts...>>;
  };
}
}
}
}

#endif // header
