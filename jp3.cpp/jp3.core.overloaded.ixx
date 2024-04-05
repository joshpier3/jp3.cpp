export module jp3.core.overloaded;

namespace jp3
{
	export template<typename ... to_overload_types>
	struct overloaded final : to_overload_types...
	{
		using to_overload_types::operator()...;
	};
}