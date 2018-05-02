//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_OPTIONAL_BAD_OPTIONAL_ACCESS_HPP
#define JASEL_V3_FUNDAMENTAL_OPTIONAL_BAD_OPTIONAL_ACCESS_HPP

#if __cplusplus > 201402L && defined __clang__
#include <optional>
#endif

#include <stdexcept>

namespace std
{
  namespace experimental
  {
  inline  namespace fundamental_v3
  {
    // 20.5.8, class bad_optional_access
    class bad_optional_access : public logic_error
    {
    public:
      explicit bad_optional_access() : logic_error{"bad_optional_access"}
      {}
    };

  }
  }
}

#endif // header
