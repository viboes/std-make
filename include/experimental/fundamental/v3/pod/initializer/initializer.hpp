// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2019 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_POD_INITIALIZER_HPP
#define JASEL_FUNDAMENTAL_V3_POD_INITIALIZER_HPP
// See https://wandbox.org/permlink/kqJAntRkc0JJ6z3A

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace pod
{
template <class POD>
struct is_default_initializable : false_type
{
};

template <class POD, JASEL_REQUIRES(is_pod<POD>::value)>
void zero_initialize(POD &p)
{
	memset(&p, 0, sizeof(POD));
}

struct zero_initializer
{
	template <class POD, JASEL_REQUIRES(is_pod<POD>::value)>
	operator POD() const
	{
		POD result;
		zero_initialize(result);
		return result;
	}
};

template <class POD, int N>
void default_initialize(POD (&arr)[N])
{
	for (int i = 0; i < N; ++i)
		default_initialize(arr[i]);
}

template <class POD, JASEL_REQUIRES(is_default_initializable<POD>::value)>
void default_initialize(POD &p)
{
	p.default_initialize();
}
template <class POD, JASEL_REQUIRES(!is_default_initializable<POD>::value)>
void default_initialize(POD &)
{
}

struct default_initializer
{
	template <class POD, JASEL_REQUIRES(is_default_initializable<POD>::value)>
	operator POD() const
	{
		POD result;
		result.default_initialize();
		return result;
	} // namespace pod
};    // namespace fundamental_v3

template <class POD, int N>
void value_initialize(POD (&arr)[N])
{
	for (int i = 0; i < N; ++i)
		value_initialize(arr[i]);
}

template <class POD, JASEL_REQUIRES(is_default_initializable<POD>::value)>
void value_initialize(POD &p)
{
	default_initialize(p);
}

template <class POD, JASEL_REQUIRES(!is_default_initializable<POD>::value && is_pod<POD>::value)>
void value_initialize(POD &p)
{
	zero_initialize(p);
}

struct value_initializer
{
	template <class POD, JASEL_REQUIRES(is_pod<POD>::value)>
	operator POD() const
	{
		POD result;
		value_initialize(result);
		return result;
	} // namespace pod
};    // namespace fundamental_v3
} // namespace pod

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
