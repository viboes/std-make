//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_DECAY_UNWRAP_HPP
#define JASEL_META_V1_DECAY_UNWRAP_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <functional> // reference_wrapper
#include <type_traits> //decay

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  namespace detail {
    template <class T>
    struct decay_unwrap_impl : id<T> {};

    template <class T>
    struct decay_unwrap_impl<reference_wrapper<T>> : id<T&> {};
  }

  template <class T>
  struct decay_unwrap : detail::decay_unwrap_impl<eval<decay<T>>> {};

  template <class T>
  using decay_unwrap_t = eval<decay_unwrap<T>>;

}
}
}
}

#endif // header
