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
#include <future>
#include <boost/detail/lightweight_test.hpp>
#include <iostream>

namespace std {

  future<void> make_ready_future()
  {
    promise<void> p;
    p.set_value();
    return p.get_future();
  }

  template <int = 0, int..., class T>
    future<experimental::meta::deduced_type_t<T>> make_ready_future(T&& x)
  {
    promise<experimental::meta::deduced_type_t<T>> p;
    p.set_value(forward<T>(x));
    return p.get_future();
  }

  // explicit overloads
  template <class T>
    future<T> make_ready_future(remove_reference_t<T> const& x)
  {
    promise<T> p;
    p.set_value(x);
    return p.get_future();
  }

  template <class T>
    future<T> make_ready_future(remove_reference_t<T>&& x)
  {
    promise<T> p;
    p.set_value(forward<remove_reference_t<T>>(x));
    return p.get_future();
  }

  // variadic overload
  template <class T, class ...Args>
    future<T> make_ready_future(Args&&... args)
  {
    promise<T> p;
    p.set_value(T(forward<Args>(args)...));
    return p.get_future();

  }

  // customization point for template (needed because std::future doesn't has a default constructor)
  future<void> make_custom(experimental::meta::type<future<void>>)
  {
    return make_ready_future();
  }

  // customization point for template (needed because std::future doesn't has a conversion constructor)
  template <class DX, class ...Xs>
  future<DX> make_custom(experimental::meta::type<future<DX>>, Xs&& ...xs)
  {
    return make_ready_future<DX>(forward<Xs>(xs)...);
  }

  // Holder customization
  template <>
  struct future<experimental::_t> : experimental::meta::lift<future> {};

//  template <>
//  struct future<experimental::_t&>
//  {
//    template<class ...T>
//    using apply = future<T& ...>;
//  };

#ifdef JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR
  namespace experimental
  {
    namespace meta
    {
      // type_constructor customization
      template <class T>
      struct type_constructor<future<T>> : id<future<_t>> {};
    }
  }
#endif

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
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<void> x = std::make_ready_future();
    std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
    x.get();
    std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<void> x = stde::make<std::future>();
    x.get();
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = std::make_ready_future(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = stde::make<std::future>(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = std::make_ready_future<int>(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = stde::make<std::future<int>>(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=1;
    std::future<A> x = std::make_ready_future<A>(v,v);
    BOOST_TEST(x.get().v == 2);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=1;
    std::future<A> x = stde::make<std::future<A>>(v,v);
    BOOST_TEST(x.get().v == 2);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int> x = std::make_ready_future<int>();
    BOOST_TEST_EQ(x.get(),  0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    std::future<int> x = stde::make<std::future<int>>();
    BOOST_TEST_EQ(x.get(),  0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = std::make_ready_future<int&>(v);
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = std::make_ready_future(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make<std::future<int&>>(v);
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make<std::future>(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int> x = stde::make<std::future<stde::_t>>(v);
    BOOST_TEST(x.get() == 0);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    std::future<int&> x = stde::make<std::future<stde::_t>>(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  {
    int v=0;
    auto x = stde::make<std::future<stde::_t>>(std::ref(v));
    BOOST_TEST(&x.get() == &v);
  }
  std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
  return ::boost::report_errors();
}
