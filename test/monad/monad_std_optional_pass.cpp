// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/monad.hpp>
// <experimental/optional.hpp>

#if defined JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
#warning JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
int main()
{
}
#else

#define JASEL_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL

#include <experimental/optional.hpp>

#include <boost/detail/lightweight_test.hpp>

std::experimental::optional<int> twice(int i) {
  if (i==0) return std::experimental::nullopt;
  return std::experimental::make_optional(2*i);
}

int main()
{
  namespace stde = std::experimental;

  static_assert(stde::is_nullable<stde::optional<int>>::value, "ERROR");
  static_assert(stde::is_type_constructible<stde::optional<int>>::value, "ERROR");
  static_assert(std::is_base_of<stde::monad::tag, stde::monad::traits<stde::optional<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_monad<stde::optional<stde::_t>>::value, "ERROR");

  {
    stde::optional<int> x = stde::none<stde::optional>();
    stde::optional<int> y = stde::monad::bind(x,twice);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    int v=1;
    stde::optional<int> x = stde::make<stde::optional>(v);
    stde::optional<int> y = stde::monad::bind(x,twice);
    BOOST_TEST(stde::has_value(y));
    BOOST_TEST(stde::deref(y) == 2);
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional>(v);
    stde::optional<int> y = stde::monad::bind(x,twice);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    int v=0;
    stde::optional<stde::optional<int>> x = stde::make<stde::optional>(v);
    stde::optional<int> y = stde::monad::unwrap(x);
    BOOST_TEST(stde::has_value(y));
    BOOST_TEST(stde::deref(y) == 0);
  }

  return ::boost::report_errors();
}
#endif
