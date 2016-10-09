//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_TYPES_HPP
#define JASEL_META_V1_TYPES_HPP

#include <tuple>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
    template < class ...Ts >
    struct types
    {};

}
}
}

  template < class ...Types >
  class tuple_size<experimental::meta::types<Types...>>
  : public integral_constant<size_t, sizeof...(Types)> { };

  template <size_t I>
  class tuple_element<I, experimental::meta::types<> >
  {
  public:
      static_assert(I == 0, "tuple_element index out of range");
      static_assert(I != 0, "tuple_element index out of range");
  };

  template <class T, class ...Ts>
  class tuple_element<0, experimental::meta::types<T, Ts...> >
  {
  public:
      typedef T type;
  };

  template <size_t I, class T, class ...Ts>
  class tuple_element<I, experimental::meta::types<T, Ts...> >
  {
  public:
      typedef typename tuple_element<I-1, experimental::meta::types<Ts...> >::type type;
  };


}

#endif // header
