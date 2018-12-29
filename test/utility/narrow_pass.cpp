// Copyright (C) 2018 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Based on https://github.com/akrzemi1/explicit/blob/master/test/test_explicit.cpp

// <experimental/.../utility/narrow.hpp>

#define JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V 1

#include <experimental/utility.hpp>
#include <memory>
#include <limits>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;


void test_narrow_throws_when_narrowing_with_value_loss()
{
    try {
        auto x = stdex::narrow<char>(300);
        (void)x;
        BOOST_TEST (false);
    } catch (stdex::narrowing_error&) {
    } catch (...) {
        BOOST_TEST (false);
    }
    try {
        auto x = stdex::narrow<signed char>(-30-128);
        (void)x;
        BOOST_TEST (false);
    } catch (stdex::narrowing_error&) {
    } catch (...) {
        BOOST_TEST (false);
    }
    try {
        signed char d = -128;
        auto x = stdex::narrow<signed char>(-d);
        (void)x;
        BOOST_TEST (false);
    } catch (stdex::narrowing_error&) {
    } catch (...) {
        BOOST_TEST (false);
    }
    try {
        auto x = stdex::narrow<unsigned char>(-128);
        (void)x;
        BOOST_TEST (false);
    } catch (stdex::narrowing_error&) {
    } catch (...) {
        BOOST_TEST (false);
    }

    const auto int32_min = std::numeric_limits<std::int32_t>::min();

    try {
        auto x = stdex::narrow<uint32_t>(int32_min);
        (void)x;
        BOOST_TEST (false);
    } catch (stdex::narrowing_error&) {
    } catch (...) {
        BOOST_TEST (false);
    }

    const auto uint32_max = std::numeric_limits<std::uint32_t>::max();
    try {
        auto x = stdex::narrow<int32_t>(uint32_max);
        (void)x;
        BOOST_TEST (false);
    } catch (stdex::narrowing_error&) {
    } catch (...) {
        BOOST_TEST (false);
    }

}

void test_narrow_allows_narrowing_without_value_loss()
{

    BOOST_TEST(stdex::narrow<std::uint8_t>(int32_t(0)) == 0);
    BOOST_TEST(stdex::narrow<std::uint16_t>(int32_t(30)) == 30);

    const auto int32_max = std::numeric_limits<std::int32_t>::max();

    BOOST_TEST(stdex::narrow<std::uint32_t>(int32_t(0)) == 0);
    BOOST_TEST(stdex::narrow<std::uint32_t>(int32_t(1)) == 1);
    BOOST_TEST(stdex::narrow<std::uint32_t>(int32_max) == static_cast<std::uint32_t>(int32_max));

    const auto uint32_min = std::numeric_limits<std::uint32_t>::min();
    BOOST_TEST(stdex::narrow<std::int32_t>(uint32_min) == static_cast<std::int32_t>(uint32_min));
}

int main()
{
    test_narrow_throws_when_narrowing_with_value_loss();
    test_narrow_allows_narrowing_without_value_loss();

  return ::boost::report_errors();
}

