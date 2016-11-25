//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_APPLICATIVE_APPLICATIVE_HPP
#define JASEL_FUNDAMENTAL_V3_APPLICATIVE_APPLICATIVE_HPP

#if __cplusplus >= 201402L

///////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////

#include <utility>
#include <type_traits>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class T>
    struct is_applicative;

  template <class T>
    constexpr bool is_applicative_v = is_applicative<T>::value;

namespace applicative
{
  using namespace functor;
  using namespace type_constructible;

  struct tag{};

  template <class A, class Enabler=void>
    struct traits : traits<A, meta::when<true>> {};

  // Default failing specialization
  template <typename Ap, bool condition>
  struct traits<Ap, meta::when<condition>>
  {
    // ap:: [T->U] x [T] -> [U]
    template <class F, class A>
      static auto ap(F&& y, A&& x) =delete;
  };

  template <class F, class A>
  // requires is_same_v<type_constructor_t<decay_t<A>>, type_constructor_t<decay_t<F>>>
  auto
    ap(F&& f, A&& x)
      JASEL_NOEXCEPT_RETURN(
          traits<type_constructor_t<decay_t<A>>>::ap(forward<F>(f), forward<A>(x))
       )

}

  template <class T>
    struct is_applicative : is_base_of<applicative::tag, applicative::traits<T>> {};
  template <class T>
    struct is_applicative<const T> : is_applicative<T> {};
  template <class T>
    struct is_applicative<volatile T> : is_applicative<T> {};
  template <class T>
    struct is_applicative<const volatile T> : is_applicative<T> {};


}
}
}
#endif
#endif // header
