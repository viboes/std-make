
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

struct NoDefaultConstructible {
    int value;
    NoDefaultConstructible() = delete;
    NoDefaultConstructible(int i) : value(i) {}
    NoDefaultConstructible(NoDefaultConstructible const&)  = default;
    NoDefaultConstructible(NoDefaultConstructible&&)  = default;
    NoDefaultConstructible& operator=(NoDefaultConstructible const&) = default;
    NoDefaultConstructible& operator=(NoDefaultConstructible&&) = default;
};

struct ThrowConstructibleAndMovable {
    int value;
    ThrowConstructibleAndMovable() : value(0) {}
    ThrowConstructibleAndMovable(int i) : value(i) {}
    ThrowConstructibleAndMovable(ThrowConstructibleAndMovable const& other) : value(other.value) { if (value%2) throw 1; }
    ThrowConstructibleAndMovable(ThrowConstructibleAndMovable&& other) : value(other.value) { if (value%2) throw 1; }
    ThrowConstructibleAndMovable& operator=(ThrowConstructibleAndMovable const&) = default;
    ThrowConstructibleAndMovable& operator=(ThrowConstructibleAndMovable&&) = default;

};
bool operator==(ThrowConstructibleAndMovable const& x, ThrowConstructibleAndMovable const& y) {
    return x.value == y.value;
}
bool operator!=(ThrowConstructibleAndMovable const& x, ThrowConstructibleAndMovable const& y) {
    return x.value != y.value;
}

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

static_assert( ! std::is_trivially_default_constructible<stdex::expected<int, short>>::value, "stdex::expected<int, short> is is_trivially_default_constructible.");
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
        return stdex::make_unexpected(-1);
}

