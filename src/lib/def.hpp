#ifndef BR_DEF_H
#define BR_DEF_H

#include <cstdint>
#include <climits>
#include <cfloat>

namespace br {

	// Aliases for sized types.
	using u8_t = uint_least8_t;
	using i8_t = int_least8_t;

	using u16_t = uint_least16_t;
	using i16_t = int_least16_t;

	using u32_t = uint_least32_t;
	using i32_t = int_least32_t;

	using u64_t = uint_least64_t;
	using i64_t = int_least64_t;


	// Named aliases.
	using byte_t   = br::u8_t;
	using char_t   = br::u32_t;
	using index_t  = br::u64_t;
	using size_t   = br::u64_t;
	using length_t = br::u64_t;
	using ptr_t    = uintptr_t;

	using nullptr_t = decltype(nullptr);


	// Integral limits.
	// 8
	constexpr u8_t max_u8() { return UCHAR_MAX; }
	constexpr i8_t max_i8() { return SCHAR_MIN; }

	constexpr u8_t min_u8() { return 0; }
	constexpr i8_t min_i8() { return SCHAR_MIN; }

	// 16
	constexpr u16_t max_u16() { return USHRT_MAX; }
	constexpr i16_t max_i16() { return SHRT_MAX; }

	constexpr u16_t min_u16() { return 0; }
	constexpr i16_t min_i16() { return SHRT_MIN; }

	// 32
	constexpr u32_t max_u32() { return UINT_MAX; }
	constexpr i32_t max_i32() { return INT_MAX; }

	constexpr u32_t min_u32() { return 0; }
	constexpr i32_t min_i32() { return INT_MIN; }

	// 64
	constexpr u64_t max_u64() { return ULLONG_MAX; }
	constexpr i64_t max_i64() { return LLONG_MAX; }

	constexpr u64_t min_u64() { return 0; }
	constexpr i64_t min_i64() { return LLONG_MIN; }


	// Detect platform.
	#define BR_PLATFORM_UNKNOWN

	// Windows.
	#ifdef _WIN64
		#define BR_PLATFORM_WINDOWS
		#undef BR_PLATFORM_UNKNOWN
	#endif

	#ifdef _WIN32
		#define BR_PLATFORM_WINDOWS
		#undef BR_PLATFORM_UNKNOWN
	#endif

	#ifdef __WINDOWS__
		#define BR_PLATFORM_WINDOWS
		#undef BR_PLATFORM_UNKNOWN
	#endif


	// Linux.
	#ifdef __linux
		#define BR_PLATFORM_LINUX
		#undef BR_PLATFORM_UNKNOWN
	#endif

	#ifdef linux
		#define BR_PLATFORM_LINUX
		#undef BR_PLATFORM_UNKNOWN
	#endif


	// BSDs.
	#ifdef __DragonFly__
		#define BR_PLATFORM_BSD
		#undef BR_PLATFORM_UNKNOWN
	#endif

	#ifdef __FreeBSD__
		#define BR_PLATFORM_BSD
		#undef BR_PLATFORM_UNKNOWN
	#endif

	#ifdef __NETBSD__
		#define BR_PLATFORM_BSD
		#undef BR_PLATFORM_UNKNOWN
	#endif

	#ifdef __OpenBSD__
		#define BR_PLATFORM_BSD
		#undef BR_PLATFORM_UNKNOWN
	#endif


	// MacOS.
	#ifdef __APPLE__
		#define BR_PLATFORM_OSX
		#undef BR_PLATFORM_UNKNOWN
	#endif

	#ifdef macintosh
		#define BR_PLATFORM_OSX
		#undef BR_PLATFORM_UNKNOWN
	#endif

	#ifdef __MACH__
		#define BR_PLATFORM_OSX
		#undef BR_PLATFORM_UNKNOWN
	#endif


	// Other UNIX.
	#ifdef __unix
		#define BR_PLATFORM_UNIX
		#undef BR_PLATFORM_UNKNOWN
	#endif

	#ifdef unix
		#define BR_PLATFORM_UNIX
		#undef BR_PLATFORM_UNKNOWN
	#endif


	// Detect compiler.
	#define BR_COMPILER_UNKNOWN

	#ifdef __GNUC__
		#define BR_COMPILER_GCC
		#undef BR_COMPILER_UNKNOWN
	#endif

	#ifdef __INTEL_COMPILER
		#define BR_COMPILER_INTEL
		#undef BR_COMPILER_UNKNOWN
	#endif

	#ifdef __clang__
		#define BR_COMPILER_CLANG
		#undef BR_COMPILER_UNKNOWN
	#endif

	#ifdef _MSC_VER
		#define BR_COMPILER_MSVC
		#undef BR_COMPILER_UNKNOWN
	#endif


	// Utility macros.
	// Stringify a macro def.
	// i.e. BR_STR(__LINE__) => "42" as opposed to "__LINE__"
	#define BR_STR_IMPL_(x) #x
	#define BR_STR(x) BR_STR_IMPL_(x)

	// Concatenate macro defs.
	// i.e. BR_CAT(__FILE__, __LINE__) => "foo.c10" as opposed to "__FILE____LINE__"
	#define BR_CAT_IMPL_(x, y) x##y
	#define BR_CAT(x, y) BR_CAT_IMPL_(x, y)

	// Create a uniquely named variable for use in a macro.
	#define BR_VAR(x) BR_CAT(var_, BR_CAT(x, BR_CAT(__LINE__, _)))

	// Evaluate expressions at beginning and ending of a scope.
	#define BR_SCOPE(open, close) \
		for ( \
			br_index BR_VAR(i) = ((open), 0); \
			!BR_VAR(i); \
			(BR_VAR(i)++), (close) \
		)

	// Evaluate expression at end of scope.
	#define BR_DEFER(close) \
		BR_SCOPE(0, close)


	// Absolute difference between 2 pointers.
	template <typename T>
	br::size_t ptrdiff(const T a, const T b) {
		return
			((b - a) * (b > a)) +  // b > a => b - a
			((a - b) * (a > b));   // a > b => a - b
	}

}

#endif

