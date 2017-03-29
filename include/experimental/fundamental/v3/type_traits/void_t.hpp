//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016-2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_VOID_T_HPP
#define JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_VOID_T_HPP

#include <type_traits>
#include <experimental/meta.hpp>

namespace std {
  namespace experimental {
#if __cplusplus <= 201402L
    using std::experimental::meta::void_t;
#endif
  }
}

#endif // header
