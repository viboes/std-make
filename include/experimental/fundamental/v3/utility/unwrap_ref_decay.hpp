//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014,2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_STD_UNWRAP_REF_DECAY_HPP
#define JASEL_STD_UNWRAP_REF_DECAY_HPP

#include <functional> // reference_wrapper
#include <type_traits> //decay

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  namespace detail {
    template <class T>
    struct unwrap_ref_decay_impl {
        using type = T;
    };

    template <class T>
    struct unwrap_ref_decay_impl<reference_wrapper<T>> {
        using type = T&;
    };
  }

  template <class T>
  struct unwrap_ref_decay : detail::unwrap_ref_decay_impl<typename decay<T>::type> {};

  template <class T>
  using unwrap_ref_decay_t = typename unwrap_ref_decay<T>::type;

}
}
}


#endif // header
