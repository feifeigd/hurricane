#pragma once

#include "../BasicStream.h"
#include "Iocp.h"

#include <memory>

namespace meshy {
	class IocpStream : public BasicStream{
		IocpStream(IocpStream const&) = delete;
	public:
		IocpStream(NativeSocket socket, SocketAddress const& address = { });

		virtual size_t receive(char* buffer, size_t bufferSize, size_t& readSize)override;
		virtual size_t send(ByteArray const& byteArray)override;

		Iocp::OperationData& GetOperationReadData() { return m_operatinReadData; };
		
	private:
		Iocp::OperationData	m_operatinReadData;
	};

	typedef std::shared_ptr<IocpStream> IocpStreamPtr;
}
