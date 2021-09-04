#ifndef BR_ASSERT_H
#define BR_ASSERT_H

#include <lib/def.hpp>
#include <lib/io.hpp>

namespace br {

	// BR_ENABLE_ASSERT  => Enable assertions even when NDEBUG is set.
	// BR_DISABLE_ASSERT => Disable assertions explicitly.

	#ifdef NDEBUG
		#ifndef BR_ENABLE_ASSERT
			#define BR_DISABLE_ASSERT
		#endif
	#endif


	#define BR_UNIMPLEMENTED() \
		do { \
			br::halt("[{}:{}] unimplemented!", __FILE__, BR_STR(__LINE__)); \
		} while (0)

	#define BR_TODO(msg) \
		do { \
			br::errlnfmt("[{}:{}] todo: {}!", __FILE__, BR_STR(__LINE__), (msg)); \
		} while (0)

	#ifndef BR_DISABLE_ASSERT
		#define BR_DEBUG_RUN(expr) \
			do { \
				( (expr) ); \
			} while (0)

		#define BR_UNREACHABLE() \
			do { \
				br::halt("[{}:{}] unreachable!", __FILE__, BR_STR(__LINE__)); \
			} while (0)

		#define BR_ASSERT(cond) \
			do { \
				if (not (cond)) { \
					br::halt("[{}:{}] assertion failed: '{}'!", __FILE__, BR_STR(__LINE__), BR_STR(cond)); \
				} \
			} while (0)

		#define BR_STATIC_ASSERT(cond) \
			do { \
				static_assert(cond, "[" __FILE__ ":" BR_STR(__LINE__) "] assertion failed: '" #cond "'!"); \
			} while (0)


		// We call this function in order to evaluate `expr` only once in case
		// it has side effects. If we used a macro for this, it would be evaluated
		// twice. Once for printing the result and once for returning it.
		namespace detail {
			template <typename T>
			decltype(auto) print_debug_impl(const char* file, const char* line, const char* expr_s, T&& expr) {
				br::printlnfmt("[{}:{}] {} = {}", file, line, expr_s, expr);
				return expr;
			}
		}

		#define BR_DEBUG(expr) \
			( br::detail::print_debug_impl(__FILE__, BR_STR(__LINE__), BR_STR(expr), (expr)) )

	#else
		#define BR_DEBUG_RUN(expr) do {} while (0)
		#define BR_UNREACHABLE() do {} while (0)
		#define BR_ASSERT(cond) do {} while (0)
		#define BR_STATIC_ASSERT(cond, msg) do {} while (0)
		#define BR_DEBUG(expr) ( (expr) )
	#endif

}

#endif
