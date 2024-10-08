#include <krn/krn.h>
#include <krn/port.h>
#include <log/log.h>
#include <cstdarg>
#include <nanoprintf.h>

namespace core
{

void crash(const char *msg, ...)
{
	va_list va;
	va_start(va, msg);

	char buffer[1024];
	buffer[sizeof buffer - 1] = '\0';
	npf_vsnprintf(buffer, sizeof buffer - 1, msg, va);

	va_end(va);

	port::crash_system(buffer);
}

}
