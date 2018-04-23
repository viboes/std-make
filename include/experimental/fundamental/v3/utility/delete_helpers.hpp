// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2018 Vicente J. Botet Escriba
//
// The design of this class follows the design of std::optional as implemented in libc++

#ifndef JASEL_EXPERIMENTAL_V3_UTILITY_DELETE_HELPERS_HPP
#define JASEL_EXPERIMENTAL_V3_UTILITY_DELETE_HELPERS_HPP

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

namespace helpers_detail
{
template <bool Can>
struct default_constructible_if {};
template <>
struct default_constructible_if<false>
{
    default_constructible_if() = delete;
    default_constructible_if(default_constructible_if const&) = default;
    default_constructible_if(default_constructible_if &&) = default;
    default_constructible_if& operator=(default_constructible_if const&) = default;
    default_constructible_if& operator=(default_constructible_if&&) = default;
};

template <bool Can>
struct copy_constructible_if {};
template <>
struct copy_constructible_if<false>
{
  copy_constructible_if() = default;
  copy_constructible_if(copy_constructible_if const&) = delete;
  copy_constructible_if(copy_constructible_if &&) = default;
  copy_constructible_if& operator=(copy_constructible_if const&) = default;
  copy_constructible_if& operator=(copy_constructible_if&&) = default;
};

template <bool Can>
struct move_constructible_if {};
template <>
struct move_constructible_if<false>
{
    move_constructible_if() = default;
    move_constructible_if(move_constructible_if const&) = default;
    move_constructible_if(move_constructible_if &&) = delete;
    move_constructible_if& operator=(move_constructible_if const&) = default;
    move_constructible_if& operator=(move_constructible_if&&) = default;
};

template <bool Can>
struct copy_assignable_if {};
template <>
struct copy_assignable_if<false>
{
    copy_assignable_if() = default;
    copy_assignable_if(copy_assignable_if const&) = default;
    copy_assignable_if(copy_assignable_if &&) = default;
    copy_assignable_if& operator=(copy_assignable_if const&) = delete;
    copy_assignable_if& operator=(copy_assignable_if&&) = default;
};

template <bool Can>
struct move_assignable_if {};
template <>
struct move_assignable_if<false>
{
    move_assignable_if() = default;
    move_assignable_if(move_assignable_if const&) = default;
    move_assignable_if(move_assignable_if &&) = default;
    move_assignable_if& operator=(move_assignable_if const&) = default;
    move_assignable_if& operator=(move_assignable_if&&) = delete;
};

}
}
}
}
#endif // header
