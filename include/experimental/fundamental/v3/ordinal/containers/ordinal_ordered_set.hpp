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
The header \c <experimantal/ordinal_ordered_set.hpp> defines a alias template for storing
Ordinal types ordered by his position.
We can say that
\c ordinal_ordered_set is the counterpart of \c std::set when the key is an `Ordinal` type and it compares by default using the ordinal::less comparator.
*/

#ifndef JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_ORDERED_SET_HPP
#define JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_ORDERED_SET_HPP

#include <experimental/ordinal.hpp>
#include <set>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
    template<typename OKey,
        class Compare = ordinal::less<OKey>,
        class Allocator = std::allocator<OKey>
    >
    using ordinal_ordered_set = std::set<OKey, Compare, Allocator>;

    template<typename OKey, class T,
        class Compare = ordinal::less<OKey>,
        class Allocator = std::allocator<OKey>
    >
    using ordinal_ordered_multiset = std::multiset<OKey, Compare, Allocator>;

#if (__cplusplus > 201402L && defined JASEL_PMR) || defined JASEL_DOXYGEN_INVOKED
    namespace pmr {
    template<typename OKey,
        class Compare = ordinal::less<OKey>
    >
    using ordinal_ordered_set = std::pmr::set<OKey, Compare>;

    template<typename OKey, class T,
        class Compare = ordinal::less<OKey>
    >
    using ordinal_ordered_multiset = std::pmr::multiset<OKey, Compare>;

    }
#endif

}}
}

#endif // header
