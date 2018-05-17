
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright Vicente J. Botet Escriba 2018.

#include <boost/detail/lightweight_test.hpp>

#include <experimental/fundamental/v3/expected2/unexpected.hpp>
#include <string>
#include <memory>

namespace stdex = std::experimental;


static_assert(std::is_trivial<stdex::unexpected<short>>::value, "Error");

template <class A>
auto g(A&& x) -> stdex::unexpected<std::unique_ptr<int>>
{
    return stdex::unexpected<std::unique_ptr<int>> (std::forward<A>(x));
}


int main()
{
    {
        stdex::unexpected<void> x;
        (void)x;
    }
#if 0
    {
        int arr[3] = {};
        stdex::unexpected<int[3]> x =  stdex::make_unexpected(arr); // compile fails as expected
    }
#endif
    {
        stdex::unexpected<std::pair<int, short>> x =  stdex::make_unexpected(std::make_pair(1,2));
        BOOST_TEST(x.value.first == 1);
        BOOST_TEST(x.value.second == 2);
    }
    {
        stdex::unexpected<std::pair<int, short>> x {stdex::in_place, 1,2};
        BOOST_TEST(x.value.first == 1);
        BOOST_TEST(x.value.second == 2);
    }
    {
        stdex::unexpected<short> x =  stdex::make_unexpected(1);
        BOOST_TEST(x.value == 1);
    }
    {
        stdex::unexpected<short> x =  stdex::make_unexpected(1);
        BOOST_TEST(x.value == 1);
    }
    {
        auto x =  stdex::make_unexpected(std::string(""));
        static_assert(! std::is_same<decltype(x),std::string>::value, "ERROR");
        BOOST_TEST(x.value == "");
    }
    {
        stdex::unexpected<std::string> x =  stdex::make_unexpected(std::string(""));
        auto y = x;
        BOOST_TEST(x == y);
        BOOST_TEST(y.value == "");
    }
    {
        stdex::unexpected<std::string> x =  stdex::make_unexpected("");
        auto y = x;
        BOOST_TEST(x == y);
        BOOST_TEST(y.value == "");
    }
    {
        auto x = stdex::make_unexpected(std::string("aaa"));
        auto y = std::move(x);
        BOOST_TEST(x != y);
        BOOST_TEST(x.value == "");
        BOOST_TEST(y.value == "aaa");
    }
#if __cplusplus > 201402L && defined __clang__
    {
        stdex::unexpected<std::pair<int, short>> x =  stdex::unexpected(std::make_pair(1,2));
        BOOST_TEST(x.value.first == 1);
        BOOST_TEST(x.value.second == 2);
    }
    {
        stdex::unexpected<short> x =  stdex::unexpected(1);
        BOOST_TEST(x.value == 1);
    }
    {
        stdex::unexpected<short> x =  stdex::unexpected(1);
        BOOST_TEST(x.value == 1);
    }
    {
        auto x =  stdex::unexpected(std::string(""));
        static_assert(! std::is_same<decltype(x),std::string>::value, "ERROR");
        BOOST_TEST(x.value == "");
    }
    {
        stdex::unexpected<std::string> x =  stdex::unexpected("");
        auto y = x;
        BOOST_TEST(x == y);
        BOOST_TEST(y.value == "");
    }
    {
        auto x = stdex::unexpected(std::string("aaa"));
        auto y = std::move(x);
        BOOST_TEST(x != y);
        BOOST_TEST(x.value == "");
        BOOST_TEST(y.value == "aaa");
    }
#endif

#if __cplusplus >= 201402L
    {
        auto x =  stdex::make_unexpected(std::make_unique<int>(1));
        static_assert(! std::is_same<decltype(x),std::unique_ptr<int>>::value, "ERROR");
        BOOST_TEST(*x.value == 1);
    }
    {
        auto x =  stdex::make_unexpected(std::make_unique<int>(1));
        auto y = std::move(x);
        BOOST_TEST(x.value == nullptr);
        BOOST_TEST(*y.value == 1);
    }
    {
        auto x =  std::make_unique<int>(1);
        auto y = stdex::make_unexpected(std::move(x));
        BOOST_TEST(*y.value == 1);
    }
    {
        auto x = stdex::make_unexpected(std::make_unique<int>(1));
        auto y = std::move(x);
        BOOST_TEST(*y.value == 1);
    }
    {
        auto x = stdex::make_unexpected(std::make_unique<int>(1));
        auto y = g(std::move(x));
        BOOST_TEST(*y.value == 1);
    }
#endif
//    {
//        auto x = stdex::make_unexpected(1);
//        auto y = stdex::make_unexpected(2);
//        using std::swap;
//        swap(x, y);
//        BOOST_TEST(x == stdex::make_unexpected(2));
//        BOOST_TEST(y == stdex::make_unexpected(1));
//
//    }
    return ::boost::report_errors();
}
