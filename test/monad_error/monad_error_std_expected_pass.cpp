// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/monad_error.hpp>
// <experimental/expected.hpp>

#include <experimental/expected.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stde = std::experimental;
template <class T>
using expected_sc = stde::expected<T, std::error_code>;

expected_sc<int> twice(int i) {
  if (i==0)
  {
    std::error_code ec = std::make_error_code(std::errc(1));
    return stde::make_unexpected(ec);
  }
  return stde::make_expected(2*i);
}

expected_sc<int> recover(std::error_code const& ) {
  return stde::make_expected(-1);
}
expected_sc<int> fwd(std::error_code const& ec) {
  return stde::make_unexpected(ec);
}
int adapt(std::error_code const& ) {
  return -1;
}

int main()
{

  static_assert(stde::is_type_constructible<expected_sc<int>>::value, "ERROR");
  static_assert(std::is_base_of<stde::monad_error::tag, stde::monad_error::traits<expected_sc<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_monad_error<expected_sc<stde::_t>>::value, "ERROR");

  {
    std::error_code ec = std::make_error_code(std::errc(1));
    expected_sc<int> x = stde::monad_error::make_error<expected_sc<stde::_t>>(ec);
    expected_sc<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
    expected_sc<int> z = stde::monad_error::catch_error(y,recover);
    BOOST_TEST(z);
    BOOST_TEST_EQ(*z,-1);
  }
  {
    std::error_code ec = std::make_error_code(std::errc(1));
    expected_sc<int> x = stde::monad_error::make_error<expected_sc<stde::_t>>(ec);
    expected_sc<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
    stde::expected<int, int> z = stde::monad_error::adapt_error(y,adapt);
    BOOST_TEST( ! z);
    BOOST_TEST_EQ(z.error(),-1);
  }
  {
    int v=1;
    expected_sc<int> x = stde::make<expected_sc<stde::_t>>(v);
    expected_sc<int> y = stde::monad_error::bind(x,twice);
    expected_sc<int> z = stde::monad_error::catch_error(y,fwd);
    BOOST_TEST(z);
    BOOST_TEST(*z == 2);
  }
  {
    int v=1;
    expected_sc<int> x = stde::make<expected_sc<stde::_t>>(v);
    expected_sc<int> y = stde::monad_error::bind(x,twice);
    stde::expected<int, int> z = stde::monad_error::adapt_error(y,adapt);
    BOOST_TEST(z);
    BOOST_TEST(*z == 2);
  }
  {
    int v=0;
    expected_sc<int> x = stde::make<expected_sc<stde::_t>>(v);
    expected_sc<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
  }

  return ::boost::report_errors();
}
