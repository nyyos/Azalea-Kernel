#include <rtl/assert.h>

extern "C" [[noreturn]] void __assert_fail(char const *assertion,
					   char const *file, unsigned int line,
					   char const *function)
{
	// TODO: PANIC
	while (true)
		asm volatile("cli;hlt");
}
