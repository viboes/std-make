//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201402L

#ifndef JASEL_FUNDAMENTAL_V3_EXPECTED_BIND_HPP
#define JASEL_FUNDAMENTAL_V3_EXPECTED_BIND_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/expected/expected.hpp>
#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v3/monad/monad.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace monad
{
template <class T, class E>
struct traits<expected<T,E>> : monad::tag
{
  template <class M, class F>
    static constexpr auto bind(M&& x, F&& f)
    {
      return x.bind(forward<F>(f));
    }
};
}
}}
}
#endif // header
#endif
