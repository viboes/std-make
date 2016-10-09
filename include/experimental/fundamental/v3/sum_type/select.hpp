//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_SUM_TYPE_SELECT_HPP
#define JASEL_FUNDAMENTAL_V3_SUM_TYPE_SELECT_HPP

#include <experimental/meta.hpp>
#include <experimental/fundamental/v3/sum_type/sum_type.hpp>
#include <experimental/utility>
#include <experimental/type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace sum_type
{

    template < class T, class Types >
    struct selector;

    template < class T, class... Types >
    struct selector<T, meta::types<Types...>>
    {
      T& value;
      using types = meta::types<Types...>;
      selector(T& v) : value(v) {}
    };

    template < class Types, class T>
    selector<std::decay_t<T>, Types> select(T& v)
    {
      return selector<std::decay_t<T>, Types>(v);
    }

    template <class T, class Types>
    struct traits_selector;

    template <class T, class ...Types>
    struct traits_selector<T, meta::types<Types...>>
    {
      using size =  tuple_size<meta::types<Types...>>;
      template <size_t I>
      using element = tuple_element<I, meta::types<Types...>>;
    };
}
}
}
}

#endif // header
