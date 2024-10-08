#include "asm.h"
#include "gdt.h"
#include "features.h"

namespace core::port
{

Features g_features;

void plat_first_init()
{
	wrmsr(kMsrGsBase, 0);
	gdt_init();

	uint32_t a, b, c, d;
	cpuid(0x80000001, &a, &b, &c, &d);
	g_features.nx = (d >> 20) & 1;
	g_features.gbpages = (d >> 26) & 1;
	cpuid(1, &a, &b, &c, &d);
	g_features.pge = (d >> 13) & 1;
	g_features.pat = (d >> 16) & 1;
	g_features.pcid = (c >> 17) & 1;
}
}
