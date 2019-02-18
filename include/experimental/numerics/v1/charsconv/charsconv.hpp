// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_NULMERIC_V1_CHARCONV_HPP
#define JASEL_NULMERIC_V1_CHARCONV_HPP

#include <experimental/contract.hpp>
#include <experimental/utility.hpp>
#include <cstdlib>
#include <system_error>

#include <experimental/numerics/v1/charsconv/strto.hpp>

//! @file See https://en.cppreference.com/w/cpp/header/charconv
//! Limited to
//! * integral types
//! * deviates from C++17 standard <charconv> as takes snprintf and strtoxx semantics instead (for easy of implementation)
//! * to_chars base can only be decimal, hexadecimal or octal
//! Extended with to_chars _fmt functions that take a format as parameter that has a single %
//! Extended with to_chars _noerr suffix functions that have a precondition that the conversion is valid
//! todo: take in account floating point types

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

//! result of to_chars function
struct JASEL_NODISCARD to_chars_result
{
	char *    ptr;
	std::errc ec;
};

//! result of to_chars function
struct JASEL_NODISCARD from_chars_result
{
	const char *ptr;
	std::errc   ec;
};

//! as std::to_char but taking a formatting c-string
template <class T>
to_chars_result to_chars_fmt(char *first, char *last,
                             const char *fmt, T value)
{
	int sz = std::snprintf(first, last - first + 1, fmt, value);
	if (sz < 0 || (sz > last - first))
		return to_chars_result{last, std::errc::value_too_large};
	return to_chars_result{first + sz, {}};
}

//! as std::to_char but taking a formatting c-string and having a pre-condition everything is ok. No checks
template <class T>
char *to_chars_fmt_noerr(char *first, char *last,
                         const char *fmt, T value)
{
	int sz = std::snprintf(first, last - first + 1, fmt, value);
	JASEL_ASSERT(sz >= 0 || (sz <= last - first));
	return first + sz;
}

#if !defined JASEL_DOXYGEN_INVOKED
namespace charsconv_detail
{
//!{ format associated to a T type using a specific base
inline const char *fmt(char, int base)
{
	return (10 == base) ? "%c" : (16 == base) ? "%c" : (8 == base) ? "%c" : "%c";
}

inline const char *fmt(signed char, int base)
{
	return (10 == base) ? "%dhh" : (16 == base) ? "%xhh" : (8 == base) ? "%ohh" : "%dhh";
}

inline const char *fmt(unsigned char, int base)
{
	return (10 == base) ? "%duhh" : (16 == base) ? "%xuhh" : (8 == base) ? "%ouhh" : "%duhh";
}

inline const char *fmt(short, int base)
{
	return (10 == base) ? "%dh" : (16 == base) ? "%xuh" : (8 == base) ? "%oh" : "%dh";
}

inline const char *fmt(unsigned short, int base)
{
	return (10 == base) ? "%duh" : (16 == base) ? "%xuh" : (8 == base) ? "%ouh" : "%duh";
}
inline const char *fmt(int, int base)
{
	return (10 == base) ? "%d" : (16 == base) ? "%x" : (8 == base) ? "%o" : "%d";
}
inline const char *fmt(unsigned int, int base)
{
	return (10 == base) ? "%du" : (16 == base) ? "%xu" : (8 == base) ? "%ou" : "%du";
}
inline const char *fmt(long, int base)
{
	return (10 == base) ? "%dl" : (16 == base) ? "%xl" : (8 == base) ? "%ol" : "%dl";
}

inline const char *fmt(unsigned long, int base)
{
	return (10 == base) ? "%dul" : (16 == base) ? "%xul" : (8 == base) ? "%oul" : "%dul";
}

inline const char *fmt(long long, int base)
{
	return (10 == base) ? "%dll" : (16 == base) ? "%xll" : (8 == base) ? "%oll" : "%dll";
}
inline const char *fmt(unsigned long long, int base)
{
	return (10 == base) ? "%dull" : (16 == base) ? "%xull" : (8 == base) ? "%oull" : "%dull";
}
//!}
} // namespace charsconv_detail
#endif

//! to_chars as specific in the standard
template <class T>
inline to_chars_result to_chars(char *first, char *last,
                                T value, int base = 10)
{
	return to_chars_fmt(first, last, charsconv_detail::fmt(value, base), value);
}

//! to_chars as specific in the standard
template <class T>
inline char *to_chars_noerr(char *first, char *last,
                            T value, int base = 10)
{
	return to_chars_fmt_noerr(first, last, charsconv_detail::fmt(value, base), value);
}

