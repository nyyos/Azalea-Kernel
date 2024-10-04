#include <limine.h>
#include <krn/krn.h>
#include <log/log.h>

#define LIMINE_REQ [[gnu::used, gnu::section(".requests")]]

extern "C" {
// clang-format off
[[gnu::used, gnu::section(".requests_start_marker")]]
static volatile LIMINE_REQUESTS_START_MARKER;
[[gnu::used, gnu::section(".requests_end_marker")]]
static volatile LIMINE_REQUESTS_END_MARKER;
// clang-format on

LIMINE_REQ static volatile LIMINE_BASE_REVISION(2);

LIMINE_REQ static volatile limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
};

LIMINE_REQ static volatile limine_bootloader_info_request loader_info_request = {
	.id = LIMINE_BOOTLOADER_INFO_REQUEST,
	.revision = 0,
};
}

namespace core
{
namespace
{

CpuData bsp_cpu;
[[gnu::used,
  gnu::section(".cpulocals.local_data")]] LocalData bsp_local_data = {
	.cpu = &bsp_cpu
};

}

extern void kinit();

void plat_first_init();

extern "C" void limine_entry()
{
	plat_first_init();
	bsp_cpu.local_data = &bsp_local_data;

	printk(INFO "Azalea//limine-" ARCHNAME " (Built on: " __DATE__
		    " " __TIME__ ")\n");

	auto loader_info = loader_info_request.response;
	printk(INFO "booted using %s version %s-%ld\n", loader_info->name,
	       loader_info->version, loader_info->revision);

	kinit();
}

}
