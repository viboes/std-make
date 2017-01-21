// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/not_null.hpp
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_NOT_NULL_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_NOT_NULL_HPP

#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

#define JASEL_RETURNS(...) noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

  template <typename Ptr>
  class not_null
  {
      Ptr ptr;
  public:
      explicit not_null(Ptr p) : ptr(::std::move(p)) {}
      explicit not_null(::std::nullptr_t) = delete;
      Ptr& get_nested() { return ptr; }
      Ptr const& get_nested() const { return ptr; }
      auto operator*() const JASEL_RETURNS(*ptr)
      auto operator*()  JASEL_RETURNS(*ptr)
      auto operator->() const JASEL_RETURNS(::std::addressof(*ptr))
      auto operator->() JASEL_RETURNS(::std::addressof(*ptr))
  };

  template <typename Ptr>
  not_null<Ptr> as_not_null(Ptr p)
  {
      return not_null<Ptr>{::std::move(p)};
  }

  void as_not_null(::std::nullptr_t) = delete;

#undef JASEL_RETURNS


}
}
}

#endif // header
