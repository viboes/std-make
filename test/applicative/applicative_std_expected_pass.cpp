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

namespace stde = std::experimental;
template <class T>
using expected_sc = stde::expected<T, std::error_code>;

int main()
{

  static_assert(stde::is_type_constructible<expected_sc<int>>::value, "ERROR");
  static_assert(std::is_base_of<stde::applicative::tag, stde::applicative::traits<expected_sc<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_applicative<expected_sc<stde::_t>>::value, "ERROR");
  static_assert(std::is_same<
      std::experimental::fundamental_v3::expected<int, std::error_code>,
      std::experimental::fundamental_v3::expected<int, std::error_code>::template rebind<int>::type
    >::value, "ERROR");
  static_assert(std::is_same<
      std::experimental::fundamental_v3::expected<int, std::error_code>,
      stde::meta::v1::rebind_t<std::experimental::fundamental_v3::expected<int, std::error_code>, int>
    >::value, "ERROR");
  {
    std::error_code ec = std::make_error_code(std::errc(1));
    expected_sc<int> x = stde::make_unexpected(ec);
    expected_sc<int(*)(int)> f = stde::make_unexpected(ec);
    expected_sc<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! y);
  }
  {
    std::error_code ec = std::make_error_code(std::errc(1));
    expected_sc<int> x = stde::make<stde::expected<stde::_t, std::error_code>>(1);
    expected_sc<int(*)(int)> f = stde::make_unexpected(ec);
    expected_sc<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! y);
  }
  {
    std::error_code ec = std::make_error_code(std::errc(1));
    expected_sc<int> x = stde::applicative::pure<expected_sc<int>>(1);
    expected_sc<int(*)(int)> f = stde::make_unexpected(ec);
    expected_sc<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! y);
  }
  {
    std::error_code ec = std::make_error_code(std::errc(1));
    expected_sc<int> x = stde::make_unexpected(ec);
    expected_sc<int(*)(int)> f = stde::make<stde::expected<stde::_t, std::error_code>>(twice);
    expected_sc<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(! y);
  }
  {
    int v=1;
    expected_sc<int> x = stde::make<stde::expected<stde::_t, std::error_code>>(v);
    expected_sc<int(*)(int)> f = stde::make<stde::expected<stde::_t, std::error_code>>(twice);
    expected_sc<int> y = stde::applicative::ap(f, x);
    BOOST_TEST(*y == 2);
  }

  return ::boost::report_errors();
}
