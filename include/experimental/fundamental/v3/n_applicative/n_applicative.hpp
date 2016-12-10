//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_N_APPLICATIVE_N_APPLICATIVE_HPP
#define JASEL_FUNDAMENTAL_V3_N_APPLICATIVE_N_APPLICATIVE_HPP

#if __cplusplus >= 201402L

///////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////

#include <utility>
#include <type_traits>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class T>
    struct is_n_applicative;

  template <class T>
    constexpr bool is_n_applicative_v = is_n_applicative<T>::value;

namespace n_applicative
{
  using namespace n_functor;
  //using namespace type_constructible;

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
    template <class A, class T>
      static auto pure(T&& x) =delete;
  };

  template <class F, class A>
  // requires is_same_v<type_constructor_t<decay_t<A>>, type_constructor_t<decay_t<F>>>
  auto
    ap(F&& f, A&& x)
      JASEL_NOEXCEPT_RETURN(
          traits<type_constructor_t<decay_t<A>>>::ap(forward<F>(f), forward<A>(x))
       )

   template <class A, class ...Ts>
   // requires is_same_v<value_type_t<A>, T>
   auto
     pure(Ts&& ...xs)
       JASEL_NOEXCEPT_RETURN(
           traits<type_constructor_t<A>>::template pure<A>(forward<Ts>(xs)...)
        )

}

  template <class T>
    struct is_n_applicative : is_base_of<n_applicative::tag, n_applicative::traits<T>> {};
  template <class T>
    struct is_n_applicative<const T> : is_n_applicative<T> {};
  template <class T>
    struct is_n_applicative<volatile T> : is_n_applicative<T> {};
  template <class T>
    struct is_n_applicative<const volatile T> : is_n_applicative<T> {};


}
}
}
#endif
#endif // header
