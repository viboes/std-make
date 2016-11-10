// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_BOOST_OPTIONAL_HPP
#define JASEL_EXAMPLE_FRAMEWORK_BOOST_OPTIONAL_HPP

#if __cplusplus >= 201402L
#include "../mem_usage.hpp"
#include <boost/optional.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace mem_usage_able
{
  template <typename U>
  struct traits< boost::optional<U> >
  {

    template <typename T>
    static constexpr auto apply(const boost::optional<T>& v) noexcept
    -> decltype( mem_usage_able::mem_usage(declval<T>()) )
    {
      size_t ans = sizeof(v);
      if (v) ans += mem_usage_able::mem_usage(*v) - sizeof(*v);
      return ans;
    }
//    template <typename T>
//    static constexpr auto apply(boost::optional<T>&& v) noexcept
//    -> decltype( mem_usage_able::mem_usage(declval<remove_reference_t<T>>()) )
//    {
//      size_t ans = sizeof(v);
//      if (v) ans += mem_usage_able::mem_usage(*v) - sizeof(*v);
//      return ans;
//    }
  };
}
}}}

#endif

#endif
