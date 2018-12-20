// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2018 Vicente J. Botet Escriba

#ifndef JASEL_NULMERIC_V1_SUBSUMES_HPP
#define JASEL_NULMERIC_V1_SUBSUMES_HPP

#include <utility>
#include <type_traits>
#include <experimental/utility.hpp>

namespace std
{
namespace experimental
{
namespace numerics
{
inline  namespace v1
{
  // A type T subsumes a type U if all the instances of U have a corresponding instance in T and U is explicitly constructible from T
  // e.g.
  // signed char < signed short < signed int < signed long < signed long long
  // unsigned char < unsigned short < unsigned int < unsigned long < unsigned long long
  // unsigned char < signed short
  // unsigned short < signed int
  // unsigned int < signed long
  // unsigned long < signed long long
  // In addition, the subsumes relationship must be closed, that is if subsumes<A,B> and subsumes<B,C> then subsumes <A,C>
  // This relation could be extended to types that are not numeric as well.
  template < class T, class U>
  struct subsumes : std::false_type {};

  template < >
  struct subsumes<signed short, signed char> : std::true_type {};
  template < >
  struct subsumes<signed int, signed char> : std::true_type {};
  template < >
  struct subsumes<signed long, signed char> : std::true_type {};
  template < >
  struct subsumes<signed long long, signed char> : std::true_type {};

  template < >
  struct subsumes<signed int, signed short> : std::true_type {};
  template < >
  struct subsumes<signed long, signed short> : std::true_type {};
  template < >
  struct subsumes<signed long long, signed short> : std::true_type {};

  template < >
  struct subsumes<signed long, signed int> : std::true_type {};
  template < >
  struct subsumes<signed long long, signed int> : std::true_type {};

  template < >
  struct subsumes<signed long long, signed long> : std::true_type {};

  //////////

  template < >
  struct subsumes<unsigned short, unsigned char> : std::true_type {};
  template < >
  struct subsumes<unsigned int, unsigned char> : std::true_type {};
  template < >
  struct subsumes<unsigned long, unsigned char> : std::true_type {};
  template < >
  struct subsumes<unsigned long long, unsigned char> : std::true_type {};

  template < >
  struct subsumes<unsigned int, unsigned short> : std::true_type {};
  template < >
  struct subsumes<unsigned long, unsigned short> : std::true_type {};
  template < >
  struct subsumes<unsigned long long, unsigned short> : std::true_type {};

  template < >
  struct subsumes<unsigned long, unsigned int> : std::true_type {};
  template < >
  struct subsumes<unsigned long long, unsigned int> : std::true_type {};

  template < >
  struct subsumes<unsigned long long, unsigned long> : std::true_type {};


  //////
  template < >
  struct subsumes<signed short, unsigned char> : std::true_type {};
  template < >
  struct subsumes<signed int, unsigned char> : std::true_type {};
  template < >
  struct subsumes<signed long, unsigned char> : std::true_type {};
  template < >
  struct subsumes<signed long long, unsigned char> : std::true_type {};

  template < >
  struct subsumes<signed int, unsigned short> : std::true_type {};
  template < >
  struct subsumes<signed long, unsigned short> : std::true_type {};
  template < >
  struct subsumes<signed long long, unsigned short> : std::true_type {};

  template < >
  struct subsumes<signed long, unsigned int> : std::true_type {};
  template < >
  struct subsumes<signed long long, unsigned int> : std::true_type {};

  template < >
  struct subsumes<signed long long, unsigned long> : std::true_type {};


// domain utilities

  //! A value of a domain U is in the domain of a type T if:
  //! * T is U
  //! * T subsumes U
  template < class T, class U >
  constexpr
    typename std::enable_if<
        std::is_same<T, U>::value
    , bool>::type
  in_domain(U const&)
  {
      return true;
  }

  template < class T, class U >
  constexpr
    typename std::enable_if<
        ! std::is_same<T, U>::value && ! subsumes<T, U>::value
    , bool>::type
  in_domain(U const& u)
  {
      return can_narrow_to<T>(u);
      //return U(T(u)) == u;
  }

  template < class T, class U >
  constexpr
    typename std::enable_if<
        subsumes<T, U>::value
    , bool>::type
  in_domain(U const&)
  {
      return true;
  }

}
}
}
}
#endif // header
