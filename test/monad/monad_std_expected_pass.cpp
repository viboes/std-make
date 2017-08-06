// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/monad.hpp>
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

int main()
{

  static_assert(stde::is_type_constructible<expected_sc<int>>::value, "ERROR");
  static_assert(std::is_base_of<stde::monad::tag, stde::monad::traits<expected_sc<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_monad<expected_sc<stde::_t>>::value, "ERROR");

  {
    std::error_code ec = std::make_error_code(std::errc(1));
    expected_sc<int> x = stde::make_unexpected(ec);
    expected_sc<int> y = stde::monad::bind(x,twice);
    BOOST_TEST(! y);
  }
  {
    int v=1;
    expected_sc<int> x = stde::make<expected_sc<stde::_t>>(v);
    expected_sc<int> y = stde::monad::bind(x,twice);
    BOOST_TEST(y);
    BOOST_TEST(*y == 2);
  }
  {
    int v=0;
    expected_sc<int> x = stde::make<expected_sc<stde::_t>>(v);
    expected_sc<int> y = stde::monad::bind(x,twice);
    BOOST_TEST(! y);
  }

  return ::boost::report_errors();
}
