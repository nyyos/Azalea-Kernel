#include "krn/krn.h"
#include <lk/lock.h>
#include <lk/spinlock.h>
#include <lk/string.h>
#include <log/log.h>
#include <krn/port.h>

namespace core
{
extern "C" {
extern char __cpulocals_start[];
extern char __cpulocals_end[];
}

void kinit()
{
	auto lock = lk::Spinlock();
	lk::string s;
	s += "teST";
	printk(INFO "TEST! New kernel in C++ %s\n", s.c_str());
	printk(INFO "percpu size: %ld\n", __cpulocals_end - __cpulocals_start);
	{
		lock.lock();
		auto guard = lk::lock_guard(lock, lk::adopt_lock_t{});
	}
	lock.lock();

	crash("end of kernel reached");
}

}

void operator delete(void *, std::size_t)
{
}
