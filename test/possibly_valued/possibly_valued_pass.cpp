// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/possibly_valued.hpp>

#include <experimental/possibly_valued.hpp>
#include <experimental/meta.hpp>

//#include <memory>
//#include <algorithm>
//#include <iterator>
//#include <vector>
#define JASEL_HAS_EXPERIMENTAL_ANY 0
#if JASEL_HAS_EXPERIMENTAL_ANY
#include <experimental/any>
#endif
#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

template <class T>
struct check;

template <class T>
struct A
{
  T v;
  A(): v(2) {}
  A(T v): v(std::move(v)) {}
};
template <class T>
struct A<T&>
{
  T* ptr;
  A(T& v): ptr(&v) {}
};
template <>
struct A<void>
{
  int v;
  A() : v(0) {}
};

// Holder specialization
template <>
struct A<std::experimental::_t> : std::experimental::meta::quote<A> {};

namespace std
{
  namespace experimental
  {
    // type_constructor customization
    template <class T>
    struct type_constructor<A<T>> : meta::id<A<_t>>
    {
    };

  }
}

int main()
{
  namespace stde = std::experimental;

  static_assert(std::is_same<
      stde::possibly_valued::value_type_t<int*>,
      int
    >::value, "ERROR");

  static_assert(stde::is_possibly_valued<int*>::value, "ERROR");

  {
    int * x = nullptr;
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
  {
    const int * x = nullptr;
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
  {
    int * x = new int(1);
    BOOST_TEST(stde::possibly_valued::has_value(x));
    BOOST_TEST(stde::possibly_valued::deref(x) == 1);
    delete x;
  }
  {
    const int * x = new int(1);
    BOOST_TEST(stde::possibly_valued::has_value(x));
    BOOST_TEST(stde::possibly_valued::deref(x) == 1);
    delete x;
  }
  // possibly_valued::value_or
  {
    int * x = nullptr;
    auto y = stde::possibly_valued::value_or(x, 1);
    BOOST_TEST_EQ(y, 1);
  }
  {
    int * x = new int(1);
    int y = stde::possibly_valued::value_or(x, 2);
    delete x;
    BOOST_TEST(y == 1);
  }
  //possibly_valued::apply_or
  {
    int * x = nullptr;
    int y = stde::possibly_valued::apply_or(x, twice, 1);
    BOOST_TEST(y == 1);
  }
  {
    int * x = new int(1);
    int y = stde::possibly_valued::apply_or(x, twice, -1);
    delete x;
    BOOST_TEST(y == 2);
  }
  return ::boost::report_errors();
}
