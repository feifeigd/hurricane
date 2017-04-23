#include <socket.h>

using meshy::Socket;
using meshy::NativeSocket;
#pragma comment(lib, "Ws2_32.lib")	// closesocket

Socket::Socket(NativeSocket nativeSocket)
	: m_nativeSocket(nativeSocket)
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

