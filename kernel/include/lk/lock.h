#pragma once

#include <concepts>

namespace lk
{
struct adopt_lock_t {};
struct defer_lock_t {};

template <typename T> concept BasicLockable = requires(T a)
{
	{
		a.lock()
	} -> std::same_as<void>;

	{
		a.unlock()

	} -> std::same_as<void>;
};

template <typename T> concept Lockable = BasicLockable<T> && requires(T a)
{
	{
		a.try_lock()
	} -> std::same_as<bool>;
};

template <BasicLockable Mutex> class lock_guard {
    public:
	using mutex_type = Mutex;

	[[nodiscard]] explicit lock_guard(mutex_type &mutex)
		: m_mutex(mutex)
	{
		m_mutex.lock();
	};

	[[nodiscard]] lock_guard(mutex_type &mutex, adopt_lock_t t)
		: m_mutex(mutex)
	{
	}

	// disallow copying
	lock_guard(const lock_guard &) = delete;
	lock_guard &operator=(const lock_guard &) = delete;

	~lock_guard()
	{
		m_mutex.unlock();
	}

    private:
	mutex_type &m_mutex;
};

template <BasicLockable Mutex> class unique_lock {
    public:
	using mutex_type = Mutex;

	friend void swap(unique_lock &u, unique_lock &v)
	{
		using std::swap;
		swap(u.m_mutex, v.m_mutex);
		swap(u.m_is_locked, v.m_is_locked);
	}

	unique_lock()
		: m_mutex{ nullptr }
		, m_is_locked{ false }
	{
	}

	[[nodiscard]] explicit unique_lock(mutex_type &mutex)
		: m_mutex{ &mutex }
		, m_is_locked{ false }
	{
		m_mutex->lock();
	}

	[[nodiscard]] unique_lock(mutex_type &mutex, adopt_lock_t t)
		: m_mutex{ &mutex }
		, m_is_locked{ true }
	{
	}

	[[nodiscard]] unique_lock(mutex_type &mutex, defer_lock_t t)
		: m_mutex{ &mutex }
		, m_is_locked{ false }
	{
	}

	~unique_lock()
	{
		if (m_is_locked)
			unlock();
	}

	unique_lock(unique_lock &&other)
		: unique_lock()
	{
		swap(*this, other);
	}

	unique_lock &operator=(unique_lock other)
	{
		swap(*this, other);
		return *this;
	}

	unique_lock(const unique_lock &) = delete;

	void unlock()
	{
		m_mutex->unlock();
		m_is_locked = false;
	}

	void lock()
	{
		m_mutex->lock();
		m_is_locked = true;
	}

	bool try_lock() requires Lockable<Mutex>
	{
		if (m_mutex->try_lock()) {
			m_is_locked = true;
			return true;
		}
		return false;
	}

	mutex_type *release()
	{
		auto mut = m_mutex;
		m_mutex = nullptr;
		m_is_locked = false;
		return mut;
	}

	mutex_type *mutex() const
	{
		return m_mutex;
	}

	explicit operator bool() const
	{
		owns_lock();
	}

	bool owns_lock() const
	{
		return m_is_locked;
	}

    private:
	mutex_type *m_mutex;
	bool m_is_locked;
};

}
