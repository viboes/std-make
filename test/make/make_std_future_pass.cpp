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


template <int = 0, int..., typename T>
  future<typename std::decay<T>::type> make_ready_future(T&& x)
{
  typedef typename std::decay<T>::type value_type;
  promise<value_type> p;
  p.set_value(std::forward<T>(x));
  return p.get_future();
}

// explicit overloads
template <typename T>
  future<T> make_ready_future(typename std::remove_reference<T>::type const& x)
{
  typedef T value_type;
  promise<value_type> p;
  p.set_value(x);
  return p.get_future();
}

template <typename T>
  future<T> make_ready_future(typename std::remove_reference<T>::type&& x)
{
  typedef T value_type;
  promise<value_type> p;
  p.set_value(std::forward<typename std::remove_reference<T>::type>(x));
  return p.get_future();
}

// variadic overload
template <typename T, typename ...Args>
  future<T> make_ready_future(Args&&... args)
{
  typedef T value_type;
  promise<value_type> p;
  p.set_value(value_type(std::forward<Args>(args)...));
  return p.get_future();

}

// customization point for template (needed because std::future don't uses experimental::in_place_t)
template <class DX, class X>
future<DX> make(std::experimental::type<std::future<DX>>, X&& x)
{
  return make_ready_future<DX>(std::forward<X>(x));
}

// customization point for template (needed because std::future don't uses experimental::in_place_t)
template <class X, class ...Args>
future<X> make(std::experimental::type<future<X>>, std::experimental::in_place_t, Args&& ...args)
{
  return make_ready_future<X>(std::forward<Args>(args)...);
}

// Holder specialization
template <>
struct future<std::experimental::_t> {};
template <>
struct future<std::experimental::_t&> {};

// customization point for holder
template <class X>
future<typename std::decay<X>::type> make(std::experimental::type<future<std::experimental::_t>>, X&& x)
{
  return std::experimental::make<future>(std::forward<X>(x));
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
  {
    int v=0;
    std::future<int> x = std::make_ready_future(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=0;
    std::future<int> x = std::experimental::make<std::future>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=0;
    std::future<int> x = std::make_ready_future<int>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=0;
    std::future<int> x = std::experimental::make<std::future<int>>(v);
    BOOST_TEST(x.get() == 0);
  }
  {
    int v=1;
    std::future<A> x = std::make_ready_future<A>(v,v);
    BOOST_TEST(x.get().v == 2);
  }
  {
    int v=1;
    std::future<A> x = std::experimental::make<std::future<A>>(v,v);
    BOOST_TEST(x.get().v == 2);
  }
  {
    std::future<int> x = std::make_ready_future<int>();
    BOOST_TEST_EQ(x.get(),  0);
  }
  {
    std::future<int> x = std::experimental::make<std::future<int>>();
    BOOST_TEST_EQ(x.get(),  0);
  }
  {
    int v=0;
    std::future<int&> x = std::make_ready_future<int&>(v);
    BOOST_TEST(&x.get() == &v);
  }
  {
    int v=0;
    std::future<int&> x = std::experimental::make<std::future<int&>>(v);
    BOOST_TEST(&x.get() == &v);
  }
  {
    using namespace std::experimental;
    int v=0;
    std::future<int> x = make<std::future<_t>>(v);
    BOOST_TEST(x.get() == 0);
  }
//  {
//    using namespace std::experimental;
//    int v=0;
//    auto x = make<std::future<_t&>>(v);
//    BOOST_TEST(&x.get() == &v);
//  }
  return ::boost::report_errors();
}
