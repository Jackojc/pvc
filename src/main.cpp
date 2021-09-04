#include <lib/def.hpp>
#include <lib/assert.hpp>
#include <lib/exit.hpp>
#include <lib/log.hpp>
#include <lib/str.hpp>
#include <lib/io.hpp>

int main(int argc, const char* argv[]) {
	auto str = cstr("hello there");
	br::println(str);
	br::println("foo bar");
	br::println('a');

	for (auto it = str; not br::eof(it); it = br::next_char(it))
		br::print(br::as_byte(it));
	br::println();

	auto it = str;
	br::char_t c;

	while (not br::eof(it)) {
		it = br::iter_next_char(it, c);
		br::println(c);
	}

	br::println("-5432: ", -5432);
	br::println("123:   ", 123);
	br::println("0:     ", 0);
	br::println("1:     ", 1);
	br::println("-1:    ", -1);

	br::println("5432u:    ", 5432u);
	br::println("362842ul: ", 362842ul);
	br::println("-362842l: ", -362842l);

	br::println((void*)0x1);
	br::println((void*)0x43f848a2b);
	br::println(&argc);
	br::println(nullptr);

	br::printlnfmt("a: {}, b: {}", "1", "2");

	int a = 4;
	br::println(BR_DEBUG(a + 1));

	// BR_UNIMPLEMENTED();
	// BR_UNREACHABLE();
	// BR_TODO("fuck");
	// BR_ASSERT(1 == 2);

	return br::EXIT_SUCCESS;
}
