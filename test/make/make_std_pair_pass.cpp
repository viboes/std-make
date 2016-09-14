// Copyright (C) 2015-2016 Vicente J. Botet Escriba
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
#include <experimental/utility.hpp>
#include <boost/detail/lightweight_test.hpp>


int main()
{
  namespace stde = std::experimental;
  static_assert(stde::meta::is_callable<std::pair<stde::_t,stde::_t>(int, short)>::value, "ERROR");
  static_assert(stde::meta::is_callable<std::pair<stde::_t,stde::_t>(int, short), std::pair<int, short>>::value, "ERROR");
  {
    int v=0;
    std::pair<int, int> x = stde::make<std::pair>(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  {
    int v=0;
    std::pair<long, long> x = stde::make<std::pair<long, long> >(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  {
    int v=0;
    std::pair<int, int> x = stde::make<std::pair<stde::_t, stde::_t> >(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
#if __cplusplus >= 201402L
  {
    constexpr int v=0;
    constexpr std::pair<int, int> x = stde::make<std::pair>(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
#endif
  return ::boost::report_errors();
}
