//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V1_META_IN_PLACE_HPP
#define JASEL_V1_META_IN_PLACE_HPP

#if defined  JASEL_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
#include <optional.hpp>
#else
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

#if ! defined STDEF_VERSION

constexpr struct in_place_t{} in_place{};

#elif STDEF_VERSION == 1

constexpr struct in_place_t{} in_place{};

#elif STDEF_VERSION == 2

struct in_place_tag{};
using in_place_t = in_place_tag (&)();
inline in_place_tag in_place() { return {}; }

#endif

}
}
}
#endif
#endif // header
