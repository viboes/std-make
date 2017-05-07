// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/future.hpp>

#include <experimental/future.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <iostream>

int main()
{
  namespace stde = std::experimental;
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    // This must fail as 1 is not an lvalue.
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//    error: no matching function for call to 'make_ready_future'
//        std::future<int&> x = stde::make_ready_future<int&>(1);
    std::future<int&> x = stde::make_ready_future<int&>(1);
  }

  return ::boost::report_errors();
}
