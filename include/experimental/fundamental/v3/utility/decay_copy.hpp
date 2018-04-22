//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_DECAY_COPY_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_DECAY_COPY_HPP

#include <utility> //
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <class T>
constexpr decay_t<T> decay_copy(T&& v)
{
    return std::forward<T>(v);
}

}
}
}


#endif // header
