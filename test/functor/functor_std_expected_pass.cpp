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

int main()
{
  namespace stde = std::experimental;

  static_assert(stde::is_type_constructible<stde::expected<int>>::value, "ERROR");
  static_assert(std::is_base_of<stde::functor::tag, stde::functor::traits<stde::expected<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_functor<stde::expected<stde::_t>>::value, "ERROR");

  std::error_code ec = std::make_error_code(std::errc(1));

  {
    stde::exception_or<int> x = stde::make_unexpected(1);
    BOOST_TEST(! x);

    stde::exception_or<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(! y);
  }
  {
    stde::expected<int> x = stde::make_unexpected(ec);
    BOOST_TEST(! x);

    stde::expected<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(! y);
  }
  {
    int v=1;
    stde::expected<int> x = stde::make_expected(v);
    BOOST_TEST(*x == 1);
    stde::expected<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
    x = 2;
    BOOST_TEST(*x == 2);
  }
  // fixme should transform be const? How the move can be done then? Or should expected don't do a move?
#if 0
  {
    int v=1;
    const stde::expected<int> x = stde::make_expected(v);
    BOOST_TEST(*x == 1);

    stde::expected<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
  }
#endif
#if defined __clang__ && __clang_major__ >= 4 && __cplusplus > 201402L
  {
    auto x = stde::make_expected<const int>(1);
    static_assert(std::is_same<decltype(x), stde::expected<const int>>::value, "");
    BOOST_TEST(*x == 1);
    stde::expected<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
  }
#endif

  return ::boost::report_errors();
}
