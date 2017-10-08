// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/value_or_error.hpp>
// <experimental/expected.hpp>

#include <experimental/expected.hpp>
#include <experimental/value_or_error.hpp>

#include <boost/detail/lightweight_test.hpp>

struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int twice(int i) {
  return 2*i;
}

std::experimental::expected<int, bool> mtwice(int i) {
  return std::experimental::expected<int, bool>(2*i);
}

std::experimental::expected<double, bool> inverse(double x) {
  if (static_cast<int>(x)==0) return std::experimental::unexpected<bool>(true);
  return 1/x;
}

template <class T>
struct check;


int res(std::experimental::unexpected<bool>) {
  return -1;
}


int main()
{
  namespace stde = std::experimental;

  static_assert(stde::is_value_or_error<stde::expected<int, bool>>::value, "ERROR");

  static_assert(std::is_same<
      stde::value_or_error::failure_type_t<stde::expected<int, bool>>,
      stde::unexpected<bool>
    >::value, "ERROR");
  static_assert(std::is_same<
      stde::value_or_error::success_type_t<stde::expected<int, bool>>,
      int
    >::value, "ERROR");

  static_assert(stde::is_value_or_error<stde::expected<int, bool>>::value, "ERROR");

  {
      auto u = stde::unexpected<bool>(true);
      auto b = stde::wrapped::unwrap(u);
      static_assert(std::is_same<
          decltype(b),
          bool
        >::value, "ERROR");
  }

  static_assert(std::is_same<
      stde::value_or_error::value_type_t<stde::expected<int, bool>>,
      int
    >::value, "ERROR");
  static_assert(std::is_same<
      stde::value_or_error::error_type_t<stde::expected<int, bool>>,
      bool
    >::value, "ERROR");

  {
    auto n = stde::unexpected<bool>(true);
    stde::expected<int, A> x{n};
    BOOST_TEST(! stde::value_or_error::succeeded(x));
  }
  {
    auto n = stde::unexpected<bool>(true);
    stde::expected<int, bool> x(n);
    BOOST_TEST(! x);
    BOOST_TEST(! stde::value_or_error::has_value(x));
    BOOST_TEST(x == stde::unexpected<bool>(true));
    BOOST_TEST(stde::unexpected<bool>(true) == x);

  }
  {
    int v=1;
    stde::expected<int, bool> x(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(x != stde::unexpected<bool>(true));
    BOOST_TEST(stde::value_or_error::deref(x) == 1);
    x = 2;
    BOOST_TEST(stde::value_or_error::deref(x) == 2);
  }
  {
    int v=1;
    const stde::expected<int, bool> x (v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(x != stde::unexpected<bool>(true));
    BOOST_TEST(stde::value_or_error::deref(x) == 1);
  }
  {
    int v=0;
    stde::expected<int, bool> x (v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(stde::value_or_error::deref(x) == 0);
    // fixme static_assert(std::is_same<int&, decltype(stde::value_or_error::deref(x))>::value, "a");
  }
  {

    int v=0;
    BOOST_TEST(stde::value_or_error::deref(stde::expected<int, bool>(v)) == 0);
  }
  static_assert(std::is_constructible<A,int, int>::value, "a");
  static_assert(std::is_constructible<A>::value, "b");

  //static_assert(std::is_constructible<stde::expected<A, bool>, stde::in_place_t,int, int>::value, "a");
  //static_assert(std::is_constructible<stde::expected<A, bool>, stde::in_place_t>::value, "b");

  // value_or_error::transform
  {
    stde::expected<int, bool> x = stde::unexpected<bool>(true);
    BOOST_TEST(! stde::value_or_error::has_value(x));
    stde::expected<int, bool> y = stde::value_or_error::transform(x, twice);
    BOOST_TEST(! stde::value_or_error::has_value(y));
  }

  {
    int v=1;
    stde::expected<int, bool> x (v);
    stde::expected<int, bool> y = stde::value_or_error::transform(x, twice);
    BOOST_TEST(stde::value_or_error::deref(y) == 2);
  }
  {
    int v=1;
    stde::expected<int, bool> y = stde::value_or_error::transform(stde::expected<int, bool>(v), twice);
    BOOST_TEST(stde::value_or_error::has_value(y));
    BOOST_TEST(stde::value_or_error::deref(y) == 2);
  }
  // value_or_error::ap
  {
    stde::expected<int, bool> x = stde::unexpected<bool>(true);
    stde::expected<int(*)(int), bool> f = stde::unexpected<bool>(true);
    stde::expected<int, bool> y = stde::value_or_error::ap(f, x);
    BOOST_TEST(! stde::value_or_error::has_value(y));
  }
  {
    stde::expected<int, bool> x (1);
    stde::expected<int(*)(int), bool> f = stde::unexpected<bool>(true);
    stde::expected<int, bool> y = stde::value_or_error::ap(f, x);
    BOOST_TEST(! stde::value_or_error::has_value(y));
  }
  {
    const stde::expected<int, bool> x (1);
    const stde::expected<int(*)(int), bool> f = stde::unexpected<bool>(true);
    stde::expected<int, bool> y = stde::value_or_error::ap(f, x);
    BOOST_TEST(! stde::value_or_error::has_value(y));
  }
  {
    stde::expected<int(*)(int), bool> f = stde::unexpected<bool>(true);
    stde::expected<int, bool> y = stde::value_or_error::ap(f, stde::expected<int, bool>(1));
    BOOST_TEST(! stde::value_or_error::has_value(y));
  }
  {
    stde::expected<int, bool> x = stde::unexpected<bool>(true);
    stde::expected<int(*)(int), bool> f = stde::expected<int(*)(int), bool>(twice);
    stde::expected<int, bool> y = stde::value_or_error::ap(f, x);
    BOOST_TEST(! stde::value_or_error::has_value(y));
  }
  {
    int v=1;
    stde::expected<int, bool> x = stde::expected<int, bool>(v);
    stde::expected<int(*)(int), bool> f = stde::expected<int(*)(int), bool>(twice);
    stde::expected<int, bool> y = stde::value_or_error::ap(f, x);
    BOOST_TEST(stde::value_or_error::deref(y) == 2);
  }
  {
    int v=1;
    stde::expected<int(*)(int), bool> f = stde::expected<int(*)(int), bool>(twice);
    stde::expected<int, bool> y = stde::value_or_error::ap(f, stde::expected<int, bool>(v));
    BOOST_TEST(stde::value_or_error::deref(y) == 2);
  }
  // value_or_error::bind
   {
     stde::expected<int, bool> x = stde::unexpected<bool>(true);
     stde::expected<int, bool> y = stde::value_or_error::bind(x, mtwice);
     BOOST_TEST(! stde::value_or_error::has_value(y));
   }
   {
     int v=1;
     stde::expected<int, bool> x (v);
     stde::expected<int, bool> y = stde::value_or_error::bind(x, mtwice);
     BOOST_TEST(stde::value_or_error::deref(y) == 2);
   }
   {
     int v=1;
     stde::expected<int, bool> y = stde::value_or_error::bind(stde::expected<int, bool>(v), mtwice);
     BOOST_TEST(stde::value_or_error::deref(y) == 2);
   }
   {
     int v=1;
     const stde::expected<int, bool> x (v);
     stde::expected<int, bool> y = stde::value_or_error::bind(x, mtwice);
     BOOST_TEST(stde::value_or_error::deref(y) == 2);
   }

  //value_or_error::value_or
  {
    stde::expected<int, bool> x = stde::unexpected<bool>(true);
    int y = stde::value_or_error::value_or(x, 1);
    BOOST_TEST(y == 1);
  }
  {
    stde::expected<int, bool> x = stde::expected<int, bool>(1);
    int y = stde::value_or_error::value_or(x, 2);
    BOOST_TEST(y == 1);
  }
  //value_or_error::apply_or
  {
    stde::expected<int, bool> x = stde::unexpected<bool>(true);
    int y = stde::value_or_error::apply_or(x, twice, 1);
    BOOST_TEST(y == 1);
  }
  {
    stde::expected<int, bool> x = stde::expected<int, bool>(1);
    int y = stde::value_or_error::apply_or(x, twice, -1);
    BOOST_TEST(y == 2);
  }
  //value_or_error::has_error
  {
    stde::expected<int, bool> x = stde::unexpected<bool>(true);
    BOOST_TEST(stde::value_or_error::check_error(x, true));
  }
  {
    stde::expected<int, bool> x = stde::expected<int, bool>(1);
    BOOST_TEST(! stde::value_or_error::check_error(x, true));
  }

  //value_or_error::resolve
  {
    stde::expected<int, bool> x = stde::unexpected<bool>(true);
    int y = stde::value_or_error::resolve(x, res);
    BOOST_TEST(y == -1);
  }
  {
    stde::expected<int, bool> x = stde::expected<int, bool>(1);
    int y = stde::value_or_error::resolve(x, res);
    BOOST_TEST(y == 1);
  }
  return ::boost::report_errors();
}
