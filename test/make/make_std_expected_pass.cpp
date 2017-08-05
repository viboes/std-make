// Copyright (C) 2014,2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/make.hpp>

//  template <template <class ...> M, class X>
//  auto make(X&& x);
//  template <class M, class ...Args>
//  auto make(Args&& ...args);


#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/expected.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace stde = std::experimental;
template <class T>
using expected_sc = stde::expected<T, std::error_code>;

struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int main()
{
  {
    int v=0;
    expected_sc<int> x = stde::make<expected_sc<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    expected_sc<int> x = stde::make<expected_sc<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    auto x = stde::make<stde::expected<int, std::string>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    expected_sc<A> x = stde::make<expected_sc<A>>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    int v=1;
    auto x = stde::make<stde::expected<A, std::string>>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    expected_sc<int> x = stde::make<expected_sc<int>>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
  }
  {
    auto x = stde::make<stde::expected<int, std::string>>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
  }
//  {
//    int v=0;
//    expected_sc<int&> x = stde::make<expected_sc<int&>>(v);
//    BOOST_TEST(&v == &x.value());
//  }
  {
    int v=0;
    auto x = stde::make<expected_sc<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    auto x = stde::make<stde::expected<stde::_t, std::string>>(v);
    BOOST_TEST(*x == 0);
  }
//  {
//    constexpr int v=0;
//    constexpr stde::expected<int, int> x = stde::make<stde::expected<stde::_t, int>>(v);
//    BOOST_TEST(*x == 0);
//  }
  return ::boost::report_errors();
}
