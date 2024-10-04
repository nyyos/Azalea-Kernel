extern "C" [[noreturn]] void abort() noexcept
{
	while (true)
		asm volatile("cli; hlt");
}
