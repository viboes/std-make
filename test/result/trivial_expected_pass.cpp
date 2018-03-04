
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright Vicente J. Botet Escriba 2018.

#include <boost/detail/lightweight_test.hpp>
#if __cplusplus > 201402L && defined __clang__

#include <experimental/expected2.hpp>
#include <memory>
#include <vector>
#include <string>

namespace stdex = std::experimental;
#if 0
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_destruct_base<int, short>>::value, "stdex::expected_detail::expected_destruct_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_storage_common_base<int, short>>::value, "stdex::expected_detail::expected_storage_common_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_storage_base<int, short>>::value, "stdex::expected_detail::expected_storage_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_default_ctor_base<int, short>>::value, "stdex::expected_detail::expected_default_ctor_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_copy_base<int, short>>::value, "stdex::expected_detail::expected_copy_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_move_base<int, short>>::value, "stdex::expected_detail::expected_move_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_copy_assign_base<int, short>>::value, "stdex::expected_detail::expected_copy_assign_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_move_assign_base<int, short>>::value, "stdex::expected_detail::expected_move_assign_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_sfinae_assign_base<int, short>>::value, "stdex::expected_detail::expected_sfinae_assign_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_sfinae_ctor_base<int, short>>::value, "stdex::expected_detail::expected_sfinae_ctor_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_sfinae_ctor_base<int, short>>::value, "stdex::expected_detail::expected_sfinae_ctor_base<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_assignable<stdex::expected_detail::expected_base<int, short>>::value, "stdex::expected_detail::expected_base<int, short> is not is_trivially_default_constructible.");

#endif

//static_assert(std::is_trivially_default_constructible<stdex::expected<int, short>>::value, "stdex::expected<int, short> is not is_trivially_default_constructible.");
static_assert(std::is_trivially_copy_constructible<stdex::expected<int, short>>::value, "stdex::expected<int, short> is not is_trivially_copy_constructible.");
static_assert(std::is_trivially_move_constructible<stdex::expected<int, short>>::value, "stdex::expected<int, short> is not is_trivially_move_constructible.");
static_assert(std::is_trivially_copyable<stdex::expected<int, short>>::value, "stdex::expected<int, short> is not is_trivially_copyable.");

static_assert(std::is_trivially_copy_assignable<stdex::expected<void, short>>::value, "stdex::expected<void, short> is  is_trivially_copy_assignable.");
static_assert(std::is_trivially_copy_assignable<stdex::expected<int, short>>::value, "stdex::expected<int, short> is  is_trivially_copy_assignable.");
static_assert(std::is_trivially_move_assignable<stdex::expected<int, short>>::value, "stdex::expected<int, short> is  is_trivially_move_assignable.");

stdex::expected<int, short> f(int v, int w) {
    if (w != 0)
        return stdex::make_success(v/w);
    else
        return stdex::make_failure(-1);
}

