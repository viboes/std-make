// Copyright (C) 2018 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/ordinal_ordered_map.hpp>

#include <experimental/ordinal_ordered_map.hpp>
#include "../../Bool.hpp"
#include "../../Bounded.hpp"

#include <boost/detail/lightweight_test.hpp>

int main()
{

  namespace stde = std::experimental;
  using Indx = Bounded<1,4,unsigned char>;
  using OMap = stde::ordinal_ordered_map<Indx, int>;

  {// Default Constructor
    OMap om;
    BOOST_TEST( 0 == om.size() );
  }
  {// insert
    OMap om;
    om.insert(std::make_pair(Indx{1}, 123));
    BOOST_TEST( 1 == om.size() );
    BOOST_TEST( om.find(Indx{1}) != om.end() );
    BOOST_TEST( om.find(Indx{1})->second == 123 );
    BOOST_TEST( om.find(Indx{2}) == om.end() );
  }
  {// find
    OMap om;
    om.insert(std::make_pair(Indx{1}, 123));
    om.find(Indx{1});
    BOOST_TEST( om.find(Indx{1}) != om.end() );
    BOOST_TEST( om.find(Indx{1})->second == 123 );
    BOOST_TEST( om.find(Indx{2}) == om.end() );
  }
  return ::boost::report_errors();
}
