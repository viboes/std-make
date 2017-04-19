// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_BYTE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_BYTE_HPP

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/bitwise.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/fundamental/v2/config.hpp>

#if defined __clang__ && __cplusplus > 201402L
#define JASEL_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
#endif

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
#if defined JASEL_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE || defined JASEL_DOXYGEN_INVOKED
    enum class byte : unsigned char {};

    template <class IntegerType= unsigned int, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr IntegerType to_integer(byte b) noexcept
    {
      return static_cast<IntegerType>(b);
    }

    inline constexpr unsigned char to_uchar( byte b ) noexcept
    {
        return to_integer<unsigned char>( b );
    }
    inline constexpr unsigned char to_uchar( int i ) noexcept
    {
        return static_cast<unsigned char>( i );
    }

    template< class T >
    inline constexpr byte to_byte( T v ) noexcept
    {
      return byte{ to_uchar(v) };
    }

    template <class IntegerType, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr byte& operator<<=(byte& b, IntegerType shift) noexcept
    {
      return b = byte{ to_uchar( b ) << shift };
    }
    template <class IntegerType, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr byte operator<<(byte b, IntegerType shift) noexcept
    {
      return byte{ to_uchar( to_uchar( b ) << shift ) };
    }
    template <class IntegerType, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr byte& operator>>=(byte& b, IntegerType shift) noexcept
    {
      return b = byte{ to_uchar( b ) >> shift };
    }

    template <class IntegerType, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr byte operator>>(byte b, IntegerType shift) noexcept
    {
      return byte{ to_uchar( b ) >> shift };
    }

    constexpr byte& operator|=(byte& l, byte r) noexcept
    {
      return l = byte{ to_uchar( to_uchar( l ) | to_uchar( r ) ) };
    }
    constexpr byte operator|(byte l, byte r) noexcept
    {
      return byte{ to_uchar( to_uchar( l ) | to_uchar( r ) ) };
    }
    constexpr byte& operator&=(byte& l, byte r) noexcept
    {
      return l = byte{ to_uchar( to_uchar( l ) & to_uchar( r ) ) };
    }
    constexpr byte operator&(byte l, byte r) noexcept
    {
      return byte{ to_uchar( to_uchar( l ) & to_uchar( r ) ) };
    }
    constexpr byte& operator^=(byte& l, byte r) noexcept
    {
      return l = byte{ to_uchar( to_uchar( l ) ^ to_uchar( r ) ) };
    }
    constexpr byte operator^(byte l, byte r) noexcept
    {
      return byte{ to_uchar( to_uchar( l ) ^ to_uchar( r ) ) };
    }
    constexpr byte operator~(byte b) noexcept
    {
      return byte{ to_uchar( ~ to_uchar( b ) ) };
    }
#else
    struct byte
        : protected_strong_type<byte, unsigned char>
        , mixin::bitwise_no_check<byte>
        , mixin::comparable<byte>
        , mixin::streamable<byte>
    {
      using base_type = protected_strong_type<byte, unsigned char>;
      using base_type::base_type;
    };

    template< class T >
    inline constexpr byte to_byte( T v ) noexcept
    {
      return { static_cast<byte>( v ) };
    }

    template <class IntegerType = unsigned int, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr IntegerType to_integer(byte b) noexcept
    {
      return static_cast<IntegerType>(b.underlying());
    }

    inline constexpr unsigned char to_uchar( byte b ) noexcept
    {
        return b.underlying();
    }
    inline constexpr unsigned char to_uchar( int i ) noexcept
    {
        return static_cast<unsigned char>( i );
    }

#if 0
    // remove after checking the implementation using bitwise mixin is really equivalent.
    template <class IntegerType, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr byte& operator<<=(byte& b, IntegerType shift) noexcept
    {
      return b = to_byte( to_uchar( b ) << shift );
    }
    template <class IntegerType, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr byte operator<<(byte b, IntegerType shift) noexcept
    {
      return to_byte( to_uchar( b ) << shift );
    }
    template <class IntegerType, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr byte& operator>>=(byte& b, IntegerType shift) noexcept
    {
      return b = to_byte( to_uchar( b ) >> shift );
    }

    template <class IntegerType, typename = enable_if<is_integral<IntegerType>::value>>
    constexpr byte operator>>(byte b, IntegerType shift) noexcept
    {
      return to_byte( to_uchar( b ) >> shift );
    }

    JASEL_MUTABLE_CONSTEXPR byte& operator|=(byte& l, byte r) noexcept
    {
      return l = to_byte( to_uchar( l ) | to_uchar( r ) );
    }
    constexpr byte operator|(byte l, byte r) noexcept
    {
      return to_byte( to_uchar( l ) | to_uchar( r ) );
    }
    JASEL_MUTABLE_CONSTEXPR byte& operator&=(byte& l, byte r) noexcept
    {
      return l = to_byte( to_uchar( l ) & to_uchar( r ) );
    }
    constexpr byte operator&(byte l, byte r) noexcept
    {
      return to_byte( to_uchar( l ) & to_uchar( r ) );
    }
    JASEL_MUTABLE_CONSTEXPR byte& operator^=(byte& l, byte r) noexcept
    {
      return l = to_byte( to_uchar( l ) ^ to_uchar( r ) );
    }
    constexpr byte operator^(byte l, byte r) noexcept
    {
      return to_byte( to_uchar( l ) ^ to_uchar( r ) );
    }
    constexpr byte operator~(byte b) noexcept
    {
      return to_byte( ~ to_uchar( b ) );
    }
#endif

#endif
    static_assert(sizeof(byte) == 1, "byte must have size 1");
}
}
}

#endif // header
