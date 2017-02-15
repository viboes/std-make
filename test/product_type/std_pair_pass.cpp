// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include <experimental/product_type.hpp>
#include <experimental/utility.hpp>
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

      using T = std::pair<int,int>;
      T p = {0,1};

      static_assert(stde::detail::has_tuple_like_size_access<T>::value, "Hrr");
      static_assert(stde::detail::has_tuple_like_element_access<0,T>::value, "Hrr");
      static_assert(stde::detail::has_tuple_like_get_access<0,T>::value, "Hrr");
      static_assert(stde::has_tuple_like_access<T>::value, "Hrr");

      static_assert(2 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<2,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      static_assert(stde::is_product_type_v<T>, "Hrr");
      static_assert(! stde::is_product_type_v<T&>, "Hrr");

      BOOST_TEST(2 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      //auto x = stde::product_type::get<2>(p); // COMPILE FAIL AS REQUIRED

  }
  {
      using T = std::pair<int,int>;
      const T p = {0,1};
      static_assert(2 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<const int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<2,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      BOOST_TEST(2 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      //auto x = stde::product_type::get<2>(p); // COMPILE FAIL AS REQUIRED
  }
  {
    static_assert(std::is_same<int&&, decltype(stde::product_type::get<0>(std::make_pair(0,1)))>::value, "Hrr");
    BOOST_TEST(0 == stde::product_type::get<0>(std::make_pair(0,1)));
  }
  {
    using T = std::pair<int,int>;
    const T p = {0,1};
    BOOST_TEST(0 == stde::product_type::apply(f, p));
  }
  {
    using T = std::pair<int,int>;
    const T p = {0,1};
    BOOST_TEST(std::make_tuple(0,1) == stde::product_type::to_tuple(p));
  }
  {
    using T = std::pair<int,int>;
    const T p = {0,1};
    using U = std::pair<int,int>;
    const U q = {2,3};
    BOOST_TEST(std::make_tuple(0,1,2,3) == stde::product_type::cat(p, q));
  }
#if __cplusplus >= 201402L and defined JASEL_SUPPORT_SWAPPABLE
  {
    using T = std::pair<int,int>;
    T p  = {0,1};
    using U = std::pair<int,int>;
    U q  = {2,3};
    stde::swappable::swap(p,q);
    BOOST_TEST(0 == stde::product_type::get<0>(q));
    BOOST_TEST(1 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
#endif
  {
    using T = std::pair<int,int>;
    T p  = {0,1};
    auto x = stde::product_type::make_from<X>(p);
    BOOST_TEST(0 == x.i);
    BOOST_TEST(1 == x.j);
  }
  {
    using T = std::pair<int,int>;
    T p  = {1,2};
    int r=0 ;
    stde::product_type::for_each(p, [&r](auto x) {r+=x;});
    BOOST_TEST(3 == r);
  }
  {
    using T = std::pair<int,int>;
    T p  = {0,1};
    stde::product_type::for_each(p,[](auto& v) {std::cout << v << "\n";});
  }
  {
    auto to_string = [](auto x) {
        std::ostringstream ss;
        ss << x;
        return ss.str();
    };

    using T = std::pair<int,int>;
    T p  = {0,1};
    auto res = stde::product_type::p_transform(p, to_string);
    static_assert(
        std::is_same<std::pair<std::string, std::string>, decltype(res)>::value,
        "");

    BOOST_TEST(stde::product_type::p_transform(p, to_string)
      ==
        std::make_pair(std::string("0"), std::string("1"))
    );
  }
  {
    auto to_string = [](auto x) {
        std::ostringstream ss;
        ss << x;
        return ss.str();
    };

    using T = std::pair<int,int>;
    T p  = {0,1};
    auto call = std::make_pair(to_string,to_string);
    auto res = stde::product_type::n_transform(p, call);
    static_assert(
        std::is_same<std::pair<std::string, std::string>, decltype(res)>::value,
        "");

    BOOST_TEST(stde::product_type::n_transform(p, call)
      ==
        std::make_pair(std::string("0"), std::string("1"))
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
        stde::product_type::fold_left(std::make_pair(2, 5.0), "1", f)
            ==
        "f(f(1, 2), 5)"
    );
    // without initial state
    BOOST_TEST_EQ(
        stde::product_type::fold_left(std::make_pair("1", 2), f)
            ,
        "f(1, 2)"
    );
  }
  return ::boost::report_errors();
}
#else
int main() { return 0; }
#endif
