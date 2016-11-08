// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_STD_VECTOR_HPP
#define JASEL_EXAMPLE_FRAMEWORK_STD_VECTOR_HPP

#if __cplusplus >= 201402L
#include "../mem_usage.hpp"
#include <vector>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace mem_usage_able
{
  template <typename U>
  struct mem_usage_traits< std::vector<U>>
  {
    template <typename T>
    static auto apply(const std::vector<T>& v) -> decltype(mem_usage_able::mem_usage(std::declval<T>()))
    {
      size_t ans = sizeof(v);
      for (const T& e : v) ans += mem_usage_able::mem_usage(e);
      ans += (v.capacity() - v.size()) * sizeof(T);
      return ans;
    }
  };
}
}}}

#endif

#endif
