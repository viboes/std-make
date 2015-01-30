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
#include <future>
#include <boost/detail/lightweight_test.hpp>

namespace std {

  future<void> make_ready_future()
  {
    promise<void> p;
    p.set_value();
    return p.get_future();
  }

  template <int = 0, int..., class T>
    future<decay_t<T>> make_ready_future(T&& x)
  {
    promise<decay_t<T>> p;
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
  future<void> make(experimental::type<future<void>>)
  {
    return make_ready_future();
  }

  // customization point for template (needed because std::future doesn't has a conversion constructor)
  template <class DX, class X>
  future<DX> make(experimental::type<future<DX>>, X&& x)
  {
    return make_ready_future<DX>(forward<X>(x));
  }

  // customization point for template (needed because std::future doesn't uses experimental::in_place_t)
  template <class X, class ...Args>
  future<X> make(experimental::type<future<X>>, experimental::in_place_t, Args&& ...args)
  {
    return make_ready_future<X>(forward<Args>(args)...);
  }

  // Holder customization
  template <>
  struct future<experimental::_t> : experimental::lift<future> {};

  template <>
  struct future<experimental::_t&>  : experimental::type_constructor_tag
  {
    template<class T>
    using apply = future<T&>;
  };

#ifdef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR
  namespace experimental
  {
    // type_constructor customization
    template <class T>
    struct type_constructor<future<T>> : identity<future<_t>> {};
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
  {
    std::future<void> x = std::make_ready_future();
    x.get();
  }
  {
    std::future<void> x = stde::make<std::future>();
    x.get();
  }
  {
    int v=0;
    std::future<int> x = std::make_ready_future(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=0;
    std::future<int> x = stde::make<std::future>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=0;
    std::future<int> x = std::make_ready_future<int>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=0;
    std::future<int> x = stde::make<std::future<int>>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=1;
    std::future<A> x = std::make_ready_future<A>(v,v);
    BOOST_TEST(x.get().v == 2);
  }
  {
    int v=1;
    std::future<A> x = stde::make<std::future<A>>(v,v);
    BOOST_TEST(x.get().v == 2);
  }
  {
    std::future<int> x = std::make_ready_future<int>();
    BOOST_TEST_EQ(x.get(),  0);
  }
  {
    std::future<int> x = stde::make<std::future<int>>();
    BOOST_TEST_EQ(x.get(),  0);
  }
  {
    int v=0;
    std::future<int&> x = std::make_ready_future<int&>(v);
    BOOST_TEST(&x.get() == &v);
  }
  {
    int v=0;
    std::future<int&> x = stde::make<std::future<int&>>(v);
    BOOST_TEST(&x.get() == &v);
  }
  {
    int v=0;
    std::future<int> x = stde::make<std::future<stde::_t>>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=0;
    auto x = stde::make<std::future<stde::_t&>>(v);
    BOOST_TEST(&x.get() == &v);
  }
  return ::boost::report_errors();
}
