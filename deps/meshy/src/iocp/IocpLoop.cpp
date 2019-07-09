
#include "win32/net_win32.h"

#include <MSWSock.h>	// LPFN_ACCEPTEX WSAID_ACCEPTEX

#include <iocp/IocpLoop.h>
#include <iocp/IocpServer.h>
#include <utils/logger.h>
#include <DataSink.h>

using meshy::IocpLoop;
using meshy::IocpServer;
using meshy::NativeSocket;
using meshy::IocpStreamPtr;
using meshy::IocpStream;

LPFN_ACCEPTEX               IocpLoop::lpAcceptEx;	///< 函数指针
LPFN_GETACCEPTEXSOCKADDRS	IocpLoop::lpGetAcceptExSockaddrs;

IocpLoop& IocpLoop::get() {
	static IocpLoop iocpLoop;
	return iocpLoop;
}

void IocpLoop::AddServer(IocpServer* server) {
	assert(server);
	m_serverQueue.push(server);
	if (m_shutdown) {
		m_shutdown = false;
		assert(m_thread_group.empty());
		start();
	}	
}

bool IocpLoop::init() {
	SOCKET nSocket = Socket::CreateNativeSocket();
	if (INVALID_SOCKET == nSocket) {
		return false;
	}
	DWORD dwResult;
	int nResult;
	//获取微软SOCKET扩展函数指针
	if (!lpAcceptEx) {
		GUID g_GUIDAcceptEx = WSAID_ACCEPTEX;
		nResult = WSAIoctl(nSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &g_GUIDAcceptEx, sizeof(g_GUIDAcceptEx), &lpAcceptEx, sizeof(lpAcceptEx), &dwResult, NULL, NULL);
		if (SOCKET_ERROR == nResult) {
			//cout << "Get AcceptEx failed: " << WSAGetLastError() << endl;
			closesocket(nSocket);
			return false;
		}
	}
	if (!lpGetAcceptExSockaddrs) {
		GUID g_GUIDAcceptEx = WSAID_GETACCEPTEXSOCKADDRS;
		nResult = WSAIoctl(nSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &g_GUIDAcceptEx, sizeof(g_GUIDAcceptEx), &lpGetAcceptExSockaddrs, sizeof(lpGetAcceptExSockaddrs), &dwResult, NULL, NULL);
		if (SOCKET_ERROR == nResult) {
			//cout << "Get AcceptEx failed: " << WSAGetLastError() << endl;
			closesocket(nSocket);
			return false;
		}
	}
	/*if (!lpTransmitFile) {
		nResult = WSAIoctl(nSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &g_GUIDTransmitFile, sizeof(g_GUIDTransmitFile), &lpTransmitFile, sizeof(lpTransmitFile), &dwResult, NULL, NULL);
		if (SOCKET_ERROR == nResult) {
			//cout << "Get TransmitFile failed: " << WSAGetLastError() << endl;
			closesocket(nSocket);
			return false;
		}
	}*/
	closesocket(nSocket);
	return true;
}

IocpLoop::IocpLoop() {
	WindowsSocketInitializer::initialize();
	GetSystemInfo(&m_systemInfo);
	bool r = init();
	assert(r);
}

IocpLoop::~IocpLoop() {
	stop();
	CloseHandle(m_completionPort);
}

void IocpLoop::run() {
	
	// 工作线程
	size_t threadCont = m_systemInfo.dwNumberOfProcessors * 2;
	//threadCont = 1;
	for (size_t i = 0; i < threadCont; ++i) {
		auto workerFunc = std::bind(&IocpLoop::WorkThread, this);
		std::thread workerThread(workerFunc);
		workerThread.detach();
		m_thread_group.push_back(std::move(workerThread));
	}

	auto func = std::bind(&IocpLoop::IocpThread, this);
	std::thread iocpThread(func);
	iocpThread.detach();	// IO主线程
	m_thread_group.push_back(std::move(iocpThread));
	TRACE_DEBUG("线程数={}", m_thread_group.size());
}

void IocpLoop::IocpThread() {
	TRACE_DEBUG("Io ready, wait for new server ...");

	while (!m_shutdown)
	{
		IocpServer* server = nullptr;
		m_serverQueue.pop(server);	// 阻塞
		if (!server)continue;
		NativeSocket listenfd = server->GetNativeSocket();
		TRACE_DEBUG("Server:{} ready, wait for new conection ...", listenfd);
		server->SetCompletionPort(m_completionPort);
		CreateIoCompletionPort((HANDLE)listenfd, m_completionPort, listenfd, 0);
		{
			std::unique_lock<std::mutex> lock(m_serverMutex);
			m_servers.insert({ listenfd , server });
		}
		for (int i = 0; i < 5; ++i)	// 每个服务器创建5个等待
		{
			server->PostAccept();
		}
	}
}

