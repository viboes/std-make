// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_EQUALITY_STREAMABLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_EQUALITY_STREAMABLE_HPP

#include <iosfwd>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  namespace mixin
  {
    template <class Final>
    struct streamable
    {
        //! input function.
        //! @par Effects:<br> Extracts a T from is and stores it in the passes x.
        //! @param is the input stream
        //! @param x the \c strong_id
        //! @par Returns:<br> \c is.

        template <class CharT, class Traits>
        friend
        std::basic_istream<CharT, Traits>&
        operator>>(std::basic_istream<CharT, Traits>& is, Final& x)
        {
          is >> x._backdoor()._underlying();
          return is;
        }

        //! output function.
        //! @param os the output stream
        //! @param x the \c strong_id
        //!
        //! @par Returns:<br> the result of the following expression
        //! @code
        //! os << x.undelying()
        //! @endcode

        template <class CharT, class Traits>
        friend
        std::basic_ostream<CharT, Traits>&
        operator<<(std::basic_ostream<CharT, Traits>& os, const Final& x)
        {
          return os << x.underlying();
        }
    };
  }
  namespace meta_mixin
  {
    template <class = void>
    struct streamable
    {
      template <class Final>
      using type = mixin::streamable<Final>;
    };
  }

}
}
}

#endif // header
