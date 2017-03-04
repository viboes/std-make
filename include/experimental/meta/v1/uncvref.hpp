//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_UNCVREF_HPP
#define JASEL_META_V1_UNCVREF_HPP

#include <experimental/meta/v1/eval.hpp>
#include <type_traits> //remove_cv_t, remove_reference_t

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{

  template <class T>
  struct uncvref : remove_cv<eval<remove_reference<T>>>  {};

  template <class T>
  using uncvref_t = eval<uncvref<T>>;

}
}
}
}

#endif // header
