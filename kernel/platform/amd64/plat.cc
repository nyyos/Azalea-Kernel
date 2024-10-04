#include "asm.h"

namespace core
{
void plat_first_init()
{
	wrmsr(kMsrGsBase, 0);
}
}
