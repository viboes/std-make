// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/signature.hpp>


#include <experimental/fundamental/v2/signature.hpp>
#include <experimental/fundamental/v3/value_type.hpp>
#include <experimental/meta/v1/id.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/in_place.hpp>
#include <experimental/fundamental/v2/functor/functor_tag.hpp>
#include <experimental/fundamental/v2/concept_map.hpp>
#include <experimental/meta/v1/invoke.hpp>
#include <experimental/make.hpp>
#include <experimental/fundamental/v2/functor/functor_tag.hpp>
#include <experimental/fundamental/v2/functor/fmap.hpp>

#include <boost/detail/lightweight_test.hpp>

template <class T>
struct A
{
  T v;
  A(): v(2) {}
  A(A&&) = default;
  A(A const&) = default;
  A(T v): v(std::move(v)) {}
  A(std::experimental::in_place_t): v(3) {}
  A(std::experimental::in_place_t, T v): v(std::move(v)) {}
  A(std::experimental::in_place_t, T v1, T v2): v(v1+v2) {}
};
template <class T>
struct A<T&>
{
  T* ptr;
  A(T& v): ptr(&v) {}
};
template <>
struct A<void>
{
  int v;
  A() : v(0) {}
};

template <class T>
A<typename std::decay<T>::type> makeA(T&& v) {
  return A<typename std::decay<T>::type>(std::forward<T>(v));
}

// Holder specialization
template <>
struct A<std::experimental::_t> : std::experimental::meta::quote<A> {};


namespace std
{
namespace experimental
{
//inline namespace fundamental_v2
//{

  template <class A, class B, typename F, typename C>
  meta::invoke<F, B> fmap2(
      signature<C, B(A)> c,
      meta::invoke<F, A>&& f
  )
  {
    return concept_instance_t<functorv2, F>::fmap_impl(std::forward<signature<C, B(A)>>(c), std::forward<meta::invoke<F, A>>(f));
  }

  template <class A, class B, template <class> class F>
  F<B> fmap3(
      B(*c)(A),
      F<A>&  f
  )
  {
    return concept_instance_t<functorv2, F<_t> >::fmap_impl(c, f);
  }

template <class T>
struct functorv2::instance<A<T>> : std::true_type
{
  template <class C, class F>
  static auto fmap_impl(C&& c, F&& f) -> decltype(makeA(0))
  {
    if (f.v)
      return makeA(c(f.v));
    else
      return makeA(0);
  }
};

  // type_constructor customization
  template <class T>
  struct type_constructor<A<T>> : meta::id<A<_t>>  {  };
//}
}
}


int f (int a) {
  return 2*a;
}

template <class A, class R>
R deduce_A_R (R(*f)(A), A a) {
  return f(a);
}

//template <class T, class R>
//R ap ((T->R) f, T a) {
//  return f(a);
//}

template <template <class> class TC, class T>
T deduce_TC_T (TC<T> a) {
  return a.v;
}


template <class F, class T>
int h (std::experimental::meta::invoke<F, T>&& a) {
  return 2*a.v;
}


int main()
{
  namespace stde = std::experimental;
  {
    int a(2);
    int b = f(a);
    BOOST_TEST(b == 4);
  }
  {
    int a(2);
    int b = deduce_A_R(f, a);
    BOOST_TEST(b == 4);
  }
  {
    A<int> a(2);
    int b = h<A<stde::_t>, int>(std::move(a));
    BOOST_TEST(b == 4);
  }
  {
    A<int> a(2);
    int b = deduce_TC_T(a);
    BOOST_TEST(b == 2);
  }
  {
    A<int> a= makeA(2);
    int b = h<A<stde::_t>, int>(std::move(a));
    BOOST_TEST(b == 4);
  }
  {
    stde::signature<int(*)(int), int(int)> sig = f;
    int a(2);
    int b = sig(a);
    BOOST_TEST(b == 4);
  }
  {
    auto sig = stde::make_signature<int, int>(f);
    int a(2);
    int b = sig(a);
    BOOST_TEST(b == 4);
  }
  {
    A<int> a(2);
    A<int> b = stde::fmap(f,a);

    BOOST_TEST(b.v == 4);
  }
  {
    A<int> a(2);
    A<int> b = stde::fmap3(f,a);

    BOOST_TEST(b.v == 4);
  }
  {
    A<int> a(2);
    A<int> b = stde::fmap2<int, int, A<stde::_t> >(stde::make_signature<int, int>(f),std::move(a));

    BOOST_TEST(b.v == 4);
  }
  return ::boost::report_errors();
}
