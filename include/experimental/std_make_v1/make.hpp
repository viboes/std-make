// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2014 Vicente J. Botet Escriba

#ifndef VIBOES_STD_EXPERIMENTAL_STD_MAKE_MAKE_HPP
#define VIBOES_STD_EXPERIMENTAL_STD_MAKE_MAKE_HPP

#include <utility>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
  template <template <class ...> class M>
  struct type_constructor {
    template <class T>
    using rebind = M<T>;
  };

  template <class T>
  struct type {
  };

  struct in_place_t {};
  constexpr in_place_t in_place = {};

  struct _t {};

  template <template <class ...> class M, class X>
  auto make(X&& x) -> decltype( make(type<M<typename std::decay<X>::type>>{}, std::forward<X>(x)) )
  {
    return make(type<M<typename std::decay<X>::type>>{}, std::forward<X>(x));
  }

  template <class M, class X>
  auto make(X&& x) -> decltype(make(type<M>{}, std::forward<X>(x)))
  {
    return make(type<M>{}, std::forward<X>(x));
  }

  template <class M, class ...Args>
  auto emplace(Args&& ...args) -> decltype(emplace(type<M>{}, std::forward<Args>(args)...))
  {
    return emplace(type<M>{}, std::forward<Args>(args)...);
  }

  // default customization point for constructor from X
  template <class M, class X>
  M make(type<M>, X&& x)
  {
    return M(std::forward<X>(x));
  }

  // default customization point for in_place constructor
  template <class M, class ...Args>
  M emplace(type<M>, Args&& ...args)
  {
    return M(in_place, std::forward<Args>(args)...);
  }

}
}
}

#endif // VIBOES_STD_EXPERIMENTAL_STD_MAKE_MAKE_HPP
