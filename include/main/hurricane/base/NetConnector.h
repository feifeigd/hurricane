#pragma once

#include "NetAddress.h"
#include <meshy.h>
#include <memory>

class NetConnector {
public:
	NetConnector(hurricane::base::NetAddress const& host) : m_host(host){}

	hurricane::base::NetAddress const& host()const { return m_host; }
	void host(hurricane::base::NetAddress const& host) { m_host = host; }

	void connect();
	size_t SendAndReceive(char const* buffer, size_t size, char* resultBuffer, size_t resultSize);

private:
	hurricane::base::NetAddress			m_host;
	std::shared_ptr<meshy::TcpClient>	m_client;
};
