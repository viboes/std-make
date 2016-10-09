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

int f(int,int) { return 0; }

struct X {
  int i;
  int j;
  X(int i, int j) : i(i), j(j) {}
};

int main()
{
  namespace stde = std::experimental;
  {
      using T = std::tuple<int,int>;
      static_assert(stde::has_tuple_like_access<T>::value, "Hrr");

      T p{0,1};
      static_assert(2 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      static_assert(stde::is_product_type_v<T>, "Hrr");
      static_assert(! stde::is_product_type_v<T&>, "Hrr");

      BOOST_TEST(2 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      //auto x = stde::product_type::get<2>(p); // COMPILE FAIL AS REQUIRED
  }
  {
      using T = std::tuple<int,int>;
      const T p{0,1};
      static_assert(2 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<const int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      BOOST_TEST(2 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      //auto x = stde::product_type::get<2>(p); // COMPILE FAIL AS REQUIRED

  }
  {
    static_assert(std::is_same<int&&, decltype(stde::product_type::get<0>(std::make_tuple(0,1)))>::value, "Hrr");
    BOOST_TEST(0 == stde::product_type::get<0>(std::make_tuple(0,1)));
  }
  {
    using T = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const T p = {0,1};
#else
    const T p = std::make_tuple(0,1);
#endif
    BOOST_TEST(0 == stde::product_type::apply(f, p));
  }
  {
    using T = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const T p = {0,1};
#else
    const T p = std::make_tuple(0,1);
#endif
    BOOST_TEST(std::make_tuple(0,1) == stde::product_type::to_tuple(p));
  }
  {
    using T = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const T p = {0,1};
#else
    const T p = std::make_tuple(0,1);
#endif
    using U = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const U q = {2,3};
#else
    const U q = std::make_tuple(2,3);
#endif
    BOOST_TEST(std::make_tuple(0,1,2,3) == stde::product_type::cat(p, q));
  }
// fixme WHy this doesn't work in C++14
#if __cplusplus > 201402L and defined JASEL_SUPPORT_SWAPPABLE and defined __clang__
  {
    using T = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    T p = {0,1};
#else
    T p = std::make_tuple(0,1);
#endif
    using U = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    U q = {2,3};
#else
    U q = std::make_tuple(2,3);
#endif

    stde::product_type::swap(p,q);
    BOOST_TEST(0 == stde::product_type::get<0>(q));
    BOOST_TEST(1 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
  {
    using T = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    T p = {0,1};
#else
    T p = std::make_tuple(0,1);
#endif
    using U = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    U q = {2,3};
#else
    U q = std::make_tuple(2,3);
#endif

    stde::swappable::swap(p,q);
    BOOST_TEST(0 == stde::product_type::get<0>(q));
    BOOST_TEST(1 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
#endif
  {
    using T = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const T p = {0,1};
#else
    const T p = std::make_tuple(0,1);
#endif
    auto x = stde::product_type::make_from_product_type<X>(p);
    BOOST_TEST(0 == x.i);
    BOOST_TEST(1 == x.j);
  }

  {
    using T = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const T p = {0,1};
#else
    const T p = std::make_tuple(0,1);
#endif
    stde::product_type::for_each(p,[](auto& v) {std::cout << v << "\n";});
  }
  {
    auto to_string = [](auto x) {
        std::ostringstream ss;
        ss << x;
        return ss.str();
    };

    using T = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const T p = {0,1};
#else
    const T p = std::make_tuple(0,1);
#endif
    auto res = stde::product_type::transform(p, to_string);
    static_assert(
        std::is_same<std::tuple<std::string, std::string>, decltype(res)>::value,
        "");

    BOOST_TEST(stde::product_type::transform(p, to_string)
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
    // with an initial state
    BOOST_TEST(
        stde::product_type::fold_left(std::make_tuple(2, '3', 4, 5.0), "1", f)
            ==
        "f(f(f(f(1, 2), 3), 4), 5)"
    );
    // without initial state
    BOOST_TEST(
        stde::product_type::fold_left(std::make_tuple("1", 2, '3', 4, 5.0), f)
            ==
        "f(f(f(f(1, 2), 3), 4), 5)"
    );
  }
  return ::boost::report_errors();
}
#else
int main() { return 0; }
#endif
