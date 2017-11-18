// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/value_or_error.hpp>

#include <experimental/value_or_error.hpp>
#include <experimental/value_or_none.hpp>
#include <experimental/meta.hpp>

//#include <memory>
//#include <algorithm>
//#include <iterator>
//#include <vector>
#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

template <class T>
struct check;

template <class T>
struct A
{
  T v;
  A(): v(2) {}
  A(T v): v(std::move(v)) {}
};
template <class T>
struct A<T&>
{
  T* ptr;
  A(T& v): ptr(&v) {}
};
template <>
struct A<void>
{
  int v;
  A() : v(0) {}
};

int main()
{
  namespace stde = std::experimental;

//  static_assert(std::is_same<
//      stde::value_or_error::value_type_t<int*>,
//      int
//    >::value, "ERROR");
//  static_assert(stde::is_value_or_error<int*>::value, "ERROR");
//  {
//    int * x = nullptr;
//    BOOST_TEST(! stde::value_or_error::has_value(x));
//    BOOST_TEST(stde::value_or_error::error(x) == nullptr);
//  }
//  {
//    const int * x = nullptr;
//    BOOST_TEST(! stde::value_or_error::has_value(x));
//    BOOST_TEST(stde::value_or_error::error(x) == nullptr);
//  }
//  {
//    int * x = new int(1);
//    BOOST_TEST(stde::value_or_error::has_value(x));
//    BOOST_TEST(stde::value_or_error::deref(x) == 1);
//    delete x;
//  }
//  {
//    const int * x = new int(1);
//    BOOST_TEST(stde::value_or_error::has_value(x));
//    BOOST_TEST(stde::value_or_error::deref(x) == 1);
//    delete x;
//  }
//  //value_or_error::value_or
//  {
//    int * x = nullptr;
//    auto y = stde::value_or_error::value_or(x, 1);
//    BOOST_TEST_EQ(y, 1);
//  }
//  {
//    int * x = new int(1);
//    int y = stde::value_or_error::value_or(x, 2);
//    delete x;
//    BOOST_TEST(y == 1);
//  }
//  //value_or_error::apply_or
//  {
//    int * x = nullptr;
//    int y = stde::value_or_error::apply_or(x, twice, 1);
//    BOOST_TEST(y == 1);
//  }
//  {
//    int * x = new int(1);
//    int y = stde::value_or_error::apply_or(x, twice, -1);
//    delete x;
//    BOOST_TEST(y == 2);
//  }
  return ::boost::report_errors();
}
