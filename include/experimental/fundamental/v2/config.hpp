//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016-2019.
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

#if __cplusplus >= 202003L
#define JASEL_CXX20_CONSTEXPR constexpr
#else
#define JASEL_CXX20_CONSTEXPR
#endif

#if __cplusplus >= 201703L
#define JASEL_CXX17_CONSTEXPR constexpr
#else
#define JASEL_CXX17_CONSTEXPR
#endif

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

#define JASEL_NOEXCEPT BOOST_NOEXCEPT

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

#define JASEL_STRINGIFY(x) JASEL_STRINGIFY_(x)
#define JASEL_STRINGIFY_(x) #x

// Attributes

#if __cplusplus > 201703L
#define JASEL_ATTR_LIKELY [[likely]]
#define JASEL_ATTR_UNLIKELY [[unlikely]]
#else
#define JASEL_ATTR_LIKELY
#define JASEL_ATTR_UNLIKELY
#endif

// this is more portable
#define JASEL_LIKELY(X) BOOST_LIKELY(X)
#define JASEL_UNLIKELY(X) BOOST_UNLIKELY(X)

#if __cplusplus > 201402L
#define JASEL_ATTR_NODISCARD [[nodiscard]]
#define JASEL_ATTR_FALLTHROUGH [[fallthrough]]
#define JASEL_ATTR_MAYBE_UNUSED [[maybe_unused]]
#else
#define JASEL_ATTR_NODISCARD
#define JASEL_ATTR_FALLTHROUGH BOOST_FALLTHROUGH
#define JASEL_ATTR_MAYBE_UNUSED BOOST_ATTRIBUTE_UNUSED
#endif
#define JASEL_NODISCARD JASEL_ATTR_NODISCARD

#if __cplusplus > 201103L
#define JASEL_ATTR_DEPRECATED [[deprecated]]
#else
#define JASEL_ATTR_DEPRECATED
#endif

#define JASEL_ATTR_NORETURN BOOST_NORETURN
#define JASEL_NORETURN JASEL_ATTR_NORETURN
#define JASEL_RESTRICT BOOST_RESTRICT
#define JASEL_ATTR_FORCEINLINE BOOST_FORCEINLINE
#define JASEL_ATTR_NOINLINE BOOST_NOINLINE
#define JASEL_UNREACHABLE_RETURN BOOST_UNREACHABLE_RETURN(result)

// builtins

#ifdef _MSC_VER
#define JASEL_ASSUME(cond) __assume(cond)

#elif defined __clang__
#define JASEL_ASSUME(cond) __builtin_assume(cond)

#elif defined __GNUC__
#define JASEL_ASSUME(cond) \
	if (cond)              \
		;                  \
	else                   \
		__builtin_unreachable()

#else
#define JASEL_ASSUME(cond) static_cast<void>(!!(cond))
#endif

#endif // header
