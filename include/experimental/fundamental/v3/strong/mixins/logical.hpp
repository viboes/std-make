// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_LOGICAL_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_LOGICAL_HPP


namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  namespace mixin
  {
    template <class Final>
    struct logical
    {
        //! Forwards to the underlying value
        friend constexpr Final operator&&(Final x, Final y)  noexcept { return Final(x._backdoor()._underlying() && y._backdoor()._underlying()); }
        //! Forwards to the underlying value
        friend constexpr Final operator||(Final x, Final y)  noexcept { return Final(x._backdoor()._underlying() || y._backdoor()._underlying()); }
        //! Forwards to the underlying value
        friend constexpr Final operator!(Final x)  noexcept { return Final(! x._backdoor()._underlying()); }
    };
  }
}
}
}

#endif // header
