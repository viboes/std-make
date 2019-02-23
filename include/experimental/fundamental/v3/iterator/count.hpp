//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_ITERATOR_COUNT_HPP
#define JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_ITERATOR_COUNT_HPP

#include <experimental/fundamental/v3/utility/size_of.hpp>
#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

//! return the number of elements of a container using the signed integer type size_of_t
template <class C>
constexpr auto elements_count(const C &c) -> common_type_t<ptrdiff_t, make_signed_t<decltype(c.size())>>
{
	return static_cast<common_type_t<ptrdiff_t, make_signed_t<decltype(c.size())>>>(c.size());
}
template <class T, size_of_t N>
constexpr size_of_t elements_count(const T (&array)[N]) noexcept
{
	return N;
}

template <class C>
constexpr auto ssize(const C &c) -> common_type_t<ptrdiff_t, make_signed_t<decltype(c.size())>>
{
	return static_cast<common_type_t<ptrdiff_t, make_signed_t<decltype(c.size())>>>(c.size());
}

template <class T, size_of_t N>
constexpr size_of_t ssize(const T (&array)[N]) noexcept
{
	return N;
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
