// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2015-2016 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_EXPECTED_BAD_EXPECTED_ACCESS_HPP
#define JASEL_EXPERIMENTAL_V3_EXPECTED_BAD_EXPECTED_ACCESS_HPP

#include <stdexcept>
#include <experimental/fundamental/v3/expected2/unexpected.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  template <class Error>
  class bad_expected_access;

  // bad_expected_access base exception class.
  template <>
  class bad_expected_access<void> : public std::exception
  {
  public:
      explicit bad_expected_access() {}
      explicit bad_expected_access(const unexpected<void>& e) {}
      virtual const char* what() const BOOST_NOEXCEPT
    { return "Bad Expected Access"; }
  };

  // bad_expected_access exception class.
  template <class Error>
  class bad_expected_access : public bad_expected_access<void>
  {
    public:
      typedef Error error_type;
    private:
      unexpected<error_type> failure_value;
    public:
      explicit bad_expected_access(const unexpected<Error>& e)
      : bad_expected_access<void>()
      , failure_value(e)
      {}

      explicit bad_expected_access(const Error& e)
      : bad_expected_access<void>()
      , failure_value(e)
      {}

      error_type&& error() && { return move(failure_value.value()); }
      error_type& error() & { return failure_value.value(); }
      const error_type&& error() const&& { return move(failure_value.value()); }
      const error_type& error() const& { return failure_value.value(); }

      // todo - Add implicit/explicit conversion to error_type ?
  };

}}
}
#endif
