// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/functor.hpp>
// <experimental/expected.hpp>


#include <experimental/expected.hpp>

#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

namespace stde = std::experimental;
template <class T>
using expected_sc = stde::expected<T, std::error_code>;


int main()
{

  static_assert(stde::is_type_constructible<expected_sc<int>>::value, "ERROR");
  static_assert(std::is_base_of<stde::functor::tag, stde::functor::traits<expected_sc<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_functor<expected_sc<stde::_t>>::value, "ERROR");

  std::error_code ec = std::make_error_code(std::errc(1));

  {
    stde::exception_or<int> x = stde::make_unexpected(1);
    BOOST_TEST(! x);

    stde::exception_or<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(! y);
  }
  {
    expected_sc<int> x = stde::make_unexpected(ec);
    BOOST_TEST(! x);

    expected_sc<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(! y);
  }
  {
    int v=1;
    expected_sc<int> x = stde::make_expected(v);
    BOOST_TEST(*x == 1);
    expected_sc<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
    x = 2;
    BOOST_TEST(*x == 2);
  }
  {
    int v=1;
    const expected_sc<int> x = stde::make_expected(v);
    BOOST_TEST(*x == 1);

    expected_sc<int> y = x.map(twice);
    BOOST_TEST(*y == 2);
  }
  {
    int v=1;
    const expected_sc<int> x = stde::make_expected(v);
    BOOST_TEST(*x == 1);


    expected_sc<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
  }
  {
    int v=1;
    const expected_sc<int> x = stde::make_expected(v);
    BOOST_TEST(*x == 1);

    expected_sc<int> y = stde::functor::map(twice, x);
    BOOST_TEST(*y == 2);
  }
  // fixme should transform be const? How the move can be done then? Or should expected don't do a move?
#if defined __clang__ && __clang_major__ >= 4 && __cplusplus > 201402L
  {
    expected_sc<const int> x = stde::make_expected<const int>(1);
    static_assert(std::is_same<decltype(x), expected_sc<const int>>::value, "");
    BOOST_TEST(*x == 1);
    expected_sc<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
  }
#endif

  return ::boost::report_errors();
}
