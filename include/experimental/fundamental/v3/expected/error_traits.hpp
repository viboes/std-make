// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2015-2016 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_EXPECTED_ERROR_TRAITS_HPP
#define JASEL_EXPERIMENTAL_V3_EXPECTED_ERROR_TRAITS_HPP

#include <experimental/fundamental/v3/expected/bad_expected_access.hpp>
//#include <boost/exception_ptr.hpp>
#include <exception>
#include <system_error>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  template <class Error>
  struct error_traits
  {
    template <class Exception>
    static Error make_error(Exception const& e)
    {
      return Error{e};
    }
    static Error make_error_from_current_exception()
    {
      try {
        throw;
      } catch (exception & e) {
        return make_error(e);
      } catch (...) {
        return Error{};
      }
    }
    static void rethrow(Error const& e)
    {
      throw bad_expected_access<Error>{e};
    }
  };

  template <>
  struct error_traits<exception_ptr>
  {
    template <class Exception>
    static exception_ptr make_error(Exception const&e)
    {
      return make_exception_ptr(e);
    }
    static exception_ptr make_error_from_current_exception()
    {
      return current_exception();
    }
    static void rethrow(exception_ptr const& e)
    {
      rethrow_exception(e);
    }
  };

  template <>
  struct error_traits<error_code>
  {
    template <class Exception>
    static error_code make_error(Exception const& e)
    {
      return error_code{e};
    }
    static error_code make_error(system_error const&e)
    {
      return e.code();
    }

    static error_code make_error_from_current_exception()
    {
      try {
        throw;
      } catch (system_error & e) {
        return make_error(e);
      } catch (...) {
        return error_code();
      }
    }
    static void rethrow(error_code const& e)
    {
      throw system_error(e);
    }
  };
}}
}
#endif
