// Copyright (C) 2014-2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/value_or_none.hpp>

#include <experimental/value_or_none.hpp>
#include <experimental/meta.hpp>

//#include <memory>
//#include <algorithm>
//#include <iterator>
//#include <vector>
#define JASEL_HAS_EXPERIMENTAL_ANY 0
#if JASEL_HAS_EXPERIMENTAL_ANY
#include <experimental/any>
#endif
#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

template <class T>
struct check;

template <class T>
struct A
{
  T v;
  A(): v(2) {}
  A(T v_): v(std::move(v_)) {}
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

// Holder specialization
template <>
struct A<std::experimental::_t> : std::experimental::meta::quote<A> {};

namespace std
{
  namespace experimental
  {
    // type_constructor customization
    template <class T>
    struct type_constructor<A<T>> : meta::id<A<_t>>
    {
    };

  }
}

//void accept_nullptr_t(std::nullptr_t) {
//}
//struct nullopt_t2
//{
//  struct init{};
//  constexpr nullopt_t2(init){}
//  //constexpr nullopt_t2(){}
//};
//constexpr nullopt_t2 nullopt2{nullopt_t2::init{}};
//
//void accept_nullopt_t2(nullopt_t2) {
//}
//template <template <class ...> class TC, class T>
//TC<T> make_none(T) {
//  //return TC{};
//  return std::add_pointer_t<T>{};
//}
//
//template <template <class ...> class TC, class T>
//TC<T> make_none3(T) {
//  return std::experimental::none<TC>();
//}
//template <template <class ...> class TC, class T>
//TC<T> make_none4(T) {
//  return std::experimental::none<TC<T>>();
//}
//template <class TC, class T>
//std::experimental::meta::invoke<TC, T> make_none5(T) {
//  return std::experimental::none<TC>();
//}
//
//#if JASEL_HAS_EXPERIMENTAL_ANY
//
//template <class>
//using any_t = std::experimental::any;
//
//template <template <class ...> class TC, class T>
//TC<T> make_none2(T) {
//  //return TC{};
//  return any_t<T>{};
//}
//#endif
int main()
{
  namespace stde = std::experimental;

//  {
//    auto ip = make_none<std::add_pointer_t>(0);
//    BOOST_TEST(ip == nullptr);
//  }
//  // no type named 'type' in 'std::__1::add_pointer<experimental::_t>'
//  static_assert(std::is_same<
//      stde::meta::invoke<stde::add_pointer_f, int>,
//      int*
//    >::value, "ERROR");
//  static_assert(std::is_same<
//      stde::value_or_none::value_type_t<int*>,
//      int
//    >::value, "ERROR");
//  {
//    auto ip = make_none5<stde::add_pointer_f>(0);
//    BOOST_TEST(ip == nullptr);
//  }
//  // no type named 'type' in 'std::__1::add_pointer<experimental::_t>'
//  {
//    auto ip = make_none4<std::add_pointer_t>(0);
//    BOOST_TEST(ip == nullptr);
//  }
//#if JASEL_HAS_EXPERIMENTAL_ANY
//  {
//    std::experimental::any a,b;
//    //BOOST_TEST(a == b); // compile fail: std::experimental::any can not be a Nullable as is not EqualityComparable :(
//    BOOST_TEST(a.empty());
//    auto x = make_none2<any_t>(0);
//    //BOOST_TEST(x == std::experimental::any{});
//    BOOST_TEST(x.empty());
//  }
//#endif
//  {
//    nullopt_t2 ip = nullopt2;
//    accept_nullopt_t2(ip);
//    accept_nullopt_t2(nullopt2);
//    ip = nullopt2;
//  }
//  {
//    //accept_nullopt_t2(nullopt_t2{}); // compile fails
//    //accept_nullopt_t2(nullopt_t2()); // compile fails
//    //accept_nullopt_t2({}); // compile fails
//    //nullopt_t2 ip = {};  // compile fails
//    //accept_nullopt_t2(ip);
//  }
//  {
//    std::nullptr_t ip = nullptr;
//    accept_nullptr_t(ip);
//    accept_nullptr_t(nullptr);
//    ip = nullptr;
//  }
//  {
//    auto np = std::nullptr_t{}; // shouldn't compile fails :(
//    accept_nullptr_t(np);
//    std::nullptr_t np1 = {}; // // shouldn't compile fails :(
//    accept_nullptr_t(np1);
//    accept_nullptr_t(std::nullptr_t()); // shouldn't compile fails :)
//    accept_nullptr_t({}); // shouldn't compile fails :)
//  }
//  static_assert(stde::is_value_or_none<int*>::value, "ERROR");
//  {
//    int * x = nullptr;
//    BOOST_TEST(! stde::value_or_none::has_value(x));
//    BOOST_TEST(x == stde::none());
//    BOOST_TEST(stde::none() == x);
//    BOOST_TEST(stde::value_or_none::deref_none(x) == nullptr);
//  }
//  {
//    int * x = stde::none();
//    BOOST_TEST(! stde::value_or_none::has_value(x));
//    BOOST_TEST(x == stde::none());
//    BOOST_TEST(stde::none() == x);
//    BOOST_TEST(stde::value_or_none::deref_none(x) == nullptr);
//  }
//  {
//    int * x = stde::none<std::add_pointer<stde::_t>>();
//    BOOST_TEST(! stde::value_or_none::has_value(x));
//    BOOST_TEST(x == stde::none());
//    BOOST_TEST(stde::none() == x);
//    BOOST_TEST(stde::value_or_none::deref_none(x) == nullptr);
//  }
//#if 0
//// no type named 'type' in 'std::__1::add_pointer<experimental::_t>'
//  {
//    int * x = stde::none<std::add_pointer_t>();
//  }
//#endif
//  {
//    const int * x = nullptr;
//    BOOST_TEST(! stde::value_or_none::has_value(x));
//    BOOST_TEST(x == stde::none());
//    BOOST_TEST(stde::none() == x);
//    BOOST_TEST(stde::value_or_none::deref_none(x) == nullptr);
//  }
//  {
//    const int * x = stde::none();
//    BOOST_TEST(! stde::value_or_none::has_value(x));
//    BOOST_TEST(x == stde::none());
//    BOOST_TEST(stde::none() == x);
//    BOOST_TEST(stde::value_or_none::deref_none(x) == nullptr);
//  }
//  {
//    const int * x = stde::none<std::add_pointer<stde::_t>>();
//    BOOST_TEST(! stde::value_or_none::has_value(x));
//    BOOST_TEST(x == stde::none());
//    BOOST_TEST(stde::none() == x);
//    BOOST_TEST(x == stde::none<std::add_pointer<stde::_t>>());
//    BOOST_TEST(stde::value_or_none::deref_none(x) == nullptr);
//  }
//  {
//    int * x = new int(1);
//    BOOST_TEST(stde::value_or_none::has_value(x));
//    BOOST_TEST(x != stde::none());
//    BOOST_TEST(stde::none() != x);
//    BOOST_TEST(stde::value_or_none::deref(x) == 1);
//    delete x;
//  }
//  {
//    const int * x = new int(1);
//    BOOST_TEST(stde::value_or_none::has_value(x));
//    BOOST_TEST(x != stde::none());
//    BOOST_TEST(stde::none()  != x);
//    BOOST_TEST(stde::value_or_none::deref(x) == 1);
//    delete x;
//  }
//  //value_or_error::value_or
//  {
//    int * x = nullptr;
//    auto y = stde::value_or_error::value_or(x, 1);
//    BOOST_TEST_EQ(y, 1);
//  }
//  {
//    int * x = new int(1);
//    int y = stde::value_or_error::value_or(x, 2);
//    delete x;
//    BOOST_TEST(y == 1);
//  }
//  //value_or_error::apply_or
//  {
//    int * x = nullptr;
//    int y = stde::value_or_error::apply_or(x, twice, 1);
//    BOOST_TEST(y == 1);
//  }
//  {
//    int * x = new int(1);
//    int y = stde::value_or_error::apply_or(x, twice, -1);
//    delete x;
//    BOOST_TEST(y == 2);
//  }
  return ::boost::report_errors();
}
