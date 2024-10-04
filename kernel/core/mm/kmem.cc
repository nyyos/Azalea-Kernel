#include <mm/kmem.h>
#include <rtl/assert.h>

namespace core::mm
{
namespace
{
constexpr const std::size_t ARENA_SIZE = 0x1000 * 8;
char arena[ARENA_SIZE];
std::size_t pos;
}

void *kmalloc(std::size_t size)
{
	assert(size < ARENA_SIZE - pos);
	auto ret = &arena[pos];
	pos += size;
	return (void *)ret;
}

void kfree(void *ptr, std::size_t size)
{
	// NOOP
}
}
