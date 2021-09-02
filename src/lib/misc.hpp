#ifndef BR_MISC_H
#define BR_MISC_H

#include <lib/def.hpp>

namespace br {

	template <typename T>
	constexpr size_t count_digits(T x) {
		size_t count = 0;

		// Loop at least once to handle `0`.
		do {
			x = x / 10;
			count++;
		} while (x != 0);

		return count;
	}

}

#endif
