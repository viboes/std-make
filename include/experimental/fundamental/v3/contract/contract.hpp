//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_CONTRACT_CONTRACT_HPP
#define JASEL_FUNDAMENTAL_V3_CONTRACT_CONTRACT_HPP

#include <stdexcept>
#include <cassert>
#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace contract
{
#define JASEL_ELIDE_CONTRACT_EXPECTS  ( 0 == ( JASEL_CONFIG_CONTRACT_LEVEL_MASK & 0x001 ) )
#define JASEL_ELIDE_CONTRACT_ENSURES  ( 0 == ( JASEL_CONFIG_CONTRACT_LEVEL_MASK & 0x010 ) )
#define JASEL_ELIDE_CONTRACT_ASSERTIONS  ( 0 == ( JASEL_CONFIG_CONTRACT_LEVEL_MASK & 0x100 ) )

#if JASEL_ELIDE_CONTRACT_EXPECTS
# define JASEL_EXPECTS( cond ) 0 /* JASEL_EXPECTS elided */
#elif JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V
# define JASEL_EXPECTS( cond )  \
        ((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_throw_failed([]() \
                        { \
                            throw std::experimental::contract_failed( \
                                    "JASEL: Pre-condition failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond) \
                                            ); \
                        }), 0)))

#else
# define JASEL_EXPECTS( cond ) \
        ((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_assert_failed([]() \
                        { \
                            assert(false && \
                                   "JASEL: Pre-condition failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond) \
                                           ); \
                        }), 0)))
#endif

#if JASEL_ELIDE_CONTRACT_ENSURES
# define JASEL_ENSURES( cond )  0/* JASEL_ENSURES elided */
#elif JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V
# define JASEL_ENSURES( cond )  \
        ((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_throw_failed([]() \
                        { \
                            throw std::experimental::contract_failed( \
                                    "JASEL: Post-condition failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond) \
                                    ); \
                        }), 0)))

#else
# define JASEL_ENSURES( cond ) \
        ((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_assert_failed([]() \
                        { \
                            assert(false && \
                                   "JASEL: Post-condition failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond) \
                                   ); \
                        }), 0)))
#endif


#if JASEL_ELIDE_CONTRACT_ASSERTIONS
# define JASEL_ASSERT( cond )  0/* JASEL_ASSERTS elided */
#elif JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V
# define JASEL_ASSERT( cond ) \
    ((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_throw_failed([]() \
                    { \
                        throw std::experimental::contract_failed( \
                                "JASEL: Assertion failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond) \
                                ); \
                    }), 0)))

#else
# define JASEL_ASSERT( cond ) \
    ((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_assert_failed([]() \
                    { \
                        assert(false && \
                               "JASEL: Assertion failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond) \
                                       ); \
                    }), 0)))
#endif

namespace detail
{
template<class Assertion>
inline void constexpr_assert_failed(Assertion a) noexcept
{
  a();
  //quick_exit(EXIT_FAILURE);
}
template<class Assertion>
inline void constexpr_throw_failed(Assertion a)
{
  a();
  //quick_exit(EXIT_FAILURE);
}
}


}
struct contract_failed : public std::logic_error
{
    explicit contract_failed( char const * message )
    : std::logic_error( message ) {}
};
}
}
}
#endif // header
