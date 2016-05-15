// Copyright (C) 2014,2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/make.hpp>

//  template <template <class ...> M, class X>
//  auto make(X&& x);
//  template <class M, class ...Args>
//  auto make(Args&& ...args);

#if defined JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_BOOST_EXPECTED_NOT_INSTALLED
#warning JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_BOOST_EXPECTED_NOT_INSTALLED
int main()
{
}
#else

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <boost/expected/expected.hpp>
#include <experimental/fundamental/v2/in_place.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace boost {

//  // customization point for template (needed because boost::expected doesn't has experimental::in_place_t constructor)
//  template <class X, class E, class ...Args>
//  expected<X, E> make_custom(std::experimental::meta::id<expected<X, E>>, std::experimental::in_place_t, Args&& ...args)
//  {
//    return expected<X, E>(boost::in_place_t{}, std::forward<Args>(args)...);
//  }

  // Holder specialization
  template <class E>
  struct expected<std::experimental::_t, E>: std::experimental::meta::bind_back<expected, E> {};

}

namespace std
{
  namespace experimental
  {

    template <class T, class E>
    struct factory_traits<boost::expected<T,E>> : factory_traits_default<boost::expected<T,E>> {
      using factory_traits_default<boost::expected<T,E>>::make;

      template <class ...Xs>
      static constexpr
      boost::expected<T,E> make(std::experimental::in_place_t, Xs&& ...xs)
      {
        return boost::expected<T, E>(boost::in_place_t{}, std::forward<Xs>(xs)...);
      }
    };

    // type_constructor customization
    template <class T, class E>
    struct type_constructor<boost::expected<T, E>> : meta::id<boost::expected<_t, E>> {};
  //    // value_type customization
  //    template <class T, class E>
  //    struct value_type<boost::expected<T, E>> : id<T> {};
  }
}

struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int main()
{
  namespace stde = std::experimental;
  {
    int v=0;
    boost::expected<int> x = stde::make<boost::expected>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    boost::expected<int> x = stde::make<boost::expected<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    auto x = stde::make<boost::expected<int, std::string>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    boost::expected<A> x = stde::make<boost::expected<A>>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    int v=1;
    auto x = stde::make<boost::expected<A, std::string>>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    boost::expected<int> x = stde::make<boost::expected<int>>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
  }
  {
    auto x = stde::make<boost::expected<int, std::string>>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
  }
//  {
//    int v=0;
//    boost::expected<int&> x = stde::make<boost::expected<int&>>(v);
//    BOOST_TEST(&v == &x.value());
//  }
  {
    int v=0;
    auto x = stde::make<boost::expected<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    auto x = stde::make<boost::expected<stde::_t, std::string>>(v);
    BOOST_TEST(*x == 0);
  }
//  {
//    constexpr int v=0;
//    constexpr boost::expected<int, int> x = stde::make<boost::expected<stde::_t, int>>(v);
//    BOOST_TEST(*x == 0);
//  }
  return ::boost::report_errors();
}
#endif
