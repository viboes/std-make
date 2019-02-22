//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_EXPLICIT_CAST_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_EXPLICIT_CAST_HPP

#include <utility> // forward

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// As for  c-style cast use explicit_cast only when there is no a more explicit cast,
// as const_cast, static_cast, reinterpret_cast.
// Of course, make use of even more explicit casts as narrow_cast, to_signed_cast, to_unsigned_cast,
// polymorphic_down_cast, polymorphic_up_cast,
// explicit_cast has the advantage over c-cast that can be grep, find easily and as long as the other cast.
// The question is if we should use T(u) or T{u} instead of (T)u or explicit_cast<T>(u) when T is the name of a class.
// While T(u) or T{u} looks more like C++, it is not easy to grep neither.
// However, in this case static_cast is more strict than explicit_cast.
// Is for this reason that we have conversion_cast
template <class T, class U>
T explicit_cast(U &&u)
{
	return (T)(std::forward<U>(u));
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
