export module jp3.core.id;

import std;

namespace jp3
{
	export template<std::integral value_type = std::size_t, value_type invalid_value = (std::numeric_limits<value_type>::max)(), typename for_type = void>
	class id
	{
	public:
		using underlying_value_type = value_type;

		static constexpr underlying_value_type c_invalid_underlying_value{ invalid_value };

		constexpr id() noexcept = default;
		
		constexpr explicit id(const underlying_value_type id_value) noexcept :
			m_value{ id_value }
		{}

		constexpr auto operator <=> (const id&) const noexcept = default;

		constexpr underlying_value_type get_value() const noexcept
		{
			return m_value;
		}

		constexpr explicit operator underlying_value_type() const noexcept
		{
			return m_value;
		}

		constexpr bool is_valid() const noexcept
		{
			return m_value != c_invalid_underlying_value;
		}

	private:
		underlying_value_type m_value{ c_invalid_underlying_value };
	};

	export template<typename for_type, std::integral id_value_type = std::size_t, id_value_type invalid_id_value = std::numeric_limits<id_value_type>::max()>
	inline id<id_value_type, invalid_id_value, for_type> generate_unique_id_for() noexcept
	{
		constexpr id_value_type c_start_value{ invalid_id_value == id_value_type{ 0 } ? 1 : 0 };

		static std::atomic<id_value_type> s_id{ c_start_value };

		return id<id_value_type, invalid_id_value, for_type>{ s_id.fetch_add(id_value_type{ 1 }, std::memory_order::relaxed) };
	}
}