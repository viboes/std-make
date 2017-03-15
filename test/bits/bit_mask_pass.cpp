// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/bit_mask.hpp>

#include <experimental/bit_mask.hpp>

#include <boost/detail/lightweight_test.hpp>

int main()
{
#if __cplusplus >= 201402L

  namespace stde = std::experimental;
  using UInt = unsigned char;
  using BitMask = stde::bit_mask<4,UInt>;
  //using BitMaskError = stde::bit_mask<4,int>;

  static_assert(sizeof(BitMask) == sizeof(UInt), "Bad sizeof");
  static_assert(alignof(BitMask) == alignof(UInt), "Bad alignof");

  {// Default Constructor
    BitMask bm;
    (void)bm;
  }
  {// Default Constructor
    BitMask bm{};
    BOOST_TEST( 0 == bm.count() );
    BOOST_TEST( bm.none() );
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
  }
  {// Default Constructor
    constexpr BitMask bm{};
    static_assert(0 == bm.count(), "");
    static_assert( bm.none(), "" );
    static_assert(! bm[UInt{1}], "");
    static_assert(! bm[UInt{2}], "");
  }
  { // Constructible from UInt
    BitMask bm(std::experimental::pos_tag_t{}, UInt{2});
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST( bm.any() );
    BOOST_TEST(bm[UInt{2}]==true);
  }
  { // Constructible from UInt
    constexpr BitMask bm(std::experimental::pos_tag_t{}, UInt{2});
    static_assert( 1 == bm.count() , "");
    static_assert( bm.any() , "");
    static_assert(bm[UInt{2}]==true, "");
  }
  { // operator [T]
    BitMask bm{};
    bm[UInt{1}]=true;
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST( bm.any() );
    BOOST_TEST(bm[UInt{1}]);
  }
  { // set
    BitMask bm{};
    bm.set();
    BOOST_TEST_EQ( 0xf , int(bm.to_integer()) );
    BOOST_TEST_EQ( 4 , std::experimental::bit_ops::popcount(0xf0) );
    BOOST_TEST_EQ( 4U , bm.count() );
    BOOST_TEST( bm.all() );
    BOOST_TEST(bm[UInt{0}]);
    BOOST_TEST(bm[UInt{1}]);
    BOOST_TEST(bm[UInt{2}]);
    BOOST_TEST(bm[UInt{3}]);
  }
  { // set
    BitMask bm{};
    bm.set(UInt{1});
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST( bm.any() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST(bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // set
    BitMask bm{};
    bm.set(UInt{1});
    bm.set(UInt{1}, false);
    BOOST_TEST( 0 == bm.count() );
    BOOST_TEST( bm.none() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // set
    BitMask bm{};
    try {
      bm.set(UInt{4});
      BOOST_TEST(false);
    } catch(...) {}
  }
  { // reset
    BitMask bm{};
    bm.set(UInt{1});
    bm.set(UInt{3});
    bm.reset();
    BOOST_TEST( 0 == bm.count() );
    BOOST_TEST( bm.none() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // reset
    BitMask bm{};
    bm.set(UInt{1});
    bm.set(UInt{3});
    bm.reset(UInt{1});
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST( bm.any() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
    BOOST_TEST( bm[UInt{3}]);
  }
  { // set
    BitMask bm{};
    try {
      bm.reset(UInt{4});
      BOOST_TEST(false);
    } catch(...) {}
  }
  { // flip
    BitMask bm{};
    bm.flip();
    BOOST_TEST( bm.all() );
    BOOST_TEST( bm[UInt{0}] );
    BOOST_TEST( bm[UInt{1}] );
    BOOST_TEST( bm[UInt{2}] );
    BOOST_TEST( bm[UInt{3}] );
  }
  { // flip
    BitMask bm{};
    bm.flip(UInt{1});
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST(! bm[UInt{0}] );
    BOOST_TEST( bm[UInt{1}] );
    BOOST_TEST(! bm[UInt{2}] );
    BOOST_TEST(! bm[UInt{3}] );
  }
  { // flip
    BitMask bm{};
    bm.flip(UInt{1});
    bm.flip(UInt{1});
    BOOST_TEST( 0 == bm.count() );
  }
  { // to_ulong
    BitMask bm{};
    auto x = bm.to_ulong();
    BOOST_TEST( 0 == x);
  }
  { // to_ulong
    constexpr BitMask bm{};
    constexpr auto x = bm.to_ulong();
    static_assert( 0 == x, "");
  }
  { // to_ulong
    BitMask bm{};
    bm.set(UInt{1});
    auto x = bm.to_ulong();
    BOOST_TEST( 2 == x);
  }
  { // to_ulong
    constexpr BitMask bm(std::experimental::pos_tag_t{}, UInt{1});
    constexpr auto x = bm.to_ulong();
    static_assert( 2 == x, "");
  }
  { // operator~
    BitMask bm{};
    auto bm2 = ~bm;
    BOOST_TEST( bm.none() );
    BOOST_TEST( bm2.all() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
    BOOST_TEST( bm2[UInt{0}]);
    BOOST_TEST( bm2[UInt{1}]);
    BOOST_TEST( bm2[UInt{2}]);
    BOOST_TEST( bm2[UInt{3}]);
  }
  { // operator~
    constexpr BitMask bm{};
    constexpr auto bm2 = ~bm;
    static_assert( bm.none() , "");
    static_assert( bm2.all() , "");
    static_assert(! bm[UInt{0}], "");
    static_assert(! bm[UInt{1}], "");
    static_assert(! bm[UInt{2}], "");
    static_assert(! bm[UInt{3}], "");
    static_assert( bm2[UInt{0}], "");
    static_assert( bm2[UInt{1}], "");
    static_assert( bm2[UInt{2}], "");
    static_assert( bm2[UInt{3}], "");
  }
  { // operator&
    BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    BitMask bm2(std::experimental::pos_tag_t{}, UInt{2});
    auto bm = bm1 & bm2;
    BOOST_TEST( bm.none() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // operator|
    BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    BitMask bm2(std::experimental::pos_tag_t{}, UInt{2});
    auto bm = bm1 | bm2;
    BOOST_TEST( 2 == bm.count() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST( bm[UInt{1}]);
    BOOST_TEST( bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // operator^
    BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    BitMask bm2(std::experimental::pos_tag_t{}, UInt{2});
    auto bm = bm1 ^ bm2;
    BOOST_TEST( 2 == bm.count() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST( bm[UInt{1}]);
    BOOST_TEST( bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // operator^
    BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    BitMask bm2(std::experimental::pos_tag_t{}, UInt{1});
    auto bm = bm1 ^ bm2;
    BOOST_TEST( 0 == bm.count() );
  }
  { // operator<<
    BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    auto bm = bm1 << 1;
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST( bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // operator<<
    BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    auto bm = bm1 << 4;
    BOOST_TEST( 0 == bm.count() );
  }
  { // operator>>
    BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    auto bm = bm1 >> 1;
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST( bm[UInt{0}]);
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // operator>>
    BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    auto bm = bm1 >> 4;
    BOOST_TEST( 0 == bm.count() );
  }

  { // operator==
    BitMask bm1{};
    BitMask bm2{};
    BOOST_TEST( bm1 == bm2 );
  }
  { // operator==
    constexpr BitMask bm1{};
    constexpr BitMask bm2{};
    static_assert( bm1 == bm2 , "");
  }
  { // operator!=
    BitMask bm1{};
    bm1.set(UInt{1});
    BitMask bm2{};
    BOOST_TEST( bm1 != bm2 );
  }
  { // operator!=
    constexpr BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    constexpr BitMask bm2{};
    static_assert( bm1 != bm2 , "");
  }
  { // explicit CopyConstructor
    BitMask bm1{};
    bm1.set(UInt{1});
    BitMask bm2{bm1};
    BOOST_TEST( bm1 == bm2 );
  }
  { // explicit CopyConstructor
    constexpr BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    constexpr BitMask bm2{bm1};
    static_assert( bm1 == bm2 , "");
  }
  { // implicit CopyConstructor
    BitMask bm1{};
    bm1.set(UInt{1});
    BitMask bm2=bm1;
    BOOST_TEST( bm1 == bm2 );
  }
  { // implicit CopyConstructor
    constexpr BitMask bm1(std::experimental::pos_tag_t{}, UInt{1});
    constexpr BitMask bm2=bm1;
    static_assert( bm1 == bm2 , "");
  }
  { // operator=
    BitMask bm1{};
    bm1.set(UInt{1});
    BitMask bm2{};
    bm2=bm1;
    BOOST_TEST( bm1 == bm2 );
  }
  { // operator[] const
    const BitMask bm{std::experimental::pos_tag_t{}, UInt{1}};
    auto x = bm[1];
    BOOST_TEST( x );
  }
  { // operator[] const
    const BitMask bm{std::experimental::pos_tag_t{}, UInt{1}};
    auto x =  ! bm[1];
    BOOST_TEST( ! x );
  }
  { // operator[] reference::flip()
    BitMask bm{};
    bm.set(UInt{1});
    auto x = bm[1];
    auto y = x.flip();
    BOOST_TEST( ! y );
  }
  { // operator[] reference::flip()
    BitMask bm{};
    bm.set(UInt{1});
    auto x = bm[1].flip();
    BOOST_TEST( ! x );
  }
  { // operator[] reference::operator~
    BitMask bm{};
    bm.set(UInt{1});
    auto x = bm[1];
    auto y = ~x;
    BOOST_TEST( ! y );
  }
  { // operator[] reference::operator~
    BitMask bm{};
    bm.set(UInt{1});
    auto x = ~bm[1];
    BOOST_TEST( ! x );
  }

#endif
  return ::boost::report_errors();
}
