// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2016 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_TYPE_TRAITS_HPP
#define JASEL_EXPERIMENTAL_TYPE_TRAITS_HPP

#include <type_traits>

#include <experimental/fundamental/v3/type_traits/conjunction.hpp>
#include <experimental/meta.hpp>

namespace std {
  namespace experimental {
#if __cplusplus > 201402L
    using std::void_t;
#else
    using std::experimental::meta::void_t;
#endif
  }
}

#endif // header
