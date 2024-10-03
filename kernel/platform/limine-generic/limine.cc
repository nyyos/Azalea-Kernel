#include <limine.h>

#define LIMINE_REQ [[gnu::used, gnu::section(".requests")]] static volatile

extern "C" {
// clang-format off
[[gnu::used, gnu::section(".requests_start_marker")]]
static volatile LIMINE_REQUESTS_START_MARKER;
[[gnu::used, gnu::section(".requests_start_marker")]]
static volatile LIMINE_REQUESTS_END_MARKER;
// clang-format on

LIMINE_REQ LIMINE_BASE_REVISION(2);

LIMINE_REQ limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
};

LIMINE_REQ limine_bootloader_info_request loader_info_request = {
	.id = LIMINE_BOOTLOADER_INFO_REQUEST,
	.revision = 0,
};
}

namespace core
{

extern void kinit();

extern "C" void limine_entry()
{
	kinit();
	asm volatile("cli; hlt");
	__builtin_trap();
}

}
