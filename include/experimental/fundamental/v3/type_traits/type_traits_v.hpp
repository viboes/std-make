//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016-2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_TYPE_TRAITS_V_HPP
#define JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_TYPE_TRAITS_V_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

#if __cplusplus == 201402L || (__cplusplus > 201402L && defined __GNUC__ && !defined __clang__)
template <class R1, class R2>
JASEL_INLINE_VAR constexpr bool is_constructible_v = is_constructible<R1, R2>::value;

template <class R>
JASEL_INLINE_VAR constexpr bool is_copy_constructible_v = is_copy_constructible<R>::value;

template <class R>
JASEL_INLINE_VAR constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<R>::value;

template <class R1, class R2>
JASEL_INLINE_VAR constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<R1, R2>::value;

template <class R>
JASEL_INLINE_VAR constexpr bool is_const_v = is_const<R>::value;

template <class R1, class R2>
JASEL_INLINE_VAR constexpr bool is_convertible_v = is_convertible<R1, R2>::value;

template <class R>
JASEL_INLINE_VAR constexpr bool is_move_constructible_v = is_move_constructible<R>::value;

template <class R>
JASEL_INLINE_VAR constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<R>::value;

template <class R>
JASEL_INLINE_VAR constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<R>::value;

template <class T>
JASEL_INLINE_VAR constexpr bool is_pointer_v = is_pointer<T>::value;

template <class T>
JASEL_INLINE_VAR constexpr bool is_void_v = is_void<T>::value;

#endif

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
