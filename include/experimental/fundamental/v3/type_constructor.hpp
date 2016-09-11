//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V2_FUNDAMENTAL_TYPE_CONSTRUCTOR_HPP
#define JASEL_V2_FUNDAMENTAL_TYPE_CONSTRUCTOR_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/meta/v1/quote.hpp>
#include <experimental/meta/v1/void_.hpp>
#include <experimental/meta/v1/quote.hpp>
#include <experimental/fundamental/v3/holder.hpp>

#include <type_traits> //add_pointer

namespace std
{
  template <>
  struct add_pointer<experimental::_t> : experimental::meta::quote<add_pointer> {};

namespace experimental
{
inline namespace fundamental_v2
{
  // T* type constructor
  using add_pointer_f = add_pointer<experimental::_t>;

  // type constructor customization point.
  template<class T, class = void>
  struct type_constructor {};

  // type constructor getter meta-function
  template <class M >
  using type_constructor_t = meta::eval<type_constructor<M>>;
  template <class M >
  using TypeConstructor = meta::eval<type_constructor<M>>;

  namespace detail {
    template <class TC, class = void>
    struct is_type_constructor : false_type {};
    template <class TC>
    struct is_type_constructor<TC, meta::void_< meta::quote<TC::template invoke> > >
      : true_type {};
  }

  /// trait stating if a metafunction \p TC is applicable with the argument \p U
  template <class TC>
  struct is_type_constructor : detail::is_type_constructor<TC> {};

  namespace detail {
    template< class, class = void >
    struct has_type_constructor_member : false_type { };
    template< class T >
    struct has_type_constructor_member<T, meta::void_<typename T::type_constructor>> : true_type { };

    template <class T, class Enabler >
    struct type_constructor_if_has_type_constructor_member;
    template <class T >
    struct type_constructor_if_has_type_constructor_member<T, true_type>: meta::id<typename T::type_constructor> {};

    template< template <class...> class TC, class = void >
    struct valid_type_constructor_tc_t : false_type { };
    template< template <class...> class TC >
    struct valid_type_constructor_tc_t<TC, meta::void_<  type_constructor_t<TC<_t>>>>
      : true_type { };

    template <template <class...> class TC, class Enabler >
    struct type_constructor_if_valid_type_constructor_tc_t;
    template <template <class...> class TC >
    struct type_constructor_if_valid_type_constructor_tc_t<TC, true_type>
      : type_constructor<TC<_t>> {};

  }

  template <class T >
  struct type_constructor<T> : detail::type_constructor_if_has_type_constructor_member<T, detail::has_type_constructor_member<T>> {};

#if 0
// fixme this doesn't compile
  template <template <class...> class TC >
  struct type_constructor<meta::quote<TC>>
    : detail::type_constructor_if_valid_type_constructor_tc_t<TC,
        detail::valid_type_constructor_tc_t<TC>> {};
#else
  template <template <class...> class TC >
  struct type_constructor<meta::quote<TC>> : type_constructor<TC<_t> > {};
#endif

//  // Default implementation make use of a nested type type_constructor
//  template <class M >
//  struct type_constructor<M> : id<typename M::type_constructor>
//  {};
//  template <class T, void_<typename T::type_constructor>>
//  struct type_constructor<T>
//    //requires requires { typename T::type_constructor; }
//    : enable_if<is_object<typename T::type_constructor>::value, typename T::type_constructor> { };


  template <class T>
  struct type_constructor<T*> : meta::id<add_pointer<_t>>  {};

}
}
}
#endif // header
