//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_PRODUCT_TYPE_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_PRODUCT_TYPE_HPP

#if __cplusplus >= 201402L

///////////////////////////////////////////////////////////////////////////////////////
///
/// A type PT is a model of a ProductType if given a variables pt of type PT
///
///   product_type::size<PT>::value
///   product_type::element<I,PT>::type
///   product_type::get<I>(pt)
///
/// The definition of these traits/functions depend on whether P0327 is adopted by the standard and
/// can be implemented only by the compiler, as it does for Range-based for loops.
///
/// What follows is just an emulation for the cases 1 (c-arrays) and 2 (tuple-like types).
/// The case 3 can be implemented only by the compiler.
///////////////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <utility>
#include <tuple>
#include <type_traits>
#include <experimental/meta.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  // this should be moved away (meta?)
  template <class ... Ts>
  void swallow(Ts&&...) {  }


namespace detail
{
  template <class TypeList>
  struct conjuntion_aux;
  template <>
  struct conjuntion_aux <meta::types<>> : true_type {  };
  template <class C1>
  struct conjuntion_aux <meta::types<C1>> : C1 {};
  template <class C1, class C2, class ...Cs>
  struct conjuntion_aux <meta::types<C1,C2,Cs...>> :
    conjuntion_aux<
      meta::types<
        integral_constant<bool, C1::value&&C2::value>,
        Cs...
      >
    > {  };

  template <class ...Cnds>
  struct conjuntion : conjuntion_aux<meta::types<Cnds...>>{};

  template <class ...Cnds>
  constexpr bool conjuntion_v = conjuntion<Cnds...>::value;

  template <class T>
  struct has_tuple_like_size_access
  {
      struct yes{ char a[1]; };
      struct no { char a[2]; };

      template<class U> static yes test(decltype(std::tuple_size<U>::value)*);
      template<class U> static no test(...);
      static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
  };

  template <size_t N, class T>
  struct has_tuple_like_element_access
  {
      struct yes{ char a[1]; };
      struct no { char a[2]; };

      template <size_t I, class U> static yes test(typename std::tuple_element<I,U>::type*);
      template <size_t I, class U> static no test(...);
      static const bool value = sizeof(test<N, T>(nullptr)) == sizeof(yes);
  };
  namespace get_adl {
    using std::get;
    template <size_t N, class T>
    auto xget(T&& t) noexcept -> decltype( get<N>(forward<T>(t)) )
    {
      return get<N>(forward<T>(t));
    }
  }
  template<size_t N, class T>
  struct has_tuple_like_get_access
  {
      struct yes{ char a[1]; };
      struct no { char a[2]; };

      template <size_t I, class U>
          static yes test(remove_reference_t<decltype(detail::get_adl::xget<I>(declval<U>()))>*);
      template <size_t I, class U>
          static no test(...);
      static constexpr bool value = sizeof(test<N,T>(nullptr)) == sizeof(yes);
  };
  template <size_t I, size_t N, class T>
  struct has_tuple_like_get_access<I, T [N]> : false_type {};

  template <class T, class  Indexes>
  struct has_tuple_like_element_get_access_aux;
#if defined JASEL_HAS_FOLD_EXPRESSIONS
  template <class T, size_t ...N>
  struct has_tuple_like_element_get_access_aux<T, index_sequence<N...>> : integral_constant<bool,
    (has_tuple_like_element_access<N, T>::value && ...)
    &&
    (has_tuple_like_get_access<N, T>::value && ...)
    > {};
#else
  template <class T, size_t ...N>
  struct has_tuple_like_element_get_access_aux<T, index_sequence<N...>> : detail::conjuntion<
    has_tuple_like_element_access<N, T> ...
    ,
    has_tuple_like_get_access<N, T> ...
    > {};
#endif

