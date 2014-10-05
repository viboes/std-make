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
template <class X, class ...Args>
expected<X> emplace(std::experimental::type<expected<X>>, Args&& ...args)
{
  expected<X> res;
  res.emplace(std::forward<Args>(args)...);
  return std::move(res);
}

// Holder specialization
template <>
struct expected<std::experimental::_t> {};

// customization point for holder
template <class X>
expected<typename std::decay<X>::type> make(std::experimental::type<expected<std::experimental::_t>>, X&& x)
{
  return std::experimental::make<expected>(std::forward<X>(x));
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
  {
    int v=0;
    boost::expected<int> x = std::experimental::make<boost::expected>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    boost::expected<int> x = std::experimental::make<boost::expected<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    boost::expected<A> x = std::experimental::emplace<boost::expected<A>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    boost::expected<int> x = std::experimental::emplace<boost::expected<int>>();
    BOOST_TEST_EQ(*x,  0);
  }
//  {
//    int v=0;
//    boost::expected<int&> x = std::experimental::make<boost::expected<int&>>(v);
//    BOOST_TEST(&v == &x.value());
//  }
  {
    using namespace std::experimental;
    int v=0;
    boost::expected<int> x = make<boost::expected<_t>>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
