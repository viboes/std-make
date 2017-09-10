// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2017 Vicente J. Botet Escriba
// Based on https://github.com/PeterSommerlad/SC22WG21_Papers/blob/master/workspace/P0052_scope_exit/src/_scope_guard_common.h#L23

#ifndef JASEL_EXPERIMENTAL_UTILITY_FINALY_HPP
#define JASEL_EXPERIMENTAL_UTILITY_FINALY_HPP

#include <utility>
#include <functional>
#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

// contribution by (c) Eric Niebler 2016, slightly adapted by Peter Sommerlad
namespace detail
{
namespace hidden
{
  template<typename T>
  constexpr conditional_t<
      (!is_nothrow_move_assignable<T>::value &&
        is_copy_assignable<T>::value),
      T const &,
      T &&>
  _move_assign_if_noexcept(T &x) noexcept
  {
      return move(x);
  }
}

  template<typename T>
  class _box
  {
      T _value;
      _box(T const &t) noexcept(noexcept(T(t)))
        : _value(t)
      {}
      _box(T &&t) noexcept(noexcept(T(move_if_noexcept(t))))
        : _value(move_if_noexcept(t))
      {}

  public:
      template<typename TT, class G
          , typename = enable_if_t<is_constructible<T, TT>::value>
      >
      explicit _box(TT &&t, G &&guard) noexcept(noexcept(_box(static_cast<T &&>(t))))
        : _box(static_cast<T &&>(t))
      {
          guard.release();
      }
      T &get() noexcept
      {
          return _value;
      }
      T const &get() const noexcept
      {
          return _value;
      }
      T &&move() noexcept
      {
          return std::move(_value);
      }
      void reset(T const &t) noexcept(noexcept(_value = t))
      {
          _value = t;
      }
      void reset(T &&t) noexcept(noexcept(_value = hidden::_move_assign_if_noexcept(t)))
      {
          _value = hidden::_move_assign_if_noexcept(t);
      }

  };

  template<typename T>
  class _box<T &>
  {
      reference_wrapper<T> _value;
  public:
      template<typename TT, class G
          , typename = enable_if_t<is_convertible<TT, T &>::value>
      >
      _box(TT &&t, G &&guard) noexcept(noexcept(static_cast<T &>(static_cast<T &&>(t))))
        : _value(static_cast<T &>(t))
      {
          guard.release();
      }
      T &get() const noexcept
      {
          return _value.get();
      }
      T &move() const noexcept
      {
          return get();
      }
      void reset(T &t) noexcept
      {
          _value = ref(t);
      }
  };

}
}}}

#endif // header
