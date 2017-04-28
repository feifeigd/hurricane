#include <http/HttpConnection.h>
#include <iostream>

using meshy::ByteArray;
using meshy::HttpConnection;
using meshy::HttpResponse;
using meshy::TcpConnection;
using namespace std;

HttpConnection::HttpConnection(TcpConnection* connection) : m_connection(connection)
{
}

void HttpConnection::HandleData(char const* buffer, size_t size) {
	cout << buffer << "\n"
		<< size << endl;
	string requestText(buffer, size);
	m_request.ParseStdString(requestText);
	if (m_requestHandler)m_requestHandler(m_request);
	if (m_dataHandler && m_request.content().size())m_dataHandler(m_request.content());
}

void HttpConnection::OnData(DataHandler handler) {
	m_dataHandler = handler;
}

void HttpConnection::SendResponse(HttpResponse const& response) {
	m_response = response;
	m_connection->send(m_response.ToStdString());
}
