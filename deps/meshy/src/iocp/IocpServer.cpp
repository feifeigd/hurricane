#include <iocp/IocpLoop.h>
#include <iocp/IocpServer.h>
#include <utils/logger.h>
#include <cassert>
#include <WS2tcpip.h>	// inet_pton

using meshy::DataSink;
using meshy::IocpServer;
using meshy::NativeSocket;
using meshy::WSAConnectionPtr;

IocpServer::IocpServer(DataSink* dataSink) : BasicServer<WSAConnectionPtr>(dataSink), m_completionPort(nullptr)
{
	WindowsSocketInitializer::initialize();
}


int32_t IocpServer::listen(std::string const& host, uint16_t port, int backlog) {
	bind(host, port);

	NativeSocket listenfd = GetNativeSocket();
	int32_t errorCode = ::listen(listenfd, backlog);
	if (SOCKET_ERROR == errorCode)
	{
		TRACE_ERROR("Listen socket failed!");
		assert(false);
		return errorCode;
	}
	IocpLoop::get().AddServer(this);
	return 0;
}

WSAConnectionPtr IocpServer::accept() {
	NativeSocketAddress saRemote;
	int remoteLen = sizeof(saRemote);
	NativeSocket acceptSocket = ::accept(GetNativeSocket(), (sockaddr*)&saRemote, &remoteLen);
	if (SOCKET_ERROR == acceptSocket)
	{
		TRACE_ERROR("Accept Socket Error: " + GetLastError());
		throw std::exception("Accept Socket Error: ");
	}
	WSAConnectionPtr connection = std::make_shared<WSAConnection>(acceptSocket, saRemote);
	connection->SetDataSink(GetDataSink());
	if (m_connectHandler)m_connectHandler(connection.get());
	char buf[250];
	sprintf(buf, "Accept socket socket=%d", acceptSocket);
	TRACE_DEBUG(buf);
	Iocp::OperationDataPtr perIoData = Iocp::CreateOperationData(connection, m_completionPort);
	connection->SetOperationData(perIoData);

	DWORD flags = 0;
	DWORD recvBytes = 0;
	WSARecv(acceptSocket, &perIoData->databuff, 1, &recvBytes, &flags, &perIoData->overlapped, nullptr);

	return connection;
}

void IocpServer::SetCompletionPort(HANDLE completionPort) {
	m_completionPort = completionPort;
}
