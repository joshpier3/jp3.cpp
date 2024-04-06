export module jp3.core:ttas_spinlock;

import std;

#if __has_include(<Windows.h>)
import <Windows.h>;
#ifdef YieldProcessor
#define jp3_yield_processor YieldProcessor
#else
#define jp3_yield_processor __builtin_ia32_pause
#endif//defined YieldProcessor
#else
#define jp3_yield_processor __builtin_ia32_pause
#endif//__has_include(<Windows.h>)

namespace jp3
{
    export template<bool yield_processor>
    class ttas_spinlock;

    export template<>
	class ttas_spinlock<true>
	{
	public:
        inline void lock() noexcept 
        {
            for (;;) 
            {
                if (!m_lock.exchange(true, std::memory_order::acquire)) 
                {
                    return;
                }
                
                while (m_lock.load(std::memory_order::relaxed)) 
                {
                    jp3_yield_processor();
                }
            }
        }

        inline bool try_lock() noexcept 
        {
            return !m_lock.load(std::memory_order::relaxed) && 
                   !m_lock.exchange(true, std::memory_order::acquire);
        }

        inline void unlock() noexcept 
        {
            m_lock.store(false, std::memory_order::release);
        }

	private:
		std::atomic<bool> m_lock{ false };
	};

    export template<>
    class ttas_spinlock<false>
    {
    public:
        inline void lock() noexcept
        {
            for (;;)
            {
                if (!m_lock.exchange(true, std::memory_order::acquire))
                {
                    return;
                }

                while (m_lock.load(std::memory_order::relaxed))
                {
                    //Does not yield in this implementation
                    //jp3_yield_processor();
                }
            }
        }

        inline bool try_lock() noexcept
        {
            return !m_lock.load(std::memory_order::relaxed) &&
                   !m_lock.exchange(true, std::memory_order::acquire);
        }

        inline void unlock() noexcept
        {
            m_lock.store(false, std::memory_order::release);
        }

    private:
        std::atomic<bool> m_lock{ false };
    };
}