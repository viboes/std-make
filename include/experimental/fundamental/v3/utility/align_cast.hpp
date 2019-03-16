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
bool check_aligned(void const *u)
{
	return (reinterpret_cast<uintptr_t>(u) % align) == 0u;
}

//! This function returns its first argument, and tells the compiler that it can assume that the returned pointer
//! is at least align bytes aligned.
//! See __builtin_assume_aligned. The major difference is that this fuction preserve the const
template <size_t align>
constexpr void const *assume_aligned(void const *u)
{
#if 1
	return __builtin_assume_aligned(u, align);
#else
	JASEL_ASSUME(check_aligned<align>(u));
	return u;
#endif
}

template <size_t align>
constexpr void *assume_aligned(void *u)
{
#if 1
	return __builtin_assume_aligned(u, align);
#else
	JASEL_ASSUME(check_aligned<align>(u));
	return u;
#endif
}

template <size_t align, uintptr_t offset>
constexpr void const *assume_aligned(void const *u)
{

#if 1
	return __builtin_assume_aligned(u, align, offset);
#else
	discard(assume_aligned<align>(reinterpret_cast<char const *>(u) - offset));
	return u;
#endif
}
template <size_t align, uintptr_t offset>
constexpr void *assume_aligned(void *u)
{

#if 1
	return __builtin_assume_aligned(u, align, offset);
#else
	discard(assume_aligned<align>(reinterpret_cast<char *>(u) - offset));
	return u;
#endif
}

//! This function returns its first argument reinterpret casted to `TPtr`,
//! and tells the compiler that it can assume that the returned pointer
//! is at least aligned as the pointee `T` where `TPtr=T*`.
//! See also: align_cast. The difference is that here we are impossing the behavior of the contract to assume
//! while in align_cast we left the behavior depend on the contract configuration
//! Use align_cast before so that you check it at runtime for a while and then move to assume_aligned_as.
template <class TPtr, class U>
constexpr TPtr assume_aligned_as(U *u)
{
	return reinterpret_cast<TPtr>(assume_aligned<alignof(remove_pointer_t<TPtr>)>(u));
}

template <class TPtr>
constexpr bool check_aligned_as(void const *u)
{
	return check_aligned<alignof(remove_pointer_t<TPtr>)>(u);
}
//! This function returns its first argument reinterpret casted to `TPtr`,
//! Expects: the parameter `u` is at least aligned as the pointee `T` where `TPtr=T*`.
template <class TPtr, class U>
TPtr aligned_as_cast(U *u)
{
	// The pointer must be aligned to the alignment of T
	JASEL_EXPECTS(check_aligned_as<TPtr>(u));
	return reinterpret_cast<TPtr>(u);
}

struct alignment_error : public exception
{
};

// This is to align_cast what narrow is to narrow_cast
template <class TPtr, class U>
TPtr aligned_as(U *u)
{
	if (check_aligned<TPtr>(u))
		return reinterpret_cast<TPtr>(u);
	throw alignment_error();
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
