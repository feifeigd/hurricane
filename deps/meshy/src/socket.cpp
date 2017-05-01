#include <socket.h>

using meshy::Socket;
using meshy::NativeSocket;
using meshy::NativeSocketAddress;

#ifdef OS_WIN32
#pragma comment(lib, "Ws2_32.lib")	// closesocket
#endif // OS_WIN32


Socket::Socket(NativeSocket nativeSocket, NativeSocketAddress const& address)
	: m_nativeSocket(nativeSocket), m_nativeSocketAddress(address)
{
	
}

Socket::~Socket() {
#ifdef OS_WIN32
	closesocket(m_nativeSocket);
#else
	close(m_nativeSocket);
#endif // OS_WIN32
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
