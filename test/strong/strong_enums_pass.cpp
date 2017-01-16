// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/optionals.hpp>

#include <experimental/strong_enums.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

enum E { E0=1, E1, E2 };


struct SE {
    enum type { E0=2, E1, E2 };
};
namespace std
{
namespace experimental
{
template <>
bool is_enumerator<E>(E x) {
    switch (x) {
        case E0:
        case E1:
        case E2:
            return true;
        default:
            return false;
    }
}

template <>
bool is_enumerator<SE::type>(SE::type x) {
    switch (x) {
        case SE::E0:
        case SE::E1:
        case SE::E2:
            return true;
        default:
            return false;
    }
}
}}

typedef stdex::safe_enum<E, std::int8_t> EC;
typedef stdex::strong_enum<E, std::int8_t> ES;
typedef stdex::strong_enum<SE::type, std::int8_t> SSE;
typedef stdex::ordinal_enum<E, std::int8_t> OE;

void f(ES) {}
void f(SSE) {}

struct X {
    int i;
};

struct Y {
    int i;
};



int main()
{

  {
      E e=E0;
      BOOST_TEST(e==E0);
  }
  {
      E e=E();
      BOOST_TEST(e==E(0));
  }
  #if 0
  {
      X x;
      Y y = (Y)x;
  }
  #endif
  {
      X* xptr;
      Y* yptr = (Y*)xptr;
  }
  // throw if not a value
  {
    EC e;
      BOOST_TEST( ! stdex::is_valid_enumerator<E>(5) );
      try {        EC es(E(5));        } catch(...) {}
  }
  {
      ES es(stdex::uninitialized_t{});
      E e = es.enum_value();
      static_cast<void>(e);
      //BOOST_TEST(e==0);
  }
  {
      ES es(stdex::zero_initialized_t{});
      E e = es.enum_value();
      BOOST_TEST(e==0);
  }
  // default construct
  {
      ES es;
      f(ES());
      E e = es.get();
      BOOST_TEST(e==E(0));
  }
  // default construct
  {
      SSE es;
      f(SSE());
      SE::type e = es.get();
      BOOST_TEST(e==SE::type(0));
  }
  // constructs from  Enum
  {
      ES es = E1;
      f(E1);
      BOOST_TEST(es.get()==E1);
  }
  // assignment from  Enum
  {
      ES es = E1;
      es = E2;
      BOOST_TEST(es.get()==E2);
  }
  // implicit conversion to int or Enum fails
  {
      //ES es = E1;
      //int i  = es;
      //E e = es;
  }
  // copy constructor
  {
      ES es = E1;
      ES es2 = es;
      BOOST_TEST(es2.get()==E1);
  }
  // copy assignment
  {
      ES es = E1;
      ES es2;
      es2 = es;
      BOOST_TEST(es2.get()==E1);
  }
  // constructor from short fails
  {
      //unsigned short s;
      //ES es(s);
  }
  // implicit constructor from int fails
  {
      //ES es(1);
      //f(1); // compile fails as expected
  }
  {
      ES es(1);
  }
  // constructor from int fails
  {
      //ES es = 1;
  }

  return ::boost::report_errors();
}

