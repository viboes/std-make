// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2019 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_POD_INITIALIZER_HPP
#define JASEL_FUNDAMENTAL_V3_POD_INITIALIZER_HPP

#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace pod
{
struct zero_initializer
{
	template <class POD>
	operator POD()
	{
		POD result;
		result.zero_initialize();
		return result;
	}
};

template <class POD>
struct is_default_initializable : false_type
{
};

struct default_initializer
{
	template <class POD, JASEL_REQUIRES(is_default_initializable<POD>::value)>
	operator POD()
	{
		POD result;
		result.default_initialize();
		return result;
	}
};
struct value_initializer
{
	template <class POD>
	operator POD()
	{
		POD result;
		result.value_initialize();
		return result;
	}
};
} // namespace pod

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
