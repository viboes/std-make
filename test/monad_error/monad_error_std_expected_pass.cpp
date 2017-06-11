// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/monad_error.hpp>
// <experimental/expected.hpp>

#include <experimental/expected.hpp>

#include <boost/detail/lightweight_test.hpp>

std::experimental::expected<int> twice(int i) {
  if (i==0)
  {
    std::error_code ec = std::make_error_code(std::errc(1));
    return std::experimental::make_unexpected(ec);
  }
  return std::experimental::make_expected(2*i);
}

std::experimental::expected<int> recover(std::error_code const& ) {
  return std::experimental::make_expected(-1);
}
std::experimental::expected<int> fwd(std::error_code const& ec) {
  return std::experimental::make_unexpected(ec);
}
int adapt(std::error_code const& ) {
  return -1;
}

int main()
{
  namespace stde = std::experimental;

  static_assert(stde::is_type_constructible<stde::expected<int>>::value, "ERROR");
  static_assert(std::is_base_of<stde::monad_error::tag, stde::monad_error::traits<stde::expected<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_monad_error<stde::expected<stde::_t>>::value, "ERROR");

  {
    std::error_code ec = std::make_error_code(std::errc(1));
    stde::expected<int> x = stde::monad_error::make_error<stde::expected>(ec);
    stde::expected<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
    stde::expected<int> z = stde::monad_error::catch_error(y,recover);
    BOOST_TEST(z);
    BOOST_TEST_EQ(*z,-1);
  }
  {
    std::error_code ec = std::make_error_code(std::errc(1));
    stde::expected<int> x = stde::monad_error::make_error<stde::expected>(ec);
    stde::expected<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
    stde::expected<int, int> z = stde::monad_error::adapt_error(y,adapt);
    BOOST_TEST( ! z);
    BOOST_TEST_EQ(z.error(),-1);
  }
  {
    int v=1;
    stde::expected<int> x = stde::make<stde::expected>(v);
    stde::expected<int> y = stde::monad_error::bind(x,twice);
    stde::expected<int> z = stde::monad_error::catch_error(y,fwd);
    BOOST_TEST(z);
    BOOST_TEST(*z == 2);
  }
  {
    int v=1;
    stde::expected<int> x = stde::make<stde::expected>(v);
    stde::expected<int> y = stde::monad_error::bind(x,twice);
    stde::expected<int, int> z = stde::monad_error::adapt_error(y,adapt);
    BOOST_TEST(z);
    BOOST_TEST(*z == 2);
  }
  {
    int v=0;
    stde::expected<int> x = stde::make<stde::expected>(v);
    stde::expected<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
  }

  return ::boost::report_errors();
}
