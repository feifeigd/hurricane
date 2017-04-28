#pragma once

#include "../meshy.h"
#include "http/HttpConnection.h"

namespace meshy {

	class HttpServer {
	public:
		typedef std::function<void(HttpConnection* connection)>	ConnectionHandler;

		void listen(std::string const& host, uint16_t port, int backlog = 20);
		void OnConnection(ConnectionHandler handler);

	private:
		TcpServer			m_server;
		ConnectionHandler	m_connectionHandler;
	};
}