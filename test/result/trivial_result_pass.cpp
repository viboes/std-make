
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright Vicente J. Botet Escriba 2018.

#include <experimental/result.hpp>
#include <memory>
#include <optional>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

static_assert(std::is_trivially_copy_constructible<stdex::result<int, short>>::value, "stdex::result<int, short> is not is_trivially_copy_constructible.");
static_assert(std::is_trivially_move_constructible<stdex::result<int, short>>::value, "stdex::result<int, short> is not is_trivially_move_constructible.");
static_assert(std::is_trivially_copyable<stdex::result<int, short>>::value, "stdex::result<int, short> is not is_trivially_copyable.");

static_assert(! std::is_trivially_copy_assignable<stdex::result<int, short>>::value, "stdex::result<int, short> is  is_trivially_copy_assignable.");
static_assert(! std::is_trivially_move_assignable<stdex::result<int, short>>::value, "stdex::result<int, short> is  is_trivially_move_assignable.");

#if __cplusplus > 201402L

stdex::result<int, short> f(int v, int w) {
    if (w != 0)
        return stdex::success(v/w);
    else
        return stdex::failure(short(-1));
}
#include <string>

template <class A>
auto g(A&& x)
{
    return stdex::success<std::unique_ptr<int>> (std::forward<A>(x));
}
int main()
{
    {
        auto x =  stdex::success(std::string(""));
        static_assert(! std::is_same<decltype(x),std::string>::value, "ERROR");
        BOOST_TEST(x.value == "");
    }
    {
        auto x =  stdex::success(std::string(""));
        auto y = x;
        BOOST_TEST(y.value == "");
    }
    {
        auto x = stdex::success(std::string("aaa"));
        auto y = std::move(x);
        BOOST_TEST(x.value == "");
        BOOST_TEST(y.value == "aaa");
    }
    {
        auto x =  stdex::success(std::make_unique<int>(1));
        static_assert(! std::is_same<decltype(x),std::unique_ptr<int>>::value, "ERROR");
        BOOST_TEST(*x.value == 1);
    }
    {
        auto x =  stdex::success(std::make_unique<int>(1));
        auto y = std::move(x);
        BOOST_TEST(x.value == nullptr);
        BOOST_TEST(*y.value == 1);
    }
    {
        auto x =  std::make_unique<int>(1);
        auto y = stdex::success(std::move(x));
        BOOST_TEST(*y.value == 1);
    }
    {
        stdex::result<int, short> res = stdex::success(1);
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == 1);
        //res = stdex::failure(short(-1));
    }
    {
        stdex::result<void, short> res = stdex::success<void>();
        BOOST_TEST(res.has_value());
    }
    {
        stdex::result<void, short> res = stdex::failure(short(-1));
        BOOST_TEST(! res.has_value());
        BOOST_TEST( res.error() == -1);
    }
    {
        stdex::result<int, short> res = f(4, 2);
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == 2);
    }
    {
        stdex::result<int, short> res = f(4, 0);
        BOOST_TEST(! res.has_value());
        BOOST_TEST( res.error() == -1);
    }
    {
        stdex::result<std::string, short> res = stdex::success(std::string(""));
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == "");
    }
    {
        stdex::result<std::unique_ptr<int>, short> res = stdex::success(std::make_unique<int>(1));
        BOOST_TEST(res.has_value());
        BOOST_TEST(*res.value() == 1);
    }
    {
        auto x = stdex::success(std::make_unique<int>(1));
        auto y = std::move(x);
        BOOST_TEST(*y.value == 1);
    }
    {
        auto x = stdex::success(std::make_unique<int>(1));
        auto y = g(std::move(x));
        BOOST_TEST(*y.value == 1);
    }
    {
        std::optional<std::unique_ptr<int>> res = std::make_unique<int>(1);
        std::optional<std::unique_ptr<int>> res2 { std::move(res) };
    }
    {
        std::optional<stdex::success<std::unique_ptr<int>>> res = stdex::success(std::make_unique<int>(1));
        std::optional<stdex::success<std::unique_ptr<int>>> res2 { std::move(res) };
    }
    {
        stdex::result<std::unique_ptr<int>, short> res = stdex::success(std::make_unique<int>(1));
        //stdex::result<std::unique_ptr<int>, short> res2 = res; // fail as expected
        stdex::result<std::unique_ptr<int>, short> res2 = std::move(res) ;
        BOOST_TEST( res2.has_value() );
        BOOST_TEST( *res2.value() == 1 );
    }
    {
        stdex::result<std::unique_ptr<int>, int> res = stdex::failure(1);
        stdex::result<std::unique_ptr<int>, int> res2 { std::move(res) };
        BOOST_TEST( ! res2.has_value() );
    }

    return ::boost::report_errors();
}
#else
int main()
{
    return ::boost::report_errors();
}
#endif
