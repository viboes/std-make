// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_STRINGS_NULL_TERMINATED_HPP
#define JASEL_EXPERIMENTAL_STRINGS_NULL_TERMINATED_HPP

#include <experimental/fundamental/v2/config.hpp>

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

template <class charT>
struct null_terminated_traits
{
	//! the zero used to identify a null terminated string
	static const charT zero;
};
template <class charT>
const charT null_terminated_traits<charT>::zero = charT();

// Null terminated byte/wise strings functions
namespace ntxs
{

//! checks if all the chars in the range [0,len) are non zero as required for a ntxs of length len
template <class charT>
JASEL_CXX14_CONSTEXPR bool all_non_zero(const charT *str, size_t len) noexcept
{
	for (int i = 0; i < len; ++i)
	{
		if (charT() == str[i])
			return false;
	}
	return true;
}
//! checks if str is  a valid NTXS, that is all chars until len are non zero and the len char is zero
template <class charT>
JASEL_CXX14_CONSTEXPR bool valid(const charT *str, size_t len) noexcept
{
	return charT() == str[len] && all_non_zero(str, len);
}

//! this function calculate the length of a NTXS a check that the length is less than a specific capacity N
template <size_t N, class charT>
JASEL_CXX14_CONSTEXPR bool test_and_set_length(const charT *str, size_t &length) noexcept
{
	for (length = 0; length < N; ++length)
	{
		if (charT() == str[length])
			return true;
	}
	return false;
}
} // namespace ntxs

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
