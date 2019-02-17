//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_OPTIONAL_NULLOPT_HPP
#define JASEL_V3_FUNDAMENTAL_OPTIONAL_NULLOPT_HPP

#if __cplusplus > 201402L && defined __clang__
#include <optional>
#endif

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// 20.5.7, Disengaged state indicator
struct nullopt_t
{
	struct init
	{
	};
	constexpr explicit nullopt_t(init) noexcept
	{
	}
};
#if (__clang_major__ >= 4) && (__cplusplus > 201402L)
inline
#endif
        constexpr nullopt_t nullopt{nullopt_t::init()};

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
