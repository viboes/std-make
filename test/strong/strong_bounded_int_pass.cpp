// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_ints.hpp>

#include <experimental/strong_bounded_int.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

#if __cplusplus >= 201402L
namespace stdex = std::experimental;

using Frame = stdex::strong_bounded_int<class FrameTag, int, -1023, 1023>;
using FrameDC = stdex::strong_bounded_int<class FrameTag, int, -1023, 1023>;
using Slot = stdex::strong_bounded_int<class SlotTag, int, 0, 6>;

Frame add(Frame x, Frame y){
  return x+y;
}
#endif
int main()
{
#if __cplusplus >= 201402L

  {
      //Frame fn1 = 1;     // error - explicit required
      Frame fn2{1};
      BOOST_TEST(fn2 == Frame{1});
      short s=1;
      Frame fn3 {s};
      BOOST_TEST(fn2 == Frame{1});

      //Frame fn4 {1.0};      // error narrowing conversion

      Slot sn1{1};
      Frame fn5 {fn2};
      BOOST_TEST(fn5 == Frame{1});
      //Slot sn2{fn2}; // error

      //int i = fn2; // error

      auto fn6 = add(fn2, fn3);
      BOOST_TEST(fn6 == Frame{2});
      //auto fn7 = add(fn2, 1); // error
      BOOST_TEST(fn2 != fn6);
      BOOST_TEST(fn2 < fn6);

  }
  { // default constructor
    FrameDC oc;
    (void)oc;
    // not initialized
    //BOOST_TEST(oc == FrameDC{});
  }
  { // constructor from UT
    Frame oc{1};
    BOOST_TEST(oc == Frame{1});
  }
  { // constructor from implicitly convertible to UT
    short s=1;
    Frame oc{s};
    BOOST_TEST(oc == Frame{1});
  }
  { // constructor from invalid Int
    try {
      Slot oc{-1};
      BOOST_TEST(false);
    } catch(stdex::bad_bounded_int_cast& ex) {
    }
  }
  { // copy constructor
    Frame oc1{1};
    Frame oc2{oc1};
    BOOST_TEST(oc1 == oc2);
  }
  { // assignment
    Frame oc1{1};
    Frame oc2;
    oc2 = oc1;
    BOOST_TEST(oc2 == oc1);
  }
#if COMPILE_ERROR
  { // assignment from UT
    Frame oc1{1};
    oc1 = 2; // error
  }
#endif

  { // operator+(x)
    Frame oc{1};
    Frame oc2 = +oc;
    BOOST_TEST(oc2 == Frame{1});
  }
  { // operator+(x,y)
    Frame oc1{1};
    Frame oc2{2};
    BOOST_TEST(oc1+oc2 == Frame{3});
  }
  { // operator+=(x)
    Frame oc1{1};
    Frame oc2{2};
    oc2 += oc1;
    BOOST_TEST(oc2 == Frame{3});
  }
  { // operator++()
    Frame oc1{1};
    auto oc2 = ++oc1;
    BOOST_TEST(oc1 == Frame{2});
    BOOST_TEST(oc2 == Frame{2});
  }
  { // operator++(int)
    Frame oc1{1};
    auto oc2 = oc1++;
    BOOST_TEST(oc1 == Frame{2});
    BOOST_TEST(oc2 == Frame{1});
  }

  { // operator-(x)
    Frame oc{1};
    Frame oc2 = -oc;
    BOOST_TEST(oc2 == Frame{-1});
  }
  { // operator-(x,y)
    Frame oc1{1};
    Frame oc2{2};
    BOOST_TEST(oc1-oc2 == Frame{-1});
  }
  { // operator-=(x)
    Frame oc1{1};
    Frame oc2{2};
    oc2 -= oc1;
    BOOST_TEST(oc2 == Frame{1});
  }
  { // operator--()
    Frame oc1{1};
    auto oc2 = --oc1;
    BOOST_TEST(oc1 == Frame{0});
    BOOST_TEST(oc2 == Frame{0});
  }
  { // operator--(int)
    Frame oc1{1};
    auto oc2 = oc1--;
    BOOST_TEST(oc1 == Frame{0});
    BOOST_TEST(oc2 == Frame{1});
  }

  { // operator*(x,y)
    Frame oc1{3};
    Frame oc2{2};
    BOOST_TEST(oc1*oc2 == Frame{6});
  }
  { // operator*=(x)
    Frame oc1{3};
    Frame oc2{2};
    oc2 *= oc1;
    BOOST_TEST(oc2 == Frame{6});
  }

  { // operator/(x,y)
    Frame oc1{6};
    Frame oc2{2};
    BOOST_TEST(oc1/oc2 == Frame{3});
  }
  { // operator/=(x)
    Frame oc1{6};
    Frame oc2{2};
    oc1 /= oc2;
    BOOST_TEST(oc1 == Frame{3});
  }

  { // operator%(x,y)
    Frame oc1{6};
    Frame oc2{5};
    BOOST_TEST(oc1%oc2 == Frame{1});
  }
  { // operator%=(x)
    Frame oc1{6};
    Frame oc2{5};
    oc1 %= oc2;
    BOOST_TEST(oc1 == Frame{1});
  }

  { // operator==(x,y)
    Frame oc1{2};
    Frame oc2{2};
    BOOST_TEST( oc1==oc2 );
  }
  { // operator!=(x,y)
    Frame oc1{2};
    Frame oc2{3};
    BOOST_TEST( oc1!=oc2 );
  }
  { // operator<(x,y)
    Frame oc1{2};
    Frame oc2{5};
    BOOST_TEST( oc1<oc2 );
  }
  { // operator<=(x,y)
    Frame oc1{2};
    Frame oc2{5};
    BOOST_TEST( oc1<=oc2 );
  }
  { // operator<=(x,y)
    Frame oc1{2};
    Frame oc2{2};
    BOOST_TEST( oc1<=oc2 );
  }
  { // operator>(x,y)
    Frame oc1{6};
    Frame oc2{5};
    BOOST_TEST( oc1>oc2 );
  }
  { // operator>=(x,y)
    Frame oc1{6};
    Frame oc2{5};
    BOOST_TEST( oc1>=oc2 );
  }
  { // operator<=(x,y)
    Frame oc1{2};
    Frame oc2{2};
    BOOST_TEST( oc1>=oc2 );
  }
  // swap
  {
    Frame oc1{1};
    Frame oc2{2};
    using std::swap;
    swap(oc1,oc2);
    BOOST_TEST(oc1.underlying()==2);
    BOOST_TEST(oc2.underlying()==1);
  }
  // operator <<
  {
    Frame oc{1};
      std::stringstream os;
      os << oc;
      BOOST_TEST_EQ(os.str(), "1");
  }
  // operator >>
  {
    Frame oc;
      std::stringstream s;
      s << 1;
      s >> oc;
      BOOST_TEST_EQ(oc, Frame{1});
  }
  // hash
  {
    Frame oc1{1};
    BOOST_TEST(std::hash<Frame>{}(oc1)==std::hash<int>{}(1));
  }
  {
    using Bounded = Slot;
    assert(stdex::ordinal::size<Bounded>() == 7);
    assert(stdex::ordinal::traits<Bounded>::pos(Bounded{0}) == 0);
    assert(stdex::ordinal::pos(Bounded{4}) == 4);
    assert(stdex::ordinal::pos(Bounded{1}) == 1);
    assert(stdex::ordinal::val<Bounded>(0) == Bounded{0});
    assert(stdex::ordinal::val<Bounded>(1) == Bounded{1});
    assert(stdex::ordinal::first<Bounded>() == Bounded{0});
    assert(stdex::ordinal::last<Bounded>() == Bounded{6});
    assert(stdex::ordinal::succ(Bounded{2}) == Bounded{3});
    assert(stdex::ordinal::pred(Bounded{3}) == Bounded{2});
  }
#endif
  return ::boost::report_errors();
}

