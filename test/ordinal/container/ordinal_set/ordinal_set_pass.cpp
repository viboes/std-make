// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/ordinal_set.hpp>

#include <experimental/ordinal_set.hpp>
#include "../../Bool.hpp"
#include "../../Bounded.hpp"

#include <boost/detail/lightweight_test.hpp>

int main()
{

  namespace stde = std::experimental;
  using Indx = Bounded<1,4,unsigned char>;
  using OSet = stde::ordinal_set<Indx>;

  {// Default Constructor
    OSet os;
    BOOST_TEST( 0 == os.count() );
    BOOST_TEST( os.none() );
    BOOST_TEST(! os[Indx{1}]);
    BOOST_TEST(! os[Indx{2}]);
  }
  { // Constructible from Indx
    OSet os(Indx{2});
    BOOST_TEST( 1 == os.count() );
    BOOST_TEST( os.any() );
    BOOST_TEST(os[Indx{2}]==true);
  }
  { // operator [T]
    OSet os;
    os[Indx{1}]=true;
    BOOST_TEST( 1 == os.count() );
    BOOST_TEST( os.any() );
    BOOST_TEST(os[Indx{1}]);
  }
  { // set
    OSet os;
    os.set();
    BOOST_TEST( 4 == os.count() );
    BOOST_TEST( os.all() );
    BOOST_TEST(os[Indx{1}]);
    BOOST_TEST(os[Indx{2}]);
    BOOST_TEST(os[Indx{3}]);
    BOOST_TEST(os[Indx{4}]);
  }
  { // set
    OSet os;
    os.set(Indx{1});
    BOOST_TEST( 1 == os.count() );
    BOOST_TEST( os.any() );
    BOOST_TEST(os[Indx{1}]);
    BOOST_TEST(! os[Indx{2}]);
    BOOST_TEST(! os[Indx{3}]);
    BOOST_TEST(! os[Indx{4}]);
  }
  { // set
    OSet os;
    os.set(Indx{1});
    os.set(Indx{1}, false);
    BOOST_TEST( 0 == os.count() );
    BOOST_TEST( os.none() );
    BOOST_TEST(! os[Indx{1}]);
    BOOST_TEST(! os[Indx{2}]);
    BOOST_TEST(! os[Indx{3}]);
    BOOST_TEST(! os[Indx{4}]);
  }
  { // reset
    OSet os;
    os.set(Indx{1});
    os.set(Indx{3});
    os.reset();
    BOOST_TEST( 0 == os.count() );
    BOOST_TEST( os.none() );
    BOOST_TEST(! os[Indx{1}]);
    BOOST_TEST(! os[Indx{2}]);
    BOOST_TEST(! os[Indx{3}]);
    BOOST_TEST(! os[Indx{4}]);
  }
  { // reset
    OSet os;
    os.set(Indx{1});
    os.set(Indx{4});
    os.reset(Indx{1});
    BOOST_TEST( 1 == os.count() );
    BOOST_TEST( os.any() );
    BOOST_TEST(! os[Indx{1}]);
    BOOST_TEST(! os[Indx{2}]);
    BOOST_TEST(! os[Indx{3}]);
    BOOST_TEST( os[Indx{4}]);
  }
  { // flip
    OSet os;
    os.flip();
    BOOST_TEST( os.all() );
    BOOST_TEST( os[Indx{1}] );
    BOOST_TEST( os[Indx{2}] );
    BOOST_TEST( os[Indx{3}] );
    BOOST_TEST( os[Indx{4}] );
  }
  { // to_ulong
    OSet os;
    auto x = os.to_ulong();
    BOOST_TEST( 0 == x);
  }
  { // to_ulong
    OSet os;
    os.set(Indx{1});
    auto x = os.to_ulong();
    BOOST_TEST( 1 == x);
  }
  { // operator~
    OSet os;
    auto os2 = ~os;
    BOOST_TEST( os.none() );
    BOOST_TEST( os2.all() );
    BOOST_TEST(! os[Indx{1}]);
    BOOST_TEST(! os[Indx{2}]);
    BOOST_TEST(! os[Indx{3}]);
    BOOST_TEST(! os[Indx{4}]);
    BOOST_TEST( os2[Indx{1}]);
    BOOST_TEST( os2[Indx{2}]);
    BOOST_TEST( os2[Indx{3}]);
    BOOST_TEST( os2[Indx{4}]);
  }
  { // operator==
    OSet os1;
    OSet os2;
    BOOST_TEST( os1 == os2 );
  }
  { // operator!=
    OSet os1;
    os1.set(Indx{1});
    OSet os2;
    BOOST_TEST( os1 != os2 );
  }
  { // explicit CopyConstructor
    OSet os1;
    os1.set(Indx{1});
    OSet os2{os1};
    BOOST_TEST( os1 == os2 );
  }
  { // implicit CopyConstructor
    OSet os1;
    os1.set(Indx{1});
    OSet os2=os1;
    BOOST_TEST( os1 == os2 );
  }
  { // operator=
    OSet os1;
    os1.set(Indx{1});
    OSet os2;
    os2=os1;
    BOOST_TEST( os1 == os2 );
  }
  { // operator[] const
    const OSet os(Indx{1});
    auto x = os[Indx{1}];
    BOOST_TEST( x );
  }
  { // operator[] const
    const OSet os(Indx{1});
    auto x =  ! os[Indx{1}];
    BOOST_TEST( ! x );
  }
  { // operator[] reference::flip()
    OSet os{};
    os.set(Indx{1});
    auto x = os[Indx{1}];
    auto y = x.flip();
    BOOST_TEST( ! y );
  }
  { // operator[] reference::flip()
    OSet os{};
    os.set(Indx{1});
    auto x = os[Indx{1}].flip();
    BOOST_TEST( ! x );
  }
  { // operator[] reference::operator~
    OSet os{};
    os.set(Indx{1});
    auto x = os[Indx{1}];
    auto y = ~x;
    BOOST_TEST( ! y );
  }
  { // operator[] reference::operator~
    OSet os{};
    os.set(Indx{1});
    auto x = ~os[Indx{1}];
    BOOST_TEST( ! x );
  }
  return ::boost::report_errors();
}
