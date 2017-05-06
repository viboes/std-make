// Copyright (C) 2014-2016 Vicente J. Botet Escriba
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
#include <experimental/future.hpp>

#include <boost/detail/lightweight_test.hpp>

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
    std::shared_future<int> x = stde::make<std::shared_future>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=0;
    std::shared_future<int> x = stde::make<std::shared_future<int>>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=1;
    std::shared_future<A> x = stde::emplace<std::shared_future<A>>(v,v);
    BOOST_TEST(x.get().v == 2);
  }
  {
    std::shared_future<int> x = stde::emplace<std::shared_future<int>>();
    BOOST_TEST_EQ(x.get(),  0);
  }
#if 0
  {
    //this fail as v is deduce as int
    // todo: move to failing tests
    int v=0;
    std::shared_future<int&> x = stde::make<std::shared_future<int&>>(v);
    BOOST_TEST(&x.get() == &v);
  }
  {
    //this fail as v is deduce as int
    // todo: move to failing tests
    int v=0;
    std::shared_future<int&> x = stde::make<std::shared_future<stde::_t&>>(v);
    BOOST_TEST(&x.get() == &v);
  }
#endif
  {
    int v=0;
    std::shared_future<int> x = stde::make<std::shared_future<stde::_t>>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=0;
    auto x = stde::make<std::shared_future<stde::_t>>(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  {
    int v=0;
    auto x = stde::make<std::shared_future>(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  return ::boost::report_errors();
}