int main()
{
    {
        stdex::expected<int, void> x;
        (void)x;
    }
    {
        stdex::expected<int, short> x;
        BOOST_TEST(x.has_value());
        BOOST_TEST(x.value() == 0);
    }
    {
        stdex::expected<int, short> x = 1;
        BOOST_TEST(x.has_value());
        BOOST_TEST(x.value() == 1);
    }
    {
        stdex::expected<int, short> x = 1;
        x = 2;
        BOOST_TEST(x.has_value());
        BOOST_TEST(x.value() == 2);
    }
    {
        stdex::expected<int, short> x = 1;
        x.emplace(2);
        BOOST_TEST(x.has_value());
        BOOST_TEST(x.value() == 2);
    }
    {
        stdex::expected<int, short> x = 1;
        x.emplace2<int>(2);
        BOOST_TEST(x.has_value());
        BOOST_TEST(x.value() == 2);
    }
    {
        stdex::expected<int, short> x = 1;
        x.emplace2<stdex::success_index>(2);
        BOOST_TEST(x.has_value());
        BOOST_TEST(x.value() == 2);
    }
    {
        stdex::expected<int, short> x;
        x = 1;
        BOOST_TEST(x.has_value());
        BOOST_TEST(x.value() == 1);
    }
    {
        stdex::expected<int, short> res = stdex::make_success(1);
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == 1);
        //res = stdex::make_unexpected(-1);
    }
    {
        stdex::expected<short, int> res = stdex::make_success(1);
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == 1);
    }
    {
        stdex::expected<void, short> res;
        BOOST_TEST(res.has_value());
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::in_place, 1, 2};
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value().first == 1);
        BOOST_TEST(res.value().second == 2);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::in_place_type<std::pair<int, short>>, 1, 2};
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value().first == 1);
        BOOST_TEST(res.value().second == 2);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::in_place_success, stdex::in_place, 1, 2};
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value().first == 1);
        BOOST_TEST(res.value().second == 2);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::in_place_type<stdex::success<std::pair<int, short>>>, stdex::in_place, 1, 2};
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value().first == 1);
        BOOST_TEST(res.value().second == 2);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::in_place_success, stdex::in_place, 1, 2};
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value().first == 1);
        BOOST_TEST(res.value().second == 2);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::in_place_type<stdex::unexpected<short>>, stdex::in_place, 1};
        BOOST_TEST(! res.has_value());
        BOOST_TEST(res.error() == 1);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::unexpect, 1};
        BOOST_TEST(! res.has_value());
        BOOST_TEST(res.error() == 1);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::unexpect, 1};
        res.emplace2<stdex::unexpected<short>>(short(2));
        BOOST_TEST(! res.has_value());
        BOOST_TEST(res.error() == 2);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::unexpect, 1};
        res.emplace2<stdex::failure_index>(short(2));
        BOOST_TEST(! res.has_value());
        BOOST_TEST(res.error() == 2);
    }
    {
        stdex::expected<std::pair<int, short>, short> res {stdex::in_place_failure, stdex::in_place, 1};
        BOOST_TEST(! res.has_value());
        BOOST_TEST(res.error() == 1);
    }

    {
        stdex::expected<void, short> res = stdex::make_unexpected(-1);
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
        stdex::expected<void, short> res;
        res.emplace();
        BOOST_TEST(res.has_value());
    }
    {
        stdex::expected<void, short> res {stdex::in_place};
        res.emplace();
        BOOST_TEST(res.has_value());
    }
    {
        stdex::expected<void, short> res {stdex::in_place_failure, short{1}};
        res.emplace();
        BOOST_TEST(res.has_value());
    }
    {
        stdex::expected<void, short> res = stdex::unexpected(short{1});
        res.emplace2<void>();
        BOOST_TEST(res.has_value());
    }
    {
        stdex::expected<void, short> res = stdex::unexpected(short{1});
        res.emplace2<stdex::success_index>();
        BOOST_TEST(res.has_value());
    }
    {
        stdex::expected<std::string, short> res = stdex::make_success("");
        res.emplace("ah");
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == "ah");
    }
    {
        stdex::expected<std::string, short> res = stdex::make_success("");
        res = "ah";
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == "ah");
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
        stdex::expected<std::unique_ptr<int>, int> res = stdex::make_unexpected(1);
        stdex::expected<std::unique_ptr<int>, int> res2 { std::move(res) };
        BOOST_TEST( ! res2.has_value() );
    }
    {
        std::vector<stdex::expected<std::string, int>> vec;
        vec.push_back(stdex::make_unexpected(1));
        vec.push_back(stdex::make_success(std::string("a")));
        vec.push_back(stdex::make_unexpected(3));
        for (auto r : vec)
        {
            if( r.has_value())
                std::cout << "success: " << r.value() << std::endl;
            else
                std::cout << "unexpected: " << r.error() << std::endl;
        }
        //vec[1] = stdex::make_unexpected(2); // this is a limitation of expected as it is not assignable.
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
        stdex::expected<int, short> x = stdex::make_unexpected(1);
        stdex::expected<int, short> y = stdex::make_unexpected(1);
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, short> x = stdex::make_unexpected(1);
        stdex::expected<int, short> y = stdex::make_unexpected(2);
        BOOST_TEST(x != y);
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        stdex::expected<int, short> y = stdex::make_unexpected(2);
        BOOST_TEST(x != y);
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        stdex::expected<int, short> y = stdex::make_unexpected(2);
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
        BOOST_TEST(x != stdex::make_unexpected(2));
    }
    {
        stdex::expected<int, short> x = stdex::make_success(1);
        BOOST_TEST(stdex::make_unexpected(2) != x);
    }
    {
        stdex::expected<int, short> x = stdex::make_unexpected(1);
        BOOST_TEST(x == stdex::make_unexpected(1));
    }
    {
        stdex::expected<int, short> x = stdex::make_unexpected(1);
        BOOST_TEST(stdex::make_unexpected(1) == x);
    }
    {
        stdex::expected<int, short> x = stdex::make_unexpected(1);
        BOOST_TEST(x != stdex::make_unexpected(2));
    }
    {
        stdex::expected<int, short> x = stdex::make_unexpected(1);
        BOOST_TEST(stdex::make_unexpected(2) != x);
    }
    {
        stdex::expected<int, short> x = stdex::make_unexpected(1);
        BOOST_TEST(x != stdex::make_success(2));
    }
    {
        stdex::expected<int, short> x = stdex::make_unexpected(1);
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
        stdex::expected<void, short> x = stdex::make_unexpected(1);
        stdex::expected<void, int> y = x;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<short, short> x = stdex::make_success(1);
        stdex::expected<int, int> y = x;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<short, short> x = stdex::make_unexpected(1);
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
        stdex::expected<int, short> y = stdex::make_unexpected(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, short> x = stdex::make_unexpected(1);
        stdex::expected<int, short> y = stdex::make_unexpected(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<int, short> x = stdex::make_unexpected(1);
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
        stdex::expected<void, short> y = stdex::make_unexpected(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<void, short> x = stdex::make_unexpected(1);
        stdex::expected<void, short> y = stdex::make_unexpected(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<void, short> x = stdex::make_unexpected(1);
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
        stdex::expected<std::string, short> y = stdex::make_unexpected(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<std::string, short> x = stdex::make_unexpected(1);
        stdex::expected<std::string, short> y = stdex::make_unexpected(2);
        x = y;
        BOOST_TEST(x == y);
    }
    {
        stdex::expected<std::string, short> x = stdex::make_unexpected(1);
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
        stdex::expected<std::string, short> x = stdex::make_unexpected(1);
        stdex::expected<std::string, short> y = stdex::make_success("2");
        x = std::move(y);
        BOOST_TEST(x.has_value());
        BOOST_TEST(y.has_value());
        BOOST_TEST(x.value() == "2");
    }
    {
        stdex::expected<std::string, short> x = stdex::make_success("1");
        stdex::expected<std::string, short> y = stdex::make_unexpected(2);
        x = std::move(y);
        BOOST_TEST(!x.has_value());
        BOOST_TEST(!y.has_value());
        BOOST_TEST(x.error() == 2);
    }
    {
        stdex::expected<std::string, short> x = stdex::make_unexpected(1);
        stdex::expected<std::string, short> y = stdex::make_unexpected(2);
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
        stdex::expected<std::unique_ptr<int>, short> x = stdex::make_unexpected(1);
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
        stdex::expected<std::unique_ptr<int>, short> y = stdex::make_unexpected(2);
        x = std::move(y);
        BOOST_TEST(!x.has_value());
        BOOST_TEST(!y.has_value());
        BOOST_TEST(x.error() == 2);
        BOOST_TEST(y.error() == 2);
    }
    {
        stdex::expected<std::unique_ptr<int>, short> x = stdex::make_unexpected(1);
        stdex::expected<std::unique_ptr<int>, short> y = stdex::make_unexpected(2);
        x = std::move(y);
        BOOST_TEST(!x.has_value());
        BOOST_TEST(!y.has_value());
        BOOST_TEST(x.error() == 2);
        BOOST_TEST(y.error() == 2);
    }
    {
        stdex::expected<int, int> x = stdex::make_success(1);
        stdex::expected<int, int> y = stdex::make_success(2);
        using std::swap;
        swap(x, y);
        BOOST_TEST(x.value() == 2);
        BOOST_TEST(y.value() == 1);
    }
    {
        stdex::expected<int, int> x = stdex::make_unexpected(1);
        stdex::expected<int, int> y = stdex::make_unexpected(2);
        using std::swap;
        swap(x, y);
        BOOST_TEST(x.error() == 2);
        BOOST_TEST(y.error() == 1);
    }
    {
        stdex::expected<int, int> x = stdex::make_success(1);
        stdex::expected<int, int> y = stdex::make_unexpected(2);
        using std::swap;
        swap(x, y);
        BOOST_TEST(x.error() == 2);
        BOOST_TEST(y.value() == 1);
    }
    {
        stdex::expected<int, int> x = stdex::make_unexpected(1);
        stdex::expected<int, int> y = stdex::make_success(2);
        using std::swap;
        swap(x, y);
        BOOST_TEST(x.value() == 2);
        BOOST_TEST(y.error() == 1);
    }
    {
        stdex::expected<int, int> x = stdex::make_unexpected(1);
        stdex::expected<int, int> y = stdex::make_success(2);
        using std::swap;
        swap(x, y);
        BOOST_TEST(x.value() == 2);
        BOOST_TEST(y.error() == 1);
    }


#if 0
    {
        stdex::expected<ThrowConstructibleAndMovable, ThrowConstructibleAndMovable> x = stdex::make_success(ThrowConstructibleAndMovable(2));
        stdex::expected<ThrowConstructibleAndMovable, ThrowConstructibleAndMovable> y = stdex::make_unexpected(ThrowConstructibleAndMovable(2));
        x = y; // compile fails as expected
    }
#endif
    {
        stdex::expected<ThrowConstructibleAndMovable, int> x { stdex::make_unexpected(1) };
        stdex::expected<ThrowConstructibleAndMovable, int> y = stdex::make_success(ThrowConstructibleAndMovable(2));
        x = y;
        BOOST_TEST( x == y );
    }
    {
        stdex::expected<ThrowConstructibleAndMovable, int> x = stdex::make_unexpected(1) ;
        stdex::expected<ThrowConstructibleAndMovable, int> y = stdex::make_success(ThrowConstructibleAndMovable(2)) ;
        y.value().value = 3;
        try {
            x = y;
            BOOST_TEST(false);
        } catch (...) {

        }
        BOOST_TEST( x != y );
        BOOST_TEST( ! x.has_value() );
        BOOST_TEST( y.has_value() );
        BOOST_TEST(x.error() == 1);
        BOOST_TEST(y.value().value == 3);
    }
    {
        stdex::expected<ThrowConstructibleAndMovable, int> x = stdex::make_success(ThrowConstructibleAndMovable(2)) ;
        stdex::expected<ThrowConstructibleAndMovable, int> y = stdex::make_unexpected(1) ;
        x.value().value = 3;
        x = y;
        BOOST_TEST( x == y );
    }
    {
        stdex::expected<int, ThrowConstructibleAndMovable> x = stdex::make_success(1) ;
        stdex::expected<int, ThrowConstructibleAndMovable> y = stdex::make_unexpected(ThrowConstructibleAndMovable(2)) ;
        y.error().value = 3;
        try {
            x = y;
            BOOST_TEST(false);
        } catch (...) {

        }
        BOOST_TEST( x != y );
        BOOST_TEST( x.has_value() );
        BOOST_TEST( ! y.has_value() );
        BOOST_TEST(x.value() == 1);
        BOOST_TEST(y.error().value == 3);
    }
    {
        stdex::expected<ThrowConstructibleAndMovable, int> x { stdex::make_unexpected(1) };
        stdex::expected<ThrowConstructibleAndMovable, int> y = stdex::make_success(ThrowConstructibleAndMovable(2));
        using std::swap;
        swap(x, y);
        BOOST_TEST( x.has_value() );
        BOOST_TEST(x.value().value == 2);
        BOOST_TEST( ! y.has_value() );
        BOOST_TEST(y.error() == 1);
    }
    {
        stdex::expected<ThrowConstructibleAndMovable, int> x { stdex::make_unexpected(1) };
        stdex::expected<ThrowConstructibleAndMovable, int> y {stdex::make_success(ThrowConstructibleAndMovable(2)) };
        y.value().value = 3;
        try {
            using std::swap;
            swap(x, y);
        } catch(...) {}
        BOOST_TEST( ! x.has_value() );
        BOOST_TEST( y.has_value() );
        BOOST_TEST(x.error() == 1);
        BOOST_TEST(y.value().value == 3);
    }
    return ::boost::report_errors();
}

#else
int main()
{
    return ::boost::report_errors();
}
#endif
