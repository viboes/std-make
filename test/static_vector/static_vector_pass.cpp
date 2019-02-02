// Copyright (C) 2019 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/status_value.hpp>

#define JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V 1
#include <experimental/static_vector.hpp>

#include <boost/detail/lightweight_test.hpp>

#if __cplusplus >= 201103L
namespace stdex = std::experimental;

int main()
{
	try
	{
		stdex::static_vector<int, 10> const sv;
		BOOST_TEST(sv.empty());
		BOOST_TEST(sv.size() == 0);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sw;
		stdex::static_vector<int, 10> sv(sw);
		BOOST_TEST(sv.empty());
		BOOST_TEST(sv.size() == 0);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sw;
		stdex::static_vector<int, 10> sv(std::move(sw));
		BOOST_TEST(sv.empty());
		BOOST_TEST(sv.size() == 0);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sv = {42};
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 1);
		BOOST_TEST_EQ(sv[0], 42);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sw = {42};
		stdex::static_vector<int, 10> sv{sw};
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 1);
		BOOST_TEST_EQ(sv[0], 42);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sw = {42};
		stdex::static_vector<int, 10> sv{std::move(sw)};
		BOOST_TEST(!sw.empty());
		BOOST_TEST_EQ(sw.size(), 1);
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 1);
		BOOST_TEST_EQ(sv[0], 42);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sv(stdex::length_tag_t{}, 4);
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 4);
		BOOST_TEST_EQ(sv[0], 0);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sv(stdex::length_tag_t{}, 4, 42);
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 4);
		BOOST_TEST_EQ(sv[0], 42);
		BOOST_TEST_EQ(sv[3], 42);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sv = {1, 2};
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 2);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5>  sw = {1, 2, 3, 4, 5};
		stdex::static_vector<int, 10> sv{sw.data() + 2, sw.data() + 4};
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 2);
		BOOST_TEST_EQ(sv[0], 3);
		BOOST_TEST_EQ(sv[1], 4);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5> sv = {1, 2, 3, 4, 5};
		int                          s  = 0;
		for (auto i : sv)
		{
			s += i;
		}
		BOOST_TEST_EQ(s, 15);
	}
	catch (...)
	{
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5>  sw = {1, 2, 3, 4, 5};
		stdex::static_vector<int, 10> sv;
		sv.insert(sv.begin(), sw.data() + 2, sw.data() + 4);
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 2);
		BOOST_TEST_EQ(sv[0], 3);
		BOOST_TEST_EQ(sv[1], 4);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5>  sw = {1, 2, 3, 4, 5};
		stdex::static_vector<int, 10> sv = {1, 2};
		sv.insert(sv.begin() + 1, sw.data() + 2, sw.data() + 4);
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 4);
		BOOST_TEST_EQ(sv[1], 3);
		BOOST_TEST_EQ(sv[2], 4);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sv = {1, 2};
		sv.insert(sv.begin() + 1, stdex::length_tag_t{}, 2, 42);
		BOOST_TEST(!sv.empty());
		BOOST_TEST_EQ(sv.size(), 4);
		BOOST_TEST_EQ(sv[1], 42);
		BOOST_TEST_EQ(sv[2], 42);
		BOOST_TEST(sv.capacity() == 10);
		BOOST_TEST(sv.max_size() == 10);
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 10> sw = {42};
		sw.pop_back();
		BOOST_TEST(sw.size() == 0);
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5> sw = {1, 2, 3, 4, 5};
		sw.erase(sw.data() + 2, sw.data() + 4);
		BOOST_TEST(sw.size() == 3);
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5> sw = {1, 2, 3, 4, 5};
		sw.erase(sw.data() + 2);
		BOOST_TEST(sw.size() == 4);
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5> sw = {1, 2, 3, 4, 5};
		sw.clear();
		BOOST_TEST(sw.size() == 0);
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5> sv = {1, 2, 3};
		stdex::static_vector<int, 5> sw = {11, 12, 13, 14, 15};
		sw.swap(sv);
		BOOST_TEST(sw.size() == 3);
		BOOST_TEST(sv.size() == 5);
		BOOST_TEST(sw[0] == 1);
		BOOST_TEST(sw[1] == 2);
		BOOST_TEST(sw[2] == 3);
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5> sv = {1, 2, 3};
		stdex::static_vector<int, 5> sw = {11, 12, 13, 14, 15};
		swap(sw, sv);
		BOOST_TEST(sw.size() == 3);
		BOOST_TEST(sv.size() == 5);
		BOOST_TEST(sw[0] == 1);
		BOOST_TEST(sw[1] == 2);
		BOOST_TEST(sw[2] == 3);
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5> sv = {1, 2, 3};
		stdex::static_vector<int, 5> sw = sv;
		BOOST_TEST(sv == sw);
		BOOST_TEST(!(sv != sw));
		BOOST_TEST(sv <= sw);
		BOOST_TEST(!(sv < sw));
		BOOST_TEST(sv >= sw);
		BOOST_TEST(!(sv > sw));
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5> sv = {1, 2, 3};
		stdex::static_vector<int, 5> sw = {11, 12, 13, 14, 15};
		BOOST_TEST(sv != sw);
		BOOST_TEST(!(sv == sw));
		BOOST_TEST(sv <= sw);
		BOOST_TEST(sv < sw);
		BOOST_TEST(!(sv >= sw));
		BOOST_TEST(!(sv > sw));
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	try
	{
		stdex::static_vector<int, 5> sv = {1, 2, 3};
		stdex::static_vector<int, 5> sw = {4};
		BOOST_TEST(sv != sw);
		BOOST_TEST(!(sv == sw));
		BOOST_TEST((sv <= sw));
		BOOST_TEST((sv < sw));
		BOOST_TEST(!(sv >= sw));
		BOOST_TEST(!(sv > sw));
	}
	catch (std::experimental::contract_failed &ex)
	{
		std::cout << ex.what() << std::endl;
		BOOST_TEST(false);
	}
	return ::boost::report_errors();
}

#else
int main()
{
	return ::boost::report_errors();
}
#endif
