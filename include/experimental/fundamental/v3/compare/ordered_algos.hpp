//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_COMPARE_ORDERED_ALGOS_HPP
#define JASEL_FUNDAMENTAL_V3_COMPARE_ORDERED_ALGOS_HPP

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

    // [cmp.alg], comparison algorithms
#if 0
    template<class T, class U> auto compare_3way(const T& a, const U& b);

    template<InputIterator I1, InputIterator I2, class Cmp>
    auto lexicographical_compare_3way(I1 b1, I1 e1, I2 b2, I2 e2, Cmp comp)
        -> common_comparison_category_t<decltype(comp(*b1,*b2)), strong_ordering>;

    template<InputIterator I1, InputIterator I2>
    auto lexicographical_compare_3way(I1 b1, I1 e1, I2 b2, I2 e2);

#endif
    // trinary comparisons

    template <typename T>
    // requires compare(a,b) -> {strong_equality}
    strong_equality strong_equal(T const& a, T const&b)
    {
        return compare(a,b);
    }
    template <typename T>
    // requires compare(a,b) -> {weak_equality}
    weak_equality weak_equal(T const& a, T const&b)
    {
        return compare(a,b);
    }


    template <typename T>
    // requires compare(a,b) -> {partial_ordering}
    partial_ordering partial_order(T const& a, T const&b)
    {
        return compare(a,b);
    }

    template <typename T>
    // requires compare(a,b) -> {weak_ordering}
    weak_ordering weak_order(T const& a, T const&b)
    {
        return compare(a,b);
    }
    template <typename T>
    // requires compare(a,b) -> {strong_ordering}
    strong_ordering strong_order(T const& a, T const&b)
    {
        return compare(a,b);
    }


   // binary comparisons

    template <typename T>
    // requires compare(a,b) -> {partial_ordering}
    bool partial_less(T const& a, T const&b)
    {
        return compare(a,b) == partial_ordering::less;
    }
    template <typename T>
    // requires compare(a,b) -> {weak_ordering}
    bool weak_less(T const& a, T const&b)
    {
        return compare(a,b) == weak_ordering::less;
    }
    template <typename T>
    // requires compare(a,b) -> {strong_ordering}
    bool strong_less(T const& a, T const&b)
    {
        return compare(a,b) == strong_ordering::less;
    }
    template <typename T>
    // requires compare(a,b) -> {strong_ordering}
    bool partial_unordered(T const&a, T const&b)
    {
        return compare(a,b) == strong_ordering::equal;
    }
    template <typename T>
    // requires compare(a,b) -> {weak_equality}
    bool weak_equivalence(T const&a, T const&b)
    {
        return compare(a,b) == weak_equality::equivalent;
    }
    template <typename T>
    // requires compare(a,b) -> {strong_equality}
    bool strong_equal(T const&a, T const&b)
    {
        return compare(a,b) == strong_equality::equal;
    }

}
}
}
#endif
#endif // header
