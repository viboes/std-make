//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_CONTRACT_CONTRACT_HPP
#define JASEL_FUNDAMENTAL_V3_CONTRACT_CONTRACT_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/utility/unreachable.hpp>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <stdexcept>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
class source_location {
	uint_least32_t line_number_;
	const char *   file_name_;
	const char *   function_name_;

public:
	source_location(uint_least32_t line, const char *file, const char *function)
	        : line_number_(line), file_name_(file), function_name_(function) {}
	constexpr uint_least32_t line_number() const noexcept { return line_number_; }
	constexpr const char *   file_name() const noexcept { return file_name_; }
	constexpr const char *   function_name() const noexcept { return function_name_; }
	//static constexpr source_location current() noexcept;
};

#define JASEL_CURRENT_SOURCE_LOCATION std::experimental::source_location(__LINE__, __FILE_, __PRETTY_FUNCTION__, )

class contract_violation {
	uint_least32_t line_number_;
	const char *   file_name_;
	const char *   function_name_;
	const char *   comment_;
	const char *   assertion_level_;

public:
	contract_violation(const char *kind, uint_least32_t line, const char *file, const char *function, const char *comment)
	        : line_number_(line), file_name_(file), function_name_(function) {}

	constexpr uint_least32_t line_number() const noexcept { return line_number_; }
	constexpr const char *   file_name() const noexcept { return file_name_; }
	constexpr const char *   function_name() const noexcept { return function_name_; }
	const char *             comment() const noexcept { return comment_; }
	const char *             assertion_level() const noexcept { return assertion_level_; }
};

inline void log_violation_handler(contract_violation const &cv)
{
	printf("%s failure at %s[%d]: %s", cv.assertion_level(), cv.file_name(), cv.line_number(), cv.comment());
}

inline void throw_violation_handler(contract_violation const &cv)
{
	throw cv;
}

inline void violation_handler(contract_violation const &cv)
{
#if JASEL_CONFIG_CONTRACT_VIOLATION_LOGS_V
	log_violation_handler(cv);
#endif
#if JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V
	throw_violation_handler(cv);
#endif
}

