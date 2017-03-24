//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_BIND_FRONT_HPP
#define JASEL_META_V1_BIND_FRONT_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/meta/v1/defer.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  // transforms a class template into a type_constructor that adds the parameter at the end

  template <template <class ...> class TC, class... Ts>
  struct bind_front
  {
    template <typename... Us>
    using invoke = eval<defer<TC, Ts..., Us...>>;
  };

}
}
}
}

#endif // header
