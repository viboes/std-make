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
#include <experimental/meta.hpp>
#include <memory>
#include <boost/detail/lightweight_test.hpp>

namespace std {

#if __cplusplus <= 201103L
  template <int = 0, int..., class T>
    unique_ptr<decay_t<T>> make_unique(T&& x)
  {
    return unique_ptr<decay_t<T>>(new decay_t<T>(x));
  }

  // explicit overloads
  template <class T>
    unique_ptr<T> make_unique(remove_reference_t<T> const& x)
  {
    return unique_ptr<T>(new T(x));
  }

  template <class T>
    unique_ptr<T> make_unique(remove_reference_t<T>&& x)
  {
    return unique_ptr<T>(new T(std::forward<remove_reference_t<T>>(x)));
  }

  // variadic overload
  template <class T, class ...Args>
    unique_ptr<T> make_unique(Args&&... args)
  {
    return unique_ptr<T>(new T(forward<Args>(args)...));
  }
#endif

  template <class D>
  nullptr_t none_custom(experimental::meta::id<unique_ptr<experimental::_t, D>>) { return nullptr; }

  // customization point for template (needed because std::unique_ptr doesn't has a conversion constructor)
  template <class T, class ...Xs>
  unique_ptr<T> make_custom(experimental::meta::type<unique_ptr<T>>, Xs&& ...xs)
  {
    return make_unique<T>(forward<Xs>(xs)...);
  }

  // Holder customization
  template <class D>
  struct unique_ptr<experimental::_t, D>
  {
      template <class ...T>
      using apply = unique_ptr<T..., experimental::meta::eval<experimental::meta::rebind<D, T...>>>;
  };

  template <>
  struct default_delete<experimental::_t> : experimental::meta::lift<default_delete> {};

  namespace experimental
  {
    namespace meta
    {
      // type_constructor customization
      template <class T, class D>
      struct type_constructor<unique_ptr<T,D>> : id<unique_ptr<_t, D>> {};
      template <class T>
      struct type_constructor<default_delete<T>> : id<default_delete<_t>> {};
    }
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

  static_assert(std::is_same<stde::meta::rebind_t<std::default_delete<int>, long>, std::default_delete<long>>::value, "ERROR");
  {
    std::unique_ptr<int> x = stde::none<std::unique_ptr>();
    BOOST_TEST( ! x);
  }
  {
    std::unique_ptr<int> x = stde::none<std::unique_ptr<stde::_t> >();
    BOOST_TEST( ! x);
  }
  {
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::unique_ptr<int> x = std::make_unique<int>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    std::unique_ptr<A> x = std::make_unique<A>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    int v=1;
    std::unique_ptr<A> x = stde::make<std::unique_ptr<A>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    std::unique_ptr<A> x = stde::make<std::unique_ptr<A>>();
    BOOST_TEST_EQ(x->v,  3);
  }
  {
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::unique_ptr<int,std::default_delete<int> > x = stde::make<std::unique_ptr<stde::_t, std::default_delete<stde::_t> >>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
