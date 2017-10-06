// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/monad_error.hpp>
// <experimental/optional.hpp>

#include <experimental/memory.hpp>

#include <boost/detail/lightweight_test.hpp>

std::unique_ptr<int> twice(int i) {
  if (i==0)
  {
    return nullptr;
  }
  return std::experimental::make<std::unique_ptr>(2*i);
}

std::unique_ptr<int> recover( ) {
  return std::experimental::make<std::unique_ptr>(-1);
}
std::unique_ptr<int> fwd() {
  return nullptr;
}

int main()
{
  namespace stde = std::experimental;

  static_assert(stde::is_monad_error<std::unique_ptr<stde::_t>>::value, "ERROR");

  {
    std::unique_ptr<int> x = stde::monad_error::make_error<std::unique_ptr>();
    std::unique_ptr<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
    std::unique_ptr<int> z = stde::monad_error::catch_error(y,recover);
    BOOST_TEST(z);
    BOOST_TEST_EQ(*z,-1);
  }
  {
    int v=1;
    std::unique_ptr<int> x = stde::make<std::unique_ptr>(v);
    std::unique_ptr<int> y = stde::monad_error::bind(x,twice);
    std::unique_ptr<int> z = stde::monad_error::catch_error(y,fwd);
    BOOST_TEST(z);
    BOOST_TEST(*z == 2);
  }
  {
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr>(v);
    std::unique_ptr<int> y = stde::monad_error::bind(x,twice);
    BOOST_TEST(! y);
  }

  return ::boost::report_errors();
}
