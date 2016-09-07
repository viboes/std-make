//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_WHEN_HPP
#define JASEL_META_V1_WHEN_HPP


namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  /// \brief An alias for `void`.
  template <bool Condition>
  struct when;

  template <typename ...T>
  constexpr bool is_valid = true;
}
}
}
}

#endif // header
