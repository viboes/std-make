// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_type.hpp>

/**
 * This example tries to see how close we can define address/offset using strong_type
 *
 */
#include <boost/detail/lightweight_test.hpp>
#include <iostream>
#include <cstdint>

#include <experimental/strong_counter.hpp>
#include <experimental/strong_random_incrementable.hpp>


namespace stdex = std::experimental;

struct offset_tag {};

namespace std {
namespace experimental {
namespace mixin {
  template <>
  struct is_compatible_with<offset_tag, offset_tag> : std::true_type {};
}
}
}

using offset = stdex::strong_counter<offset_tag, std::int64_t>;

using address = stdex::strong_random_incrementable<struct address_tag, std::int64_t, offset>;



template <class T> struct check;
int main()
{
  {
    offset o1 {3};
    offset o2 {4};
    auto o =  o1 + o2;
    BOOST_TEST_EQ(o.underlying() , 7);
  }
  {
    address a {4};
    address a2 = ++a;
    BOOST_TEST_EQ(a2.underlying() , 5);
    BOOST_TEST_EQ(a.underlying() , 5);
  }
  {
    address a {4};
    address a2 = a++;
    BOOST_TEST_EQ(a2.underlying() , 4);
    BOOST_TEST_EQ(a.underlying() , 5);
  }
//  {
//    address a1 {4};
//    address a2 {3};
//    auto a =  a1 + a2; // compile fails as expected
//  }
  {
    address a1 {4};
    address a2 {3};
    offset a =  a1 - a2;
    BOOST_TEST_EQ(a.underlying() , 1);
  }
  {
    address a1 {4};
    offset o {4};
    address a2 =  a1 + o;
    BOOST_TEST_EQ(a2.underlying() , 8);
  }
  {
    address a1 {4};
    offset o {4};
    address a2 =  a1 + 3 * o;
    BOOST_TEST_EQ(a2.underlying() , 16);
  }
  {
    address a {4};
    offset o {4};
    a += o;
    BOOST_TEST_EQ(a.underlying() , 8);
  }
  {
    address a1 {4};
    offset o {4};
    address a2 =  a1 - o;
    BOOST_TEST_EQ(a2.underlying() , 0);
  }
  {
    address a {4};
    offset o {4};
    a -=  o;
    BOOST_TEST_EQ(a.underlying() , 0);
  }
  {
    address a {4};
    offset o {4};
    advance(a,o);
    BOOST_TEST_EQ(a.underlying() , 8);
  }
  {
    address a {4};
    address b {4};
    auto x =  distance(a,b);
    BOOST_TEST_EQ(x.underlying() , 0);
  }
  {
    address a {4};
    auto x =  next(a);
    BOOST_TEST_EQ(x.underlying() , 5);
  }
  {
    address a {4};
    auto x =  prev(a);
    BOOST_TEST_EQ(x.underlying() , 3);
  }
    return ::boost::report_errors();
}

