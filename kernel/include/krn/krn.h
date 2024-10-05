#pragma once

#include <krn/port.h>

namespace core
{
namespace port
{
/* core::port contains functions that have to be present in every port */
void crash_system(const char *msg);
}

struct LocalData;

struct CpuData {
	LocalData *local_data;

	int num;
};

struct LocalData : public port::PortLocalData {
	CpuData *cpu;
};

#ifdef AMD64
static inline CpuData &curcpu()
{
	return *((struct LocalData * __seg_gs)0)->cpu;
}
#endif

void crash(const char *msg, ...);

}
