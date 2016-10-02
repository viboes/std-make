//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_RESULT_TYPE_HPP
#define JASEL_META_V1_RESULT_TYPE_HPP

#include <type_traits>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
    template < class F, class ...Args >
    using result_type = result_of<F(Args...)>;

    template < class F, class ...Args >
    using result_type_t = eval<result_type<F, Args...> >;
    template < class F, class ...Args >
    using ResultType = eval<result_type<F, Args...> >;
}
}
}
}
#endif // header
