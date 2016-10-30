// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include "framework/mem_usage.hpp"
#include "framework/Array.hpp"
#include "framework/boost/optional.hpp"

#include <iostream>

#include <boost/detail/lightweight_test.hpp>

int main()
{
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
    boost::optional<std::tuple<int>> ot;

    static_assert(framework::is_valid<decltype(framework::mem_usage(i))>, "");
    std::cout << framework::mem_usage(i) << "\n";
    std::cout << framework::mem_usage(a) << "\n";
    std::cout << framework::mem_usage(v) << "\n";
    std::cout << framework::mem_usage(o) << "\n";

    //static_assert( not framework::is_valid<decltype(framework::mem_usage(t))>, "");
    //static_assert( not framework::is_valid<decltype(framework::mem_usage(ot))>, "");
    //std::cout << framework::mem_usage(t) << "\n";
    //std::cout << framework::mem_usage(ot) << "\n";
    std::cout << framework::mem_usage(vp) << "\n";
    std::cout << framework::mem_usage(pv) << "\n";
    std::cout << framework::mem_usage(ov) << "\n";
    std::cout << framework::mem_usage(vo) << "\n";
  }
  int i = 0;
  framework::Array<int> a (10);
  std::vector<int> v (10);
  std::vector<std::pair<int, int>> vp;
  boost::optional<int> o;
  boost::optional<std::vector<int>> ov;

  std::cout << framework::mem_usage(i) << "\n";
  // fixme std::cout << framework::mem_usage(a) << "\n";
  std::cout << framework::mem_usage(v) << "\n";
  std::cout << framework::mem_usage(o) << "\n";
  // fixme std::cout << framework::mem_usage(vp) << "\n";
  std::cout << framework::mem_usage(ov) << "\n";

  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
