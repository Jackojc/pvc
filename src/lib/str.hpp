#ifndef BR_STR_H
#define BR_STR_H

#include <lib/def.hpp>
#include <lib/assert.hpp>

// String view library.
// This library is value oriented and so
// mutability is rare or unused and functions
// are designed to return new views instead
// of updating the passed views.

namespace br {

	// Non-owning string view.
	struct str_view {
		const char* begin;
		const char* end;
	};

	// Convert a string literal to a str_view.
	// We can determine the size if it's a literal
	// because it hasn't decayed and so the length
	// is available to us.
	#define cstr(s) \
		(br::str_view { s, ((const char*)s) + (sizeof(s) - 1) })


	// Prototypes.
	constexpr str_view make_sv(const char* const, const char* const);
	constexpr str_view make_sv(const char* const, size_t);

	constexpr size_t length(str_view);
	constexpr size_t length(const char*);
	constexpr size_t utf_length(str_view);
	constexpr bool eq(str_view, str_view);
	constexpr bool utf_validate(str_view);
	constexpr bool eof(str_view);

	constexpr char_t as_char(str_view);
	constexpr byte_t as_byte(str_view);
	constexpr str_view as_view(str_view);

	constexpr char_t char_at(str_view, index_t);
	constexpr byte_t byte_at(str_view, index_t);
	constexpr str_view view_at(str_view, index_t);

	constexpr u8_t utf_char_length(const char* const);
	constexpr char_t utf_char_decode(const char* const, u8_t);

	constexpr str_view iter_next_char(str_view, char_t&, index_t = 1);
	constexpr str_view iter_next_byte(str_view, byte_t&, index_t = 1);
	constexpr str_view iter_next_view(str_view, str_view&, index_t = 1);

	constexpr str_view iter_prev_char(str_view, char_t&, index_t = 1);
	constexpr str_view iter_prev_byte(str_view, byte_t&, index_t = 1);
	constexpr str_view iter_prev_view(str_view, str_view&, index_t = 1);

	[[nodiscard]] constexpr const char* utf_char_next(const char* const);
	[[nodiscard]] constexpr const char* utf_char_prev(const char* const);

	[[nodiscard]] constexpr str_view next_char(str_view, index_t = 1);
	[[nodiscard]] constexpr str_view prev_char(str_view, index_t = 1);
	[[nodiscard]] constexpr str_view grow_char(str_view, index_t = 1);
	[[nodiscard]] constexpr str_view shrink_char(str_view, index_t = 1);

	[[nodiscard]] constexpr str_view next_byte(str_view, index_t = 1);
	[[nodiscard]] constexpr str_view prev_byte(str_view, index_t = 1);
	[[nodiscard]] constexpr str_view grow_byte(str_view, index_t = 1);
	[[nodiscard]] constexpr str_view shrink_byte(str_view, index_t = 1);


	// Return size by getting the absolute difference between
	// begin and end pointers.
	constexpr size_t length(str_view sv) {
		return ptrdiff(sv.begin, sv.end);
	}

	// Return length of null terminated string.
	constexpr size_t length(const char* str) {
		size_t sz = 0;

		while (*str++)
			sz++;

		return sz;
	}


	constexpr str_view make_sv(const char* const begin, const char* const end) {
		return { begin, end };
	}

	constexpr str_view make_sv(const char* const str, size_t length) {
		return { str, str + length };
	}

	// Check if 2 str_views are equal.
	// We perform a series of checks ranging from least
	// expensive to most expensive.
	// 1. Compare pointers
	// 2. Compare lengths
	// 3. Compare characters
	constexpr bool eq(str_view a, str_view b) {
		// Compare the pointers.
		if (a.begin == b.begin and a.end == b.end)
			return true;

		// Compare the length.
		if (length(a) != length(b))
			return false;

		// Compare every character.
		for (index_t i = 0; i < length(a); i++) {
			if (*(a.begin + i) != *(b.begin + i))
				return false;
		}

		return true;
	}


