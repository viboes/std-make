//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_INCOMPLETE_CAST_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_INCOMPLETE_CAST_HPP

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// As for  c-stype cast use incomplete_cast only when there is no a more explicit cast.
// incomplete_cast has the advantage over c-cast that can be grep
template <class T, class U>
T incomplete_cast(U* u) {
    return (T)(u);
}

template <class T, class U>
T incomplete_cast(U& u) {
    return (T)(u);
}


}
}
}

#endif // header
