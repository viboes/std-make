// Copyright (C) 2018 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Based on https://github.com/akrzemi1/explicit/blob/master/test/test_explicit.cpp

// <experimental/.../utility/narrow.hpp>

#define JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V 1

#include <experimental/utility.hpp>
#include <limits>
#include <memory>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

#if __cplusplus >= 201402L
static_assert(stdex::narrow_to<short>(1) == std::make_pair(true, short(1)), "error");
static_assert(stdex::narrow_to<short>(100000) == std::make_pair(false, short(0)), "error");
static_assert(stdex::can_narrow_to<short>(100000) == false, "error");
#endif

void test_narrow_throws_when_narrowing_with_value_loss()
{
	BOOST_TEST(!stdex::can_narrow_to<char>(300));
	try
	{
		auto x = stdex::narrow<char>(300);
		(void)x;
		BOOST_TEST(false);
	}
	catch (stdex::narrowing_error &)
	{
	}
	catch (...)
	{
		BOOST_TEST(false);
	}

	BOOST_TEST(!stdex::can_narrow_to<char>(-30 - 128));
	try
	{
		auto x = stdex::narrow<signed char>(-30 - 128);
		(void)x;
		BOOST_TEST(false);
	}
	catch (stdex::narrowing_error &)
	{
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		signed char d = -128;
		BOOST_TEST(!stdex::can_narrow_to<signed char>(-d));
		auto x = stdex::narrow<signed char>(-d);
		(void)x;
		BOOST_TEST(false);
	}
	catch (stdex::narrowing_error &)
	{
	}
	catch (...)
	{
		BOOST_TEST(false);
	}

	BOOST_TEST(!stdex::can_narrow_to<unsigned char>(-128));
	try
	{
		auto x = stdex::narrow<unsigned char>(-128);
		(void)x;
		BOOST_TEST(false);
	}
	catch (stdex::narrowing_error &)
	{
	}
	catch (...)
	{
		BOOST_TEST(false);
	}

	const auto int32_min = std::numeric_limits<std::int32_t>::min();
	BOOST_TEST(!stdex::can_narrow_to<uint32_t>(int32_min));
	try
	{
		auto x = stdex::narrow<uint32_t>(int32_min);
		(void)x;
		BOOST_TEST(false);
	}
	catch (stdex::narrowing_error &)
	{
	}
	catch (...)
	{
		BOOST_TEST(false);
	}

	const auto uint32_max = std::numeric_limits<std::uint32_t>::max();
	BOOST_TEST(!stdex::can_narrow_to<int32_t>(uint32_max));
	try
	{
		auto x = stdex::narrow<int32_t>(uint32_max);
		(void)x;
		BOOST_TEST(false);
	}
	catch (stdex::narrowing_error &)
	{
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
}

void test_narrow_allows_narrowing_without_value_loss()
{

	{
		using T = int;
		using U = int;
		U u     = 1;
		T t     = static_cast<T>(u);
		BOOST_TEST(static_cast<U>(t) == u);
		BOOST_TEST((stdex::detail::have_same_sign<T, U>::value));
		BOOST_TEST((t < T{}) == (u < U{}));
	}
	BOOST_TEST(stdex::can_narrow_to<int>(1));
	BOOST_TEST(stdex::can_narrow_to<unsigned int>(1));
	{
		std::uint8_t t = static_cast<std::uint8_t>(std::int32_t(0));
		BOOST_TEST_EQ(static_cast<std::int32_t>(t), std::int32_t(0));

		// (!detail::have_same_sign<T, U>::value && ((t < T{}) != (u < U{})))

		BOOST_TEST((!stdex::detail::have_same_sign<std::uint8_t, std::int32_t>::value));
		BOOST_TEST((t < std::uint8_t{}) == (std::int32_t(0) < std::int32_t{}));
	}
	{
		using T = std::uint8_t;
		using U = std::int32_t;
		U u     = 1;
		T t     = static_cast<T>(u);
		BOOST_TEST(static_cast<U>(t) == u);
		BOOST_TEST(!(stdex::detail::have_same_sign<T, U>::value));
		BOOST_TEST((t < T{}) == (u < U{}));
	}
	BOOST_TEST(stdex::can_narrow_to<std::uint8_t>(int32_t(0)));
	BOOST_TEST(stdex::narrow<std::uint8_t>(int32_t(0)) == 0);
	BOOST_TEST(stdex::can_narrow_to<std::uint16_t>(int32_t(30)));
	BOOST_TEST(stdex::narrow<std::uint16_t>(int32_t(30)) == 30);

	BOOST_TEST(stdex::narrow<std::uint32_t>(int32_t(0)) == 0);
	BOOST_TEST(stdex::can_narrow_to<std::uint32_t>(int32_t(0)));
	BOOST_TEST(stdex::narrow<std::uint32_t>(int32_t(1)) == 1);

	const auto int32_max = std::numeric_limits<std::int32_t>::max();
	BOOST_TEST(stdex::narrow<std::uint32_t>(int32_max) == static_cast<std::uint32_t>(int32_max));
	BOOST_TEST(stdex::can_narrow_to<std::uint32_t>(int32_max));

	const auto uint32_min = std::numeric_limits<std::uint32_t>::min();
	BOOST_TEST(stdex::narrow<std::int32_t>(uint32_min) == static_cast<std::int32_t>(uint32_min));

#if __cplusplus > 201103L
	static_assert(stdex::narrow<std::uint16_t>(int32_t(30)) == 30, "");
#endif
}

int main()
{
	test_narrow_throws_when_narrowing_with_value_loss();
	test_narrow_allows_narrowing_without_value_loss();

	return ::boost::report_errors();
}
