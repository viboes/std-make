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

#include <experimental/make.hpp>
#include <boost/optional.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace boost {

// customization point for template (needed because boost::optional doesn't has experimental::in_place_t constructor)
template <class X, class ...Args>
optional<X> make(std::experimental::type<optional<X>>, std::experimental::in_place_t, Args&& ...args)
{
  optional<X> res;
  res.emplace(std::forward<Args>(args)...);
  return std::move(res);
}

template <>
struct optional<void> {};
// Holder specialization
template <>
struct optional<std::experimental::_t> {};

// customization point for holder
template <class X>
optional<typename std::decay<X>::type> make(std::experimental::type<optional<std::experimental::_t>>, X&& x)
{
  return std::experimental::make<optional>(std::forward<X>(x));
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
    boost::optional<int> x = stde::make<boost::optional>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    boost::optional<int> x = stde::make<boost::optional<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    boost::optional<A> x = stde::make<boost::optional<A>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    boost::optional<int> x = stde::make<boost::optional<int>>();
    BOOST_TEST_EQ(*x,  0);
  }
//  {
//    int v=0;
//    boost::optional<int&> x = stde::make<boost::optional<int&>>(v);
//    BOOST_TEST(&v == x.get());
//  }
  {
    int v=0;
    boost::optional<int> x = stde::make<boost::optional<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
