// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/not_null.hpp
// See https://github.com/akrzemi1/explicit/blob/master/doc/not_null.md
// Note this not_null is different from gsl::not_null.
// Copyright (C) 2016 Andrzej Krzemienski.
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_NOT_NULL_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_NOT_NULL_HPP

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
      Ptr _ptr;
  public:
      static_assert(is_assignable<Ptr&, nullptr_t>::value, "T cannot be assigned nullptr.");

      // todo add conversion from convertible to Ptr
      explicit not_null(Ptr p) : _ptr(move(p))
      {
        //Expects(_ptr != nullptr);
      }
      // todo add conversion from not_null<U>

      explicit not_null(nullptr_t) = delete;

      Ptr& underlying() { return _ptr; }
      Ptr const& underlying() const { return _ptr; }

      auto operator*() const JASEL_DECLTYPE_RETURN_NOEXCEPT(*_ptr)
      auto operator*()  JASEL_DECLTYPE_RETURN_NOEXCEPT(*_ptr)
      auto operator->() const JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*_ptr))
      auto operator->() JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*_ptr))
  };

  template <typename T>
  class not_null<T*>
  {
      T* _ptr;
  public:
      explicit not_null(T* p) : _ptr(p)
      {
        //Expects(_ptr != nullptr);
      }
      not_null(T& r) // NOLINT google-explicit-constructor
      : _ptr(&r) {}
      explicit not_null(nullptr_t) = delete;
      T*& underlying() { return _ptr; }
      T* const& underlying() const { return _ptr; }

      auto operator*() const JASEL_DECLTYPE_RETURN_NOEXCEPT(*_ptr)
      auto operator*()  JASEL_DECLTYPE_RETURN_NOEXCEPT(*_ptr)
      auto operator->() const JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*_ptr))
      auto operator->() JASEL_DECLTYPE_RETURN_NOEXCEPT(::std::addressof(*_ptr))
  };

  template <typename Ptr>
  not_null<Ptr> as_not_null(Ptr p)
  {
      return not_null<Ptr>{move(p)};
  }
  template <typename T>
  not_null<T*> as_not_null(T& r)
  {
      return not_null<T*>{r};
  }

  void as_not_null(nullptr_t) = delete;



}
}
}

#endif // header
