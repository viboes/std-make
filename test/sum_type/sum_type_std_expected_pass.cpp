// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/sum_type.hpp>
// <experimental/expected.hpp>

#if __cplusplus < 201402L
int main()
{
}
#else

#include <experimental/expected.hpp>
#include <experimental/functional.hpp>
#include <experimental/sum_type.hpp>

#include <boost/detail/lightweight_test.hpp>

int main()
{
  namespace stde = std::experimental;
  using error_t = stde::unexpected_type<std::error_code>;
  auto error = stde::unexpected_type<std::error_code>(std::make_error_code(std::errc(1)));

  static_assert(stde::is_sum_type<stde::expected<int>>::value, "ERROR");
  static_assert(std::is_same<stde::sum_type::alternative_t<0,stde::expected<int>>, int>::value, "ERROR");
  static_assert(std::is_same<stde::sum_type::alternative_t<1,stde::expected<int>>, error_t>::value, "ERROR");
  static_assert(2 == stde::sum_type::size<stde::expected<int>>::value, "ERROR");

  std::error_code ec = std::make_error_code(std::errc(1));

  {
    stde::expected<int> o = stde::make_unexpected(ec);
    auto v = stde::overload<bool>([](error_t){ return true;}, [](int){ return false;});
    BOOST_TEST( true == v(error));
    BOOST_TEST( true == stde::sum_type::traits<stde::expected<int>>::visit(v, o));
    BOOST_TEST( true == stde::sum_type::match<bool>(o, [](error_t){ return true;}, [](int){ return false;}));
  }
  {
    stde::expected<int> o1= stde::make_unexpected(ec);
    stde::expected<int> o2= stde::make_unexpected(ec);

    auto v = stde::overload(
        [](error_t, error_t){ return true;},
        [](error_t, int const &){ return false;},
        [](int const &, int const &){ return false;},
        [](int const &, error_t){ return false;}
        );
    BOOST_TEST( true == v(error, error));
    BOOST_TEST( true == stde::product_type::apply(v, std::make_tuple(error, error)));
    BOOST_TEST( false == stde::product_type::apply(v, std::make_tuple(error, 1)));
    BOOST_TEST( false == stde::product_type::apply(v, std::make_tuple(1, error)));
    BOOST_TEST( false == stde::product_type::apply(v, std::make_tuple(1, 1)));

    auto t = std::make_tuple(o1, o2);
    BOOST_TEST( true == stde::sum_type::match<bool>(t,
        [](error_t, error_t){ return true;},
        [](error_t, int){ return false;},
        [](int, int){ return false;},
        [](int, error_t){ return false;}
      )
      );
  }


  return ::boost::report_errors();
}
#endif
