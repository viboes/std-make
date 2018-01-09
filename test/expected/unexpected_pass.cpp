//! \file unexpected_pass.cpp

// Copyright Vicente J. Botet Escriba 2018.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <experimental/expected.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stde = std::experimental;

struct X {};
inline bool operator==(const X&,const X&) { return true;}

struct Y {
    operator X() const { return X{}; }
};

struct Z {
    explicit operator X() const  { return X{}; }
};

static_assert( std::is_constructible<stde::unexpected<X>, stde::unexpected<Y>>::value, "");
static_assert( std::is_convertible<stde::unexpected<Y>, stde::unexpected<X>>::value, "");
static_assert( std::is_constructible<stde::unexpected<X>, stde::unexpected<Z>>::value, "");
static_assert( ! std::is_convertible<stde::unexpected<Z>, stde::unexpected<X>>::value, "");

int main()
{


#if 0
  {
    stde::unexpected<X> e; // compile fails as expected
  }
  {
    stde::unexpected<X> e = X{}; // compile fails as expected
  }
  {
    stde::unexpected<X> e = Y{}; // compile fails as expected
  }
  {
    stde::unexpected<X> e{X{}};
    e = X{}; // compile fails as expected
  }
  {
      stde::unexpected<Z> e2{Z{}};
      stde::unexpected<X> e1{e2};
      BOOST_TEST(e1 == e2);  // compile fails as expected
  }
  {
      stde::unexpected<Z> e2{Z{}};
      stde::unexpected<X> e1{X{}};
      e1 = e2; // compile fails as expected
  }

#endif
  {
    stde::unexpected<X> e{X{}};
  }
  {
    stde::unexpected<X> e{Y{}};
  }
  {
    stde::unexpected<X> e{Z{}};
  }
  {
      stde::unexpected<int> e1{1};
      stde::unexpected<int> e2{e1};
      BOOST_TEST(e1 == e2);
  }
  {
      stde::unexpected<int> e1{1};
      stde::unexpected<int> e2{2};
      e1 = e2;
      BOOST_TEST(e1 == e2);
      BOOST_TEST_EQ(e1.value(), 2);
  }
  {
      stde::unexpected<X> e2{X{}};
      stde::unexpected<X> e1{e2};
      BOOST_TEST(e1 == e2);
  }
  {
      stde::unexpected<Y> e2{Y{}};
      stde::unexpected<X> e1{e2};
      BOOST_TEST(e1 == e2);
  }
  {
      stde::unexpected<Y> e2{Y{}};
      stde::unexpected<X> e1 = e2;
      BOOST_TEST(e1 == e2);
  }
  {
      stde::unexpected<Y> e2{Y{}};
      stde::unexpected<X> e1{X{}};
      e1 = e2;
      BOOST_TEST(e1 == e2);
  }
  {
    stde::unexpected<int> e(5);
    stde::unexpected<int> e2(8);

    using std::swap;
    swap(e, e2);

    BOOST_TEST_EQ(e.value(), 8);
    BOOST_TEST_EQ(e2.value(), 5);

    swap(e2, e);

    BOOST_TEST_EQ(e.value(), 5);
    BOOST_TEST_EQ(e2.value(), 8);
  }


  return ::boost::report_errors();
}

