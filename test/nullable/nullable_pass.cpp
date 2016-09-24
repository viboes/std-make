// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/nullable.hpp>

#include <experimental/nullable.hpp>
#include <experimental/meta.hpp>

//#include <memory>
//#include <algorithm>
//#include <iterator>
//#include <vector>
#include <boost/detail/lightweight_test.hpp>

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
  static_assert(stde::is_nullable<int*>::value, "ERROR");
  static_assert(stde::nullable_traits<stde::type_constructor_t<int*>>::value, "ERROR");
  {
    int * x = nullptr;
    BOOST_TEST(! stde::has_value(x));
    BOOST_TEST(x == stde::none());
  }
  {
    int * x = stde::none();
    BOOST_TEST(! stde::has_value(x));
    BOOST_TEST(x == stde::none());
  }
  return ::boost::report_errors();
}
