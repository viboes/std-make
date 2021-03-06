// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2013,2014,2016 Vicente J. Botet Escriba
// (C) Copyright 2013 Pierre Talbot

#ifndef JASEL_EXPERIMENTAL_V3_EXPECTED_CONFIG_HPP
#define JASEL_EXPERIMENTAL_V3_EXPECTED_CONFIG_HPP

#include <experimental/fundamental/v2/config.hpp>


#if defined BOOST_NO_CXX11_REF_QUALIFIERS
#error
#  define JASEL_NO_CXX11_RVALUE_REFERENCE_FOR_THIS
#endif

# if __cplusplus < 201402L
#  define JASEL_NO_CXX11_MOVE_ACCESSORS
# elif defined __clang__
#  if (__clang_major__ < 3) || (__clang_major__ == 3) && (__clang_minor__ < 5)
#   define JASEL_NO_CXX11_MOVE_ACCESSORS
#  endif
# elif defined __GNUC__
#  if (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__ < 50000)
#   define JASEL_NO_CXX11_MOVE_ACCESSORS
#  endif
# elif defined _MSC_VER
#  if _MSC_VER < 1900 // VS14
#   define JASEL_NO_CXX11_MOVE_ACCESSORS
#  endif
# else
#  define JASEL_NO_CXX11_MOVE_ACCESSORS
# endif

# if defined JASEL_NO_CXX11_MOVE_ACCESSORS
#define JASEL_CONSTEXPR_IF_MOVE_ACCESSORS
#else
#define JASEL_CONSTEXPR_IF_MOVE_ACCESSORS BOOST_CONSTEXPR
#endif

// ../../../boost/expected/expected.hpp: In instantiation of ‘class boost::expected<int>’:
// test_expected.cpp:79:17:   required from here
// ../../../boost/expected/expected.hpp:596:15: desole, pas implante: use of ‘type_pack_expansion’ in template

#if defined BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#error
# define JASEL_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#else
# if defined __GNUC__ and not defined __clang__
#  if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8) || !defined(__GXX_EXPERIMENTAL_CXX0X__)
#error
#   define JASEL_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#  endif
# endif
#endif

#if defined __GNUC__
# if (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__ < 40800) || !defined(__GXX_EXPERIMENTAL_CXX0X__)
#  define JASEL_NO_IF_THEN_ELSE
# endif
#endif

# if defined __clang__
#  if (__clang_major__ < 3) || (__clang_major__ == 3) && (__clang_minor__ < 0)
#   define JASEL_NO_CXX11_UNRESTRICTED_UNIONS
#  endif
# elif defined __GNUC__
#  if (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__ < 40600)
#   define JASEL_NO_CXX11_UNRESTRICTED_UNIONS
#  endif
# elif defined _MSC_VER
#  if _MSC_VER < 1900 // VS14
#   define JASEL_NO_CXX11_UNRESTRICTED_UNIONS
#  endif
# else
#  define JASEL_NO_CXX11_UNRESTRICTED_UNIONS
# endif

# if defined BOOST_NO_CXX11_CONSTEXPR
#error
#  define JASEL_USE_STD_ADDRESSOF
# endif


#endif // JASEL_EXPERIMENTAL_V3_EXPECTED_CONFIG_HPP
