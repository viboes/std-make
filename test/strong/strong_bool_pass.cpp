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
using UEngineStarted = stdex::strong_bool<class EngineStartedTag, unsigned char>;
using UCrewReady     = stdex::strong_bool<class CrewReadyTag, unsigned char>;

static_assert(sizeof(EngineStarted)==sizeof(bool), "");
static_assert(sizeof(UEngineStarted)==sizeof(unsigned char), "");

using IBool     = stdex::strong_bool<class IBoolTag, int>;

static_assert(sizeof(UEngineStarted)==1, "");

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

struct S {
  void foo() {}
};

int main()
{

#if defined COMPILER_ERROR
  {
    IBool b = 1; // error

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
      BOOST_TEST(es);
      BOOST_TEST(!cr);

      //EngineStarted es6 =true;       // error
      //EngineStarted es6 =cr;       // error
      //bool b = cr;                  // error - implicit conversion required

#if defined __clang__ && (__clang_major__ < 4)
        EngineStarted es7 {cr};       // fixme: clang-3.9.0 compile this while gcc and clang-4.0.0 don't.
#else
      //EngineStarted es7 {cr};       // ko:no conversion between different strong bools

      bool ans = es && !cr;           // ok - explicit conversion implied by the use of &&
      BOOST_TEST(ans);
      bool ans2 = es || !cr;           // ok - explicit conversion implied by the use of &&
      BOOST_TEST(ans2);
#endif
      if (cr)                         // ok - explicit conversion implied by the use cr in a if
        BOOST_TEST(false);
      else
        BOOST_TEST(true);

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
      EngineStarted es1 {true};
      auto b = es1 || false;
      BOOST_TEST(b==true);
  }
  {
      EngineStarted es1 {false};
      auto b = es1 || false;
      BOOST_TEST(b==false);
  }
  {
      EngineStarted es1 {true};
      auto b = false || es1;
      BOOST_TEST(b==true);
  }
  {
      EngineStarted es1 {false};
      auto b = false || es1;
      BOOST_TEST(b==false);
  }
  {
      EngineStarted es1 {true};
      auto b = es1 || true;
      BOOST_TEST(b==true);
  }
  {
      EngineStarted es1 {false};
      auto b = es1 || true;
      BOOST_TEST(b==true);
  }
  {
      EngineStarted es1 {true};
      auto b = (true) || es1 ;
      BOOST_TEST(b==true);
  }
  {
      EngineStarted es1 {false};
      auto b = (true) || es1 ;
      BOOST_TEST(b==true);
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
      auto b = es1 && false;
      BOOST_TEST(!b);
  }
  {
      EngineStarted es1 {false};
      auto b = es1 && false;
      BOOST_TEST(!b);
  }
  {
      EngineStarted es1 {true};
      auto b = es1 && true;
      BOOST_TEST(b);
  }
  {
      EngineStarted es1 {false};
      auto b = es1 && true;
      BOOST_TEST(!b);
  }
  {
      EngineStarted es1 {true};
      EngineStarted es = ! es1 ;
      BOOST_TEST(es==EngineStarted{false});
  }
  {
      EngineStarted es1 {false};
      EngineStarted es = ! es1 ;
      BOOST_TEST(es==EngineStarted{true});
  }
  {
      EngineStarted es {true};
      if (es) // explicit conversion implied by the use inside an if condition
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
  {
    UEngineStarted es {true};        // ok
    UCrewReady     cr {false};       // ok

    bool b = bool(es);
    BOOST_TEST_EQ(b, true);
    BOOST_TEST(!cr);

  }
  {
      UEngineStarted es {true};
      if (es) // explicit conversion implied by the use inside an if condition
        BOOST_TEST(true);
      else
        BOOST_TEST(false);
  }
  {
      UEngineStarted es {true};
      f (bool(es)); // explicit conversion required
  }
  {
    //UEngineStarted es1 = true;     // error - explicit required

    //UEngineStarted es2 {nullptr};  // error void* deleted

    //UEngineStarted es3 {1};        // error int deleted
    //short s;
    //UEngineStarted es4 {s};        // error int deleted

    //UEngineStarted es5 {1.0};      // error double deleted
    //UEngineStarted es5 {1.0f};      // error double deleted

    // fixme: do we want this to fail
    //unsigned char s;
    //UEngineStarted es6 {s};        // error unsigned char deleted
  }
  {
    using Bool = stdex::boolean;
    BOOST_TEST(stdex::ordinal::size<Bool>() == 2);
    BOOST_TEST(stdex::ordinal::traits<Bool>::pos(Bool{false}) == 0);
    BOOST_TEST(stdex::ordinal::pos(Bool{false}) == 0);
    BOOST_TEST(stdex::ordinal::pos(Bool{true}) == 1);
    BOOST_TEST(stdex::ordinal::val<Bool>(0) == Bool{false});
    BOOST_TEST(stdex::ordinal::val<Bool>(1) == Bool{true});
    BOOST_TEST(stdex::ordinal::first<Bool>() == Bool{false});
    BOOST_TEST(stdex::ordinal::last<Bool>() == Bool{true});
    BOOST_TEST(stdex::ordinal::succ(Bool{false}) == Bool{true});
    BOOST_TEST(stdex::ordinal::pred(Bool{true}) == Bool{false});
  }
  {
    //stdex::boolean b{&S::foo}; // this fails as expected
    //UEngineStarted es {&S::foo}; // this fails as expected
  }
  {
    //stdex::boolean b{&f}; // this fails as expected
    //UEngineStarted es {&f}; // this fails as expected
  }
  return ::boost::report_errors();
}

