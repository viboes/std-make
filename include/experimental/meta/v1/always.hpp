//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_ALWAYS_HPP
#define JASEL_META_V1_ALWAYS_HPP

#include <experimental/meta/v1/eval.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  /// \brief A Metafunction Class that always returns \c T.
  template<typename T>
  struct always
  {
  private:
    // Redirect through a class template for compilers that have not
    // yet implemented CWG 1558:
    // <http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1558>
    template<typename...>
    struct impl
    {
      using type = T;
    };
  public:
    template<typename...Ts>
    using apply = eval<impl<Ts...>>;
  };

}
}
}
}

#endif // header
