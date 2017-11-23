
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

SOCKET CreateSocket() {
	SOCKET nSocket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == nSocket) {
		//cout << "WSASocket() failed: " << WSAGetLastError() << endl;
	}
	return nSocket;
}

IocpLoop& IocpLoop::get() {
	static IocpLoop iocpLoop;
	return iocpLoop;
}

void IocpLoop::AddServer(IocpServer* server) {
	assert(server);
	m_serverQueue.push(server);
	if (m_shutdown) {
		m_shutdown = false;
		start();
	}	
}

bool IocpLoop::init() {
	SOCKET nSocket = CreateSocket();
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

IocpLoop::IocpLoop() : m_shutdown(true), m_completionPort(Iocp::CreateCompletionPort()){
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
	threadCont = 1;
	for (size_t i = 0; i < threadCont; ++i) {
		auto workerFunc = std::bind(&IocpLoop::WorkThread, this);
		std::thread workerThread(workerFunc);
		workerThread.detach();
		m_thread_group.push_back(std::thread(workerFunc));
	}

	auto func = std::bind(&IocpLoop::IocpThread, this);
	std::thread iocpThread(func);
	iocpThread.detach();	// IO主线程
}

void IocpLoop::IocpThread() {
	TRACE_DEBUG("Io ready, wait for new server ...");

	while (!m_shutdown)
	{
		IocpServer* server = nullptr;
		m_serverQueue.pop(server);	// 阻塞
		if (!server)continue;
		NativeSocket listenfd = server->GetNativeSocket();
		TRACE_DEBUG("Server:%d ready, wait for new conection ...", listenfd);
		server->SetCompletionPort(m_completionPort);
		CreateIoCompletionPort((HANDLE)listenfd, m_completionPort, listenfd, 0);
		{
			std::unique_lock<std::mutex> lock(m_serverMutex);
			m_servers.insert({ listenfd , server });
		}
		for (int i = 0; i < 1; ++i)	// 每个服务器创建5个等待
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
			TRACE_ERROR("GetQueuedCompletionStatus Error: " + GetLastError());
			
			if (lpOverlapped)
			{

			}
			continue;
		}
		if (!lpOverlapped)
		{
			return; // 关闭了
		}
		Iocp::OperationData* perIoData = (Iocp::OperationData*)CONTAINING_RECORD(lpOverlapped, Iocp::OperationData, overlapped);

		TRACE_DEBUG("perIoData=%p,stream=%p,socket=%u, key=%u", perIoData, perIoData->stream, perIoData->stream->GetNativeSocket(), key);
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
				IocpServer::ConnectionType stream = server->accept(perIoData->stream->GetNativeSocket());				
				continue;
			}
		}
		break;
		case Iocp::OperationType::Read:
		{
			IocpStream* stream = perIoData->stream;
			TRACE_DEBUG("Read:stream socket=%u, key=%u", stream->GetNativeSocket(), key);
			//assert(stream && stream->GetNativeSocket() == key);
			assert(perIoData == &stream->GetOperationReadData());
			if (stream->GetNativeSocket() != key)
			{
				continue;
			}
			if (!bytesReceived)	// 网络断开了
			{
				stream->disconnect();
				continue;
			}
			perIoData->databuff.buf[bytesReceived] = 0;
			enqueue(stream, perIoData->databuff.buf, bytesReceived);
			Iocp::ResetOperationData(perIoData);
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
}
