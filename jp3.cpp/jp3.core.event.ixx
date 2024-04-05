export module jp3.core.event;

import std;

import jp3.core.id;

namespace jp3
{
	export template<typename ... arg_types>
	class event
	{
	public:
		using handler_id = jp3::id<std::size_t, std::numeric_limits<std::size_t>::max(), event>;

		constexpr event() noexcept = default;

		constexpr explicit event(const std::size_t reserve_size)
		{
			m_callbacks.reserve(reserve_size);
		}

		template<typename Func>
		[[nodiscard("The handler_id is used to unsubscribe.")]]
		constexpr const handler_id subscribe(Func func)
		{
			const callback& new_callback{ m_callbacks.emplace_back(func, generate_unique_id_for<event>()) };

			return new_callback.id;
		}

		constexpr const handler_id subscribe(std::nullptr_t) = delete;

		constexpr bool unsubscribe(const handler_id id)
		{
			for (std::size_t i = 0; i < m_callbacks.size(); ++i)
			{
				if (m_callbacks[i].id == id)
				{
					m_callbacks.erase(m_callbacks.begin() + i);
					return true;
				}
			}

			return false;
		}

		constexpr void clear_all_subscriptions()
		{
			m_callbacks.clear();
		}

		constexpr void cleanup_unused_memory()
		{
			m_callbacks.shrink_to_fit();
		}

		constexpr void fire(arg_types... args)
		{
			for (callback& callback : m_callbacks)
			{
				callback.function(std::forward<arg_types>(args)...);
			}
		}

	private:
		struct callback
		{
			std::function<void(arg_types...)> function;
			handler_id id;
		};

		std::vector<callback> m_callbacks{};
	};
}