namespace contract
{
#define JASEL_CONTRACT_EXPECTS "Pre-condition"
#define JASEL_CONTRACT_ENSURES "Post-condition"
#define JASEL_CONTRACT_ASSERTS "Assertion"

#define JASEL_ELIDE_CONTRACT_EXPECTS (0 == (JASEL_CONFIG_CONTRACT_LEVEL_MASK & 0x001))
#define JASEL_ELIDE_CONTRACT_ENSURES (0 == (JASEL_CONFIG_CONTRACT_LEVEL_MASK & 0x010))
#define JASEL_ELIDE_CONTRACT_ASSERTIONS (0 == (JASEL_CONFIG_CONTRACT_LEVEL_MASK & 0x100))

#define JASEL_CONTRACT_IGNORE(KIND, cond) ((void)sizeof(decltype((cond) ? true : false)))
#define JASEL_CONTRACT_ASSUME(KIND, cond, MSG) JASEL_ASSUME(cond &&MSG)
#define JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(KIND, cond, MSG) \
	((void)((cond) ? 0 : (std::experimental::violation_handler(std::experimental::contract_violation(KIND, __LINE__, __FILE_, __PRETTY_FUNCTION__, JASEL_STRINGIFY(cond) MSG)), 0)))

#define JASEL_CONTRACT_CHECK_NEVER_CONTINUE(KIND, cond, MSG) \
	((void)((cond) ? 0 : (std::experimental::violation_handler(std::experimental::contract_violation(KIND, __LINE__, __FILE_, __PRETTY_FUNCTION__, JASEL_STRINGIFY(cond) MSG)), std::abort(), 0)))

#define JASEL_CONFIG_CONTRACT_AS_IGNORE 1
#define JASEL_CONFIG_CONTRACT_AS_ASSUME 2
#define JASEL_CONFIG_CONTRACT_AS_CHECK_MAYBE_CONTINUE 3
#define JASEL_CONFIG_CONTRACT_AS_CHECK_NEVER_CONTINUE 4

#ifndef JASEL_CONFIG_CONTRACT_DEFAULT
#if !defined NDEBUG
#define JASEL_CONFIG_CONTRACT_DEFAULT JASEL_CONFIG_CONTRACT_AS_CHECK_NEVER_CONTINUE
#else
#define JASEL_CONFIG_CONTRACT_DEFAULT JASEL_CONFIG_CONTRACT_AS_CHECK_MAYBE_CONTINUE
#endif
#endif

#ifndef JASEL_CONFIG_CONTRACT_AUDIT
#if !defined NDEBUG
#define JASEL_CONFIG_CONTRACT_AUDIT JASEL_CONFIG_CONTRACT_AS_CHECK_NEVER_CONTINUE
#else
#define JASEL_CONFIG_CONTRACT_AUDIT JASEL_CONFIG_CONTRACT_AS_IGNORE
#endif
#endif

#ifndef JASEL_CONFIG_CONTRACT_AXIOM
#define JASEL_CONFIG_CONTRACT_AXIOM JASEL_CONFIG_CONTRACT_AS_ASSUME
#endif

#define JASEL_EXPECTS_IGNORE(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_EXPECTS, cond)
#define JASEL_EXPECTS_ASSUME(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_EXPECTS, cond)
#define JASEL_EXPECTS_CHECK_MAYBE_CONTINUE(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_EXPECTS, cond, "")
#define JASEL_EXPECTS_CHECK_NEVER_CONTINUE(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_EXPECTS, cond, "")

#define JASEL_ENSURES_IGNORE(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_ENSURES, cond)
#define JASEL_ENSURES_ASSUME(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_ENSURES, cond)
#define JASEL_ENSURES_CHECK_MAYBE_CONTINUE(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_ENSURES, cond, "")
#define JASEL_ENSURES_CHECK_NEVER_CONTINUE(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_ENSURES, cond, "")

#define JASEL_ASSERTS_IGNORE(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_ASSERTS, cond)
#define JASEL_ASSERTS_ASSUME(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_ASSERTS, cond)
#define JASEL_ASSERTS_CHECK_MAYBE_CONTINUE(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_ASSERTS, cond, "")
#define JASEL_ASSERTS_CHECK_NEVER_CONTINUE(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_ASSERTS, cond, "")

#if JASEL_CONFIG_CONTRACT_DEFAULT == JASEL_CONFIG_CONTRACT_AS_IGNORE
#define JASEL_EXPECTS_DEFAULT(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_EXPECTS, cond)
#define JASEL_ENSURES_DEFAULT(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_ENSURES, cond)
#define JASEL_ASSERTS_DEFAULT(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_ASSERTS, cond)
#elif JASEL_CONFIG_CONTRACT_DEFAULT == JASEL_CONFIG_CONTRACT_AS_ASSUME
#define JASEL_EXPECTS_DEFAULT(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_EXPECTS, cond)
#define JASEL_ENSURES_DEFAULT(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_ENSURES, cond)
#define JASEL_ASSERTS_DEFAULT(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_ASSERTS, cond)
#elif JASEL_CONFIG_CONTRACT_DEFAULT == JASEL_CONFIG_CONTRACT_AS_CHECK_MAYBE_CONTINUE
#define JASEL_EXPECTS_DEFAULT(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_EXPECTS, cond, "")
#define JASEL_ENSURES_DEFAULT(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_ENSURES, cond, "")
#define JASEL_ASSERTS_DEFAULT(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_ASSERTS, cond, "")
#elif JASEL_CONFIG_CONTRACT_DEFAULT == JASEL_CONFIG_CONTRACT_AS_CHECK_NEVER_CONTINUE
#define JASEL_EXPECTS_DEFAULT(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_EXPECTS, cond, "")
#define JASEL_ENSURES_DEFAULT(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_ENSURES, cond, "")
#define JASEL_ASSERTS_DEFAULT(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_ASSERTS, cond, "")
#endif

#if JASEL_CONFIG_CONTRACT_AUDIT == JASEL_CONFIG_CONTRACT_AS_IGNORE
#define JASEL_EXPECTS_AUDIT(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_EXPECTS, cond)
#define JASEL_ENSURES_AUDIT(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_ENSURES, cond)
#define JASEL_ASSERTS_AUDIT(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_ASSERTS, cond)
#elif JASEL_CONFIG_CONTRACT_AUDIT == JASEL_CONFIG_CONTRACT_AS_ASSUME
#define JASEL_EXPECTS_AUDIT(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_EXPECTS, cond)
#define JASEL_ENSURES_AUDIT(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_ENSURES, cond)
#define JASEL_ASSERTS_AUDIT(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_ASSERTS, cond)
#elif JASEL_CONFIG_CONTRACT_AUDIT == JASEL_CONFIG_CONTRACT_AS_CHECK_MAYBE_CONTINUE
#define JASEL_EXPECTS_AUDIT(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_EXPECTS, cond, "")
#define JASEL_ENSURES_AUDIT(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_ENSURES, cond, "")
#define JASEL_ASSERTS_AUDIT(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_ASSERTS, cond, "")
#elif JASEL_CONFIG_CONTRACT_AUDIT == JASEL_CONFIG_CONTRACT_AS_CHECK_NEVER_CONTINUE
#define JASEL_EXPECTS_AUDIT(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_EXPECTS, cond, "")
#define JASEL_ENSURES_AUDIT(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_ENSURES, cond, "")
#define JASEL_ASSERTS_AUDIT(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_ASSERTS, cond, "")
#endif

#if JASEL_CONFIG_CONTRACT_AXIOM == JASEL_CONFIG_CONTRACT_AS_IGNORE
#define JASEL_EXPECTS_AXIOM(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_EXPECTS, cond)
#define JASEL_ENSURES_AXIOM(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_ENSURES, cond)
#define JASEL_ASSERTS_AXIOM(cond) JASEL_CONTRACT_IGNORE(JASEL_CONTRACT_ASSERTS, cond)
#elif JASEL_CONFIG_CONTRACT_AXIOM == JASEL_CONFIG_CONTRACT_AS_ASSUME
#define JASEL_EXPECTS_AXIOM(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_EXPECTS, cond)
#define JASEL_ENSURES_AXIOM(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_ENSURES, cond)
#define JASEL_ASSERTS_AXIOM(cond) JASEL_CONTRACT_ASSUME(JASEL_CONTRACT_ASSERTS, cond)
#elif JASEL_CONFIG_CONTRACT_AXIOM == JASEL_CONFIG_CONTRACT_AS_CHECK_MAYBE_CONTINUE
#define JASEL_EXPECTS_AXIOM(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_EXPECTS, cond, "")
#define JASEL_ENSURES_AXIOM(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_ENSURES, cond, "")
#define JASEL_ASSERTS_AXIOM(cond) JASEL_CONTRACT_CHECK_MAYBE_CONTINUE(JASEL_CONTRACT_ASSERTS, cond, "")
#elif JASEL_CONFIG_CONTRACT_AXIOM == JASEL_CONFIG_CONTRACT_AS_CHECK_NEVER_CONTINUE
#define JASEL_EXPECTS_AXIOM(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_EXPECTS, cond, "")
#define JASEL_ENSURES_AXIOM(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_ENSURES, cond, "")
#define JASEL_ASSERTS_AXIOM(cond) JASEL_CONTRACT_CHECK_NEVER_CONTINUE(JASEL_CONTRACT_ASSERTS, cond, "")
#endif

#if JASEL_ELIDE_CONTRACT_EXPECTS
#define JASEL_EXPECTS(cond) 0 /* JASEL_EXPECTS elided */
#elif JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V
#define JASEL_EXPECTS(cond)                                                                                                                   \
	((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_throw_failed([]() {                                                  \
		                      throw std::experimental::contract_failed(                                                                       \
		                              "JASEL: Pre-condition failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond)); \
	                      }),                                                                                                                 \
	                      0)))

#else
#define JASEL_EXPECTS(cond)                                                                   \
	((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_assert_failed([]() { \
		                      assert(false &&                                                 \
		                             "JASEL: Pre-condition failure: " #cond);                 \
	                      }),                                                                 \
	                      0)))
#define JASEL_EXPECTS_MSG(cond, MSG)                                                          \
	((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_assert_failed([]() { \
		                      assert(false &&                                                 \
		                             "JASEL: Pre-condition failure: " #cond MSG);             \
	                      }),                                                                 \
	                      0)))
#endif
//                                     "JASEL: Pre-condition failure: " #cond);                 
//"JASEL: Pre-condition failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond)

#if JASEL_ELIDE_CONTRACT_ENSURES
#define JASEL_ENSURES(cond) 0 /* JASEL_ENSURES elided */
#elif JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V
#define JASEL_ENSURES(cond)                                                                                                                    \
	((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_throw_failed([]() {                                                   \
		                      throw std::experimental::contract_failed(                                                                        \
		                              "JASEL: Post-condition failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond)); \
	                      }),                                                                                                                  \
	                      0)))

#else
#define JASEL_ENSURES(cond)                                                                                                                   \
	((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_assert_failed([]() {                                                 \
		                      assert(false &&                                                                                                 \
		                             "JASEL: Post-condition failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond)); \
	                      }),                                                                                                                 \
	                      0)))
#endif

#if JASEL_ELIDE_CONTRACT_ASSERTIONS
#define JASEL_ASSERT(cond) 0 /* JASEL_ASSERTS elided */
#elif JASEL_CONFIG_CONTRACT_VIOLATION_THROWS_V
#define JASEL_ASSERT(cond)                                                                                                                \
	((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_throw_failed([]() {                                              \
		                      throw std::experimental::contract_failed(                                                                   \
		                              "JASEL: Assertion failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond)); \
	                      }),                                                                                                             \
	                      0)))

#else
#define JASEL_ASSERT(cond)                                                                                                               \
	((void)((cond) ? 0 : (std::experimental::contract::detail::constexpr_assert_failed([]() {                                            \
		                      assert(false &&                                                                                            \
		                             "JASEL: Assertion failure at " __FILE__ "[" JASEL_STRINGIFY(__LINE__) "]: " JASEL_STRINGIFY(cond)); \
	                      }),                                                                                                            \
	                      0)))
#endif

namespace detail
{
template <class Assertion>
inline void constexpr_assert_failed(Assertion a) noexcept
{
	a();
	//quick_exit(EXIT_FAILURE);
}
template <class Assertion>
inline void constexpr_throw_failed(Assertion a)
{
	a();
	//quick_exit(EXIT_FAILURE);
}
} // namespace detail

} // namespace contract

struct contract_failed : public std::logic_error
{
	explicit contract_failed(char const *message)
	        : std::logic_error(message) {}
};

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
