//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_VOID_HPP
#define JASEL_META_V1_VOID_HPP

#include <experimental/meta/v1/always.hpp>
#include <experimental/meta/v1/invoke.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  /// \brief An alias for `void`.
  template<typename...Ts>
  //using void_ = void;
  using void_ = invoke<always<void>, Ts...>;

}
}
}
}

#endif // header
