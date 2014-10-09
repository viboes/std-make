// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2014 Vicente J. Botet Escriba

#ifndef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_HPP
#define VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_HPP

#include <utility>
#include <type_traits>

#if defined VIBOES_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
#include <optional.hpp>
#endif

namespace std
{
namespace experimental
{
#if ! defined VIBOES_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
  constexpr struct in_place_t{} in_place{};
#endif

inline namespace fundamental_v2
{

  template <class T=void>
  struct type {};

  struct _t {};

  struct type_constructor_t {};

  template <template <class ...> class TC, class... Args>
  struct lift : type_constructor_t
  {
    template<class... Args2>
    using type = TC<Args..., Args2...>;
  };

  template <template <class ...> class TC, class... Args>
  struct reverse_lift : type_constructor_t
  {
    template<class... Args2>
    using type = TC<Args2..., Args...>;
  };

  template<class TC, class... Args>
  using apply = typename TC::template type<Args...>;

  template<class T>
  struct identity
  {
    typedef T type;
  };


  template <class M > // customization point
  struct type_constructor_impl : identity<typename  M::type_constructor> {};

  template <class M >
  using type_constructor = typename type_constructor_impl<M>::type;

  template <class M, class U>
  using rebind = apply<type_constructor<M>, U>;

  template <class TC >
  struct is_type_constructor : is_base_of<type_constructor_t, TC> {};

  template <template <class ...> class M>
  auto make() -> decltype( make(type<M<void>>{}) )
  {
    return make(type<M<void>>{});
  }

  template <template <class ...> class M, int = 0, int..., class X>
  M<typename std::decay<X>::type>
  make(X&& x)
  {
    return make(type<M<typename std::decay<X>::type>>{}, std::forward<X>(x));
  }

  template <class TC, int = 0, int..., class X>
  typename enable_if<is_type_constructor<TC>::value,
    apply<TC, typename std::decay<X>::type>
  >::type
  make(X&& x)
  {
    return make(type<apply<TC, typename std::decay<X>::type>>{}, std::forward<X>(x));
  }

  template <class M, int = 0, int..., class X>
  typename enable_if<! is_type_constructor<M>::value,
    M
  >::type
  make(X&& x)
  {
    return make(type<M>{}, std::forward<X>(x));
  }

  template <class M, class ...Args>
  M
  make(Args&& ...args)
  {
    return make(type<M>{}, in_place, std::forward<Args>(args)...);
  }

  // default customization point for TC<void> default constructor
  template <class M>
  M make(type<M>)
  {
    return M();
  }

  // default customization point for constructor from X
  template <class M, class X>
  M make(type<M>, X&& x)
  {
    return M(std::forward<X>(x));
  }

  // default customization point for in_place constructor
  template <class M, class ...Args>
  M make(type<M>, in_place_t, Args&& ...args)
  {
    return M(in_place, std::forward<Args>(args)...);
  }

}
}
}

#endif // VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_HPP
