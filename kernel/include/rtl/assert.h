#pragma once

#include <azaleaconf.h>

#ifdef __cplusplus
extern "C" {
#endif

#undef assert
#ifdef CONFIG_DISABLE_ASSERTS
#define assert(...) ((void)0)
#else
#define assert(expr)      \
	((void)((expr) || \
		(__assert_fail(#expr, __FILE__, __LINE__, __func__), 0)))

#endif

[[noreturn]] void __assert_fail(const char *assertion, const char *file,
				unsigned int line, const char *function);

#ifdef __cplusplus
}
#endif