int main()
{
    {
        stdex::expected<int, short> x;
        BOOST_TEST(x.has_value());
        BOOST_TEST(x.value() == 0);
        (void)x;
    }
    {
        stdex::expected<int, short> res = stdex::make_success(1);
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == 1);
        //res = stdex::make_failure(-1);
    }
    {
        stdex::expected<short, int> res = stdex::make_success(1);
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == 1);
    }
    {
        //stdex::expected<void, short> res; // compile fails as expected
    }
    {
        stdex::expected<void, short> res = stdex::success<void>();
        BOOST_TEST(res.has_value());
    }
    {
        stdex::expected<void, short> res = stdex::make_success();
        BOOST_TEST(res.has_value());
    }
    {
        stdex::expected<std::pair<int, short>, short> res {std::in_place_type<stdex::success<std::pair<int, short>>>, std::in_place, 1, 2};
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value().first == 1);
        BOOST_TEST(res.value().second == 2);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {std::in_place_type<stdex::failure<short>>, std::in_place, 1};
        BOOST_TEST(! res.has_value());
        BOOST_TEST(res.error() == 1);
    }

    {
        stdex::expected<void, short> res = stdex::make_failure(-1);
        BOOST_TEST(! res.has_value());
        BOOST_TEST( res.error() == -1);
    }
    {
        stdex::expected<int, short> res = f(4, 2);
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == 2);
    }
    {
        stdex::expected<int, short> res = f(4, 0);
        BOOST_TEST(! res.has_value());
        BOOST_TEST( res.error() == -1);
    }
    {
        stdex::expected<std::string, short> res = stdex::make_success("");
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == "");
    }
    {
        stdex::expected<std::unique_ptr<int>, short> res = stdex::make_success(std::make_unique<int>(1));
        BOOST_TEST(res.has_value());
        BOOST_TEST(*res.value() == 1);
    }
    {
        stdex::expected<std::unique_ptr<int>, short> res = stdex::make_success(std::make_unique<int>(1));
        //stdex::expected<std::unique_ptr<int>, short> res2 = res; // fail as expected
        stdex::expected<std::unique_ptr<int>, short> res2 = std::move(res) ;
        BOOST_TEST( res2.has_value() );
        BOOST_TEST( *res2.value() == 1 );
    }
    {
        stdex::expected<std::unique_ptr<int>, int> res = stdex::make_failure(1);
        stdex::expected<std::unique_ptr<int>, int> res2 { std::move(res) };
        BOOST_TEST( ! res2.has_value() );
    }
    {
        std::vector<stdex::expected<std::string, int>> vec;
        vec.push_back(stdex::make_failure(1));
        vec.push_back(stdex::make_success(std::string("a")));
        vec.push_back(stdex::make_failure(3));
        for (auto r : vec)
        {
            if( r.has_value())
                std::cout << "success: " << r.value() << std::endl;
            else
                std::cout << "failure: " << r.error() << std::endl;
        }
        //vec[1] = stdex::make_failure(2); // this is a limitation of expected as it is not assignable.
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        stdex::expected<int, short> y = x;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        stdex::expected<int, short> y = stdex::make_success(2);
        BOOST_TEST(x != y);
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        stdex::expected<int, short> y = stdex::make_failure(1);
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        stdex::expected<int, short> y = stdex::make_failure(2);
        BOOST_TEST(x != y);
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        stdex::expected<int, short> y = stdex::make_failure(2);
        BOOST_TEST(x != y);
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        stdex::expected<int, short> y = stdex::make_failure(2);
        BOOST_TEST(y != x);
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        BOOST_TEST(x == stdex::make_success(1));
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        BOOST_TEST(stdex::make_success(1) == x);
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        BOOST_TEST(x != stdex::make_success(2));
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        BOOST_TEST(stdex::make_success(2) != x);
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        BOOST_TEST(x != stdex::make_failure(2));
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        BOOST_TEST(stdex::make_failure(2) != x);
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        BOOST_TEST(x == stdex::make_failure(1));
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        BOOST_TEST(stdex::make_failure(1) == x);
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        BOOST_TEST(x != stdex::make_failure(2));
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        BOOST_TEST(stdex::make_failure(2) != x);
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        BOOST_TEST(x != stdex::make_success(2));
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        BOOST_TEST(stdex::make_success(2) != x);
    }
    {
        stdex::expected<void, short> x = stdex::make_success();
        stdex::expected<void, short> y;
        y = x;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<void, short> x = stdex::make_success();
        stdex::expected<void, int> y = x;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<void, short> x = stdex::make_failure(1);
        stdex::expected<void, int> y = x;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<short, short> x = stdex::make_success(1);
        stdex::expected<int, int> y = x;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<short, short> x = stdex::make_failure(1);
        stdex::expected<int, int> y = x;
        BOOST_TEST(x == y);
    }
    // assignment
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        stdex::expected<int, short> y = stdex::make_success(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        stdex::expected<int, short> y = stdex::make_failure(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        stdex::expected<int, short> y = stdex::make_failure(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, short> x = stdex::make_failure(1);
        stdex::expected<int, short> y = stdex::make_success(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<void, short> x = stdex::make_success();
        stdex::expected<void, short> y = stdex::make_success();
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<void, short> x = stdex::make_success();
        stdex::expected<void, short> y = stdex::make_failure(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<void, short> x = stdex::make_failure(1);
        stdex::expected<void, short> y = stdex::make_failure(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<void, short> x = stdex::make_failure(1);
        stdex::expected<void, short> y = stdex::make_success();
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, int> x = stdex::make_success(1);
        stdex::expected<short, short> y = stdex::make_success(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, int> x = stdex::make_success(1);
        stdex::expected<short, int> y = stdex::make_success(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, int> x = stdex::make_success(1);
        stdex::expected<int, short> y = stdex::make_success(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<std::string, short> x = stdex::make_success("1");
        stdex::expected<std::string, short> y = stdex::make_success("2");
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<std::string, short> x = stdex::make_success("1");
        stdex::expected<std::string, short> y = stdex::make_failure(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<std::string, short> x = stdex::make_failure(1);
        stdex::expected<std::string, short> y = stdex::make_failure(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<std::string, short> x = stdex::make_failure(1);
        stdex::expected<std::string, short> y = stdex::make_success("2");
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<std::string, short> x = stdex::make_success("1");
        stdex::expected<std::string, short> y = stdex::make_success("2");
        x = std::move(y);
        BOOST_TEST(x.has_value());
        BOOST_TEST(y.has_value());
        BOOST_TEST(x.value() == "2");
    }
    {
        stdex::expected<std::string, short> x = stdex::make_failure(1);
        stdex::expected<std::string, short> y = stdex::make_success("2");
        x = std::move(y);
        BOOST_TEST(x.has_value());
        BOOST_TEST(y.has_value());
        BOOST_TEST(x.value() == "2");
    }
    {
        stdex::expected<std::string, short> x = stdex::make_success("1");
        stdex::expected<std::string, short> y = stdex::make_failure(2);
        x = std::move(y);
        BOOST_TEST(!x.has_value());
        BOOST_TEST(!y.has_value());
        BOOST_TEST(x.error() == 2);
    }
    {
        stdex::expected<std::string, short> x = stdex::make_failure(1);
        stdex::expected<std::string, short> y = stdex::make_failure(2);
        x = std::move(y);
        BOOST_TEST(!x.has_value());
        BOOST_TEST(!y.has_value());
        BOOST_TEST(x.error() == 2);
    }
#if 0
    {
        stdex::expected<std::unique_ptr<int>, short> x = stdex::make_success(std::make_unique<int>(1));
        stdex::expected<std::unique_ptr<int>, short> y = stdex::make_success(std::make_unique<int>(2));
        x = y; // fails as expected
    }
#endif
    {
        stdex::expected<std::unique_ptr<int>, short> x = stdex::make_success(std::make_unique<int>(1));
        stdex::expected<std::unique_ptr<int>, short> y = stdex::make_success(std::make_unique<int>(2));
        x = std::move(y);
        BOOST_TEST(x.has_value());
        BOOST_TEST(y.has_value());
        BOOST_TEST(x.value());
        BOOST_TEST( ! y.value());
        BOOST_TEST(*x.value() == 2);
    }
    {
        stdex::expected<std::unique_ptr<int>, short> x = stdex::make_failure(1);
        stdex::expected<std::unique_ptr<int>, short> y = stdex::make_success(std::make_unique<int>(2));
        x = std::move(y);
        BOOST_TEST(x.has_value());
        BOOST_TEST(y.has_value());
        BOOST_TEST(x.value());
        BOOST_TEST( ! y.value());
        BOOST_TEST(*x.value() == 2);
    }
    {
        stdex::expected<std::unique_ptr<int>, short> x = stdex::make_success(std::make_unique<int>(1));
        stdex::expected<std::unique_ptr<int>, short> y = stdex::make_failure(2);
        x = std::move(y);
        BOOST_TEST(!x.has_value());
        BOOST_TEST(!y.has_value());
        BOOST_TEST(x.error() == 2);
        BOOST_TEST(y.error() == 2);
    }
    {
        stdex::expected<std::unique_ptr<int>, short> x = stdex::make_failure(1);
        stdex::expected<std::unique_ptr<int>, short> y = stdex::make_failure(2);
        x = std::move(y);
        BOOST_TEST(!x.has_value());
        BOOST_TEST(!y.has_value());
        BOOST_TEST(x.error() == 2);
        BOOST_TEST(y.error() == 2);
    }
    return ::boost::report_errors();
}

#else
int main()
{
    return ::boost::report_errors();
}
#endif
