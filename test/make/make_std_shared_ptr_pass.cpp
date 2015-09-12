// Copyright (C) 2014 Vicente J. Botet Escriba
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
#include <memory>
#include <boost/detail/lightweight_test.hpp>

namespace std {


  // Holder customization
  template <>
  struct shared_ptr<experimental::_t>
  //: public experimental::lift<shared_ptr> {};
  {
    template <class  T>
    using apply = shared_ptr<T>;
  };

  namespace experimental
  {
    namespace meta
    {
      // type_constructor customization
      template <class T>
      struct type_constructor<shared_ptr<T>> : id<shared_ptr<_t>> {};
    }
  }

  nullptr_t none_custom(experimental::meta::id<shared_ptr<experimental::_t>>) { return nullptr; }

  // customization point for template (needed because std::shared_ptr doesn't has a conversion constructor)
  template <class DX, class ...Xs>
  shared_ptr<DX> make_custom(experimental::meta::id<shared_ptr<DX>>, Xs&& ...xs)
  {
    return make_shared<DX>(forward<Xs>(xs)...);
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
  static_assert(stde::meta::is_applicable_with<std::shared_ptr<stde::_t>, int>::value, "ERROR");

  {
    std::shared_ptr<int> x = stde::none<std::shared_ptr>();
    BOOST_TEST( ! x);
  }
  {
    std::shared_ptr<int> x = stde::none<std::shared_ptr<stde::_t> >();
    BOOST_TEST( ! x);
  }
  {
    int v=0;
    std::shared_ptr<int> x = stde::make<std::shared_ptr>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::shared_ptr<int> x = std::make_shared<int>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::shared_ptr<int> x = stde::make<std::shared_ptr<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    std::shared_ptr<A> x = std::make_shared<A>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    int v=1;
    std::shared_ptr<A> x = stde::make<std::shared_ptr<A>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    std::shared_ptr<A> x = stde::make<std::shared_ptr<A>>();
    BOOST_TEST_EQ(x->v,  3);
  }
  {
    int v=0;
    std::shared_ptr<int> x = stde::make<std::shared_ptr<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
