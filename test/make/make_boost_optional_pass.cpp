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

#include <experimental/make.hpp>
#include <boost/optional.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace boost {

  none_t none_custom(std::experimental::meta::type<optional<std::experimental::_t>>) { return boost::none; }
  none_t none_custom(std::experimental::meta::template_class<optional>) { return boost::none; }

  // customization point for template (needed because boost::optional doesn't has experimental::in_place_t constructor)
  template <class X, class ...Args>
  optional<X> make_custom(std::experimental::meta::type<optional<X>>, std::experimental::in_place_t, Args&& ...args)
  {
    optional<X> res;
    res.emplace(std::forward<Args>(args)...);
    return std::move(res);
  }

  // Holder specialization
  template <>
  struct optional<std::experimental::_t>
  //: std::experimental::meta::quote<optional> {};
  {
    template <class  T>
    using apply = optional<T>;
  };

}

#ifdef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR
namespace std
{
  namespace experimental
  {
    namespace meta
    {
      // type_constructor customization
      template <class T>
      struct type_constructor<boost::optional<T>> : id<boost::optional<_t>> {};
    }
  }
}
#endif

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
  static_assert(stde::meta::is_applicable_with<boost::optional<stde::_t>, int>::value, "ERROR");
  {
    int v=0;
    boost::optional<int> x = stde::make<boost::optional>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    boost::optional<int> x = stde::make<boost::optional<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    boost::optional<A> x = stde::make<boost::optional<A>>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    boost::optional<int> x = stde::make<boost::optional<int>>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
  }
  {
    boost::optional<int> x = stde::make<boost::optional<int>>();
    BOOST_TEST(! x);
  }
//  {
//    int v=0;
//    boost::optional<int&> x = stde::make<boost::optional<int&>>(v);
//    BOOST_TEST(&v == x.get());
//  }
  {
    int v=0;
    boost::optional<int> x = stde::make<boost::optional<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
