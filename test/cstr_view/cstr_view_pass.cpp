// Copyright (C) 2019 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/status_value.hpp>

#define JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V 1
#include <experimental/cstr_view.hpp>

#include <sstream>

#include <boost/detail/lightweight_test.hpp>

#if __cplusplus >= 201103L
namespace stdex = std::experimental;

int main()
{
	{
		stdex::cstr_view sv;
		BOOST_TEST(sv.empty());
	}
	{
		const char *     str = "Hello";
		stdex::cstr_view sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(!sv.empty());
	}
#if __cplusplus > 201402L
	{
		const char *       str = "Hello";
		stdex::cstr_view   sv(stdex::null_terminated_t{}, str);
		std::ostringstream os;
		for (auto ch : sv)
			os << ch;
		BOOST_TEST(os.str() == str);
	}
#endif
	{
		const char *       str = "Hello";
		stdex::cstr_view   sv(stdex::null_terminated_t{}, str);
		std::ostringstream os;
		for (std::size_t i = 0, l = sv.size(); i < l; ++i)
			os << sv[i];
		BOOST_TEST(os.str() == str);
	}
#if __cplusplus > 201402L
	{
		const char *     str = "Hello";
		stdex::cstr_view sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.at(0) == 'H');
	}
#endif
	{
		const char *     str = "Hello";
		stdex::cstr_view sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.front() == 'H');
#if __cplusplus > 201402L
		BOOST_TEST(sv.back() == 'o');
#endif
	}
	{
		const char *     str = "Hello";
		stdex::cstr_view sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.data() == str);
	}
	{
		const char *     str = "Hello";
		stdex::cstr_view sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.c_str() == str);
	}
	{
		const char *     str = "Hello";
		stdex::cstr_view sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.to_string() == str);
	}
	// constructor from const char *
	{
		const char *     str = "Hello";
		stdex::cstr_view sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(std::string(sv) == str);
	}
	// constructor from std::string
	{
		std::string      str = "Hello";
		stdex::cstr_view sv  = str;
		BOOST_TEST(sv.to_string() == str);
	}
	// remove_prefix
	{
		const char *     str = "Hello World";
		stdex::cstr_view sv(stdex::null_terminated_t{}, str);
		sv.remove_prefix(6);
		BOOST_TEST(sv.to_string() == "World");
	}
	// swap
	{
		const char *     hello = "Hello";
		const char *     world = "World";
		stdex::cstr_view sv(stdex::null_terminated_t{}, hello);
		stdex::cstr_view sw(stdex::null_terminated_t{}, world);
		swap(sv, sw);
		BOOST_TEST(sv.to_string() == "World");
	}
	// compare
	{
		const char *     hello = "Hello";
		const char *     world = "World";
		stdex::cstr_view sv(stdex::null_terminated_t{}, hello);
		stdex::cstr_view sw(stdex::null_terminated_t{}, world);
		BOOST_TEST(sv.compare(sw) < 0);
		BOOST_TEST(sw.compare(sv) > 0);
	}
	{
		const char *     hello = "Hello";
		stdex::cstr_view sv(stdex::null_terminated_t{}, hello);
		stdex::cstr_view sw(stdex::null_terminated_t{}, hello);
		BOOST_TEST(sv.compare(sw) == 0);
	}
	{
		const char *     hello = "Hello";
		stdex::cstr_view sv(stdex::null_terminated_t{}, hello);
		BOOST_TEST(sv.compare("Hello") == 0);
	}

	return ::boost::report_errors();
}

#else
int main()
{
	return ::boost::report_errors();
}
#endif
