#define NANOPRINTF_IMPLEMENTATION

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#include <nanoprintf.h>

#include <log/log.h>
#include <cstddef>
#include "platform/amd64/asm.h"

namespace core
{

constexpr size_t LOG_MAX_BUF = 256;

constexpr auto prefix_debug = "[ \x1b[35mDEBUG \x1b[0m] ";
constexpr auto prefix_info = "[ \x1b[32mINFO  \x1b[0m] ";
constexpr auto prefix_warn = "[ \x1b[33mWARN  \x1b[0m] ";
constexpr auto prefix_err = "[ \x1b[31mERROR \x1b[0m] ";
constexpr auto prefix_panic = "[ \x1b[31mPANIC \x1b[0m] ";
constexpr auto prefix_trace = "[ \x1b[36mTRACE \x1b[0m] ";

#define COM1 0x3f8
static void serial_putc(int c)
{
	while (!(inb(COM1 + 5) & 0x20))
		;
	outb(COM1, c);
}

static void serial_write(const char *msg, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		serial_putc(msg[i]);
	}
}

[[clang::no_sanitize("undefined", "address")]] void printk(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	std::size_t size;
	[[gnu::aligned(_Alignof(char))]] char buf[LOG_MAX_BUF + 1];
	buf[LOG_MAX_BUF] = '\0';
	int lvl = *fmt;
	// get log level
	switch (lvl) {
	// info
	case 1:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, prefix_info);
		break;
	// warn
	case 2:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, prefix_warn);
		break;
	// error
	case 3:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, prefix_err);
		break;
	// debug
	case 4:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, prefix_debug);
		break;
	// panic
	case 5:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, prefix_panic);
		break;
	case 6:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, prefix_trace);
		break;
	default:
		size = 0;
	}

	// XXX: LOCK!!!

	size += npf_vsnprintf(buf + size, LOG_MAX_BUF - size, fmt, args);
	serial_write(buf, size);

	va_end(args);
}
}
