
#include <iocp/IocpLoop.h>
#include <iocp/IocpServer.h>
#include <utils/logger.h>
#include <DataSink.h>

#include <thread>

using meshy::IocpLoop;
using meshy::IocpServer;
using meshy::NativeSocket;
using meshy::IocpStreamPtr;

IocpLoop& IocpLoop::get() {
	static IocpLoop iocpLoop;
	return iocpLoop;
}

void IocpLoop::AddServer(IocpServer* server) {
	m_serverQueue.push(server);
}

IocpLoop::IocpLoop() : m_shutdown(false){
	GetSystemInfo(&m_systemInfo);
}

void IocpLoop::run() {
	auto func = std::bind(&IocpLoop::IocpThread, this);
	std::thread iocpThread(func);
	iocpThread.detach();
}

void IocpLoop::IocpThread() {
	TRACE_DEBUG("Server ready, wait for new connection ...");
	HANDLE completionPort = Iocp::GetCompletionPort();

	while (!m_shutdown)
	{
		IocpServer* server = nullptr;
		m_serverQueue.pop(server);	// ×èÈû
		if (!server)continue;
		server->SetCompletionPort(completionPort);
		NativeSocket listenfd = server->GetNativeSocket();
		for (size_t i = 0; i < m_systemInfo.dwNumberOfProcessors * 2; ++i) {
			auto workerFunc = std::bind(&IocpLoop::WorkThread, this, listenfd, completionPort);
			std::thread workerThread(workerFunc);
			workerThread.detach();
		}
		auto connectionFunc = std::bind(&IocpLoop::IocpConnectionThread, this, server);
		std::thread acceptThread(connectionFunc);
		acceptThread.detach();
	}
}

void IocpLoop::IocpConnectionThread(IocpServer* server) {
	while (!m_shutdown) {
		TRACE_INFO("To Accept");
		WSAConnectionPtr connection = server->accept();
		if (connection)m_streams[connection->GetNativeSocket()] = connection;
	}
}

void IocpLoop::WorkThread(NativeSocket listenfd, HANDLE completionPort) {

	while (!m_shutdown) {
		DWORD bytesReceived = 0;
		ULONG_PTR key = 0;
		LPOVERLAPPED lpOverlapped = nullptr;
		BOOL result = GetQueuedCompletionStatus(completionPort, &bytesReceived, &key, &lpOverlapped, INFINITE);
		if (!result)
		{
			TRACE_ERROR("GetQueuedCompletionStatus Error: " + GetLastError());
			if (lpOverlapped)
			{

			}
			continue;
		}

		Iocp::OperationData* perIoData = (Iocp::OperationData*)CONTAINING_RECORD(lpOverlapped, Iocp::OperationData, overlapped);
		if (!bytesReceived)
		{
			perIoData->stream = nullptr;
			delete perIoData;
			continue;
		}

		IocpStreamPtr stream = m_streams[perIoData->stream->GetNativeSocket()];
		enqueue(stream, perIoData->databuff.buf, bytesReceived);
		Iocp::ResetOperationData(perIoData);

		DWORD recvBytes = 0;
		DWORD flags = 0;		 
		WSARecv(stream->GetNativeSocket(), &stream->GetOperationData()->databuff, 1, &recvBytes, &flags, &stream->GetOperationData()->overlapped, nullptr);
	}
}


void IocpLoop::enqueue(IocpStreamPtr stream, char const* buf, size_t nread) {
	TRACE_DEBUG(__FUNCTION__);
	if (stream->GetDataSink())
	{
		TRACE_DEBUG("enqueue, datasink registered.");
		stream->GetDataSink()->OnDataIndication(stream.get(), buf, nread);
	}
}
