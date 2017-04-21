//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V2_FUNDAMENTAL_NULLABLE_NONE_TYPE_HPP
#define JASEL_V2_FUNDAMENTAL_NULLABLE_NONE_TYPE_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/type_traits.hpp>

namespace std
{

namespace experimental
{
inline namespace fundamental_v3
{
  template <class T, class Enabler =  void >
  struct none_type;

  // none_type getter meta-function
  template <class T>
  using none_type_t = meta::eval<none_type<T>>;
  template <class T>
  using NoneType = meta::eval<none_type<T>>;

  template <class T >
  struct none_type<T*>
#if ! defined JASEL_DOXYGEN_INVOKED
  : enable_if<is_object<T>::value, nullptr_t>  {}
#endif
  ;
  template <class I>
  struct none_type<I const> : none_type<decay_t<I>> { };
  template <class I>
  struct none_type<I volatile> : none_type<decay_t<I>> { };
  template <class I>
  struct none_type<I const volatile> : none_type<decay_t<I>> { };

#if ! defined JASEL_DOXYGEN_INVOKED
  namespace nullable_detail {
    template< class, class = void >
    struct has_none_type_member : false_type { };
    template< class T >
    struct has_none_type_member<T, void_t<typename T::none_type>> : true_type { };

    template <class T, class Enabler>
    struct none_type;
    template <class T >
    struct none_type<T, true_type>: meta::id<typename T::none_type> {};
  }
#endif
  template <class T >
  struct none_type<T>
#if ! defined JASEL_DOXYGEN_INVOKED
  : nullable_detail::none_type<T, nullable_detail::has_none_type_member<T>> {}
#endif
  ;

//  // customization point defaults to nested type
//  struct none_type<T>
//    //requires requires { typename T::none_type; }
//    : enable_if<is_object<typename T::none_type>::value, typename T::none_type> { };

}
}
}
#endif // header
