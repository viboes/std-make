// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Based on https://github.com/akrzemi1/explicit/blob/master/test/test_explicit.cpp

// <experimental/.../utility/only_when.hpp>

#include <experimental/utility.hpp>
#include <string>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

struct SuperInt
{
  operator int () const { return 1; }
};

struct SuperDouble
{
  operator double () const { return 1.5; }
};

struct NoConv {};


struct Rational
{
  int numerator, denominator;
  Rational(stdex::only_int num, stdex::only_int den = 0) : numerator(num.get()), denominator(den.get()) {}
};

void static_test_only_int_convertible()
{
  static_assert(std::is_constructible<Rational, int>::value, "failed only_int");
  static_assert(std::is_constructible<Rational, short>::value, "failed only_int");
  static_assert(std::is_constructible<Rational, SuperInt>::value, "failed only_int");

  static_assert(std::is_constructible<Rational, bool>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, float>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, double>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, long double>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, SuperDouble>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, NoConv>::value, "failed only_int");
  static_assert(!std::is_constructible<Rational, unsigned>::value, "failed only_int");
}

struct filesystem_path
{
  filesystem_path(std::string const&) {}
};

template <typename, typename S>
struct is_a_non_string : std::conditional<
  std::is_convertible<S, std::string>::value,
  std::false_type, std::true_type>::type
{};

void static_test_only_when_convertible()
{
  typedef stdex::only_when<filesystem_path, is_a_non_string> only_path;

  static_assert(std::is_convertible<filesystem_path, only_path>::value, "test failure");
  static_assert(!std::is_convertible<std::string, only_path>::value, "test failure");
  static_assert(!std::is_convertible<const char*, only_path>::value, "test failure");

  static_assert(std::is_constructible<filesystem_path, std::string>::value, "test failure");
  static_assert(std::is_constructible<filesystem_path, const char *>::value, "test failure");
  static_assert(!std::is_constructible<only_path, std::string>::value, "test failure");
  static_assert(!std::is_constructible<only_path, const char *>::value, "test failure");
}


int main()
{
  static_test_only_int_convertible();
  static_test_only_when_convertible();

  return ::boost::report_errors();
}

