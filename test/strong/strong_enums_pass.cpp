// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_enums.hpp>

#include <experimental/strong_enums.hpp>
#include <utility>
#include <sstream>
#include <iostream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

enum E { E0=1, E1, E2 };


struct SE {
    enum type { E0=2, E1, E2 };
};
enum class byte : unsigned char {};
enum class Other  {A,B,C};

namespace std
{
namespace experimental
{
template <>
bool is_enumerator<E>(int x) {
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
bool is_enumerator<SE::type>(int x) {
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

#if __cplusplus <= 201402L || (! defined __clang__ && defined __GNUC__ && __GNUC__ <= 6)
static_assert(std::is_pod<EC>::value, "");
static_assert(std::is_trivially_default_constructible<EC>::value, "");
static_assert(std::is_trivially_copyable<EC>::value, "");
static_assert(std::is_standard_layout<EC>::value, "");
static_assert(std::is_trivial<EC>::value, "");
static_assert(std::is_trivially_destructible<EC>::value, "");
static_assert(std::is_nothrow_default_constructible<EC>::value, "");

static_assert(std::is_pod<ES>::value, "");
static_assert(std::is_trivially_default_constructible<ES>::value, "");
static_assert(std::is_trivially_copyable<ES>::value, "");
static_assert(std::is_standard_layout<ES>::value, "");
static_assert(std::is_trivial<ES>::value, "");
static_assert(std::is_trivially_destructible<ES>::value, "");
static_assert(std::is_nothrow_default_constructible<ES>::value, "");

static_assert(std::is_pod<OE>::value, "");
static_assert(std::is_trivially_default_constructible<OE>::value, "");
static_assert(std::is_trivially_copyable<OE>::value, "");
static_assert(std::is_standard_layout<OE>::value, "");
static_assert(std::is_trivial<OE>::value, "");
static_assert(std::is_trivially_destructible<OE>::value, "");
static_assert(std::is_nothrow_default_constructible<OE>::value, "");
#endif

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
  // throw if not a value
  {
    //EC e;
      BOOST_TEST( ! stdex::is_valid_enumerator<E>(5) );
      try {
          EC es(E(5));
          (void)es;
      } catch(...) {}
  }
  {
      ES es;
      static_cast<void>(es);
  }
  {
      ES es = ES();
      E e = es.to_enum();
      BOOST_TEST(e==0);
  }
  {
      ES es = ES();
      E e = stdex::to_enum(es);
      E e2 = stdex::to_enum(e);
      BOOST_TEST(e2==0);
  }
  // default construct
  {
      ES es;
      f(ES());
      (void)es;
      //not initialized
      //E e = es.to_enum();
      //BOOST_TEST(e==E(0));
  }
  {
      ES es{};
      f(ES{});
      E e = es.to_enum();
      BOOST_TEST(e==E(0));
  }
  {
      ES es= ES();
      f(ES{});
      E e = es.to_enum();
      BOOST_TEST(e==E(0));
  }
  // default construct
  {
      SSE es;
      f(SSE());
      (void)es;
      //not initialized
      //SE::type e = es.to_enum();
      //BOOST_TEST(e==SE::type(0));
  }
  // constructs from  Enum
  {
      ES es = E1;
      f(E1);
      BOOST_TEST(es.to_enum()==E1);
  }
  // assignment from  Enum
  {
      ES es = E1;
      es = E2;
      BOOST_TEST(es.to_enum()==E2);
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
      BOOST_TEST(es2.to_enum()==E1);
  }
  // copy assignment
  {
      ES es = E1;
      ES es2;
      es2 = es;
      BOOST_TEST(es2.to_enum()==E1);
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
      (void)es;
  }
  // constructor from int fails
  {
      //ES es = 1;
  }

  // swap
  {
    ES es1 = E1;
    ES es2 = E2;
    using std::swap;
    swap(es1,es2);
    BOOST_TEST(es1.to_enum()==E2);
    BOOST_TEST(es2.to_enum()==E1);
  }
  // operator <<
  {
      ES oc{E1};
      std::stringstream os;
      os << int(oc.to_enum());  // We need to convert to int to have a printable characters type, but we can not do it directly.
      BOOST_TEST_EQ(os.str(), "2");
  }
  // operator >>
  {
      ES oc;
      std::stringstream s;
      s << ES{E1};
      s >> oc;
      BOOST_TEST_EQ(oc.underlying(), 2);
      BOOST_TEST_EQ(oc.to_enum(), E1);
      BOOST_TEST_EQ(oc, ES{E1});
  }
  // hash
  {
    ES es1 = E1;

    //auto h = std::hash<stdex::strong_enum<E,std::int8_t>>{};
    //auto h1 = std::hash<E>{}(E1);
    //auto h2 = std::hash<ES>{}(es1);
#if __cplusplus >= 201402L
    BOOST_TEST(std::hash<ES>{}(es1)==std::hash<E>{}(E1));
#else
    BOOST_TEST(std::hash<ES>{}(es1)==std::hash<int>{}(int(E1)));
#endif
  }
  {
    //Other o {1};
    //byte b {1};
    //std::cout << b << "\n";
  }

  return ::boost::report_errors();
}
