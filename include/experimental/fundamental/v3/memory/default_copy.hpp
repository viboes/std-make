//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_EXPERIMENTAL_FUNDAMENTAL_MEMORY_DEFAULT_COPY_HPP
#define JASEL_EXPERIMENTAL_FUNDAMENTAL_MEMORY_DEFAULT_COPY_HPP

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1007r3.pdf

#include <experimental/fundamental/v2/config.hpp>

//#if __cplusplus <= 201703L

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// [ptr.aligned], pointer alignment hint
template <class T>
struct default_copy
{
	T *operator()(const T &t) const
	{
		return new T(t);
	}
};
} // namespace fundamental_v3
} // namespace experimental
} // namespace std

//#endif

#endif // header
