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
#include <experimental/memory.hpp>

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
  static_assert(stde::is_nullable<std::unique_ptr<int>>::value, "ERROR");

  static_assert(std::is_same<stde::meta::rebind_t<std::default_delete<int>, long>, std::default_delete<long>>::value, "ERROR");
  {
    std::unique_ptr<int> x = stde::none<std::unique_ptr>();
    BOOST_TEST( ! x);
  }
  {
    std::unique_ptr<int> x = stde::none<std::unique_ptr<stde::_t> >();
    BOOST_TEST( ! x);
  }
  {
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::unique_ptr<int> x = std::make_unique<int>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    std::unique_ptr<A> x = std::make_unique<A>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    int v=1;
    std::unique_ptr<A> x = stde::make<std::unique_ptr<A>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    std::unique_ptr<A> x = stde::make<std::unique_ptr<A>>();
    BOOST_TEST_EQ(x->v,  3);
  }
  {
    //static_assert(! std::experimental::meta::is_callable<std::default_delete<stde::_t>(void)>::value, "");
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::unique_ptr<int,std::default_delete<int> > x = stde::make<std::unique_ptr<stde::_t, std::default_delete<stde::_t> >>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
