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
	// default constructor implies empty string
	{
		stdex::static_cstring<20> sv{};
		BOOST_TEST(sv.empty());
		BOOST_TEST(sv.size() == 0);
		BOOST_TEST(sv.c_str() == std::string());
	}
	std::cout << __LINE__ << std::endl;
	// constructor from a NTXS of length > 0
	{
		const char *str = "Hello";
		std::size_t len;
		bool        b = stdex::ntxs::test_and_set_length<20>(str, len);
		BOOST_TEST(b);
		BOOST_TEST_EQ(5, len);
	}
	{
		const char *              str = "Hello";
		std::size_t               len = 5;
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str, len);
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(5, sv.size());
	}
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv{stdex::null_terminated_t{}, str};
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(5, sv.size());
		BOOST_TEST_EQ(stdex::string_view(sv), std::string(str));
		BOOST_TEST_EQ(sv.to_string(), std::string(str));
		BOOST_TEST_EQ(std::string(sv), std::string(str));
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
	// string_view doesn't throws when not valid as it is required to be UB
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
	// construction from std::string
	{
		std::string               str{"Hello"};
		stdex::static_cstring<20> sv(str);
		BOOST_TEST(!sv.empty());
		BOOST_TEST(sv.c_str() == str);
	}
	std::cout << __LINE__ << std::endl;
	// construction from N char
	{
		stdex::static_cstring<20> sv(stdex::length_tag_t{}, 3, 'X');
		BOOST_TEST(!sv.empty());
		BOOST_TEST(sv.to_string() == "XXX");
	}
	std::cout << __LINE__ << std::endl;
// implicit construction from N char  compile fails
#if 0
	{
		stdex::static_cstring<20> sv(48, 'X');
	}
#endif
	std::cout << __LINE__ << std::endl;
	// implicit construction from a initializer list
	// construction from N char
	{
		stdex::static_cstring<20> sv{48, 'X'};
		BOOST_TEST(!sv.empty());
		BOOST_TEST(sv.to_string() == "0X");
	}
	std::cout << __LINE__ << std::endl;
	{
		stdex::static_cstring<20> sv = {48, 'X'};
		BOOST_TEST(!sv.empty());
		BOOST_TEST(sv.to_string() == "0X");
	}
	std::cout << __LINE__ << std::endl;
	// iteration over the chars using range-based for loop
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		std::ostringstream        os;
		for (auto ch : sv)
			os << ch;
		BOOST_TEST(os.str() == str);
	}
	// iteration over the chars using range-based for loop
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		for (auto &ch : sv)
			ch = 'a';
		BOOST_TEST(sv.to_string() == "aaaaa");
	}
	// operator=

	// assign
	std::cout << __LINE__ << std::endl;
	// Assign n chars
	{
		stdex::static_cstring<20> sv;
		sv.assign(3, 'X');
		BOOST_TEST(sv.to_string() == "XXX");
	}
	std::cout << __LINE__ << std::endl;
	// Assign another string
	{
		stdex::static_cstring<20> sw{stdex::null_terminated_t{}, "Hello"};
		stdex::static_cstring<20> sv;
		sv.assign(sw);
		BOOST_TEST(sv.to_string() == "Hello");
	}
	std::cout << __LINE__ << std::endl;
	// Assign another substring
	{
		stdex::static_cstring<20> sw{stdex::null_terminated_t{}, "Hello"};
		stdex::static_cstring<20> sv;
		sv.assign(sw, 1, 3);
		BOOST_TEST(sv.to_string() == "ell");
	}
	std::cout << __LINE__ << std::endl;
	// Assign another substring over
	{
		stdex::static_cstring<20> sw{stdex::null_terminated_t{}, "Hello"};
		stdex::static_cstring<20> sv;
		sv.assign(sw, 1, 5);
		std::cout << sv.c_str() << " " << sv.size() << std::endl;
		BOOST_TEST(sv.to_string() == "ello");
	}
	std::cout << __LINE__ << std::endl;
	// Assign another substring npos
	{
		stdex::static_cstring<20> sw{stdex::null_terminated_t{}, "Hello"};
		stdex::static_cstring<20> sv;
		sv.assign(sw, 1, stdex::static_cstring<20>::npos);
		std::cout << sv.c_str() << " " << sv.size() << std::endl;
		BOOST_TEST(sv.to_string() == "ello");
	}
	std::cout << __LINE__ << std::endl;
	// Assign another empty substring
	{
		stdex::static_cstring<20> sw{stdex::null_terminated_t{}, "Hello"};
		stdex::static_cstring<20> sv;
		sv.assign(sw, 1, 0);
		std::cout << sv.c_str() << " " << sv.size() << std::endl;
		BOOST_TEST(sv.to_string() == "");
	}
	std::cout << __LINE__ << std::endl;
	// Assign a literal string
	{
		stdex::static_cstring<20> sv;
		sv.assign("Hello");
		BOOST_TEST(sv.to_string() == "Hello");
	}
	std::cout << __LINE__ << std::endl;
	// Assign a const char *
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv;
		sv.assign(str);
		BOOST_TEST(sv.to_string() == "Hello");
	}
	std::cout << __LINE__ << std::endl;
	// Assign a prefix of a const char *
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv;
		sv.assign(str, 3);
		BOOST_TEST(sv.to_string() == "Hel");
	}
	std::cout << __LINE__ << std::endl;
	// Assign a range
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv;
		sv.assign(str + 1, str + 3);
		BOOST_TEST(sv.to_string() == "el");
	}
	std::cout << __LINE__ << std::endl;
	// Assign a range
	{
		stdex::static_cstring<20> sv;
		sv.assign({'e', 'l'});
		BOOST_TEST(sv.to_string() == "el");
	}

	// range based for loop
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
	std::cout << __LINE__ << std::endl;
	// iteration over the chars using index and size
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		for (std::size_t i = 0, l = sv.size(); i < l; ++i)
			sv[i] = 'b';
		BOOST_TEST(sv.to_string() == "bbbbb");
	}
	std::cout << __LINE__ << std::endl;
	// at good index read access
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.at(0) == 'H');
	}
	std::cout << __LINE__ << std::endl;
	// at good index write access
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.at(0) = 'h';
		BOOST_TEST(sv.to_string() == "hello");
	}
	// front/back good index access
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.front() == 'H');
		BOOST_TEST(sv.back() == 'o');
	}
	// front/back good index access
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.front() = 'h';
		BOOST_TEST(sv.to_string() == "hello");
	}
	// front/back good index access
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		sv.back() = 'a';
		BOOST_TEST(sv.to_string() == "Hella");
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
	// implicit conversion to string_view
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> scstr(stdex::null_terminated_t{}, str);
		stdex::string_view        sv{scstr};
		BOOST_TEST(sv.data() == scstr.data());
	}
	// implicit conversion to cstring_view
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> scstr(stdex::null_terminated_t{}, str);
		stdex::cstring_view       sv{scstr};
		BOOST_TEST(sv.data() == scstr.data());
	}
