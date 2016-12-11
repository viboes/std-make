// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/applicative.hpp>
// <experimental/tuple.hpp>

#include <experimental/tuple.hpp>

#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

int main()
{
#if __cplusplus >= 201402L
  namespace stde = std::experimental;

  static_assert(stde::is_type_constructible_v<std::pair<int, int>>, "ERROR");
  static_assert(std::is_base_of<stde::applicative::tag, stde::applicative::traits<std::tuple<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_applicative<std::tuple<stde::_t>>::value, "ERROR");

  {
    int v=1;
    std::tuple<int> x = stde::make<std::tuple>(v);
    std::tuple<int(*)(int)> f = stde::make<std::tuple>(twice);
    std::tuple<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(std::get<0>(y) == 2);
  }
  {
    int v=1;
    std::tuple<int, int> x = stde::make<std::tuple>(v,v);
    auto f = stde::make<std::tuple>(twice,twice);
    std::tuple<int, int> y = stde::applicative::ap(f, x);
    BOOST_TEST(std::get<0>(y) == 2);
    BOOST_TEST(std::get<1>(y) == 2);
  }
#endif

  return ::boost::report_errors();
}
