#include <cstdint>
#include <cstring>
#include <krn/port.h>
#include <lk/spinlock.h>
#include <lk/lock.h>
#include "gdt.h"

namespace core::port
{

extern "C" {
struct [[gnu::packed]] Gdtr {
	uint16_t limit;
	uint64_t base;
};

struct [[gnu::packed]] TssEntry {
	uint16_t length;
	uint16_t low;
	uint8_t mid;
	uint8_t access;
	uint8_t flags;
	uint8_t high;
	uint32_t upper;
	uint32_t rsv;
};

struct [[gnu::packed]] GdtEntry {
	uint16_t limit;
	uint16_t low;
	uint8_t mid;
	uint8_t access;
	uint8_t granularity;
	uint8_t high;
};

struct [[gnu::packed]] Gdt {
	GdtEntry entries[4];
	TssEntry tss;
};
}

namespace
{
Gdt g_gdt;
lk::Spinlock g_tsslock;

void makeGdtEntry(Gdt &gdt, int index, uint32_t base, uint16_t limit,
		  uint8_t access, uint8_t granularity)
{
	GdtEntry &ent = gdt.entries[index];
	ent.limit = limit;
	ent.access = access;
	ent.granularity = granularity;
	ent.low = (uint16_t)base;
	ent.mid = (uint8_t)(base >> 16);
	ent.high = (uint8_t)(base >> 24);
}
void makeTssEntry(Gdt &gdt)
{
	gdt.tss.length = 104;
	gdt.tss.access = 0x89;
	gdt.tss.flags = 0;
	gdt.tss.rsv = 0;

	gdt.tss.low = 0;
	gdt.tss.mid = 0;
	gdt.tss.high = 0;
	gdt.tss.upper = 0;
}
}

void gdt_init()
{
	memset(&g_gdt, 0, sizeof(Gdt));
	makeGdtEntry(g_gdt, kGdtNull, 0, 0, 0, 0);
	makeGdtEntry(g_gdt, kGdtKernelCode, 0, 0, 0x9a, 0x20);
	makeGdtEntry(g_gdt, kGdtKernelData, 0, 0, 0x92, 0);
	makeGdtEntry(g_gdt, kGdtUserData, 0, 0, 0xf2, 0);
	makeGdtEntry(g_gdt, kGdtUserCode, 0, 0, 0xfa, 0x20);
	makeTssEntry(g_gdt);
}

extern "C" void _gdt_load(Gdtr *gdtr, int cs, int ss);

void gdt_load()
{
	Gdtr gdtr = { .limit = sizeof(Gdt) - 1, .base = (uint64_t)&g_gdt };
	_gdt_load(&gdtr, kGdtKernelCode * 8, kGdtKernelData * 8);
}

void tss_load(Tss *tss)
{
	auto guard = lk::lock_guard(g_tsslock);
	uintptr_t tss_addr = reinterpret_cast<uintptr_t>(tss);
	g_gdt.tss.low = (uint16_t)tss_addr;
	g_gdt.tss.mid = (uint8_t)(tss_addr >> 16);
	g_gdt.tss.high = (uint8_t)(tss_addr >> 24);
	g_gdt.tss.upper = (uint32_t)(tss_addr >> 32);
	g_gdt.tss.flags = 0;
	g_gdt.tss.access = 0b10001001;
	g_gdt.tss.rsv = 0;
	asm volatile("ltr %0" ::"rm"(kGdtTss * 8) : "memory");
}
}
