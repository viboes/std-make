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
  // holder type
  struct _t {};

namespace meta
{

  /// evaluates a meta-expression \p T by returning the nested \c T::type
  template <class T>
  using eval = typename T::type;

  // identity meta-function
  template<class T>
  struct id
  {
    using type = T;
  };

  template <class ...Ts>
  struct types
  {
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

  /// applies a meta-function \p TC to the arguments \p Args
  template<class TC, class... Args>
  using apply = typename TC::template apply<Args...>;

  /// \brief A Metafunction Class that always returns \c T.
  template<typename T>
  struct always
  {
  private:
    // Redirect through a class template for compilers that have not
    // yet implemented CWG 1558:
    // <http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1558>
    template<typename...>
    struct impl
    {
      using type = T;
    };
  public:
    template<typename...Ts>
    using apply = eval<impl<Ts...>>;
  };

  /// \brief An alias for `void`.
  template<typename...Ts>
  using void_ = apply<always<void>, Ts...>;

  namespace detail {
    template <class TC, class List, class = void>
    struct is_applicable_with : std::false_type {};
    template <class TC, class ...U>
    struct is_applicable_with<TC, types<U...>, void_<typename TC::template apply<U...>>>
      : std::true_type {};
  }

  /// trait stating if a metafunction \p TC is applicable with the argument \p U
  template <class TC, class ...U >
  struct is_applicable_with : eval<detail::is_applicable_with<TC, types<U...>>> {};

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
}
  template <template <class ...> class TC>
  constexpr auto none()
  {
    return none_custom(meta::template_class<TC>{});
  }

  template <class TC>
  constexpr auto none() {
    return none_custom(meta::type<TC>{});
  }

  // make() overload
  template <template <class> class M>
  constexpr M<void> make()
  {
    return make(meta::type<M<void>>{});
  }

  template <class TC, int = 0, int...>
  constexpr typename enable_if<
  meta::is_applicable_with<TC, void>::value,
  meta::apply<TC, void>
  >::type
  make()
  {
    return make(meta::type<meta::apply<TC, void>>{});
  }

  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, int = 0, int..., class ...X>
  constexpr M<meta::eval<meta::deduced_type<X>>...>
  make(X&& ...x)
  {
    return make(meta::type<M<meta::eval<meta::deduced_type<X>>...>>{}, std::forward<X>(x)...);
  }

  // make overload: requires a type constructor, deduce the underlying type
  template <class TC, int = 0, int..., class ...X>
  constexpr typename enable_if<meta::is_applicable_with<TC, meta::eval<meta::deduced_type<X>>...>::value,
  meta::apply<TC, meta::eval<meta::deduced_type<X>>...>
  >::type
  make(X&& ...x)
  {
    return make(meta::type<meta::apply<TC, meta::eval<meta::deduced_type<X>>...>>{}, std::forward<X>(x)...);
  }

  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from X
  template <class M, int = 0, int..., class ...Xs>
  constexpr typename enable_if<   ! meta::is_applicable_with<M, meta::eval<meta::deduced_type<Xs>>...>::value
                     //&&   is_same<M, rebind_t<M, X>>::value
  , M
  >::type
  make(Xs&& ...xs)
  {
    return make(meta::type<M>{}, std::forward<Xs>(xs)...);
  }

  template <class MFC>
  struct maker_mfc
  {
    template <class ...Xs>
    constexpr meta::apply<MFC, meta::eval<meta::deduced_type<Xs>>...>
    operator()(Xs&& ...xs) const
    {
      return make<MFC>(std::forward<Xs>(xs)...);
    }
  };

  template <template <class ...> class TC>
  struct maker_tc
  {
    template <class ...Xs>
    constexpr TC<meta::eval<meta::deduced_type<Xs>>...>
    operator()(Xs&& ...xs) const
    {
      return make<TC>(std::forward<Xs>(xs)...);
    }
  };

  template <class M>
  struct maker_t
  {
    template <class ...Args>
    constexpr M
    operator()(Args&& ...args) const
    {
      return make<M>(std::forward<Args>(args)...);
    }
  };

  template <template <class ...> class TC>
  struct maker : maker_tc<TC> {};

namespace meta
{
  // default customization point for TC<void> default constructor
  template <class M>
  constexpr typename enable_if<std::is_default_constructible<M>::value,  M>::type
  make(meta::type<M>)
  {
    return M();
  }

  // default customization point for constructor from X
  template <class M, class ...X>
  constexpr typename enable_if<std::is_constructible<M, meta::eval<meta::deduced_type<X>>...>::value,  M>::type
  make(meta::type<M>, X&& ...x)
  {
    return M(std::forward<X>(x)...);
  }

}
}
}
}

#endif // VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_HPP
