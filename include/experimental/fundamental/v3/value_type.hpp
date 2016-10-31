//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_VALUE_TYPE_HPP
#define JASEL_V3_FUNDAMENTAL_VALUE_TYPE_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/type_traits.hpp>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  template< class, class = void >
  struct has_value_type_member : false_type { };
  template< class T >
  struct has_value_type_member<T, void_t<typename T::value_type>> : true_type { };

  template< class, class = void >
  struct has_element_type_member : false_type { };
  template< class T >
  struct has_element_type_member<T, void_t<typename T::element_type>> : true_type { };

  template<class T, class = void>
  struct value_type;

  // value_type getter meta-function
  template <class T>
  using value_type_t = meta::eval<value_type<T>>;
  template <class T>
  using ValueType = meta::eval<value_type<T>>;

  template <class T >
  struct value_type<T*> : enable_if<is_object<T>::value, typename remove_cv<T>::type>  {};

  template <class I>
  struct value_type<I const> : value_type<typename decay<I>::type> { };
  template <class I>
  struct value_type<I volatile> : value_type<typename decay<I>::type> { };
  template <class I>
  struct value_type<I const volatile> : value_type<typename decay<I>::type> { };


  namespace detail {
    template <class T, bool IsArray, bool HasValueTypeMember , bool HasElementTypeMember >
    struct value_type;
    template <class T>
    struct value_type<T, true, false, false>: value_type_t<typename decay<T>::type> {};
    template <class T >
    struct value_type<T, false, true, false>: meta::id<typename T::value_type> {};
    template <class T >
    struct value_type<T, false, false, true>: meta::id<typename T::element_type> {};

  }
  template <class T >
  struct value_type<T> : detail::value_type<T, is_array<T>::value, has_value_type_member<T>::value,
  has_element_type_member<T>::value> {};

//  template<class I>
//    requires is_array<I>::value
//  struct value_type<I> : value_type_t<decay_t<I> { };
//  template <class T>
//  struct value_type<T>
//    requires requires { typename T::value_type; }
//    : enable_if<is_object<typename T::value_type>::value, typename T::value_type> { };
//  template <class T>
//    requires requires { typename T::element_type; }
//  struct value_type<T>
//    : enable_if<is_object<typename T::element_type>::value, typename T::element_type> { };


}
}
}
#endif // header
