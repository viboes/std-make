// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/ordinal_range.hpp>

#include <experimental/ordinal_range.hpp>
#include "../../Bool.hpp"
#include "../../Bounded.hpp"

#include <boost/detail/lightweight_test.hpp>

int main()
{
  namespace stde = std::experimental;

  using Indx = Bounded<1,4,unsigned char>;

  {
    stde::ordinal_range<Indx> rng;
    auto b = rng.begin();
    BOOST_TEST(b->value==1);
  }
  return ::boost::report_errors();
}
