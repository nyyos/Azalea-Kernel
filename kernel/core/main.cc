#include <lk/lock.h>
#include <lk/spinlock.h>
#include <log/log.h>
#include <krn/port.h>
#include <krn/krn.h>

namespace core
{
extern "C" {
extern char __cpulocals_start[];
extern char __cpulocals_end[];

using InitPtr = void (*)();
extern InitPtr __init_array_start[];
extern InitPtr __init_array_end[];
}

static void run_global_constructors()
{
	if (*__init_array_end == *__init_array_start)
		return;
	std::size_t cnt =
		((uintptr_t)__init_array_end - (uintptr_t)__init_array_start) /
		sizeof(void *);
	printk(DEBUG "running %ld global constructors\n", cnt);
	for (std::size_t i = 0; i < cnt; i++) {
		__init_array_start[i]();
	}
}

void kinit()
{
	auto lock = lk::Spinlock();
	printk(INFO "percpu size: %ld\n", __cpulocals_end - __cpulocals_start);
	{
		lock.lock();
		auto guard = lk::lock_guard(lock, lk::adopt_lock_t{});
	}
	lock.lock();

	run_global_constructors();

	crash("end of kernel reached");
}

}

void operator delete(void *, std::size_t)
{
}
