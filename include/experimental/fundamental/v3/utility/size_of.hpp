//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_STD_UTILITY_SIZE_OF_HPP
#define JASEL_STD_UTILITY_SIZE_OF_HPP

#include <experimental/type_traits.hpp>
#include <cstddef>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

// Even if size_of_t is signed, all the values must be >= 0.
// This is due to avoid modulo arithmetic of C unsigned type size_t.
using size_of_t = common_type_t<ptrdiff_t, make_signed_t<size_t>>;

//! return the size in bytes of the object t as a signed integer type
template <class T>
constexpr size_of_t size_of(T const &t) noexcept
{
	return sizeof t;
}

//! return the size in bytes of the type T as a signed integer type
template <class T>
constexpr size_of_t size_of() noexcept
{
	return sizeof(T);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
