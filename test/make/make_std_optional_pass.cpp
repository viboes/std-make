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

#if defined JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
#warning JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
int main()
{
}
#else

#define JASEL_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <optional.hpp>
#include <experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp>
#include <experimental/fundamental/v2/functor/mcd/possible_valued_mcd.hpp>
#include <experimental/fundamental/v2/monad/mcd/possible_valued_mcd.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace std {
  namespace experimental {


    // Holder specialization
    template <>
    struct optional<_t>  : meta::lift<optional> {};

    template <class T>
    struct possible_value::tag<optional<T>> : meta::id<pointer_like> {};

    template <class T>
    struct functor::tag<optional<T>> : meta::id<possible_value> {};

    template <class T>
    struct monad::tag<optional<T>> : meta::id<possible_value> {};

    namespace meta
    {
      // type_constructor customization
      template <class T>
      struct type_constructor<optional<T>> : id<optional<_t>> {};

      template <class T>
      struct value_type<optional<T>> : id<T> { };
    }

    nullopt_t none_custom(meta::type<optional<_t>>) { return nullopt; }

  }
}

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
  if (x==0) return std::experimental::nullopt;
  return 1/x;
}

int main()
{
  namespace stde = std::experimental;
  static_assert(stde::meta::is_applicable_with<stde::optional<stde::_t>, int>::value, "ERROR");
  static_assert(std::is_same<stde::meta::value_type_t<stde::optional<int&>>, int&>::value, "ERROR");

  {
    stde::optional<int> x = stde::none<stde::optional>();
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
    stde::optional<int> y = stde::map(x, twice);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    stde::optional<int> x = stde::none<stde::optional<stde::_t>>();
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
  {
    int v=1;
    stde::optional<int> x = stde::make<stde::optional>(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(value(x) == 1);
    stde::optional<int> y = stde::map(x, twice);
    BOOST_TEST(stde::value(y) == 2);
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional<int>>(v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(value(x) == 0);
  }
  {
    short v=0;
    stde::optional<int> x = stde::make<stde::optional<int>>(v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(value(x) == 0);
  }
  {
    int v=1;
    stde::optional<A> x = stde::make<stde::optional<A>>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
    BOOST_TEST(value(x).v == 2);
  }
  {
    stde::optional<int> x = stde::make<stde::optional<int>>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
    BOOST_TEST(value(x) == 0);
  }
  {
    stde::optional<int> x = stde::make<stde::optional<int>>();
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
  {
    int v=0;
    stde::optional<int&> x = stde::make<stde::optional<int&>>(std::ref(v));
    BOOST_TEST(&v == &x.value());
    BOOST_TEST(&v == std::addressof(x.value()));
    BOOST_TEST(&v == std::addressof(value(x)));
  }
  {
    int v=0;
    stde::optional<int> x = stde::make<stde::optional<stde::_t>>(v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(value(x) == 0);
  }
//  {
//    constexpr int v=0;
//    constexpr stde::optional<int> x = stde::make<stde::optional>(v);
//    BOOST_TEST(*x == 0);
//  }

  return ::boost::report_errors();
}
#endif
