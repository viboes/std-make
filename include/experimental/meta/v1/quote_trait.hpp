//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_QUOTE_TRAIT_HPP
#define JASEL_META_V1_QUOTE_TRAIT_HPP

#include <experimental/meta/v1/compose.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/meta/v1/quote.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{

  template <template <class ...> class TC>
  using quote_trait = compose<quote<eval>, quote<TC>>;

}
}
}
}

#endif // header
