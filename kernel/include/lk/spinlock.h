#pragma once

#include <atomic>
namespace lk
{
class Spinlock {
    public:
	constexpr Spinlock()
		: m_is_locked(false)
	{
	}

	constexpr ~Spinlock() = default;

	Spinlock(const Spinlock &) = delete;
	Spinlock &operator=(const Spinlock &) = delete;

	inline void lock()
	{
		bool unlocked = false;
		while (!m_is_locked.compare_exchange_strong(
			unlocked, true, std::memory_order_acquire,
			std::memory_order_relaxed)) {
			unlocked = false;
			// XXX: spin hint
		}
	}

	inline void unlock()
	{
		m_is_locked.store(false, std::memory_order_release);
	}

	inline bool try_lock()
	{
		bool unlocked = 0;
		return m_is_locked.compare_exchange_strong(
			unlocked, true, std::memory_order_acquire,
			std::memory_order_relaxed);
	}

    private:
	std::atomic<bool> m_is_locked;
};
}
