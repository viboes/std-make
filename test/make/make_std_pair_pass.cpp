// Copyright (C) 2015 Vicente J. Botet Escriba
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
#include <utility>
#include <boost/detail/lightweight_test.hpp>

namespace std {

  // Holder customization
  template <>
  struct pair<experimental::_t, experimental::_t>
  {
#ifdef __clang__
    template <class T, class U>
    using apply = pair<T, U>;
#else
  private:
    template <class Types>
    struct impl;
    template <class T, class U>
    struct impl<experimental::meta::types<T, U>> : experimental::meta::id<pair<T, U>>{};
    // this specialization is needed to avoid instantiation of pair<T>
    template <class T>
    struct impl<experimental::meta::types<T>> : experimental::meta::id<T>{};
  public:
    template <class ...Ts>
    using apply = experimental::meta::eval<impl<experimental::meta::types<Ts...>>>;
#endif
  };

  namespace experimental
  {
    namespace meta
    {
      // type_constructor customization
      template <class T1, class T2>
      struct type_constructor<pair<T1,T2>> : id<pair<_t, _t>> {};

      template <>
      struct type_constructor<template_class<pair>> : id<pair<_t, _t>> {};
    }
  }
}

int main()
{
  namespace stde = std::experimental;
  {
    int v=0;
    std::pair<int, int> x = stde::make<std::pair>(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  {
    int v=0;
    std::pair<long, long> x = stde::make<std::pair<long, long> >(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  {
    int v=0;
    std::pair<int, int> x = stde::make<std::pair<stde::_t, stde::_t> >(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  {
    constexpr int v=0;
    constexpr std::pair<int, int> x = stde::make<std::pair>(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  return ::boost::report_errors();
}
