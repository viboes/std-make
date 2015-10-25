//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_POSSIBLE_VALUED_MCD_POINTER_LIKE_HPP
#define JASEL_FUNDAMENTAL_V2_POSSIBLE_VALUED_MCD_POINTER_LIKE_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// This type that models the PointerLike concept can be seen as a model of a PossibleValued
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v2/possible_valued.hpp>
#include <experimental/fundamental/v2/pointer_like.hpp>
#include <experimental/fundamental/v2/none.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class M>
  bool has_value_custom(pointer_like, M const& v)
  {
    return bool(v);
  }

  template <class M>
  meta::value_type_t<M>&
  value_custom(pointer_like, M& ptr)
  {
    return *ptr;
  }
  template <class M>
  meta::value_type_t<M> const&
  value_custom(pointer_like, M const& ptr)
  {
    return *ptr;
  }

  // This overload a little bit intrusive as it requires in addition to the pointer semantic that none<TC>().
  // That means that either we have no pointer-like mapping,
  //    that the mapping must be partial (we need to use instance<Tag>) or
  //    that this operation should be moved outside the concept.

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

  // fixme
  // In order to break the cycle we use a woraround none_wa
  // alternatively we can use a different name for none<TC>() and none(pv), e.g. not_a_value(pv)

#if defined JASEL_FUNDAMENTAL_V2_NONE_CUSTOM
  template <class M>
  auto none_custom(pointer_like, M const& ptr)
  -> decltype(none<meta::type_constructor_t<M>>())
  {
    return none<meta::type_constructor_t<M>>();
  }
#else
  template <class M>
  auto none_custom(pointer_like, M const& ptr)
  -> decltype(none_wa<meta::type_constructor_t<M>>())
  {
    return none_wa<meta::type_constructor_t<M>>();
  }
#endif
}
}
}


#endif // header

