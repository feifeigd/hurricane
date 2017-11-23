#include <iocp/Iocp.h>
#include <utils/logger.h>
#include <iocp/IocpStream.h>
#include <iocp/IocpLoop.h>

using meshy::Iocp;
using meshy::IocpLoop;
using meshy::IocpStreamPtr;

HANDLE Iocp::CreateCompletionPort() {
	HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (!completionPort)
	{
		TRACE_ERROR("CreateIoCompletionPort failed.Error:" + GetLastError());
		throw std::exception("CreateIoCompletionPort failed.");
	}
	return completionPort;
}

Iocp::OperationData& Iocp::CreateOperationData(IocpStreamPtr stream, HANDLE completionPort) {
	Iocp::OperationData& perIoData = stream->GetOperationReadData();

	IocpLoop::get().enqueue(perIoData.stream, perIoData.databuff.buf, perIoData.overlapped.InternalHigh);

	ResetOperationData(perIoData);
	perIoData.operationType = Iocp::OperationType::Read;
	// 关联IOCP
	NativeSocket socket = stream->GetNativeSocket();
	CreateIoCompletionPort((HANDLE)socket, completionPort, socket, 0);
	return perIoData;
}

void Iocp::ResetOperationData(OperationData& perIoData) {
	ZeroMemory(&perIoData.overlapped, sizeof(OVERLAPPED));
	perIoData.databuff.len = sizeof perIoData.buffer;
	perIoData.databuff.buf = perIoData.buffer;
	perIoData.operationType = 0;
}
