//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V1_META_TYPE_CONSTRUCTOR_HPP
#define JASEL_V1_META_TYPE_CONSTRUCTOR_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/fundamental/v2/holder.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{

  // type constructor customization point.
  // Default implementation make use of a nested type type_constructor
  template <class M >
  struct type_constructor : id<typename M::type_constructor>
  {};

  // type constructor getter meta-function
  template <class M >
  using type_constructor_t = eval<type_constructor<M>>;

  template <template <class...> class TC>
  struct type_constructor_template : type_constructor<TC<_t> >
  {};

  // type constructor getter meta-function
  template <template <class...> class TC>
  using type_constructor_template_t = eval<type_constructor_template<TC>>;

}
}
}
}
#endif // header
