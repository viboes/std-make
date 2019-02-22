//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

// see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0627r3.pdf

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_UNREACHABLE_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_UNREACHABLE_HPP

#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

//! see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0627r3.pdf
inline JASEL_ATTR_NORETURN void unreachable()
{
	__builtin_unreachable();
}

inline JASEL_ATTR_NORETURN void unreachable(const char *)
{
	__builtin_unreachable();
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
