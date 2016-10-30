// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP
#define JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP

#if __cplusplus >= 201402L

#include <type_traits>
#include <vector>
#include <utility>
#include <cstddef>
#include <experimental/meta.hpp>

namespace framework
{
  using std::experimental::meta::when;
  using std::experimental::meta::is_valid;
  //template <typename T>
  //size_t mem_usage(const T& v);

  namespace adl_mem_usage
  {
    template <typename T>
    typename std::enable_if<std::is_trivial<T>::value, size_t>::type
    mem_usage(const T& v)
    { return sizeof v;}

    template <typename T>
      size_t mem_usage(const std::vector<T>& v);

    template <typename T, typename U>
    size_t mem_usage(const std::pair<T, U>& v)
    {
      return mem_usage(v.first) + mem_usage(v.second);
    }

    template <typename T>
    size_t mem_usage(const std::vector<T>& v)
    {
      size_t ans = sizeof(v);
      for (const T& e : v) ans += mem_usage(e);
      ans += (v.capacity() - v.size()) * sizeof(T);
      return ans;
    }

    struct mem_usage_fn
    {
      template <typename T>
      auto operator()(const T& v)  const-> decltype(mem_usage(v))
      { return mem_usage(v);}
    };
  }
#if 1
  // To avoid ODR violations:
    template <class T>  constexpr T __static_const{};
    // std::begin is a global function object
    namespace {
      constexpr auto const & mem_usage = __static_const<adl_mem_usage::mem_usage_fn>;
    }
#else
    template <typename T>
    size_t mem_usage(const T& v)
    //-> decltype( adl_mem_usage::mem_usage_fn{}(v) )
    { return adl_mem_usage::mem_usage_fn{}(v);}
#endif
  }

#endif

#endif
