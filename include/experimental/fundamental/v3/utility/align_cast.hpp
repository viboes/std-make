//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_ALIGN_CAST_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_ALIGN_CAST_HPP

#include <experimental/contract.hpp>
#include <experimental/fundamental/v3/utility/discard.hpp>
#include <experimental/type_traits.hpp>
#include <cstdint>

// See also __builtin_assume_aligned
namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
template <size_t align>
// mandates align is a power of 2
bool check_aligned_to(void const *u)
{
	return (reinterpret_cast<uintptr_t>(u) % align) == 0u;
}
template <class T>
bool check_aligned_as(void const *u)
{
	return check_aligned_to<alignof(T)>(u);
}

//! This function returns its argument, and tells the compiler that it can assume that the returned pointer
//! is at least align bytes aligned.
//! See __builtin_assume_aligned. The major difference is that this fuction preserve the const
// The function cannot be yet constexpr and check the a contract. Use `check_aligned`
template <size_t align, class T>
JASEL_NODISCARD JASEL_ATTR_ASSUME_ALIGNED(align) JASEL_CXX14_CONSTEXPR T *assume_aligned(T *u)
{
#if 1
	discard(JASEL_ASSUME_ALIGNED(u, align));
#else
	JASEL_ASSUME(check_aligned<align>(u));
#endif
	return u;
}

//! This function returns its first argument reinterpret casted to `TPtr`,
//! Tells the compiler that it can assume that the returned pointer is at least aligned as pointee `T`.
// the difference with assume_aligned is that this function has a pre-condition,
// and so depending on the configuration the check could be done
//! Expects: the parameter `u` is at least aligned as the pointee `T` where `TPtr=T*`.
template <class T, class U>
T *JASEL_ATTR_ASSUME_ALIGNED(alignof(T)) aligned_as_cast(U *u)
{
	// The pointer must be aligned to the alignment of T
	JASEL_EXPECTS(check_aligned_as<T>(u));
	return reinterpret_cast<T *>(u);
}
template <class T, class U>
T const *JASEL_ATTR_ASSUME_ALIGNED(alignof(T)) aligned_as_cast(U const *u)
{
	// The pointer must be aligned to the alignment of T
	JASEL_EXPECTS(check_aligned_as<T>(u));
	return reinterpret_cast<T const *>(u);
}

template <size_t align, class U>
U *JASEL_ATTR_ASSUME_ALIGNED(align) aligned_to_cast(U *u)
{
	// The pointer must be aligned to the alignment of T
	JASEL_EXPECTS(check_aligned_to<align>(u));
	return u;
}

struct alignment_error : public exception
{
};

// This is to align_cast what narrow is to narrow_cast
template <class T, class U>
T *JASEL_ATTR_ASSUME_ALIGNED(alignof(T)) aligned_as(U *u)
{
	if (check_aligned_as<T>(u))
		return reinterpret_cast<T *>(u);
	throw alignment_error();
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
