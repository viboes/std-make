//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_CONCEPT_MAP_HPP
#define JASEL_FUNDAMENTAL_V2_CONCEPT_MAP_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class C, class T>
  using concept_tag_t = meta::eval<typename C::template tag<T>>;

  template <class C, class T>
  using concept_instance_t = meta::eval<typename C::template instance<concept_tag_t<C,T>>>;
}
}
}
#endif // header

