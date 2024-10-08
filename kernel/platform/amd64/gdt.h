#pragma once

#include <cstdint>

namespace core::port
{
extern "C" struct [[gnu::packed]] Tss {
	uint32_t unused0;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t unused1;
	uint64_t ist1; // nmi
	uint64_t ist2; // debug/breakpoint
	uint64_t ist3; // double fault
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t unused2;
	uint32_t iopb;
};

enum GdtEntries {
	kGdtNull = 0,
	kGdtKernelCode,
	kGdtKernelData,
	kGdtUserData,
	kGdtUserCode,
	kGdtTss,
};

void gdt_init();
void gdt_load();
void tss_load(Tss *tss);

}
