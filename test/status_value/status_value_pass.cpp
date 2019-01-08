// Copyright (C) 2019 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/status_value.hpp>

#include <experimental/status_value.hpp>

#include <boost/detail/lightweight_test.hpp>

#if __cplusplus >= 201402L
namespace stdex = std::experimental;

struct X {
    int value;
};

int main()
{
    {
        stdex::status_value<int, int> sv(1);
        BOOST_TEST(sv.has_value() == false);
        BOOST_TEST(bool(sv) == false);
        BOOST_TEST(sv.status() == 1);
    }
    {
        stdex::status_value<int, int> sv(1, 0);
        BOOST_TEST(sv.has_value() == true);
        BOOST_TEST(bool(sv) == true);
        BOOST_TEST(sv.status() == 1);
        BOOST_TEST(sv.value() == 0);
        BOOST_TEST(*sv == 0);
    }
    {
        stdex::status_value<int, X> sv(1, X{0});
        BOOST_TEST(sv.has_value() == true);
        BOOST_TEST(bool(sv) == true);
        BOOST_TEST(sv.status() == 1);
        BOOST_TEST(sv.value().value == 0);
        BOOST_TEST(sv->value == 0);
    }
    return ::boost::report_errors();
}

#else
int main()
{
    return ::boost::report_errors();
}
#endif
