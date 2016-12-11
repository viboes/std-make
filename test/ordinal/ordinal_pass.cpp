// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/ordinal.hpp>

#include <experimental/ordinal.hpp>
#include "Bool.hpp"
#include "Bounded.hpp"

#include <cassert>

#include <boost/detail/lightweight_test.hpp>

int main()
{
#if __cplusplus >= 201402L

  namespace stde = std::experimental;

  {
    static_assert(stde::ordinal::meta::size<Bool>::value == 2, "");
    static_assert(stde::ordinal::meta::pos<Bool, Bool::False>::value == 0, "");
    static_assert(stde::ordinal::meta::pos<Bool, Bool::True>::value == 1, "");
    static_assert(stde::ordinal::meta::val<Bool, 0>::value == Bool::False, "");
    static_assert(stde::ordinal::meta::val<Bool, 1>::value == Bool::True, "");
  }
  {
    assert(stde::ordinal::size<Bool>() == 2);
    assert(stde::ordinal::traits<Bool>::pos(Bool::False) == 0);
    assert(stde::ordinal::pos(Bool::False) == 0);
    assert(stde::ordinal::pos(Bool::True) == 1);
    assert(stde::ordinal::val<Bool>(0) == Bool::False);
    assert(stde::ordinal::val<Bool>(1) == Bool::True);
    assert(stde::ordinal::first<Bool>() == Bool::False);
    assert(stde::ordinal::last<Bool>() == Bool::True);
    assert(stde::ordinal::succ(Bool::False) == Bool::True);
    assert(stde::ordinal::pred(Bool::True) == Bool::False);
  }
  {
    static_assert(stde::ordinal::meta::size<Bounded<0,3,int>>::value == 4, "");
  }
  {
    using T = Bounded<1,4,unsigned char>;
    assert(stde::ordinal::size<T>() == 4);
    assert(stde::ordinal::pos(T{1}) == 0);
    assert(stde::ordinal::pos(T{2}) == 1);
    assert(stde::ordinal::pos(T{3}) == 2);
    assert(stde::ordinal::pos(T{4}) == 3);
    assert(stde::ordinal::val<T>(0).value == 1);
    assert(stde::ordinal::val<T>(1).value == 2);
    assert(stde::ordinal::first<T>().value == 1);
    assert(stde::ordinal::last<T>().value == 4);
    assert(stde::ordinal::succ(T{1}).value == 2);
    assert(stde::ordinal::succ(T{2}).value == 3);
    assert(stde::ordinal::succ(T{3}).value == 4);
    assert(stde::ordinal::pred(T{4}).value == 3);
    assert(stde::ordinal::pred(T{3}).value == 2);
    assert(stde::ordinal::pred(T{2}).value == 1);

  }
#endif
  return ::boost::report_errors();
}
