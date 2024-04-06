export module jp3.net:network;

import "jp3.net.includes.hpp";
import jp3.core;

namespace jp3::net
{
#ifdef JP3_NET_BUILD_WINDOWS

	class network final : public jp3::initializable
	{
	public:
		inline network() noexcept
		{
			WSADATA wsaData{};

			m_initialization_error_code = WSAStartup(WINSOCK_VERSION, &wsaData);

			if(m_initialization_error_code == 0)
				this->set_initialization_state(true);
		}

		inline ~network() noexcept
		{
			if (this->is_initialized())
				std::ignore = WSACleanup();
		}

		network(const network&) = delete;
		network& operator = (const network&) = delete;

		network(network&&) = delete;
		network& operator = (network&&) = delete;

		inline int get_initialization_error_code() const noexcept
		{
			return m_initialization_error_code;
		}

	private:
		int m_initialization_error_code{ 0 };
	};

#else

	class network final : public jp3::initializable
	{
	public:
		inline network() noexcept :
			jp3::initializable{ true }
		{
		}

		inline ~network() noexcept = default;

		network(const network&) = delete;
		network& operator = (const network&) = delete;

		network(network&&) = delete;
		network& operator = (network&&) = delete;

		inline int get_initialization_error_code() const noexcept
		{
			return m_initialization_error_code;
		}

	private:
		int m_initialization_error_code{ 0 };
	};

#endif//JP3_NET_BUILD_WINDOWS

	inline const network* get_network() noexcept
	{
		static network s_network{};
		return std::addressof(s_network);
	}

	export [[nodiscard]]
	inline bool initialize_network() noexcept
	{
		return get_network()->is_initialized();
	}

	export [[nodiscard]]
	inline int get_network_initialization_error_code() noexcept
	{
		return get_network()->get_initialization_error_code();
	}
}