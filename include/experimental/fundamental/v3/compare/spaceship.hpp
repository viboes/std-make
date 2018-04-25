//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_COMPARE_SPACESHIP_HPP
#define JASEL_FUNDAMENTAL_V3_COMPARE_SPACESHIP_HPP

#if __cplusplus >= 201402L

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/compare/compare.hpp>
#include <experimental/utility.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

    // this should be the customization point of ordered
    // depending on the type returned by compare the type will have an associated order

    strong_ordering compare(bool x, bool y)
    {
        return (x < y) ? strong_ordering::less
                       : (x == y) ? strong_ordering::equal
                                  : strong_ordering::greater;
    }
    template <class T>
    strong_ordering compare(T* x, T* y)
    {
        return (x < y) ? strong_ordering::less
                       : (x == y) ? strong_ordering::equal
                                  : strong_ordering::greater;
    }

    template <class T>
    strong_ordering compare(T* x, nullptr_t)
    {
        return (x == nullptr) ? strong_ordering::equal : strong_ordering::greater;
    }
    template <class T>
    strong_ordering compare(nullptr_t, T* x)
    {
        return (x == nullptr) ? strong_ordering::equal : strong_ordering::less;
    }
    strong_ordering compare(nullptr_t, nullptr_t)
    {
        return strong_ordering::equal;
    }

#if 0
    template <typename T>
    // requires compare(a,b) -> {strong_equality}
    weak_equality compare(T const& a, T const&b)
    {
        strong_equality o = compare(a,b);
        switch ( o )
        {
            case strong_equality::equivalent: return weak_equality::equivalent;
            case strong_equality::nonequivalent: return weak_equality::nonequivalent;
        }
    }

    template <typename T>
    // requires compare(a,b) -> {strong_ordering}
    weak_ordering compare(T const& a, T const&b)
    {
        strong_ordering o = compare(a,b);
        switch ( o )
        {
            case strong_ordering::less: return weak_ordering::less;
            case strong_ordering::greater: return weak_ordering::greater;
            case strong_ordering::equal: return weak_ordering::equivalent;
        }
    }

    template <typename T>
    // requires compare(a,b) -> {strong_ordering}
    partial_ordering compare(T const& a, T const&b)
    {
        strong_ordering o = compare(a,b);
        switch ( o )
        {
            case strong_ordering::less: return weak_ordering::less;
            case strong_ordering::greater: return weak_ordering::greater;
            case strong_ordering::equal: return weak_ordering::equivalent;
        }
    }
#endif

    //Define compare for integer types. The other orders are equivalent.
    //Define partial, weak and total order for floating types. They are distinct.
    //Define compare for type_info that refine operator== and before.
    //Define compare for error_code that refine operators.
    //Define compare for error_category that refine operators.
    //Define compare for unique_ptr that refine operators.
    //Define compare for shared_ptr that refine operators.
    //Define compare for bitset that refines operator==.
    //Define compare for allocator that refines operator==.


    template <class T>
    struct order {
        using type = decltype(compare(declval<T>(), declval<T>()));
    };
    template <class T>
    using order_t = typename order<T>::type;

}
}
}
#endif
#endif // header
