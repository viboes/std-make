//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

/// This file contains a mixin helper class that pretends to almost
///     introduce the relational operators associated to a spaceship operation.

#ifndef JASEL_FUNDAMENTAL_V3_COMPARE_ORDERED_HPP
#define JASEL_FUNDAMENTAL_V3_COMPARE_ORDERED_HPP

#if __cplusplus >= 201402L

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/compare/compare.hpp>
#include <experimental/fundamental/v3/compare/spaceship.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

namespace mixin
{
    // Add explicit defaults for user-defined operators !=, ==, >, !=, >= and <= in function of compare using CRTP
    template <class Derived, class Order>
    struct ordered;
    template <class StrongEquality>
    struct ordered<StrongEquality, strong_equality>
    {
        friend bool operator==(const StrongEquality& a, const StrongEquality& b)
        {
            return strong_equal(a, b);
        }
        friend bool operator!=(const StrongEquality& a, const StrongEquality& b)
        {
            return ! strong_equal(a, b);
        }
    };

    template <class StrongOrdered>
    struct ordered<StrongOrdered, strong_ordering> : ordered<StrongOrdered, strong_equality>
    {
        friend bool operator<(const StrongOrdered& a, const StrongOrdered& b)
        {
            return strong_less(a, b);
        }
        friend bool operator<=(const StrongOrdered& a, const StrongOrdered& b)
        {
            return ! strong_less(b, a);
        }
        friend bool operator>(const StrongOrdered& a, const StrongOrdered& b)
        {
            return strong_less(b, a);
        }
        friend bool operator<=(const StrongOrdered& a, const StrongOrdered& b)
        {
            return ! strong_less(b, a);
        }
    };


    template <class Derived>
    struct spaceship_comparable : ordered<Derived, order_t<Derived> >
    {};




}

}
}
}
#endif
#endif // header
