// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>

#include <experimental/product_type.hpp>
#include <array>

#include <boost/detail/lightweight_test.hpp>

int main()
{
  namespace stde = std::experimental;
  {
      using T = std::array<int,3>;
      static_assert(stde::detail::has_tuple_like_size_access<T>::value, "Hrr");
      static_assert(stde::detail::has_tuple_like_element_access<0, T>::value, "Hrr");
      static_assert(stde::detail::has_tuple_like_get_access<0, T>::value, "Hrr");
      static_assert(stde::has_tuple_like_access<T>::value, "Hrr");


      T p  = { {0,1,2} };
      static_assert(3 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      BOOST_TEST(3 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      BOOST_TEST(2 == stde::product_type::get<2>(p));
      //auto x = stde::product_type::get<3>(p); // COMPILE FAIL AS REQUIRED
  }
  {
      using T = std::array<int,3>;
      const T p  = { {0,1,2} };
      static_assert(3 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<const int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<const int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      BOOST_TEST(3 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      BOOST_TEST(2 == stde::product_type::get<2>(p));
      //auto x = stde::product_type::get<3>(p); // COMPILE FAIL AS REQUIRED
  }
  return ::boost::report_errors();
}
