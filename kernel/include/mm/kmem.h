#pragma once

#include <cstddef>
namespace core
{

void *kmalloc(std::size_t size);
void kfree(void *ptr, std::size_t size);

}
