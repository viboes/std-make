// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include <experimental/product_type.hpp>
#include <experimental/array.hpp>
#include <sstream>
#include <memory>

#include <boost/detail/lightweight_test.hpp>

int f(int,int,int) { return 0; }

struct X {
  int i;
  int j;
  int k;
  X(int i, int j, int k) : i(i), j(j), k(k){}
};

template <class T>
struct check;

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
#if 0
  namespace details {
    template<class> struct is_ref_wrapper : std::false_type {};
    template<class T> struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

    template<class T>
    using not_ref_wrapper = std::negation<is_ref_wrapper<std::decay_t<T>>>;

    template <class D, class...> struct return_type_helper { using type = D; };
    template <class... Types>
    struct return_type_helper<void, Types...> : std::common_type<Types...> {
        static_assert(std::conjunction_v<not_ref_wrapper<Types>...>,
                      "Types cannot contain reference_wrappers when D is void");
    };

    template <class D, class... Types>
    using return_type = std::array<typename return_type_helper<D, Types...>::type,
                                   sizeof...(Types)>;
  }

  template < class D = void, class... Types>
  constexpr details::return_type<D, Types...> make_array(Types&&... t) {
    return {std::forward<Types>(t)... };
  }
#else
std::array<int,2> make_array(int i, int j) {
  return {{i,j}};
}
#endif
}
}
}
namespace op {
struct  sepc {
  template <std::size_t I, class OSTREAM, class T>
  OSTREAM& operator()(std::integral_constant<size_t, I>, OSTREAM& os, T const& element) {
    if (I>0) os << ", ";
    return  os << element;
  }
};

template <class OSTREAM, class PT>
OSTREAM& operator<<(OSTREAM& os, PT const& pt) {
  os << "{" ;
  std::experimental::product_type::fold_left_index(pt, os, sepc{});
  return os << "}";
};
}
int main()
{
  namespace stde = std::experimental;
  auto to_string = [](auto const& x) {
      std::ostringstream ss;
      ss << x;
      return ss.str();
  };
  auto sep_to_string = [&](auto i, std::string s, auto const& element) {
      if (decltype(i)::value>0) s+= ", ";
      return  s + to_string(element) ;
  };
  auto pt_to_string = [&](auto const& pt) {
    return std::string("{") + stde::product_type::fold_left_index(pt,std::string{}, sep_to_string) + "}";
  };
  {
      using T = std::array<int,3>;
      static_assert(stde::product_type_detail::has_tuple_like_size_access<T>::value, "Hrr");
      static_assert(stde::product_type_detail::has_tuple_like_element_access<0, T>::value, "Hrr");
      static_assert(stde::product_type_detail::has_tuple_like_get_access<0, T>::value, "Hrr");
      static_assert(stde::has_tuple_like_access<T>::value, "Hrr");
      static_assert(stde::is_product_type_v<T>, "Hrr");
      static_assert(! stde::is_product_type_v<T&>, "Hrr");

      T p  = { {0,1,2} };
      static_assert(3 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      static_assert(std::is_same<int&, decltype(stde::product_type::get<0>(p))>::value, "Hrr");
      BOOST_TEST(3 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      BOOST_TEST(2 == stde::product_type::get<2>(p));
      //auto x = stde::product_type::get<3>(p); // COMPILE FAIL AS REQUIRED
  }
  {
      using T = std::array<int,3>;
      const T p  = { {0,1,2} };
      static_assert(3 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<const int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<const int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      BOOST_TEST(3 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      BOOST_TEST(2 == stde::product_type::get<2>(p));
      //auto x = stde::product_type::get<3>(p); // COMPILE FAIL AS REQUIRED
  }
  {
    static_assert(std::is_same<int&&, decltype(stde::product_type::get<0>(stde::make_array(0,1)))>::value, "Hrr");
    BOOST_TEST(0 == stde::product_type::get<0>(stde::make_array(0,1)));
  }
  {
    using T = std::array<int,3>;
    const T arr  = { {0,1,2} };
    BOOST_TEST(0 == stde::product_type::apply(f, arr));
  }
  {
      using T = std::array<int,3>;
      const T arr  = { {0,1,2} };
      BOOST_TEST(std::make_tuple(0,1,2) == stde::product_type::to_tuple(arr));
  }
  {
      using T = std::array<std::unique_ptr<int>,1>;
      T arr  = { {std::unique_ptr<int>(new int(1))} };
      BOOST_TEST(arr[0]);
      auto res = stde::product_type::to_tuple(std::move(arr));
      BOOST_TEST(! arr[0]);
      BOOST_TEST(stde::product_type::get<0>(res));
  }
  {
    using T = std::array<int,2>;
    const T p  = { {0,1} };
    using U = std::array<int,2>;
    const U q  = { {2,3} };
    BOOST_TEST(std::make_tuple(0,1,2,3) == stde::product_type::cat(p, q));
  }
#if __cplusplus >= 201402L and defined JASEL_SUPPORT_SWAPPABLE
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::array<int,2>;
    U q  = { {2,3} };
    stde::swappable::swap(p,q);
    BOOST_TEST(0 == stde::product_type::get<0>(q));
    BOOST_TEST(1 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
#endif
  {
    using T = std::array<int,3>;
    const T arr  = { {0,1,2} };
    auto x = stde::product_type::make_from<X>(arr);
    BOOST_TEST(0 == x.i);
    BOOST_TEST(1 == x.j);
    BOOST_TEST(2 == x.k);
  }
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::array<short,2>;
    U q  = { {2,3} };
    stde::product_type::copy(q,p);
    BOOST_TEST(2 == stde::product_type::get<0>(q));
    BOOST_TEST(3 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
#if 0
  // Fails as std::array doen't defines the assignment from another convertible array
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::array<short,2>;
    U q  = { {2,3} };
    p = q;
    BOOST_TEST(2 == stde::product_type::get<0>(q));
    BOOST_TEST(3 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
#endif
  {
    using T = std::array<std::unique_ptr<int>,1>;
    T p  = { {std::unique_ptr<int>(new int(1))} };
    T q;
    stde::product_type::copy(std::move(p), q);
    BOOST_TEST(! p[0]);
    BOOST_TEST(stde::product_type::get<0>(q));
  }
  // COMPILE FAILS AS EXPECTED
#if 0
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::array<int,1>;
    U q  = { {2} };
    stde::product_type::copy(q,p);
  }
#endif
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const U q = {2,3};
#else
    const U q = std::make_tuple(2,3);
#endif
    stde::product_type::copy(q,p);
    BOOST_TEST(2 == stde::product_type::get<0>(q));
    BOOST_TEST(3 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const U q  = {2,3};
#else
    const U q = std::make_tuple(2,3);
#endif
    stde::product_type::copy(q,p);
    BOOST_TEST(2 == stde::product_type::get<0>(q));
    BOOST_TEST(3 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    stde::product_type::copy(std::make_tuple(2,3), p);
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
  {
    using T = std::array<int,2>;
    T p  = { {1,2} };
    int r=0 ;
    stde::product_type::for_each(p, [&r](auto x) {r+=x;});
    BOOST_TEST(3 == r);
  }
  {
    using T = std::array<int,2>;
    T p  = { {1,2} };
    stde::product_type::for_each(p,[](auto& v) {std::cout << v << "\n";});
  }
  {
//    auto to_string = [](auto x) {
//        std::ostringstream ss;
//        ss << x;
//        return ss.str();
//    };

    using T = std::array<int,2>;
    T p  = { {0,1} };
    auto call = std::make_array(to_string,to_string);
    auto res = stde::product_type::n_transform(p, call);
    static_assert(
        std::is_same<std::array<std::string, 2>, decltype(res)>::value,
        "");

    BOOST_TEST(stde::product_type::n_transform(p, call)
      ==
        std::make_array(std::string("0"), "1")
    );
  }
  {
//    auto to_string = [](auto x) {
//        std::ostringstream ss;
//        ss << x;
//        return ss.str();
//    };

    using T = std::array<int,2>;
    T p  = { {0,1} };
    auto call = std::make_array(to_string,to_string);
    auto res = stde::product_type::n_transform(p, call);
    static_assert(
        std::is_same<std::array<std::string, 2>, decltype(res)>::value,
        "");

    using T1 = std::array<decltype(to_string),2>;
    T1 arrToString = {{to_string, to_string}};
    BOOST_TEST(stde::product_type::ap(arrToString, p)
      ==
        std::make_array(std::string("0"), "1")
    );
  }
  {
//    auto to_string = [](auto x) {
//        std::ostringstream ss;
//        ss << x;
//        return ss.str();
//    };
    auto f = [=](std::string s, auto element) {
        return "f(" + s + ", " + to_string(element) + ")";
    };
    // with an initial state
    using T = std::array<int,4>;
    T p  = { {2,3,4,5} };

    BOOST_TEST(
        stde::product_type::fold_left(p, "1", f)
            ==
        "f(f(f(f(1, 2), 3), 4), 5)"
    );
    // without initial state
    using U = std::array<std::string, 5>;
    U q  = { {"1", "2", "3", "4", "5"} };
    BOOST_TEST(
        stde::product_type::fold_left(q, f)
            ==
        "f(f(f(f(1, 2), 3), 4), 5)"
    );
  }
  {

    std::cout << "===========\n";
    using U = std::array<int, 5>;
    U q  = { {1, 2, 3, 4, 5} };
    auto x = pt_to_string(q);
    static_assert(std::is_same<std::string, decltype(x)>::value, "Error");
    std::cout << pt_to_string(q)<<"\n";
    BOOST_TEST_EQ(
        pt_to_string(q)
            ,
        "{1, 2, 3, 4, 5}"
    );
  }
  {
    std::cout << "===========\n";
    using U = std::array<int, 5>;
    U q  = { {1, 2, 3, 4, 5} };
    using namespace op;

    //std::experimental::product_type::fold_left_index(pt,std::string{}, sepc<OSTREAM, int>) ;
    std::cout << q<<"\n";
  }
  {
    std::cout << "===========\n";
    using U = std::array<int, 5>;
    U q  = { {1, 2, 3, 4, 5} };
    BOOST_TEST( ! stde::product_type::is_empty(q) );
    BOOST_TEST_EQ(stde::product_type::front(q), 1);
    BOOST_TEST_EQ(stde::product_type::back(q), 5);
    {
      BOOST_TEST_EQ(
          pt_to_string(stde::product_type::drop_back<2>(q))
              ,
          "{1, 2, 3}"
      );
    }
    {
      BOOST_TEST_EQ(
          pt_to_string(stde::product_type::drop_back<0>(q))
              ,
          "{1, 2, 3, 4, 5}"
      );
    }
#if 0
    //This fails we can not create an array<T,0> :(
    {
      BOOST_TEST_EQ(
          pt_to_string(stde::product_type::drop_back<5>(q))
              ,
          "{}"
      );
    }
#endif
    {
      BOOST_TEST_EQ(
          pt_to_string(stde::product_type::drop_front<2>(q))
              ,
          "{3, 4, 5}"
      );
    }
    {
      BOOST_TEST_EQ(
          pt_to_string(stde::product_type::drop_front<0>(q))
              ,
          "{1, 2, 3, 4, 5}"
      );
    }
    {

      std::cout << "=====================\n";
      auto a = stde::product_type::insert<2>(q, 10);
      BOOST_TEST_EQ(a[0], 1);
      BOOST_TEST_EQ(a[1], 2);
      BOOST_TEST_EQ(a[2], 10);
      BOOST_TEST_EQ(a[3], 3);
      BOOST_TEST_EQ(a[4], 4);
      BOOST_TEST_EQ(a[5], 5);
      BOOST_TEST_EQ(stde::product_type::size<decltype(a)>(), 6);
      std::cout << "=====================\n";
      // fixme
      //std::cout << pt_to_string(a) << std::endl;
//      BOOST_TEST_EQ(
//          pt_to_string(a)
//              ,
//          "{1, 2, 10, 3, 4, 5}"
//      );
    }

  }
  return ::boost::report_errors();
}
#else
int main() { return 0; }
#endif
