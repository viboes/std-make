// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include <experimental/product_type.hpp>
#include <experimental/tuple.hpp>
#include <sstream>
#include <experimental/fundamental/v3/product_type/stream_insert.hpp>

#include <boost/detail/lightweight_test.hpp>

struct Point3D {
  int x;
  int y;
  int z;
};

struct Point2D {
  int x;
  int y;
};

namespace std {
namespace experimental {
inline namespace fundamental_v3 {
namespace product_type {

  template <>
  struct traits<Point2D>
  {
    using size = integral_constant<size_t, 2>;

    template <size_t I>
    using element = meta::id<int>;

    static constexpr int& get_detail(integral_constant<size_t, 0>, Point2D & pt) noexcept
    {      return pt.x;    }
    static constexpr int const& get_detail(integral_constant<size_t, 0>, Point2D const& pt) noexcept
    {      return pt.x;    }
    static constexpr int&& get_detail(integral_constant<size_t, 0>, Point2D && pt) noexcept
    {      return move(pt).x;    }
//    static constexpr int && get_detail(integral_constant<size_t, 0>, Point2D const&& pt) noexcept
//    {      return pt.x;    }

    static constexpr int& get_detail(integral_constant<size_t, 1>, Point2D & pt) noexcept
    {      return pt.y;    }
    static constexpr int const& get_detail(integral_constant<size_t, 1>, Point2D const& pt) noexcept
    {      return pt.y;    }
    static constexpr int&& get_detail(integral_constant<size_t, 1>, Point2D && pt) noexcept
    {      return move(pt).y;    }
//    static constexpr int && get_detail(integral_constant<size_t, 1>, Point2D const&& pt) noexcept
//    {      return pt.y;    }

    template <size_t I>
      static constexpr int& get(Point2D & pt) noexcept
      {
        return get_detail(integral_constant<size_t, I>{}, pt);
      }
    template <size_t I>
      static constexpr int const& get(Point2D const& pt) noexcept
      {
        return get_detail(integral_constant<size_t, I>{}, pt);
      }
    template <size_t I>
      static constexpr auto get(Point2D && pt) noexcept
      {
        return get_detail(integral_constant<size_t, I>{}, move(pt));
      }
  };

  template <>
  struct traits<Point3D>
  {
    using size = integral_constant<size_t, 3>;

    template <size_t I>
    using element = meta::id<int>;

    static constexpr int& get_detail(integral_constant<size_t, 0>, Point3D & pt) noexcept
    {      return pt.x;    }
    static constexpr int const& get_detail(integral_constant<size_t, 0>, Point3D const& pt) noexcept
    {      return pt.x;    }
    static constexpr int&& get_detail(integral_constant<size_t, 0>, Point3D && pt) noexcept
    {      return move(pt).x;    }
//    static constexpr int && get_detail(integral_constant<size_t, 0>, Point3D const&& pt) noexcept
//    {      return pt.x;    }

    static constexpr int& get_detail(integral_constant<size_t, 1>, Point3D & pt) noexcept
    {      return pt.y;    }
    static constexpr int const& get_detail(integral_constant<size_t, 1>, Point3D const& pt) noexcept
    {      return pt.y;    }
    static constexpr int&& get_detail(integral_constant<size_t, 1>, Point3D && pt) noexcept
    {      return move(pt).y;    }
//    static constexpr int && get_detail(integral_constant<size_t, 1>, Point3D const&& pt) noexcept
//    {      return pt.y;    }

    static constexpr int& get_detail(integral_constant<size_t, 2>, Point3D & pt) noexcept
    {      return pt.z;    }
    static constexpr int const& get_detail(integral_constant<size_t, 2>, Point3D const& pt) noexcept
    {      return pt.z;    }
    static constexpr int&& get_detail(integral_constant<size_t, 2>, Point3D && pt) noexcept
    {      return move(pt).z;    }
//    static constexpr int && get_detail(integral_constant<size_t, 2>, Point3D const&& pt) noexcept
//    {      return pt.z;    }


    template <size_t I>
      static constexpr int& get(Point3D & pt) noexcept
      {
        return get_detail(integral_constant<size_t, I>{}, pt);
      }
    template <size_t I>
      static constexpr int const& get(Point3D const& pt) noexcept
      {
        return get_detail(integral_constant<size_t, I>{}, pt);
      }
    template <size_t I>
      static constexpr auto get(Point3D && pt) noexcept
      {
        return get_detail(integral_constant<size_t, I>{}, move(pt));
      }
  };
}
}}}

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
      Point3D arr = {0,1,2};
      static_assert(! stde::has_tuple_like_access<int>::value, "Hrr");
      static_assert(! stde::has_tuple_like_access<decltype(arr)>::value, "Hrr");

      static_assert(3 == stde::product_type::size<decltype(arr)>::value, "Hrr");

      static_assert(std::is_same<int, stde::product_type::element_t<0,decltype(arr)>>::value, "Hrr");
      static_assert(stde::is_product_type_v<decltype(arr)>, "Hrr");

      BOOST_TEST(3 == stde::product_type::size<decltype(arr)>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(arr));
      BOOST_TEST(1 == stde::product_type::get<1>(arr));
      BOOST_TEST(2 == stde::product_type::get<2>(arr));
      //auto x = stde::product_type::get<3>(arr); // COMPILE FAIL AS REQUIRED
      static_assert(std::is_same<int&, decltype(stde::product_type::get<0>(arr))>::value, "Hrr");
      //check<decltype(stde::product_type::get<0>(arr))> xx
  }
  {
      constexpr Point3D arr = {0,1,2};
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
      const Point3D arr = {0,1,2};
      static_assert(3 == stde::product_type::size<decltype(arr)>::value, "Hrr");

      BOOST_TEST(3 == stde::product_type::size<decltype(arr)>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(arr));
      BOOST_TEST(1 == stde::product_type::get<1>(arr));
      BOOST_TEST(2 == stde::product_type::get<2>(arr));
  }
  {
      Point3D arr = {0,1,2};
      BOOST_TEST(0 == stde::product_type::apply(f, arr));
  }
  {
      Point3D arr = {0,1,2};
      BOOST_TEST(std::make_tuple(0,1,2) == stde::product_type::to_tuple(arr));
  }
  {
    Point2D p = {0,1};
    Point2D q = {2,3};
    BOOST_TEST(std::make_tuple(0,1,2,3) == stde::product_type::cat(p, q));
  }
