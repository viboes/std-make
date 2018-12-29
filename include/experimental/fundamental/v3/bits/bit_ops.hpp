//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017-2018.
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
 Most of them are based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0553r3.html
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

    //! @par Requires:
    //!   T is an integral type?
    //! @par Returns
    //!   the number of bits of the type T
    template <class T>
    JASEL_NODISCARD constexpr int bitsof()
    {
      return sizeof(T) * CHAR_BIT;
    }
    template <class T>
    JASEL_NODISCARD constexpr int bitsof(T)
    {
      return bitsof<T>();
    }

    //! @par Returns
    //!   The bits in x rotated to the left s times
    //! @par Remarks:
    //!   Participates in overload resolution only if T is an unsigned integer type
    template <class T, typename= enable_if_t<
            is_integral<T>::value && is_unsigned<T>::value
        >
    >
    JASEL_NODISCARD JASEL_MUTABLE_CONSTEXPR T rotl(T x, int s) noexcept
    {
      constexpr unsigned int N = std::numeric_limits<T>::digits;
      constexpr int r = s % N;
      return (r == 0) ? x
           : (r > 0)  ? (x << r) | (x >> (N - r))
           :            rotr(x, -r)
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
    JASEL_NODISCARD JASEL_MUTABLE_CONSTEXPR T rotr(T x, int s) noexcept
    {
      constexpr unsigned int N = std::numeric_limits<T>::digits;
      constexpr int r = s % N;
      return (r == 0) ? x
           : (r > 0)  ? (x >> r) | (x << (N - r))
           :            rotl(x, -r)
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
    JASEL_NODISCARD constexpr int countl_zero(T x) noexcept
    {
      return __builtin_clz(x);

    }
    JASEL_NODISCARD constexpr int countl_zero(unsigned long x) noexcept
    {
      return __builtin_clzl(x);

    }
    JASEL_NODISCARD constexpr int countl_zero(unsigned long long x) noexcept
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
    JASEL_NODISCARD constexpr int countr_zero(T x) noexcept
    {
      return __builtin_ctz(x);
    }
    JASEL_NODISCARD constexpr int countr_zero(unsigned long x) noexcept
        {
          return __builtin_ctzl(x);
        }
    JASEL_NODISCARD constexpr int countr_zero(unsigned long long x) noexcept
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
    JASEL_NODISCARD constexpr int countl_one(T x) noexcept
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
    JASEL_NODISCARD constexpr int countr_one(T x) noexcept
    {
      return countr_zero(~x);
    }

    //! @par Returns
    //!   The number of bits set to 1 in x
    template <class T>
    JASEL_NODISCARD constexpr int popcount(T x) noexcept
    {
      return __builtin_popcount(x);
    }

    //! @par Requires:
    //!   N <= bitsof<T>()
    //! @par Returns
    //!   A mask with all the bits set to 1 up to the bit N
    //!   [ Note: Returns T(-1} if x > std::numeric_limits<T>::digits. ]
    template <size_t N, class T=unsigned>
    JASEL_NODISCARD constexpr T up_to() noexcept
    {
      static_assert(N < bitsof<T>(), "Error");
      return (N >= bitsof<T>()) ?
          T(-1) : T(1u << N) - 1;
    }

    //! @par Pre-condition:
    //!   0 < n <= bitsof<T>()
    //! @par Returns
    //!   A mask with all the bits set to 1 up to the bit n.
    template <class T=unsigned>
    JASEL_NODISCARD constexpr T up_to(size_t n) noexcept
    {
      return (n == bitsof<T>()) ?
          T(-1) : T(1u << n) - 1;
           //T(-1) >> (digits-n)
    }

    //! @par Requires:
    //!   0 <= N < bitsof<T>()
    //! @par Returns
    //!   A mask with the bit P set to 1
    template <size_t P, class T=unsigned>
    JASEL_NODISCARD constexpr T single() noexcept
    {
      static_assert(P < bitsof<T>(), "Error");
      return T(T(1u) << P);
    }
    //! @par Pre-condition:
    //!   pos < bitsof<T>()
    //! @par Returns
    //!   A mask with the bit s set to 1
    template <class T>
    JASEL_NODISCARD constexpr T single(size_t pos)
    {
      return T(T(1u) << pos);
    }

    //! @par Returns
    template <size_t N, class T=unsigned>
    JASEL_NODISCARD constexpr T from_up_to(T value, size_t pos) noexcept
    {
      // (pos+N <= bitsof<T>()) ?
      return (value >> pos) & up_to<N,T>();
    }

    template <class Integral>
    JASEL_NODISCARD constexpr Integral set_bit(Integral x, int pos) noexcept
    {
      return x | single<Integral>(pos);
    }

    template <class Integral>
    constexpr Integral reset_bit(Integral x, int pos) noexcept
    {
      return x & ~single<Integral>(pos);
    }
    template <class Integral>
    JASEL_NODISCARD constexpr Integral flip_bit(Integral x, int pos) noexcept
    {
      return x ^ single<Integral>(pos);
    }
    template <class Integral>
    JASEL_NODISCARD constexpr bool test_bit(Integral x, int pos) noexcept
    {
      return bool(x & single<Integral>(pos));
    }

    template <size_t Pos, class Integral>
    JASEL_NODISCARD constexpr Integral set_bit(Integral x) noexcept
    {
      return x | single<Pos,Integral>();
    }
    template <size_t Pos, class Integral>
    JASEL_NODISCARD constexpr Integral reset_bit(Integral x) noexcept
    {
      return x & ~single<Pos, Integral>();
    }
    template <size_t Pos, class Integral>
    JASEL_NODISCARD constexpr Integral flip_bit(Integral x) noexcept
    {
      return x ^ single<Pos, Integral>();
    }
    template <size_t Pos, class Integral>
    JASEL_NODISCARD constexpr bool test_bit(Integral x) noexcept
    {
      return bool(x & single<Pos, Integral>());
    }

  }
}}
} // std

#endif // header

