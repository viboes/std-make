//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/enums for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_CONFIG_HPP
#define JASEL_FUNDAMENTAL_V2_CONFIG_HPP

#include <boost/config.hpp>


#if __cplusplus > 201402L
#define JASE_CXX14_CONSTEXPR_NO_CONST BOOST_CONSTEXPR
#else
#define JASE_CXX14_CONSTEXPR_NO_CONST
#endif

#define JASEL_DECLTYPE_RETURN_NOEXCEPT(...)               \
    noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) ->   \
    decltype(__VA_ARGS__)                                       \
    { return (__VA_ARGS__); }                                   \
/**/

#endif // BOOST_ENUMS_CONFIG_HPP
