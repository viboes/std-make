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
#include <functional> // std::invoke
#include <experimental/functional.hpp>

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
namespace detail
{
  struct not_a_functor_tag{};
}

  //! This class must be specialized by the user
  //! @tparam F the Functor to specialize
  //! @tparam Enabler SFINAE enabler condition
  //! By default ot forwards to thesame template specialized with `when<true>`.
  template <class F, class Enabler=void>
    struct traits
#if  ! defined JASEL_DOXYGEN_INVOKED
        : traits<F, meta::when<true>> {}
#endif
        ;

  //! Default failing specialization
  //! It inherits from the functor::detail::not_a_functor_tag and deletes any signature to make the code more explicit
  template <typename U, bool condition>
  struct traits<U, meta::when<condition>> : detail::not_a_functor_tag
  {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
      template <class T, class F>
        static auto transform(T&& x, F&& y) = delete;

      template <class Functor_T, class Pred_T, class Callable_T_U>
        static auto adjust_if(Functor_T && xs, Pred_T  p, Callable_T_U  f) = delete;
#endif
  };

  //! Applies the `Callable` `F` over the `Functor` `T`
  //! @tparam T the Functor
  //! @tparam F the callable map to apply to the elements
  //! @par Equivalent To
  //!   Forward to in the customization point
  template <class T, class F>
  auto
    transform(T&& x, F&& f)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
            traits<type_constructor_t<meta::uncvref_t<T>>>::transform(forward<T>(x),forward<F>(f))
        )

  //! Applies the `Callable` `F` over the `Functor` `T`. This the equivalent to transform but the argument are flip.
  //! @tparam T the Functor
  //! @tparam F the Callable map to apply to the elements
  //! @par Equivalent To
  //!   transform(x,f)
  template <class F, class T>
   auto
     map(F&& f, T&& x)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
            functor::transform(forward<T>(x), forward<F>(f))
        )



  //! Applies the `Callable_T_U` `f` over the elements of `Functor_T` `xs` satisfying the predicate `Pred_T` `p`.
  //! @tparam Functor_T the Functor [T]
  //! @tparam Pred_T the Predicate<T>
  //! @tparam Callable_T_U the Callable map to apply to the elements
  //! @par Equivalent To
  //!   Forward to in the customization point
  //! @par Remark
  //!   This function shouldn't participate in overload resolution until `T` be convertible to `U`.
  // fixme Should `U` be convertible to `T` instead and return a Functor_T instead of a Functor_U?

  template <class Functor_T, class Pred_T, class Callable_T_U>
    auto adjust_if(Functor_T && xs, Pred_T  p, Callable_T_U  f)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
            traits<type_constructor_t<meta::uncvref_t<Functor_T>>>::adjust_if(forward<Functor_T>(xs),p, f)
        )

#if ! defined JASEL_DOXYGEN_INVOKED
  namespace detail {

    template <class Pred_T, class Callable_T_U>
    struct adjust_if_helper
    {
      Pred_T pred;
      Callable_T_U fct;
      template <class T>
      auto operator()(T && x) const -> decltype(this->fct(forward<T>(x)))
      {
        return (JASEL_INVOKE(pred,forward<T>(x)))
          ? JASEL_INVOKE(fct,forward<T>(x))
          : decltype(this->fct(forward<T>(x)))(forward<T>(x))
          ;
      }
    };
  }
#endif

  //! minimal complete definition based on transform
  struct mcd_transform
  {
    //! Applies the `Callable_T_U` `f` over the elements of `Functor_T` `xs` satisfying the predicate `Pred_T` `p`.
    //! @tparam Functor_T the Functor [T]
    //! @tparam Pred_T the Predicate<T>
    //! @tparam Callable_T_U the Callable map to apply to the elements
    //! @par Equivalent To
    //!   transform(x, [&](auto x) { if pred(x) then return f(x) else return x; })
    //! @par Remark
    //!   This function shouldn't participate in overload resolution until `T` be convertible to `U`.
    // fixme Should `U` be convertible to `T` instead and return a Functor_T instead of a Functor_U?

    template <class Functor_T, class Pred_T, class Callable_T_U>
    static auto adjust_if(Functor_T && xs, Pred_T  p, Callable_T_U  f)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
          functor::transform(forward<Functor_T>(xs), detail::adjust_if_helper<Pred_T, Callable_T_U>{p, f})
      )
  };
  //! minimal complete definition based on adjust_if
  struct mcd_adjust_if
  {
    template <class Functor_T, class Callable_T_U>
    static auto transform(Functor_T && xs, Callable_T_U  f)
      JASEL_DECLTYPE_RETURN_NOEXCEPT(
          functor::adjust_if(forward<Functor_T>(xs), always<bool>(true), f)
      )
  };
}

  template <class T>
    struct is_functor
#if ! defined JASEL_DOXYGEN_INVOKED
        : integral_constant<bool,
              ! is_base_of<functor::detail::not_a_functor_tag, functor::traits<T>>::value
          > {}
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
