// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_bool.hpp>

#include <experimental/strong_bool.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

using EngineStarted = stdex::strict_bool<class EngineStartedTag>;
using CrewReady     = stdex::strict_bool<class CrewReadyTag>;
using UEngineStarted = stdex::strict_bool<class EngineStartedTag, unsigned char>;
using UCrewReady     = stdex::strict_bool<class CrewReadyTag, unsigned char>;

void set_status(EngineStarted started, CrewReady ready)
{
  (void)started;
  (void)ready;
}

struct X {
    operator bool() { return true; }
};

struct Y {
    explicit operator bool() { return true; }
};

enum class boolean : bool {};
enum class byte : unsigned char{};

void f(bool) {}

int main()
{
#if defined __clang__ && __cplusplus > 201402L
  {
    //boolean b1 = true;     // error - explicit required
    boolean b2 {true};
    BOOST_TEST(static_cast<bool>(b2) == true);
    //boolean b2 {1}; // fails as expected
  }
  {
    byte b{0xab};
    BOOST_TEST(static_cast<unsigned char>(b) == 0xab);
  }
#endif
  {
      //EngineStarted es1 = true;     // error - explicit required

      //EngineStarted es2 {nullptr};  // error void* deleted

      //EngineStarted es3 {1};        // error int deleted
      //short s;
      //EngineStarted es4 {s};        // error int deleted

      //EngineStarted es5 {1.0};      // error double deleted

      EngineStarted es {true};        // ok
      CrewReady     cr {false};       // ok

      //EngineStarted es6 =cr;       // error
      //EngineStarted es7 {cr};       // fixme: how can we forbid this providing explicit conversion to bool? clang compile this while gcc don't.

      //bool b = cr;                  // error - implicit conversion required

      bool ans = es==EngineStarted{true} && ! (cr==CrewReady{true});           // ok - explicit conversion required by the use of &&
      BOOST_TEST(ans);
      if (cr==CrewReady{true})                         // ok - explicit conversion required by the use cr in an if
          std::cout << "True\n";

      //set_status(true, true);       // error - implicit conversion required
      set_status(EngineStarted{true}, CrewReady{true});        // function call

      //auto r = es == 1;
  }
  {
      //EngineStarted es {Y{}};        // error, explicit conversion
      // we can not prevent all the implicit conversions, or can we?
      EngineStarted es {X{}};        // ok, implicit conversion from UDT
      BOOST_TEST(es==EngineStarted{true});
  }
  {
      EngineStarted es1 {true};
      EngineStarted es2 {false};
      BOOST_TEST(es2<es1);
      BOOST_TEST(es2!=es1);
      //CrewReady     cr {false};
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
      if (es.underlying())
        BOOST_TEST(true);
      else
        BOOST_TEST(false);
  }
  {
      EngineStarted es {true};
      f (es.underlying());
  }

  {
      EngineStarted es {true};
      std::stringstream os;
      os << std::boolalpha << es;
      BOOST_TEST_EQ(os.str(), "true");
  }
  {
      EngineStarted es;
      std::stringstream s;
      s << std::boolalpha << true;
      s >> std::boolalpha >> es;

      BOOST_TEST_EQ(es, EngineStarted{true});
  }
  {
//    UEngineStarted es {true};        // ok
//    UCrewReady     cr {false};       // ok

  }
  return ::boost::report_errors();
}

