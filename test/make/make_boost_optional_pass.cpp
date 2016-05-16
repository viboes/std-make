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

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/in_place.hpp>
#include <boost/optional.hpp>
#include <experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp>
#include <experimental/fundamental/v2/functor/mcd/possible_valued_mcd.hpp>
#include <experimental/fundamental/v2/applicative/mcd/possible_valued_mcd.hpp>
#include <experimental/fundamental/v2/monad/mcd/possible_valued_mcd.hpp>
#include <experimental/fundamental/v2/monoid/mcd/possible_valued_mcd.hpp>
#include <experimental/fundamental/v2/product.hpp>
#include <experimental/fundamental/v2/sum.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace boost {

  // Holder specialization
  template <>
  struct optional<std::experimental::_t>
  : std::experimental::meta::quote<optional> {};

}

namespace std
{
  namespace experimental
  {

    template <class T>
    struct possible_value::tag<boost::optional<T>> : meta::id<pointer_like> {};

    template <class T>
    // requires Monoid<T>
    struct monoid::tag<boost::optional<T>> : meta::id<possible_value> {};

    template <class T>
    struct functor::tag<boost::optional<T>> : meta::id<possible_value> {};

    template <class T>
    struct applicative::tag<boost::optional<T>> : meta::id<possible_value> {};

    template <class T>
    struct monad::tag<boost::optional<T>> : meta::id<possible_value> {};

    // type_constructor customization
    template <class T>
    struct type_constructor<boost::optional<T>> : meta::id<boost::optional<_t>> {};

    template <class T>
    struct none_type<boost::optional<T>> : meta::id<boost::none_t> { };

    template <class T>
    struct factory_traits<boost::optional<T>> : factory_traits_default<boost::optional<T>> {
      using factory_traits_default<boost::optional<T>>::make;

      template <class ...Xs>
      static constexpr
      boost::optional<T> make(std::experimental::in_place_t, Xs&& ...xs)
      {
        boost::optional<T> res;
        res.emplace(std::forward<Xs>(xs)...);
        return std::move(res);
      }
    };
    template <class T>
    struct nullable_traits<boost::optional<T>> {
      static //constexpr
      boost::none_t none() { return boost::none; }
    };
  }
}

namespace boost {

