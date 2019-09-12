#include <iocp/IocpLoop.h>
#include <iocp/IocpServer.h>
#include <utils/logger.h>
#include <cassert>
#include <WS2tcpip.h>	// inet_pton
#include <Mswsock.h>

using meshy::DataSink;
using meshy::IocpServer;
using meshy::NativeSocket;
using meshy::WSAConnectionPtr;

IocpServer::IocpServer(DataSink* dataSink) : BasicServer<WSAConnectionPtr>(dataSink), m_completionPort(nullptr)
{
	IocpLoop::get();	// 初始化网络
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

IocpServer::ConnectionType IocpServer::accept(NativeSocket fd) {
	ConnectionType connection = find(fd);
	
	if (!connection) {
		PostAccept();
		return nullptr;
	}
	Iocp::OperationData& opData = connection->GetOperationReadData();
	if (opData.overlapped.InternalHigh <= 0)
	{
		remove(fd); // 断开了
		PostAccept();
		return nullptr;
	}
	DWORD sockaddrLength = sizeof(NativeSocketAddress);
	DWORD addressLength = sockaddrLength + 16;
	NativeSocketAddress *localSockaddr, *remoteSockaddr;
	int locallen, remotelen; 
	DWORD dataLen = sizeof(opData.buffer) - 2 * addressLength;
	IocpLoop::lpGetAcceptExSockaddrs(opData.buffer, dataLen, addressLength, addressLength, (sockaddr**)&localSockaddr, &locallen, (sockaddr**)&remoteSockaddr, &remotelen);
		
	connection->SetSocketAddress(*remoteSockaddr);
	auto remoteIp = remoteSockaddr->sin_addr.s_addr;

	TRACE_INFO("新建连接socket={}", connection->GetNativeSocket());
	if (ChangeIpCount(remoteIp, true) >= MAX_IP_COUNT) {
		remove(fd);
		PostAccept();
		return nullptr;
	}
	
	connection->SetDataSink(GetDataSink());	// 继承服务器的DataSink
	if (m_connectHandler)
		m_connectHandler(connection.get());
	connection->SetConnected(true);
	TRACE_DEBUG("Accept socket socket={}", fd);
	Iocp::OperationData& perIoData = Iocp::CreateOperationData(connection, m_completionPort);
	
	DWORD flags = 0;
	DWORD recvBytes = 0;
	WSARecv(fd, &perIoData.databuff, 1, &recvBytes, &flags, &perIoData.overlapped, nullptr);

	PostAccept();
	return connection;
}

void IocpServer::SetCompletionPort(HANDLE completionPort) {
	m_completionPort = completionPort;
}

void IocpServer::PostAccept()
{
	NativeSocket acceptfd = Socket::CreateNativeSocket();
	IocpServer::ConnectionType connection = std::make_shared<WSAConnection>(this, acceptfd);	// 创建一个tcp连接
	int32_t option = 1;
	connection->setsockopt(SOL_SOCKET, SO_REUSEADDR, option);
	Iocp::OperationData& operationReadData = connection->GetOperationReadData();
	
	operationReadData.operationType = Iocp::OperationType::Accept;
	NativeSocket listenfd = GetNativeSocket();

	DWORD addressLength = sizeof(NativeSocketAddress) + 16;
	//AcceptEx
	DWORD dwBytes = 0;
	DWORD dataLen = sizeof(operationReadData.buffer) - 2 * addressLength;
	BOOL r = IocpLoop::lpAcceptEx(listenfd, acceptfd, operationReadData.buffer, dataLen, addressLength, addressLength, &dwBytes, &operationReadData.overlapped);
	if (!r)
	{
		int errorno = WSAGetLastError();
		// https://msdn.microsoft.com/query/dev15.query?appId=Dev15IDEF1&l=ZH-CN&k=k(MSWSOCK%2FAcceptEx);k(AcceptEx);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
		// 文档说ERROR_IO_PENDING是成功的
		// If WSAGetLastError returns ERROR_IO_PENDING, then the operation was successfully initiated and is still in progress.
		if (ERROR_IO_PENDING != errorno) {
			TRACE_ERROR("AcceptEx failed with error: {}", WSAGetLastError());
			return;
		}
	}
	assert(!find(acceptfd));
	insert(acceptfd, connection);
	assert(find(acceptfd));
	TRACE_INFO("To Accept: socket={}", connection->GetNativeSocket());
}
