// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#define JASEL_UT_TRACE

#include "framework/mem_usage.hpp"
#include "framework/Array.hpp"
#include "framework/std/utility.hpp"
#include "framework/std/vector.hpp"

#include "framework/boost/optional.hpp"
#include <tuple>
#include <array>

#include <iostream>

#include <boost/detail/lightweight_test.hpp>

bool trace = false;


struct X {
  X() {}
  X(int ) {}
};

std::size_t mem_usage(X const&) {
  if (trace)
  std::cout << "X \n";
  return sizeof (X);
}

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
  struct traits<Point2D> : tag
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
  namespace mem_usage_able = std::experimental;
  {
    int i = 0;
    std::experimental::Array<int> a (10);
    std::vector<int> v (10);
    std::vector<std::pair<int, int>> vp;
    std::pair<std::vector<int>, std::vector<int>> pv;
    boost::optional<int> o;
    boost::optional<std::vector<int>> ov;
    std::vector<boost::optional<int>> vo;
    std::unique_ptr<int> up;
    boost::optional<std::tuple<int>> ot;

    static_assert(stdx::meta::is_valid<decltype(mem_usage_able::mem_usage(i))>, "");
    std::cout << mem_usage_able::mem_usage(i) << "\n";
    std::cout << mem_usage_able::mem_usage(a) << "\n";
    std::cout << mem_usage_able::mem_usage(v) << "\n";
    std::cout << mem_usage_able::mem_usage(o) << "\n";

#if 0
    // compile fails as expected
    decltype(mem_usage_able::mem_usage(up)) xx;
#endif

    std::cout << mem_usage_able::mem_usage(vp) << "\n";
    std::cout << mem_usage_able::mem_usage(pv) << "\n";
    std::cout << mem_usage_able::mem_usage(ov) << "\n";
    std::cout << mem_usage_able::mem_usage(vo) << "\n";
  }
  int i = 0;
  std::experimental::Array<int> a (10);
  std::vector<int> v (10);
  std::vector<std::pair<int, int>> vp;
  boost::optional<int> o;
  boost::optional<std::vector<int>> ov;

  std::cout << mem_usage_able::mem_usage(i) << "\n";
  std::cout << mem_usage_able::mem_usage(a) << "\n";
  std::cout << mem_usage_able::mem_usage(v) << "\n";
  std::cout << mem_usage_able::mem_usage(o) << "\n";
  std::cout << mem_usage_able::mem_usage(vp) << "\n";
  std::cout << mem_usage_able::mem_usage(ov) << "\n";

  {
    int* t;
    std::cout << "int* " << mem_usage_able::mem_usage(t) << "\n";
  }
  {
    trace = true;
    int* t=new int;
    std::cout << "int* " << mem_usage_able::mem_usage(t) << "\n";
    trace = false;
  }

#if __cplusplus > 201402L
  {
    std::tuple<int, int> t;
    std::cout <<"std::tuple<int, int> " << mem_usage_able::mem_usage(t) << "\n";
  }
  {
    std::tuple<std::vector<int>, int> t;
    std::cout << mem_usage_able::mem_usage(t) << "\n";
  }
  {
    trace = false;
    using T = std::pair<int, int>;
    static_assert(! std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << mem_usage_able::mem_usage(t) << "\n";
    trace = false;
  }
  {
    trace = false;
    typedef Point2D T;
    T t;
    std::cout << "X=== " << mem_usage_able::mem_usage(t) << "\n";
    trace = false;

  }
#if 0
  {
    // COMPILE FAIL AS EXPECTED: mem_usage for pt is been defined
    // However we don't want this behavior.
    trace = true;
    using T = std::pair<short, Point2D>;
    static_assert(! std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << "std::pair<short, std::pair<X, X>>=== " << mem_usage_able::mem_usage(t) << "\n";
    trace = false;
  }
#endif
  {
    // fixme : why this works? mem_usage for pt is been defined
    trace = false;
    using T = std::pair<short, std::pair<Point2D, X>>;
    //using T = std::pair<short, Point2D>;
    static_assert(stdx::product_type::size<std::pair<short, std::pair<X, long>>>::value == 2, "");
    static_assert(! std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << "std::pair<short, std::pair<X, X>>=== " << mem_usage_able::mem_usage(t) << "\n";
    trace = false;
  }
  {
    trace = false;
    using T = std::array<int, 3>;
    static_assert(std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << mem_usage_able::mem_usage(t) << "\n";
    trace = false;

  }
  {
    typedef int T[3];
    static_assert(std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << "int[3] " << mem_usage_able::mem_usage(t) << "\n";
  }
  {
    typedef std::pair<int,int> T[3];
    static_assert(!std::is_trivial<T>::value, "");
    static_assert(stdx::is_product_type<T>::value, "");
    T t;
    std::cout << mem_usage_able::mem_usage(t) << "\n";
  }
  {
    std::vector<std::tuple<int, int>> t;
    std::cout << mem_usage_able::mem_usage(t) << "\n";
  }
  {
    trace = false;
    std::array<std::pair<int, short>,2> t;
    std::cout << "std::array<std::pair<int, short>,2> "<< mem_usage_able::mem_usage(t) << "\n";
    trace = false;
  }
  {
    std::pair<std::array<int,2>, std::array<int,2> > t;
    std::cout << mem_usage_able::mem_usage(t) << "\n";
  }
#if 0
  {
    trace = true;
    Point2D* t=new Point2D;
    std::cout << "Point2D* " << mem_usage_able::mem_usage(t) << "\n";
    trace = false;
  }
#endif

#endif
  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
