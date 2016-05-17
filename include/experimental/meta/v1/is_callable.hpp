//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_IS_CALLABLE_HPP
#define JASEL_META_V1_IS_CALLABLE_HPP

#include <experimental/meta/v1/void_.hpp>
#include <experimental/meta/v1/invoke.hpp>
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
    template <class Sig, class R, class = void>
    struct is_callable : false_type {};
    template <class Fn, class ...U>
    struct is_callable<Fn(U...), void, void_<typename Fn::template invoke<U...>>>
    : true_type {};
    template <class Fn, class ...U, class R>
    struct is_callable<Fn(U...), R, void_<typename Fn::template invoke<U...>>>
    : is_convertible<invoke<Fn, U...>, R> {  };

    template <class TC, class = void>
    struct is_invokable : false_type {};
    template <class TC>
    struct is_invokable<TC, meta::void_< meta::quote<TC::template invoke> > >
      : true_type {};

  }

  /// trait stating if a metafunction \p TC is applicable with the argument \p U
  template <class Sig, class R = void >
  struct is_callable; // not defined

  template <class Fn, class ...U, class R >
  struct is_callable<Fn(U...), R> : detail::is_callable<Fn(U...), R> {};

  template <class TC>
  struct is_invokable : detail::is_invokable<TC> {};

}
}
}
}

#endif // header
