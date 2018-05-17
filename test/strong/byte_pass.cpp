// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_id.hpp>

#include <experimental/byte.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

#if __cplusplus <= 201402L || (! defined __clang__ && defined __GNUC__ && __GNUC__ <= 6)
static_assert(std::is_pod<stdex::byte>::value, "byte is not a POD");
#endif

int main()
{
  {
#if defined JASEL_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    stdex::byte b = static_cast<stdex::byte>( 4 );
    BOOST_TEST( static_cast<unsigned char>(b) == 4 );
    BOOST_TEST( stdex::to_integer<int>( b ) == 4 );
#endif
  }
  {
#if defined JASEL_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    stdex::byte b = stdex::byte( 4 );
    BOOST_TEST( stdex::to_integer<int>( b ) == 4 );
#endif
  }
  {
    stdex::byte b = stdex::to_byte( 4 );
    BOOST_TEST( stdex::to_integer<int>( b ) == 4 );
  }
  {
    stdex::byte a = stdex::to_byte( 3 );
    stdex::byte b = stdex::to_byte( 7 );

    BOOST_TEST(     a == a );
    BOOST_TEST(     a != b );

    BOOST_TEST(     a <  b );
    BOOST_TEST(     a <= a );
    BOOST_TEST(     a <= b );

    BOOST_TEST(     b >  a );
    BOOST_TEST(     b >= a );
    BOOST_TEST(     b >= b );

    BOOST_TEST( ! (a == b) );
    BOOST_TEST( ! (a != a) );
    BOOST_TEST( ! (b <  a) );
    BOOST_TEST( ! (a > b) );
  }
  {
    stdex::byte const b = stdex::to_byte( 0xa5 );

    BOOST_TEST( ( b | b ) == b );
    BOOST_TEST( ( b | stdex::to_byte( 0x00 ) ) == b );
    BOOST_TEST( ( b | stdex::to_byte( 0xff ) ) == stdex::to_byte( 0xff ) );
  }
  {
    stdex::byte const b = stdex::to_byte( 0xa5 );

    BOOST_TEST( ( b & b ) == b );
    BOOST_TEST( ( b & stdex::to_byte( 0xff ) ) == b );
    BOOST_TEST( ( b & stdex::to_byte( 0x00 ) ) == stdex::to_byte( 0x00 ) );
  }
  {
    stdex::byte const b = stdex::to_byte( 0xa5 );

    BOOST_TEST( ( b ^ b ) == stdex::to_byte( 0 ) );
    BOOST_TEST( ( b ^ stdex::to_byte( 0x00 ) ) ==  b );
    BOOST_TEST( ( b ^ stdex::to_byte( 0xff ) ) == ~b );
  }
  {
    stdex::byte const b_org = stdex::to_byte( 0xa5 );
    stdex::byte       b     = b_org;

    BOOST_TEST( ( b |= b               ) == b_org           );
    BOOST_TEST( ( b |= stdex::to_byte( 0x00 ) ) == b_org           );
    BOOST_TEST( ( b |= stdex::to_byte( 0xff ) ) == stdex::to_byte( 0xff ) );
  }
  {
    stdex::byte const b_org = stdex::to_byte( 0xa5 );
    stdex::byte       b     = b_org;

    BOOST_TEST( ( b &= b               ) == b_org           );
    BOOST_TEST( ( b &= stdex::to_byte( 0xff ) ) == b_org           );
    BOOST_TEST( ( b &= stdex::to_byte( 0x00 ) ) == stdex::to_byte( 0x00 ) );
  }
  {
    stdex::byte const b_org = stdex::to_byte( 0xa5 );
    stdex::byte       b     = b_org;

    BOOST_TEST( ( b ^= b               ) == stdex::to_byte( 0 ) );
    b ^= stdex::to_byte( 0x00 );
  }
  {
    stdex::byte const b_org = stdex::to_byte( 0xa5 );
    stdex::byte       b     = b_org;

    b ^= stdex::to_byte( 0x00 );
    BOOST_TEST_EQ( stdex::to_integer<>( b ),  stdex::to_integer<>(b_org)       );
  }
  {
    stdex::byte const b_org = stdex::to_byte( 0xa5 );
    stdex::byte       b     = b_org;

    b ^= stdex::to_byte( 0xff );
    BOOST_TEST_EQ( stdex::to_integer<>( b ),  stdex::to_integer<>(~b_org) );
  }
  {
    stdex::byte const b = stdex::to_byte( 0xa5 );

    BOOST_TEST( ( b << 3 ) == stdex::to_byte( to_uchar( b ) << 3 ) );

  }
  return ::boost::report_errors();
}

