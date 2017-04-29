// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2013,2014,2017 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_UTILITY_CONSTEXPR_UTILITY_HPP
#define JASEL_EXPERIMENTAL_V3_UTILITY_CONSTEXPR_UTILITY_HPP

#include <utility>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

// workaround: std utility functions aren't constexpr in ???
template <class T> inline
constexpr T&& constexpr_forward(typename remove_reference<T>::type& t) noexcept
{
  return static_cast<T&&>(t);
}

template <class T> inline
constexpr T&& constexpr_forward(typename remove_reference<T>::type&& t) noexcept
{
    static_assert(!is_lvalue_reference<T>::value, "!!");
    return static_cast<T&&>(t);
}

template <class T> inline
constexpr typename remove_reference<T>::type&& constexpr_move(T&& t) noexcept
{
    return static_cast<typename remove_reference<T>::type&&>(t);
}

#if 0
template<class T> inline
constexpr T * constexpr_addressof(T& Val)
{
  return ((T *) &(char&)Val);
}
#endif

}}
} // namespace


#endif // JASEL_EXPERIMENTAL_V3_EXPECTED_DETAIL_CONSTEXPR_UTILITY_HPP
