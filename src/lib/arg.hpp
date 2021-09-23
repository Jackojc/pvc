#ifndef BR_ARG_H
#define BR_ARG_H

#include <lib/def.hpp>
#include <lib/str.hpp>
#include <lib/exit.hpp>
#include <lib/trait.hpp>
#include <lib/assert.hpp>
#include <lib/print.hpp>

namespace br {

	namespace detail {
		using arg_err_t = u8_t;
		enum: arg_err_t {
			ARG_SUCCESS         = 0b00000000,
			ARG_ERR_UNKNOWN_ARG = 0b00000001,
			ARG_ERR_TAKES_ARG   = 0b00000010,
			ARG_POSITIONAL      = 0b00000100,
		};

		enum {
			MESSAGE_TAKES_ARG,
			MESSAGE_UNKNOWN_ARG,
		};

		constexpr str_view messages[] = {
			cstr("error: option '{}' takes an argument"),
			cstr("error: unknown argument: '{}'"),
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

	// Put back an argument.
	void argunshift(int& argc, const char**& argv) {
		argv--, argc++;
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
		if (as_byte(arg) != '-') {
			flags |= detail::ARG_POSITIONAL;
			return true;
		}

		if (none(eq(arg, shrt), eq(arg, lng))) {
			flags |= detail::ARG_ERR_UNKNOWN_ARG;
			return false;
		}

		// Call user provided handler and pass in argument.
		return fn(argc, argv, arg, flags);
	}


	// Convenience functions for common types.
	decltype(auto) opt_toggle(bool& ref) {
		return [&ref] (int&, const char**&, str_view arg, detail::arg_err_t&) { ref = not ref; return true; };
	}

	decltype(auto) opt_set(bool& ref) {
		return [&ref] (int&, const char**&, str_view arg, detail::arg_err_t&) { ref = true; return true; };
	}

	decltype(auto) opt_unset(bool& ref) {
		return [&ref] (int&, const char**&, str_view arg, detail::arg_err_t&) { ref = false; return true; };
	}

	decltype(auto) opt_sv(str_view& ref) {
		return [&ref] (int&, const char**&, str_view arg, detail::arg_err_t&) { ref = arg; return true; };
	}

	// Consumes an extra argument for an option that requires an argument.
	decltype(auto) opt_arg(str_view& ref) {
		return [&ref] (int& argc, const char**& argv, str_view arg, detail::arg_err_t& flags) {
			arg = argshift(argc, argv);

			if (is_null(arg)) {
				flags |= detail::ARG_ERR_TAKES_ARG;
				return false;
			}

			ref = arg;
			return true;
		};
	}

	// Wrap user handler for positional arg where the user only cares about the string.
	template <typename F>
	decltype(auto) positional(F func) {
		return [&func] (int&, const char**&, br::str_view sv, br::detail::arg_err_t&) {
			func(sv);
		};
	}

	// Do nothing with positional arg.
	void ignore_positional(int&, const char**&, br::str_view, br::detail::arg_err_t&) {}


	template <typename T, typename... Ts>
	inline void argparse(int& argc, const char**& argv, str_view help, T positional_opt, Ts... parsers) {
		BR_STATIC_ASSERT((conjunction_v<is_specialisation<Ts, opt_t>...>));

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
			bool found = (
				optparse(argc, argv, arg, flags, parsers) or ... or // User args
				optparse(argc, argv, arg, flags, help_opt) // Help
			);

			// If we found no matches, we can assume an error occurred.
			if (not found) {
				if (flags & detail::ARG_ERR_TAKES_ARG) {
					errlnfmt(detail::messages[detail::MESSAGE_TAKES_ARG], arg);
				}

				else if (flags & detail::ARG_ERR_UNKNOWN_ARG) {
					errlnfmt(detail::messages[detail::MESSAGE_UNKNOWN_ARG], arg);
				}

				// Print help and exit.
				println(help);
				exit(EXIT_FAILURE);
			}

			// At this point the argument has been matched.
			// We check if the argument is positional and if so,
			// call the user specified handler for positional args.
			if (flags & detail::ARG_POSITIONAL) {
				positional_opt(argc, argv, arg, flags);
			}

			// Print help if the user asked for it.
			else if (want_help) {
				println(help);
				exit(EXIT_SUCCESS);
			}

			// Shift to the next argument.
			arg = argshift(argc, argv);
		}
	}

}

#endif
