//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_MONAD_ERROR_MONAD_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_MONAD_ERROR_MONAD_ERROR_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////

#include <utility>
#include <type_traits>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/monad.hpp>

//! Usage
//! Intended usage
//! <code>
//! E e;
//! auto x = monad_error::make_error<expected>(e);
//! //decltype(x) is unexpected<E>
//!
//! <end_code>
//! <code>
//!
//! A1 a1;
//! A2 a2;
//! auto x = monad_error::make_error<expected, E>(a1, a2);
//! //decltype(x) is unexpected<E>
//!
// The problem is that type_constructor<expected<T,E>> is expected<_t,E>
// And we are unable to deduce from expected that it has 2 parameters
// Currently we have that
// type_constructor<expected> is expected<_t>
// We are lucky as we have that Error has a default parameter
//!
//! Other usage
//! <code>
//! E e;
//! auto x = monad_error::make_error<expected<_t>>(e);
//! //decltype(x) is unexpected<E>
//!
//! <end_code>
//! <code>
//!
//! A1 a1;
//! A2 a2;
//! auto x = monad_error::make_error<expected<_t, E>>(a1, a2);
//! //decltype(x) is unexpected<E>
//!

//! <end_code>


// Should we name this MonadError or MonadException

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class T>
    struct is_monad_error;

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
    constexpr bool is_monad_error_v = is_monad_error<T>::value;
#endif

namespace monad_error
{
  using namespace monad; // NOLINT google-build-using-namespace

namespace detail
{
    struct not_a_monad_error_tag{};
}

  template <class M, class Enabler=void>
    struct traits
#if  ! defined JASEL_DOXYGEN_INVOKED
        : traits<M, meta::when<true>> {}
#endif
        ;

  // Default failing specialization
  template <typename MM, bool condition>
  struct traits<MM, meta::when<condition>> : detail::not_a_monad_error_tag
  {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED

    // make_error<M>: E -> M
    template <class M, class ...Xs>
     constexpr
     auto make_error(Xs&& ...xs) = delete;

    // catch_error: [T]:E x (E->T) -> [T]:E
    // catch_error: [T]:E x (E->[T]:E) -> [T]:E
    template <class M, class F>
      static auto catch_error(M&& x, F&& y) = delete;

    // adapt_error: [T]:E x (E->G) -> [T]:G
    template <class M, class F>
      static auto adapt_error(M&& x, F&& y) = delete;
#endif
  };

  template <class M>
  struct error_type
  {
    using type = typename traits<M>::template error_type<M>;
  };

  template <class M>
  using error_type_t = typename error_type<M>::type;

  template <class M, class F>
  auto
  catch_error(M&& x, F&& f)
       JASEL_DECLTYPE_RETURN_NOEXCEPT(
          traits<type_constructor_t<decay_t<M>>>::catch_error(forward<M>(x), forward<F>(f))
       )

   template <class M, class F>
   auto
   adapt_error(M&& x, F&& f)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
           traits<type_constructor_t<decay_t<M>>>::adapt_error(forward<M>(x), forward<F>(f))
        )

  // make_error overload: requires a type constructor, , deduce the error type associated to the type constructor
  //template <class TC, int = 0, int..., class ...Xs>
  template <class TC, class ...Xs>
  constexpr auto
  make_error(Xs&& ...xs)
    JASEL_DECLTYPE_RETURN_NOEXCEPT(
      traits<type_constructor_t<TC>>::template make_error<TC>(std::forward<Xs>(xs)...)
    )

  // make_error overload: requires a template class, deduce the error type associated to the template
  //template <template <class ...> class TC, int = 0, int..., class ...Xs>
  template <template <class ...> class TC, class ...Xs>
  constexpr auto
  make_error(Xs&& ...xs)
    JASEL_DECLTYPE_RETURN_NOEXCEPT(
      traits<type_constructor_t<meta::quote<TC>>>::template make_error<type_constructor_t<meta::quote<TC>>>(std::forward<Xs>(xs)...)
    )

}

  template <class T>
    struct is_monad_error
#if ! defined JASEL_DOXYGEN_INVOKED
            : integral_constant<bool,
                  ! is_base_of<monad_error::detail::not_a_monad_error_tag, monad_error::traits<T>>::value
                  && is_monad<T>::value
              > {}
#endif
        ;
  template <class T>
    struct is_monad_error<const T> : is_monad_error<T> {};
  template <class T>
    struct is_monad_error<volatile T> : is_monad_error<T> {};
  template <class T>
    struct is_monad_error<const volatile T> : is_monad_error<T> {};


}
}
}
#endif // header
