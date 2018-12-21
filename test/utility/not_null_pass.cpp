// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Based on https://github.com/akrzemi1/explicit/blob/master/test/test_explicit.cpp

// <experimental/.../utility/not_null.hpp>

#include <experimental/utility.hpp>
#include <memory>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

int read_value(stdex::not_null<std::unique_ptr<int>> p)
{
    return *p;
}

void test_not_null_unique_ptr()
{
    int ans = read_value(stdex::as_not_null(std::unique_ptr<int>{new int{2}}));
    BOOST_TEST (2 == ans);

}
void test_not_null_implicit_constructible_from_ref()
{
    int i=1;
    stdex::not_null<int*> ptr = i;
    BOOST_TEST (1 == *ptr);

}
void test_not_null_explicit_constructible_from_ptr()
{
    int i=1;
    stdex::not_null<int*> ptr {&i};
    BOOST_TEST (1 == *ptr);

}
#if 0
void test_not_null_not_constructible_from_nullptr()
{
    stdex::not_null<int*> ptr {nullptr};
//    utility/not_null_pass.cpp:44:27: error: call to deleted constructor of 'stdex::not_null<int *>'
//        stdex::not_null<int*> ptr {nullptr};
//                              ^   ~~~~~~~~~
//    ../include/experimental/fundamental/v3/utility/not_null.hpp:77:9: note: 'not_null' has been explicitly marked deleted here
//            not_null(nullptr_t) = delete;
//            ^
}
#endif

void test_not_null_copy_constructible()
{
    int i=1;
    stdex::not_null<int*> ptr {&i};
    stdex::not_null<int*> ptr2 {ptr};
    BOOST_TEST (1 == *ptr2);
}

void test_not_null_move_constructible()
{
    int i=1;
    stdex::not_null<int*> ptr {&i};
    stdex::not_null<int*> ptr2 {std::move(ptr)};
    BOOST_TEST (1 == *ptr2);
    BOOST_TEST (1 == *ptr);
}

void test_not_null_implicit_heterogeneous_copy_constructible()
{
    struct X {};
    struct Y : X {};
    Y y;
    stdex::not_null<Y*> ptr {&y};
    stdex::not_null<X*> ptr2 {ptr};
}

void test_not_null_copy_assignable()
{
    int i=1;
    stdex::not_null<int*> ptr {&i};
    int j=2;
    stdex::not_null<int*> ptr2 = j;
    ptr = ptr2;
    BOOST_TEST (2 == *ptr);
}

void test_not_null_move_assignable()
{
    int i=1;
    stdex::not_null<int*> ptr {&i};
    int j=2;
    stdex::not_null<int*> ptr2 = j;
    ptr2  = std::move(ptr);
    BOOST_TEST (1 == *ptr2);
    BOOST_TEST (1 == *ptr);
}

void test_not_null_heterogeneous_copy_assignable()
{
    struct X {};
    struct Y : X {};
    Y y;
    stdex::not_null<Y*> ptr = y;
    X x;
    stdex::not_null<X*> ptr2 = x;
    ptr2 = ptr;
    BOOST_TEST (&y == ptr2.get());
    BOOST_TEST (&y == ptr.get());

}
void test_not_null_heterogeneous_move_assignable()
{
    struct X {};
    struct Y : X {};
    Y y;
    stdex::not_null<Y*> ptr = y;
    X x;
    stdex::not_null<X*> ptr2 = x;
    ptr2 = std::move(ptr);
    BOOST_TEST (&y == ptr2.get());
    BOOST_TEST (&y == ptr.get());
}


void test_not_null_explicit_convertible_to_ptr()
{
    int i=1;
    stdex::not_null<int*> ptr {&i};
    BOOST_TEST (&i == static_cast<int*>(ptr));
}
void test_not_null_explicit_convertible_to_bool()
{
    int i=1;
    stdex::not_null<int*> ptr {&i};
    BOOST_TEST (true == static_cast<bool>(ptr));
}
#if 0
void test_not_null_heterogeneous_comparable()
{
    struct X {};
    struct Y : X {};
    Y* ptr=nullptr;
    X* ptr2 = ptr;
    BOOST_TEST (ptr2 == ptr);
}
#endif

void test_not_null_heterogeneous_comparable()
{
    struct X {};
    struct Y : X {};
    Y y;
    stdex::not_null<Y*> ptr = y;
    stdex::not_null<X*> ptr2 = ptr;
    BOOST_TEST (ptr2 == ptr);
}


void test_as_not_null_from_ref()
{
    int i=1;
    stdex::not_null<int*> ptr = stdex::as_not_null_ref(i);
    BOOST_TEST (1 == *ptr);

}
void test_as_not_null_from_ptr()
{
    int i=1;
    stdex::not_null<int*> ptr = stdex::as_not_null(&i);
    BOOST_TEST (1 == *ptr);

}

int main()
{
    test_not_null_explicit_constructible_from_ptr();
    test_not_null_implicit_constructible_from_ref();
    test_not_null_copy_constructible();
    test_not_null_move_constructible();
    test_not_null_implicit_heterogeneous_copy_constructible();
    test_not_null_copy_assignable();
    test_not_null_move_assignable();
    test_not_null_heterogeneous_copy_assignable();
    test_not_null_heterogeneous_move_assignable();
    test_not_null_explicit_convertible_to_ptr();
    test_not_null_explicit_convertible_to_bool();
    test_not_null_heterogeneous_comparable();

    test_not_null_unique_ptr();

    return ::boost::report_errors();
}

