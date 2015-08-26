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
  template< class, class = void >
  struct has_novalue_type_member : false_type { };
  template< class T >
  struct has_novalue_type_member<T, void_<typename T::novalue_type>> : true_type { };

  template <class T, class Enabler =  void >
  struct novalue_type {};

  // novalue_type getter meta-function
  template <class T>
  using novalue_type_t = eval<novalue_type<T>>;
  template <class T>
  using NoValueType = eval<novalue_type<T>>;

  template <class T >
  struct novalue_type<T*> : enable_if<is_object<T>::value, nullptr_t>  {};
  template <class I>
  struct novalue_type<I const> : novalue_type<decay_t<I>> { };
  template <class I>
  struct novalue_type<I volatile> : novalue_type<decay_t<I>> { };
  template <class I>
  struct novalue_type<I const volatile> : novalue_type<decay_t<I>> { };


  namespace detail {
    template <class T, class IsArray, class HasValueTypeMember>
    struct novalue_type;
    template <class T>
    struct novalue_type<T, true_type, false_type>: novalue_type_t<decay_t<T>> {};
    template <class T >
    struct novalue_type<T, false_type, true_type>: id<typename T::novalue_type> {};

  }
  template <class T >
  struct novalue_type<T> : detail::novalue_type<T, is_array<T>, has_value_type_member<T>> {};

//  template<class I, enable_if<is_array<I>::value>>
//    //requires is_array<I>::value
//  struct novalue_type<I> :  novalue_type<decay_t<I>> { };
//  // customization point defaults to nested type
//  struct novalue_type<T>
//    //requires requires { typename T::novalue_type; }
//    : enable_if<is_object<typename T::novalue_type>::value, typename T::novalue_type> { };



}
}
}
}
#endif // header
