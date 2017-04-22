// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_ints.hpp>

#include <experimental/strong_ints.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

using OrangeCount = stdex::strong_int<class OrangeTag, int>;
using OrangeCountDC = stdex::strong_int<class OrangeTag, int>;
using AppelCount = stdex::strong_int<class AppelTag, int>;
using UOrangeCount = stdex::strong_int<class OrangeTag, unsigned int>;
using UCOrangeCount = stdex::strong_int<class OrangeTag, unsigned char>;
using SOrangeCount = stdex::strong_int<class OrangeTag, short>;
using COrangeCount = stdex::strong_int<class OrangeTag, char>;

OrangeCount add(OrangeCount x, OrangeCount y){
  return x+y;
}

struct X {
    operator int() { return true; }
};

struct Y {
    explicit operator int() { return true; }
};

int main()
{

  {
    auto x = stdex::make_strong_int<OrangeTag>(1);
    BOOST_TEST(x == OrangeCount(1));
  }

  {
      //OrangeCount oc1 = 1;     // error - explicit required
  }
  {
      OrangeCount oc2{1};
      BOOST_TEST(oc2 == OrangeCount{1});
      short s=1;
      OrangeCount oc3 {s};
      BOOST_TEST(oc2 == OrangeCount{1});

      //OrangeCount oc4 {1.0};      // error narrowing conversion

      AppelCount ac1{1};
      OrangeCount oc5 {oc2};
      BOOST_TEST(oc5 == OrangeCount{1});

      //AppelCount ac2=oc2; // error
      //AppelCount ac2{oc2}; // error
      AppelCount ac2{oc2.underlying()};
      BOOST_TEST(ac2 == AppelCount{1});

      //int i = oc2; // error
      //int i{oc2}; // error
      int i{oc2.underlying()};
      BOOST_TEST(i == 1);

      auto oc6 = add(oc2, oc3);
      BOOST_TEST(oc6 == OrangeCount{2});
      //auto oc7 = add(oc2, 1); // error
      BOOST_TEST(oc2 != oc6);
      BOOST_TEST(oc2 < oc6);

      {
          //OrangeCount es {Y{}};        // error
          OrangeCount es {X{}};        // ok, but shouldn't this be forbiden also?
      }
      { // uninitialize default constructor
        OrangeCountDC oc;
        (void)oc;
        //not initialized
        //BOOST_TEST(oc == OrangeCountDC{});
      }
      { // constructor from UT
        OrangeCount oc{1};
        BOOST_TEST(oc == OrangeCount{1});
      }
      { // zero-initializer C++98 default constructor
        OrangeCountDC oc = OrangeCountDC();
        BOOST_TEST(oc == OrangeCountDC{});
      }
      { // zero_initializee C++11 default constructor
        OrangeCountDC oc{};
        BOOST_TEST(oc == OrangeCountDC{});
      }
      { // constructor from UT
        OrangeCount oc{1};
        BOOST_TEST(oc == OrangeCount{1});
      }
      { // constructor from implicitly convertible to UT
        short s=1;
        OrangeCount oc{s};
        BOOST_TEST(oc == OrangeCount{1});
      }
      { // copy constructor
        OrangeCount oc1{1};
        OrangeCount oc2{oc1};
        BOOST_TEST(oc1 == oc2);
      }
      { // assignment
        OrangeCount oc1{1};
        OrangeCount oc2;
        oc2 = oc1;
        BOOST_TEST(oc2 == oc1);
      }
#if COMPILE_ERROR
      { // assignment from UT
        OrangeCount oc1{1};
        oc1 = 2; // error
      }
#endif
      { // operator+(x)
        OrangeCount oc{1};
        OrangeCount oc2 = +oc;
        BOOST_TEST(oc2 == OrangeCount{1});
      }
      { // operator+(x,y)
        OrangeCount oc1{1};
        OrangeCount oc2{2};
        BOOST_TEST(oc1+oc2 == OrangeCount{3});
      }
      { // operator+(x,y)
        SOrangeCount oc1{1};
        SOrangeCount oc2{2};
        BOOST_TEST(oc1+oc2 == SOrangeCount{3});
      }
      { // operator+(x,y)
        SOrangeCount oc1{1};
        SOrangeCount oc2{2};
        auto oc = oc1+oc2;
        static_assert(std::is_same<decltype(oc), SOrangeCount>::value, "error");

        BOOST_TEST(oc == SOrangeCount{3});
      }

      { // operator+=(x)
        OrangeCount oc1{1};
        OrangeCount oc2{2};
        oc2 += oc1;
        BOOST_TEST(oc2 == OrangeCount{3});
      }
      { // operator++()
        OrangeCount oc1{1};
        auto oc2 = ++oc1;
        BOOST_TEST(oc1 == OrangeCount{2});
        BOOST_TEST(oc2 == OrangeCount{2});
      }
      { // operator++(int)
        OrangeCount oc1{1};
        auto oc2 = oc1++;
        BOOST_TEST(oc1 == OrangeCount{2});
        BOOST_TEST(oc2 == OrangeCount{1});
      }

      { // operator-(x)
        OrangeCount oc{1};
        OrangeCount oc2 = -oc;
        BOOST_TEST(oc2 == OrangeCount{-1});
      }
      { // operator-(x)
        UOrangeCount oc{1};
        UOrangeCount oc2 = -oc;
        BOOST_TEST(oc2 == UOrangeCount{-1u});
      }
      { // operator-(x,y)
        OrangeCount oc1{1};
        OrangeCount oc2{2};
        BOOST_TEST(oc1-oc2 == OrangeCount{-1});
      }
      { // operator-(x,y)
        SOrangeCount oc1{1};
        SOrangeCount oc2{2};
        BOOST_TEST(oc1-oc2 == SOrangeCount{-1});
      }
      { // operator-(x,y)
        SOrangeCount oc1{1};
        SOrangeCount oc2{2};
        auto oc = oc1-oc2;
        static_assert(std::is_same<decltype(oc), SOrangeCount>::value, "error");

        BOOST_TEST(oc == SOrangeCount{-1});
      }
      { // operator-(x,y)
        UOrangeCount oc1{3};
        UOrangeCount oc2{2};
        BOOST_TEST(oc1-oc2 == UOrangeCount{1});
      }
      { // operator-(x,y)
        UOrangeCount oc1{3};
        UOrangeCount oc2{2};
        auto oc = oc1-oc2;
        static_assert(std::is_same<decltype(oc), UOrangeCount>::value, "error");
        BOOST_TEST(oc == UOrangeCount{1});
      }
      { // operator-=(x)
        OrangeCount oc1{1};
        OrangeCount oc2{2};
        oc2 -= oc1;
        BOOST_TEST(oc2 == OrangeCount{1});
      }
      { // operator--()
        OrangeCount oc1{1};
        auto oc2 = --oc1;
        BOOST_TEST(oc1 == OrangeCount{0});
        BOOST_TEST(oc2 == OrangeCount{0});
      }
      { // operator--(int)
        OrangeCount oc1{1};
        auto oc2 = oc1--;
        BOOST_TEST(oc1 == OrangeCount{0});
        BOOST_TEST(oc2 == OrangeCount{1});
      }

      { // operator*(x,y)
        OrangeCount oc1{3};
        OrangeCount oc2{2};
        BOOST_TEST(oc1*oc2 == OrangeCount{6});
      }
      { // operator*=(x)
        OrangeCount oc1{3};
        OrangeCount oc2{2};
        oc2 *= oc1;
        BOOST_TEST(oc2 == OrangeCount{6});
      }

      { // operator/(x,y)
        OrangeCount oc1{6};
        OrangeCount oc2{2};
        BOOST_TEST(oc1/oc2 == OrangeCount{3});
      }
      { // operator/=(x)
        OrangeCount oc1{6};
        OrangeCount oc2{2};
        oc1 /= oc2;
        BOOST_TEST(oc1 == OrangeCount{3});
      }

      { // operator%(x,y)
        OrangeCount oc1{6};
        OrangeCount oc2{5};
        BOOST_TEST(oc1%oc2 == OrangeCount{1});
      }
      { // operator%=(x)
        OrangeCount oc1{6};
        OrangeCount oc2{5};
        oc1 %= oc2;
        BOOST_TEST(oc1 == OrangeCount{1});
      }

      { // operator==(x,y)
        OrangeCount oc1{2};
        OrangeCount oc2{2};
        BOOST_TEST( oc1==oc2 );
      }
      { // operator!=(x,y)
        OrangeCount oc1{2};
        OrangeCount oc2{3};
        BOOST_TEST( oc1!=oc2 );
      }
      { // operator<(x,y)
        OrangeCount oc1{2};
        OrangeCount oc2{5};
        BOOST_TEST( oc1<oc2 );
      }
      { // operator<=(x,y)
        OrangeCount oc1{2};
        OrangeCount oc2{5};
        BOOST_TEST( oc1<=oc2 );
      }
      { // operator<=(x,y)
        OrangeCount oc1{2};
        OrangeCount oc2{2};
        BOOST_TEST( oc1<=oc2 );
      }
      { // operator>(x,y)
        OrangeCount oc1{6};
        OrangeCount oc2{5};
        BOOST_TEST( oc1>oc2 );
      }
      { // operator>=(x,y)
        OrangeCount oc1{6};
        OrangeCount oc2{5};
        BOOST_TEST( oc1>=oc2 );
      }
      { // operator<=(x,y)
        OrangeCount oc1{2};
        OrangeCount oc2{2};
        BOOST_TEST( oc1>=oc2 );
      }
      // swap
      {
        OrangeCount oc1{1};
        OrangeCount oc2{2};
        using std::swap;
        swap(oc1,oc2);
        BOOST_TEST(oc1.underlying()==2);
        BOOST_TEST(oc2.underlying()==1);
      }

      // hash
      {
        OrangeCount oc1{1};
        BOOST_TEST(std::hash<OrangeCount>{}(oc1)==std::hash<int>{}(1));
      }
      // operator <<
      {
          OrangeCount oc{1};
          std::stringstream os;
          os << oc;
          BOOST_TEST_EQ(os.str(), "1");
      }
      // operator >>
      {
          OrangeCount oc;
          std::stringstream s;
          s << 1;
          s >> oc;
          BOOST_TEST_EQ(oc, OrangeCount{1});
      }
      {
          SOrangeCount oc;
          std::stringstream s;
          s << 1;
          s >> oc;
          BOOST_TEST_EQ(oc, SOrangeCount{1});
      }
      {
          COrangeCount oc;
          std::stringstream s;
          s << COrangeCount{5};
          s >> oc;
          BOOST_TEST_EQ(oc, COrangeCount{5});
      }
      {
        //OrangeCount id {&S::foo}; // this fails as expected
      }
      {
        //OrangeCount id  {&f}; // this fails as expected
      }
      {
        //double d;
        //OrangeCount id  {d}; // this fails as expected
      }
      {
        short s = 1;
        OrangeCount id  {s}; // OK
        BOOST_TEST_EQ(id.underlying(), 1);
      }
      {
        //short s = -1;
        //UOrangeCount id  {s}; // this fails as expected - narrowing conversion
      }
      {
        //short s = 1;
        //UCOrangeCount id  {s}; // this fails as expected - narrowing conversion
      }
      {
        //char c = 1;
        //UCOrangeCount id  {c}; // this fails as expected - narrowing conversion
      }
  }
  return ::boost::report_errors();
}

