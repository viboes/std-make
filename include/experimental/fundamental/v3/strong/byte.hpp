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
#include <experimental/fundamental/v3/strong/mixins/convertible.hpp>
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

    //! helper function to cast from byte to IntegralType
    template <class IntegerType= unsigned int, typename = enable_if_t<is_integral<IntegerType>::value>>
    constexpr IntegerType to_integer(byte b) noexcept
    {
      return static_cast<IntegerType>(b);
    }

    //! helper function to cast from byte to the underlying unsigned char
    inline constexpr unsigned char to_uchar( byte b ) noexcept
    {
        return to_integer<unsigned char>( b );
    }
    inline constexpr unsigned char to_uchar( int i ) noexcept
    {
        return static_cast<unsigned char>( i );
    }

    //! helper function to cast from IntegralType to byte
    template< class IntegralType, typename = enable_if_t<is_integral<IntegralType>::value> >
    inline constexpr byte to_byte( IntegralType v ) noexcept
    {
      return byte{ static_cast<unsigned char>(v) };
    }

    template <class IntegerType, typename = enable_if_t<is_integral<IntegerType>::value>>
    constexpr byte& operator<<=(byte& b, IntegerType shift) noexcept
    {
      return b = byte{ to_uchar(to_uchar( b ) << shift) };
    }
    template <class IntegerType, typename = enable_if_t<is_integral<IntegerType>::value>>
    constexpr byte operator<<(byte b, IntegerType shift) noexcept
    {
      return byte{ to_uchar(to_uchar( b ) << shift) };
    }
    template <class IntegerType, typename = enable_if_t<is_integral<IntegerType>::value>>
    constexpr byte& operator>>=(byte& b, IntegerType shift) noexcept
    {
      return b = byte{ to_uchar(to_uchar( b ) >> shift) };
    }

    template <class IntegerType, typename = enable_if_t<is_integral<IntegerType>::value>>
    constexpr byte operator>>(byte b, IntegerType shift) noexcept
    {
      return byte{ to_uchar(to_uchar( b ) >> shift) };
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
        : strong_type<byte, unsigned char>
        , mixin::bitwise<byte>
        , mixin::comparable<byte>
        , mixin::explicit_convertible_to<byte, unsigned char>
        , mixin::streamable<byte>
    {
      using base_type = strong_type<byte, unsigned char>;
      using base_type::base_type;

      byte() noexcept = default;

      template< class IntegralType, typename = enable_if_t<is_same<IntegralType, unsigned char>::value> >
      explicit constexpr byte( IntegralType v ) noexcept : base_type(v) { }
    };

    //! helper function to cast from IntegralType to byte
    template< class IntegralType, typename = enable_if_t<is_integral<IntegralType>::value> >
    inline constexpr byte to_byte( IntegralType v ) noexcept
    {
      return { static_cast<byte>( static_cast<unsigned char>(v) ) };
    }

    template <class IntegerType = unsigned int, typename = enable_if_t<is_integral<IntegerType>::value>>
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
    template <class IntegerType, typename = enable_if_t<is_integral<IntegerType>::value>>
    constexpr byte& operator<<=(byte& b, IntegerType shift) noexcept
    {
      return b = to_byte( to_uchar (to_uchar( b ) << shift) );
    }
    template <class IntegerType, typename = enable_if_t<is_integral<IntegerType>::value>>
    constexpr byte operator<<(byte b, IntegerType shift) noexcept
    {
      return to_byte( to_uchar(to_uchar( b ) << shift) );
    }
    template <class IntegerType, typename = enable_if_t<is_integral<IntegerType>::value>>
    constexpr byte& operator>>=(byte& b, IntegerType shift) noexcept
    {
      return b = to_byte( to_uchar(to_uchar( b ) >> shift) );
    }

    template <class IntegerType, typename = enable_if_t<is_integral<IntegerType>::value>>
    constexpr byte operator>>(byte b, IntegerType shift) noexcept
    {
      return to_byte( to_uchar(to_uchar( b ) >> shift) );
    }

    JASEL_MUTABLE_CONSTEXPR byte& operator|=(byte& l, byte r) noexcept
    {
      return l = to_byte( to_uchar(to_uchar( l ) | to_uchar( r )) );
    }
    constexpr byte operator|(byte l, byte r) noexcept
    {
      return to_byte( to_uchar(to_uchar( l ) | to_uchar( r )) );
    }
    JASEL_MUTABLE_CONSTEXPR byte& operator&=(byte& l, byte r) noexcept
    {
      return l = to_byte( to_uchar(to_uchar( l ) & to_uchar( r )) );
    }
    constexpr byte operator&(byte l, byte r) noexcept
    {
      return to_byte( to_uchar(to_uchar( l ) & to_uchar( r )) );
    }
    JASEL_MUTABLE_CONSTEXPR byte& operator^=(byte& l, byte r) noexcept
    {
      return l = to_byte( to_uchar(to_uchar( l ) ^ to_uchar( r )) );
    }
    constexpr byte operator^(byte l, byte r) noexcept
    {
      return to_byte( to_uchar(to_uchar( l ) ^ to_uchar( r )) );
    }
    constexpr byte operator~(byte b) noexcept
    {
      return to_byte( to_uchar(~ to_uchar( b )) );
    }
#endif

#endif
    static_assert(sizeof(byte) == 1, "byte must have size 1");
}
}
}

#endif // header
