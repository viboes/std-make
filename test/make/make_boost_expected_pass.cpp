// Copyright (C) 2014 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/make.hpp>

//  template <template <class ...> M, class X>
//  auto make(X&& x);
//  template <class M, class ...Args>
//  auto make(Args&& ...args);


#include <experimental/make.hpp>
#include <boost/expected/expected.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace boost {

// customization point for template (needed because boost::expected doesn't has experimental::in_place_t constructor)
template <class X, class E, class ...Args>
expected<X, E> make(std::experimental::type<expected<X, E>>, std::experimental::in_place_t, Args&& ...args)
{
  expected<X, E> res;
  res.emplace(std::forward<Args>(args)...);
  return std::move(res);
}

// Holder specialization
template <class E>
struct expected<std::experimental::_t, E> {};

// customization point for holder
template <class X, class E>
expected<typename std::decay<X>::type, E> make(std::experimental::type<expected<std::experimental::_t, E>>, X&& x)
{
  return std::experimental::make<expected<typename std::decay<X>::type, E>>(std::forward<X>(x));
}

}
struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int main()
{
  namespace stde = std::experimental;
  {
    int v=0;
    boost::expected<int> x = stde::make<boost::expected>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    boost::expected<int> x = stde::make<boost::expected<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    auto x = stde::make<boost::expected<int, std::string>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    boost::expected<A> x = stde::make<boost::expected<A>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    int v=1;
    auto x = stde::make<boost::expected<A, std::string>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    boost::expected<int> x = stde::make<boost::expected<int>>();
    BOOST_TEST_EQ(*x,  0);
  }
  {
    auto x = stde::make<boost::expected<int, std::string>>();
    BOOST_TEST_EQ(*x,  0);
  }
//  {
//    int v=0;
//    boost::expected<int&> x = stde::make<boost::expected<int&>>(v);
//    BOOST_TEST(&v == &x.value());
//  }
  {
    int v=0;
    auto x = stde::make<boost::expected<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    auto x = stde::make<boost::expected<stde::_t, std::string>>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
