// Copyright (C) 2019 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/status_value.hpp>

#define JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V 1
#include <experimental/static_cstring.hpp>

#include <iostream>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

#if __cplusplus >= 201103L
namespace stdex = std::experimental;

int main()
{
	std::cout << __LINE__ << std::endl;
	// default constructor implies empty string
	{
		stdex::static_cstring<20> sv;
		BOOST_TEST(sv.empty());
		BOOST_TEST(sv.size() == 0);
		BOOST_TEST(sv.c_str() == std::string());
	}
	std::cout << __LINE__ << std::endl;
	// constructor from a NTXS of length > 0
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(!sv.empty());
		BOOST_TEST(sv.c_str() == std::string(str));
	}
	std::cout << __LINE__ << std::endl;
#if 0
	// constructor from nullptr_t must compile fail
	{
		
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, nullptr);
	}
#endif
	std::cout << __LINE__ << std::endl;
	// constructor from a const char* equal to nullptr is UB, don't do it
	if (0)
	{
		try
		{
			const char *              str = nullptr;
			stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
			BOOST_TEST(false);
		}
		catch (...)
		{
		}
	}
	std::cout << __LINE__ << std::endl;
	// construction from c-string literal
	{
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "Hello");
		BOOST_TEST(!sv.empty());
	}
	std::cout << __LINE__ << std::endl;
	// construction from empty c-string literal
	{
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "");
		BOOST_TEST(sv.to_string().empty());
		BOOST_TEST(sv.empty());
		BOOST_TEST(sv.c_str() == std::string());
	}
	std::cout << __LINE__ << std::endl;
	// construction from std::string
	{
		std::string               str("Hello");
		stdex::static_cstring<20> sv(str);
		BOOST_TEST(!sv.empty());
		BOOST_TEST(sv.c_str() == str);
	}
	std::cout << __LINE__ << std::endl;
#if __cplusplus > 201402L
	// iteration over the chars using range-based for loop
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		std::ostringstream        os;
		for (auto ch : sv)
			os << ch;
		BOOST_TEST(os.str() == str);
	}
#endif
	std::cout << __LINE__ << std::endl;
	// iteration over the chars using index and size
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		std::ostringstream        os;
		for (std::size_t i = 0, l = sv.size(); i < l; ++i)
			os << sv[i];
		BOOST_TEST(os.str() == str);
	}
#if __cplusplus > 201402L
	std::cout << __LINE__ << std::endl;
	// at good index access
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.at(0) == 'H');
	}
#endif
	// front/back good index access
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.front() == 'H');
#if __cplusplus > 201402L
		BOOST_TEST(sv.back() == 'o');
#endif
	}
	// data access
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.data() == std::string(str));
	}
	// c_str access
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.c_str() == std::string(str));
	}
	// to_string explicit conversion
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.to_string() == str);
	}
	// string explicit conversion
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(std::string(sv) == str);
	}
	// clear()
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.clear();
		BOOST_TEST(sv.to_string() == "");
	}
	// insert
	// erase
	// push_back
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.push_back(' ');
		BOOST_TEST(sv.to_string() == "Hello ");
	}
	// pop_back
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.pop_back();
		BOOST_TEST(sv.to_string() == "Hell");
	}
	// replace
	// resize
	{
		const char *              str = "XXX";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.resize(5, ' ');
		BOOST_TEST(sv.to_string() == "XXX  ");
	}
	// resize
	{
		const char *              str = "XXX";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.resize(2, ' ');
		BOOST_TEST(sv.to_string() == "XX");
	}
	// remove_prefix
	// resize
	{
		const char *              str = "XXX";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.resize(5);
		BOOST_TEST(sv.size() == 5);
		BOOST_TEST(sv.to_string().size() == 5);
		std::cout << "#" << sv.to_string() << "#" << std::endl;
		// fixme: this doesn't work yet
		//BOOST_TEST(sv.to_string() == "XXX");
	}
	// resize
	{
		const char *              str = "XXX";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.resize(2);
		BOOST_TEST(sv.to_string() == "XX");
		BOOST_TEST(sv.size() == 2);
	}
	{
		const char *              str = "Hello World";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.remove_prefix(6);
		BOOST_TEST(sv.to_string() == "World");
	}
	// remove_suffix
	{
		const char *              str = "Hello World";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.remove_suffix(6);
		BOOST_TEST(sv.to_string() == "Hello");
	}
	// swap
	{
		const char *              hello = "Hello";
		const char *              world = "World";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, hello);
		stdex::static_cstring<20> sw(stdex::null_terminated_t{}, world);
		swap(sv, sw);
		BOOST_TEST(sv.to_string() == "World");
	}
#if __cplusplus > 201402L
	// compare different
	{
		const char *              hello = "Hello";
		const char *              world = "World";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, hello);
		stdex::static_cstring<20> sw(stdex::null_terminated_t{}, world);
		BOOST_TEST(sv.compare(sw) < 0);
		BOOST_TEST(sw.compare(sv) > 0);
	}
	// compare equals
	{
		const char *              hello = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, hello);
		stdex::static_cstring<20> sw(stdex::null_terminated_t{}, hello);
		BOOST_TEST(sv.compare(sw) == 0);
	}
	// compare equals with const char *
	{
		const char *              hello = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, hello);
		BOOST_TEST(sv.compare("Hello") == 0);
	}
	// test for all the string_view specific functions go here
#endif

	return ::boost::report_errors();
}

#else
int main()
{
	return ::boost::report_errors();
}
#endif
