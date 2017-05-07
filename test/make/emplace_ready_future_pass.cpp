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
#include <iostream>

struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int & refInt() {
    static int i=-1;
    ++i;
    return i;
};

int const& refConstInt() {
    static const int i=0;
    return i;
};

int main()
{
  namespace stde = std::experimental;

  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<void> x = stde::emplace_ready_future();
    x.wait();
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<void> x = stde::emplace_ready_future<void>();
    x.wait();
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int> x = stde::emplace_ready_future(42);
    BOOST_TEST(x.get() == 42);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int lv=1;
    std::future<int> x = stde::emplace_ready_future(lv);
    BOOST_TEST(x.get() == 1);
  }

  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int> x = stde::emplace_ready_future<int>();
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int> x = stde::emplace_ready_future<int>(42);
    BOOST_TEST(x.get() == 42);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int> x = stde::emplace_ready_future<int>(42ull);
    BOOST_TEST(x.get() == 42);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int lv=1;
    std::future<int> x = stde::emplace_ready_future<int>(lv);
    BOOST_TEST(x.get() == 1);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    short lv=1;
    std::future<int> x = stde::emplace_ready_future<int>(lv);
    BOOST_TEST(x.get() == 1);
  }


  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<A> x = stde::emplace_ready_future<A>();
    BOOST_TEST(x.get().v == 3);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=1;
    std::future<A> x = stde::emplace_ready_future<A>(v,v);
    BOOST_TEST(x.get().v == 2);
  }

  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::emplace_ready_future<int&>(v);
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int&> x = stde::emplace_ready_future<int&>(refInt());
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int const&> x = stde::emplace_ready_future<int const&>(refConstInt());
    BOOST_TEST(x.get() == 0);
  }


  return ::boost::report_errors();
}
