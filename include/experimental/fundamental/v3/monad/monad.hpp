//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_MONAD_MONAD_HPP
#define JASEL_FUNDAMENTAL_V3_MONAD_MONAD_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////

#include <utility>
#include <type_traits>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/applicative.hpp>
#include <experimental/functional.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class T>
    struct is_monad;

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
    constexpr bool is_monad_v = is_monad<T>::value;
#endif

namespace monad
{
  using namespace applicative; // NOLINT google-build-using-namespace

namespace detail
{
struct not_a_monad_tag{};
}

  template <class M, class Enabler=void>
    struct traits
#if  ! defined JASEL_DOXYGEN_INVOKED
        : traits<M, meta::when<true>> {}
#endif
        ;

  // Default failing specialization
  template <typename MM, bool condition>
  struct traits<MM, meta::when<condition>> : detail::not_a_monad_tag
  {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
    // bind:: [T] x [T->[U]] -> [U]
    template <class M, class F>
      static auto bind(M&& x, F&& y) = delete;
    template <class M, class F>
      static auto unwrap(M&& x) = delete;

#endif
  };

  template <class M, class F>
  auto
    bind(M&& x, F&& f)
       JASEL_DECLTYPE_RETURN_NOEXCEPT(
          traits<type_constructor_t<decay_t<M>>>::bind(forward<M>(x), forward<F>(f))
       )

  template <class M>
  auto
   unwrap(M&& x)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
          traits<type_constructor_t<decay_t<M>>>::unwrap(forward<M>(x))
         //bind(forward<M>(x), identity{})
      )

  //! minimal complete definition based on transform
  struct mcd_bind
  {
    template <class M>
    static auto unwrap(M&& x)
         JASEL_DECLTYPE_RETURN_NOEXCEPT(
            monad::bind(forward<M>(x), identity{})
         )
  };

  //! minimal complete definition based on transform
  struct mcd_unwrap
  {
    template <class M, class F>
    auto
      bind(M&& x, F&& f)
         JASEL_DECLTYPE_RETURN_NOEXCEPT(
             monad::unwrap(functor::transform(forward<M>(x), forward<F>(f)))
         )
  };

}

  template <class T>
    struct is_monad
#if ! defined JASEL_DOXYGEN_INVOKED
            : integral_constant<bool,
                  ! is_base_of<monad::detail::not_a_monad_tag, monad::traits<T>>::value
                  && is_applicative<T>::value
              > {}
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
#endif // header
