#ifndef BR_IO_H
#define BR_IO_H

#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <lib/def.hpp>
#include <lib/trait.hpp>
#include <lib/str.hpp>
#include <lib/exit.hpp>
#include <lib/misc.hpp>

namespace br {

	namespace detail {
		// Functions to print pointer in hex format.
		template <typename T>
		void print_ptr(T ptr) {
			ptr_t x = reinterpret_cast<ptr_t>(ptr);

			constexpr auto buffer_size = sizeof(ptr_t) * 2;
			constexpr char digits[] = "0123456789abcdef";

			char buf[buffer_size];

			index_t s = 0; // Number of skipped zeros.
			index_t l = 1; // Flag which determines if we are leading or not.

			for (index_t i = 0; i < buffer_size; i++) {
				u8_t v = (x >> ((buffer_size - i - 1) * 4)) & 0xF;
				buf[i] = digits[v];

				l = (v == 0) * l; // If we are not at the start, set leading to zero.
				s += l; // Increment skipped zeros if we are leading and `v` is zero.
			}

			// If `x` is zero, shift `s` backwards by one so that
			// we print 0. Otherwise, we would print nothing.
			s -= (x == 0);

			std::fwrite("0x", sizeof(byte_t), sizeof("0x"), stdout);
			std::fwrite(buf + s, sizeof(byte_t), buffer_size - s, stdout);
		}

		// We want to print `const char*` as a string of characters,
		// not a hex literal.
		template <> void print_ptr<const char*>(const char* str) {
			std::fwrite(str, sizeof(byte_t), std::strlen(str), stdout);
		}
	}

	namespace detail {
		template <bool SIGN, size_t DIGITS, typename T>
		void print_impl(T orig) {
			// Signed.
			if constexpr(SIGN) {
				// Count max digits in i32.
				constexpr auto md = DIGITS;
				char buf[md];

				// Fill buffer with `-` character so we don't need
				// to conditionall handle negative numbers.
				// If the number is negative, we just print `buf - 1`.
				for (index_t i = 1; i < md; i++)
					buf[i] = '-';

				// Get absolute value of `orig`.
				i32_t x = orig * ((orig > 0) - (orig < 0));

				// Setup pointer and counter that we can use
				// later to pass to fwrite.
				char* ptr = buf + md;
				size_t count = 0;

				do {
					// Move pointer backwards from end of buffer.
					// Increment count.
					ptr--, count++;

					// Pull off least significant digit and add
					// the `0` character literal to bring it into
					// ASCII range.
					*ptr = (x % 10) + '0';
					x /= 10;
				} while (x);

				// Write `count` bytes starting from `ptr`.
				// We subtract one from `ptr` if the number is
				// negative in order to include the sign.
				// We add one to the count if the number is negative
				// also so that we don't drop the last digit after
				// moving the pointer backwards.
				std::fwrite(ptr - (orig < 0), sizeof(byte_t), count + (orig < 0), stdout);
			}

			// Unsigned.
			else {
				// See above for explanation.
				constexpr auto md = DIGITS;
				char buf[md];

				char* ptr = buf + md;
				size_t count = 0;

				while (orig) {
					ptr--, count++;
					*ptr = (orig % 10) + '0';
					orig /= 10;
				}

				std::fwrite(ptr, sizeof(byte_t), count, stdout);
			}
		}
	}


	template <typename T>
	void print(T t) {
		// Check if `T` is a pointer type. If it is, we use
		// print_ptr instead to print the `t` in hex format.
		if constexpr(is_pointer_v<T> or is_null_pointer_v<T>) {
			detail::print_ptr(t);
		}

		else {
			BR_UNIMPLEMENTED();
		}
	}

	template <> void print<str_view>(str_view str) {
		std::fwrite(str.begin, sizeof(byte_t), length(str), stdout);
	}

	template <> void print<u8_t>(u8_t c) { // Also handles byte_t.
		std::fwrite(&c, sizeof(u8_t), 1, stdout);
	}

	template <> void print<i8_t>(i8_t c) {
		std::fwrite(&c, sizeof(i8_t), 1, stdout);
	}

	template <> void print<char>(char c) {
		std::fwrite(&c, sizeof(char), 1, stdout);
	}

	template <> void print<bool>(bool b) {
		std::fwrite(b ? "true" : "false", sizeof(byte_t), std::strlen(b ? "true" : "false"), stdout);
	}

	template <> void print<i16_t>(i16_t x) {
		detail::print_impl<true, count_digits(max_i16())>(x);
	}

	template <> void print<u16_t>(u16_t x) {
		detail::print_impl<false, count_digits(max_u16())>(x);
	}

	template <> void print<i32_t>(i32_t x) {
		detail::print_impl<true, count_digits(max_i32())>(x);
	}

	template <> void print<u32_t>(u32_t x) { // Also handles char_t.
		detail::print_impl<false, count_digits(max_u32())>(x);
	}

	template <> void print<i64_t>(i64_t x) {
		detail::print_impl<true, count_digits(max_i64())>(x);
	}

	template <> void print<u64_t>(u64_t x) { // Also handles char_t.
		detail::print_impl<false, count_digits(max_u64())>(x);
	}


	template <typename T1, typename T2, typename... Ts>
	void print(T1 first, T2 second, Ts... rest) {
		print(first);
		print(second);
		(print(rest), ...);
	}

	template <typename... Ts>
	void println(Ts... args) {
		(print(args), ...);
		print('\n');
	}


	namespace detail {
		str_view printfmt(str_view fmt) {
			print(fmt);
			return fmt;
		}

		template <typename T>
		str_view printfmt_impl(str_view fmt, T first) {
			str_view sv;

			while (not eof(fmt)) {
				fmt = iter_next_view(fmt, sv);

				if (eq(sv, cstr("{"))) {
					fmt = iter_next_view(fmt, sv);

					// Ensure if `}` immediately follows `{`.
					BR_ASSERT(eq(sv, cstr("}")));

					// We have found `{}`...
					print(first);

					// Break here because we want to pass off to printfmt again
					// to handle remaining placeholders.
					// Otherwise, we would print the same value for every
					// placeholder.
					break;
				}

				// Just print character.
				else {
					print(sv);
				}
			}

			return fmt;
		}

		template <typename T, typename... Ts>
		str_view printfmt(str_view fmt, T first, Ts... args) {
			fmt = detail::printfmt_impl(fmt, first);
			return printfmt(fmt, args...);
		}

		template <typename... Ts>
		str_view printlnfmt(str_view fmt, Ts... args) {
			fmt = printfmt(fmt, args...);
			println();
			return fmt;
		}
	}


	template <typename... Ts>
	void printfmt(str_view fmt, Ts... args) {
		detail::printfmt(fmt, args...);
	}

	template <typename... Ts>
	void printlnfmt(str_view fmt, Ts... args) {
		detail::printlnfmt(fmt, args...);
	}

	template <typename... Ts>
	void printfmt(const char* fmt, Ts... args) {
		printfmt(from_cstr(fmt), args...);
	}

	template <typename... Ts>
	void printlnfmt(const char* fmt, Ts... args) {
		printlnfmt(from_cstr(fmt), args...);
	}

}

#endif

