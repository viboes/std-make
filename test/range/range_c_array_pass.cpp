// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include <experimental/range.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>


int main()
{
  namespace stde = std::experimental;
  {
      int arr[] = {0,1};
      static_assert(stde::is_range<decltype(arr)>::value, "Hrr");

      auto it = stde::range::begin(arr);
      BOOST_TEST(0 == *it);
      ++it;
      BOOST_TEST(1 == *it );
      ++it;
      BOOST_TEST(it == stde::range::end(arr) );
  }
  {
      const int arr[] = {0,1};
      static_assert(stde::is_range<decltype(arr)>::value, "Hrr");

      auto it = stde::range::begin(arr);
      BOOST_TEST(0 == *it);
      ++it;
      BOOST_TEST(1 == *it );
      ++it;
      BOOST_TEST(it == stde::range::end(arr) );
  }
  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
