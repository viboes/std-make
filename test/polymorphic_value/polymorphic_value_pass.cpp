// Copyright (C) 2018 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/bit_mask.hpp>

#include <experimental/polymorphic_value.hpp>
#include <experimental/utility.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

struct X {
    int value;
};

class Base
{
public:
    virtual void foo() const = 0;
    virtual ~Base() {};
};

class Derived : public Base
{
public:
    void foo() const override {}
};


void test_polymorphic_value_explicit_constructible_from_value()
{
    X x = {42};
    stdex::polymorphic_value<X> ptr {x};
    stdex::maybe_unused(ptr);
    BOOST_TEST (42 == ptr->value);

}

void test_xx()
{
// GIVEN base and derived classes.

    using namespace stdex;

// WHEN a polymorphic_value to base is formed from a derived object
    polymorphic_value<Base> poly { Derived() };
// AND the polymorphic_value to base is copied.
    auto poly_copy = poly;

// THEN the copy owns a distinct object
    BOOST_TEST(&*poly != &*poly_copy);
// AND the copy owns a derived type.
    BOOST_TEST(dynamic_cast<Derived*>(&*poly_copy));

}
void example_tree()
{
    using namespace stdex;

    struct Tree
    {
        std::string const name;
        polymorphic_value<Tree> left;
        polymorphic_value<Tree> right;

        Tree(std::string const& name, polymorphic_value<Tree> const& left = polymorphic_value<Tree> {},
             polymorphic_value<Tree> const& right = polymorphic_value<Tree> {})
                        : name { name }, left { left }, right { right }
        {
        }

    };

    Tree t1 { "l0" };
    Tree root = Tree("root", make_polymorphic_value(Tree { "l0" }), make_polymorphic_value<Tree>("r0"));

    root.left = make_polymorphic_value(root); // Copy of root assigned to left
    root.right = make_polymorphic_value(root); // and we don't have cyclic references

}

int main()
{
    test_polymorphic_value_explicit_constructible_from_value();
    test_xx();

    example_tree();

  return ::boost::report_errors();
}
