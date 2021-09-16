#include <lib/svec.hpp>
#include <lib/def.hpp>
#include <lib/assert.hpp>
#include <lib/exit.hpp>
#include <lib/misc.hpp>
#include <lib/str.hpp>
#include <lib/mem.hpp>
#include <lib/print.hpp>
#include <lib/file.hpp>
#include <lib/vec.hpp>
#include <lib/log.hpp>
#include <lib/debug.hpp>

int main(int argc, const char* argv[]) {
	// auto str = cstr("hello there");
	// br::println(str);
	// br::println("foo bar");
	// br::println('a');

	// for (auto it = str; not br::eof(it); it = br::next_char(it))
	// 	br::print(br::as_byte(it));
	// br::println();

	// auto it = str;
	// br::char_t c;

	// while (not br::eof(it)) {
	// 	it = br::iter_next_char(it, c);
	// 	br::println(c);
	// }

	// br::println("-5432: ", -5432);
	// br::println("123:   ", 123);
	// br::println("0:     ", 0);
	// br::println("1:     ", 1);
	// br::println("-1:    ", -1);

	// br::println("5432u:    ", 5432u);
	// br::println("362842ul: ", 362842ul);
	// br::println("-362842l: ", -362842l);

	// br::println((void*)0x1);
	// br::println((void*)0x43f848a2b);
	// br::println(&argc);
	// br::println(nullptr);

	// br::printlnfmt("a: {}, b: {}", "1", "2");

	// int a = 4;
	// br::println(BR_DEBUG(a + 1));

	// BR_UNIMPLEMENTED();
	// BR_UNREACHABLE();
	// BR_TODO("fuck");
	// BR_ASSERT(1 == 2);

	// BR_LOG(br::LOG_LEVEL_INFO, "foo");
	// BR_LOG(br::LOG_LEVEL_WARN, "foo");
	// BR_LOG(br::LOG_LEVEL_ERROR, "foo");
	// BR_LOG(br::LOG_LEVEL_SUCCESS, "foo");
	// BR_LOG(br::LOG_LEVEL_SUCCESS);

	// int a[10];
	// br::println(br::length(a));

	{
		auto v = br::make_svec<int, 10>();

		for (br::index_t i = 0; i < 10; i++) {
			v = br::emplace(v, i + 1);
		}

		// br::printlnfmt("cap: {}, length: {}", br::capacity(v), br::length(v));
		br::println(v);
	}

	auto v = br::make_vec(3, 2, 1);

	BR_DEFER(v = br::destroy_vec(v)) {
		for (br::index_t i = 0; i < 10; i++) {
			v = br::emplace(v, i + 1);
		}

		br::printlnfmt("cap: {}, length: {}", br::capacity(v), br::length(v));
		br::println(v);

		v = pop(v, 4);
		v = push(v, 999);

		br::println(v);
	}

	auto map = br::map_file(cstr("README.md"));
	br::print(map);

	// BR_INTERNAL_ASSERT(1 == 2);
	// BR_INTERNAL_UNIMPLEMENTED();

	// br::halt("uh {} oh", "fuck");

	return br::EXIT_SUCCESS;
}
