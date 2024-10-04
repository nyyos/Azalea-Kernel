
#include <cstdint>
#include <log/log.h>

#if UINT32_MAX == UINTPTR_MAX
constexpr const uint32_t DEFAULT_STACK_GUARD = 0xe2dee396;
#else
constexpr const uint64_t DEFAULT_STACK_GUARD = 0x595e9fbd94fda766;
#endif

extern "C" {
std::uintptr_t __stack_chk_guard = DEFAULT_STACK_GUARD;

[[gnu::noreturn]] void __stack_chk_fail(void)
{
	core::printk("Stack smashing detected\n");
	asm volatile("cli; hlt");
	__builtin_unreachable();
}
}
