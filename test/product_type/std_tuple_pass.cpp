// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>

#include <experimental/product_type.hpp>
#include <tuple>

#include <boost/detail/lightweight_test.hpp>

int f(int,int) { return 0; }

int main()
{
  namespace stde = std::experimental;
  {
      using T = std::tuple<int,int>;
      static_assert(stde::has_tuple_like_access<T>::value, "Hrr");

      T p{0,1};
      static_assert(2 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      BOOST_TEST(2 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      //auto x = stde::product_type::get<2>(p); // COMPILE FAIL AS REQUIRED
  }
  {
      using T = std::tuple<int,int>;
      const T p{0,1};
      static_assert(2 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<const int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      BOOST_TEST(2 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      //auto x = stde::product_type::get<2>(p); // COMPILE FAIL AS REQUIRED

  }
  {
    using T = std::pair<int,int>;
    const T p = {0,1};
    BOOST_TEST(0 == stde::product_type::apply(f, p));
  }
  return ::boost::report_errors();
}