void IocpLoop::WorkThread() {
	
	while (!m_shutdown) {
		DWORD bytesReceived = 0;
		ULONG_PTR key = 0;
		LPOVERLAPPED lpOverlapped = nullptr;
		BOOL result = GetQueuedCompletionStatus(m_completionPort, &bytesReceived, &key, &lpOverlapped, INFINITE);
		if (!result)
		{
			TRACE_ERROR("GetQueuedCompletionStatus errno={0},Error: {1}", errno, WSAGetLastError());
			if (ERROR_NETNAME_DELETED == GetLastError() && lpOverlapped)
			{
				Iocp::OperationData* perIoData = (Iocp::OperationData*)CONTAINING_RECORD(lpOverlapped, Iocp::OperationData, overlapped);
				WSAConnection *connection = dynamic_cast<WSAConnection *>(perIoData->stream);
				if (connection)
				{
					connection->server()->PostAccept();
				}
				perIoData->stream->disconnect();
			}
			continue;
		}
		if (!lpOverlapped)
		{
			return; // 关闭了
		}
		Iocp::OperationData* perIoData = (Iocp::OperationData*)CONTAINING_RECORD(lpOverlapped, Iocp::OperationData, overlapped);

		//TRACE_DEBUG("perIoData=%p,stream=%p,socket=%u, key=%u", perIoData, perIoData->stream, perIoData->stream->GetNativeSocket(), key);
		switch (perIoData->operationType)
		{
		case Iocp::OperationType::Accept:
		{
			IocpServer* server = nullptr;
			{
				std::unique_lock<std::mutex> lock(m_serverMutex);
				auto it = m_servers.find(key);
				if (it != m_servers.end())
					server = it->second;
			}
			if (server)
			{
				perIoData->databuff.buf[bytesReceived] = 0;
				char* buf = perIoData->databuff.buf;
				IocpServer::ConnectionType stream = server->accept(perIoData->stream->GetNativeSocket());		
				if(stream)
					enqueue(stream.get(), buf, bytesReceived);
				continue;
			}
			else {
				assert(false && "找不到服务器");
			}
		}
		break;
		case Iocp::OperationType::Read:
		{
			IocpStream* stream = perIoData->stream;
			assert(stream && stream->GetNativeSocket() == key);
			assert(perIoData == &stream->GetOperationReadData());
			if (stream->GetNativeSocket() != key)
			{
				TRACE_ERROR("{0}: socket={1}, key={2}", __FUNCTION__, stream->GetNativeSocket(), key);
				stream->disconnect();
				continue;
			}
			if (!bytesReceived)	// 网络断开了
			{
				stream->disconnect();
				continue;
			}
			perIoData->databuff.buf[bytesReceived] = 0;
			enqueue(stream, perIoData->databuff.buf, bytesReceived);
			Iocp::ResetOperationData(*perIoData);
			perIoData->operationType = Iocp::OperationType::Read;
			DWORD recvBytes = 0;
			DWORD flags = 0;
			Iocp::OperationData& operationReadData = stream->GetOperationReadData();
			WSARecv(stream->GetNativeSocket(), &operationReadData.databuff, 1, &recvBytes, &flags, &operationReadData.overlapped, nullptr);
		}
		break;
		default:
			break;
		}
	}
}

void IocpLoop::enqueue(IocpStream* stream, char const* buf, size_t nread) {
	TRACE_DEBUG("socket: {0} receive {1} Bytes by thread={2}.", stream->GetNativeSocket(), nread, std::this_thread::get_id());
	
	if (stream->GetDataSink())
	{
		stream->GetDataSink()->OnDataIndication(stream, buf, nread);
	}
}

void meshy::IocpLoop::stop()
{
	m_shutdown = true;
	for (size_t i = 0; i < m_systemInfo.dwNumberOfProcessors * 2; ++i) {
		PostQueuedCompletionStatus(m_completionPort, 0, 0, nullptr);
	}
	for (auto& th : m_thread_group) {
		try
		{
			if (std::this_thread::get_id() == th.get_id())
			{
				TRACE_ERROR("Cannot join self thread.");
				continue;
			}
			if(th.joinable())
				th.join();
		}
		catch (std::system_error const& e)
		{
			TRACE_ERROR("{0}:{1}", __FUNCTION__, e.what());
			continue;
		}
		catch (...)
		{
			continue;
		}
	}
	m_thread_group.clear();
}
