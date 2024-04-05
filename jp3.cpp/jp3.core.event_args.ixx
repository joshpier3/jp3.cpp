export module jp3.core.event_args;

import std;

namespace jp3
{
	template<typename caller_type>
	class event_args_caller final
	{
	public:
		constexpr explicit event_args_caller(caller_type* caller) noexcept :
			m_caller{ caller }
		{}

		constexpr ~event_args_caller() noexcept = default;

		event_args_caller(const event_args_caller&) = delete;
		event_args_caller& operator = (const event_args_caller&) = delete;

		event_args_caller(event_args_caller&&) = delete;
		event_args_caller& operator = (event_args_caller&&) = delete;

		constexpr caller_type* get() const noexcept
		{
			return m_caller;
		}

		constexpr caller_type* operator -> () const noexcept
		{
			return m_caller;
		}

	private:
		caller_type* m_caller;
	};

	export template<typename caller_type>
	requires(std::is_class<caller_type>::value)
	struct event_args
	{
		explicit event_args(caller_type* caller) :
			caller{ caller }
		{
			if (caller == nullptr)
				throw std::invalid_argument{ "Invalid argument: jp3::event_args::event_args(caller_type* caller) caller must not be nullptr." };
		}

		explicit event_args(caller_type& caller) noexcept :
			caller{ &caller }
		{}

		event_args_caller<caller_type> caller;
	};
}