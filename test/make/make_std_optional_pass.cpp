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
struct optional<_t> {};

// customization point for holder
template <class X>
optional<typename std::decay<X>::type> make(type<optional<_t>>, X&& x)
{
  return make<optional>(std::forward<X>(x));
}

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
    std::experimental::optional<int> x = std::experimental::make<std::experimental::optional>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::experimental::optional<int> x = std::experimental::make<std::experimental::optional<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    std::experimental::optional<A> x = std::experimental::make<std::experimental::optional<A>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    std::experimental::optional<int> x = std::experimental::make<std::experimental::optional<int>>();
    BOOST_TEST_EQ(*x,  0);
  }
  {
    int v=0;
    std::experimental::optional<int&> x = std::experimental::make<std::experimental::optional<int&>>(v);
    BOOST_TEST(&v == &x.value());
  }
  {
    using namespace std::experimental;
    int v=0;
    std::experimental::optional<int> x = make<std::experimental::optional<_t>>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
