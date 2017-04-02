//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_SWAPPABLE_SWAPPABLE_HPP
#define JASEL_FUNDAMENTAL_V3_SWAPPABLE_SWAPPABLE_HPP

#if (__cplusplus >= 201402L && defined JASEL_SUPPORT_SWAPPABLE) || defined JASEL_DOXYGEN_INVOKED

///////////////////////////////////////////////////////////////////////////////////////
///
/// In these definitions, let t denote an expression of type T, and let u denote an expression of type U.
/// An object t is swappable with an object u if and only if:
///
/// - the expressions std::experimental::swappable::swap(t, u) and std::experimental::swappable::swap(u, t) are valid, and
/// - these expressions have the following effects:
///   - the object referred to by t has the value originally held by u and
///   - the object referred to by u has the value originally held by t.
///
/// std::experimental::swappable::swap is defined as:
///
/// If is_adl_swappable<T,U> calls swap(a,b).
/// Otherwise, the expression swappable::traits<T,U>::swap(a,b) if it is a valid expression
/// Otherwise if it is MoveConstructible and MoveAssignable exchange
/// Otherwise if it is a c-array of swappables swap the parts
/// Otherwise it is not defined
///
/// is_adl_swappable<T,U> is true if the expression to swap(a,b) is a valid expression when evaluated in the following context:
/// @code
/// template <class T>
/// void swap(T&, T&) = delete;
/// template <class T, size_t N>
/// void swap(T(&)[N], T(&)[N]) = delete;
/// @endcode
///
/// std::experimental::swappable::traits<T,U> is specialized for types for which the expression
/// swap(t, u) is a valid when evaluated in the following context:
///////////////////////////////////////////////////////////////////////////////////////

#include <utility>
#include <experimental/type_traits.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class T, class U=T>
    struct is_swappable;

  template <class T, class U=T>
    constexpr bool is_swappable_v = is_swappable<T,U>::value;

  template<typename T, typename U = T>
    struct is_nothrow_swappable;

  template <class T, class U=T>
    constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T,U>::value;

namespace swappable {

  template <class R, class S, class Enabler=void>
    struct traits
#if ! defined JASEL_DOXYGEN_INVOKED
        : traits<R, S, meta::when<true>> {}
#endif
        ;

  // Default failing specialization
  template <typename R, typename S, bool condition>
  struct traits<R, S, meta::when<condition>>
  {
      template <class T, class U>
        static auto swap(T& x, U& y) =delete;
  };
}
namespace adl_swappable {

  template <class T> void swap(T&, T&) = delete;
  template <class T, std::size_t N> void swap(T(&)[N], T(&)[N]) = delete;

  template <class T, class U, typename = void>
    struct is_adl_swappable
      : false_type {};
  template <class T, class U>
    struct is_adl_swappable<T, U, void_t<decltype(swap((T&&)declval<T>(), (U&&)declval<U>())) > >
      : true_type {};

  template <class T, class U>
    constexpr bool is_adl_swappable_v = is_adl_swappable<T,U>::value;

  template <class T, class U>
    static auto apply_swap(T&& t, U&& u)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
        swap(std::forward<T>(t), std::forward<U>(u))
      )
}

namespace swappable {
  template <class T, class U=T>
    constexpr bool is_adl_swappable_v = adl_swappable::is_adl_swappable<T,U>::value;

  template <class T, class U=T, typename = void>
    struct is_trait_swappable
      : false_type {};
  template <class T, class U>
    struct is_trait_swappable<T, U, void_t< decltype( traits<T, U>::swap(declval<T>(),declval<U>()) ) > >
      : true_type {};

  template <class T, class U=T>
    constexpr bool is_trait_swappable_v = is_trait_swappable<T,U>::value;

#if 0
  // overload for adl swappables: for backward compatibility
  template <class T, class U>
    enable_if_t <
      is_adl_swappable_v<T,U>
    >
    swap(T&& t, U&& u)
    JASEL_NOEXCEPT_RETURN(
        adl_swappable::apply_swap(std::forward<T>(t), std::forward<U>(u))
    )

  // overload for swappable by trait: extension
  template <class T, class U>
    enable_if_t<
         ! is_adl_swappable_v<T,U>
      &&   is_trait_swappable_v<T,U>
    >
    swap(T&& x, U&& y)
      JASEL_NOEXCEPT_RETURN(
          traits<meta::uncvref_t<T>, meta::uncvref_t<U>>::swap(forward<T>(x),forward<U>(y))
       )

