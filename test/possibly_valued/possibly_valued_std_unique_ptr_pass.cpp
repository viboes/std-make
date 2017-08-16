// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/possibly_valued.hpp>

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
  static_assert(stde::is_possibly_valued<std::unique_ptr<int>>::value, "ERROR");
  static_assert(std::is_same<
      stde::possibly_valued::none_type_t<std::unique_ptr<int>>,
      std::nullptr_t
    >::value, "ERROR");
  static_assert(std::is_same<
      stde::possibly_valued::value_type_t<std::unique_ptr<int>>,
      int
    >::value, "ERROR");

  static_assert(std::is_same<stde::meta::rebind_t<std::default_delete<int>, long>, std::default_delete<long>>::value, "ERROR");
  {
    std::unique_ptr<int> x = nullptr;
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
  {
    int v=0;
    std::unique_ptr<int> x = std::make_unique<int>(v);
    BOOST_TEST( stde::possibly_valued::has_value(x));
    BOOST_TEST( stde::possibly_valued::deref(x) == 0);
  }
  {
    int v=0;
    BOOST_TEST(stde::possibly_valued::deref(std::make_unique<int>(v)) == 0);
  }
  {
    int v=1;
    std::unique_ptr<A> x = std::make_unique<A>(v,v);
    BOOST_TEST(stde::possibly_valued::deref(x).v == 2);
  }
  {
    std::unique_ptr<A> x = std::make_unique<A>();
    BOOST_TEST(stde::possibly_valued::deref(x).v == 3);
  }
  return ::boost::report_errors();
}
