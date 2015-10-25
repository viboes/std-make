// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_NONE_HPP
#define JASEL_FUNDAMENTAL_V2_NONE_HPP

# if defined __clang__ || defined __GNU_LIBRARY__
#   define JASEL_FUNDAMENTAL_V2_NONE_CUSTOM
# elif defined __GNUC__
# endif

//darwin.compile.c++ bin/make_boost_optional_pass.test/darwin-5.1.0_14/debug/make/make_boost_optional_pass.o
//In file included from make/make_boost_optional_pass.cpp:19:0:
//../include/experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp: In substitution of ‘template<class M> decltype (none<std::experimental::meta::v1::type_constructor_t<M> >()) std::experimental::fundamental_v2::none_custom(std::experimental::fundamental_v2::pointer_like, const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’:
//../include/experimental/fundamental/v2/possible_valued/none.hpp:23:25:   required by substitution of ‘template<class M> decltype (none_custom(std::experimental::fundamental_v2::concept_tag_t<std::experimental::fundamental_v2::possible_value, M>{}, m)) std::experimental::fundamental_v2::none(const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//../include/experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp:56:48:   required by substitution of ‘template<class M> decltype (none<std::experimental::meta::v1::type_constructor_t<M> >()) std::experimental::fundamental_v2::none_custom(std::experimental::fundamental_v2::pointer_like, const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//../include/experimental/fundamental/v2/possible_valued/none.hpp:23:25:   required by substitution of ‘template<class M> decltype (none_custom(std::experimental::fundamental_v2::concept_tag_t<std::experimental::fundamental_v2::possible_value, M>{}, m)) std::experimental::fundamental_v2::none(const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//../include/experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp:56:48:   required by substitution of ‘template<class M> decltype (none<std::experimental::meta::v1::type_constructor_t<M> >()) std::experimental::fundamental_v2::none_custom(std::experimental::fundamental_v2::pointer_like, const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//../include/experimental/fundamental/v2/possible_valued/none.hpp:23:25:   required by substitution of ‘template<class M> decltype (none_custom(std::experimental::fundamental_v2::concept_tag_t<std::experimental::fundamental_v2::possible_value, M>{}, m)) std::experimental::fundamental_v2::none(const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//../include/experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp:56:48:   [ skipping 117 instantiation contexts, use -ftemplate-backtrace-limit=0 to disable ]
//../include/experimental/fundamental/v2/possible_valued/none.hpp:23:25:   required by substitution of ‘template<class M> decltype (none_custom(std::experimental::fundamental_v2::concept_tag_t<std::experimental::fundamental_v2::possible_value, M>{}, m)) std::experimental::fundamental_v2::none(const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//../include/experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp:56:48:   required by substitution of ‘template<class M> decltype (none<std::experimental::meta::v1::type_constructor_t<M> >()) std::experimental::fundamental_v2::none_custom(std::experimental::fundamental_v2::pointer_like, const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//../include/experimental/fundamental/v2/possible_valued/none.hpp:23:25:   required by substitution of ‘template<class M> decltype (none_custom(std::experimental::fundamental_v2::concept_tag_t<std::experimental::fundamental_v2::possible_value, M>{}, m)) std::experimental::fundamental_v2::none(const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//../include/experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp:56:48:   required by substitution of ‘template<class M> decltype (none<std::experimental::meta::v1::type_constructor_t<M> >()) std::experimental::fundamental_v2::none_custom(std::experimental::fundamental_v2::pointer_like, const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//../include/experimental/fundamental/v2/possible_valued/none.hpp:23:25:   required by substitution of ‘template<class M> decltype (none_custom(std::experimental::fundamental_v2::concept_tag_t<std::experimental::fundamental_v2::possible_value, M>{}, m)) std::experimental::fundamental_v2::none(const M&) [with M = boost::optional<std::experimental::fundamental_v2::_t>]’
//make/make_boost_optional_pass.cpp:162:68:   required from here
//../include/experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp:56:48: erreur fatale: template instantiation depth exceeds maximum of 128 (use -ftemplate-depth= to increase the maximum)
//   -> decltype(none<meta::type_constructor_t<M>>())
//                                                ^
//compilation terminée.

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
#if defined JASEL_FUNDAMENTAL_V2_NONE_CUSTOM
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

  template <class TC>
  constexpr meta::NoneType<TC> none_wa()
  {
    return meta::NoneType<TC>();
  }

  template <template <class ...> class TC>
  constexpr auto none_wa()
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
