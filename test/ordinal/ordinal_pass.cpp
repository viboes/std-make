// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/ordinal.hpp>

#include <experimental/ordinal.hpp>
#include "Bool.hpp"
#include "Bounded.hpp"

#include <boost/detail/lightweight_test.hpp>

int main()
{
  namespace stde = std::experimental;

  {
    static_assert(stde::ordinal::meta::size<Bool>::value == 2, "");
    static_assert(stde::ordinal::meta::pos<Bool, Bool::False>::value == 0, "");
    static_assert(stde::ordinal::meta::pos<Bool, Bool::True>::value == 1, "");
    static_assert(stde::ordinal::meta::val<Bool, 0>::value == Bool::False, "");
    static_assert(stde::ordinal::meta::val<Bool, 1>::value == Bool::True, "");
  }
  {
    BOOST_TEST(stde::ordinal::size<Bool>() == 2);
    BOOST_TEST(stde::ordinal::traits<Bool>::pos(Bool::False) == 0);
    BOOST_TEST(stde::ordinal::pos(Bool::False) == 0);
    BOOST_TEST(stde::ordinal::pos(Bool::True) == 1);
    BOOST_TEST(stde::ordinal::val<Bool>(0) == Bool::False);
    BOOST_TEST(stde::ordinal::val<Bool>(1) == Bool::True);
    BOOST_TEST(stde::ordinal::first<Bool>() == Bool::False);
    BOOST_TEST(stde::ordinal::last<Bool>() == Bool::True);
    BOOST_TEST(stde::ordinal::succ(Bool::False) == Bool::True);
    BOOST_TEST(stde::ordinal::pred(Bool::True) == Bool::False);
  }
  {
    static_assert(stde::ordinal::meta::size<Bounded<0,3,int>>::value == 4, "");
  }
  {
    using T = Bounded<1,4,unsigned char>;
    BOOST_TEST(stde::ordinal::size<T>() == 4);
    BOOST_TEST(stde::ordinal::pos(T{1}) == 0);
    BOOST_TEST(stde::ordinal::pos(T{2}) == 1);
    BOOST_TEST(stde::ordinal::pos(T{3}) == 2);
    BOOST_TEST(stde::ordinal::pos(T{4}) == 3);
    BOOST_TEST(stde::ordinal::val<T>(0).value == 1);
    BOOST_TEST(stde::ordinal::val<T>(1).value == 2);
    BOOST_TEST(stde::ordinal::first<T>().value == 1);
    BOOST_TEST(stde::ordinal::last<T>().value == 4);
    BOOST_TEST(stde::ordinal::succ(T{1}).value == 2);
    BOOST_TEST(stde::ordinal::succ(T{2}).value == 3);
    BOOST_TEST(stde::ordinal::succ(T{3}).value == 4);
    BOOST_TEST(stde::ordinal::pred(T{4}).value == 3);
    BOOST_TEST(stde::ordinal::pred(T{3}).value == 2);
    BOOST_TEST(stde::ordinal::pred(T{2}).value == 1);

  }
  return ::boost::report_errors();
}
