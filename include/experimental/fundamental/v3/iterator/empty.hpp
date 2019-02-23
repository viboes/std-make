// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

// See also
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0769r2.pdf
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1233r1.html

#ifndef JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_ITERATOR_EMPTY_HPP
#define JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_ITERATOR_EMPTY_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <cstddef>
#include <initializer_list>
#include <iterator>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <class C>
JASEL_NODISCARD constexpr auto empty(const C &c) -> decltype(c.empty())
{
	return c.empty();
}

template <class T, std::size_t N>
JASEL_NODISCARD constexpr bool empty(const T (&array)[N]) noexcept
{
	return false;
}

template <class E>
JASEL_NODISCARD constexpr bool empty(std::initializer_list<E> il) noexcept
{
	return il.size() == 0;
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
