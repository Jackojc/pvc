#ifndef BR_MISC_H
#define BR_MISC_H

#include <lib/def.hpp>

namespace br {

	// Absolute difference between 2 pointers.
	template <typename T>
	constexpr size_t ptrdiff(const T a, const T b) {
		return
			((b - a) * (b > a)) +  // b > a => b - a
			((a - b) * (a > b));   // a > b => a - b
	}


	// Branchless ternary.
	template <typename T>
	auto condition(bool cond, T a, T b) {
		return (cond * a) + (!cond * b);
	}


	// Branchless absolute function.
	template <typename T> constexpr auto abs(T v) {
		return v * ( (v > 0) - (v < 0) );
	}


	// Branchless min & max.
	template <typename T> constexpr auto min(T a, T b) {
		return condition(a < b, a, b);
	}

	template <typename T> constexpr auto max(T a, T b) {
		return condition(a > b, a, b);
	}


	// Count the number of digits in an integer (radix 10).
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


	// Get length of static array.
	template <typename T, size_t N>
	constexpr size_t length(T(&)[N]) {
		return N;
	}


	// Check if any arguments are equal.
	template <typename T, typename... Ts>
	constexpr bool any(T&& first, Ts&&... rest) {
		return ((first == rest) or ...);
	}

	// Check if all arguments are equal.
	template <typename T, typename... Ts>
	constexpr bool all(T&& first, Ts&&... rest) {
		return ((first == rest) and ...);
	}



	// Read/write data at specified index.
	template <typename T> constexpr auto at(T v, index_t i) {
		return *data(v, i);
	}

	template <typename T> constexpr auto set(T v, index_t i, T x) {
		*data(v, i) = x;
		return v;
	}



	// Access the front and back of a container with random access.
	template <typename T> constexpr auto front(T v) {
		return at(v, 0);
	}

	template <typename T> constexpr auto back(T v) {
		return at(v, length(v) - 1);
	}

}

#endif