#if __cplusplus >= 201402L and defined JASEL_SUPPORT_SWAPPABLE
  {
    Point2D p = {0,1};
    Point2D q = {2,3};
    stde::product_type::swap(p,q);
    BOOST_TEST(0 == q.x);
    BOOST_TEST(1 == q.y);
    BOOST_TEST(2 == p.x);
    BOOST_TEST(3 == p.y);
  }
  {
    Point2D p = {0,1};
    Point2D q = {2,3};
    stde::swappable::swap(p,q);
    BOOST_TEST(0 == q.x);
    BOOST_TEST(1 == q.y);
    BOOST_TEST(2 == p.x);
    BOOST_TEST(3 == p.y);
  }
#endif
  {
    Point3D arr = {0,1,2};
    auto x = stde::product_type::make_from<X>(arr);
    BOOST_TEST(0 == x.i);
    BOOST_TEST(1 == x.j);
    BOOST_TEST(2 == x.k);
  }
  {
    Point2D p = {0,1};
    Point2D q = {2,3};
    static_assert(std::is_same<int&, decltype(stde::product_type::get<0>(p))>::value, "Hrr");
    static_assert(std::is_same<int&, decltype(stde::product_type::get<1>(p))>::value, "Hrr");

    stde::product_type::copy(q,p);
    BOOST_TEST(2 == q.x);
    BOOST_TEST(3 == q.y);
    BOOST_TEST(2 == p.x);
    BOOST_TEST(3 == p.y);
  }
  // COMPILE FAILS AS EXPECTED
#if 0
  {
    Point2D p = {0,1};
    std::string q[] = {"2","3"};
    stde::product_type::copy(q,p);
  }
#endif
  {
    Point2D p = {0,1};
    const Point2D q = {2,3};
    stde::product_type::copy(q,p);
  }
  {
    Point2D p = {0,1};
    stde::product_type::for_each(p,[](auto& v) {std::cout << v << "\n";});
  }
  {
    auto to_string = [](auto x) {
        std::ostringstream ss;
        ss << x;
        return ss.str();
    };

    Point2D p = {0,1};
    auto res = stde::product_type::p_transform<stde::tuple_tc>(p, to_string);
    static_assert(
        std::is_same<std::tuple<std::string, std::string>, decltype(res)>::value,
        "");

    BOOST_TEST(stde::product_type::p_transform<stde::tuple_tc>(p, to_string)
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

    Point3D p = {2, 3, 4};

    // with an initial state
    BOOST_TEST(
        stde::product_type::fold_left(p, "1", f)
            ==
        "f(f(f(1, 2), 3), 4)"
    );
  }
  {
    Point3D p = {2, 3, 4};
    using namespace stde::product_type::operators;
    std::cout<< p << "\n";
  }
  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
