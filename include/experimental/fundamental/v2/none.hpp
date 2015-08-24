// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_NONE_HPP
#define JASEL_FUNDAMENTAL_V2_NONE_HPP

#include <experimental/fundamental/v2/holder.hpp>
#include <experimental/meta/v1/type.hpp>
#include <experimental/meta/v1/type_constructor.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <template <class ...> class TC>
  constexpr auto none()
  {
    return none_custom(meta::type<meta::type_constructor_template_t<TC>>{});
  }

  template <class TC>
  constexpr auto none() {
    return none_custom(meta::type<TC>{});
  }

}
}
}

#endif // header
