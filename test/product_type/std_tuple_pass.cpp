// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include <experimental/product_type.hpp>
#include <experimental/tuple.hpp>
#include <experimental/functional.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

int f(int,int) { return 0; }

struct X {
  int i;
  int j;
  X(int i, int j) : i(i), j(j) {}
};

template <class PT>
bool has_padding(PT const& pt) {
  auto sum = std::experimental::product_type::fold_left(pt, 0u, [](std::size_t s, auto const& element) -> std::size_t
      {
        return s + sizeof(decltype(element));
      }
  );
  return sizeof(PT) != sum;
};

struct  SizeOf {
  template <class T>
  constexpr std::size_t operator()(T const& element)
    {
      return sizeof(element);
    }
};

struct  accumulate_sizeof {
  template <class T>
  constexpr std::size_t operator()(std::size_t s, T const& element)
    {
      return s + sizeof(element);
    }
};

template <class PT>
constexpr bool has_padding_constexpr(PT const& pt) {
  constexpr auto const sum = std::experimental::product_type::fold_left(pt, 0u, accumulate_sizeof{});
  return sizeof(PT) != sum;
};



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
  std::experimental::product_type::fold_left_index(pt, os, [](auto i, OSTREAM& os, auto const& element) -> OSTREAM&
      {
        if (decltype(i)::value>0) os << ", ";
        return os << element;
      }
  );
  return os << "}";
};
}

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
    BOOST_TEST(2 == stde::product_type::apply(
        stde::overload(
            []() {return 0;},
            [](auto) {return 1;},
            [](auto, auto) {return 2;}
        ), p));
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
    auto x = stde::product_type::make_from<X>(p);
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
    auto res = stde::product_type::p_transform(p, to_string);
    static_assert(
        std::is_same<std::tuple<std::string, std::string>, decltype(res)>::value,
        "");

    BOOST_TEST(stde::product_type::p_transform(p, to_string)
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

    using T = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const T p = {0,1};
#else
    const T p = std::make_tuple(0,1);
#endif
    auto _to_string = std::make_tuple(to_string,to_string);
    auto res = stde::product_type::n_transform(p, _to_string);
    static_assert(
        std::is_same<std::tuple<std::string, std::string>, decltype(res)>::value,
        "");

    BOOST_TEST(stde::product_type::n_transform(p, _to_string)
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
  {
    std::cout << "===========\n";
    auto q  = std::make_tuple(2, '3', 4, 5.2);
    using namespace op;

    std::cout << q<<"\n";
  }
  {
    BOOST_TEST(has_padding(std::make_tuple(2, '3', 4, 5.2)));
    BOOST_TEST(has_padding(std::make_tuple(2, '3')));
    BOOST_TEST(! has_padding(std::make_tuple(2, 3)));
    BOOST_TEST(! has_padding(std::make_tuple(2, (unsigned short)3, (unsigned short)4  )));
  }
#if not defined __clang__
  {
    constexpr auto q = std::make_tuple(2, '3', 4, 5.2);

    static_assert(has_padding_constexpr(q), "");
  }
#endif
  {
    auto q1 = std::make_tuple(2, '3', 4, 5.2);
    auto _sizeof = [](auto & x) {return sizeof(decltype(x));};
    auto call = std::make_tuple(_sizeof, _sizeof, _sizeof, _sizeof);
    auto q2 = stde::product_type::n_transform(q1, call);
    auto sum = stde::product_type::fold_left(q2, 0, [](auto x, auto y) { return x+y;});

    BOOST_TEST(sizeof(q1) > sum);
  }

  return ::boost::report_errors();
}
#else
int main() { return 0; }
#endif
