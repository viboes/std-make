//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_OPTIONAL_NULLOPT_HPP
#define JASEL_V3_FUNDAMENTAL_OPTIONAL_NULLOPT_HPP

namespace std
{
  namespace experimental
  {
  inline  namespace fundamental_v3
  {
    // 20.5.7, Disengaged state indicator
    struct nullopt_t
    {
      struct init
      {};
      constexpr explicit nullopt_t(init) noexcept
      {}
    };
    constexpr nullopt_t nullopt
    { nullopt_t::init()};

  }
  }
  using std::experimental::nullopt_t;
  using std::experimental::nullopt;

}
#endif // header
