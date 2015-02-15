// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

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
inline namespace fundamental_v1
{
  constexpr struct in_place_t{} in_place{};
}
#endif

inline namespace fundamental_v2
{
//namespace meta
//{

  /// evaluates a meta-expression \p T by returning the nested \c T::type
  template <class T>
  using eval = typename T::type;

  // identity meta-function
  template<class T>
  struct id
  {
    using type = T;
  };

  // tag type
  template <class T>
  struct type
  {
    using underlying_type = T;
  };

  // tag template class
  template <template <class...> class T>
  struct template_class
  {
  };

  // holder type
  struct _t {};

  namespace detail {
    template <class T>
    struct deduced_type_impl : id<T> {};

    template <class T>
    struct deduced_type_impl<reference_wrapper<T>> : id<T&> {};
  }

  template <class T>
  struct deduced_type : detail::deduced_type_impl<eval<decay<T>>> {};

  template <class T>
  using deduced_type_t = eval<deduced_type<T>>;

  namespace detail {
    template <class TC, class ...U>
    struct is_applicable_with
    {
    private:
        struct two {char a; char b;};
        template <class X> static two test(...);
        template <class X> static char test(typename X::template apply<U...>* = 0);
    public:
        static const bool value = sizeof(test<TC>(0)) == 1;
    };
    template <class TC, class U>
    struct is_applicable_with1
    {
    private:
        struct two {char a; char b;};
        template <class X> static two test(...);
        template <class X> static char test(typename X::template apply<U>* = 0);
    public:
        static const bool value = sizeof(test<TC>(0)) == 1;
    };
  }

  /// trait stating if a metafunction \p TC is applicable with the argument \p U
  template <class TC, class ...U >
  struct is_applicable_with : detail::is_applicable_with<TC, U...> {};

  template <class TC, class U >
  struct is_applicable_with1 : detail::is_applicable_with1<TC, U> {};

  /// applies a meta-function \p TC to the arguments \p Args
  template<class TC, class... Args>
  using apply = typename TC::template apply<Args...>;

#ifdef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR
  // type constructor customization point.
  // Default implementation make use of a nested type type_constructor
  template <class M >
  struct type_constructor : id<typename  M::type_constructor> {};

  // type constructor getter meta-function-class
  template <class M >
  using type_constructor_t = eval<type_constructor<M>>;

  // rebinds a type having a underlying type with another underlying type
  template <class M, class ...U>
  using rebind2 = apply<eval<type_constructor<M>>, U...>;
#endif

  template <class M, class ...U>
  struct rebind : id<typename M::template rebind<U...>> {};

  template <template<class ...> class TC, class ...Ts, class ...Us>
  struct rebind<TC<Ts...>, Us...> : id<TC<Us...>> {};

  template <class M, class ...Us>
  using rebind_t = eval<rebind<M, Us...>>;

  // transforms a template class into a type_constructor that adds the parameter at the end
  template <template <class ...> class TC>
  struct quote
  {
  private:
    /// Indirection here needed to avoid Core issue 1430
    /// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
    template <class... Args>
    struct impl : id<TC<Args...>> {};
  public:
    template <class... Args>
    using apply = eval<impl<Args...>>;
  };

  // transforms a template class into a type_constructor that adds the parameter at the end
  template <template <class ...> class TC, class... Args>
  struct lift
  {
  private:
    /// Indirection here needed to avoid Core issue 1430
    /// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
    template <class... Args2>
    struct impl : id<TC<Args..., Args2...>> {};
  public:
    template <class... Args2>
    using apply = eval<impl<Args2...>>;
  };

  // transforms a template class into a type_constructor that adds the parameter at the begining
  template <template <class ...> class TC, class... Args>
  struct reverse_lift
  {
  private:
    /// Indirection here needed to avoid Core issue 1430
    /// http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
    template <class... Args2> struct impl : id<TC<Args2..., Args...>> {};
  public:
    template<class... Args2>
    using apply = eval<impl<Args2...>>;
  };

  //  // undelying_type customization point.
  //  // Default implementation make use of a nested type underlying_type
  //  template <class M >
  //  struct underlying_type : id<typename  M::underlying_type> {};
  //
  //  // underlying_type getter meta-function
  //  template <class M >
  //  using underlying_type_t = typename underlying_type<M>::type;

  template <template <class ...> class TC>
  auto none()
  {
    return none_custom(template_class<TC>{});
  }

  template <class TC>
  auto none() {
    return none_custom(type<TC>{});
  }

  template <class TC>
  struct noner
  {
    auto operator()() {
      return none_custom(type<TC>{});
    }
  };

  // make() overload
  template <template <class> class M>
  M<void> make()
  {
    return make(type<M<void>>{});
  }

//  template <class TC, int = 0, int...>
//  typename enable_if<
//    is_applicable_with1<TC, void>::value,
//    apply<TC, void>
//  >::type
//  make()
//  {
//    return make(type<apply<TC, void>>{});
//  }

  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, int = 0, int..., class ...X>
  M<eval<deduced_type<X>>...>
  make(X&& ...x)
  {
    return make(type<M<eval<deduced_type<X>>...>>{}, std::forward<X>(x)...);
  }

  // make overload: requires a type constructor, deduce the underlying type
  template <class TC, int = 0, int..., class ...X>
  typename enable_if<is_applicable_with<TC, eval<deduced_type<X>>...>::value,
    apply<TC, eval<deduced_type<X>>...>
  >::type
  make(X&& ...x)
  {
    return make(type<apply<TC, eval<deduced_type<X>>...>>{}, std::forward<X>(x)...);
  }

  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from X
  template <class M, int = 0, int..., class X>
  typename enable_if<   ! is_applicable_with<M, eval<deduced_type<X>>>::value
                     //&&   is_same<M, rebind_t<M, X>>::value
  , M
  >::type
  make(X&& x)
  {
    return make(type<M>{}, std::forward<X>(x));
  }

  // make emplace overload: requires a type with a specific underlying type, don't deduce the underlying type from X
  template <class M, class ...Args>
  typename enable_if<    ! is_applicable_with<M, eval<deduced_type<Args>>...>::value
  , M
  >::type
  make(Args&& ...args)
  {
    return make(type<M>{}, in_place, std::forward<Args>(args)...);
  }

  template <class TC>
  struct maker
  {
    // make overload: requires a type constructor, deduce the underlying type
    template <class ...X>
    apply<TC, eval<deduced_type<X>>...>
    operator()(X&& ...x)
    {
      return make<TC>(std::forward<X>(x)...);
    }
  };

  template <class M>
  struct emplacer
  {
    // make overload: requires a type with a specific underlying type, don't deduce the underlying type from X
    template <class X>
    M operator()(X&& x)
    {
      return make<M>(std::forward<X>(x));
    }

    // make emplace overload: requires a type with a specific underlying type, don't deduce the underlying type from X
    template <class ...Args>
    M operator()(Args&& ...args)
    {
      return make<M>(std::forward<Args>(args)...);
    }
  };

  // default customization point for TC<void> default constructor
  template <class M>
  typename enable_if<std::is_default_constructible<M>::value,  M>::type
  make(type<M>)
  {
    return M();
  }

  // default customization point for constructor from X
  template <class M, class ...X>
  typename enable_if<std::is_constructible<M, eval<deduced_type<X>>...>::value,  M>::type
  make(type<M>, X&& ...x)
  {
    return M(std::forward<eval<deduced_type<X>>>(x)...);
  }

  // default customization point for in_place constructor
  template <class M, class ...Args>
  typename enable_if<std::is_constructible<M, in_place_t, eval<deduced_type<Args>>...>::value,  M>::type
  make(type<M>, in_place_t, Args&& ...args)
  {
    return M(in_place, std::forward<Args>(args)...);
  }

}
}
}

#endif // VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_HPP
