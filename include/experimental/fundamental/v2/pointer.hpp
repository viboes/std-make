//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_POINTER_HPP
#define JASEL_FUNDAMENTAL_V2_POINTER_HPP

///////////////////////////////////////////////////////////////////////////////////////
/// T*.
///
/// This type is a model of a PossibleValued with the following customizations
///
///   type_constructor<T*> : add_pointer<_t>
///
///   value_type<T*> : T
///   novalue_type<T*> : nullptr_t
///
///   make<add_pointer<_t>(T v) -> T* { return new T{v}; }
///   none<type<add_pointer<_t>>() -> nullptr_t { return nullptr; }
///   has_value(const T* ptr) -> bool { return ptr; }
///   value(T*) -> T& { return *ptr; }
///   value(const T*) -> T const & { return *ptr; }
///   novalue(const T*) -> nullptr_t { return nullptr_t; }
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v2/make.hpp>
#include <experimental/fundamental/v2/none.hpp>
#include <experimental/meta/v1/lift.hpp>
#include <experimental/meta/v1/novalue_type.hpp>
#include <experimental/meta/v1/type_constructor.hpp>
#include <experimental/meta/v1/value_type.hpp>

#include <type_traits> //add_pointer

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  // T* type constructor
  using add_pointer_f = experimental::meta::lift<add_pointer>;

  template <>
  struct add_pointer<experimental::_t> : add_pointer_f {};
}

namespace meta
{
inline namespace v1
{

  template <class T>
  struct type_constructor<T*> : id<add_pointer<_t>>  {};

  template <class T >
  struct value_type<T*> : id<T>  {};

  template <class T >
  struct novalue_type<T*> : id<nullptr_t>  {};

  template <class T>
  T* make_custom(meta::type<add_pointer<_t>>, T v) {
    return new T{v};
  }
  nullptr_t none_custom(meta::type<add_pointer<_t>>) {
    return nullptr;
  }

}
}

inline namespace fundamental_v2
{

  template <class T>
  bool has_value(T* ptr) {
    return ptr != nullptr;
  }

  template <class T>
  T& value(T* ptr) {
    return *ptr;
  }
  template <class T>
  const T& value(const T* ptr) {
    return *ptr;
  }

  template <class T>
  nullptr_t novalue(const T* ) {
    return nullptr;
  }

}
}
}


#endif // header

