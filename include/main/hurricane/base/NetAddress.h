#pragma once

#include <string>

namespace hurricane {
	namespace base {
		class NetAddress {
		public:
			NetAddress(std::string const& host, uint16_t port) : m_host(host), m_port(port){}

			std::string const& host()const { return m_host; }
			void host(std::string const& host) { m_host = host; }

			uint16_t port()const { return m_port; }
			void port(uint16_t port) { m_port = port; }
		private:
			std::string	m_host;
			uint16_t	m_port;
		};
	}
}