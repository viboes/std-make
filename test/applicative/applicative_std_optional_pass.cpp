// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/applicative.hpp>
// <experimental/optional.hpp>


#include <boost/detail/lightweight_test.hpp>
#include <experimental/applicative.hpp>
#include <experimental/optional.hpp>

int twice(int i) {
  return 2*i;
}

int main()
{
#if __cplusplus >= 201402L

  namespace stde = std::experimental;

  static_assert(stde::is_nullable_v<stde::optional<int>>, "ERROR");
  static_assert(stde::is_type_constructible_v<stde::optional<int>>, "ERROR");
  static_assert(stde::is_applicative<stde::optional<stde::_t>>::value, "ERROR");

  {
    stde::optional<int> x = stde::none<stde::optional>();
    stde::optional<int(*)(int)> f = stde::none<stde::optional>();
    stde::optional<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    stde::optional<int> x = stde::make<stde::optional>(1);
    stde::optional<int(*)(int)> f = stde::none<stde::optional>();
    stde::optional<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    stde::optional<int> x = stde::none<stde::optional>();
    stde::optional<int(*)(int)> f = stde::make<stde::optional>(twice);
    stde::optional<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    int v=1;
    stde::optional<int> x = stde::make<stde::optional>(v);
    stde::optional<int(*)(int)> f = stde::make<stde::optional>(twice);
    stde::optional<int> y = stde::applicative::ap(f, x);
    (void)y;
    BOOST_TEST(stde::value_or_error::has_value(y));
    //BOOST_TEST(stde::deref(y) == 2);
    BOOST_TEST(stde::value_or_error::deref(y) == 2);
    BOOST_TEST(*y == 2);
  }
#endif

  return ::boost::report_errors();
}
