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
template <typename T, typename U>
constexpr pair<bool, T> overflow_cvt(U u ) noexcept
{
#if 0
    // ! in_range<C>(a)
    if ( (static_cast<C>(a) < numeric_limits<C>::min()) || ( static_cast<C>(a) > numeric_limits<C>::max()) )
        return {true, C{}};
    return {false, static_cast<C>(a)};
#else
    T t = static_cast<T>(u);
    if (static_cast<U>(t) != u)
    {
        return std::make_pair(true, T{});
    }
    if (!detail::have_same_sign<T, U>::value && ((t < T{}) != (u < U{})))
    {
        return std::make_pair(true, T{});
    }
    return std::make_pair(false, t);
#endif
}
template <typename C, typename T>
constexpr bool check_overflow_cvt( T a ) noexcept
{
#if 0
    // ! in_range<C>(a)
    if ( (static_cast<C>(a) < numeric_limits<C>::min()) || ( static_cast<C>(a) > numeric_limits<C>::max()) )
        return true;
    return false;
#else
    return ! experimental::can_narrow_to<C>(a);
#endif

}

    template <typename C, typename T>
    bool overflow_cvt( C* result, T a ) noexcept
    {
#if 0
        // ! in_range<C>(a)
        if ( (static_cast<C>(a) < numeric_limits<C>::min()) || ( static_cast<C>(a) > numeric_limits<C>::max()) )
        return true;
        *result = static_cast<C>(a);
        return false;
#else
        return ! narrow_to(result, a);
#endif
    }

    template <typename T>
    enable_if_t< is_signed<T>::value, bool> overflow_neg( T* result, T a ) noexcept
    {
        // The single case of overflow is when doing -(-128)
        if (a == numeric_limits<T>::min()) return true;
        *result = -a;
        return false;
        //auto x = -to_double_width(a);
        //return overflow_cvt(result, x);
    }

    // The value of a << b is the unique value congruent to a * 2^b modulo 2^N
    // where N is the number of bits in the return type (that is, bitwise left shift is
    // performed and the bits that get shifted out of the destination type are discarded).
    // In any case, if the value of the right operand is negative or is greater or equal
    // to the number of bits in the promoted left operand, the behavior is undefined.
    template <typename T>
    bool overflow_lsh( T* result, T a, int b ) noexcept
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
    bool overflow_rsh( T* result, T a, int b ) noexcept
    {
        using promoted = decltype(a>>b);
        if ( ! (0 <= b && b < numeric_limits<promoted>::digits) )
        return true;
        // Since C++20 operator>> is arithmetic left shift and so there is no other constraints
        *result = a >> b;
        return false;
    }

    //__builtin_add_overflow
    template <typename T>
    enable_if_t< (sizeof(T) <= 4), bool> overflow_add( T* result, T a, T b ) noexcept
    {
        auto x = to_double_width(a) + to_double_width(b);
        return overflow_cvt(result, x);
    }

    template <typename T>
    enable_if_t< (sizeof(T) <= 4), bool> overflow_sub( T* result, T a, T b ) noexcept
    {
        auto x = to_signed_bit_cast(to_double_width(a)) - to_signed_bit_cast(to_double_width(b));
        return overflow_cvt(result, x);
    }
    template <typename T>
    enable_if_t< (sizeof(T) <= 4), bool> overflow_mul( T* result, T a, T b ) noexcept
    {
        auto x = to_double_width(a) * to_double_width(b);
        return overflow_cvt(result, x);
    }
    template <typename T>
    enable_if_t< (sizeof(T) <= 4), bool> overflow_div( T* result, T a, T b ) noexcept
    {
        if ( b == T(0) )
        {
            return true;
        }
        auto x = to_double_width(a) / to_double_width(b);
        return overflow_cvt(result, x);
    }
}
}
}
}

#endif // header
