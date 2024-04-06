export module jp3.core:initializable;

namespace jp3
{
	export class initializable
	{
	public:
		constexpr initializable() noexcept = default;

		constexpr explicit initializable(const bool is_initialized) noexcept :
			m_is_initialized{ is_initialized }
		{}

		constexpr bool is_initialized() const noexcept
		{
			return m_is_initialized;
		}

	protected:
		constexpr void set_initialization_state(const bool is_initialized) noexcept
		{
			m_is_initialized = is_initialized;
		}

	private:
		bool m_is_initialized{ false };
	};
}