// Copyright (C) 2014-2015 Vicente J. Botet Escriba
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
#include <experimental/fundamental/v2/in_place.hpp>

#include <memory>
#include <algorithm>
#include <iterator>
#include <vector>
#include <boost/detail/lightweight_test.hpp>

template <class T>
struct A
{
  T v;
  A(): v(2) {}
  A(T v): v(std::move(v)) {}
  A(std::experimental::in_place_t): v(3) {}
  A(std::experimental::in_place_t, T v): v(std::move(v)) {}
  A(std::experimental::in_place_t, T v1, T v2): v(v1+v2) {}
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
struct A<std::experimental::_t> : std::experimental::meta::lift<A> {};

namespace std
{
  namespace experimental
  {
    namespace meta
    {
      // type_constructor customization
      template <class T>
      struct type_constructor<A<T>> : id<A<_t>>
      {
      };
    }
  }
}

template <class Tag>
void xx(Tag&& ) {
  namespace stde = std::experimental;
  static_assert(std::is_same<Tag, stde::in_place_t>::value, "b");
}
int main()
{
  namespace stde = std::experimental;
  xx(stde::in_place);
  static_assert(std::is_constructible<A<int>, stde::in_place_t,int, int>::value, "a");
  static_assert(std::is_constructible<A<int>, stde::in_place_t>::value, "b");

  {
    stde::in_place_t f = stde::in_place;
    (void)f;
  }
//  {
//    stde::in_place_t f = 0; // compile fails :)
//    (void)f;
//  }
  {
    A<void> x = stde::make<A>();
    BOOST_TEST(x.v == 0);
  }
  {
    int v=0;
    A<int> x = stde::make<A>(v);
    BOOST_TEST(x.v == 0);
  }
  {
    A<int> x = stde::make<A<int>>();
    BOOST_TEST(x.v == 2);
  }
  {
    int v=0;
    A<int> x = stde::make<A<int>>(v);
    BOOST_TEST(x.v == 0);
  }
  {
    int v=1;
    A<int> x = stde::make<A<int>>(stde::in_place, v,v);
    BOOST_TEST(x.v == 2);
  }
  {
    A<int> x = stde::make<A<int>>(stde::in_place);
    BOOST_TEST(x.v == 3);
  }
  {
    int v=0;
    A<int&> x = stde::make<A<int&>>(std::ref(v));
    BOOST_TEST(&v == x.ptr);
  }
  {
    int v=0;
    A<int> x = stde::make<stde::meta::lift<A>>(v);
    BOOST_TEST(x.v == 0);
  }
  {
    int v=0;
    A<int> x = stde::make<A>(v);
    BOOST_TEST(x.v == 0);
  }
  {
    int v=0;
    A<int&> x = stde::make<A>(std::ref(v));
    BOOST_TEST(&v == x.ptr);
  }
  {
    int v=0;
    A<int> x = stde::make<A<stde::_t>>(v);
    BOOST_TEST(x.v == 0);
  }
  {
    int v=0;
    A<int&> x = stde::make<A<stde::_t>>(std::ref(v));
    BOOST_TEST(&v == x.ptr);
  }
  {
    std::vector<int> xs;
    std::vector<A<int> > ys;
    std::transform(xs.begin(), xs.end(), std::back_inserter(ys), stde::maker_tc<A>{});
  }
  {
    std::vector<int> xs;
    std::vector<A<int> > ys;
    std::transform(xs.begin(), xs.end(), std::back_inserter(ys), stde::maker<A>{});
  }
  {
    std::vector<int> xs;
    std::vector<A<int> > ys;
    std::transform(xs.begin(), xs.end(), std::back_inserter(ys), stde::maker_mfc<A<stde::_t>>{});
  }
  {
    std::vector<int> xs;
    std::vector<A<long> > ys;
    std::transform(xs.begin(), xs.end(), std::back_inserter(ys), stde::maker_t<A<long>>{});
  }
  return ::boost::report_errors();
}
