#ifndef BR_ASSERT_H
#define BR_ASSERT_H

#include <cstdio>

#include <lib/def.hpp>
#include <lib/exit.hpp>

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
			std::fprintf(stderr, "[%s:%s] unimplemented!\n", __FILE__, BR_STR(__LINE__)); \
			br::exit(1); \
		} while (0)

	#define BR_TODO(msg) \
		do { \
			std::fprintf(stderr, "[%s:%s] todo: %s!\n", __FILE__, BR_STR(__LINE__), (msg)); \
		} while (0)

	#ifndef BR_DISABLE_ASSERT
		#define BR_UNREACHABLE() \
			do { \
				std::fprintf(stderr, "[%s:%s] unreachable!\n", __FILE__, BR_STR(__LINE__)); \
				br::exit(1); \
			} while (0)

		#define BR_ASSERT(cond) \
			do { \
				if (not (cond)) { \
					std::fprintf(stderr, "[%s:%s] assertion failed: '%s'!\n", __FILE__, BR_STR(__LINE__), BR_STR(cond)); \
					br::exit(1); \
				} \
			} while (0)

		#define BR_STATIC_ASSERT(cond) \
			do { \
				static_assert(cond, "[" __FILE__ ":" BR_STR(__LINE__) "] assertion failed: '" #cond "'!"); \
			} while (0)

	#else
		#define BR_UNREACHABLE() do {} while (0)
		#define BR_ASSERT(cond) do {} while (0)
		#define BR_STATIC_ASSERT(cond, msg) do {} while (0)
	#endif

}

#endif
