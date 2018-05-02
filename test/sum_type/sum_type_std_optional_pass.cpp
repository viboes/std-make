// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/sum_type.hpp>
// <experimental/optional.hpp>

#if __cplusplus < 201402L
int main()
{
}
#else
#include <experimental/optional.hpp>
#include <experimental/functional.hpp>
#include <experimental/sum_type.hpp>

#include <boost/detail/lightweight_test.hpp>

int main()
{
  namespace stde = std::experimental;

  static_assert(stde::is_sum_type<stde::optional<int>>::value, "ERROR");
  static_assert(std::is_same<stde::sum_type::alternative_t<0,stde::optional<int>>, stde::nullopt_t>::value, "ERROR");
  static_assert(std::is_same<stde::sum_type::alternative_t<1,stde::optional<int>>, int>::value, "ERROR");
  static_assert(2 == stde::sum_type::size<stde::optional<int>>::value, "ERROR");

  {
    stde::optional<int> o;
    auto v = stde::overload<bool>([](stde::nullopt_t){ return true;}, [](int){ return false;});
    BOOST_TEST( true == v(stde::nullopt));
    BOOST_TEST( true == stde::sum_type::traits<stde::optional<int>>::visit(v, o));
    BOOST_TEST( true == stde::sum_type::match<bool>(o, [](stde::nullopt_t){ return true;}, [](int){ return false;}));
    BOOST_TEST( true == stde::sum_type::inspect(o)(stde::overload<bool>(
                    [](stde::nullopt_t){ return true;},
                    [](int){ return false;}
                    ))
                    );
  }
  {
    stde::optional<int> o1, o2;

    auto v = stde::overload(
        [](stde::nullopt_t, stde::nullopt_t){ return true;},
        [](stde::nullopt_t, int const &){ return false;},
        [](int const &, int const &){ return false;},
        [](int const &, stde::nullopt_t){ return false;}
        );
    BOOST_TEST( true == v(stde::nullopt, stde::nullopt));
    BOOST_TEST( true == stde::product_type::apply(v, std::make_tuple(stde::nullopt, stde::nullopt)));
    BOOST_TEST( false == stde::product_type::apply(v, std::make_tuple(stde::nullopt, 1)));
    BOOST_TEST( false == stde::product_type::apply(v, std::make_tuple(1, stde::nullopt)));
    BOOST_TEST( false == stde::product_type::apply(v, std::make_tuple(1, 1)));

    auto t = std::make_tuple(o1, o2);
    BOOST_TEST( true == stde::sum_type::match<bool>(t,
        [](stde::nullopt_t, stde::nullopt_t){ return true;},
        [](stde::nullopt_t, int){ return false;},
        [](int, int){ return false;},
        [](int, stde::nullopt_t){ return false;}
      )
      );
    BOOST_TEST( true == stde::sum_type::inspect(o1, o2)(stde::overload<bool>(
        [](stde::nullopt_t, stde::nullopt_t){ return true;},
        [](stde::nullopt_t, int){ return false;},
        [](int, int){ return false;},
        [](int, stde::nullopt_t){ return false;}
      ))
      );
  }


  return ::boost::report_errors();
}
#endif
