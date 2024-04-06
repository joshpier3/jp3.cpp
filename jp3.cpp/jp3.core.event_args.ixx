export module jp3.core:event_args;

import std;

namespace jp3
{
	export template<typename caller_type>
	requires(std::is_class_v<caller_type>)
	struct event_args
	{
		constexpr explicit event_args(caller_type& caller) noexcept :
			caller{ caller }
		{}

		caller_type& caller;
	};
}