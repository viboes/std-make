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
#include <memory>
#include <boost/detail/lightweight_test.hpp>

namespace std {

//template <int = 0, int..., typename T>
//  shared_ptr<typename decay<T>::type> make_shared(T&& x)
//{
//  typedef typename decay<T>::type X;
//  return shared_ptr<X>(new X(x));
//}
//
//// explicit overloads
//template <typename T>
//  shared_ptr<T> make_shared(typename remove_reference<T>::type const& x)
//{
//  typedef T X;
//  return shared_ptr<X>(new X(x));
//}
//
//template <typename T>
//  shared_ptr<T> make_shared(typename remove_reference<T>::type&& x)
//{
//  typedef T X;
//  return shared_ptr<X>(new X(std::forward<typename remove_reference<T>::type>(x)));
//}
//
//// variadic overload
//template <typename T, typename ...Args>
//  shared_ptr<T> make_shared(Args&&... args)
//{
//  typedef T X;
//  return shared_ptr<X>(new X(forward<Args>(args)...));
//}

// customization point for template (needed because std::shared_ptr doesn't has a conversion constructor)
template <class DX, class X>
shared_ptr<DX> make(experimental::type<shared_ptr<DX>>, X&& x)
{
  return make_shared<DX>(forward<X>(x));
}

// customization point for template (needed because std::shared_ptr doesn't uses experimental::in_place_t)
template <class X, class ...Args>
shared_ptr<X> make(experimental::type<shared_ptr<X>>, experimental::in_place_t, Args&& ...args)
{
  return make_shared<X>(forward<Args>(args)...);
}

// Holder customization
template <>
struct shared_ptr<experimental::_t> : experimental::lift<shared_ptr> {};

// todo remove this specialization
template <>
struct shared_ptr<void> {};

namespace experimental
{
// type_constructor customization
template <class T>
struct type_constructor<shared_ptr<T>> : identity<shared_ptr<_t>> {};
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
