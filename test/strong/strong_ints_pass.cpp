// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_ints.hpp>

#include <experimental/strong_ints.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

using OrangeCount = stdex::strong_int<class OrangeTag, int>;
using AppelCount = stdex::strong_int<class AppelTag, int>;

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
    BOOST_TEST(x == OrangeCount{1});
  }

  {
      //OrangeCount oc1 = 1;     // error - explicit required
      OrangeCount oc2{1};
      BOOST_TEST(oc2 == OrangeCount{1});
      short s=1;
      OrangeCount oc3 {s};
      BOOST_TEST(oc2 == OrangeCount{1});

      //OrangeCount oc4 {1.0};      // error narrowing conversion

      AppelCount ac1{1};
      OrangeCount oc5 {oc2};
      BOOST_TEST(oc5 == OrangeCount{1});
      //AppelCount ac2{oc2}; // error

      //int i = oc2; // error

      auto oc6 = add(oc2, oc3);
      BOOST_TEST(oc6 == OrangeCount{2});
      //auto oc7 = add(oc2, 1); // error
      BOOST_TEST(oc2 != oc6);
      BOOST_TEST(oc2 < oc6);

      {
          //OrangeCount es {Y{}};        // error
    #ifdef JASEL_CONVERTIBLE_DELETED
          //OrangeCount es {X{}};        // error
    #else
          OrangeCount es {X{}};        // ok, but shouldn't this be forbiden also?
    #endif
      }

  }
  return ::boost::report_errors();
}

