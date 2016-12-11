// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/p_functor.hpp>
// <experimental/utility.hpp> tuple

#include <experimental/tuple.hpp>
#include <experimental/p_functor.hpp>
#include <experimental/product_type.hpp>

#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

int main()
{
#if __cplusplus >= 201402L
  namespace stde = std::experimental;

  using T = std::tuple<int, int>;

  static_assert(stde::is_product_type_v<T>, "ERROR");
  static_assert(stde::is_type_constructible_v<T>, "ERROR");
  static_assert(std::is_base_of<stde::p_functor::tag, stde::p_functor::traits<std::tuple<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_p_functor<std::tuple<stde::_t>>::value, "ERROR");

  {
    int v=1;
    T x = std::make_tuple(v,v);
    BOOST_TEST(std::get<0>(x) == 1);
    BOOST_TEST(std::get<1>(x) == 1);
    T y = stde::p_functor::transform(x, twice);
    BOOST_TEST(std::get<0>(y) == 2);
    BOOST_TEST(std::get<1>(y) == 2);
  }
#endif

  return ::boost::report_errors();
}
