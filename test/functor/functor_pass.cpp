// Copyright (C) 2014-2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/functor.hpp>

#include <experimental/functor.hpp>
#include <experimental/value_or_none.hpp>
#include <experimental/make.hpp>
#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

int main()
{
  namespace stde = std::experimental;

  static_assert(! stde::is_functor<int>::value, "");

  static_assert(stde::is_nullable<int*>::value, "ERROR");
  static_assert(stde::is_type_constructible<int*>::value, "ERROR");
  static_assert(stde::is_functor<int*>::value, "ERROR");

  {
    int* x = stde::none<int*>();
    BOOST_TEST(! x);
    BOOST_TEST(! stde::has_value(x));
    BOOST_TEST(x == stde::none());
    BOOST_TEST(stde::none() == x);
    BOOST_TEST(x == stde::none<int*>());

    int* y = stde::functor::transform(x, twice);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    int v=1;
    int* x = stde::make<int*>(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(x != stde::none());
    int* y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
    delete x; // don't use T* when you must manage memory. Use instead unique_ptr
    delete y;
    x = stde::make<int*>(2);
    BOOST_TEST(*x == 2);
    delete x;
  }
  {
    int v=1;
    const int* x = stde::make<int*>(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(x != stde::none());

    int* y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
    delete x;
    delete y;
  }
  return ::boost::report_errors();
}
