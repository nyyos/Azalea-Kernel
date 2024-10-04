#pragma once

#include <krn/port.h>

namespace core
{
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

}
