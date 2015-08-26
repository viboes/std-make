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
#include <experimental/meta/v1/lift.hpp>
#include <experimental/meta/v1/void_.hpp>
#include <experimental/fundamental/v2/holder.hpp>

#include <type_traits> //add_pointer

namespace std
{
  template <>
  struct add_pointer<experimental::_t> : experimental::meta::lift<add_pointer> {};

namespace experimental
{
inline namespace fundamental_v2
{
  // T* type constructor
  using add_pointer_f = experimental::meta::lift<add_pointer>;

}
namespace meta
{
inline namespace v1
{
  // type constructor customization point.
  template<class T, class = void>
  struct type_constructor {};

  template< class, class = void >
  struct has_type_constructor_member : false_type { };
  template< class T >
  struct has_type_constructor_member<T, void_<typename T::type_constructor>> : true_type { };

  namespace detail {
    template <class T, class HasTypeConstructorTypeMember >
    struct type_constructor;
    template <class T >
    struct type_constructor<T, true_type>: id<typename T::type_constructor> {};

  }

  template <class T >
  struct type_constructor<T> : detail::type_constructor<T, has_type_constructor_member<T>> {};

//  // Default implementation make use of a nested type type_constructor
//  template <class M >
//  struct type_constructor<M> : id<typename M::type_constructor>
//  {};
//  template <class T, void_<typename T::type_constructor>>
//  struct type_constructor<T>
//    //requires requires { typename T::type_constructor; }
//    : enable_if<is_object<typename T::type_constructor>::value, typename T::type_constructor> { };

  // type constructor getter meta-function
  template <class M >
  using type_constructor_t = eval<type_constructor<M>>;
  template <class M >
  using TypeConstructor = eval<type_constructor<M>>;

  template <template <class...> class TC>
  struct type_constructor_template : type_constructor<TC<_t> >
  {};

  // type constructor getter meta-function
  template <template <class...> class TC>
  using type_constructor_template_t = eval<type_constructor_template<TC>>;

  template <class T>
  struct type_constructor<T*> : id<add_pointer<_t>>  {};

}
}
}
}
#endif // header
