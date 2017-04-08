// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_id.hpp>

#include <experimental/strong_id.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

using EngineId = stdex::strong_id<class EngineIdTag>;
using BoardId     = stdex::strong_id<class BoardIdTag>;

void set_ids(EngineId engine, BoardId board){
  (void)engine;
  (void)board;
}

struct X {
    operator int() { return 0; }
};

struct Y {
    explicit operator int() { return 1; }
};


int main()
{

  {
      //EngineId es1 = 1;     // error - explicit required

      //EngineId es2 {nullptr};  // error void* deleted

      short s=1;
      EngineId es4 {s};        // error int deleted

      //EngineId es5 {1.0};      // error narrow

      EngineId es {0};        // ok
      BoardId     cr {1};       // ok

      //EngineId es6 {cr};       // error

      //int b = cr;                  // error - implicit conversion required

      //bool ans = es && !cr;           // ok - explicit conversion required by the use of &&
      //if (cr)                         // ok - explicit conversion required by the use cr in a if
      //    std::cout << "True\n";

      //set_ids(0, 1);       // error - implicit conversion required
      set_ids(EngineId{0}, BoardId{2});        // function call
  }
  {
      //EngineId es {Y{}};        // error, explicit conversion
      EngineId es {X{}};        // ok, implicit conversion from UDT
  }
  {
      EngineId es1 {1};
      EngineId es2 {2};
      BoardId     cr {3};
      BOOST_TEST(es1 < es2);
      BOOST_TEST(es2 != es1);
      //BOOST_TEST(cr < es1); // error
  }
  // operator <<
  {
      EngineId oc{1};
      std::stringstream os;
      os << oc;
      BOOST_TEST_EQ(os.str(), "1");
  }
  // operator >>
  {
      EngineId oc;
      std::stringstream s;
      s << 1;
      s >> oc;
      BOOST_TEST_EQ(oc, EngineId{1});
  }
  return ::boost::report_errors();
}

