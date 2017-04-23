#include <iocp/IocpLoop.h>
#include <iocp/IocpServer.h>
#include <utils/logger.h>
#include <cassert>
#include <WS2tcpip.h>	// inet_pton

using meshy::IocpServer;
using meshy::NativeSocket;
using meshy::WSAConnectionPtr;

IocpServer::IocpServer() : m_completionPort(nullptr)
{
	WindowsSocketInitializer::initialize();
}

int32_t IocpServer::bind(std::string const& host, uint16_t port) {
	NativeSocket listenfd = socket(AF_INET, SOCK_STREAM, 0);
	int32_t option = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));
	NativeSocketAddress srvAddr;
	inet_pton(AF_INET, host.c_str(), &srvAddr.sin_addr);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(port);
	int32_t errorCode = ::bind(listenfd, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == errorCode)
	{
		TRACE_ERROR("Bind failed. Error: " + GetLastError());
		assert(false);
		return errorCode;
	}
	SetNativeSocket(listenfd);
	return 0;
}

int32_t IocpServer::listen(std::string const& host, uint16_t port, int backlog) {
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
	NativeSocket acceptSocket = ::accept(GetNativeSocket(), (SOCKADDR*)&saRemote, &remoteLen);
	if (SOCKET_ERROR == acceptSocket)
	{
		TRACE_ERROR("Accept Socket Error: " + GetLastError());
		throw std::exception("Accept Socket Error: ");
	}
	WSAConnectionPtr connection = std::make_shared<WSAConnection>(acceptSocket, saRemote);
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
