#pragma once

#include <atomic>
namespace lk
{
class Spinlock {
    public:
	Spinlock()
	{
		is_locked_ = false;
	}

	~Spinlock()
	{
	}

	Spinlock(const Spinlock &) = delete;
	Spinlock &operator=(const Spinlock &) = delete;

	inline void lock()
	{
		bool unlocked = false;
		while (!is_locked_.compare_exchange_strong(
			unlocked, true, std::memory_order_acquire,
			std::memory_order_relaxed)) {
			unlocked = false;
			// XXX: spin hint
		}
	}

	inline void unlock()
	{
		is_locked_.store(false, std::memory_order_release);
	}

	inline bool try_lock()
	{
		bool unlocked = 0;
		return is_locked_.compare_exchange_strong(
			unlocked, true, std::memory_order_acquire,
			std::memory_order_relaxed);
	}

    private:
	std::atomic<bool> is_locked_;
};
}
