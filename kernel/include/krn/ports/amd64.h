#pragma once

/*
from where I got this:
then, somewhere, probably in a function named init_smp() or something, you
iterate over APs and allocate (__percpu_end-__percpu_start) sized 
memory blocks for them too (the BSP can use the default-location one). 
The APs will then have GSBASE be the offset to add to __percpu_start 
to get the start of their percpu memory block

wrmsr(MSR_GSBASE, allocated_memory - start_of_cpulocals_section)
*/
#define __cpulocal [[gnu::section(".cpulocals")]] __seg_gs

namespace core ::port
{

struct PortLocalData {};

}
