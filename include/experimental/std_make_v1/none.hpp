// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_NONE_HPP
#define VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_NONE_HPP

#include <utility>
#include <type_traits>
#include <experimental/fundamental/v1/in_place.hpp>
#include <experimental/fundamental/v2/holder.hpp>
#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/types.hpp>
#include <experimental/meta/v1/type.hpp>
#include <experimental/meta/v1/template_class.hpp>
#include <experimental/meta/v1/deduced_type.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/meta/v1/always.hpp>
#include <experimental/meta/v1/void_.hpp>
#include <experimental/meta/v1/is_applicable_with.hpp>
#include <experimental/meta/v1/rebind.hpp>
#include <experimental/meta/v1/quote.hpp>
#include <experimental/meta/v1/lift.hpp>
#include <experimental/meta/v1/reverse_lift.hpp>
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
#ifdef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR
    return none_custom(meta::type<type_constructor_template_t<TC>>{});
#else
    return none_custom(meta::template_class<TC>{});
#endif
  }

  template <class TC>
  constexpr auto none() {
    return none_custom(meta::type<TC>{});
  }

}
}
}

#endif // VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_HPP
