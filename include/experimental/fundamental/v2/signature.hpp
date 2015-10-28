//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_SIGNATURE_HPP
#define JASEL_FUNDAMENTAL_V2_SIGNATURE_HPP

#include <experimental/meta/v1/types.hpp>
#include <utility>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
  template <class F, class Sig>
  class signature;

  template <class F, class R, class ...Ts>
  //requires Callable(F, R(TS...));
  class signature<F, R(Ts...)> {
  public:
    using return_type =  R;
    using argument_list_type =  meta::types<Ts...>;
    using function_type =  F;

    template <class G>
    signature(G&& g) : f(move(g)) {}

    template <class ...OTs>
    R operator()(OTs... ts)
    {
      return f(forward<OTs>(ts)...);
    }
  private:
    function_type f;

  };

  template <class R, class ...Ts, class F>
  signature<decay_t<F>, R(Ts...)> make_signature(F&& f)
  {
    return signature<decay_t<F>, R(Ts...)>(forward<F>(f));
  }
}
}
}

#endif // header
