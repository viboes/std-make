// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/bit_mask.hpp>

#include <experimental/observer_ptr.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

struct X {
    int value;
};

void test_observer_ptr_default_constructible()
{
    stdex::observer_ptr<int> ptr;
    BOOST_TEST (nullptr == ptr);
    BOOST_TEST (ptr == nullptr);
}
void test_observer_ptr_constructible_from_nullptr_t()
{
    stdex::observer_ptr<int> ptr{nullptr};
    BOOST_TEST (nullptr == ptr);
    BOOST_TEST (ptr == nullptr);
}

void test_observer_ptr_explicit_constructible_from_ptr()
{
    int i=1;
    stdex::observer_ptr<int> ptr {&i};
    BOOST_TEST (&i == ptr.get());
    BOOST_TEST (1 == *ptr);

    X x = {42};
    stdex::observer_ptr<X> ptr2 {&x};
    BOOST_TEST (42 == ptr2->value);

}

void test_observer_ptr_copy_constructible()
{
    int i=1;
    stdex::observer_ptr<int> ptr {&i};
    stdex::observer_ptr<int> ptr2 {ptr};
    BOOST_TEST (1 == *ptr2);
}

void test_observer_ptr_move_constructible()
{
    int i=1;
    stdex::observer_ptr<int> ptr {&i};
    stdex::observer_ptr<int> ptr2 {std::move(ptr)};
    BOOST_TEST (&i == ptr2.get());
    BOOST_TEST (&i == ptr.get());
    BOOST_TEST (1 == *ptr);
}

void test_observer_ptr_implicit_heterogeneous_copy_constructible()
{
    struct X {};
    struct Y : X {};
    Y y;
    stdex::observer_ptr<Y> ptr {&y};
    stdex::observer_ptr<X> ptr2 {ptr};
    BOOST_TEST (&y == ptr2.get());
}

void test_observer_ptr_copy_assignable()
{
    int i=1;
    stdex::observer_ptr<int> ptr {&i};
    int j=2;
    stdex::observer_ptr<int> ptr2 {&j};
    ptr = ptr2;
    BOOST_TEST (&j == ptr.get());
    BOOST_TEST (&j == ptr2.get());
    BOOST_TEST (2 == *ptr);
}

void test_observer_ptr_move_assignable()
{
    int i=1;
    stdex::observer_ptr<int> ptr {&i};
    int j=2;
    stdex::observer_ptr<int> ptr2 {&j};
    ptr2  = std::move(ptr);
    BOOST_TEST (&i == ptr.get());
    BOOST_TEST (&i == ptr2.get());
    BOOST_TEST (1 == *ptr2);
    BOOST_TEST (1 == *ptr);
}

void test_observer_ptr_heterogeneous_copy_assignable()
{
    struct X {};
    struct Y : X {};
    Y y;
    stdex::observer_ptr<Y> ptr {&y};
    X x;
    stdex::observer_ptr<X> ptr2 {&x};
    ptr2 = ptr;
    BOOST_TEST (&y == ptr2.get());
    BOOST_TEST (&y == ptr.get());
}
void test_observer_ptr_heterogeneous_move_assignable()
{
    struct X {};
    struct Y : X {};
    Y y;
    stdex::observer_ptr<Y> ptr {&y};
    X x;
    stdex::observer_ptr<X> ptr2 {&x};
    ptr2 = std::move(ptr);
    BOOST_TEST (&y == ptr2.get());
    BOOST_TEST (&y == ptr.get());
}
void test_observer_ptr_explicit_convertible_to_ptr()
{
    int i=1;
    stdex::observer_ptr<int> ptr {&i};
    BOOST_TEST (&i == static_cast<int*>(ptr));
}

void test_observer_ptr_explicit_convertible_to_bool()
{
    int i=1;
    stdex::observer_ptr<int> ptr {&i};
    BOOST_TEST (true == static_cast<bool>(ptr));
    stdex::observer_ptr<int> ptr2;
    BOOST_TEST (false == static_cast<bool>(ptr2));
}

void test_observer_ptr_release()
{
    int i=1;
    stdex::observer_ptr<int> ptr {&i};
    auto* p = ptr.release();
    BOOST_TEST (p == &i);
    BOOST_TEST (false == static_cast<bool>(ptr));
}

void test_observer_ptr_reset()
{
    stdex::observer_ptr<int> ptr;
    int j=2;
    ptr.reset(&j);
    BOOST_TEST (&j == static_cast<int*>(ptr));
    ptr.reset();
    BOOST_TEST (false == static_cast<bool>(ptr));
}

void test_observer_ptr_swap()
{
    int i=1;
    stdex::observer_ptr<int> ptr {&i};
    stdex::observer_ptr<int> ptr2;
    swap(ptr, ptr2);
    BOOST_TEST (false == static_cast<bool>(ptr));
    BOOST_TEST (true == static_cast<bool>(ptr2));

}

void test_observer_ptr_heterogeneous_eq_comparable()
{
    struct X {};
    struct Y : X {};
    Y y;
    stdex::observer_ptr<Y> ptr {&y};
    stdex::observer_ptr<X> ptr2 = ptr;
    BOOST_TEST (ptr2 == ptr);
    BOOST_TEST (! (ptr2 != ptr) );
    ptr.reset();
    BOOST_TEST (ptr2 != ptr);
}
void test_observer_ptr_heterogeneous_eq_comparable_with_nullptr()
{
    stdex::observer_ptr<int> ptr;
    BOOST_TEST (nullptr == ptr);
    BOOST_TEST (ptr == nullptr);
    int j=2;
    ptr.reset(&j);
    BOOST_TEST (nullptr != ptr);
    BOOST_TEST (ptr != nullptr);
}

int f(int arr[])
{
    return arr[0];
}

int g(int *ptr)
{
    return ptr[0];
}

int h(stdex::observer_ptr<int> ptr)
{
    return *ptr;
}

void example_observer_ptr_x1()
{
    int arr[] = {1, 2, 3};
    BOOST_TEST(1 == f(arr));
    BOOST_TEST(1 == g(arr));
    int * ptr= & arr[0];
    BOOST_TEST(1 == f(ptr));
    BOOST_TEST(1 == g(ptr));
    BOOST_TEST(1 == h(stdex::make_observer(ptr)));
    BOOST_TEST(1 == h(stdex::make_observer(arr)));
}
void example_observer_ptr_x2()
{
    int const i = 0;
    auto ptr = stdex::make_observer(&i);
    BOOST_TEST(0 == *ptr);
#if 0
    *ptr = 1;
//    error: cannot assign to return value because function 'operator*' returns a const value
//        *ptr = 1;
//        ~~~~ ^
#endif
}
int main()
{

    test_observer_ptr_default_constructible();
    test_observer_ptr_constructible_from_nullptr_t();
    test_observer_ptr_explicit_constructible_from_ptr();
    test_observer_ptr_copy_constructible();
    test_observer_ptr_move_constructible();
    test_observer_ptr_implicit_heterogeneous_copy_constructible();
    test_observer_ptr_copy_assignable();
    test_observer_ptr_move_assignable();
    test_observer_ptr_heterogeneous_copy_assignable();
    test_observer_ptr_heterogeneous_move_assignable();
    test_observer_ptr_explicit_convertible_to_ptr();
    test_observer_ptr_release();
    test_observer_ptr_reset();
    test_observer_ptr_swap();
    test_observer_ptr_heterogeneous_eq_comparable();
    test_observer_ptr_heterogeneous_eq_comparable_with_nullptr();

    example_observer_ptr_x1();
    example_observer_ptr_x2();

    return ::boost::report_errors();
}
