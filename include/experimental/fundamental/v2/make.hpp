// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_MAKE_HPP
#define JASEL_FUNDAMENTAL_V2_MAKE_HPP

#include <utility>
#include <type_traits>
#include <experimental/meta/v1/types.hpp>
#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/is_applicable_with.hpp>
#include <experimental/meta/v1/type_constructor.hpp>
#include <experimental/meta/v1/deduced_type.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  // make() overload

  template <class TC, int = 0, int...>
  constexpr
  typename enable_if<
      meta::is_applicable_with<TC, void>::value,
      meta::apply<TC, void>
  >::type
  make()
  {
    return make_custom(meta::id<meta::apply<TC, void>>{});
  }

  template <template <class> class M>
  constexpr M<void> make()
  {
    //return make_custom(meta::id<M<void>>{});
    return make<meta::type_constructor_t<meta::template_class<M>>>();

  }

  // make overload: requires a type constructor, deduce the underlying type
  template <class TC, int = 0, int..., class ...X>
  constexpr typename enable_if<
    meta::is_applicable_with<TC, meta::deduced_type_t<X>...>::value,
    meta::apply<TC, meta::deduced_type_t<X>...>
  >::type
  make(X&& ...x)
  {
    return make_custom(meta::id<meta::apply<TC, meta::deduced_type_t<X>...>>{}, std::forward<X>(x)...);
  }

  // make overload: requires a type with a specific underlying type, don't deduce the underlying type from X
  template <class M, int = 0, int..., class ...Xs>
  constexpr typename enable_if<
    ! meta::is_applicable_with<M, meta::deduced_type_t<Xs>...>::value
    , M
  >::type
  make(Xs&& ...xs)
  {
    return make_custom(meta::id<M>{}, std::forward<Xs>(xs)...);
  }

  // make overload: requires a template class, deduce the underlying type
  template <template <class ...> class M, int = 0, int..., class ...X>
  constexpr M<meta::deduced_type_t<X>...>
  make(X&& ...x)
  {
    //return make_custom(meta::id<M<meta::deduced_type_t<X>...>>{}, std::forward<X>(x)...);
    return make<meta::type_constructor_t<meta::template_class<M>>>(std::forward<X>(x)...);
  }
}
}
}
namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  // default customization point for constructor from X...
  template <class M, class ...X>
  constexpr auto
  make_custom(meta::id<M>, X&& ...x)
  -> decltype(M(std::forward<X>(x)...))
  {
    return M(std::forward<X>(x)...);
  }

  // default customization point for constructor from X...
  template <class T, class ...X>
  constexpr auto
  make_custom(meta::id<T*>, X&& ...x)
  -> decltype(new T(std::forward<X>(x)...))
  {
    return new T(std::forward<X>(x)...);
  }
}
}
}
}

#endif // header
