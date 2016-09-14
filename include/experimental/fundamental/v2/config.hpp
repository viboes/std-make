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



#if __cplusplus >= 201402L
#define JASEL_VARIABLE_TEMPLATES
#else
#endif

#if __cplusplus > 201402L
#define JASE_HAS_FOLD_EXPRESSIONS
#else
#endif

#if __cplusplus > 201402L
#define JASE_HAS_INVOKE
#else
#endif

#if __cplusplus >= 201402L
#define JASE_MUTABLE_CONSTEXPR BOOST_CONSTEXPR
#else
#define JASE_MUTABLE_CONSTEXPR
#endif

#define JASEL_DECLTYPE_RETURN_NOEXCEPT(...)               \
    noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) ->   \
    decltype(__VA_ARGS__)                                       \
    { return (__VA_ARGS__); }                                   \

#define JASEL_NOEXCEPT_RETURN(...)               \
    noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__)))    \
    { return (__VA_ARGS__); }                                   \

/**/

#endif // BOOST_ENUMS_CONFIG_HPP
