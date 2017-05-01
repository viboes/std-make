// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/applicative.hpp>
// <experimental/array.hpp>

#if defined JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
#warning JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
int main()
{
}
#else

//#define JASEL_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL

#include <experimental/array.hpp>

#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

int main()
{
#if __cplusplus >= 201402L
  namespace stde = std::experimental;

  static_assert(std::is_base_of<stde::applicative::tag, stde::applicative::traits<stde::array_tc>> ::value, "ERROR");
  static_assert(stde::is_applicative<stde::array_tc>::value, "ERROR");

  {
    int v=1;
    std::array<int, 1> x = stde::make<stde::array_tc>(v);
    std::array<int(*)(int), 1> f = stde::make<stde::array_tc>(twice);
    std::array<int, 1> y = stde::applicative::ap(f, x);
    BOOST_TEST(std::get<0>(y) == 2);
  }
  {
    int v=1;
    std::array<int, 2> x = stde::make<stde::array_tc>(v,v);
    auto f = stde::make<stde::array_tc>(twice, twice);
    std::array<int, 2> y = stde::applicative::ap(f, x);
    BOOST_TEST(std::get<0>(y) == 2);
    BOOST_TEST(std::get<1>(y) == 2);
  }
#endif

  return ::boost::report_errors();
}
#endif
