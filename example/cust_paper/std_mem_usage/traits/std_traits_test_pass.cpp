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
#include "framework/boost/optional.hpp"
#include "framework/Array.hpp"
#include <iostream>
#include <tuple>

#include <boost/detail/lightweight_test.hpp>


int main()
{
  using namespace std::experimental;

  int i = 0;
  Array<int> a (10);
  std::vector<int> v (10);
  std::vector<std::pair<int, int>> vp;
  std::pair<std::vector<int>, std::vector<int>> pv;
  boost::optional<int> o;
  boost::optional<std::vector<int>> ov;
  std::vector<boost::optional<int>> vo;
  std::tuple<int> t (10);
  boost::optional<std::tuple<int>> ot;

  static_assert(meta::is_valid<decltype(mem_usage_able::mem_usage(i))>, "");
  std::cout << mem_usage_able::mem_usage(i) << "\n";
  std::cout << mem_usage_able::mem_usage(a) << "\n";
  std::cout << mem_usage_able::mem_usage(v) << "\n";
  std::cout << mem_usage_able::mem_usage(o) << "\n";

#if 0
    // compile fails as expected
    decltype(mem_usage_able::mem_usage(t)) xx;
#endif

  std::cout << mem_usage_able::mem_usage(vp) << "\n";

  std::cout << mem_usage_able::mem_usage(pv) << "\n";
  std::cout << mem_usage_able::mem_usage(ov) << "\n";
  std::cout << mem_usage_able::mem_usage(vo) << "\n";
#if 1
  {
    int* t=new int;
    std::cout << "int* " << mem_usage_able::mem_usage_traits<int*>::apply(t) << "\n";
    //std::cout << "int* " << mem_usage_able::mem_usage(t) << "\n";
  }
#endif
  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
