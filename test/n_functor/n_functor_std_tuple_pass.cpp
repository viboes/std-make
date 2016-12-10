// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/n_functor.hpp>
// <experimental/utility.hpp> tuple

#include <experimental/tuple.hpp>
#include <experimental/n_functor.hpp>
#include <experimental/product_type.hpp>

#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

int main()
{
#if __cplusplus >= 201402L
  namespace stde = std::experimental;

  using T = std::tuple<int, int>;

  static_assert(stde::is_product_type_v<T>, "ERROR");
  static_assert(stde::is_type_constructible_v<T>, "ERROR");
  static_assert(std::is_base_of<stde::n_functor::tag, stde::n_functor::traits<std::tuple<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_n_functor<std::tuple<stde::_t>>::value, "ERROR");

  {
    int v=1;
    T x = std::make_tuple(v,v);
    BOOST_TEST(std::get<0>(x) == 1);
    BOOST_TEST(std::get<1>(x) == 1);
    auto call = std::make_tuple(twice,twice);
    T y = stde::n_functor::transform(x, call);
    BOOST_TEST(std::get<0>(y) == 2);
    BOOST_TEST(std::get<1>(y) == 2);
  }
#endif

  return ::boost::report_errors();
}