// implicit conversion to cstr_view
#if __cplusplus > 201402L
	// fixme: why this is ambiguous and not the following
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> scstr(stdex::null_terminated_t{}, str);
		stdex::cstr_view          sv(scstr);
		BOOST_TEST(sv.data() == scstr.data());
	}
#endif
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> scstr(stdex::null_terminated_t{}, str);
		stdex::cstr_view          sv = scstr;
		BOOST_TEST(sv.data() == scstr.data());
	}
	// to_string
	{
		const char *              str = "Hello";
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, str);
		BOOST_TEST(sv.to_string() == str);
	}
	// explicit conversion to string
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
	// ambiguous interface when using 0, which is convertible to size_t and char const* :(
	//         basic_static_cstring &insert(size_type 	 index, size_type count, charT ch)
	//         iterator 			 insert(const_iterator pos, size_type count, charT ch)
	{
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "0123456");
		sv.insert(0ul, 3, 'x');
		BOOST_TEST(sv.to_string() == "xxx0123456");
	}
	{
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "0123456");
		sv.insert(3, 2, 'x');
		BOOST_TEST(sv.to_string() == "012xx3456");
	}
	{
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "01234");
		sv.insert(5, 2, 'x');
		BOOST_TEST(sv.to_string() == "01234xx");
	}
	try
	{

		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "01234");
		sv.insert(6, 2, 'x');
		BOOST_TEST(false);
	}
	catch (stdex::contract_failed &ex)
	{
		stdex::string_view msg = ex.what();
		BOOST_TEST_EQ(msg.substr(0, 28), "JASEL: Pre-condition failure");
	}
	{
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "0123456");
		sv.insert(0ul, "xxx");
		BOOST_TEST(sv.to_string() == "xxx0123456");
	}
	{
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "0123456");
		sv.insert(2, "xxx");
		BOOST_TEST(sv.to_string() == "01xxx23456");
	}
	try
	{
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "012");
		sv.insert(6, "xxx");
		BOOST_TEST(false);
	}
	catch (stdex::contract_failed &ex)
	{
		stdex::string_view msg = ex.what();
		BOOST_TEST_EQ(msg.substr(0, 28), "JASEL: Pre-condition failure");
	}
	{
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "0123456");
		auto                      it = sv.insert(sv.data(), 3, 'x');
		BOOST_TEST(it == sv.data());
		BOOST_TEST(sv.to_string() == "xxx0123456");
	}
	{
		stdex::static_cstring<20> letters(stdex::null_terminated_t{}, "abcdef");
		stdex::static_cstring<20> sv(stdex::null_terminated_t{}, "0123456");
		sv.insert(sv.data() + 2, letters.data() + 2, letters.data() + 4);
		BOOST_TEST(sv.to_string() == "01cd23456");
	}

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
		BOOST_TEST(sv[3] == '\0');
		BOOST_TEST(sv[4] == '\0');
		BOOST_TEST(sv[5] == '\0');
		BOOST_TEST(sv.c_str() == std::string("XXX"));
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
	{
		auto str = stdex::to_static_cstring<10>(100);
		BOOST_TEST(str.to_string() == std::string("100"));
	}
	{
		stdex::static_cstring<20> str(stdex::null_terminated_t{}, "100");
		int                       value;
		auto                      result = stdex::from_chars(str.c_str(), str.c_str() + str.size(), value, 10);
		BOOST_TEST(result.ec == std::errc{});
		auto i = stdex::sto<int>(str);
		BOOST_TEST_EQ(i, 100);
		//BOOST_TEST_EQ(stdex::sto<int>(str), 100);
		//BOOST_TEST_EQ(stdex::stoi(str), 100);
	}
	return ::boost::report_errors();
}

#else
int main()
{
	return ::boost::report_errors();
}
#endif
