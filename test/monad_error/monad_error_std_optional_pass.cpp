// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/monad_error.hpp>
// <experimental/optional.hpp>

#include <experimental/optional.hpp>

#include <boost/detail/lightweight_test.hpp>

std::experimental::optional<int> twice(int i) {
  if (i==0)
  {
    return std::experimental::nullopt;
  }
  return std::experimental::make_optional(2*i);
}

std::experimental::optional<int> recover( ) {
  return std::experimental::make_optional(-1);
}
std::experimental::optional<int> fwd() {
  return std::experimental::nullopt;
}

int main()
{
  namespace stde = std::experimental;

  static_assert(stde::is_type_constructible<stde::optional<int>>::value, "ERROR");
  static_assert(std::is_base_of<stde::monad_error::tag, stde::monad_error::traits<stde::optional<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_monad_error<stde::optional<stde::_t>>::value, "ERROR");

  {
    stde::optional<int> x = stde::monad_error::make_error<stde::optional>();
    stde::optional<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
    stde::optional<int> z = stde::monad_error::catch_error(y,recover);
    BOOST_TEST(z);
    BOOST_TEST_EQ(*z,-1);
  }
  {
    int v=1;
    stde::optional<int> x = stde::make<stde::optional>(v);
    stde::optional<int> y = stde::monad_error::bind(x,twice);
    stde::optional<int> z = stde::monad_error::catch_error(y,fwd);
    BOOST_TEST(z);
    BOOST_TEST(*z == 2);
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional>(v);
    stde::optional<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
  }

  return ::boost::report_errors();
}
