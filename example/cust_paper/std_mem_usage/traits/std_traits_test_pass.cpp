// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include "framework/mem_usage.hpp"
#include "framework/std/utility.hpp"
#include "framework/std/vector.hpp"
#include "framework/std/optional.hpp"
#include "framework/std/product_type.hpp"
#include "framework/boost/optional.hpp"
#include "framework/Array.hpp"
#include <iostream>
#include <tuple>
#include <array>
#include "experimental/utility.hpp"

#include <boost/detail/lightweight_test.hpp>

struct Point2D {
  Point2D() {}
  Point2D(int,int) {}
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


}
}}}

int main()
{
  namespace stdx = std::experimental;

  int i = 0;
  stdx::Array<int> a (10);
  std::vector<int> v (10);
  std::vector<std::pair<int, int>> vp;
  std::pair<std::vector<int>, std::vector<int>> pv;
  boost::optional<int> o;
  boost::optional<std::vector<int>> ov;
  std::vector<boost::optional<int>> vo;
  std::tuple<int> t (10);
  (void)t;
  boost::optional<std::tuple<int>> ot;

  static_assert(stdx::meta::is_valid<decltype(stdx::mem_usage_able::mem_usage(i))>, "");
  std::cout << stdx::mem_usage_able::mem_usage(i) << "\n";
  std::cout << stdx::mem_usage_able::mem_usage(a) << "\n";
  std::cout << stdx::mem_usage_able::mem_usage(v) << "\n";
  std::cout << stdx::mem_usage_able::mem_usage(o) << "\n";

#if 0
    // compile fails as expected
    decltype(stdx::mem_usage_able::mem_usage(t)) xx;
#endif

  std::cout << stdx::mem_usage_able::mem_usage(vp) << "\n";

  std::cout << stdx::mem_usage_able::mem_usage(pv) << "\n";
  std::cout << stdx::mem_usage_able::mem_usage(vo) << "\n";
  std::cout << stdx::mem_usage_able::mem_usage(ov) << "\n";
  {
    int* t2;
    std::cout << "int* " << stdx::mem_usage_able::mem_usage(t2) << "\n";
  }
  {
    int* t2=new int;
    std::cout << "int* " << stdx::mem_usage_able::mem_usage(t2) << "\n";
  }
#if __cplusplus > 201402L
  {
    Point2D* t=new Point2D;
    std::cout << "Point2D* " << stdx::mem_usage_able::mem_usage(t) << "\n";
  }

  {
    std::tuple<int, int> t;
    std::cout <<"std::tuple<int, int> " << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    std::tuple<std::vector<int>, int> t;
    std::cout << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    using T = std::pair<int, int>;
    static_assert(! std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    typedef Point2D T;
    T t;
    std::cout << "Point2D=== " << stdx::mem_usage_able::mem_usage(t) << "\n";

  }
  {
    using T = std::pair<short, Point2D>;
    static_assert(! std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << "std::pair<short, Point2D>=== " << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    using T = std::pair<short, std::pair<Point2D, int>>;
    static_assert(! std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << "std::pair<short, std::pair<Point2D, int>> === " << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    using T = std::array<int, 3>;
    static_assert(std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << stdx::mem_usage_able::mem_usage(t) << "\n";

  }
  {
    typedef int T[3];
    static_assert(std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << "int[3] " << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    typedef std::pair<int,int> T;
    static_assert(!std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    typedef std::pair<int,int> T[3];
    static_assert(!std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    std::vector<std::tuple<int, int>> t;
    std::cout << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    std::array<std::pair<int, short>,2> t;
    std::cout << "std::array<std::pair<int, short>,2> "<< stdx::mem_usage_able::mem_usage(t) << "\n";
  }
  {
    std::pair<std::array<int,2>, std::array<int,2> > t;
    std::cout << stdx::mem_usage_able::mem_usage(t) << "\n";
  }
#endif
  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
