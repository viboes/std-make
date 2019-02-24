//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

// see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1357r0.pdf
// see http://wiki.edg.com/pub/Wg21kona2019/StrawPolls/p1357r1_array_traits.pdf

#ifndef JASEL_STD_ARRAY_TRAITS_HPP
#define JASEL_STD_ARRAY_TRAITS_HPP

// fixme: adapt as soon as this trait is implemented
#if __cplusplus <= 202303L

#include <type_traits>

namespace std
{

template <typename>
struct is_bounded_array : false_type
{
};
template <typename U, size_t N>
struct is_bounded_array<U[N]> : true_type
{
};

#if __cplusplus > 201402L
template <typename T>
inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;
#endif

template <typename>
struct is_unbounded_array : false_type
{
};
template <typename U>
struct is_unbounded_array<U[]> : true_type
{
};

#if __cplusplus > 201402L
template <typename T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;
#endif

} // namespace std

#endif
#endif // header
