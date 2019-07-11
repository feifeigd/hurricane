#include <socket.h>
#include <utils/logger.h>

using meshy::Socket;
using meshy::NativeSocket;
using meshy::NativeSocketAddress;

#ifdef OS_WIN32
#pragma comment(lib, "Ws2_32.lib")	// closesocket
#endif // OS_WIN32

NativeSocket Socket::CreateNativeSocket() {
	NativeSocket fd = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == fd)
	{
#ifdef OS_WIN32
		TRACE_ERROR("Create socket failed! errorno={0}", WSAGetLastError());
#else
		TRACE_ERROR("Create socket failed! errorno={0}", errno);
#endif
		exit(1);
	}
	return fd;
}

Socket::Socket(NativeSocket nativeSocket, NativeSocketAddress const& address)
	: m_nativeSocket(nativeSocket), m_nativeSocketAddress(address)
{
	
}

Socket::~Socket() {
	close();
}

void Socket::close() {
	TRACE_ERROR("关闭连接socket={}", m_nativeSocket);
#ifdef OS_WIN32
	closesocket(m_nativeSocket);
#else
	close(m_nativeSocket);
#endif // OS_WIN32
	m_nativeSocket = -1;
}

NativeSocket Socket::GetNativeSocket()const {
	return m_nativeSocket;
}

void Socket::SetNativeSocket(NativeSocket nativeSocket) {
	m_nativeSocket = nativeSocket;
}

NativeSocketAddress const& Socket::GetNativeSocketAddress()const {
	return m_nativeSocketAddress;
}
