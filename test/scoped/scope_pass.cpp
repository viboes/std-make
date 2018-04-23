// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/scope.hpp>

#include <experimental/scope.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

using std::experimental::make_scope_exit;
using std::experimental::scope_exit;
#if defined JASEL_STD_HAS_UNCAUGHT_EXCEPTIONS
using std::experimental::make_scope_success;
using std::experimental::make_scope_fail;
using std::experimental::scope_fail;
using std::experimental::scope_success;
#endif


void thisIsATest() {
  std::ostringstream out { };
  {
    auto guard = make_scope_exit([&] {out << "done\n";});
  }
  BOOST_TEST_EQ("done\n", out.str());
}

void testScopeExitWithCPP17DeducingCtors() {
  std::ostringstream out { };
  {
    JASEL_SCOPE_EXIT(guard, [&] {out << "done\n";})
  }
  BOOST_TEST_EQ("done\n", out.str());
}
#if defined JASEL_STD_HAS_UNCAUGHT_EXCEPTIONS
void testScopeFailWithCPP17DeducingCtors(){
  std::ostringstream out { };
  {
    JASEL_SCOPE_FAIL(guard, [&] {out << "not done\n";});

  }
  BOOST_TEST_EQ("",out.str());
  try {
    JASEL_SCOPE_FAIL(guard, [&] {out << "done\n";});
    throw 0;
  } catch(int){}
  BOOST_TEST_EQ("done\n", out.str());
}
void testScopeSuccessWithCPP17DeducingCtors(){
  std::ostringstream out { };
  {
    JASEL_SCOPE_SUCCESS(guard, [&] {out << "done\n";});
  }
  BOOST_TEST_EQ("done\n", out.str());
  try {
    JASEL_SCOPE_SUCCESS(guard, [&] {out << "not done\n";});
    throw 0;
  } catch(int){}
  BOOST_TEST_EQ("done\n", out.str());
}
#endif

void testDismissedGuard() {
  std::ostringstream out { };
  {
    auto guard = make_scope_exit([&] {out << "done1\n";});
    auto guard2dismiss = make_scope_exit([&] {out << "done2\n";});
    guard2dismiss.release();
  }
  BOOST_TEST_EQ("done1\n", out.str());
}
void testThrowDoesntCrashIt() { // LEWG wants it to crash!
  std::ostringstream out { };
  {
    auto guard = make_scope_exit([&] {out << "done\n";});
    auto guard1 = make_scope_exit([] {throw 42;});
    guard1.release(); // we no longer want throwing scope guards
  }
  BOOST_TEST_EQ("done\n", out.str());

}
void testScopeExitWithReferenceWrapper(){
  std::ostringstream out { };
  const auto &lambda = [&] {out << "lambda done.\n";};
  {
    auto guard=make_scope_exit(std::cref(lambda));
  }
  BOOST_TEST_EQ("lambda done.\n",out.str());
}

int main()
{
  thisIsATest() ;
  testScopeExitWithCPP17DeducingCtors();
#if defined JASEL_STD_HAS_UNCAUGHT_EXCEPTIONS
  testScopeFailWithCPP17DeducingCtors();
  testScopeSuccessWithCPP17DeducingCtors();
#endif
  testDismissedGuard();
  testThrowDoesntCrashIt();
  testScopeExitWithReferenceWrapper();
  return ::boost::report_errors();
}

