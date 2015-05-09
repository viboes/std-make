//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_TYPE_HPP
#define JASEL_META_V1_TYPE_HPP

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
    template < class T >
    struct type
    {
      using underlying_type = T;
    };

}
}
}
}
#endif // header