  //none_t none_custom(std::experimental::meta::id<optional<std::experimental::_t>>) { return boost::none; }

//  // customization point for template (needed because boost::optional doesn't has experimental::in_place_t constructor)
//  template <class X, class ...Args>
//  optional<X> make_custom(std::experimental::meta::id<optional<X>>, std::experimental::in_place_t, Args&& ...args)
//  {
//    optional<X> res;
//    res.emplace(std::forward<Args>(args)...);
//    return std::move(res);
//  }
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

boost::optional<double> inverse(double x) {
  if (x==0) return boost::none;
  return 1/x;
}

struct twice_fn {
  int operator()(int i) const { return twice(i); }
};

int main()
{

  namespace stde = std::experimental;
  static_assert(stde::meta::is_invokable_with<boost::optional<stde::_t>, int>::value, "ERROR");
  static_assert(stde::has_value_type_member<const boost::optional<int>>::value, "ERROR");
  static_assert(! stde::has_element_type_member<const boost::optional<int>>::value, "ERROR");
  static_assert(!std::is_array<boost::optional<int> >::value, "ERROR");
  static_assert(std::is_same<stde::value_type_t<boost::optional<int>>, int>::value, "ERROR");
  static_assert(std::is_same<stde::concept_tag_t<stde::applicative, boost::optional<int>>, stde::possible_value>::value, "ERROR");
  static_assert(std::is_same<stde::concept_tag_t<stde::monoid, stde::product<int>>, stde::meta::id<stde::product<int>>>::value, "ERROR");

  {
    stde::product<int> x(1);
    BOOST_TEST(x == 1);
  }
  {
    stde::product<int> x = stde::monoid_id<stde::product<int>>();
    BOOST_TEST_EQ(x, 1);
  }
  {
    stde::sum<int> x = stde::monoid_id<stde::sum<int>>();
    BOOST_TEST(x == 0);
  }
  {
    auto x = stde::monoid_id<boost::optional<stde::product<int>>>();
    BOOST_TEST(! x);
  }
  {
    auto x = stde::make<boost::optional>(stde::sum<int>(2));
    auto y = stde::make<boost::optional>(stde::sum<int>(3));
    auto z = stde::monoid_op(x, y);
    BOOST_TEST( *z == 5);
    boost::optional<stde::sum<int>> w = stde::monoid_op(x, y, z);
    BOOST_TEST( *w == 10);
  }
  {
    boost::optional<stde::product<int>> x = stde::none<boost::optional>();
    auto y = stde::make<boost::optional>(stde::product<int>(3));
    auto z = stde::monoid_op(x, y);
    BOOST_TEST( ! z);
  }
  {
    boost::optional<stde::product<int>> y = stde::none<boost::optional>();
    auto x = stde::make<boost::optional>(stde::product<int>(3));
    auto z = stde::monoid_op(x, y);
    BOOST_TEST( ! z);
  }
  {
    boost::optional<int> x = stde::none<boost::optional>();
    BOOST_TEST(! x);
    BOOST_TEST(! stde::has_value(x));
    boost::optional<int> y = stde::fmap(twice, x);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    boost::optional<int> x = stde::none<boost::optional<stde::_t>>();
    BOOST_TEST(! x);
    BOOST_TEST(! stde::has_value(x));
  }
  {
    int v=1;
    boost::optional<int> x = stde::make<boost::optional>(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(stde::value(x) == 1);
    boost::optional<int> y = stde::fmap(twice, x);
    BOOST_TEST(stde::value(y) == 2);
  }
  {
    boost::optional<int> x = stde::make<boost::optional>(1);
    //auto f = stde::make<boost::optional>(twice_fn{});
    auto f = stde::make<boost::optional>([](int a){return 2*a;});
    boost::optional<int> y = stde::ap(f, x);
    BOOST_TEST(stde::value(y) == 2);
  }
  {
    boost::optional<int> x = stde::make<boost::optional>(1);
    boost::optional<int> y = stde::make<boost::optional>(1);
    boost::optional<int> z = stde::fmap([](int a, int b){return a+b;}, x, y);
    BOOST_TEST_EQ(stde::value(z), 2);
    BOOST_TEST(stde::have_value(x, y, z));
  }
  {
    double v=2;
    boost::optional<double> x = stde::make<boost::optional>(v);
    BOOST_TEST(stde::value(x) == 2);
    boost::optional<double> y = stde::mbind(x, inverse);
    BOOST_TEST(stde::value(y) == 1.0/2);
  }
  {
    double v=0;
    boost::optional<double> x = stde::make<boost::optional>(v);
    BOOST_TEST(stde::value(x) == 0);
    boost::optional<double> y = stde::mbind(x, inverse);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    int v=0;
    boost::optional<int> x = stde::make<boost::optional<int>>(v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(stde::value(x) == 0);
  }
  {
    int v=1;
    boost::optional<A> x = stde::make<boost::optional<A>>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
    BOOST_TEST(stde::value(x).v == 2);
  }
  {
    boost::optional<int> x = stde::make<boost::optional<int>>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
    BOOST_TEST(stde::value(x) == 0);
  }
  {
    boost::optional<int> x = stde::make<boost::optional<int>>();
    BOOST_TEST(! x);
    BOOST_TEST(! stde::has_value(x));
  }
  {
    int v=0;
    boost::optional<int&> x = stde::make<boost::optional<int&>>(v);
    BOOST_TEST(&v == &(x.get()));
  }
  {
    int v=0;
    boost::optional<int> x = stde::make<boost::optional<stde::_t>>(v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(stde::value(x) == 0);
  }
  return ::boost::report_errors();
}
