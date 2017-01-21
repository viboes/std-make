// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/not_null.hpp
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_NOT_NULL_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_NOT_NULL_HPP

#include <experimental/fundamental/v2/config.hpp>

#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

  template <typename Ptr>
  class not_null
  {
      Ptr ptr;
  public:
      explicit not_null(Ptr p) : ptr(::std::move(p)) {}
      explicit not_null(::std::nullptr_t) = delete;
      Ptr& underlying() { return ptr; }
      Ptr const& underlying() const { return ptr; }
      auto operator*() const JASEL_DECLTYPE_RETURN_NOEXCEPT(*ptr)
      auto operator*()  JASEL_DECLTYPE_RETURN_NOEXCEPT(*ptr)
      auto operator->() const JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*ptr))
      auto operator->() JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*ptr))
  };

  template <typename T>
  class not_null<T*>
  {
      T* ptr;
  public:
      explicit not_null(T* p) : ptr(p) {}
      not_null(T& r) : ptr(&r) {}
      explicit not_null(::std::nullptr_t) = delete;
      T*& underlying() { return ptr; }
      T* const& underlying() const { return ptr; }
      auto operator*() const JASEL_DECLTYPE_RETURN_NOEXCEPT(*ptr)
      auto operator*()  JASEL_DECLTYPE_RETURN_NOEXCEPT(*ptr)
      auto operator->() const JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*ptr))
      auto operator->() JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*ptr))
  };

  template <typename Ptr>
  not_null<Ptr> as_not_null(Ptr p)
  {
      return not_null<Ptr>{::std::move(p)};
  }
  template <typename T>
  not_null<T*> as_not_null(T& r)
  {
      return not_null<T*>{r};
  }

  void as_not_null(::std::nullptr_t) = delete;



}
}
}

#endif // header
