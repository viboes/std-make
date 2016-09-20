//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V2_FUNDAMENTAL_NONE_TYPE_HPP
#define JASEL_V2_FUNDAMENTAL_NONE_TYPE_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/meta/v1/void_.hpp>

namespace std
{

namespace experimental
{
inline namespace fundamental_v2
{
  template <class T, class Enabler =  void >
  struct none_type;;

  // none_type getter meta-function
  template <class T>
  using none_type_t = meta::eval<none_type<T>>;
  template <class T>
  using NoneType = meta::eval<none_type<T>>;

  template <class T >
  struct none_type<T*> : enable_if<is_object<T>::value, nullptr_t>  {};
  template <class I>
  struct none_type<I const> : none_type<typename decay<I>::type> { };
  template <class I>
  struct none_type<I volatile> : none_type<typename decay<I>::type> { };
  template <class I>
  struct none_type<I const volatile> : none_type<typename decay<I>::type> { };


  namespace detail {
    template< class, class = void >
    struct has_none_type_member : false_type { };
    template< class T >
    struct has_none_type_member<T, meta::void_<typename T::none_type>> : true_type { };

    template <class T, class Enabler>
    struct none_type;
    template <class T >
    struct none_type<T, true_type>: meta::id<typename T::none_type> {};

  }
  template <class T >
  struct none_type<T> : detail::none_type<T, detail::has_none_type_member<T>> {};

//  // customization point defaults to nested type
//  struct none_type<T>
//    //requires requires { typename T::none_type; }
//    : enable_if<is_object<typename T::none_type>::value, typename T::none_type> { };

}
}
}
#endif // header
