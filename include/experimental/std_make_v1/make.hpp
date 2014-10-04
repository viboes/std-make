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
  struct tc {};

  template <class T>
  struct c {};

  struct in_place_t {};
  constexpr in_place_t in_place = {};

  struct _t {};

  template <template <class ...> class M, class X>
  auto make(X&& x) -> decltype(make(tc<M>{}, std::forward<X>(x)))
  {
    return make(tc<M>{}, std::forward<X>(x));
  }

  template <class M, class ...Args>
  auto make(Args&& ...args) -> decltype(make(c<M>{}, std::forward<Args>(args)...))
  {
    return make(c<M>{}, std::forward<Args>(args)...);
  }

  // default customization point
  template <template <class ...> class M, class X>
  M<typename std::decay<X>::type> make(tc<M>, X&& x)
  {
    return M<typename std::decay<X>::type>(in_place, std::forward<typename std::decay<X>::type>(x));
  }

  // default customization point
  template <class M, class ...Args>
  M make(c<M>, Args&& ...args)
  {
    return M(in_place, std::forward<Args>(args)...);
  }

}
}
}

#endif // VIBOES_STD_EXPERIMENTAL_STD_MAKE_MAKE_HPP
