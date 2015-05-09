//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V1_META_TYPE_CONSTRUCTOR_HPP
#define JASEL_V1_META_TYPE_CONSTRUCTOR_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <type_traits> //is_base_of

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
#ifdef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR

    // type constructor tag type used to check if a class is a type constructor
    struct type_constructor_tag {};

    // type trait based on inheritance from type_constructor_tag
    // todo change for has TC::template type
    template <class TC >
    struct is_type_constructor : is_base_of<type_constructor_tag, TC> {};

    // type constructor customization point.
    // Default implementation make use of a nested type type_constructor
    template <class M >
    struct type_constructor : id<typename  M::type_constructor> {};

    // type constructor getter meta-function
    template <class M >
    using type_constructor_t = eval<type_constructor<M>>;
#endif

}
}
}
}
#endif // header
