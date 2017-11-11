// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_HASHABLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_HASHABLE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/strong_type.hpp>

#include <functional>

namespace std
{
  namespace experimental
  {
    inline  namespace fundamental_v3
  {
    namespace mixin
    {
      // Credits to Jonathan Boccara from where this trick has been adopted
      // https://www.fluentcpp.com/2017/05/30/implementing-a-hash-function-for-strong-types/

      template <class Final>
      struct hashable
      {
        static constexpr bool is_hashable = true;
      };
    }
    namespace meta_mixin
    {
      template <class = void>
      struct hashable
      {
        template <class Final>
        using type = mixin::hashable<Final>;
      };
    }

    // todo: traits are specialized type by type, but we want to specialize all the types that model another concept , e.g. wrapped.
    // todo: try to use when<is_wrapped<T>>
    template <class W
    //, class = enable_if_t<is_hashable<underlying_type_t<W>{}>
    >
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

  // We can not specialize hash for a type that satisfy a condition as it has T::is_hashable and it is true
  // We need to specialize it for specific types

  // The solution is to move either to a hash specialization that has an Enabler void_t or to move to wait until hash_value overload is standard.
  // Until then we cannot define a hashable mixin to hash all the strong types with a hashable mixin.
  // However for type aliases of new_type we can always define it as we don't define a new class but an alias.

  template <
    typename Tag,
    typename UT,
    typename ...MetaMixins
  >
  struct hash<experimental::new_type<Tag, UT, MetaMixins...>>
  :
    experimental::wrapped_hash<experimental::new_type<Tag, UT, MetaMixins...>>
  {
    using enabler = enable_if_t<experimental::new_type<Tag, UT, MetaMixins...>::is_hashable, void>;
  };

}

#endif // header
