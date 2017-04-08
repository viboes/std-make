// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_bool.hpp>

#include <experimental/strong_bool.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

using EngineStarted = stdex::strong_bool<class EngineStartedTag>;
using CrewReady     = stdex::strong_bool<class CrewReadyTag>;

void set_status(EngineStarted started, CrewReady ready){
  (void)started;
  (void)ready;
}

struct X {
    operator bool() { return true; }
};

struct Y {
    explicit operator bool() { return true; }
};

void f(bool) {}

int main()
{

  {
      //EngineStarted es1 = true;     // error - explicit required

      //EngineStarted es2 {nullptr};  // error void* deleted

      //EngineStarted es3 {1};        // error int deleted
      //short s;
      //EngineStarted es4 {s};        // error int deleted

      //EngineStarted es5 {1.0};      // error double deleted

      EngineStarted es {true};        // ok
      CrewReady     cr {false};       // ok

      //EngineStarted es6 =true;       // error
      //EngineStarted es6 =cr;       // error
      //EngineStarted es7 {cr};       // fixme: how can we forbid this providing explicit conversion to bool? clang compile this while gcc don't.

      //bool b = cr;                  // error - implicit conversion required

#if 0
      bool ans = bool(es && !cr);           // ko - explicit conversion required by the use of &&
      BOOST_TEST(ans);
      if (cr)                         // ok - explicit conversion required by the use cr in a if
          std::cout << "True\n";
#endif
      //set_status(true, true);       // error - implicit conversion required
      set_status(EngineStarted{true}, CrewReady{true});        // function call
  }
  {
      //EngineStarted es {Y{}};        // error, explicit conversion
      EngineStarted es {X{}};        // ok, implicit conversion from UDT
  }
  {
      EngineStarted es1 {true};
      EngineStarted es2 {false};
      CrewReady     cr {false};
      BOOST_TEST(es2<es1);
      BOOST_TEST(es2!=es1);
      //BOOST_TEST(cr<es1); // error
  }
  {
      EngineStarted es1 {true};
      EngineStarted es2 {false};
      EngineStarted es = es1 || es2;
      BOOST_TEST(es==EngineStarted{true});
  }
  {
      EngineStarted es1 {true};
      EngineStarted es = es1 || EngineStarted{false};
      BOOST_TEST(es==EngineStarted{true});
  }
  {
      EngineStarted es2 {false};
      EngineStarted es = EngineStarted{true} || es2;
      BOOST_TEST(es==EngineStarted{true});
  }
  {
      EngineStarted es1 {true};
      EngineStarted es2 {false};
      EngineStarted es = es1 && es2;
      BOOST_TEST(es==EngineStarted{false});
  }
  {
      EngineStarted es1 {true};
      EngineStarted es = ! es1 ;
      BOOST_TEST(es==EngineStarted{false});
  }
  {
      EngineStarted es {true};
      if (es) // explicit conversion implied
        BOOST_TEST(true);
      else
        BOOST_TEST(false);
  }
  {
      EngineStarted es {true};
      f (bool(es)); // explicit conversion required
  }

  {
      EngineStarted es {true};
      std::stringstream os;
      os << std::boolalpha << es;
      std::cout << std::boolalpha << true;

      BOOST_TEST_EQ(os.str(), "true");
  }
  {
      EngineStarted es;
      std::stringstream s;
      s << std::boolalpha << true;
      s >> std::boolalpha >> es;

      BOOST_TEST_EQ(bool(es), true);
  }
  return ::boost::report_errors();
}

