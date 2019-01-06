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
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/contract.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

#if !defined JASEL_DOXYGEN_INVOKED
namespace detail
{
template <class T, class U>
struct have_same_sign
        : public integral_constant<bool, is_signed<T>::value == is_signed<U>::value>
{
};
}
#endif

// checks if we can convert u to T without losing information and if succeed return the converted T
// Note that int is convertible to unsigned int, that int(unsigned(-1)) == -1, no loss of information, but that
// -1 < 0 and unsigned(-1) > 0, so
// not only we need that there is no loss of information, but that the information is coherent.

// if we want to convert int(-1) to unsigned we could use bit_cast instead.

// However how to convert int(-1) to unsigned short? bit_cast doesn't work here as the size are not the same.
// We will need to do a static_cast<unsigned short>(-1)) which do the expected conversion.

template <class T, class U>
// requires
//  T and U are integral,
//  T is convertible from U and
//  U is convertible from T
// better to use optional<T> or expected<T> as result type
JASEL_CXX14_CONSTEXPR std::pair<bool, T> narrow_to(U u) noexcept
{
    T t = static_cast<T>(u);
    if (static_cast<U>(t) != u)
    {
        return std::make_pair(false, T{});
    }
    if ((! detail::have_same_sign<T, U>::value) &&  ((t < T{}) != (u < U{})) )
    {
        return std::make_pair(false, T{});
    }
    return std::make_pair(true, t);
}

template <class T, class U>
// requires
//  T and U are integral,
//  T is convertible from U and
//  U is convertible from T
bool narrow_to(T* t, U u) noexcept
{
    *t = static_cast<T>(u);
    if (static_cast<U>(*t) != u)
    {
        return false;
    }
    if ( (! detail::have_same_sign<T, U>::value) &&  ((*t < T{}) != (u < U{})) )
    {
        return false;
    }
    return true;
}

// checks if we can convert u to T without losing information
template <class T, class U>
// requires
//  T and U are integral (for the same sign),
//  T is convertible from U and
//  U is convertible from T
JASEL_CXX14_CONSTEXPR bool can_narrow_to(U u) noexcept
{
    return narrow_to<T>(u).first;
}


// precondition: can_narrow_to<T>(u)
template <class T, class U>
JASEL_CXX14_CONSTEXPR T narrow_cast(U u) noexcept
{
    JASEL_EXPECTS(can_narrow_to<T>(u));
	return static_cast<T>( u );
}

struct narrowing_error : public exception
{
};

/// a checked version of narrow_cast() that throws if the cast loss information
template <class T, class U>
JASEL_CXX14_CONSTEXPR T narrow(U u)
{
    auto res = narrow_to<T>(u);
    if (! res.first)
    {
        throw narrowing_error();
    }
    return res.second;
}
}
}
}

#endif // header
