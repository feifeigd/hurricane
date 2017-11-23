
#include <iocp/IocpStream.h>
#include <ByteArray.h>
#include <utils/logger.h>

using meshy::BasicStream;
using meshy::ByteArray;
using meshy::Iocp;
using meshy::IocpStream;
using meshy::NativeSocket;
using meshy::NativeSocketAddress;

IocpStream::IocpStream(NativeSocket socket, NativeSocketAddress const& address) : BasicStream(socket, address)
{
	Iocp::ResetOperationData(m_operatinReadData);
	m_operatinReadData.stream = this;
}

size_t IocpStream::receive(char* buffer, size_t bufferSize, size_t& readSize) {
	return 0;	// 在IOCP里面实现了，不用自己去接收
}

size_t IocpStream::send(ByteArray const& byteArray) {
	size_t data_size = byteArray.size();
	int n = (int)data_size;
	NativeSocket fd = GetNativeSocket();
	char const* buf = byteArray.data();
	while (n > 0)
	{
		int nwrite = ::send(fd, buf + data_size - n, n, 0);
		if (nwrite < n)
		{
			if (-1 == nwrite && EAGAIN != errno)
			{
				TRACE_ERROR("send error");
			}
			break;
		}
		
		n -= nwrite;
	}
	TRACE_DEBUG("%s %u Bytes.", __FUNCTION__, data_size - n);
	return data_size - n;
}
