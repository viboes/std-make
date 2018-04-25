//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_STD_UTILITY_PRIORITY_TAG_HPP
#define JASEL_STD_UTILITY_PRIORITY_TAG_HPP

#include <cstdlib>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <size_t I>
struct priority_tag : priority_tag<I-1> {};
template<> struct priority_tag<0> {};

}
}
}


#endif // header
