// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/ordinal_array.hpp>

#include <experimental/ordinal_array.hpp>
#include "../../Bool.hpp"
#include "../../Bounded.hpp"

#include <boost/detail/lightweight_test.hpp>

int main()
{

  namespace stde = std::experimental;
  using Indx = Bounded<1,3,unsigned char>;

  {
    stde::ordinal_array<int,Indx> arr;
    BOOST_TEST(arr.size() == 3);
  }
  // fixme: shouldn't {1, 2, 3.5} work?
  {
    stde::ordinal_array<int,Indx> c {{1, 2, 3}};
    BOOST_TEST(c.size() == 3);
    BOOST_TEST(c[Indx{1}] == 1);
    BOOST_TEST(c[Indx{2}] == 2);
    BOOST_TEST(c[Indx{3}] == 3);
  }
  return ::boost::report_errors();
}
