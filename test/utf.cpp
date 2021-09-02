#include <cstdio>
#include <cstring>

#include <lib/def.hpp>
#include <lib/exit.hpp>
#include <lib/io.hpp>
#include <lib/assert.hpp>
#include <lib/log.hpp>
#include <lib/str.hpp>

// https://unicodemap.org/
int main(int argc, const char* argv[]) {
	BR_ASSERT(br::eq(cstr("abc"), cstr("abc")) == true);
	BR_ASSERT(br::eq(cstr("abc"), cstr("abd")) == false);

	auto str = cstr("Γαζέες");
	auto invalid = cstr("abcd\x80");

	BR_ASSERT(br::utf_length(str) == 6);
	BR_ASSERT(br::utf_validate(str) == 1);
	BR_ASSERT(br::utf_validate(invalid) == false);
	BR_ASSERT(br::length(str) == 12);

	br::char_t valid[] = { 915, 945, 950, 941, 949, 962 };

	{
		int i = 0;

		for (auto it = str; not br::eof(it); it = br::next_char(it))
			BR_ASSERT(valid[i++] == br::as_char(it));
	}

	{
		int i = 0;
		auto it = str;
		br::char_t c;

		while (not br::eof(it)) {
			it = br::iter_next_char(it, c);
			BR_ASSERT(valid[i++] == c);
		}
	}

	return br::EXIT_SUCCESS;
}

