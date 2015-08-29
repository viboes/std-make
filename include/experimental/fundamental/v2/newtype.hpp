// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_NEWTYPE_HPP
#define JASEL_FUNDAMENTAL_V2_NEWTYPE_HPP


namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class T, class Tag, class Deriving=void>
  struct newtype
  {
    using value_type = T;
    value_type value;
    explicit newtype(value_type v) : value(v) {}
    operator value_type() const { return value;}
  };

}
}
}

#endif // header
