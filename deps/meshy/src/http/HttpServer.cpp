#include <http/HttpServer.h>

using meshy::DataSink;
using meshy::HttpServer;

HttpServer::HttpServer(DataSink* dataSink) : m_server(dataSink){

}

void HttpServer::listen(std::string const& host, uint16_t port, int backlog) {
	m_server.listen(host, port, backlog);
	m_server.OnConnectIndication([this](IStream* stream) {
		TcpConnection* connection = dynamic_cast<TcpConnection*>(stream);
		std::shared_ptr<HttpConnection> httpConnection = std::make_shared<HttpConnection>(connection);
		if (m_connectionHandler)m_connectionHandler(httpConnection.get());
	});
}

void HttpServer::OnConnection(ConnectionHandler handler) {
	m_connectionHandler = handler;
}