  // overload for move swappable: std
  template <class T>
    enable_if_t<
         ! is_adl_swappable_v<T>
      && ! is_trait_swappable_v<T>
      &&   is_move_constructible<T>::value
      &&   is_move_assignable<T>::value
    >
    swap(T& x, T& y)
      JASEL_NOEXCEPT_RETURN(
          (void)(y = ::std::exchange(x, ::std::move(y)))
      )

  // overload for c-arrays of swappables: std
  template <class T, std::size_t N>
    enable_if_t<
         ! is_adl_swappable_v<T (&)[N]>
      && ! is_trait_swappable_v<T (&)[N]>
      &&   ::std::experimental::is_swappable_v<T>
    >
    swap(T (&t)[N], T (&u)[N])
      noexcept(::std::experimental::is_nothrow_swappable_v<T>)
    {
      for (std::size_t i = 0; i < N; ++i)
      {
          ::std::experimental::swappable::swap(t[i], u[i]);
      }
    }
#else
  //! overload for swappable by trait: extension
  //! @par Effects:<br> Forward to swappable::traits
  template <class T, class U>
#if defined JASEL_DOXYGEN_INVOKED
  void swap(T&& x, U&& y);
#else
  enable_if_t<
    is_trait_swappable_v<T,U>
  >
  swap(T&& x, U&& y)
      JASEL_NOEXCEPT_RETURN(
          traits<meta::uncvref_t<T>, meta::uncvref_t<U>>::swap(forward<T>(x),forward<U>(y))
       )
#endif

  //! overload for swappable: std
  //! @par Equivalent to:<br> y = ::std::exchange(x, ::std::move(y))
  template <class T>
#if defined JASEL_DOXYGEN_INVOKED
void swap(T& x, T& y);
#else
   enable_if_t<
     not is_trait_swappable_v<T>
     and is_move_constructible<T>::value
     and is_move_assignable<T>::value
   >
   swap(T& x, T& y)
     JASEL_NOEXCEPT_RETURN(
         (void)(y = ::std::exchange(x, ::std::move(y)))
     )
#endif

  template <typename R, typename S>
  struct traits<R, S, meta::when<
   is_adl_swappable_v<R,S>
  >>
  {
     template <class T, class U>
       static auto swap(T& x, U& y)
       JASEL_NOEXCEPT_RETURN(
         adl_swappable::apply_swap(x, y)
       )
  };

  template <class R, std::size_t M>
  struct traits<R[M], R[M], meta::when<
    not is_adl_swappable_v<R[M],R[M]>
    and ::std::experimental::is_swappable_v<R>
  >>
  {
    template <class T, std::size_t N>
    static void swap(T (&t)[N], T (&u)[N])
      noexcept(::std::experimental::is_nothrow_swappable_v<T>)
    {
      for (std::size_t i = 0; i < N; ++i)
      {
          ::std::experimental::swappable::swap(t[i], u[i]);
      }
    }
  };

#endif
}

namespace swap_detail {
  template <class T, class U=T, typename = void>
    struct is_swappable
#if ! defined JASEL_DOXYGEN_INVOKED
        : is_swappable<T,U, meta::when<true>> {}
#endif
        ;
  template <class T, class U, bool B>
    struct is_swappable<T, U, meta::when<B>> : false_type {};

  template <class T, class U>
    struct is_swappable<T, U,
      meta::when<meta::is_valid<
        decltype(swappable::swap((T&&)(declval<T>()), (U&&)(declval<U>())))
      >>
    >: true_type {};

  template <class T, class U=T, typename = void>
    struct is_nothrow_swappable : false_type {};
  template <class T, class U>
    struct is_nothrow_swappable<T, U,
      meta::when<
        noexcept(swappable::swap((T&&)declval<T>(), (U&&)declval<U>()))
      >
    >: true_type {};

}

  template <class T, class U>
    struct is_swappable : integral_constant<bool,
         swap_detail::is_swappable<T>::value
      && swap_detail::is_swappable<U>::value
      && swap_detail::is_swappable<T,U>::value
      && swap_detail::is_swappable<U,T>::value
    >
    {};

  template <class T, class U>
    struct is_nothrow_swappable : integral_constant<bool,
         swap_detail::is_nothrow_swappable<T>::value
      && swap_detail::is_nothrow_swappable<U>::value
      && swap_detail::is_nothrow_swappable<T,U>::value
      && swap_detail::is_nothrow_swappable<U,T>::value
    >
    {};

}}
}
#endif
#endif // header
