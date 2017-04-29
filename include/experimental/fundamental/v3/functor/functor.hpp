//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_FUNCTOR_FUNCTOR_HPP
#define JASEL_FUNDAMENTAL_V3_FUNCTOR_FUNCTOR_HPP

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
    struct is_functor;

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
    constexpr bool is_functor_v = is_functor<T>::value;
#endif

namespace functor
{
  struct tag{};

  template <class F, class Enabler=void>
    struct traits
#if  ! defined JASEL_DOXYGEN_INVOKED
        : traits<F, meta::when<true>> {}
#endif
        ;

  // Default failing specialization
  template <typename U, bool condition>
  struct traits<U, meta::when<condition>>
  {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
      template <class T, class F>
        static auto transform(T&& x, F&& y) =delete;
#endif
  };

  template <class T, class F>
  auto
    transform(T&& x, F&& f)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
            traits<type_constructor_t<meta::uncvref_t<T>>>::transform(forward<T>(x),forward<F>(f))
        )

  template <class F, class T>
   auto
     map(F&& f, T&& x)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
            functor::transform(forward<F>(f), forward<T>(x))
        )
}

  template <class T>
    struct is_functor
#if ! defined JASEL_DOXYGEN_INVOKED
        : is_base_of<functor::tag, functor::traits<T>> {}
#endif
        ;
  template <class T>
    struct is_functor<const T> : is_functor<T> {};
  template <class T>
    struct is_functor<volatile T> : is_functor<T> {};
  template <class T>
    struct is_functor<const volatile T> : is_functor<T> {};

}
}
}

#endif // header
