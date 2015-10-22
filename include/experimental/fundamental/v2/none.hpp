// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_NONE_HPP
#define JASEL_FUNDAMENTAL_V2_NONE_HPP

#include <experimental/fundamental/v2/holder.hpp>
#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/type_constructor.hpp>
#include <experimental/meta/v1/none_type.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
#if 1
  template <class TC>
  constexpr auto none()
    -> decltype(none_custom(meta::id<TC>{}))
  {
    return none_custom(meta::id<TC>{});
  }

  template <template <class ...> class TC>
  constexpr auto none()
  ->  decltype(none<meta::type_constructor_t<meta::template_class<TC>>>())
  {
    return none<meta::type_constructor_t<meta::template_class<TC>>>();
  }
#else
  // the following doesn't works as nullopt_t{}
  // 5.5 p3 Type nullopt_t shall not have a default constructor. It shall be a literal type. Constant nullopt shall be initialized with an argument of literal type.
  template <class TC>
  constexpr meta::NoneType<TC> none()
  {
    return meta::NoneType<TC>();
  }

  template <template <class ...> class TC>
  constexpr auto none()
  {
    return meta::NoneType<meta::TypeConstructor<meta::template_class<TC>>>();
  }

#endif
}
namespace meta
{
inline namespace v1
{
  nullptr_t none_custom(id<add_pointer<_t>>) { return nullptr; }
}
}

}
}

#endif // header
