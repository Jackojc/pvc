#ifndef BR_ARG_H
#define BR_ARG_H

#include <lib/def.hpp>
#include <lib/str.hpp>
#include <lib/exit.hpp>
#include <lib/trait.hpp>
#include <lib/print.hpp>

namespace br {

	namespace detail {
		using arg_err_t = u8_t;
		enum: arg_err_t {
			ARG_SUCCESS         = 0b00000000,
			ARG_ERR_UNKNOWN_ARG = 0b00000001,
			ARG_ERR_TAKES_ARG   = 0b00000010,
			ARG_SHOW_HELP       = 0b00100000,
		};
	}


	// Return the next argument.
	str_view argshift(int& argc, const char**& argv) {
		if (argc <= 0)
			return {};

		str_view result = make_sv(*argv, length(*argv));
		argv++, argc--;

		return result;
	}


	template <typename T>
	struct opt_t {
		T fn;
		str_view long_opt;
		str_view short_opt;
	};

	template <typename T, typename... Ts>
	opt_t(T, Ts...) -> opt_t<T>;


	// Attempt to parse an option by comparing string in opt_t to arg.
	template <typename T>
	inline bool optparse(int& argc, const char**& argv, str_view arg, detail::arg_err_t& flags, opt_t<T> parser) {
		auto [fn, lng, shrt] = parser;

		// If argument doesn't match long or short option,
		// we return false (to break the `or` chain) and
		// we set the unknown arg flag.
		if (none(eq(arg, shrt), eq(arg, lng))) {
			flags |= detail::ARG_ERR_UNKNOWN_ARG;
			return false;
		}

		// Call user provided handler and pass in argument.
		fn(arg);

		flags |= detail::ARG_SUCCESS;
		return true;
	}


	// Convenience functions for common types.
	decltype(auto) opt_toggle(bool& ref) {
		return [&ref] (str_view arg) { ref = not ref; };
	}

	decltype(auto) opt_set(bool& ref) {
		return [&ref] (str_view arg) { ref = true; };
	}

	decltype(auto) opt_unset(bool& ref) {
		return [&ref] (str_view arg) { ref = false; };
	}

	decltype(auto) opt_sv(str_view& ref) {
		return [&ref] (str_view arg) { ref = arg; };
	}


	namespace detail {
		enum {
			MESSAGE_TAKES_ARG,
			MESSAGE_UNKNOWN_ARG,
		};

		constexpr str_view messages[] = {
			cstr("error: option '{}' takes an argument"),
			cstr("error: unknown argument: '{}'"),
		};
	}


	template <typename... Ts>
	inline bool argparse(int argc, const char* argv[], str_view help, Ts... parsers) {
		str_view exe = argshift(argc, argv); // argv[0]
		println(exe);

		bool want_help = false;
		opt_t help_opt = opt_t{opt_set(want_help), cstr("--help"), cstr("-h")};

		// Loop over all arguments.
		str_view arg = argshift(argc, argv);

		while (not is_null(arg)) {
			detail::arg_err_t flags = detail::ARG_SUCCESS;

			// Run all option parsers on the current argument.
			// Because we use `or` here, we get lazy evaluation.
			bool found = (optparse(argc, argv, arg, flags, parsers) or ... or optparse(argc, argv, arg, flags, help_opt));

			// If we found no matches, we can assume an error occurred.
			if (not found) {
				if (flags & detail::ARG_ERR_TAKES_ARG)   { errlnfmt(detail::messages[detail::MESSAGE_TAKES_ARG], arg); }
				if (flags & detail::ARG_ERR_UNKNOWN_ARG) { errlnfmt(detail::messages[detail::MESSAGE_UNKNOWN_ARG], arg); }

				// Print help and exit.
				println(help);
				exit(EXIT_FAILURE);
			}

			// Print help if the user asked for it.
			if (want_help) {
				println(help);
				exit(EXIT_SUCCESS);
			}

			// Shift to the next argument.
			arg = argshift(argc, argv);
		}

		return true;
	}

}

#endif
