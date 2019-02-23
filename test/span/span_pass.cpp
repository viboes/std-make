// Copyright (C) 2019 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/status_value.hpp>

#include <experimental/span.hpp>

#include <vector>

#include <boost/detail/lightweight_test.hpp>

#if __cplusplus >= 201103L
namespace stdex = std::experimental;

int main()
{
	{
		stdex::span<int> sp;
		BOOST_TEST(sp.size() == 0);
		BOOST_TEST(sp.data() == nullptr);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp(arr);
		BOOST_TEST(sp.size() == 10);
		BOOST_TEST(sp.data() == arr);
	}
	{
		int                        arr[10];
		stdex::span<const int, 10> sp(arr);
		BOOST_TEST(sp.size() == 10);
		BOOST_TEST(sp.data() == arr);
	}
	{
		const int                  arr[10] = {};
		stdex::span<const int, 10> sp(arr);
		BOOST_TEST(sp.size() == 10);
		BOOST_TEST(sp.data() == arr);
	}
#if 0
	{
		int                  arr[11];
		stdex::span<int, 10> sp(arr);
	}
#endif
#if 0
	{
		const int            arr[10] = {};
		stdex::span<int, 10> sp(arr);
	// error: no matching constructor for initialization of 'stdex::span<int, 10>'
    //             stdex::span<int, 10> sp(arr);
	// 	^~~~
    // note: candidate template ignored: could not match 'int' against 'const int'
	}
	{
		int                 arr[10];
		stdex::span<int, 5> sp(arr);
	// error: static_assert failed "Mismatch between fixed-size extent and size of initializing data."
    //             static_assert(Other == Ext || Other == dynamic_extent,
    //             ^             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}
#endif
	{
		int              arr[10];
		stdex::span<int> sp(arr, 5);
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr);
	}
	{
		int              arr[10];
		stdex::span<int> sp(arr + 2, arr + 7);
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr + 2);
	}
	{
		std::vector<int> vec(10);
		stdex::span<int> sp(vec);
		BOOST_TEST(sp.size() == 10);
		BOOST_TEST(sp.data() == vec.data());
	}
	{
		const std::vector<int> vec(10);
		stdex::span<const int> sp(vec);
		BOOST_TEST(sp.size() == 10);
		BOOST_TEST(sp.data() == vec.data());
	}
	// copy constructor
	{
		int              arr[5];
		stdex::span<int> sp2(arr);
		stdex::span<int> sp = sp2;
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr);
	}
	{
		int                 arr[5];
		stdex::span<int, 5> sp2(arr);
		stdex::span<int>    sp = sp2;
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr);
	}
	{
		int                 arr[5];
		stdex::span<int, 5> sp2(arr);
		stdex::span<int, 5> sp = sp2;
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp2(arr);
		stdex::span<int, 5>  sp = sp2.first<5>();
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp2(arr);
		stdex::span<int, 5>  sp = sp2.last<5>();
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr + 5);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp2(arr);
		stdex::span<int, 5>  sp = sp2.subspan<2, 5>();
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr + 2);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp2(arr);
		stdex::span<int, 8>  sp = sp2.subspan<2>();
		BOOST_TEST(sp.size() == 8);
		BOOST_TEST(sp.data() == arr + 2);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp2(arr);
		stdex::span<int>     sp = sp2.subspan<2>();
		BOOST_TEST(sp.size() == 8);
		BOOST_TEST(sp.data() == arr + 2);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp2(arr);
		stdex::span<int>     sp = sp2.first(5);
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp2(arr);
		stdex::span<int>     sp = sp2.last(5);
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr + 5);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp2(arr);
		stdex::span<int>     sp = sp2.subspan(2, 5);
		BOOST_TEST(sp.size() == 5);
		BOOST_TEST(sp.data() == arr + 2);
	}
	{
		int                  arr[10];
		stdex::span<int, 10> sp2(arr);
		stdex::span<int>     sp = sp2.subspan(2);
		BOOST_TEST(sp.size() == 8);
		BOOST_TEST(sp.data() == arr + 2);
	}
	{
		stdex::span<int> sp;
		BOOST_TEST(sp.empty());
		BOOST_TEST(sp.data() == nullptr);
	}
	{
		int              arr[10] = {};
		stdex::span<int> sp(arr);
		for (int i = 0; i < 10; ++i)
			BOOST_TEST(sp[i] == 0);
	}
	{
		int              arr[10] = {};
		stdex::span<int> sp(arr);
		for (auto i : sp)
			BOOST_TEST(i == 0);
	}
	return ::boost::report_errors();
}

#else
int main()
{
	return ::boost::report_errors();
}
#endif
