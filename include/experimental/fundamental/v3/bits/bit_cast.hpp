//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_BIT_BIT_CAST_HPP
#define JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_BIT_BIT_CAST_HPP

// http://open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0476r2.html#bitcast

#include <cstring>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <class Target, class Source>
Target bit_cast (const Source& from) {
    static_assert(is_trivially_copyable<Source>::value, "Source must be trivially copyable");
    static_assert(is_trivially_copyable<Target>::value, "Target must be trivially copyable");
    static_assert(sizeof(Target) == sizeof(Source), "Provided types should have the same size");

    Target to;
    std::memcpy(&to, &from, sizeof(to));
    return to;
}

}
}
}


#endif // header
