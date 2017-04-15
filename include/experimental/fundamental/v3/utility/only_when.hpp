// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/only_when.hpp
// See https://github.com/akrzemi1/explicit/blob/master/doc/only_when.md
// Copyright (C) 2016 Andrzej Krzemienski.
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_ONLY_WHEN_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_ONLY_WHEN_HPP

#include <utility>
#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

template <typename T, template <typename, typename> class TypePred>
class only_when
{
  T _val;

public:
  template <typename U, JASEL_ENABLE_IF(TypePred<T, U>::value)>
    only_when (U&& v) : _val(::std::forward<U>(v)) {}

  template <typename U, JASEL_ENABLE_IF(!TypePred<T, U>::value)>
    only_when (U&&) = delete;

  T get() const { return _val; }
};


}
}
}

#endif // header
