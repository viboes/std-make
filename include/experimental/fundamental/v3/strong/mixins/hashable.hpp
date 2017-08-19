// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_HASHABLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_HASHABLE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>

#include <functional>

namespace std
{
  namespace experimental
  {
    inline  namespace fundamental_v3
  {
    namespace mixin
    {
      template <class Final>
      struct hashable
      {
        static constexpr bool is_hashable = true;
      };
    }

    // todo: traits are specialized type by type, but we want to specialize all the types that model another concept , e.g. wrapped.
    // todo: try to use when<is_wrapped<T>>
    template <class W>
    struct wrapped_hash  {
      using argument_type = W;
      using UT = underlying_type_t<W> ;
      using result_type = std::size_t;
      result_type operator()(argument_type const& s) const
      {
          return std::hash<UT>{}(underlying(s));
      }
    };
  }
}

#if 0
  // We can not specialize hash for a type that satisfy a condition as it has T::is_hashable and it is true
  // We need to specialize it for specific types
  template <>
  struct hash<XXX>
  // requires XXX::is_hashable
  :
    experimental::wrapped_hash<T>
  {
  };

  // The solution is to move either to a hash specialization that has an Enabler void_t or to move to wait until hash_value overload is standard.
  // Until then we cannot do define a hashable mixin to hash strong types.
#endif
}

#endif // header
