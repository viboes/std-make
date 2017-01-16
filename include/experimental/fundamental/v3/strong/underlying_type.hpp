// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_UNDERLYING_TYPE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_UNDERLYING_TYPE_HPP

#include <type_traits>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

    template <class E>
    struct underlying_type
    //: std::underlying_type<E>
    {};

    template <class T>
    auto underlying(T const& v) -> decltype(v.underlying())
    { return v.underlying(); }

    template <class T>
    auto underlying(T & v) -> decltype(v.underlying())
    { return v.underlying();}

}
}
}

#endif // header
