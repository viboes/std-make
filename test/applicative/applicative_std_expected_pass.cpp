// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/applicative.hpp>
// <experimental/expected.hpp>

#include <experimental/expected.hpp>

#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

int main()
{
#if __cplusplus >= 201402L
  namespace stde = std::experimental;

  static_assert(stde::is_type_constructible_v<stde::expected<int>>, "ERROR");
  static_assert(std::is_base_of<stde::applicative::tag, stde::applicative::traits<stde::expected<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_applicative<stde::expected<stde::_t>>::value, "ERROR");

  {
    stde::expected<int> x = stde::make_unexpected(1);
    stde::expected<int(*)(int)> f = stde::make_unexpected(1);
    stde::expected<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! y);
  }
  {
    stde::expected<int> x = stde::make<stde::expected>(1);
    stde::expected<int(*)(int)> f = stde::make_unexpected(1);
    stde::expected<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! y);
  }
  {
    stde::expected<int> x = stde::make_unexpected(1);
    stde::expected<int(*)(int)> f = stde::make<stde::expected>(twice);
    stde::expected<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! y);
  }
  {
    int v=1;
    stde::expected<int> x = stde::make<stde::expected>(v);
    stde::expected<int(*)(int)> f = stde::make<stde::expected>(twice);
    stde::expected<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(*y == 2);
  }
#endif

  return ::boost::report_errors();
}
