#ifndef BR_LOG_H
#define BR_LOG_H

#include <lib/log.hpp>

namespace br {

	#define BR_ANSI_RESET  "\x1b[0m"
	#define BR_ANSI_BOLD   "\x1b[1m"

	#define BR_ANSI_FG_BLACK    "\x1b[30m"
	#define BR_ANSI_FG_RED      "\x1b[31m"
	#define BR_ANSI_FG_GREEN    "\x1b[32m"
	#define BR_ANSI_FG_YELLOW   "\x1b[33m"
	#define BR_ANSI_FG_BLUE     "\x1b[34m"
	#define BR_ANSI_FG_MAGENTA  "\x1b[35m"
	#define BR_ANSI_FG_CYAN     "\x1b[36m"
	#define BR_ANSI_FG_WHITE    "\x1b[37m"

	#define BR_ANSI_FG_BRIGHT_BLACK    "\x1b[30;1m"
	#define BR_ANSI_FG_BRIGHT_RED      "\x1b[31;1m"
	#define BR_ANSI_FG_BRIGHT_GREEN    "\x1b[32;1m"
	#define BR_ANSI_FG_BRIGHT_YELLOW   "\x1b[33;1m"
	#define BR_ANSI_FG_BRIGHT_BLUE     "\x1b[34;1m"
	#define BR_ANSI_FG_BRIGHT_MAGENTA  "\x1b[35;1m"
	#define BR_ANSI_FG_BRIGHT_CYAN     "\x1b[36;1m"
	#define BR_ANSI_FG_BRIGHT_WHITE    "\x1b[37;1m"

	#define BR_ANSI_BG_BLACK    "\x1b[40m"
	#define BR_ANSI_BG_RED      "\x1b[41m"
	#define BR_ANSI_BG_GREEN    "\x1b[42m"
	#define BR_ANSI_BG_YELLOW   "\x1b[43m"
	#define BR_ANSI_BG_BLUE     "\x1b[44m"
	#define BR_ANSI_BG_MAGENTA  "\x1b[45m"
	#define BR_ANSI_BG_CYAN     "\x1b[46m"
	#define BR_ANSI_BG_WHITE    "\x1b[47m"

	#define BR_ANSI_BG_BRIGHT_BLACK    "\x1b[40;1m"
	#define BR_ANSI_BG_BRIGHT_RED      "\x1b[41;1m"
	#define BR_ANSI_BG_BRIGHT_GREEN    "\x1b[42;1m"
	#define BR_ANSI_BG_BRIGHT_YELLOW   "\x1b[43;1m"
	#define BR_ANSI_BG_BRIGHT_BLUE     "\x1b[44;1m"
	#define BR_ANSI_BG_BRIGHT_MAGENTA  "\x1b[45;1m"
	#define BR_ANSI_BG_BRIGHT_CYAN     "\x1b[46;1m"
	#define BR_ANSI_BG_BRIGHT_WHITE    "\x1b[47;1m"

	#define BR_LOG_INFO_STYLE     BR_ANSI_BOLD BR_ANSI_FG_BRIGHT_WHITE  "[-]" BR_ANSI_RESET
	#define BR_LOG_WARN_STYLE     BR_ANSI_BOLD BR_ANSI_FG_BRIGHT_BLUE   "[*]" BR_ANSI_RESET
	#define BR_LOG_ERROR_STYLE    BR_ANSI_BOLD BR_ANSI_FG_BRIGHT_RED    "[!]" BR_ANSI_RESET
	#define BR_LOG_SUCCESS_STYLE  BR_ANSI_BOLD BR_ANSI_FG_BRIGHT_YELLOW "[^]" BR_ANSI_RESET

	#define BR_TRACE __FILE__ ":" BR_STR(__LINE__)

	void log_info(const char* const fmt, ...) {
		// fprintf(stderr, BR_LOG_INFO_STYLE " ");
		// va_list args;
		// va_start(args, fmt);
		// vfprintf(stderr, fmt, args);
		// va_end(args);
	}

	void log_warn(const char* const fmt, ...) {
		// fprintf(stderr, BR_LOG_WARN_STYLE " ");
		// va_list args;
		// va_start(args, fmt);
		// vfprintf(stderr, fmt, args);
		// va_end(args);
	}

	void log_error(const char* const fmt, ...) {
		// fprintf(stderr, BR_LOG_ERROR_STYLE " ");
		// va_list args;
		// va_start(args, fmt);
		// vfprintf(stderr, fmt, args);
		// va_end(args);
	}

	void log_success(const char* const fmt, ...) {
		// fprintf(stderr, BR_LOG_SUCCESS_STYLE" ");
		// va_list args;
		// va_start(args, fmt);
		// vfprintf(stderr, fmt, args);
		// va_end(args);
	}

}

#endif

