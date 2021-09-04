#ifndef BR_ASSERT_INTERNAL_H
#define BR_ASSERT_INTERNAL_H

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


	#define BR_INTERNAL_UNIMPLEMENTED() \
		do { \
			std::puts("[" __FILE__ ":" BR_STR(__LINE__) "] unimplemented!"); \
			br::exit(1); \
		} while (0)

	#ifndef BR_DISABLE_ASSERT
		#define BR_INTERNAL_ASSERT(cond) \
			do { \
				if (not (cond)) { \
					std::puts("[" __FILE__ ":" BR_STR(__LINE__) "] assertion failed: '" BR_STR(cond) "'!"); \
					br::exit(1); \
				} \
			} while (0)

	#else
		#define BR_INTERNAL_ASSERT(cond) do {} while (0)
	#endif

}

#endif