  template <class T, bool B>
  struct has_tuple_like_element_get_access:
      has_tuple_like_element_get_access_aux<T, make_index_sequence<tuple_size<T>::value>>
  {};
  template <class T>
  struct has_tuple_like_element_get_access<T, false>: false_type {};

}

  template <class T>
  struct has_tuple_like_access : integral_constant<bool,
    detail::has_tuple_like_size_access<T>::value &&
    detail::has_tuple_like_element_get_access<T, detail::has_tuple_like_size_access<T>::value>::value
    > {};

  template <size_t N, class T>
  struct has_tuple_like_access<T [N]> : false_type {};
  template <size_t N, class T>
  struct has_tuple_like_access<T (&)[N]> : false_type {};

  template <class PT, class Enabler=void>
  struct product_type_traits : product_type_traits<PT, meta::when<true>> {};

  // Default failing specialization
  template <class  PT, bool condition>
  struct product_type_traits<PT, meta::when<condition>>
  {
      template <class T>
        static constexpr auto get(T&& x) =delete;
  };


  struct product_type_tag{};

  // Forward to customized class using tuple-like access
  template <class PT>
  struct product_type_traits
  <  PT, meta::when< has_tuple_like_access<PT>::value >  > : product_type_tag
  {
    using size = tuple_size<PT>;

    template <size_t I>
    using element = tuple_element<I, PT>;

    template <size_t I, class PT2, class= std::enable_if_t< I < size::value > >
      static constexpr decltype(auto) get(PT2&& pt) noexcept
      {
        return detail::get_adl::xget<I>(forward<PT2>(pt));
      }
  };

  // customization for C-arrays
  template <class T, size_t N>
  struct product_type_traits<T [N]> : product_type_tag
  {
    using size = integral_constant<size_t, N>;
    template <size_t I>
    struct element { using type = T; };

    template <size_t I, class U, size_t M, class= std::enable_if_t< I<N >>
        static constexpr U& get(U (&arr)[M]) noexcept { return arr[I]; }
  };

  namespace product_type {
    template <class T>
      using traits = product_type_traits<T>;

    template <class PT>
    struct size : traits<PT>::size {};
    template <class T> struct size<const T> : size<T> {};
    template <class T> struct size<volatile T> : size<T> {};
    template <class T> struct size<const volatile T> : size<T> {};
    template <class PT>
    constexpr size_t size_v = size<PT>::value;

    template <size_t I, class PT, class= std::enable_if_t< I<size_v<PT> >>
    struct element : traits<PT>::template element<I> {};
    template <size_t I, class PT>
    using element_t = typename element<I,PT>::type;
    template <size_t I, class T> struct element<I, const T> { using type = const element_t<I, T>; };
    template <size_t I, class T> struct element<I, volatile T> { using type = volatile element_t<I, T>; };
    template <size_t I, class T> struct element<I, const volatile T> { using type = const volatile element_t<I, T>; };

    template <size_t I, class PT
      , class= std::enable_if_t< I < size_v<remove_cv_t<remove_reference_t<PT>>> >
    >
    constexpr decltype(auto) get(PT && pt) noexcept
    {
        return traits<remove_cv_t<remove_reference_t<PT>>>::template get<I>(forward<PT>(pt));
    }
    template <class PT>
    using element_sequence_for = make_index_sequence<product_type::size_v<remove_cv_t<remove_reference_t<PT>>>>;

  }

  // fixme redefine it as we did for has_tuple_like_access
  template <class T>
  struct is_product_type : is_base_of<product_type_tag, product_type_traits<T>> {};
  template <class T>
  struct is_product_type<const T> : is_product_type<T> {};
  template <class T>
  struct is_product_type<volatile T> : is_product_type<T> {};
  template <class T>
  struct is_product_type<const volatile T> : is_product_type<T> {};
  template <class T>
  constexpr bool is_product_type_v = is_product_type<T>::value ;

  // fixme: Is this needed? Do we want that is_product_type<T(&)[N]>::value to be true?
  //template <class T, size_t N>
  //struct is_product_type<T(&)[N]> : true_type {};


}}
}
#endif
#endif // header
