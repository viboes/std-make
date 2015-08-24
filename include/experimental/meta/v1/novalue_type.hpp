//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V1_META_NOVALUE_TYPE_HPP
#define JASEL_V1_META_NOVALUE_TYPE_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{

  // customization point defaults to nested type
  template <class M >
  struct novalue_type : id<typename M::novalue_type>
  {};

  // novalue_type getter meta-function
  template <class M >
  using novalue_type_t = eval<novalue_type<M>>;

}
}
}
}
#endif // header
