//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016-2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_APPLICATIVE_APPLICATIVE_HPP
#define JASEL_FUNDAMENTAL_V3_APPLICATIVE_APPLICATIVE_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////

#include <utility>
#include <type_traits>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class T>
    struct is_applicative;

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
    constexpr bool is_applicative_v = is_applicative<T>::value;
#endif

namespace applicative
{
  using namespace functor; // NOLINT google-build-using-namespace
  //using namespace type_constructible;

namespace detail
{
  struct not_an_applicative_tag {};
}

  template <class A, class Enabler=void>
    struct traits
#if ! defined JASEL_DOXYGEN_INVOKED
    : traits<A, meta::when<true>> {}
#endif
        ;

  // Default failing specialization
  template <typename Ap, bool condition>
  struct traits<Ap, meta::when<condition>> : detail::not_an_applicative_tag
  {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
    // ap:: [T->U] x [T] -> [U]
    template <class F, class A>
      static auto ap(F&& y, A&& x) =delete;
    template <class A, class T>
      static auto pure(T&& x) =delete;
#endif
  };

  template <class F, class A>
  // requires is_same_v<type_constructor_t<remove_cvref_t<A>>, type_constructor_t<decay_t<F>>>
  auto ap(F&& f, A&& x)
     JASEL_DECLTYPE_RETURN_NOEXCEPT(
          traits<type_constructor_t<remove_cvref_t<A>>>::ap(forward<F>(f), forward<A>(x))
     )

   template <class A, class T>
   // requires is_same_v<value_type_t<A>, T>
   auto pure(T&& x)
     JASEL_DECLTYPE_RETURN_NOEXCEPT(
           traits<type_constructor_t<A>>::template pure<A>(forward<T>(x))
     )

}

  template <class T>
    struct is_applicative
#if ! defined JASEL_DOXYGEN_INVOKED
            : integral_constant<bool,
                  ! is_base_of<applicative::detail::not_an_applicative_tag, applicative::traits<T>>::value
                  && is_functor<T>::value
              > {}
#endif
        ;
  template <class T>
    struct is_applicative<const T> : is_applicative<T> {};
  template <class T>
    struct is_applicative<volatile T> : is_applicative<T> {};
  template <class T>
    struct is_applicative<const volatile T> : is_applicative<T> {};


}
}
}
#endif // header
