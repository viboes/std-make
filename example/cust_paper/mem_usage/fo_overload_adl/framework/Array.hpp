// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_ARRAY_HPP
#define JASEL_EXAMPLE_FRAMEWORK_ARRAY_HPP

#if __cplusplus >= 201402L
#include <memory>
#include "mem_usage.hpp"

namespace framework
{
  template <typename T>
  class Array
  {
    std::unique_ptr<T[]> ptr_;
    size_t size_;

  public:
    explicit Array(size_t s) : ptr_(new T [s]), size_(s) {}
    T const* begin() const { return &ptr_[0]; }
    T const* end() const { return &ptr_[size_]; }

    //template <typename T>
    friend  size_t mem_usage(const Array<T>& v)
      {
        size_t ans = sizeof(v);
        for (const T& e : v) ans += framework::mem_usage(e);
        return ans;
      }

  };


}
#endif

#endif
