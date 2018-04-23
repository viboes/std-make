// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2017-2018 Vicente J. Botet Escriba
// Based on https://github.com/PeterSommerlad/SC22WG21_Papers/blob/master/workspace/P0052_scope_exit/src/_scope_guard_common.h#L23
/*
 * MIT License
Copyright (c) 2016/2017 Eric Niebler, adapted by Peter Sommerlad
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#ifndef JASEL_EXPERIMENTAL_SCOPED_BOX_HPP
#define JASEL_EXPERIMENTAL_SCOPED_BOX_HPP

#include <utility>
#include <functional>
#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

namespace scope_detail
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
