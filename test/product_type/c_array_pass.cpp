// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>

#include <experimental/product_type.hpp>

#include <boost/detail/lightweight_test.hpp>


int f(int,int,int) { return 0; }

int main()
{
  namespace stde = std::experimental;
  {
      int arr[] = {0,1,2};
      static_assert(! stde::has_tuple_like_access<int>::value, "Hrr");
      static_assert(! stde::has_tuple_like_access<decltype(arr)>::value, "Hrr");

      static_assert(3 == stde::product_type::size<decltype(arr)>::value, "Hrr");

      static_assert(std::is_same<int, stde::product_type::element_t<0,decltype(arr)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<3,decltype(arr)>>::value, "Hrr");// COMPILE FAIL AS REQUIRED
      static_assert(stde::is_product_type_v<decltype(arr)>, "Hrr");

      BOOST_TEST(3 == stde::product_type::size<decltype(arr)>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(arr));
      BOOST_TEST(1 == stde::product_type::get<1>(arr));
      BOOST_TEST(2 == stde::product_type::get<2>(arr));
      //auto x = stde::product_type::get<3>(arr); // COMPILE FAIL AS REQUIRED
  }
  {
      constexpr int arr[] = {0,1,2};
      static_assert(0 == stde::product_type::get<0>(arr), "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<0,decltype(arr)>>::value, "Hrr");
  }
  {
      const int arr[] = {0,1,2};
      static_assert(3 == stde::product_type::size<decltype(arr)>::value, "Hrr");

      BOOST_TEST(3 == stde::product_type::size<decltype(arr)>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(arr));
      BOOST_TEST(1 == stde::product_type::get<1>(arr));
      BOOST_TEST(2 == stde::product_type::get<2>(arr));
  }
  {
      int arr[] = {0,1,2};
      BOOST_TEST(0 == stde::product_type::apply(f, arr));
  }
  {
      int arr[] = {0,1,2};
      BOOST_TEST(std::make_tuple(0,1,2) == stde::product_type::to_tuple(arr));
  }
  {
    int p[] = {0,1};
    int q[] = {2,3};
    BOOST_TEST(std::make_tuple(0,1,2,3) == stde::product_type::cat(p, q));
  }
  {
    int p[] = {0,1};
    int q[] = {2,3};
    stde::swappable::swap(p,q);
    BOOST_TEST(0 == q[0]);
    BOOST_TEST(1 == q[1]);
    BOOST_TEST(2 == p[0]);
    BOOST_TEST(3 == p[1]);
  }
  return ::boost::report_errors();
}
