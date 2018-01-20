//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

/*!
\file
\brief
The header \c <experimantal/ordinal_ordered_map.hpp> defines a alias template for storing
ordered maps having an Ordinal key.
We can say that
\c ordinal_ordered_map is the counterpart of \c std::map when the key is an Ordinal type and it compares by default using the ordinal::less comparator.
*/

#ifndef JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_AORDERED_MAP_HPP
#define JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_AORDERED_MAP_HPP

#include <experimental/ordinal.hpp>
#include <map>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
    template<typename OKey, class T,
        class Compare = ordinal::less<OKey>,
        class Allocator = std::allocator<std::pair<const OKey, T> >
    >
    using ordinal_ordered_map = std::map<OKey, T, Compare, Allocator>;

    template<typename OKey, class T,
        class Compare = ordinal::less<OKey>,
        class Allocator = std::allocator<std::pair<const OKey, T> >
    >
    using ordinal_ordered_multimap = std::multimap<OKey, T, Compare, Allocator>;

#if (__cplusplus > 201402L && defined JASEL_PMR) || defined JASEL_DOXYGEN_INVOKED
    namespace pmr {
    template<typename OKey, class T,
        class Compare = ordinal::less<OKey>
    >
    using ordinal_ordered_map = std::pmr::map<OKey, T, Compare>;

    template<typename OKey, class T,
        class Compare = ordinal::less<OKey>
    >
    using ordinal_ordered_multimap = std::pmr::multimap<OKey, T, Compare>;

    }
#endif
}}
}

#endif // header
