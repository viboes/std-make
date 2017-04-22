// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_bool.hpp>

#include <experimental/strong_bool.hpp>
#include <sstream>
#include <cassert>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

static_assert(sizeof(stdex::boolean)==1, "");
static_assert(sizeof(stdex::boolean8_t)==1, "");

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
  {
      stdex::boolean b = true;
      BOOST_TEST(b);
  }
#if defined COMPILER_ERROR
  {
      stdex::boolean es2 {nullptr};  // error void* deleted

      stdex::boolean es3 {1};        // error int deleted
      short s;
      stdex::boolean es4 {s};        // error int deleted

      stdex::boolean es5 {1.0};      // error double deleted
  }
#endif
  {
      stdex::boolean es = true;        // ok
      stdex::boolean8_t     cr = false;       // ok
      BOOST_TEST(es);
      BOOST_TEST(!cr);

      stdex::boolean es6 =true;
      BOOST_TEST(es6);
      //stdex::boolean b6 =cr;       // fixme: do we want these to be convertible?
      //bool b = cr;                  // error as expected- implicit conversion required

      //stdex::boolean es7 {cr};       // fixme:no conversion between different strong bools

      bool ans = es && !cr;           // ok - explicit conversion to bool implied by the use of &&
      BOOST_TEST(ans);
      bool ans2 = es || !cr;           // ok - explicit conversion implied by the use of ||
      BOOST_TEST(ans2);
      bool ans3 = !cr;           // ok - explicit conversion implied by the use of !
      BOOST_TEST(ans3);

      if (cr)                         // ok - explicit conversion implied by the use cr in a if
        BOOST_TEST(false);
      else
        BOOST_TEST(true);

  }
  {
      //stdex::boolean es {Y{}};        // error, explicit conversion
      stdex::boolean es {X{}};        // ok, implicit conversion from UDT
  }
  {
      stdex::boolean es1 = true;
      stdex::boolean es2 = false;
      BOOST_TEST(es2<es1);
      BOOST_TEST(es2!=es1);
      //stdex::boolean8_t     cr = false;
      //BOOST_TEST(cr<es1); // fixme
  }
  {
      stdex::boolean es1 = true;
      stdex::boolean es2 = false;
      stdex::boolean es = es1 || es2;
      BOOST_TEST(es==true);
  }
  {
      stdex::boolean es1 = true;
      stdex::boolean es = es1 || false;
      BOOST_TEST(es==true);
  }
  {
      stdex::boolean es1 = true;
      auto b = es1 || false;
      BOOST_TEST(b==true);
  }
  {
      stdex::boolean es1 = false;
      auto b = es1 || false;
      BOOST_TEST(b==false);
  }
  {
      stdex::boolean es1 = true;
      auto b = false || es1;
      BOOST_TEST(b==true);
  }
  {
      stdex::boolean es1 = false;
      auto b = false || es1;
      BOOST_TEST(b==false);
  }
  {
      stdex::boolean es1 = true;
      auto b = es1 || true;
      BOOST_TEST(b==true);
  }
  {
      stdex::boolean es1 = false;
      auto b = es1 || true;
      BOOST_TEST(b==true);
  }
  {
      stdex::boolean es1 = true;
      auto b = true || es1 ;
      BOOST_TEST(b==true);
  }
  {
      stdex::boolean es1 = false;
      auto b = true || es1 ;
      BOOST_TEST(b==true);
  }
  {
      stdex::boolean es2 = false;
      stdex::boolean es = true || es2;
      BOOST_TEST(es==true);
  }
  {
      stdex::boolean es1 = true;
      stdex::boolean es2 = false;
      stdex::boolean es = es1 && es2;
      BOOST_TEST(es==false);
  }
  {
      stdex::boolean es1 = true;
      auto b = es1 && false;
      BOOST_TEST(!b);
  }
  {
      stdex::boolean es1 = false;
      auto b = es1 && false;
      BOOST_TEST(!b);
  }
  {
      stdex::boolean es1 = true;
      auto b = es1 && true;
      BOOST_TEST(b);
  }
  {
      stdex::boolean es1 = false;
      auto b = es1 && true;
      BOOST_TEST(!b);
  }
  {
      stdex::boolean es1 = true;
      stdex::boolean es = ! es1 ;
      BOOST_TEST(es==false);
  }
  {
      stdex::boolean es1 = false;
      stdex::boolean es = ! es1 ;
      BOOST_TEST(es==true);
  }
  {
      stdex::boolean es = true;
      if (es) // explicit conversion implied by the use inside an if condition
        BOOST_TEST(true);
      else
        BOOST_TEST(false);
  }
  {
      stdex::boolean es = true;
      f (bool(es)); // explicit conversion required
  }

  {
      stdex::boolean es = true;
      std::stringstream os;
      os << std::boolalpha << es;
      std::cout << std::boolalpha << true;

      BOOST_TEST_EQ(os.str(), "true");
  }
  {
      stdex::boolean es;
      std::stringstream s;
      s << std::boolalpha << true;
      s >> std::boolalpha >> es;

      BOOST_TEST_EQ(bool(es), true);
  }


  {
    using Bool = stdex::boolean;
    assert(stdex::ordinal::size<Bool>() == 2);
    assert(stdex::ordinal::traits<Bool>::pos(Bool{false}) == 0);
    assert(stdex::ordinal::pos(Bool{false}) == 0);
    assert(stdex::ordinal::pos(Bool{true}) == 1);
    assert(stdex::ordinal::val<Bool>(0) == Bool{false});
    assert(stdex::ordinal::val<Bool>(1) == Bool{true});
    assert(stdex::ordinal::first<Bool>() == Bool{false});
    assert(stdex::ordinal::last<Bool>() == Bool{true});
    assert(stdex::ordinal::succ(Bool{false}) == Bool{true});
    assert(stdex::ordinal::pred(Bool{true}) == Bool{false});
  }
  {
    //stdex::boolean b{&S::foo}; // this fails as expected
  }
  {
    //stdex::boolean b{&f}; // this fails as expected
  }
  return ::boost::report_errors();
}

