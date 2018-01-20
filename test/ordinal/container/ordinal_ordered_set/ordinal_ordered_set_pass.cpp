// Copyright (C) 2018 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/ordinal_set.hpp>

#include <experimental/ordinal_ordered_set.hpp>
#include "../../Bool.hpp"
#include "../../Bounded.hpp"

#include <boost/detail/lightweight_test.hpp>

int main()
{

  namespace stde = std::experimental;
  using Indx = Bounded<1,4,unsigned char>;
  using OSet = stde::ordinal_ordered_set<Indx>;

  {// Default Constructor
    OSet os;
    BOOST_TEST( 0 == os.size() );
  }
  {// insert
    OSet os;
    os.insert(Indx{1});
    BOOST_TEST( 1 == os.size() );
    BOOST_TEST( os.find(Indx{2}) == os.end() );
    BOOST_TEST( *os.find(Indx{1}) == Indx{1} );
  }
  {// find
    OSet os;
    os.insert(Indx{1});
    os.find(Indx{1});
    BOOST_TEST( os.find(Indx{1}) != os.end() );
    BOOST_TEST( os.find(Indx{2}) == os.end() );
  }
  return ::boost::report_errors();
}
