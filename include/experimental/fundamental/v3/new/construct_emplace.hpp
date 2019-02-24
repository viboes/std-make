//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_NEW_CONSTRUCT_EMPLACE_HPP
#define JASEL_FUNDAMENTAL_V3_NEW_CONSTRUCT_EMPLACE_HPP

#include <new>     // nothrow
#include <utility> // forward

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <class T, class... Args>
T *construct_emplace(void *buf, Args &&... args)
{
	if (buf)
		return new (buf) T(forward<Args>(args)...);
	else
		throw bad_alloc();
}

template <class T, class... Args>
T *construct_emplace(nothrow_t, void *buf, Args &&... args)
{
	if (buf)
		return new (buf) T(forward<Args>(args)...);
	else
		return nullptr;
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
