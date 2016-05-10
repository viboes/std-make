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
struct A<std::experimental::_t> : std::experimental::meta::quote<A> {};

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

void accept_in_place_t(std::experimental::in_place_t) {
}
void accept_nullptr_t(std::nullptr_t) {
}

struct nullopt_t2
{
  struct init{};
  constexpr nullopt_t2(init){};
  //constexpr nullopt_t2(){};
};
constexpr nullopt_t2 nullopt2{nullopt_t2::init{}};

void accept_nullopt_t2(nullopt_t2) {
}


int main()
{
  namespace stde = std::experimental;
  xx(stde::in_place);
  static_assert(std::is_constructible<A<int>, stde::in_place_t,int, int>::value, "a");
  static_assert(std::is_constructible<A<int>, stde::in_place_t>::value, "b");

  {
    nullopt_t2 ip = nullopt2;
    accept_nullopt_t2(ip);
    accept_nullopt_t2(nullopt2);
    ip = nullopt2;
  }
  {
    //accept_nullopt_t2(nullopt_t2{}); // compile fails
    //accept_nullopt_t2(nullopt_t2()); // compile fails
    //accept_nullopt_t2({}); // compile fails
    //nullopt_t2 ip = {};  // compile fails
    //accept_nullopt_t2(ip);
  }
  {
    stde::in_place_t ip = stde::in_place;
    accept_in_place_t(ip);
    accept_in_place_t(stde::in_place);
    //ip = stde::in_place;
  }
//  {
//    stde::in_place_t ip = 0; // compile fails :)
//    (void)ip;
//  }
//  {
//    stde::in_place_t ip1 = stde::in_place_t();  // compile fails
//    accept_in_place_t(ip1); // compile fails :)
//    accept_in_place_t(stde::in_place_t()); // compile fails
//    stde::in_place_t ip2 = {}; // compile fails :)
//    accept_in_place_t({}); // compile fails :)
//    auto x = stde::in_place_t{}; // compile fails :)
//  }
  {
    std::nullptr_t ip = nullptr;
    accept_nullptr_t(ip);
    accept_nullptr_t(nullptr);
    ip = nullptr;
  }
  {
    auto np = std::nullptr_t{}; // shouldn't compile fails :(
    accept_nullptr_t(np);
    std::nullptr_t np1 = {}; // // shouldn't compile fails :(
    accept_nullptr_t(np1);
    accept_nullptr_t(std::nullptr_t()); // shouldn't compile fails :)
    accept_nullptr_t({}); // shouldn't compile fails :)
  }
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
    A<int> x = stde::make<stde::meta::quote<A>>(v);
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
    std::transform(xs.begin(), xs.end(), std::back_inserter(ys), stde::maker<A>());
  }
  {
    std::vector<int> xs;
    std::vector<A<int> > ys;
    std::transform(xs.begin(), xs.end(), std::back_inserter(ys), stde::maker<A<stde::_t>>());
  }
  {
    std::vector<int> xs;
    std::vector<A<long> > ys;
    std::transform(xs.begin(), xs.end(), std::back_inserter(ys), stde::maker<A<long>>());
  }
  return ::boost::report_errors();
}
