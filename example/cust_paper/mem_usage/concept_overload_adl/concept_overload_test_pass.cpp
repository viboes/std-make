// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include "framework/mem_usage.hpp"
#include "framework/Array.hpp"
#include "framework/boost/optional.hpp"
#include <tuple>

#include <iostream>

#include <boost/detail/lightweight_test.hpp>

template <class T>
struct check;

int main()
{
  int i = 0;
  framework::Array<int> a (10);
  std::vector<int> v (10);
  std::vector<std::pair<int, int>> vp;
  std::pair<std::vector<int>, std::vector<int>> pv;
  boost::optional<int> o;
  boost::optional<std::vector<int>> ov;
  std::vector<boost::optional<int>> vo;
  std::tuple<int> t (10);
  (void)t;
  boost::optional<std::tuple<int>> ot;

  static_assert(framework::is_valid<decltype(framework::concept::mem_usage(i))>, "");
  std::cout << framework::concept::mem_usage(i) << "\n";
  std::cout << framework::concept::mem_usage(a) << "\n";
  std::cout << framework::concept::mem_usage(v) << "\n";
  std::cout << framework::concept::mem_usage(o) << "\n";

#if 0
    // compile fails as expected
    decltype(framework::concept::mem_usage(t)) xx;
#endif
  std::cout << framework::concept::mem_usage(vp) << "\n";
  std::cout << framework::concept::mem_usage(pv) << "\n";
  std::cout << framework::concept::mem_usage(ov) << "\n";
  std::cout << framework::concept::mem_usage(vo) << "\n";


  {
      int i2;
      assert(framework::concept::mem_usage(i2)==4);
  }
  {
      boost::optional<int> o2;
      std::cout << framework::concept::mem_usage(o2) << "\n";
      //assert(framework::concept::mem_usage(o)==8);
  }
  {
      boost::optional<boost::optional<int>> o2;
      std::cout << framework::concept::mem_usage(o2) << "\n";
      //assert(framework::concept::mem_usage(o)==8);
  }
  {
      std::experimental::optional<int> o2;
      std::cout << framework::concept::mem_usage(o2) << "\n";
      //assert(framework::concept::mem_usage(o)==8);
  }
  {
      std::vector<std::experimental::optional<int>> o2;
      std::cout << framework::concept::mem_usage(o2) << "\n";
      //assert(framework::concept::mem_usage(o)==8);
  }
  {
      std::experimental::optional<std::vector<int>> o2;
      std::cout << framework::concept::mem_usage(o2) << "\n";
      //assert(framework::concept::mem_usage(o)==8);
  }
  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