	// Get the size of a UTF-8 codepoint.
	// We use the first byte to determine
	// the number of bytes that follow.
	// - 1xxxxxxx = 0 bytes follow
	// - 110xxxxx = 1 bytes follow
	// - 1110xxxx = 2 bytes follow
	// - 11110xxx = 3 bytes follow
	constexpr u8_t utf_char_length(const char* const ptr) {
		// 0b1000_0000 = 0x80
		// 0b1100_0000 = 0xC0
		// 0b1110_0000 = 0xE0
		// 0b1111_0000 = 0xF0
		// 0b1111_1000 = 0xF8

		const bool vals[] = {
			(*ptr & 0x80) == 0x00,
			(*ptr & 0xE0) == 0xC0,
			(*ptr & 0xF0) == 0xE0,
			(*ptr & 0xF8) == 0xF0,
		};

		u8_t out = 0;

		for (u8_t i = 0; i < (sizeof(vals) / sizeof(bool)); ++i)
			vals[i] and (out = i);

		return out + 1;
	}

	// Return ptr advanced by one codepoint.
	constexpr const char* utf_char_next(const char* const ptr) {
		return ptr + utf_char_length(ptr);
	}

	// Return ptr reversed by one codepoint.
	constexpr const char* utf_char_prev(const char* ptr) {
		// Walk backwards one byte at a time while we
		// see continuation bytes.
		// 10xxxxxx = 0x80
		// 01xxxxxx = 0x40
		// We want the first bit to be set but _not_
		// the second bit.
		while ((*ptr & 0x80) and not (*ptr & 0x40))
			--ptr;

		return ptr;
	}

	// Decode a codepoint and return it as a br_u32.
	constexpr char_t utf_char_decode(const char* const ptr, u8_t len) {
		char_t out = (u32_t)*ptr;

		switch (len) {
			case 1:
				return out;

			case 2:
				return
					((out & 31u) << 6u) |
					((u32_t)ptr[1] & 63u);

			case 3:
				return
					((out & 15u) << 12u) |
					(((u32_t)ptr[1] & 63u) << 6u) |
					((u32_t)ptr[2] & 63u);

			case 4:
				return
					((out & 7u) << 18u) |
					(((u32_t)ptr[1] & 63u) << 12u) |
					(((u32_t)ptr[2] & 63u) << 6u) |
					((u32_t)ptr[3] & 63u);
		}

		return 0;
	}

	// Calculate the character size of a UTF-8 encoded string.
	// We iterate character by character until we reach the end
	// pointer.
	constexpr size_t utf_length(str_view sv) {
		size_t length = 0;

		for (; not eof(sv); sv = next_char(sv))
			length++;

		return length;
	}

	// Validate UTF-8 string.
	// https://bjoern.hoehrmann.de/utf-8/decoder/dfa/
	constexpr u8_t INTERNAL_UTF_TABLE__[] = {
		// The first part of the table maps bytes to character classes that
		// to reduce the size of the transition table and create bitmasks.
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		8, 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		10,3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3,  11,6, 6, 6, 5, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,

		// The second part is a transition table that maps a combination
		// of a state of the automaton and a character class to a state.
		0,  12, 24, 36, 60, 96, 84, 12, 12, 12, 48, 72,  12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 0,  12, 12, 12, 12, 12, 0,  12, 0,  12, 12,  12, 24, 12, 12, 12, 12, 12, 24, 12, 24, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 24, 12, 12, 12, 12,  12, 24, 12, 12, 12, 12, 12, 12, 12, 24, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 36, 12, 36, 12, 12,  12, 36, 12, 12, 12, 12, 12, 36, 12, 36, 12, 12,
		12, 36, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	};

	#define BR_UTF_VALID    0
	#define BR_UTF_INVALID  1

	// Validate a UTF-8 encoded string.
	constexpr bool utf_validate(str_view sv) {
		char_t state = BR_UTF_VALID;

		for (; not eof(sv); sv = next_byte(sv)) {
			char_t type = INTERNAL_UTF_TABLE__[(u8_t)as_byte(sv)];
			state = INTERNAL_UTF_TABLE__[256 + state + type];

			if (state == BR_UTF_INVALID)
				break;
		}

		return state == BR_UTF_VALID;
	}


	// Character iteration.
	constexpr str_view next_char(str_view sv, index_t n) {
		if (n == 0)
			return sv;

		// Loop to advance n characters.
		auto ptr = sv.begin;

		while (n--)
			ptr = utf_char_next(ptr);

		// Make sure we don't walk forwards beyond the end pointer.
		BR_ASSERT(ptr <= sv.end);

		return { ptr, sv.end };
	}

