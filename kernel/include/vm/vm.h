#pragma once

namespace core::vm
{
enum {
	kVmRead = (1 << 0),
	kVmWrite = (1 << 1),
	kVmExec = (1 << 2),
	kVmUser = (1 << 3),
	kVmGlobal = (1 << 4)
};

enum class Inheritance { Shared, Copy, None };

}
