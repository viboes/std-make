//! \file test_expected.cpp

// Copyright Pierre Talbot 2013.
// Copyright Vicente J. Botet Escriba 2013,2014.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <experimental/expected.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stde = std::experimental;

struct CopyableAndMovable
{
};

struct NoDefaultConstructible
{
  NoDefaultConstructible() = delete;
};

struct NoCopyConstructible
{
  NoCopyConstructible()  {}
  NoCopyConstructible(NoCopyConstructible const&) = delete;
  NoCopyConstructible(NoCopyConstructible &&) noexcept = default;
};
struct NoMoveConstructible
{
  NoMoveConstructible()  {}
  NoMoveConstructible(NoMoveConstructible const&) noexcept = default;
  NoMoveConstructible(NoMoveConstructible &&) = delete;
  NoMoveConstructible& operator=(NoMoveConstructible const&) noexcept = default;
  NoMoveConstructible& operator=(NoMoveConstructible &&) = delete;
};

struct X {};
inline bool operator==(const X&,const X&) { return true;}

struct Y {
    operator X() const { return X{}; }
};

struct Z {
    explicit operator X() const  { return X{}; }
};

int main()
{
#if 0
  static_assert(! std::is_default_constructible<NoDefaultConstructible>::value, "");
  static_assert(! std::is_default_constructible<expected_sc<NoDefaultConstructible>>::value, "");

  static_assert(! std::is_copy_constructible<NoCopyConstructible>::value, "");
  static_assert(! std::is_constructible<expected_sc<NoCopyConstructible>, NoCopyConstructible const& >::value, "");
  static_assert(! std::is_constructible<stde::exception_or<NoCopyConstructible>, stde::exception_or<NoCopyConstructible> const& >::value, "");
  static_assert(! std::is_copy_constructible<stde::exception_or<NoCopyConstructible>>::value, "");


  static_assert(! std::is_move_constructible<NoMoveConstructible>::value, "");
  static_assert( std::is_constructible<expected_sc<NoMoveConstructible>, NoMoveConstructible && >::value, "");
  static_assert( std::is_move_constructible<expected_sc<NoMoveConstructible>>::value, "");

#endif

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

    e.swap(e2);

    BOOST_TEST_EQ(e.value(), 8);
    BOOST_TEST_EQ(e2.value(), 5);

    e2.swap(e);

    BOOST_TEST_EQ(e.value(), 5);
    BOOST_TEST_EQ(e2.value(), 8);
  }


  return ::boost::report_errors();
}

