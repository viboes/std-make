
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright Vicente J. Botet Escriba 2018.

#include <boost/detail/lightweight_test.hpp>

#include <experimental/fundamental/v3/result/success_failure.hpp>
#include <string>

namespace stdex = std::experimental;

template <class A>
auto g(A&& x) -> stdex::success<std::unique_ptr<int>>
{
    return stdex::success<std::unique_ptr<int>> (std::forward<A>(x));
}
int main()
{
    {
        stdex::success<std::pair<int, short>> x =  stdex::make_success(std::make_pair(1,2));
        BOOST_TEST(x.value.first == 1);
        BOOST_TEST(x.value.second == 2);
    }
    {
        stdex::success<std::pair<int, short>> x {std::in_place, 1,2};
        BOOST_TEST(x.value.first == 1);
        BOOST_TEST(x.value.second == 2);
    }
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
        BOOST_TEST(x == y);
        BOOST_TEST(y.value == "");
    }
    {
        stdex::success<std::string> x =  stdex::make_success("");
        auto y = x;
        BOOST_TEST(x == y);
        BOOST_TEST(y.value == "");
    }
    {
        auto x = stdex::make_success(std::string("aaa"));
        auto y = std::move(x);
        BOOST_TEST(x != y);
        BOOST_TEST(x.value == "");
        BOOST_TEST(y.value == "aaa");
    }
#if __cplusplus > 201402L && defined __clang__
    {
        stdex::success<std::pair<int, short>> x =  stdex::success(std::make_pair(1,2));
        BOOST_TEST(x.value.first == 1);
        BOOST_TEST(x.value.second == 2);
    }
    {
        stdex::success<short> x =  stdex::success(1);
        BOOST_TEST(x.value == 1);
    }
    {
        stdex::failure<short> x =  stdex::failure(1);
        BOOST_TEST(x.value == 1);
    }
    {
        auto x =  stdex::success(std::string(""));
        static_assert(! std::is_same<decltype(x),std::string>::value, "ERROR");
        BOOST_TEST(x.value == "");
    }
    {
        stdex::success<std::string> x =  stdex::success("");
        auto y = x;
        BOOST_TEST(x == y);
        BOOST_TEST(y.value == "");
    }
    {
        auto x = stdex::success(std::string("aaa"));
        auto y = std::move(x);
        BOOST_TEST(x != y);
        BOOST_TEST(x.value == "");
        BOOST_TEST(y.value == "aaa");
    }
#endif

#if __cplusplus >= 201402L
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
        auto x = stdex::make_success(std::make_unique<int>(1));
        auto y = std::move(x);
        BOOST_TEST(*y.value == 1);
    }
    {
        auto x = stdex::make_success(std::make_unique<int>(1));
        auto y = g(std::move(x));
        BOOST_TEST(*y.value == 1);
    }
#endif
    {
        auto x = stdex::make_success(1);
        auto y = stdex::make_success(2);
        using std::swap;
        swap(x, y);
        BOOST_TEST(x == stdex::make_success(2));
        BOOST_TEST(y == stdex::make_success(1));

    }
    return ::boost::report_errors();
}
