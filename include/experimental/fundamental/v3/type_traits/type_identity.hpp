//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_TYPE_IDENTITY_HPP
#define JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_TYPE_IDENTITY_HPP

#if __cplusplus <= 201703L

#include <type_traits>

namespace std {

template <class T>
struct type_identity {
    using type = T;
};

template <class T>
using type_identity_t = typename type_identity<T>::type;

template <class T>
using non_deduced_type_t = type_identity_t<T>;

}

#endif

#endif // header
