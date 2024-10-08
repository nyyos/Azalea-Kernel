#include <lk/lock.h>
#include <lk/spinlock.h>
#include <log/log.h>
#include <krn/port.h>
#include <string>
#include <krn/krn.h>

namespace core
{
extern "C" {
extern char __cpulocals_start[];
extern char __cpulocals_end[];
}

void kinit()
{
	auto lock = lk::Spinlock();
	std::string s;
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
