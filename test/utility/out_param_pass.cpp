// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Based on https://github.com/akrzemi1/explicit/blob/master/test/test_explicit.cpp

// <experimental/.../utility/out_param.hpp>

#include <experimental/utility.hpp>
#include <string>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

struct Big1 { std::string data; Big1(Big1&&) = delete; Big1() = default; };
struct Big2 { std::string data; Big2(Big2&&) = delete; Big2() = default; };

struct Processor
{
  Big1 const& _big1;
  Big2 const& _big2;
  explicit Processor(stdex::lvalue_ref<const Big1> b1, stdex::lvalue_ref<const Big2> b2) : _big1(b1), _big2(b2) {}
  bool process() { return _big1.data == _big2.data; } // this triggers UB on dangling references
};

//# if (defined __GNUC__) && (!defined __clang__) && (__GNUC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ < 8 || __GNUC__ == 4 && __GNUC_MINOR__ == 8 && __GNUC_PATCHLEVEL__ < 1)
//
//void static_test_proxy_ref(){}
//
//# else

struct ProxyReference
{
  int i;
  operator int&() & { return i; }
  operator const int&() const & { return i; }
  operator const int&() const && = delete;
};

void static_test_proxy_ref()
{
  static_assert(std::is_convertible<ProxyReference&, stdex::lvalue_ref<int>>::value, "test failure");
  static_assert(std::is_convertible<ProxyReference const&, stdex::lvalue_ref<const int>>::value, "test failure");
  static_assert(!std::is_convertible<ProxyReference&&, stdex::lvalue_ref<int>>::value, "test failure");
  static_assert(!std::is_convertible<ProxyReference&&, stdex::lvalue_ref<const int>>::value, "test failure");
  static_assert(!std::is_convertible<ProxyReference const&&, stdex::lvalue_ref<const int>>::value, "test failure");
}
//# endif

void static_lvalue_ref_conversions_succeed()
{
  static_assert(std::is_constructible<Processor, const Big1&, const Big2&>::value, "test failure");
  static_assert(std::is_constructible<Processor, std::reference_wrapper<const Big1>&, std::reference_wrapper<const Big2>>::value, "test failure");
}

void static_rvalue_ref_conversions_fail()
{
  static_assert(!std::is_constructible<Processor, Big1, const Big2&>::value, "test failure");
  static_assert(!std::is_constructible<Processor, const Big1&, const Big2&&>::value, "test failure");
}

void demonstrate_lvalue_ref()
{
  const Big1 b1 {}; // just shows usage
  const Big2 b2 {};
  Processor p {b1, b2};
  (void)p.process();
}


int main()
{
  static_test_proxy_ref();
  static_lvalue_ref_conversions_succeed();
  static_rvalue_ref_conversions_fail();
  demonstrate_lvalue_ref();

  return ::boost::report_errors();
}

