//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

// http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p1011r0.html

#ifndef JASEL_STD_UTILITY_AS_PTR_CONST_HPP
#define JASEL_STD_UTILITY_AS_PTR_CONST_HPP

#include <type_traits>

#if __cplusplus <= 202303L

namespace std
{

template <typename T>
std::add_const_t<T>*
as_ptr_const( T* t ) noexcept
{
    return t;
}

}

#endif

#endif // header
