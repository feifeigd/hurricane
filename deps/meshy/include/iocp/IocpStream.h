#pragma once

#include "../BasicStream.h"
#include "Iocp.h"

#include <memory>

namespace meshy {
	class IocpStream : public BasicStream{
		IocpStream(IocpStream const&) = delete;
	public:
		IocpStream(NativeSocket socket, NativeSocketAddress const& address);

		virtual size_t receive(char* buffer, size_t bufferSize, size_t& readSize)override;
		virtual size_t send(ByteArray const& byteArray)override;

		void SetOperationData(Iocp::OperationDataPtr operationData);
		Iocp::OperationDataPtr GetOperationData();

	private:
		Iocp::OperationDataPtr	m_operatinData;
		NativeSocketAddress		m_clientAddress;
	};

	typedef std::shared_ptr<IocpStream> IocpStreamPtr;
}
