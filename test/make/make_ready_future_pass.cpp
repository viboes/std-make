// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/future.hpp>

//#include <experimental/make.hpp>
//#include <experimental/meta.hpp>
#include <experimental/future.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <iostream>

struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int main()
{
  namespace stde = std::experimental;

  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<void> x = stde::make_ready_future();
    std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
    x.get();
    std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = stde::make_ready_future(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make_ready_future(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }


  return ::boost::report_errors();
}
