//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_IS_INVOKABLE_HPP
#define JASEL_META_V1_IS_INVOKABLE_HPP

#include <experimental/meta/v1/void_.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <type_traits> //false_type

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  namespace detail {
    template <class TC, class List, class = void>
    struct is_invokable_with : false_type {};
    template <class TC, class ...U>
    struct is_invokable_with<TC, types<U...>, void_<typename TC::template invoke<U...>>>
      : true_type {};

    template <class TC, class = void>
    struct is_invokable : false_type {};
    template <class TC>
    struct is_invokable<TC, meta::void_< meta::quote<TC::template invoke> > >
      : true_type {};

  }

  /// trait stating if a metafunction \p TC is applicable with the argument \p U
  template <class TC, class ...U >
  struct is_invokable_with : eval<detail::is_invokable_with<TC, types<U...>>> {};

  template <class TC>
  struct is_invokable : eval<detail::is_invokable<TC>> {};

}
}
}
}

#endif // header
