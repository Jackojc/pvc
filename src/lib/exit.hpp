#ifndef BR_EXIT_H
#define BR_EXIT_H

#include <cstdio>
#include <cstdlib>

namespace br {

	#undef EXIT_FAILURE
	#undef EXIT_SUCCESS

	constexpr int EXIT_FAILURE = 1;
	constexpr int EXIT_SUCCESS = 0;

	void exit(i32_t code) {
		std::exit(code);
	}

	void abort() {
		std::abort();
	}

}

#endif

