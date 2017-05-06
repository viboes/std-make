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

int main()
{
  namespace stde = std::experimental;
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<void> x = stde::make_ready_future();
    std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
    x.get();
    std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = stde::make_ready_future(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make_ready_future(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
#if 0
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    // This must fail as 1 is not an lvalue.
    // todo: move to failing tests
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    std::future<int&> x = stde::make_ready_future<int&>(1);
  }
#endif


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
    std::future<int> x = stde::emplace_ready_future<int>(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int> x = stde::emplace_ready_future<int>();
    BOOST_TEST_EQ(x.get(),  0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::emplace_ready_future<int&>(v);
    BOOST_TEST(&x.get() == &v);
  }




  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<void> x = stde::make<std::future>();
    x.wait();
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<void> x = stde::make<std::future<void>>();
    x.wait();
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<void> x = stde::make<std::future<stde::_t>>();
    x.wait();
  }

  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = stde::make<std::future>(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = stde::make<std::future<int>>(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = stde::make<std::future<stde::_t>>(v);
    BOOST_TEST(x.get() == 0);
  }

  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make<std::future>(std::ref(v));
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make<std::future>(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make<std::future<int&>>(std::ref(v));
    BOOST_TEST(x.get() == 0);
  }
#if 0
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    //this fail as v is deduce as int
    // todo: move to failing tests
    int v=0;
    std::future<int&> x = stde::make<std::future<int&>>(v);
    BOOST_TEST(x.get() == 0);
  }
#endif
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make<std::future<int&>>(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make<std::future<stde::_t>>(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make<std::future<stde::_t>>(std::ref(v));
    BOOST_TEST(x.get() == 0);
  }
#if 0
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    //this fail as v is deduce as int
    // todo: move to failing tests
    int v=0;
    std::future<int&> x = stde::make<std::future<stde::_t&>>(v);
    BOOST_TEST(&x.get() == &v);
  }
#endif


  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int> x = stde::emplace<std::future<int>>();
    BOOST_TEST_EQ(x.get(),  0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::emplace<std::future<int&>>(v);
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=1;
    std::future<A> x = stde::emplace<std::future<A>>(v,v);
    BOOST_TEST(x.get().v == 2);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::emplace<std::future<stde::_t&>>(v);
    BOOST_TEST(&x.get() == &v);
  }
#if 0
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    //this fail as 1 is not an lvalue
    // todo: move to failing tests
    std::future<int&> x = stde::make<std::future<stde::_t&>>(1);
  }
#endif
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    auto x = stde::make<std::future<stde::_t>>(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  return ::boost::report_errors();
}
