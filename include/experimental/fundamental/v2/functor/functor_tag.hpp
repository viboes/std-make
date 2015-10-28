//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_FUNCTOR_FUNCTOR_TAG_HPP
#define JASEL_FUNDAMENTAL_V2_FUNCTOR_FUNCTOR_TAG_HPP

#include <experimental/meta/v1/id.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  struct functor {
    template <class Tag>
    struct instance;

    template <class T>
    struct tag : meta::id <T> {};
    //struct tag : meta::id <meta::id<T>> {};
  };

}
}
}
#endif // header
