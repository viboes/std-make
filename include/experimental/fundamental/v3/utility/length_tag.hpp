// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_UTILITY_LENGTH_TAG_HPP
#define JASEL_EXPERIMENTAL_UTILITY_LENGTH_TAG_HPP

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

struct length_tag_t
{
};
#if __cplusplus > 201402L
inline constexpr length_tag_t length_tag;
#endif

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