	constexpr str_view prev_char(str_view sv, index_t n) {
		if (n == 0)
			return sv;

		auto ptr = sv.begin;

		while (n--)
			ptr = utf_char_prev(ptr);

		return { ptr, sv.end };
	}

	constexpr str_view grow_char(str_view sv, index_t n) {
		if (n == 0)
			return sv;

		auto ptr = sv.end;

		while (n--)
			ptr = utf_char_next(ptr);

		return { sv.begin, ptr };
	}

	constexpr str_view shrink_char(str_view sv, index_t n) {
		if (n == 0)
			return sv;

		auto ptr = sv.end;

		while (n--)
			ptr = utf_char_prev(ptr);

		// Make sure we don't walk backwards beyond the beginning pointer.
		BR_ASSERT(ptr >= sv.begin);

		return { sv.begin, ptr };
	}


	// Byte iteration.
	constexpr str_view next_byte(str_view sv, index_t n) {
		return { sv.begin + n, sv.end };
	}

	constexpr str_view prev_byte(str_view sv, index_t n) {
		return { sv.begin - n, sv.end };
	}

	constexpr str_view grow_byte(str_view sv, index_t n) {
		return { sv.begin, sv.end + n };
	}

	constexpr str_view shrink_byte(str_view sv, index_t n) {
		return { sv.begin, sv.end - n };
	}


	// Mutable iteration. (decode and skip forward together)
	constexpr str_view iter_next_char(str_view sv, char_t& c, index_t i) {
		// Skip i-1 chars. If i is zero, we add 1 so we dont overflow.
		sv = next_char(sv, i - (i != 0));

		// Get current codepoint length and then decode.
		const auto len = utf_char_length(sv.begin);
		c = utf_char_decode(sv.begin, len);

		// Advance pointer.
		sv.begin += len;

		return sv;
	}

	constexpr str_view iter_next_view(str_view sv, str_view& c, index_t i) {
		// Skip i-1 chars. If i is zero, we add 1 so we dont overflow.
		sv = next_char(sv, i - (i != 0));

		// Get current codepoint length.
		const auto len = utf_char_length(sv.begin);

		// Assign before we increment pointer so
		// we get the current codepoint.
		c = { sv.begin, sv.begin + len };

		// Advance pointer.
		sv.begin += len;

		return sv;
	}

	constexpr str_view iter_next_byte(str_view sv, byte_t& c, index_t i) {
		sv = next_byte(sv, i);
		c = as_byte(sv);
		return sv;
	}


	// Mutable iteration (previous)
	// These versions don't really need to exist because
	// walking backwards doesn't require codepoint length
	// calculation but they do exist for consistancies sake.
	constexpr str_view iter_prev_char(str_view sv, char_t& c, index_t i) {
		sv = prev_char(sv, i);
		c = as_char(sv);
		return sv;
	}

	constexpr str_view iter_prev_view(str_view sv, str_view& c, index_t i) {
		sv = prev_char(sv, i);
		c = as_view(sv);
		return sv;
	}

	constexpr str_view iter_prev_byte(str_view sv, byte_t& c, index_t i) {
		sv = prev_byte(sv, i);
		c = as_byte(sv);
		return sv;
	}


	// If the size of the view is 0, it means the pointers
	// are equal and so we are at the end.
	constexpr bool eof(str_view sv) {
		return length(sv) == 0;
	}


	// Return a UTF-8 codepoint decoded to a u32 integer.
	constexpr char_t as_char(str_view sv) {
		return utf_char_decode(sv.begin, utf_char_length(sv.begin));
	}

	// Just derefence the begin pointer.
	constexpr byte_t as_byte(str_view sv) {
		return *sv.begin;
	}

	// This function returns a view of a character (2 pointers)
	// rather than decoding to an integer. This can be useful
	// if you don't care what the character is and don't want to
	// pay the cost of decoding.
	constexpr str_view as_view(str_view sv) {
		return { sv.begin, sv.begin + utf_char_length(sv.begin) };
	}


	// Indexing.
	constexpr byte_t byte_at(str_view sv, index_t i) {
		sv = next_byte(sv, i);
		return as_byte(sv);
	}

	constexpr char_t char_at(str_view sv, index_t i) {
		sv = next_char(sv, i);
		return as_char(sv);
	}

	constexpr str_view view_at(str_view sv, index_t i) {
		sv = next_char(sv, i);
		return as_view(sv);
	}

}

#endif

