#include <krn/krn.h>
#include <krn/port.h>
#include <log/log.h>

namespace core
{

void crash(const char *msg)
{
	printk(PANIC "KERNEL PANIC\n");
	printk(PANIC "Reason: %s\n", msg);
	port::hcf(true);
}

}