#if !defined JASEL_DOXYGEN_INVOKED
namespace charsconv_detail
{
//! helper from_chars function stating the type Via used to do the parsing.
//! This is needed as the C function doesn't have strtoxx functions for all the types.
template <class Via, class T>
from_chars_result from_chars_via(const char *first, const char *last,
                                 T &value, int base = 10)
{
	char *tmp;
	Via   via = strto<Via>(first, &tmp, base);
	if (tmp == first)
	{
		if (ERANGE == errno)
		{
			if (via == 0)
				return from_chars_result{first, std::errc::invalid_argument};
			else
				return from_chars_result{first, std::errc::result_out_of_range};
		}
		return from_chars_result{first, std::errc::invalid_argument};
	}
	if (tmp > last)
		return from_chars_result{first, std::errc::invalid_argument};
	if (!can_narrow_to<T>(via))
		return from_chars_result{first, std::errc::result_out_of_range};
	value = static_cast<T>(via);
	return from_chars_result{tmp, std::errc{}};
}

template <class T>
from_chars_result from_chars_direct(const char *first, const char *last,
                                    T &value, int base = 10)
{
	char *tmp;

	value = strto<T>(first, &tmp, base);
	if (tmp == first)
	{
		if (ERANGE == errno)
		{
			if (value == 0)
				return from_chars_result{first, std::errc::invalid_argument};
			else
				return from_chars_result{first, std::errc::result_out_of_range};
		}
		return from_chars_result{first, std::errc::invalid_argument};
	}
	if (tmp > last)
		return from_chars_result{first, std::errc::invalid_argument};
	return from_chars_result{tmp, std::errc{}};
}

template <class Via, class T>
const char *from_chars_noerr_via(const char *first, const char *last,
                                 T &value, int base = 10) noexcept
{
	char *tmp;
	Via   l = strto<Via>(first, &tmp, base);
	JASEL_ASSERT(tmp != first && "invalid_argument");
	JASEL_ASSERT(tmp <= last && "invalid_argument");
	JASEL_ASSERT(!(ERANGE == errno && l == 0) && "invalid_argument");
	JASEL_ASSERT(!(ERANGE == errno && l != 0) && "result_out_of_range");
	JASEL_ASSERT(can_narrow_to<T>(l) && "result_out_of_range");
	value = static_cast<T>(l);
	return tmp;
}

template <class T>
const char *from_chars_noerr_direct(const char *first, const char *last,
                                    T &value, int base = 10) noexcept
{
	char *tmp;
	value = strto<T>(first, &tmp, base);
	JASEL_ASSERT(tmp != first && "invalid_argument");
	JASEL_ASSERT(tmp <= last && "invalid_argument");
	JASEL_ASSERT(!(ERANGE == errno && value == 0) && "invalid_argument");
	JASEL_ASSERT(!(ERANGE == errno && value != 0) && "result_out_of_range");
	return tmp;
}
} // namespace charsconv_detail
#endif

//! parse from the string [first, last) an integral type T using a specific base
//! if there is an error returns it in the result
inline from_chars_result from_chars(const char *first, const char *last,
                                    unsigned char &value, int base = 10)
{
	return charsconv_detail::from_chars_via<charsconv_detail::strto_via_type_t<unsigned char>>(first, last, value, base);
}
inline from_chars_result from_chars(const char *first, const char *last,
                                    unsigned short &value, int base = 10)
{
	return charsconv_detail::from_chars_via<charsconv_detail::strto_via_type_t<unsigned short>>(first, last, value, base);
}
inline from_chars_result from_chars(const char *first, const char *last,
                                    unsigned int &value, int base = 10)
{
	return charsconv_detail::from_chars_via<charsconv_detail::strto_via_type_t<unsigned int>>(first, last, value, base);
}
inline from_chars_result from_chars(const char *first, const char *last,
                                    unsigned long &value, int base = 10)
{
	return charsconv_detail::from_chars_direct(first, last, value, base);
}
inline from_chars_result from_chars(const char *first, const char *last,
                                    unsigned long long &value, int base = 10)
{
	return charsconv_detail::from_chars_direct(first, last, value, base);
}

inline from_chars_result from_chars(const char *first, const char *last,
                                    signed char &value, int base = 10)
{
	return charsconv_detail::from_chars_via<charsconv_detail::strto_via_type_t<signed char>>(first, last, value, base);
}
inline from_chars_result from_chars(const char *first, const char *last,
                                    signed short &value, int base = 10)
{
	return charsconv_detail::from_chars_via<charsconv_detail::strto_via_type_t<signed short>>(first, last, value, base);
}
inline from_chars_result from_chars(const char *first, const char *last,
                                    signed int &value, int base = 10)
{
	return charsconv_detail::from_chars_via<charsconv_detail::strto_via_type_t<signed int>>(first, last, value, base);
}
inline from_chars_result from_chars(const char *first, const char *last,
                                    signed long &value, int base = 10)
{
	return charsconv_detail::from_chars_direct(first, last, value, base);
}
inline from_chars_result from_chars(const char *first, const char *last,
                                    signed long long &value, int base = 10)
{
	return charsconv_detail::from_chars_direct(first, last, value, base);
}

//! parse from the string [first, last) an integral type T using a specific base
//! The pre-condition is that there is such a T type, and so no error needs to be reported
inline const char *from_chars_noerr(const char *first, const char *last,
                                    unsigned char &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_via<unsigned long>(first, last, value, base);
}
inline const char *from_chars_noerr(const char *first, const char *last,
                                    unsigned short &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_via<unsigned long>(first, last, value, base);
}
inline const char *from_chars_noerr(const char *first, const char *last,
                                    unsigned int &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_via<unsigned long>(first, last, value, base);
}
inline const char *from_chars_noerr(const char *first, const char *last,
                                    unsigned long &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_direct(first, last, value, base);
}
inline const char *from_chars_noerr(const char *first, const char *last,
                                    unsigned long long &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_direct(first, last, value, base);
}

inline const char *from_chars_noerr(const char *first, const char *last,
                                    signed char &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_via<signed long>(first, last, value, base);
}
inline const char *from_chars_noerr(const char *first, const char *last,
                                    signed short &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_via<signed long>(first, last, value, base);
}
inline const char *from_chars_noerr(const char *first, const char *last,
                                    signed int &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_via<signed long>(first, last, value, base);
}
inline const char *from_chars_noerr(const char *first, const char *last,
                                    signed long &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_direct(first, last, value, base);
}
inline const char *from_chars_noerr(const char *first, const char *last,
                                    signed long long &value, int base = 10)
{
	return charsconv_detail::from_chars_noerr_direct(first, last, value, base);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
