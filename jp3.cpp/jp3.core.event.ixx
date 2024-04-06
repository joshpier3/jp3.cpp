export module jp3.core:event;

import std;

import :id;

namespace jp3
{
	export template<typename ... arg_types>
	class event
	{
	public:
		using handler_id = jp3::id<std::size_t, std::numeric_limits<std::size_t>::max(), event>;

		inline event() noexcept = default;

		inline explicit event(const std::size_t reserve_size)
		{
			m_callbacks.reserve(reserve_size);
		}

		template<typename Func>
		[[nodiscard("The handler_id is used to unsubscribe.")]]
		inline const handler_id subscribe(Func func)
		{
			const callback& new_callback{ m_callbacks.emplace_back(func, generate_unique_id_for<event>()) };

			return new_callback.id;
		}

		inline const handler_id subscribe(std::nullptr_t) = delete;

		inline bool unsubscribe(const handler_id id)
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

		inline void clear_all_subscriptions()
		{
			m_callbacks.clear();
		}

		inline void cleanup_unused_memory()
		{
			m_callbacks.shrink_to_fit();
		}

		inline std::size_t get_number_of_handlers() const
		{
			return m_callbacks.size();
		}

		inline void fire(arg_types... args)
		{
			for (callback& callback : m_callbacks)
			{
				callback.function(std::forward<arg_types>(args)...);
			}
		}

		inline void fire_x_amount(const std::size_t amount, const std::size_t start_index, arg_types... args)
		{
			const std::size_t num_callbacks = m_callbacks.size();

			if (start_index < num_callbacks)
			{
				const std::size_t max_amount = num_callbacks - start_index - std::size_t{ 1 };
				const std::size_t amount_to_fire = std::min(max_amount, amount);

				for (std::size_t i = start_index; i < amount_to_fire; ++i)
				{
					m_callbacks[i].function(std::forward<arg_types>(args)...);
				}
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