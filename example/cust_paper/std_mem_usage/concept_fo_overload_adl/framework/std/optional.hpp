// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_STD_OPTIONAL_HPP
#define JASEL_EXAMPLE_FRAMEWORK_STD_OPTIONAL_HPP

#if __cplusplus > 201402L
#include "../mem_usage.hpp"
#include <optional>

namespace std
{
  template <typename T>
    size_t mem_usage_impl(const optional<T>& v)
  -> decltype( std::experimental::mem_usage_able::mem_usage(*v) )
    {
      size_t ans = sizeof(v);
      if (v) ans += std::experimental::mem_usage_able::mem_usage(*v) - sizeof(*v);
      return ans;
    }
}

#endif

#endif
