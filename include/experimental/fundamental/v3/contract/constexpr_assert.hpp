// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2013,2014,2017 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_CONTRACT_CONSTEXPR_ASSERT_HPP
#define JASEL_EXPERIMENTAL_V3_CONTRACT_CONSTEXPR_ASSERT_HPP

#include <cassert>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace detail
{
  template<class Assertion>
  inline void constexpr_assert_failed(Assertion a) noexcept
  {
    a();
    //quick_exit(EXIT_FAILURE);
  }
}

  // When evaluated at compile time emits a compilation error if condition is not true.
  // Invokes the standard assert at run time.
  #define JASEL_CONSTEXPR_ASSERT(cond) \
      ((void)((cond) ? 0 : (detail::constexpr_assert_failed([](){ assert(false && #cond);}), 0)))

}
}
} // namespace


#endif // JASEL_EXPERIMENTAL_V3_EXPECTED_DETAIL_CONSTEXPR_UTILITY_HPP
