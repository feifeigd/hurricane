#pragma once

#include "../meshy.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace meshy {
	
	class HttpConnection
	{
	public:
		typedef std::function<void(HttpRequest const& request)>	RequestHandler;
		typedef std::function<void(std::string const& data)>	DataHandler;

		HttpConnection(TcpConnection* connection);

		void HandleData(char const* buffer, size_t size);
		void OnData(DataHandler handler);

		void SendResponse(HttpResponse const& response);

	private:
		TcpConnection*	m_connection;
		HttpRequest		m_request;
		HttpResponse	m_response;
		RequestHandler	m_requestHandler;
		DataHandler		m_dataHandler;
	};
}