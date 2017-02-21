// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/ordinal_set.hpp>

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
    BOOST_TEST( 0 == bm.count() );
    BOOST_TEST( bm.none() );
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
  }
  { // Constructible from UInt
    BitMask bm(std::experimental::pos_tag_t{}, UInt{2});
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST( bm.any() );
    BOOST_TEST(bm[UInt{2}]==true);
  }
  { // operator [T]
    BitMask bm;
    bm[UInt{1}]=true;
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST( bm.any() );
    BOOST_TEST(bm[UInt{1}]);
  }
  { // set
    BitMask bm;
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
    BitMask bm;
    bm.set(UInt{1});
    BOOST_TEST( 1 == bm.count() );
    BOOST_TEST( bm.any() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST(bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // set
    BitMask bm;
    bm.set(UInt{1});
    bm.set(UInt{1}, false);
    BOOST_TEST( 0 == bm.count() );
    BOOST_TEST( bm.none() );
    BOOST_TEST(! bm[UInt{0}]);
    BOOST_TEST(! bm[UInt{1}]);
    BOOST_TEST(! bm[UInt{2}]);
    BOOST_TEST(! bm[UInt{3}]);
  }
  { // reset
    BitMask bm;
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
    BitMask bm;
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
  { // flip
    BitMask bm;
    bm.flip();
    BOOST_TEST( bm.all() );
    BOOST_TEST( bm[UInt{0}] );
    BOOST_TEST( bm[UInt{1}] );
    BOOST_TEST( bm[UInt{2}] );
    BOOST_TEST( bm[UInt{3}] );
  }
  { // to_ulong
    BitMask bm;
    auto x = bm.to_ulong();
    BOOST_TEST( 0 == x);
  }
  { // to_ulong
    BitMask bm;
    bm.set(UInt{1});
    auto x = bm.to_ulong();
    BOOST_TEST( 2 == x);
  }
  { // operator~
    BitMask bm;
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
  { // operator==
    BitMask bm1;
    BitMask bm2;
    BOOST_TEST( bm1 == bm2 );
  }
  { // operator!=
    BitMask bm1;
    bm1.set(UInt{1});
    BitMask bm2;
    BOOST_TEST( bm1 != bm2 );
  }
  { // explicit CopyConstructor
    BitMask bm1;
    bm1.set(UInt{1});
    BitMask bm2{bm1};
    BOOST_TEST( bm1 == bm2 );
  }
  { // implicit CopyConstructor
    BitMask bm1;
    bm1.set(UInt{1});
    BitMask bm2=bm1;
    BOOST_TEST( bm1 == bm2 );
  }
  { // operator=
    BitMask bm1;
    bm1.set(UInt{1});
    BitMask bm2;
    bm2=bm1;
    BOOST_TEST( bm1 == bm2 );
  }

#endif
  return ::boost::report_errors();
}
