// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "flat_non_zero_integer.hpp"

#include <boost/detail/lightweight_test.hpp>
#include <iostream>


int main()
{
#ifdef COMPILE_FAILS
    {
        non_zero_integer a = 1; // compile fails
    }
#endif

    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        BOOST_TEST(bool(onzi));
        BOOST_TEST(onzi->underlying() == 5);
        non_zero_integer nzi = *onzi;
        std::cout << nzi << '\n';
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(0);
        BOOST_TEST( ! bool(onzi) );
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        BOOST_TEST(a == a);
    }
    {
        int i = 5;
        // while the following operation is not safe, we know the value of i
        non_zero_integer a = non_zero_integer::cast(i);
        BOOST_TEST(a == 5);
    }
    {
        non_zero_integer a = non_zero_integer::cast(5);
        BOOST_TEST(a == 5);
    }
#ifdef RUN_FAILS
    {
        non_zero_integer a = non_zero_integer::cast(0);
        static_cast<void>(a);
    }
#endif
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        static_assert(std::is_same<decltype(-a), non_zero_integer>::value);
        BOOST_TEST(a == -(-a));
        BOOST_TEST(-a == -5);
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        static_assert(std::is_same<decltype(+a), non_zero_integer>::value);
        BOOST_TEST(a == +a);
        BOOST_TEST(a == 5);
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        BOOST_TEST(a+1 == 1+a);
        static_assert(std::is_same<decltype(a+1), int>::value);
        static_assert(std::is_same<decltype(1+a), int>::value);
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        non_zero_integer b = a;
        BOOST_TEST(a == b);
        BOOST_TEST(a+b == b+a);
        static_assert(std::is_same<decltype(a+b), non_zero_integer>::value);
        static_assert(std::is_same<decltype(b+a), non_zero_integer>::value);
        BOOST_TEST(a+b == 10);
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        non_zero_integer b = a;
        a += b;
        BOOST_TEST(a == 10);
        BOOST_TEST(b == 5);
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        BOOST_TEST(a-1 == 4);
        BOOST_TEST(1-a == -4);
        BOOST_TEST(a-1 == -(1-a));
        static_assert(std::is_same<decltype(a-1), int>::value);
        static_assert(std::is_same<decltype(1-a), int>::value);
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        non_zero_integer b = a;
        BOOST_TEST(a-b == 0);
        static_assert(std::is_same<decltype(a-b), int>::value);
    }

#ifdef COMPILE_FAILS
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        non_zero_integer b = a;
        a -= b; // compile fails
    }
#endif
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        BOOST_TEST(a*1 == 1*a);
        BOOST_TEST(a*1 == a);
        static_assert(std::is_same<decltype(a*1), int>::value);
        static_assert(std::is_same<decltype(1*a), int>::value);
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        non_zero_integer b = a;
        BOOST_TEST(a == b);
        BOOST_TEST(a*b == b*a);
        static_assert(std::is_same<decltype(a*b), non_zero_integer>::value);
        static_assert(std::is_same<decltype(b*a), non_zero_integer>::value);
        BOOST_TEST(a*b == 25);
    }
    {
        opt_non_zero_integer onzi = non_zero_integer::make(5);
        non_zero_integer a = *onzi;
        non_zero_integer b = a;
        a *= b;
        BOOST_TEST(a == b*b);
        BOOST_TEST(a == 25);
        BOOST_TEST(b == 5);
    }

    return ::boost::report_errors();
}

