#include <iocp/WSAConnection.h>

using meshy::NativeSocket;
using meshy::NativeSocketAddress;
using meshy::WSAConnection;
using meshy::WSABasicServer;

WSAConnection::WSAConnection(WSABasicServer* server, NativeSocket socket) : IocpStream(socket), m_server(server)
{
	assert(m_server);
}

WSAConnection::~WSAConnection() {
	if (m_server)
	{
		m_server->ChangeIpCount(m_nativeSocketAddress.sin_addr.s_addr, false);
	}
}

void meshy::WSAConnection::disconnect()
{
	__super::disconnect();
	if (m_server)
	{
		m_server->remove(GetNativeSocket());
	}
}
