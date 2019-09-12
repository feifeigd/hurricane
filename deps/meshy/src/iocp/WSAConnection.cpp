#include <iocp/WSAConnection.h>
#include <iocp/IocpServer.h>
using meshy::IocpServer;
using meshy::NativeSocket;
using meshy::WSAConnection;
//using meshy::WSABasicServer;

WSAConnection::WSAConnection(IocpServer* server, NativeSocket socket) : IocpStream(socket), m_server(server)
{
	assert(m_server);
}

WSAConnection::~WSAConnection() {
	if (m_server)
	{
		m_server->ChangeIpCount(m_socketAddress.nativeSocketAddress.sin_addr.s_addr, false);
	}
}

void WSAConnection::disconnect()
{
	__super::disconnect();
	if (m_server)
	{
		m_server->remove(GetNativeSocket());
	}
}

IocpServer* WSAConnection::server() const
{
	return m_server;
}
