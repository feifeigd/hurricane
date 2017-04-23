#include <iocp/Iocp.h>
#include <utils/logger.h>
#include <iocp/IocpStream.h>

using meshy::Iocp;
using meshy::IocpStreamPtr;

HANDLE Iocp::GetCompletionPort() {
	HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (!completionPort)
	{
		TRACE_ERROR("CreateIoCompletionPort failed.Error:" + GetLastError());
		throw std::exception("CreateIoCompletionPort failed.");
	}
	return completionPort;
}

Iocp::OperationDataPtr Iocp::CreateOperationData(IocpStreamPtr stream, HANDLE completionPort) {
	Iocp::OperationData* perIoData = new Iocp::OperationData;
	perIoData->stream = stream.get();
	ResetOperationData(perIoData);
	perIoData->operationType = Iocp::OperationType::Read;
	// ¹ØÁªIOCP
	CreateIoCompletionPort((HANDLE)stream->GetNativeSocket(), completionPort, stream->GetNativeSocket(), 0);
	return Iocp::OperationDataPtr(perIoData);
}

void Iocp::ResetOperationData(OperationData* perIoData) {
	ZeroMemory(&perIoData->overlapped, sizeof(OVERLAPPED));
	perIoData->databuff.len = BUFSIZ;
	perIoData->databuff.buf = perIoData->buffer;
	perIoData->operationType = 0;
}
