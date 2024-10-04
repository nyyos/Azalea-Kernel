#pragma once

#define INFO "\1"
#define WARN "\2"
#define ERR "\3"
#define DEBUG "\4"
#define PANIC "\5"
#define TRACE "\6"

namespace core
{
[[gnu::format(printf, 1, 2)]] void printk(const char *fmt, ...);
}
