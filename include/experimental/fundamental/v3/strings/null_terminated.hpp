// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_STRINGS_NULL_TERMINATED_HPP
#define JASEL_EXPERIMENTAL_STRINGS_NULL_TERMINATED_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/utility/length_tag.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

struct null_terminated_t
{
};
#if __cplusplus > 201402L
inline constexpr null_terminated_t null_terminated;
#endif

// struct length_tag_t
// {
// };
// #if __cplusplus > 201402L
// inline constexpr length_tag_t length_tag;
// #endif

template <class CharT>
struct null_terminated_traits
{
	//! the zero used to identify a null terminated string
	static const CharT zero;
};
template <class CharT>
const CharT null_terminated_traits<CharT>::zero = CharT();

// Null terminated byte/wise strings functions
namespace ntxs
{

//! checks if all the chars in the range [0,len) are non zero as required for a ntxs of length len
template <class CharT>
JASEL_CXX14_CONSTEXPR bool all_non_zero(const CharT *str, size_t len) noexcept
{
	for (size_t i = 0; i < len; ++i)
	{
		if (CharT() == str[i])
			return false;
	}
	return true;
}
//! checks if str is  a valid NTXS, that is all chars until len are non zero and the len char is zero
template <class CharT>
JASEL_CXX14_CONSTEXPR bool valid(const CharT *str, size_t len) noexcept
{
	return CharT() == str[len] && all_non_zero(str, len);
}

//! this function calculate the length of a NTXS and checks that the length is less than a specific capacity N
template <size_t N, class CharT>
JASEL_CXX14_CONSTEXPR bool test_and_set_length(const CharT *str, size_t &length) noexcept
{
	for (length = 0; length < N; ++length)
	{
		if (CharT() == str[length])
			return true;
	}
	return false;
}
} // namespace ntxs

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
