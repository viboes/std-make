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
#if __cplusplus <= 201103L
  template <class T>
  using decay_t = typename decay<T>::type;

  template <class T>
  using remove_reference_t = typename remove_reference<T>::type;

#endif

namespace experimental
{
#if ! defined VIBOES_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
  constexpr struct in_place_t{} in_place{};
#endif

inline namespace fundamental_v2
{

  // identity meta-function
  template<class T>
  struct identity
  {
    typedef T type;
  };

  // tag type
  template <class T>
  struct type {
    using underlying_type = T;
  };

  // holder type
  struct _t {};

  // type constructor tag type used to check if a class is a type constructor
  struct type_constructor_tag {};

  // type trait based on inheritance from type_constructor_tag
  // todo change for has TC::template type
  template <class TC >
  struct is_type_constructor : is_base_of<type_constructor_tag, TC> {};

  // apply a type constuctor TC to the type parameters Args
  template<class TC, class... Args>
  using apply = typename TC::template apply<Args...>;

  // type constructor customization point.
  // Default implementation make use of a nested type type_constructor
  template <class M >
  struct type_constructor : identity<typename  M::type_constructor> {};

  // type constructor getter meta-function
  template <class M >
  using type_constructor_t = typename type_constructor<M>::type;

  // rebinds a type having a underlying type with another underlying type
  template <class M, class U>
  using rebind = apply<type_constructor_t<M>, U>;

  // transforms a template class into a type_constructor that add the parameter at the end
  template <template <class ...> class TC, class... Args>
  struct lift : type_constructor_tag
  {
    template<class... Args2>
    using apply = TC<Args..., Args2...>;
  };

  // transforms a template class into a type_constructor that add the parameter at the begining
  template <template <class ...> class TC, class... Args>
  struct reverse_lift : type_constructor_tag
  {
    template<class... Args2>
    using apply = TC<Args2..., Args...>;
  };



  // make() overload
  template <template <class ...> class M>
  M<void> make()
  {
    return make(type<M<void>>{});
  }

  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, int = 0, int..., class X>
  M<std::decay_t<X>>
  make(X&& x)
  {
    return make(type<M<std::decay_t<X>>>{}, std::forward<X>(x));
  }

  // make overload: requires a type construcor, deduce the underlying type
  template <class TC, int = 0, int..., class X>
  typename enable_if<is_type_constructor<TC>::value,
    apply<TC, std::decay_t<X>>
  >::type
  make(X&& x)
  {
    return make(type<apply<TC, std::decay_t<X>>>{}, std::forward<X>(x));
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
