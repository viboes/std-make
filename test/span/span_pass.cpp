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

void f0(stdex::span<const int, 10> sp)
{
}

template <class T, std::size_t N>
void f2(stdex::span<T const, N> sp)
{
}

// While  f1 can not write on the pointee we can not constraint it enough
// so that template class deduction is  possible
// this is why we need to use SFINAE and forward the call to a function that
// is constrained appropriately but that cannot be deduce the parameter conveniently.
// This idiom is needed if we want to be strict.
// The STL is not strict to this regard and e.g. the iterator passed to the algorithm
// std::copy are not constrained at the signature level, but at the concept level
// template <class InputIt, class OutputIt>
// OutputIt copy(InputIt first, InputIt last, OutputIt d_first);
// As concepts are not checked by the definition, we cannot be sure that
// the implementation is not using the parameters incorrectly.
// Having the parameter qualified by const protects both the caller as well the callee,
// as this can be checked by the compiler.

template <class T, std::size_t N>
auto f1(stdex::span<T, N> sp) -> decltype(f2(stdex::span<T const, N>(sp)))
{
	//once deduced, a stdex::span<T, N> is convertible to a stdex::span<T const, N>
	stdex::span<T const, N> sp2 = sp;
	f2(sp2);
}

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
#if defined __clang__
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
#endif
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
	{
		int                      arr[10]  = {};
		int                      arr2[10] = {};
		stdex::span_ref<int, 10> sp2(arr2);
		stdex::span_ref<int, 10> sp(arr);
		arr2[0] = 1;
		sp      = sp2;
		arr2[0] = 2;
		BOOST_TEST(sp[0] == 1);
	}
	{
		int                      arr[10] = {};
		stdex::span<int, 10>     sp(arr);
		int                      arr2[10] = {};
		stdex::span_ref<int, 10> spr2(arr2);
		stdex::span_ref<int, 10> spr(sp);
		arr2[0] = 1;
		spr     = spr2;
		arr2[0] = 2;
		BOOST_TEST(spr[0] == 1);
	}
	{
		int                  arr[10]  = {};
		int                  arr2[10] = {};
		stdex::span_ref<int> sp2(arr2);
		stdex::span_ref<int> sp(arr);
		arr2[0] = 1;
		sp      = sp2;
		arr2[0] = 2;
		BOOST_TEST(sp[0] == 1);
	}
	{
		int                  arr[10]  = {};
		int                  arr2[10] = {};
		stdex::span<int, 10> sp2(arr2);
		stdex::span<int, 10> sp(arr);
		arr2[0] = 1;
		sp      = sp2;
		arr2[0] = 2;
		BOOST_TEST(sp[0] == 2);
	}
	{
		int                  arr[10]  = {};
		int                  arr2[10] = {};
		stdex::span<int, 10> sp2(arr2);
		stdex::span<int, 10> sp(arr);
		arr2[0] = 1;
		*sp     = sp2;
		arr2[0] = 2;
		BOOST_TEST(sp[0] == 1);
	}
	{
		int                  arr[10] = {};
		stdex::span<int, 10> sp(arr);
		f0(sp);
	}
	{
		int const                  arr[10] = {};
		stdex::span<int const, 10> sp(arr);
		f0(sp);
	}
	{
		int                  arr[10] = {};
		stdex::span<int, 10> sp(arr);
		f1(sp);
	}
	{
		int const                  arr[10] = {};
		stdex::span<int const, 10> sp(arr);
		f1(sp);
	}
	{
		int                  arr[10] = {};
		stdex::span<int, 10> sp(arr);
		f2<const int, 10>(sp); // note the explicit template arguments <const int, 10> that can not be deduced
	}
	{
		int                  arr[10] = {};
		stdex::span<int, 10> sp(arr);
		f2(stdex::span<int const, 10>(sp)); // note the explicit unfriendly cast
	}
	{
		int const                  arr[10] = {};
		stdex::span<int const, 10> sp(arr);
		f2(sp);
	}

	return ::boost::report_errors();
}

#else
int main()
{
	return ::boost::report_errors();
}
#endif
