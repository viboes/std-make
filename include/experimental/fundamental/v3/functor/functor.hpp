//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_FUNCTOR_FUNCTOR_HPP
#define JASEL_FUNDAMENTAL_V3_FUNCTOR_FUNCTOR_HPP

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
    struct is_functor;

  template <class T>
    constexpr bool is_functor_v = is_functor<T>::value;

  struct functor_tag{};

  template <class F, class Enabler=void>
    struct functor_traits : functor_traits<F, meta::when<true>> {};

  // Default failing specialization
  template <typename U, bool condition>
  struct functor_traits<U, meta::when<condition>>
  {
      template <class T, class F>
        static auto transform(T&& x, F&& y) =delete;
  };

namespace functor2 {
  template <class T>
    using traits = functor_traits<T>;

  template <class T, class F>
  auto
    transform(T&& x, F&& f)
      JASEL_NOEXCEPT_RETURN(
          traits<remove_cv_t<remove_reference_t<T>>>::transform(forward<T>(x),forward<F>(f))
       )
}

  template <class T>
    struct is_functor : is_base_of<functor_tag, functor_traits<T>> {};
  template <class T>
  struct is_functor<const T> : is_functor<T> {};
  template <class T>
  struct is_functor<volatile T> : is_functor<T> {};
  template <class T>
  struct is_functor<const volatile T> : is_functor<T> {};


}
}
}
#endif
#endif // header
