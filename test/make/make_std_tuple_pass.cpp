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
#include <tuple>
#include <boost/detail/lightweight_test.hpp>

namespace std {

// There is no way to have a variadic type constructor so we opt for a type construcotr with a single type holder

  // Holder customization
  template <>
  struct tuple<experimental::_t>
  {
#ifdef __clang__
    template <class ...Ts>
    using apply = tuple<Ts...>;
#else
  private:
    template <class Types>
    struct impl;
    template <class ...Ts>
    struct impl<experimental::meta::types<Ts...>> : experimental::meta::id<tuple<Ts...>>{};
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
      template <class ...Ts>
      struct type_constructor<tuple<Ts...>> : id<tuple<_t>> {};
    }
  }
}

int main()
{
  namespace stde = std::experimental;
  {
    int v=0;
    std::tuple<int, int> x = stde::make<std::tuple>(v, v);
    BOOST_TEST(std::get<0>(x) == 0);
    BOOST_TEST(std::get<1>(x) == 0);
  }
  {
    int v=0;
    std::tuple<long, long> x = stde::make<std::tuple<long, long> >(v, v);
    BOOST_TEST(std::get<0>(x) == 0);
    BOOST_TEST(std::get<1>(x) == 0);
  }
  {
    int v=0;
    std::tuple<int, int> x = stde::make<std::tuple<stde::_t> >(v, v);
    BOOST_TEST(std::get<0>(x) == 0);
    BOOST_TEST(std::get<1>(x) == 0);
  }
  {
    constexpr int v=0;
    constexpr std::tuple<int, int> x = stde::make<std::tuple>(v, v);
    BOOST_TEST(std::get<0>(x) == 0);
    BOOST_TEST(std::get<1>(x) == 0);
  }
  return ::boost::report_errors();
}
