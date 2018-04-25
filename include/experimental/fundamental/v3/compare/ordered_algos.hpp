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
#include <experimental/utility.hpp>
#include <limits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

    // [cmp.alg], comparison algorithms
#if 0
    // todo: implement
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

    template <typename T
        , enable_if_t<
            numeric_limits<T>::is_iec559 == false
            &&
            ! is_convertible<
                decltype(compare(declval<T>(),declval<T>())),
                strong_ordering
            >::value
        , int> = 0
    >
    strong_ordering strong_order(T const& a, T const&b) = delete;

#if (__cplusplus <= 201402L || ! defined __clang__)
    template <typename T
        , enable_if_t<
            is_convertible_v<
                decltype(declval<T>() == declval<T>()),
                bool
            >
            &&
            is_convertible_v<
                decltype(declval<T>() < declval<T>()),
                bool
            >
        , int> =0
    >
    strong_ordering strong_order(T const& a, T const&b, priority_tag<0>)
    {
        if (a == b) return strong_ordering::equal;
        if (a < b) return strong_ordering::less;
        return strong_ordering::greater;
    }
    template <typename T
        , enable_if_t<
            is_convertible_v<
                decltype(compare(declval<T>(),declval<T>())),
                strong_ordering
            >
        , int> =0
    >
    strong_ordering strong_order(T const& a, T const&b, priority_tag<1>)
    {
        return compare(a,b);
    }

    template <typename T
        , enable_if_t<
            numeric_limits<T>::is_iec559 == true
        , int> =0
    >
    strong_ordering strong_order(T const& a, T const&b, priority_tag<2>)
    {
        // todo: strong_ordering value onsistent with the totalOrder operation as specified in ISO/IEC/IEEE 60559
        return strong_ordering::equal;
    }
    template <typename T>
    auto strong_order(T const& a, T const&b)
    JASEL_DECLTYPE_RETURN_NOEXCEPT(
                    strong_order(a, b, priority_tag<2>{})
         )

#else

    template <typename T
        , enable_if_t<
            numeric_limits<T>::is_iec559 == true
        ||
            is_convertible_v<
                decltype(compare(declval<T>(),declval<T>())),
                strong_ordering
            >
        ||
        (
            is_convertible_v<
                decltype(declval<T>() == declval<T>()),
                bool
            >
            &&
            is_convertible_v<
                decltype(declval<T>() < declval<T>()),
                bool
            >
        )
        , int> =0
    >
    strong_ordering strong_order(T const& a, T const&b)
    {
        if constexpr (numeric_limits<T>::is_iec559)
            // todo: strong_ordering value onsistent with the totalOrder operation as specified in ISO/IEC/IEEE 60559
            return strong_ordering::equal;
        if constexpr (is_convertible_v<
                      decltype(compare(declval<T>(),declval<T>())),
                      strong_ordering
                  >)
            return compare(a,b);
        else {
            if (a == b) return strong_ordering::equal;
            if (a < b) return strong_ordering::less;
            return strong_ordering::greater;
        }
    }
#endif

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
