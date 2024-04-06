export module jp3.core:unique_resource;

import std;

namespace jp3
{
	export template<typename resource_type, resource_type invalid_resource_value, typename deleter_type>
	class unique_resource
	{
	public:
		constexpr unique_resource() noexcept = default;

		constexpr explicit unique_resource(resource_type resource) noexcept :
			m_resource{ resource }
		{}

		constexpr ~unique_resource() noexcept
		{
			this->reset();
		}

		constexpr unique_resource(const unique_resource&) = delete;
		constexpr unique_resource& operator = (const unique_resource&) = delete;

		constexpr unique_resource(unique_resource&& other) noexcept :
			m_resource{ std::exchange(other.m_resource, invalid_resource_value) }
		{}

		constexpr unique_resource& operator = (unique_resource&& rhs) noexcept
		{
			if (this != std::addressof(rhs))
			{
				this->reset();
				m_resource = std::exchange(rhs.m_resource, invalid_resource_value);
			}

			return *this;
		}

		constexpr void reset() noexcept
		{
			if (m_resource != invalid_resource_value)
			{
				deleter_type{}(m_resource);
				m_resource = invalid_resource_value;
			}
		}

		[[nodiscard]]
		constexpr bool is_valid() const noexcept
		{
			return m_resource != invalid_resource_value;
		}

		[[nodiscard]]
		constexpr explicit operator bool() const noexcept
		{
			return m_resource != invalid_resource_value;
		}

		constexpr void swap(unique_resource& other) noexcept
		{
			std::swap(m_resource, other.m_resource);
		}

		[[nodiscard]]
		constexpr explicit operator resource_type() const noexcept
		{
			return m_resource;
		}

		[[nodiscard]]
		constexpr resource_type get() const noexcept
		{
			return m_resource;
		}

		[[nodiscard]]
		constexpr const resource_type* get_address_of() const noexcept
		{
			return std::addressof(m_resource);
		}

		constexpr void take_ownership(resource_type resource) noexcept
		{
			this->reset();
			m_resource = resource;
		}

		[[nodiscard("Releasing ownership of resource.")]]
		constexpr resource_type release_ownership() noexcept
		{
			return std::exchange(m_resource, invalid_resource_value);
		}

	private:
		resource_type m_resource{ invalid_resource_value };

		static_assert(noexcept(deleter_type{}(invalid_resource_value)), "The unique_resource deleter_type operator () must be noexcept.");
	};
}