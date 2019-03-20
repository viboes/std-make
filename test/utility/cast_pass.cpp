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

struct I
{
};
struct X : I
{
};
struct Y : I
{
};
struct Z : I
{
};

int main()
{
	// type_erase_cast
	{
		int    u;
		void * uptr = &u;
		short *tptr = static_cast<short *>(uptr); // This is dangerous, as it allows to reinterpret a memory from another
		                                          // better to use a specific cast to state explicitly that we type erase the pointer
		BOOST_TEST(tptr == uptr);
	}
	{
		int   u;
		void *uptr = &u;
		//short *tptr = uptr; // fail to compile
		//short *tptr = static_cast<short *>(uptr); // compiles, but is not explicit enough
		short *tptr = stdex::any_ptr_cast<short *>(uptr);
		// Wondering if there is no UB when trying to derreference tptr.
		BOOST_TEST(tptr == uptr);
	}
	{
		int const    u    = 1;
		void const * uptr = &u;
		short const *tptr = stdex::any_ptr_cast<short const *>(uptr);
		BOOST_TEST(tptr == uptr);
	}
	{
		X  u;
		I *uptr = &u;
		X *tptr = stdex::erased_ptr_cast<X *>(uptr);
		BOOST_TEST(tptr == uptr);
	}
	{
		X  u;
		I *uptr = &u;
		Y *tptr = stdex::erased_ptr_cast<Y *>(uptr);
		// Wondering if there is no UB when trying to derreference tptr.
		BOOST_TEST(tptr == uptr);
	}
	{
		X const  u;
		I const *uptr = &u;
		X const *tptr = stdex::erased_ptr_cast<X const *>(uptr);
		BOOST_TEST(tptr == uptr);
	}
	// {
	// 	int const   u    = 1;
	// 	void const *uptr = &u;
	// 	short *     tptr = stdex::any_ptr_cast<short *>(uptr); // this MUST NOT compile
	// }
	// {
	// 	int const   u    = 1;
	// 	void const *uptr = &u;
	// 	int *       tptr = static_cast<int *>(uptr); // this MUST NOT compile
	// 	// error:
	// 	// 	static_cast from 'const void *' to 'int *' casts away qualifiers int *tptr = static_cast<int *>(uptr);
	// 	// 	^~~~~~~~~~~~~~~~~~~~~~~~
	// }

	// explicit_cast
	{
		int    u;
		int *  uptr = &u;
		short *tptr = reinterpret_cast<short *>(uptr);
		BOOST_TEST(reinterpret_cast<int *>(tptr) == uptr);
	}
	{
		int    u;
		int *  uptr = &u;
		short *tptr = stdex::explicit_cast<short *>(uptr);
		BOOST_TEST(stdex::explicit_cast<int *>(tptr) == uptr);
	}

	// convert_cast
	// should we use convert_cast while converting pointer/references?
	{
		using T = int;
		using U = int;
		U u     = 1;
		T t     = stdex::convert_cast<T>(u);
		BOOST_TEST(stdex::convert_cast<U>(t) == u);
	}
	// incomplete_cast
	{
		struct T;
		struct U
		{
		};
		U  u;
		U *uptr = &u;
		T *tptr = stdex::incomplete_cast<T *>(uptr);
		BOOST_TEST(stdex::incomplete_cast<U *>(tptr) == uptr);
	}
	{
		struct T;
		struct U
		{
		};
		U  u;
		U &uref = u;
		T &tref = stdex::incomplete_cast<T &>(uref);
		BOOST_TEST(&stdex::incomplete_cast<U &>(tref) == &uref);
	}
	// assume_aligned
	{
		int   i   = 1;
		char *ptr = reinterpret_cast<char *>(&i);
		ptr       = stdex::assume_aligned<alignof(int)>(ptr);
		BOOST_TEST_EQ(*reinterpret_cast<int *>(ptr), i);
	}
	{
		int const i   = 1;
		auto      ptr = reinterpret_cast<char const *>(&i);
		ptr           = stdex::assume_aligned<alignof(int)>(ptr);
		BOOST_TEST_EQ(*reinterpret_cast<int const *>(ptr), i);
	}

	// aligned_as_cast
	{
		int  i   = 1;
		auto ptr = reinterpret_cast<char *>(&i);
		auto p   = stdex::aligned_as_cast<int>(ptr);
		BOOST_TEST_EQ(*p, i);
	}
	{
		int const   i   = 1;
		char const *ptr = reinterpret_cast<char const *>(&i);
		auto        p   = stdex::aligned_as_cast<int>(ptr);
		BOOST_TEST_EQ(*p, i);
	}
	//aligned_as
	{
		struct X
		{
			short i;
			short j;
		};
		union Y
		{
			X   x;
			int y;
		};

		Y k;
		k.x.i  = 1;
		k.x.j  = 1;
		auto p = stdex::aligned_as<int>(&k.x.i);
		BOOST_TEST_EQ(*p, 0x00010001);
	}
	{
		struct X
		{
			short i;
			short j;
		};
		union Y
		{
			X   x;
			int y;
		};

		Y k;
		k.x.i = 1;
		k.x.j = 1;
		try
		{
			auto p = stdex::aligned_as<int>(&k.x.j);
			stdex::maybe_unused(p);
			BOOST_TEST(false && "should throw an exception");
		}
		catch (...)
		{
		}
	}

	return ::boost::report_errors();
}
