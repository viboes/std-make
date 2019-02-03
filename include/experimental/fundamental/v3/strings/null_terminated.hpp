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
	static const charT zero;
};
template <class charT>
const charT null_terminated_traits<charT>::zero = charT();

template <class charT>
JASEL_CXX14_CONSTEXPR bool all_non_zero(const charT *str, size_t end)
{
	for (int i = 0; i < end; ++i)
	{
		if (charT() == str[i])
			return false;
	}
	return true;
}
template <class charT>
JASEL_CXX14_CONSTEXPR bool valid_ntbs(const charT *str, size_t len)
{
	return charT() == *(str + len) && all_non_zero(str, len);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
