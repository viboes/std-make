//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016-2018.
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

#ifndef JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V
#define JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V 0
#endif
#ifndef JASEL_CONFIG_CONTRACT_LEVEL_MASK
#define JASEL_CONFIG_CONTRACT_LEVEL_MASK 0x111
#endif

#if __cplusplus >= 201402L
#define JASEL_VARIABLE_TEMPLATES
#else
#endif

#if __cplusplus > 201402L

#if defined __clang__
#define JASEL_HAS_FOLD_EXPRESSIONS
#define JASEL_HAS_INVOKE
#define JASEL_SUPPORT_SWAPPABLE
#define JASEL_SUPPORT_TUPLE

#elif defined __GNUC__
#define JASEL_SUPPORT_SWAPPABLE
#if __GNUC__ > 6
#define JASEL_HAS_FOLD_EXPRESSIONS
#define JASEL_HAS_INVOKE
#define JASEL_SUPPORT_TUPLE
#else
#endif

#else

#define JASEL_HAS_FOLD_EXPRESSIONS
#define JASEL_HAS_INVOKE
#define JASEL_SUPPORT_SWAPPABLE
#define JASEL_SUPPORT_TUPLE
#endif

#elif __cplusplus >= 201402L

#if defined __clang__
#define JASEL_SUPPORT_SWAPPABLE
#elif defined __GNUC__
#define JASEL_SUPPORT_SWAPPABLE
#else
#endif

#endif

#define JASEL_CONSTEXPR BOOST_CONSTEXPR

#if __cplusplus >= 201402L
#define JASEL_MUTABLE_CONSTEXPR BOOST_CXX14_CONSTEXPR
#define JASEL_CXX14_CONSTEXPR BOOST_CXX14_CONSTEXPR
#else
#define JASEL_MUTABLE_CONSTEXPR
#define JASEL_CXX14_CONSTEXPR
#endif

#define JASEL_ENABLE_IF(...) typename ::std::enable_if<(__VA_ARGS__), bool>::type = true

#ifdef JASEL_HAS_INVOKE
#include <functional>

#define JASEL_INVOKE(F, ...) std::invoke((F), __VA_ARGS__)
#else
#define JASEL_INVOKE(F, ...) F(__VA_ARGS__)
#endif

#if __cplusplus > 201402L && defined __clang__
#define JASEL_INLINE_VAR inline
#else
#define JASEL_INLINE_VAR
#endif

#if __cplusplus > 201402L
#define JASEL_NODISCARD [[nodiscard]]
#else
#define JASEL_NODISCARD
#endif

#define JASEL_NOEXCEPT_DECLTYPE_RETURN(...)                \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
	        ->decltype(__VA_ARGS__)                        \
	{                                                      \
		return (__VA_ARGS__);                              \
	}

#define JASEL_DECLTYPE_RETURN_NOEXCEPT(...) \
	JASEL_NOEXCEPT_DECLTYPE_RETURN(__VA_ARGS__)

#define JASEL_DECLTYPE_RETURN(...) \
	->decltype(__VA_ARGS__)        \
	{                              \
		return (__VA_ARGS__);      \
	}

#define JASEL_NOEXCEPT_RETURN(...)                         \
	noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
	{                                                      \
		return (__VA_ARGS__);                              \
	}

#define JASEL_NORETURN BOOST_NORETURN

#define JASEL_STRINGIFY(x) JASEL_STRINGIFY_(x)
#define JASEL_STRINGIFY_(x) #x

/**/

#endif // header
