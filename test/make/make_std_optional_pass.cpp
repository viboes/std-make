// Copyright (C) 2014-2016 Vicente J. Botet Escriba
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

#if defined JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
#warning JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
int main()
{
}
#else

//#define JASEL_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
#define JASEL_FUNDAMENTAL_EXTENDED

#include <experimental/optional.hpp>

#include <boost/detail/lightweight_test.hpp>

std::experimental::nullopt_t null() { return std::experimental::nullopt;}

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

std::experimental::optional<double> inverse(double x) {
  if (x==0.0) return std::experimental::nullopt;
  return 1/x;
}

int main()
{
  namespace stde = std::experimental;
  static_assert(not stde::is_nullable<stde::none_t>::value, "ERROR");
  static_assert(stde::is_nullable<stde::optional<int>>::value, "ERROR");
  static_assert(stde::meta::is_callable<stde::optional<stde::_t>(int)>::value, "ERROR");
#if defined JASEL_FUNDAMENTAL_EXTENDED
  static_assert(std::is_same<stde::value_type_t<stde::optional<int&>>, int&>::value, "ERROR");
#endif
#if defined __clang__
  {
    stde::optional<stde::optional<int>> x { { null() } };
    BOOST_TEST( ! x);
  }
  {
    stde::optional<stde::optional<int>> x { { stde::nullopt } };
    BOOST_TEST( ! x);
  }
#endif

  {
    int v=1;
    stde::optional<int> x = stde::make<stde::optional>(v);
    BOOST_TEST(*x == 1);
#if defined JASEL_FUNDAMENTAL_EXTENDED
    stde::optional<int> y = stde::fmap(twice, x);
    BOOST_TEST(y);
#endif
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    short v=0;
    stde::optional<int> x = stde::make<stde::optional<int>>(v);
    BOOST_TEST(*x == 0);
  }
  static_assert(std::is_constructible<A,int, int>::value, "a");
  static_assert(std::is_constructible<A>::value, "b");

  static_assert(std::is_constructible<stde::optional<A>, stde::in_place_t,int, int>::value, "a");
  static_assert(std::is_constructible<stde::optional<A>, stde::in_place_t>::value, "b");

  {
    int v=1;
    stde::optional<A> x = stde::make<stde::optional<A>>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    stde::optional<int> x = stde::make<stde::optional<int>>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
  }
  {
    stde::optional<int> o((stde::in_place));
    BOOST_TEST(o);
  }
  {
    stde::optional<int> o((stde::in_place));
    (void)o;
    stde::optional<int> x = stde::make<stde::optional<int>>();
    BOOST_TEST(! x);
  }
  {
    int v=0;
    stde::optional<int&> x = stde::make<stde::optional<int&>>(std::ref(v));
    BOOST_TEST(&v == &x.value());
    BOOST_TEST(&v == std::addressof(x.value()));
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  {
//    make/make_std_optional_pass.cpp:166:35: error: constexpr variable 'x' must be initialized by a constant expression
//        constexpr stde::optional<int> x = v;
//
//    constexpr int const v=0;
//    constexpr stde::optional<int> x = v;
//    BOOST_TEST(*x == 0);
  }

  return ::boost::report_errors();
}
#endif
