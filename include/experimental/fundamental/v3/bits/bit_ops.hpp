//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//////////////////////////////////////////////////////////////////////////////

/*
 \file
 \brief
 The header \c <experimental/.../bits/algorithms.hpp> defines some common bits algorithms.
 Most of them are based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0553r1.html
 */

#ifndef JASEL_FUNDAMENTAL_V3_BITS_BITOPS_HPP
#define JASEL_FUNDAMENTAL_V3_BITS_BITOPS_HPP

#include <stdexcept>
#include <iosfwd>
#include <string>
#include <cstddef>
#include <climits>
#include <string>
#include <limits>
#include <type_traits>

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  namespace bit_ops
  {
    //! @par Returns
    //!   The bits in x rotated to the left s times
    //! @par Remarks:
    //!   Participates in overload resolution only if T is an unsigned integer type
    template <class T, typename= enable_if_t<
            is_integral<T>::value && is_unsigned<T>::value
        >
    >
    JASEL_MUTABLE_CONSTEXPR T rotl(T x, unsigned int s) noexcept
    {
      constexpr unsigned int N = std::numeric_limits<T>::digits;
      constexpr unsigned int m = s % N;
      return m==0
           ? x
           : (x << m) | (x >> (N - m))
           ;
    }

    //! @par Returns:
    //!   The bits in x rotated to the right s times
    //! @par Remarks:
    //!   Participates in overload resolution only if T is an unsigned integer type
    template <class T, typename= enable_if_t<
            is_integral<T>::value && is_unsigned<T>::value
        >
    >
    JASEL_MUTABLE_CONSTEXPR T rotr(T x, unsigned int s) noexcept
    {
      constexpr unsigned int N = std::numeric_limits<T>::digits;
      constexpr unsigned int m = s % N;
      return m==0
           ? x
           : (x >> m) | (x << (N - m))
           ;
    }

    //! @par Returns
    //!   The number of consecutive 0 bits, starting from the most significant bit.
    //!   [ Note: Returns std::numeric_limits<T>::digits if x == 0. ]
    //! @par Remarks:
    //!   Participates in overload resolution only if T is an unsigned integer type
    template<class T, typename= enable_if_t<
            is_integral<T>::value && is_unsigned<T>::value
        >
    >
    constexpr int countl_zero(T x) noexcept
    {
      return __builtin_clz(x);

    }
    inline constexpr int countl_zero(unsigned long x) noexcept
    {
      return __builtin_clzl(x);

    }
    inline constexpr int countl_zero(unsigned long long x) noexcept
    {
      return __builtin_clzll(x);

    }
    //! @par Returns
    //!   The number of consecutive 0 bits, starting from the least significant bit.
    //!   [ Note: Returns std::numeric_limits<T>::digits if x == 0. ]
    //! @par Remarks:
    //!   Participates in overload resolution only if T is an unsigned integer type
    template<class T, typename= enable_if_t<
            is_integral<T>::value && is_unsigned<T>::value
        >
    >
    constexpr int countr_zero(T x) noexcept
    {
      return __builtin_ctz(x);
    }
    inline constexpr int countr_zero(unsigned long x) noexcept
        {
          return __builtin_ctzl(x);
        }
    inline constexpr int countr_zero(unsigned long long x) noexcept
        {
          return __builtin_ctzll(x);
        }

    //! @par Returns
    //!   The number of consecutive 1 bits, starting from the most significant bit.
    //!   [ Note: Returns std::numeric_limits<T>::digits if x == std::numeric_limits<T>::max(). ]
    //! @par Remarks:
    //!   Participates in overload resolution only if T is an unsigned integer type
    template<class T, typename= enable_if_t<
            is_integral<T>::value && is_unsigned<T>::value
        >
    >
    constexpr int countl_one(T x) noexcept
    {
      return countl_zero(~x);
    }

    //! @par Returns
    //!   The number of consecutive 1 bits, starting from the least significant bit.
    //!   [ Note: Returns std::numeric_limits<T>::digits if x == std::numeric_limits<T>::max(). ]
    //! @par Remarks:
    //!   Participates in overload resolution only if T is an unsigned integer type
    template<class T, typename= enable_if_t<
            is_integral<T>::value && is_unsigned<T>::value
        >
    >
    constexpr int countr_one(T x) noexcept
    {
      return countr_zero(~x);
    }

    //! @par Returns
    //!   The number of bits set to 1 in x
    template <class T>
    constexpr int popcount(T x) noexcept
    {
      return __builtin_popcount(x);
    }

    //! @par Requires:
    //!   N <= sizeof(T) * CHAR_BIT
    //! @par Returns
    //!   A mask with all the bits set to 1 up to the bit N
    //!   [ Note: Returns T(-1} if x > std::numeric_limits<T>::digits. ]
    template <size_t N, class T=unsigned>
    constexpr T up_to() noexcept
    {
      return (N >= sizeof(T) * CHAR_BIT) ?
          T(-1) : T(1u << N) - 1;
    }

    //! @par Pre-condition:
    //!   n <= sizeof(T) * CHAR_BIT
    //! @par Returns
    //!   A mask with all the bits set to 1 up to the bit n.
    template <class T=unsigned>
    constexpr T up_to(size_t n) noexcept
    {
      return (n >= sizeof(T) * CHAR_BIT) ?
          T(-1) : T(1u << n) - 1;
    }

    //! @par Requires:
    //!   S < sizeof(T) * CHAR_BIT
    //! @par Returns
    //!   A mask with the bit S set to 1
    template <size_t S, class T=unsigned>
    constexpr T single() noexcept
    {
      //constexpr unsigned int N = std::numeric_limits<T>::digits;
      //constexpr unsigned int m = S % std::numeric_limits<T>::digits;
      return T(1u << (S % std::numeric_limits<T>::digits));
    }
    //! @par Pre-condition:
    //!   s < sizeof(T) * CHAR_BIT
    //! @par Returns
    //!   A mask with the bit s set to 1
    template <class T>
    constexpr T single(size_t s) noexcept
    {
      //constexpr unsigned int N = std::numeric_limits<T>::digits;
      //const unsigned int m = s % std::numeric_limits<T>::digits;
      return T(1u << (s % std::numeric_limits<T>::digits));
    }

    //! @par Requires:
    //!   N <= sizeof(T) * CHAR_BIT
    //! @par Returns
    //!   A mask with all the bits set to 1 up to the bit N
    //!   [ Note: Returns T(-1} if x > std::numeric_limits<T>::digits. ]
    //! abcdefgh >> 2
    //!   abcdef && 3
    //!       ef
    template <size_t N, class T=unsigned>
    constexpr T from_up_to(T value, size_t pos) noexcept
    {
      return (N >= sizeof(T) * CHAR_BIT) ?
          value : (value >> pos) & up_to<N,T>();
    }

  }
}}
} // std

#endif // header

