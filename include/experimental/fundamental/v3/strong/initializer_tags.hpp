// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba
#error
#ifndef JASEL_FUNDAMENTAL_V3_STRONG_INITIALIZER_TAGS_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_INITIALIZER_TAGS_HPP

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

  //! tag used to explicitly default initialize
  struct default_initialized_t
  {};
  //! tag used to explicitly don't initialize
  struct uninitialized_t
  {};
  //! tag used to explicitly zero initialize
  struct zero_initialized_t
  {};

}
}
}

#endif // header
