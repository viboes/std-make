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

  // type wrapper
  template <class T=void>
  struct type {};

  // holder type
  struct _t {};

  // type constructor tag type
  struct type_constructor_t {};

  // lift a template class to a type_constructor
  template <template <class ...> class TC, class... Args>
  struct lift : type_constructor_t
  {
    template<class... Args2>
    using type = TC<Args..., Args2...>;
  };

  // reverse lift a template class to a type_constructor
  template <template <class ...> class TC, class... Args>
  struct reverse_lift : type_constructor_t
  {
    template<class... Args2>
    using type = TC<Args2..., Args...>;
  };

  // apply a type constuctor TC to the type parameters Args
  template<class TC, class... Args>
  using apply = typename TC::template type<Args...>;

  // identity meta-function
  template<class T>
  struct identity
  {
    typedef T type;
  };

  // type constructor customization point. Default implementation make use of nested type type_constructor
  template <class M >
  struct type_constructor_impl : identity<typename  M::type_constructor> {};

  // type constructor meta-function
  template <class M >
  using type_constructor = typename type_constructor_impl<M>::type;

  // rebinds a type havind a underlying type with another underlying type
  template <class M, class U>
  using rebind = apply<type_constructor<M>, U>;

  // type trait based on inheritance from type_constructor_t
  template <class TC >
  struct is_type_constructor : is_base_of<type_constructor_t, TC> {};

  // make() overload
  template <template <class ...> class M>
  M<void> make()
  {
    return make(type<M<void>>{});
  }

  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, int = 0, int..., class X>
  M<typename std::decay<X>::type>
  make(X&& x)
  {
    return make(type<M<typename std::decay<X>::type>>{}, std::forward<X>(x));
  }

  // make overload: requires a type construcor, deduce the underlying type
  template <class TC, int = 0, int..., class X>
  typename enable_if<is_type_constructor<TC>::value,
    apply<TC, typename std::decay<X>::type>
  >::type
  make(X&& x)
  {
    return make(type<apply<TC, typename std::decay<X>::type>>{}, std::forward<X>(x));
  }

  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from X
  template <class M, int = 0, int..., class X>
  typename enable_if<! is_type_constructor<M>::value,
    M
  >::type
  make(X&& x)
  {
    return make(type<M>{}, std::forward<X>(x));
  }

  // make emplace overload: requires a type with a specific underlying type, don't deduce the underlying type from X
  template <class M, class ...Args>
  M make(Args&& ...args)
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
