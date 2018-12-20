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
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <class Target, class Source>
typename std::enable_if<
    (sizeof(Target) == sizeof(Source)) &&
    std::is_trivially_copyable<Source>::value &&
    std::is_trivial<Target>::value,
    // this implementation requires that Target is trivially default constructible, which is not surprising
    Target>::type
// constexpr support needs compiler magic
bit_cast (const Source& from)
{
    Target to;
    std::memcpy(&to, &from, sizeof(to));
    return to;
}

}
}
}


#endif // header
