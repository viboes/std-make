// Copyright (C) 2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/make.hpp>

//  template <template <class ...> class M, class X>
//  auto make(X&& x);
//  template <class M, class X>
//  auto make(X&& x);
//  template <class M, class ...Args>
//  auto make(Args&& ...args);


#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/tuple.hpp>
#include <boost/detail/lightweight_test.hpp>


int main()
{
  namespace stde = std::experimental;
  {
    int v=0;
    std::tuple<int, int> x = stde::make<std::tuple>(v, v);
    BOOST_TEST(std::get<0>(x) == 0);
    BOOST_TEST(std::get<1>(x) == 0);
  }
  {
    int v=0;
    std::tuple<long, long> x = stde::make<std::tuple<long, long> >(v, v);
    BOOST_TEST(std::get<0>(x) == 0);
    BOOST_TEST(std::get<1>(x) == 0);
  }
  {
    int v=0;
    std::tuple<int, int> x = stde::make<std::tuple<stde::_t> >(v, v);
    BOOST_TEST(std::get<0>(x) == 0);
    BOOST_TEST(std::get<1>(x) == 0);
  }
#if __cplusplus >= 201402L
  {
    constexpr int v=0;
    constexpr std::tuple<int, int> x = stde::make<std::tuple>(v, v);
    BOOST_TEST(std::get<0>(x) == 0);
    BOOST_TEST(std::get<1>(x) == 0);
  }
#endif
  return ::boost::report_errors();
}
