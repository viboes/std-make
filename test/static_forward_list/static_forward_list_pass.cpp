// Copyright (C) 2019 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/static_forward_list.hpp>

#include <iostream>
#define JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V 1
#include <experimental/static_forward_list.hpp>

#include <boost/detail/lightweight_test.hpp>

#if __cplusplus >= 201103L
namespace stdex = std::experimental;

int main()
{
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Action
		stdex::static_forward_list<int, 5>        cont;
		const stdex::static_forward_list<int, 5> &ccont = cont;
		//Assert
		BOOST_TEST_EQ(0, cont.size());
		BOOST_TEST(cont.empty());
		BOOST_TEST(!cont.full());
		BOOST_TEST_EQ(5, cont.max_size());
		BOOST_TEST_EQ(5, cont.capacity());

		BOOST_TEST_EQ(0, ccont.size());
		BOOST_TEST(ccont.empty());
		BOOST_TEST(!ccont.full());
		BOOST_TEST_EQ(5, ccont.max_size());
		BOOST_TEST_EQ(5, ccont.capacity());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Action
		const stdex::static_forward_list<int, 5> cont;
		//Assert
		BOOST_TEST_EQ(0, cont.size());
		BOOST_TEST(cont.empty());
		BOOST_TEST(!cont.full());
		BOOST_TEST_EQ(5, cont.max_size());
		BOOST_TEST_EQ(5, cont.capacity());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;
		//Action
		cont.push_front(123);
		//Assert
		BOOST_TEST_EQ(1, cont.size());
		BOOST_TEST(!cont.empty());
		BOOST_TEST(!cont.full());
		BOOST_TEST_EQ(123, cont.front());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;
		cont.push_front(123);
		//Action
		cont.front() = 321;
		//Assert
		BOOST_TEST_EQ(321, cont.front());
		BOOST_TEST_EQ(1, cont.size());
		BOOST_TEST(!cont.empty());
		BOOST_TEST(!cont.full());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;
		cont.push_front(123);
		const stdex::static_forward_list<int, 5> &ccont = cont;
		//Action
		auto r = ccont.front();
		//Assert
		BOOST_TEST_EQ(123, r);
	}
#if 0
	std::cout ">>>>> " < < < < __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;
		cont.push_front(123);
		const stdex::static_forward_list<int, 5> ccont = cont;
		//Action
		ccont.front() = 123; // compile fails
	}
#endif
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		//Action
		stdex::static_forward_list<int, 0> cont;
		BOOST_TEST_EQ(0, cont.size());
		BOOST_TEST(cont.empty());
		BOOST_TEST(cont.full());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 0> cont;
		//Action
		try
		{
			cont.push_front(123);
			BOOST_TEST(false);
		}
		catch (...)
		{
			BOOST_TEST_EQ(0, cont.size());
			BOOST_TEST(cont.empty());
			BOOST_TEST(cont.full());
		}
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 1> cont;
		//Action
		cont.push_front(123);
		//Assert
		BOOST_TEST_EQ(1, cont.size());
		BOOST_TEST(!cont.empty());
		BOOST_TEST(cont.full());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 1> cont;
		cont.push_front(123);
		BOOST_TEST_EQ(1, cont.size());
		BOOST_TEST(!cont.empty());
		BOOST_TEST(cont.full());
		//Action
		try
		{
			cont.push_front(123);
			BOOST_TEST(false);
		}
		catch (...)
		{
			BOOST_TEST_EQ(1, cont.size());
			BOOST_TEST(!cont.empty());
			BOOST_TEST(cont.full());
		}
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;
		cont.push_front(123);
		//Action
		cont.pop_front();
		//Assert
		BOOST_TEST_EQ(0, cont.size());
		BOOST_TEST(cont.empty());
		BOOST_TEST(!cont.full());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;
		int                                i  = 123;
		auto                               it = cont.before_begin();
		//Action
		it = cont.insert_after(it, stdex::length_tag2_t{}, 1, i);
		//Assert
		BOOST_TEST_EQ(*it, 123);
		BOOST_TEST_EQ(1, cont.size());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;
		int                                i  = 123;
		auto                               it = cont.before_begin();
		//Action
		it = cont.insert_after(it, stdex::length_tag2_t{}, 3, i);
		//Assert
		BOOST_TEST_EQ(*it, 123);
		BOOST_TEST_EQ(3, cont.size());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;

		int  i  = 123;
		auto it = cont.before_begin();
		it      = cont.insert_after(it, stdex::length_tag2_t{}, 1, i);
		BOOST_TEST_EQ(*it, 123);
		std::cout << ">>>>> " << __LINE__ << std::endl;
		//Action
		int j = 4;
		it    = cont.insert_after(it, j);
		//Assert
		BOOST_TEST_EQ(*it, 4);
		BOOST_TEST_EQ(2, cont.size());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;
		int                                i  = 123;
		auto                               it = cont.before_begin();
		it                                    = cont.insert_after(it, stdex::length_tag2_t{}, 1, i);
		//Action
		it = cont.insert_after(it, 321);
		//Assert
		BOOST_TEST_EQ(*it, 321);
		BOOST_TEST_EQ(2, cont.size());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;
		auto                               it  = cont.before_begin();
		int                                a[] = {1, 2, 3};
		//Action
		it = cont.insert_after(it, a, a + 3);
		//Assert
		BOOST_TEST_EQ(3, cont.size());
	}
	std::cout << ">>>>> " << __LINE__ << std::endl;
	{
		//Arrange
		stdex::static_forward_list<int, 5> cont;

		auto it  = cont.before_begin();
		int  a[] = {1, 2};
		it       = cont.insert_after(it, a, a + 2);
		it       = cont.before_begin()++;
		//Action
		auto it2 = cont.insert_after(it, a, a + 2);
		//Assert
		BOOST_TEST_EQ(*it2, 2);
		BOOST_TEST_EQ(4, cont.size());
	}

	std::cout << ">>>>> " << __LINE__ << std::endl;
	return ::boost::report_errors();
}

#else
int main()
{
	return ::boost::report_errors();
}
#endif
