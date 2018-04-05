// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_counter.hpp>

#include <experimental/strong_counter.hpp>
#include <sstream>
#include <experimental/fundamental/v3/strong/mixins/is_compatible_with.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

class OrangeTag {};
class AppelTag {};

namespace std {
namespace experimental {
inline  namespace fundamental_v3{
namespace mixin {
template <>
struct is_compatible_with<OrangeTag, OrangeTag> : std::true_type {};
template <>
struct is_compatible_with<AppelTag, AppelTag> : std::true_type {};
}
}
}
}

using OrangeCount = stdex::strong_counter<OrangeTag, int>;
using OrangeCountDC = stdex::strong_counter<OrangeTag, int>;
using AppelCount = stdex::strong_counter<AppelTag, int>;

static_assert(sizeof(OrangeCount)==sizeof(int), "");

OrangeCount add(OrangeCount x, OrangeCount y){
  return x+y;
}

struct X {
    constexpr operator int() const { return 42; }
};

struct Y {
    explicit constexpr operator int() const { return 3; }
};

int main()
{

#if  __cplusplus >= 201002L
  {
    auto x = stdex::make_strong_counter<OrangeTag>(1);
    BOOST_TEST(x == OrangeCount(1));
  }

  {
      //OrangeCount oc1 = 1;     // error - explicit required
  }
  {
    {
      OrangeCount oc2{1};
      BOOST_TEST(oc2 == OrangeCount{1});
      //short s=1;
      //OrangeCount oc3 {s};
      //BOOST_TEST(oc3 == OrangeCount{1});
      OrangeCount oc3 {1};
      //OrangeCount oc32 = s; // error

      //OrangeCount oc4 {1.0};      // error narrowing conversion

      AppelCount ac1{1};
      (void)ac1;
      OrangeCount oc5 {oc2};
      BOOST_TEST(oc5 == OrangeCount{1});
      //AppelCount ac2{oc2}; // error

      //int i = oc2; // error

      auto oc6 = add(oc2, oc3);
      BOOST_TEST(oc6 == OrangeCount{2});
      //auto oc7 = add(oc2, 1); // error
      BOOST_TEST(oc2 != oc6);
      BOOST_TEST(oc2 < oc6);
    }
      {
          //OrangeCount es {Y{}};        // error
        //OrangeCount es {X{}};        // ok, but shouldn't this be forbiden also?
        //OrangeCount esx = X{};        // ERROR
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
      { // explicit constructor from implicitly convertible to UT
        short s=1;
        OrangeCount oc{int(s)};
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
#if defined COMPILE_ERROR
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
#if defined COMPILE_ERROR
      { // operator+(x,y)
        OrangeCount oc1{1};
        OrangeCount oc2 = oc1+1; // error
      }
#endif
      { // operator+=(x)
        OrangeCount oc1{1};
        OrangeCount oc2{2};
        oc2 += oc1;
        BOOST_TEST(oc2 == OrangeCount{3});
      }
      { // operator++()
        OrangeCount oc1{1};
        auto oc2 = ++oc1;
        static_assert(std::is_same<decltype(oc2), OrangeCount>::value, "error");
        BOOST_TEST(oc1 == OrangeCount{2});
        BOOST_TEST(oc2 == OrangeCount{2});
      }
      { // operator++(int)
        OrangeCount oc1{1};
        auto oc2 = oc1++;
        static_assert(std::is_same<decltype(oc2), OrangeCount>::value, "error");
        BOOST_TEST(oc1 == OrangeCount{2});
        BOOST_TEST(oc2 == OrangeCount{1});
      }

      { // operator-(x)
        OrangeCount oc{1};
        auto oc2 = -oc;
        static_assert(std::is_same<decltype(oc2), OrangeCount>::value, "error");
        BOOST_TEST(oc2 == OrangeCount{-1});
      }
      { // operator-(x,y)
        OrangeCount oc1{1};
        OrangeCount oc2{2};
        BOOST_TEST(oc1-oc2 == OrangeCount{-1});
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
        static_assert(std::is_same<decltype(oc2), OrangeCount>::value, "error");
        BOOST_TEST(oc1 == OrangeCount{0});
        BOOST_TEST(oc2 == OrangeCount{0});
      }
      { // operator--(int)
        OrangeCount oc1{1};
        auto oc2 = oc1--;
        static_assert(std::is_same<decltype(oc2), OrangeCount>::value, "error");
        BOOST_TEST(oc1 == OrangeCount{0});
        BOOST_TEST(oc2 == OrangeCount{1});
      }

      { // operator*(x,y)
        OrangeCount oc1{3};
        BOOST_TEST(oc1*2 == OrangeCount{6});
      }
      { // operator*=(x)
        OrangeCount oc1{3};
        oc1 *= 2;
        BOOST_TEST(oc1 == OrangeCount{6});
      }

      { // operator/(x,y)
        OrangeCount oc1{6};
        OrangeCount oc2{2};
        BOOST_TEST(oc1/oc2 == 3);
      }
      { // operator/(x,y)
        OrangeCount oc1{6};
        OrangeCount oc2{2};
        (void)oc2;
        BOOST_TEST(oc1/2 == OrangeCount{3});
      }
      { // operator/=(x)
        OrangeCount oc1{6};
        oc1 /= 2;
        BOOST_TEST(oc1 == OrangeCount{3});
      }

      { // operator%(x,y)
        OrangeCount oc1{6};
        OrangeCount oc2{5};
        BOOST_TEST(oc1%oc2 == OrangeCount{1});
      }
      { // operator%(x,y)
        OrangeCount oc1{6};
        BOOST_TEST(oc1%5 == OrangeCount{1});
      }
      { // operator%=(x)
        OrangeCount oc1{6};
        OrangeCount oc2{5};
        oc1 %= oc2;
        BOOST_TEST(oc1 == OrangeCount{1});
      }
      { // operator%=(x)
        OrangeCount oc1{6};
        oc1 %= 5;
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
      // hash
      {
        OrangeCount oc1{1};
        BOOST_TEST(std::hash<OrangeCount>{}(oc1)==std::hash<int>{}(1));
      }
  }
#endif
  return ::boost::report_errors();
}

