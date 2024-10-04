#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_VISIBILITY_STATIC

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

#define LOG_MAX_BUF 256
#define LOG_DEBUG "[ \x1b[35mDEBUG \x1b[0m] "
#define LOG_INFO "[ \x1b[32mINFO  \x1b[0m] "
#define LOG_WARN "[ \x1b[33mWARN  \x1b[0m] "
#define LOG_ERR "[ \x1b[31mERROR \x1b[0m] "
#define LOG_PANIC "[ \x1b[31mPANIC \x1b[0m] "
#define LOG_TRACE "[ \x1b[36mTRACE \x1b[0m] "

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
		size = npf_snprintf(buf, LOG_MAX_BUF, LOG_INFO);
		break;
	// warn
	case 2:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, LOG_WARN);
		break;
	// error
	case 3:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, LOG_ERR);
		break;
	// debug
	case 4:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, LOG_DEBUG);
		break;
	// panic
	case 5:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, LOG_PANIC);
		break;
	case 6:
		fmt++;
		size = npf_snprintf(buf, LOG_MAX_BUF, LOG_TRACE);
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
