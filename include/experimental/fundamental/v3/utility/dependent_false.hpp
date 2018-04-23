//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_DEPENDENT_FALSE_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_DEPENDENT_FALSE_HPP

#include <utility> //
#include <type_traits>
#include <experimental/fundamental/v2/config.hpp>

// See https://quuxplusone.github.io/blog/2018/04/02/false-v/

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

namespace dependent_false_detail {
    class class_with_a_really_long_name_that_you_should_not_use{};
}

template <class T>
struct dependent_false : std::false_type{};

template<>
struct dependent_false<dependent_false_detail::class_with_a_really_long_name_that_you_should_not_use> : std::true_type{};

#if __cplusplus >= 201402L

template <typename...>
JASEL_INLINE_VAR constexpr bool false_v = false;

#endif
}
}
}


#endif // header
