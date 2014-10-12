// Copyright (C) 2014 Vicente J. Botet Escriba
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

#define VIBOES_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
#include <experimental/make.hpp>
#include <optional.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace std {
  namespace experimental {

    // Holder specialization
    template <>
    struct optional<_t>  : lift<optional> {};

    template <>
    struct optional<void> {};

    // type_constructor customization
    template <class T>
    struct type_constructor<optional<T>> : identity<optional<_t>> {};
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
    stde::optional<int> x = stde::make<stde::optional>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    short v=0;
    stde::optional<int> x = stde::make<stde::optional<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    stde::optional<A> x = stde::make<stde::optional<A>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    stde::optional<int> x = stde::make<stde::optional<int>>();
    BOOST_TEST_EQ(*x,  0);
  }
  {
    int v=0;
    stde::optional<int&> x = stde::make<stde::optional<int&>>(v);
    BOOST_TEST(&v == &x.value());
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
