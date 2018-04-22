//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_STD_UTILITY_EXCHANGE_HPP
#define JASEL_STD_UTILITY_EXCHANGE_HPP

#if __cplusplus < 201402L

#include <experimental/fundamental/v2/config.hpp>

#include <utility> //
#include <type_traits>

namespace std
{

template<class T, class U = T>
JASEL_CXX14_CONSTEXPR T exchange(T& obj, U&& new_value)
{
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}

}

#endif

#endif // header
