// Copyright (C) 2019 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/status_value.hpp>

#define JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V 1
#include <experimental/string_view.hpp>

#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

int main()
{
	{
		stdex::string_view sv;
		BOOST_TEST(sv.empty());
	}
	{
		const char *       str = "Hello";
		stdex::string_view sv(str);
		BOOST_TEST(!sv.empty());
	}
	{
		const char *       str = "Hello";
		stdex::string_view sv(str);
		std::ostringstream os;
		for (auto ch : sv)
			os << ch;
		BOOST_TEST(os.str() == str);
	}
	{
		const char *       str = "Hello";
		stdex::string_view sv(str);
		std::ostringstream os;
		for (std::size_t i = 0, l = sv.size(); i < l; ++i)
			os << sv[i];
		BOOST_TEST(os.str() == str);
	}
	{
		const char *       str = "Hello";
		stdex::string_view sv(str);
		BOOST_TEST(sv.at(0) == 'H');
	}
	{
		const char *       str = "Hello";
		stdex::string_view sv(str);
		BOOST_TEST(sv.front() == 'H');
		BOOST_TEST(sv.back() == 'o');
	}
	// constructor from const char *
	{
		const char *       str = "Hello";
		stdex::string_view sv(str);
		BOOST_TEST(sv.data() == str);
	}
	// constructor from std::string
	{
		std::string        str = "Hello";
		stdex::string_view sv  = str;
		BOOST_TEST(sv.data() == str.data());
	}
	// remove_prefix
	{
		const char *       str = "Hello World";
		stdex::string_view sv(str);
		sv.remove_prefix(6);
		BOOST_TEST(std::string(sv) == "World");
	}
	// swap
	{
		const char *       hello = "Hello";
		const char *       world = "World";
		stdex::string_view sv(hello);
		stdex::string_view sw(world);
		swap(sv, sw);
		BOOST_TEST(std::string(sv) == "World");
	}
	// compare
	{
		const char *       hello = "Hello";
		const char *       world = "World";
		stdex::string_view sv(hello);
		stdex::string_view sw(world);
		BOOST_TEST(sv.compare(sw) < 0);
		BOOST_TEST(sw.compare(sv) > 0);
	}
	{
		const char *       hello = "Hello";
		stdex::string_view sv(hello);
		stdex::string_view sw(hello);
		BOOST_TEST(sv.compare(sw) == 0);
	}
	{
		stdex::string_view sv("Hello");
		BOOST_TEST(sv.compare("Hello") == 0);
	}
	{
		stdex::string_view sv("Hello");
		BOOST_TEST(sv.find('H') == 0);
	}
	{
		stdex::string_view sv("Hello");
		BOOST_TEST(sv.find('l') == 2);
	}
	{
		stdex::string_view sv("Hello");
		BOOST_TEST(sv.find('w') == stdex::string_view::npos);
	}
	{
		const char *       hello = "Hello";
		stdex::string_view sv(hello);
		BOOST_TEST(sv.find('l', 3) == 3);
	}
	{
		stdex::string_view sv("Hello");
		BOOST_TEST(sv.find("el") == 1);
	}
	return ::boost::report_errors();
}
