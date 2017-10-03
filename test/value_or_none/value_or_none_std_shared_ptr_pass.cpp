// Copyright (C) 2014,2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/value_or_none.hpp>

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
  static_assert(stde::is_value_or_none<std::shared_ptr<int>>::value, "ERROR");
  static_assert(stde::meta::is_callable<std::shared_ptr<stde::_t>(int)>::value, "ERROR");
  static_assert(std::is_same<
      stde::value_or_none::none_type_t<std::shared_ptr<int>>,
      std::nullptr_t
    >::value, "ERROR");
  static_assert(std::is_same<
      stde::value_or_none::value_type_t<std::shared_ptr<int>>,
      int
    >::value, "ERROR");

  {
    std::shared_ptr<int> x = stde::none<std::shared_ptr>();
    BOOST_TEST( ! x);
  }
  {
    std::shared_ptr<int> x = stde::none<std::shared_ptr<stde::_t> >();
    BOOST_TEST( ! x);
  }
  {
    int v=0;
    std::shared_ptr<int> x = std::make_shared<int>(v);

    BOOST_TEST(x);
    BOOST_TEST(stde::has_value(x));
    BOOST_TEST(x != stde::none());
    BOOST_TEST(stde::none() != x);
    BOOST_TEST(x != stde::none<std::shared_ptr<stde::_t>>());

    BOOST_TEST(*x == 0);
    BOOST_TEST(stde::deref(x) == 0);
  }
  {
    int v=0;
    BOOST_TEST(stde::deref(std::make_shared<int>(v)) == 0);
  }
  {
    int v=1;
    std::shared_ptr<A> x = std::make_shared<A>(v,v);
    BOOST_TEST(x->v == 2);
    BOOST_TEST(stde::deref(x).v == 2);
  }
  {
    int v=1;
    std::shared_ptr<A> x = std::make_shared<A>(v,v);
    BOOST_TEST(x->v == 2);
    BOOST_TEST(stde::deref(x).v == 2);
  }
  {
    std::shared_ptr<A> x = std::make_shared<A>();
    BOOST_TEST_EQ(x->v,  3);
    BOOST_TEST(stde::deref(x).v == 3);
  }
  return ::boost::report_errors();
}
