
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright Vicente J. Botet Escriba 2018.

#include <boost/detail/lightweight_test.hpp>
#if __cplusplus >= 201402L && defined __clang__

#include <experimental/result.hpp>
#include <memory>
#include <vector>
#include <string>

namespace stdex = std::experimental;

static_assert(std::is_trivially_copy_constructible<stdex::result<int, short>>::value, "stdex::result<int, short> is not is_trivially_copy_constructible.");
static_assert(std::is_trivially_move_constructible<stdex::result<int, short>>::value, "stdex::result<int, short> is not is_trivially_move_constructible.");
static_assert(std::is_trivially_copyable<stdex::result<int, short>>::value, "stdex::result<int, short> is not is_trivially_copyable.");

static_assert(! std::is_trivially_copy_assignable<stdex::result<int, short>>::value, "stdex::result<int, short> is  is_trivially_copy_assignable.");
static_assert(! std::is_trivially_move_assignable<stdex::result<int, short>>::value, "stdex::result<int, short> is  is_trivially_move_assignable.");


stdex::result<int, short> f(int v, int w) {
    if (w != 0)
        return stdex::make_success(v/w);
    else
        return stdex::make_failure(-1);
}

template <class A>
auto g(A&& x)
{
    return stdex::success<std::unique_ptr<int>> (std::forward<A>(x));
}
int main()
{
    {
        stdex::success<short> x =  stdex::make_success(1);
        BOOST_TEST(x.value == 1);
    }
    {
        stdex::failure<short> x =  stdex::make_failure(1);
        BOOST_TEST(x.value == 1);
    }
    {
        auto x =  stdex::make_success(std::string(""));
        static_assert(! std::is_same<decltype(x),std::string>::value, "ERROR");
        BOOST_TEST(x.value == "");
    }
    {
        stdex::success<std::string> x =  stdex::make_success(std::string(""));
        auto y = x;
        BOOST_TEST(y.value == "");
    }
    {
        stdex::success<std::string> x =  stdex::make_success("");
        auto y = x;
        BOOST_TEST(y.value == "");
    }
    {
        auto x = stdex::make_success(std::string("aaa"));
        auto y = std::move(x);
        BOOST_TEST(x.value == "");
        BOOST_TEST(y.value == "aaa");
    }
    {
        auto x =  stdex::make_success(std::make_unique<int>(1));
        static_assert(! std::is_same<decltype(x),std::unique_ptr<int>>::value, "ERROR");
        BOOST_TEST(*x.value == 1);
    }
    {
        auto x =  stdex::make_success(std::make_unique<int>(1));
        auto y = std::move(x);
        BOOST_TEST(x.value == nullptr);
        BOOST_TEST(*y.value == 1);
    }
    {
        auto x =  std::make_unique<int>(1);
        auto y = stdex::make_success(std::move(x));
        BOOST_TEST(*y.value == 1);
    }
    {
        stdex::result<int, short> res = stdex::make_success(1);
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == 1);
        //res = stdex::make_failure(-1);
    }
    {
        stdex::result<short, int> res = stdex::make_success(1);
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == 1);
    }
    {
        stdex::result<void, short> res = stdex::success<void>();
        BOOST_TEST(res.has_value());
    }
    {
        stdex::result<void, short> res = stdex::make_failure(-1);
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
        stdex::result<std::string, short> res = stdex::make_success("");
        BOOST_TEST(res.has_value());
        BOOST_TEST(res.value() == "");
    }
    {
        stdex::result<std::unique_ptr<int>, short> res = stdex::make_success(std::make_unique<int>(1));
        BOOST_TEST(res.has_value());
        BOOST_TEST(*res.value() == 1);
    }
    {
        auto x = stdex::make_success(std::make_unique<int>(1));
        auto y = std::move(x);
        BOOST_TEST(*y.value == 1);
    }
    {
        auto x = stdex::make_success(std::make_unique<int>(1));
        auto y = g(std::move(x));
        BOOST_TEST(*y.value == 1);
    }
    {
        stdex::result<std::unique_ptr<int>, short> res = stdex::make_success(std::make_unique<int>(1));
        //stdex::result<std::unique_ptr<int>, short> res2 = res; // fail as expected
        stdex::result<std::unique_ptr<int>, short> res2 = std::move(res) ;
        BOOST_TEST( res2.has_value() );
        BOOST_TEST( *res2.value() == 1 );
    }
    {
        stdex::result<std::unique_ptr<int>, int> res = stdex::make_failure(1);
        stdex::result<std::unique_ptr<int>, int> res2 { std::move(res) };
        BOOST_TEST( ! res2.has_value() );
    }
    {
        std::vector<stdex::result<std::string, int>> vec;
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
        //vec[1] = stdex::make_failure(2); // this is a limitation of result as it is not assignable.
    }

    return ::boost::report_errors();
}

#else
int main()
{
    return ::boost::report_errors();
}
#endif
