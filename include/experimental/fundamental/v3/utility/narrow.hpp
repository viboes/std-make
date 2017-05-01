// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2017 Vicente J. Botet Escriba
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef JASEL_EXPERIMENTAL_UTILITY_NARROW_HPP
#define JASEL_EXPERIMENTAL_UTILITY_NARROW_HPP

#include <exception>
#include <type_traits>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

/// a searchable way to do narrowing casts of values
template <class T, class U>
inline constexpr T narrow_cast(U &&u) noexcept
{
	return static_cast<T>(forward<U>(u));
}

/// narrowing_error exception
struct narrowing_error : public exception
{
};

#if !defined JASEL_DOXYGEN_INVOKED
namespace details
{
template <class T, class U>
struct _is_same_signedness
        : public integral_constant<bool, is_signed<T>::value == is_signed<U>::value>
{
};
}
#endif

/// a checked version of narrow_cast() that throws if the cast changed the value
template <class T, class U>
inline T narrow(U u)
{
	T t = narrow_cast<T>(u);
	if (static_cast<U>(t) != u)
	{
		throw narrowing_error();
	}
	if (!details::_is_same_signedness<T, U>::value && ((t < T{}) != (u < U{})))
	{
		throw narrowing_error();
	}
	return t;
}
}
}
}

#endif // header
