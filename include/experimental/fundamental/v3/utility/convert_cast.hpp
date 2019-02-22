
//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_CONVERT_CAST_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_CONVERT_CAST_HPP

#include <utility> // forward, enable_if

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// The question is if we should use T(u) or T{u} instead of (T)u or static_cast<T>(u) when T is the name of a class.
// While T(u) or T{u} looks more like C++, it is not easy to grep neither.
// static_cast can be used for more things than for converting between types.
// Use convert_cast, when you want to state clearly that you are calling to the explicit convertion constructor or conversion operatot.
template <class T, class U, typename = typename enable_if<!is_pointer<T>::value>::type>
T convert_cast(U &&u)
{
	return static_cast<T>(std::forward<U>(u));
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
