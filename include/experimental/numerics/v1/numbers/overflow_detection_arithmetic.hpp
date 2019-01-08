// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba
// Based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0103r1.html

#ifndef JASEL_NUMERIC_V1_NUMBERS_OVERFLOW_DETECTION_ARITHMETIC_HPP
#define JASEL_NUMERIC_V1_NUMBERS_OVERFLOW_DETECTION_ARITHMETIC_HPP
// see https://en.cppreference.com/w/cpp/language/operator_arithmetic for
#include <experimental/numerics/v1/numbers/double_wide_types.hpp>
#include <experimental/utility.hpp>
#include <experimental/fundamental/v2/config.hpp>

#define JASEL_BUILTIN_ARITH_OVERFLOW_CHECK 1

namespace std
{
namespace experimental
{
namespace numerics
{
inline namespace v1
{

    // overflow_cvt is similar to narrow_to with a different interface,
    // overflow_cvt returns if there was an overflow, while
    // narrow_to returns if the conversion succeeded.
    // an alternative is to replace it with status_value but the semantic becomes the same than with narrow then
    // * status_value<bool, T> true means success and false overflow.
    // * status_value<errc, T> - the single errc needed are success and overflow.
template <typename T, typename U>
JASEL_CXX14_CONSTEXPR pair<bool, T> overflow_cvt(U u) noexcept
{
    pair<bool, T> res = narrow_to<T>(u);
    res.first = !res.first;
    return res;
}

template <typename C, typename T>
JASEL_CXX14_CONSTEXPR bool check_overflow_cvt( T a ) noexcept
{
    return ! experimental::can_narrow_to<C>(a);
}

template <typename C, typename T>
JASEL_CXX14_CONSTEXPR bool overflow_cvt( C* result, T a ) noexcept
{
    return ! narrow_to(result, a);
}

template <typename T>
JASEL_CXX14_CONSTEXPR enable_if_t< is_signed<T>::value, bool> overflow_neg( T* result, T a ) noexcept
{
    // The single case of overflow is when doing -(-128)
    if (a == numeric_limits<T>::min()) return true;
    *result = -a;
    return false;
}

// The value of a << b is the unique value congruent to a * 2^b modulo 2^N
// where N is the number of bits in the return type (that is, bitwise left shift is
// performed and the bits that get shifted out of the destination type are discarded).
// In any case, if the value of the right operand is negative or is greater or equal
// to the number of bits in the promoted left operand, the behavior is undefined.
template <typename T>
JASEL_CXX14_CONSTEXPR bool overflow_lsh( T* result, T a, int b ) noexcept
{
    using promoted = decltype(a<<b);
    if ( ! (0 <= b && b < numeric_limits<promoted>::digits) )
    return true;
    // Since C++20 operator<< is arithmetic left shift and so there is no other constraints
    *result = a << b;
    return false;
}

// The value of a >> b is a/2^b, rounded down (in other words, right shift on signed a
// is arithmetic right shift).
// In any case, if the value of the right operand is negative or is greater or equal
// to the number of bits in the promoted left operand, the behavior is undefined.
template <typename T>
JASEL_CXX14_CONSTEXPR bool overflow_rsh( T* result, T a, int b ) noexcept
{
    using promoted = decltype(a>>b);
    if ( ! (0 <= b && b < numeric_limits<promoted>::digits) )
    return true;
    // Since C++20 operator>> is arithmetic left shift and so there is no other constraints
    *result = a >> b;
    return false;
}

template <typename T>
JASEL_CXX14_CONSTEXPR enable_if_t< (JASEL_BUILTIN_ARITH_OVERFLOW_CHECK || sizeof(T) <= 4), bool> overflow_add( T* result, T a, T b ) noexcept
{
#if JASEL_BUILTIN_ARITH_OVERFLOW_CHECK==1
    return __builtin_add_overflow(a, b, result);
#else
    auto x = to_double_width(a) + to_double_width(b);
    return overflow_cvt(result, x);
#endif
}

// better use status_value<errc, T>
template <typename T>
JASEL_CXX14_CONSTEXPR enable_if_t< (JASEL_BUILTIN_ARITH_OVERFLOW_CHECK || sizeof(T) <= 4),
pair<bool, T>> overflow_add( T a, T b ) noexcept
{
    pair<bool, T> result;
#if JASEL_BUILTIN_ARITH_OVERFLOW_CHECK==1
    result.first = __builtin_add_overflow(a, b, &result.second);
#else
    auto x = to_double_width(a) + to_double_width(b);
    result.first = overflow_cvt(&result.second, x);
#endif
    return result;
}

template <typename T>
JASEL_CXX14_CONSTEXPR enable_if_t< (JASEL_BUILTIN_ARITH_OVERFLOW_CHECK || sizeof(T) <= 4), bool> overflow_sub( T* result, T a, T b ) noexcept
{
#if JASEL_BUILTIN_ARITH_OVERFLOW_CHECK==1
    return __builtin_sub_overflow(a, b, result);
#else
    auto x = to_signed_bit_cast(to_double_width(a)) - to_signed_bit_cast(to_double_width(b));
    return overflow_cvt(result, x);
#endif
}

template <typename T>
JASEL_CXX14_CONSTEXPR enable_if_t< (JASEL_BUILTIN_ARITH_OVERFLOW_CHECK || sizeof(T) <= 4),
pair<bool, T>> overflow_sub( T a, T b ) noexcept
{
    pair<bool, T> result;
#if JASEL_BUILTIN_ARITH_OVERFLOW_CHECK==1
    result.first = __builtin_sub_overflow(a, b, &result.second);
#else
    auto x = to_double_width(a) - to_double_width(b);
    result.first = overflow_cvt(&result.second, x);
#endif
    return result;
}

template <typename T>
JASEL_CXX14_CONSTEXPR enable_if_t< (JASEL_BUILTIN_ARITH_OVERFLOW_CHECK || sizeof(T) <= 4), bool> overflow_mul( T* result, T a, T b ) noexcept
{
#if JASEL_BUILTIN_ARITH_OVERFLOW_CHECK==1
    return __builtin_mul_overflow(a, b, result);
#else
    auto x = to_double_width(a) * to_double_width(b);
    return overflow_cvt(result, x);
#endif
}

template <typename T>
JASEL_CXX14_CONSTEXPR enable_if_t< (JASEL_BUILTIN_ARITH_OVERFLOW_CHECK || sizeof(T) <= 4),
pair<bool, T>> overflow_mul( T a, T b ) noexcept
{
    pair<bool, T> result;
#if JASEL_BUILTIN_ARITH_OVERFLOW_CHECK==1
    result.first = __builtin_mul_overflow(a, b, &result.second);
#else
    auto x = to_double_width(a) * to_double_width(b);
    result.first = overflow_cvt(&result.second, x);
#endif
    return result;
}

template <typename T>
JASEL_CXX14_CONSTEXPR pair<bool, T> overflow_div( T a, T b ) noexcept
{
    if ( b == T(0) )
    {
        return {true, T{}};
    }
    T result = a / b;
    return {false, result};
}

template <typename T>
JASEL_CXX14_CONSTEXPR bool overflow_div( T* result, T a, T b ) noexcept
{
    if ( b == T(0) )
    {
        return true;
    }
    *result = a / b;
    return false;
}


}
}
}
}

#endif // header
