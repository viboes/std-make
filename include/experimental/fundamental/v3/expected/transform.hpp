//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_EXPECTED_TRANSFORM_HPP
#define JASEL_FUNDAMENTAL_V3_EXPECTED_TRANSFORM_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/expected/expected.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/functor/functor.hpp>
#include <utility>
#include <functional>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace functor
{
  template <class T, class E>
  struct traits<expected<T,E>> : functor::mcd_transform
  {
    template <class Expected, class F>
      static constexpr auto transform(Expected&& x, F&& f)
        JASEL_DECLTYPE_RETURN_NOEXCEPT(
          x.map(forward<F>(f))
        )
  };
}

}}
}
#endif // header
