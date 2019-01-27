// Copyright (C) 2019 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Based on https://github.com/akrzemi1/explicit/blob/master/test/test_explicit.cpp

// <experimental/numerics/v1/charconv.hpp>

//#define JASEL_CONFIG_CONTRACT_LEVEL_MASK 0x111
#define JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V 1

#include <iostream>
#include <experimental/numerics/v1/charconv.hpp>
#include <string>
#include <array>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

int main()
{
#if __cplusplus > 201402L
    {
        std::array<char, 10> str{"42"};
        int result;
        auto [p, ec] = stdex::from_chars(str.data(), str.data()+2, result);
        //(void)p;
        BOOST_TEST_EQ(p, str.data()+2);
        BOOST_TEST(std::errc{} == ec);
        BOOST_TEST_EQ(42, result);
    }
#endif
    {
        std::array<char, 10> str{"42"};
        int result=0;
        auto p = stdex::from_chars_noerr(str.data(), str.data()+2, result);
        BOOST_TEST_EQ(p, str.data()+2);
        BOOST_TEST_EQ(42, result);
    }
#if __cplusplus > 201402L
    {
        std::array<char, 10> str{""};
        int result;
        auto [p, ec] = stdex::from_chars(str.data(), str.data()+0, result);
        BOOST_TEST_EQ(p, str.data());
        BOOST_TEST(std::errc::invalid_argument == ec);
    }
    {
        BOOST_TEST(! stdex::can_narrow_to<signed char>(300));
        std::array<char, 10> str{"300"};
        signed char result;
        auto [p, ec] = stdex::from_chars(str.data(), str.data()+10, result);
        BOOST_TEST_EQ(p, str.data());
        BOOST_TEST(std::errc::result_out_of_range == ec);
    }
#endif
    {
        std::array<char, 10> str{"300"};
        char* tmp;
        auto via = stdex::strto_via<unsigned int>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data()+3);
        BOOST_TEST_EQ(300 , via);
    }
    {
        std::array<char, 10> str{"300"};
        char* tmp;
        auto via = stdex::strto_via<unsigned short>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data()+3);
        BOOST_TEST_EQ(300 , via);
    }
    {
        std::array<char, 10> str{"200"};
        char* tmp;
        auto via = stdex::strto_via<unsigned char>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data()+3);
        BOOST_TEST_EQ(200 , via);
    }
    {
        std::array<char, 10> str{"300"};
        char* tmp;
        auto via = stdex::strto_via<unsigned char>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data());
        BOOST_TEST_EQ(0 , via);
    }
    {
        std::array<char, 10> str{"300"};
        char* tmp;
        auto via = stdex::strto_via<int>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data()+3);
        BOOST_TEST_EQ(300 , via);
    }
    {
        std::array<char, 10> str{"300"};
        char* tmp;
        auto via = stdex::strto<unsigned long>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data()+3);
        BOOST_TEST_EQ(300 , via);
        BOOST_TEST(! stdex::can_narrow_to<signed char>(via));
    }
    {
        std::array<char, 10> str{"300"};
        char* tmp;
        auto via = stdex::strto<long>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data()+3);
        BOOST_TEST_EQ(300 , via);
        BOOST_TEST(! stdex::can_narrow_to<signed char>(via));
    }
    {
        std::array<char, 10> str{"-30"};
        char* tmp;
        auto via = stdex::strto<long>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data()+3);
        BOOST_TEST_EQ(-30 , via);
        BOOST_TEST(stdex::can_narrow_to<signed char>(via));
    }
    {
        std::array<char, 10> str{"300"};
        char* tmp;
        auto via = stdex::strto<unsigned long long>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data()+3);
        BOOST_TEST_EQ(300 , via);
    }
    {
        std::array<char, 10> str{"-30"};
        char* tmp;
        auto via = stdex::strto<long long>(str.data(), &tmp, 10);
        BOOST_TEST_EQ(tmp , str.data()+3);
        BOOST_TEST_EQ(-30 , via);
    }
#if __cplusplus > 201402L
    {
        BOOST_TEST(! stdex::can_narrow_to<unsigned char>(300));
        std::array<char, 10> str{"300"};
        unsigned char result;
        auto [p, ec] = stdex::from_chars(str.data(), str.data()+10, result);
        BOOST_TEST_EQ(p, str.data());
        BOOST_TEST(std::errc::result_out_of_range == ec);
    }
    {
        errno = 0;
        std::array<char, 10> str{"AB"};
        int result;
        auto [p, ec] = stdex::from_chars(str.data(), str.data()+10, result);
        BOOST_TEST_EQ(p, str.data());
        BOOST_TEST(std::errc::invalid_argument == ec);
    }
#endif
    {
        std::array<char, 100> arr;
        (void) stdex::to_chars(arr.data(), arr.data()+100, 10);
        BOOST_TEST_EQ(std::strcmp("10", arr.data()), 0);
    }
#if __cplusplus > 201402L
    {
        std::array<char, 10> str;
        int value = 44;
        auto [p, ec] = stdex::to_chars(str.data(), str.data()+2, value);
        BOOST_TEST(std::errc{} == ec);
        BOOST_TEST(p == str.data()+2);
        BOOST_TEST_EQ(std::strcmp("44", str.data()), 0);
    }
    {
        std::array<char, 10> str;
        int value = -43;
        auto [p, ec] = stdex::to_chars(str.data(), str.data()+9, value);
        BOOST_TEST(std::errc{} == ec);
        BOOST_TEST_EQ(p, str.data()+3);
        BOOST_TEST_EQ(std::strcmp("-43", str.data()), 0);
    }
#endif
    {
        std::array<char, 10> str;
        int value = 45;
        auto p = stdex::to_chars_noerr(str.data(), str.data()+9, value);
        BOOST_TEST(p == str.data()+2);
        BOOST_TEST_EQ(std::strcmp("45", str.data()), 0);
    }
    return boost::report_errors();
}
