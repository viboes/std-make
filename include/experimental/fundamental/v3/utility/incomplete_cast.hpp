//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_INCOMPLETE_CAST_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_INCOMPLETE_CAST_HPP

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// As for  c-stype cast use incomplete_cast only when there is no a more explicit cast.
// incomplete_cast has the advantage over c-cast that can be grep
template <class TPtr, class U>
constexpr TPtr incomplete_cast(U *u) noexcept
{
	return (TPtr)(u);
}

template <class TRef, class U>
constexpr TRef incomplete_cast(U &u) noexcept
{
	return (TRef)(u);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
