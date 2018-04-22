//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_DETECTION_HPP
#define JASEL_V3_FUNDAMENTAL_TYPE_TRAITS_DETECTION_HPP

#include <type_traits>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/type_traits/void_t.hpp>

namespace std {

namespace experimental
{
inline  namespace fundamental_v2
{
    struct nonesuch
    {
        nonesuch() = delete;
        ~nonesuch() = delete;
        nonesuch (nonesuch const&) = delete;
        void operator=(nonesuch const&) = delete;
    };

namespace detector_detail {
    template <class Default, class AlwaysVoid, template <class...> class Op, class... Args>
    struct detector
    {
        using value_t = false_type;
        using type = Default;
    };

    template <class Default, template <class...> class Op, class... Args>
    struct detector<Default, void_t<Op<Args...>>, Op, Args...>
    {
        using value_t = true_type;
        using type = Op<Args...>;
    };
}

    template <template<class...> class Op, class... Args>
        using is_detected = typename detector_detail::detector<nonesuch, void, Op, Args...>::value_t;
    template <template<class...> class Op, class... Args>
        using detected_t = typename detector_detail::detector<nonesuch, void, Op, Args...>::type;
    #if __cplusplus >= 201402L
    template <template<class...> class Op, class... Args>
        constexpr bool is_detected_v = is_detected<Op, Args...>::value;
    #endif

    template <class Default, template<class...> class Op, class... Args>
        using detected_or = detector_detail::detector<Default, void, Op, Args...>;
    template <class Default, template<class...> class Op, class... Args>
        using detected_or_t = typename detected_or<Default, Op, Args...>::type;

    template <class Expected, template<class...> class Op, class... Args>
        using is_detected_exact = is_same<Expected, detected_t<Op, Args...>>;
    #if __cplusplus >= 201402L
    template <class Expected, template<class...> class Op, class... Args>
        constexpr bool is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;
    #endif

    template <class To, template<class...> class Op, class... Args>
        using is_detected_convertible = is_convertible<detected_t<Op, Args...>, To>;
    #if __cplusplus >= 201402L
        template <class To, template<class...> class Op, class... Args>
        constexpr bool is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;
    #endif

}
}

}

#endif // header
