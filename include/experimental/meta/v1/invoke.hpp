//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014,2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_INVOKE_HPP
#define JASEL_META_V1_INVOKE_HPP

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  /// applies a meta-function \p TC to the arguments \p Args
  ///
  /// invoke<type_constructor_t<M>, value_type_t<M>> == M
  /// value_type_t<invoke<TC, T>> == T
  /// type_constructor_t<invoke<TC, T>> == TC

  template<class TC, class... Args>
  using invoke = typename TC::template invoke<Args...>;

}
}
}
}

#endif // header
