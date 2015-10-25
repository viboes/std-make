//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_COMPOSE_HPP
#define JASEL_META_V1_COMPOSE_HPP

#include <experimental/meta/v1/apply.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  /// Compose the Alias Classes \p Fs in the parameter pack \p Ts.
  template <typename... Fs>
  struct compose
  {
  };

  template <typename F>
  struct compose<F>
  {
      template <typename... Ts>
      using apply = apply<F, Ts...>;
  };

  template <typename F0, typename... Fs>
  struct compose<F, Fs...>
  {
      template <typename... Ts>
      using apply = apply<F, apply<compose<Fs...>, Ts...>>;
  };
}
}
}
}

#endif // header
