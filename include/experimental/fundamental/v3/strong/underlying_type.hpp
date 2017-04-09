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

    //! underlying_type customization point
    template <class E>
    struct underlying_type
    : std::underlying_type<E>
    {};

    template <class E>
    using underlying_type_t = typename underlying_type<E>::type;

    //! get access to the underlying_type associated to T
    template <class T>
    auto underlying(T const& v) -> decltype(v.underlying())
    {
      return v.underlying();
    }

    //! used instead of static_cast
    template <class U, class T>
    U underlying_cast(T const& v)
    {
      return static_cast<U>(v.underlying());
    }
}
}
}

#endif // header
