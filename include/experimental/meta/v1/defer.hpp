//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_DEFER_HPP
#define JASEL_META_V1_DEFER_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/meta/v1/void_.hpp>
#include <experimental/meta/v1/types.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  namespace detail
  {
    template <template <typename...> class, typename, typename = void>
    struct defer
    {
    };

    template <template <typename...> class C, typename... Ts>
    struct defer<C, types<Ts...>, void_<C<Ts...>>>
    {
        using type = C<Ts...>;
    };

  }

  template <template <typename...> class C, typename... Ts>
  struct defer : detail::defer<C, types<Ts...>>
  {
  };

}
}
}
}

#endif // header
