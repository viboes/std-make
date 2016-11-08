// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_STD_UTILITYL_HPP
#define JASEL_EXAMPLE_FRAMEWORK_STD_UTILITYL_HPP

#if __cplusplus >= 201402L
#include "../mem_usage.hpp"
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace mem_usage_able
{
  template <typename R, typename S>
  struct mem_usage_traits< std::pair<R, S> >
  {
    template <typename T, typename U>
    static auto apply(const std::pair<T, U>& v) -> decltype(mem_usage_able::mem_usage(v.first)
           + mem_usage_able::mem_usage(v.second))
    {
      return mem_usage_able::mem_usage(v.first)
      + mem_usage_able::mem_usage(v.second)
      + sizeof(v)
      - sizeof(v.first)
      - sizeof(v.second);
    }
  };
}
}}}

#endif

#endif
