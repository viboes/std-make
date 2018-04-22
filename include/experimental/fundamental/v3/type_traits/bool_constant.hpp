//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_STD_TYPE_TRAITS_BOOL_CONSTANT_HPP
#define JASEL_STD_TYPE_TRAITS_BOOL_CONSTANT_HPP

#if __cplusplus < 201703L

#include <type_traits>

namespace std {

template <bool B>
using bool_constant = integral_constant<bool, B>;

}
#endif

#endif // header
