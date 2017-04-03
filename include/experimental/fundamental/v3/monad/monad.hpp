//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_MONAD_MONAD_HPP
#define JASEL_FUNDAMENTAL_V3_MONAD_MONAD_HPP

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED

///////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////

#include <utility>
#include <type_traits>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/applicative.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class T>
    struct is_monad;

  template <class T>
    constexpr bool is_monad_v = is_monad<T>::value;

namespace monad
{
  using namespace applicative;

  struct tag{};

  template <class M, class Enabler=void>
    struct traits
#if  ! defined JASEL_DOXYGEN_INVOKED
        : traits<M, meta::when<true>> {}
#endif
        ;

  // Default failing specialization
  template <typename MM, bool condition>
  struct traits<MM, meta::when<condition>>
  {
    // bind:: [T] x [T->[U]] -> [U]
    template <class M, class F>
      static auto bind(M&& x, F&& y) =delete;
  };

  template <class M, class F>
  auto
    bind(M&& x, F&& f)
      JASEL_NOEXCEPT_RETURN(
          traits<type_constructor_t<decay_t<M>>>::bind(forward<M>(x), forward<F>(f))
       )

}

  template <class T>
    struct is_monad
#if ! defined JASEL_DOXYGEN_INVOKED
        : is_base_of<monad::tag, monad::traits<T>> {}
#endif
        ;
  template <class T>
    struct is_monad<const T> : is_monad<T> {};
  template <class T>
    struct is_monad<volatile T> : is_monad<T> {};
  template <class T>
    struct is_monad<const volatile T> : is_monad<T> {};


}
}
}
#endif
#endif // header
