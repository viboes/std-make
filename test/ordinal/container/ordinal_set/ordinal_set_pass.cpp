// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/ordinal_set.hpp>

#include <experimental/ordinal_set.hpp>
#include "../../Bool.hpp"
#include "../../Bounded.hpp"

#include <boost/detail/lightweight_test.hpp>

int main()
{
#if __cplusplus >= 201402L

  namespace stde = std::experimental;
  using Indx = Bounded<1,4,unsigned char>;
  using OSet = stde::ordinal_set<Indx>;

  {
    OSet os;
    os[Indx{1}]=true;
    BOOST_TEST(os[Indx{1}]);
  }
  { // Constructible from Indx
    OSet e(Indx{2});
    BOOST_TEST(e[Indx{2}]==true);
  }
#endif
  return ::boost::report_errors();
}
