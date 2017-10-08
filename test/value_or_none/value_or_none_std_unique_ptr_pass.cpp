// Copyright (C) 2014-2017 Vicente J. Botet Escriba
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
  A(int v_): v(std::move(v_)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int main()
{
  namespace stde = std::experimental;
  static_assert(stde::is_value_or_none<std::unique_ptr<int>>::value, "ERROR");
  static_assert(std::is_same<
      stde::value_or_none::none_type_t<std::unique_ptr<int>>,
      std::nullptr_t
    >::value, "ERROR");
  static_assert(std::is_same<
      stde::value_or_none::value_type_t<std::unique_ptr<int>>,
      int
    >::value, "ERROR");

  static_assert(std::is_same<stde::meta::rebind_t<std::default_delete<int>, long>, std::default_delete<long>>::value, "ERROR");
  {
    std::unique_ptr<int> x = stde::none<std::unique_ptr>();
    BOOST_TEST( ! x);
    BOOST_TEST(! stde::value_or_none::has_value(x));
    BOOST_TEST(x == stde::none());
    BOOST_TEST(stde::none() == x);
    BOOST_TEST(x == stde::none<std::unique_ptr<stde::_t>>());
  }
  {
    int v=0;
    std::unique_ptr<int> x = std::make_unique<int>(v);
    BOOST_TEST(*x == 0);

    BOOST_TEST(stde::value_or_none::has_value(x));
    BOOST_TEST(x != stde::none());
    BOOST_TEST(stde::none() != x);
    BOOST_TEST(x != stde::none<std::unique_ptr<stde::_t>>());

    BOOST_TEST(stde::value_or_none::deref(x) == 0);
  }
  {
    int v=0;
    BOOST_TEST(stde::value_or_none::deref(std::make_unique<int>(v)) == 0);
  }
  {
    int v=1;
    std::unique_ptr<A> x = std::make_unique<A>(v,v);
    BOOST_TEST(x->v == 2);
    BOOST_TEST(stde::value_or_none::deref(x).v == 2);
  }
  {
    std::unique_ptr<A> x = std::make_unique<A>();
    BOOST_TEST_EQ(x->v,  3);
    BOOST_TEST(stde::value_or_none::deref(x).v == 3);
  }
  return ::boost::report_errors();
}
