// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_NUMERIC_V1_CHARCONV_STRTO_HPP
#define JASEL_NUMERIC_V1_CHARCONV_STRTO_HPP

#include <experimental/contract.hpp>
#include <experimental/utility.hpp>
#include <cstdlib>

//! @file See https://fr.cppreference.com/w/cpp/string/byte/strtol

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

//!{ helper function to make generic strtoxx C functions depending on the result.
template <class T>
T strto(const char *str, char **str_end, int base = 10);

template <>
long strto<long>(const char *str, char **str_end, int base)
{
	return strtol(str, str_end, base);
}
template <>
long long strto<long long>(const char *str, char **str_end, int base)
{
	return strtoll(str, str_end, base);
}

template <>
unsigned long strto<unsigned long>(const char *str, char **str_end, int base)
{
	return strtoul(str, str_end, base);
}
template <>
unsigned long long strto<unsigned long long>(const char *str, char **str_end, int base)
{
	return strtoull(str, str_end, base);
}

//!}

#if !defined JASEL_DOXYGEN_INVOKED
namespace charsconv_detail
{
//! { helper traits mapping a type and the integral type used to do the parsing
template <class T, bool IsUnsigned>
struct strto_via_type_aux
{
	using type = unsigned long;
};

template <class T>
struct strto_via_type_aux<T, false>
{
	using type = long;
};
template <>
struct strto_via_type_aux<long long, false>
{
	using type = long long;
};
template <>
struct strto_via_type_aux<long long, true>
{
	using type = unsigned long long;
};

template <class T>
struct strto_via_type : strto_via_type_aux<T, std::is_unsigned<T>::value>
{
};

template <class T>
using strto_via_type_t = typename strto_via_type<T>::type;
//!}

// helper function
template <class T, class Via>
T strto_T_via(const char *str, char **str_end, int base)
{
	auto via = strto<Via>(str, str_end, base);
	if (*str_end == str)
		return 0;
	if (!can_narrow_to<T>(via))
	{
		*str_end = const_cast<char *>(str);
		return 0;
	}
	return narrow_cast<T>(via);
}

} // namespace charsconv_detail
#endif

template <class T>
T strto_via(const char *str, char **str_end, int base = 10)
{
	return strto<T>(str, str_end, base);
}

template <>
unsigned int strto_via<unsigned int>(const char *str, char **str_end, int base)
{
	return charsconv_detail::strto_T_via<unsigned int, unsigned long>(str, str_end, base);
}
template <>
unsigned short strto_via<unsigned short>(const char *str, char **str_end, int base)
{
	return charsconv_detail::strto_T_via<unsigned short, unsigned long>(str, str_end, base);
}
template <>
unsigned char strto_via<unsigned char>(const char *str, char **str_end, int base)
{
	return charsconv_detail::strto_T_via<unsigned char, unsigned long>(str, str_end, base);
}

template <>
signed int strto_via<signed int>(const char *str, char **str_end, int base)
{
	return charsconv_detail::strto_T_via<signed int, signed long>(str, str_end, base);
}
template <>
signed short strto_via<signed short>(const char *str, char **str_end, int base)
{
	return charsconv_detail::strto_T_via<signed short, signed long>(str, str_end, base);
}
template <>
signed char strto_via<signed char>(const char *str, char **str_end, int base)
{
	return charsconv_detail::strto_T_via<signed char, signed long>(str, str_end, base);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
