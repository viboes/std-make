//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_STD_UTILITY_AS_CONST_HPP
#define JASEL_STD_UTILITY_AS_CONST_HPP

#include <experimental/type_traits.hpp>

#if __cplusplus < 201703L

namespace std
{

template <class T>
constexpr std::add_const_t<T>& as_const(T& t) noexcept
{
    return t;
}

}

#endif

#endif // header
