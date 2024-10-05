#include <cstdint>
#include <krn/krn.h>
#include <log/log.h>
#include <krn/port.h>

extern "C" {
extern char azalea_symmap_start[];
extern char azalea_symmap_end[];

struct [[gnu::packed]] SymmapEntry {
	uint64_t addr;
	uint64_t size;
	uint64_t name;
};
}

namespace core::port

{

const char *resolve_symbol(uintptr_t at)
{
	std::size_t entry_count =
		(azalea_symmap_end - azalea_symmap_start) / sizeof(SymmapEntry);
	auto entries = (SymmapEntry *)(azalea_symmap_start);
	for (std::size_t i = 0; i < entry_count; i++) {
		auto &entry = entries[i];
		if (at < entry.addr)
			continue;
		if (at > entry.addr + entry.size)
			continue;

		return (const char *)entry.name;
	}
	return nullptr;
}

void crash_system(const char *msg)
{
	printk(DEBUG "crash todo: stop other cpus\n");

	printk("***** KERNEL PANIC *****\n");
	printk("REASON: %s\n", msg);

	printk("STACKTRACE:\n");

	std::uint64_t *rbp, *rip;
	asm volatile("mov %%rbp, %0" : "=r"(rbp)::"memory");
	while (rbp) {
		rip = rbp + 1;
		if (!rip || *rip == 0)
			break;

		auto sym = resolve_symbol(*rip);
		if (sym) {
			printk(" 0x%lx %s\n", *rip, sym);
		} else {
			printk(" 0x%lx\n", *rip);
		}

		rbp = (std::uint64_t *)*rbp;
	}

	hcf(true);
}
}
