// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include <experimental/product_type.hpp>
#include <experimental/tuple.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>


int f(int,int,int) { return 0; }

struct X {
  int i;
  int j;
  int k;
  X(int i, int j, int k) : i(i), j(j), k(k){}
};

#if 0
// fixme: We can not have rvalue to c-arrays
int[2] make_carray(int i, int j) {
  return {i,j};
}
#endif

int main()
{
  namespace stde = std::experimental;
  {
      int arr[] = {0,1,2};
      static_assert(! stde::has_tuple_like_access<int>::value, "Hrr");
      static_assert(! stde::has_tuple_like_access<decltype(arr)>::value, "Hrr");

      static_assert(3 == stde::product_type::size<decltype(arr)>::value, "Hrr");

      static_assert(std::is_same<int, stde::product_type::element_t<0,decltype(arr)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<3,decltype(arr)>>::value, "Hrr");// COMPILE FAIL AS REQUIRED
      static_assert(stde::is_product_type_v<decltype(arr)>, "Hrr");
      static_assert(! stde::is_product_type_v<int(&)[3]>, "Hrr");

      BOOST_TEST(3 == stde::product_type::size<decltype(arr)>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(arr));
      BOOST_TEST(1 == stde::product_type::get<1>(arr));
      BOOST_TEST(2 == stde::product_type::get<2>(arr));
      //auto x = stde::product_type::get<3>(arr); // COMPILE FAIL AS REQUIRED
  }
  {
      constexpr int arr[] = {0,1,2};
      static_assert(0 == stde::product_type::get<0>(arr), "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<0,decltype(arr)>>::value, "Hrr");
  }
#if 0
// We can not have rvalue to c-arrays
  {
    static_assert(std::is_same<int&&, decltype(stde::product_type::get<0>(make_carray(0,1)))>::value, "Hrr");
    BOOST_TEST(0 == stde::product_type::get<0>(make_carray(0,1)));
  }
#endif
  {
      const int arr[] = {0,1,2};
      static_assert(3 == stde::product_type::size<decltype(arr)>::value, "Hrr");

      BOOST_TEST(3 == stde::product_type::size<decltype(arr)>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(arr));
      BOOST_TEST(1 == stde::product_type::get<1>(arr));
      BOOST_TEST(2 == stde::product_type::get<2>(arr));
  }
  {
      int arr[] = {0,1,2};
      BOOST_TEST(0 == stde::product_type::apply(f, arr));
  }
  {
      int arr[] = {0,1,2};
      BOOST_TEST(std::make_tuple(0,1,2) == stde::product_type::to_tuple(arr));
  }
  {
    int p[] = {0,1};
    int q[] = {2,3};
    BOOST_TEST(std::make_tuple(0,1,2,3) == stde::product_type::cat(p, q));
  }
#if __cplusplus >= 201402L and defined JASEL_SUPPORT_SWAPPABLE
  {
    int p[] = {0,1};
    int q[] = {2,3};
    stde::swappable::swap(p,q);
    BOOST_TEST(0 == q[0]);
    BOOST_TEST(1 == q[1]);
    BOOST_TEST(2 == p[0]);
    BOOST_TEST(3 == p[1]);
  }
#endif
  {
    int arr[] = {0,1,2};
    auto x = stde::product_type::make_from_product_type<X>(arr);
    BOOST_TEST(0 == x.i);
    BOOST_TEST(1 == x.j);
    BOOST_TEST(2 == x.k);
  }
  {
    int p[] = {0,1};
    int q[] = {2,3};
    stde::product_type::copy(q,p);
    BOOST_TEST(2 == q[0]);
    BOOST_TEST(3 == q[1]);
    BOOST_TEST(2 == p[0]);
    BOOST_TEST(3 == p[1]);
  }
  // COMPILE FAILS AS EXPECTED
#if 0
  {
    int p[] = {0,1};
    std::string q[] = {"2","3"};
    stde::product_type::copy(q,p);
  }
#endif
  {
    int p[] = {0,1};
    const int q[] = {2,3};
    stde::product_type::copy(q,p);
  }
  {
    int p[] = {0,1};
    stde::product_type::for_each(p,[](auto& v) {std::cout << v << "\n";});
  }
  {
    auto to_string = [](auto x) {
        std::ostringstream ss;
        ss << x;
        return ss.str();
    };

    int p[] = {0,1};
    auto res = stde::product_type::transform<stde::tuple_tc>(p, to_string);
    static_assert(
        std::is_same<std::tuple<std::string, std::string>, decltype(res)>::value,
        "");

    BOOST_TEST(stde::product_type::transform<stde::tuple_tc>(p, to_string)
      ==
        std::make_tuple("0", "1")
    );
  }
  {
    auto to_string = [](auto x) {
        std::ostringstream ss;
        ss << x;
        return ss.str();
    };
    auto f = [=](std::string s, auto element) {
        return "f(" + s + ", " + to_string(element) + ")";
    };

    int p[] = {2, 3, 4, 5};

    // with an initial state
    BOOST_TEST(
        stde::product_type::fold_left(p, "1", f)
            ==
        "f(f(f(f(1, 2), 3), 4), 5)"
    );
    // without initial state
    std::string q[] = {"1", "2", "3", "4", "5"};
    BOOST_TEST(
        stde::product_type::fold_left(q, f)
            ==
        "f(f(f(f(1, 2), 3), 4), 5)"
    );
  }
  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
